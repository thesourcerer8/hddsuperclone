// Copyright (C) 2015-2022 Scott Dwyer.
// You may use/distribute/modify this freely, under the terms of
// the GNU General Public License version 2 or later version.
// This software is distributed WITHOUT ANY WARRANTY.


#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/vmalloc.h>
#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/hdreg.h>
#include <linux/delay.h>
#include <linux/debugfs.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <linux/blk-mq.h>
#include <scsi/sg.h>
#include <linux/bsg.h>

//This defines are available in blkdev.h from kernel 4.17 (vanilla).
#ifndef SECTOR_SHIFT
#define SECTOR_SHIFT 9
#endif
#ifndef SECTOR_SIZE
#define SECTOR_SIZE (1 << SECTOR_SHIFT)
#endif

#ifndef VM_RESERVED
#define VM_RESERVED (VM_DONTEXPAND | VM_DONTDUMP)
#endif

#include <linux/version.h>

#define KERNEL_SECTOR_SIZE (SECTOR_SIZE)
#define DRIVER_CONTROL_BUFFER_SIZE 131072    // make sure to adjust with page power
#define DRIVER_TRANSFER_BUFFER_SIZE 4194304   // make sure to adjust with page power
#define DRIVER_MAIN_DATA_BUFFER_SIZE 4194304   // make sure to adjust with page power
#define USER_DATA_BUFFER_SIZE 65536

#define IOCTL_CMD_HDDSC 0xa5a5a5a5
#define START_DRIVE_COMMAND 123
#define STOP_DRIVE_COMMAND 321
#define START_FILE_COMMAND 456
#define STOP_FILE_COMMAND 654
#define ACTIVATE_DRIVE_COMMAND 555
#define READ_MEMORY_DRIVE_COMMAND 777

#define MAIN_DRIVER_IOCTL_NAME "hddscbdc"
#define MAIN_DRIVER_MMAP_NAME "hddscmap_m"
#define MAIN_DRIVER_MMAPTB_NAME "hddscmap_tb"
#define MAIN_DRIVER_MMAPMDB_NAME "hddscmap_mdb"

#define CTRL_STOP_SIGNAL 0
#define CTRL_DATA_REQUEST 1
#define CTRL_DATA_READY 2
#define CTRL_KSECTOR_START 3
#define CTRL_KSECTOR_COUNT 4
#define CTRL_KSECTORS_REQUESTED 5
#define CTRL_DATA_RETURN_SIZE 6
#define CTRL_RESET_READ_TIMER 7
#define CTRL_DATA_DRIVE_ACTIVE 8
#define CTRL_DATA_VALID 9
#define CTRL_RESET_REQUEST_TIMER 10
#define CTRL_ACK_REQUEST 11
#define CTRL_READ_TIMEOUT 12
#define CTRL_REQUEST_TIMEOUT 13
#define CTRL_RETURN_NO_DATA_FAST 14
#define CTRL_NO_DRIVER_SLEEP 15
#define CTRL_ACK_TIMEOUT 16

#define SCSI_INQUIRY 0X12
#define SCSI_READ10 0X28
#define SCSI_READ16 0X88
#define SCSI_READCAPACITY10 0X25
#define SCSI_READCAPACITY16 0X9E
#define SCSI_READCAPACITY16SERVICEACTION 0X10
#define SCSI_WRITE10 0x2A
#define SCSI_WRITE16 0x8A

#define LOCAL_DEBUG 0
#define LOCAL_REPORT()                             \
do                                                 \
{                                                  \
  if (LOCAL_DEBUG) {       \
  printk(KERN_NOTICE "At %s:%d\n", __FILE__, __LINE__);       \
  }                                                \
} while (0)

#define DRIVER_VERSION "2.6_20211126"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Driver for HDDSuperClone");
MODULE_AUTHOR("Scott Dwyer");
MODULE_VERSION(DRIVER_VERSION);

static int data_major_num = 0;

static char filename_ioctl[32] = MAIN_DRIVER_IOCTL_NAME;
module_param_string(ioctl, filename_ioctl, 32, 0);

static char filename_mmap_m[32] = MAIN_DRIVER_MMAP_NAME;
module_param_string(mmap_m, filename_mmap_m, 32, 0);

static char filename_mmap_tb[32] = MAIN_DRIVER_MMAPTB_NAME;
module_param_string(mmap_tb, filename_mmap_tb, 32, 0);

static char filename_mmap_mdb[32] = MAIN_DRIVER_MMAPMDB_NAME;
module_param_string(mmap_mdb, filename_mmap_mdb, 32, 0);

static struct lock_class_key super_bio_compl_lkclass;

// TODO always make sure this matches the structure in the program!
/*
struct control_data
{
  long long command;
  long long logical_block_size;
  long long physical_block_size;
  long long total_logical_sectors;
  long long chs_heads;
  long long chs_sectors;
  long long chs_cylinders;
  long long return_zeros_on_error;
  long long io_scsi_only;
  unsigned long long read_timeout;
  unsigned long long request_timeout;
  unsigned long long ack_timeout;
  unsigned long long process_id;
  char name[16];
  char *buffer;
};
*/
struct __attribute__((packed)) control_data
{
  int64_t command;
  int64_t  logical_block_size;
  int64_t  physical_block_size;
  int64_t  total_logical_sectors;
  int64_t  chs_heads;
  int64_t  chs_sectors;
  int64_t  chs_cylinders;
  int64_t  return_zeros_on_error;
  int64_t  io_scsi_only;
  uint64_t read_timeout;
  uint64_t request_timeout;
  uint64_t ack_timeout;
  uint64_t process_id;
  char name[16];
  char buffer[USER_DATA_BUFFER_SIZE];
};



static unsigned char *main_buffer = NULL;
static unsigned char *transfer_buffer = NULL;
static unsigned char *main_data_buffer = NULL;
static unsigned char *block_io_buffer = NULL;


static int stop_signal = 0;
static int data_drive_active = 0;
static int lockup_detected = 0;
static long long request_number = 0;
static long long read_number = 0;
static long long return_zeros_on_error = 0;
static long long io_scsi_only = 0;
static unsigned long long process_id;
static int device_is_open = 0;
static int operation_in_progress = 0;
static long long current_offset = 0;
static int sg_version_num = 40000;
static unsigned int working_queue = 0;
static unsigned int request_queue = 0;
static int queue_count = 0;



static struct request_queue *data_queue;

typedef struct sblkdev_cmd_s
{
    //nothing
} sblkdev_cmd_t;

static struct data_device_structure
{
  unsigned long long size;
  unsigned long long sectors;
  spinlock_t lock;
  unsigned long long block_size;
  long long chs_sectors;
  long long chs_heads;
  long long chs_cylinders;
  char device_name[16];
  #if LINUX_VERSION_CODE >= KERNEL_VERSION(5,0,0)
  struct blk_mq_tag_set tag_set;
  #endif
  struct gendisk *gd;
} data_device;



static unsigned long long read_ctrl_data(int item)
{
  unsigned long long value = 0;
  if (main_buffer)
  {
    memcpy(&value, main_buffer + (item * 8), 8);
  }
  return value;
}

static void write_ctrl_data(int item, unsigned long long value)
{
  if (main_buffer)
  {
    memcpy(main_buffer + (item * 8), &value, 8);
  }
}



static int check_io_error_bitmap(unsigned long long sect, unsigned long long nsect)
{
  int i;
  char c;

  // sanity check
  if (sect + nsect > DRIVER_TRANSFER_BUFFER_SIZE / KERNEL_SECTOR_SIZE)
  {
    return -1;
  }
  for (i = 0; i < nsect; i++)
  {
    memcpy(&c, main_buffer + PAGE_SIZE + sect + i, 1);
    if (c)
    {
      return 1;
    }
  }
  return 0;
}



static void next_queue(void)
{
  working_queue++;
  if (working_queue > 65535)
  {
    working_queue = 0;
  }
  queue_count--;
  if (queue_count < 0)
  {
    queue_count = 0;
    printk(KERN_NOTICE "hddsuperdrive: internal error, queue count less than 0\n");
  }
}



static int wait_for_queue(const int current_queue)
{
  #if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
  struct timeval tv1, tv2;
  #else
  struct timespec64 tv1, tv2;
  #endif
  long long elapsed_usec = 0;
  int queue_limit = 2;
  request_queue++;
  if (request_queue > 65535)
  {
    request_queue = 0;
  }
  queue_count++;
  if (queue_count > queue_limit)
  {
    printk(KERN_NOTICE "hddsuperdrive: too many requests, queue full\n");
    return -EBUSY;
  }
  //printk(KERN_INFO "hddsc wait_for_queue %d %d %d\n", current_queue, working_queue, queue_count);    //debug
  if (current_queue != working_queue)
  {
    printk(KERN_INFO "hddsuperdrive: queue wait %d %d %d\n", current_queue, working_queue, queue_count);
    #if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
    do_gettimeofday(&tv1);
    #else
    ktime_get_real_ts64(&tv1);
    #endif
    while (current_queue != working_queue)
    {
      usleep_range(1, 1);
      #if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
      do_gettimeofday(&tv2);
      elapsed_usec = ((long long)tv2.tv_usec + (1000000 * (long long)tv2.tv_sec)) - ((long long)tv1.tv_usec + (1000000 * (long long)tv1.tv_sec));
      #else
      ktime_get_real_ts64(&tv2);
      elapsed_usec = (((long long)tv2.tv_nsec / 1000) + (1000000 * (long long)tv2.tv_sec)) - (((long long)tv1.tv_nsec / 1000) + (1000000 * (long long)tv1.tv_sec));
      #endif
      if (elapsed_usec > read_ctrl_data(CTRL_REQUEST_TIMEOUT) )
      {
        printk(KERN_NOTICE "hddsuperdrive: timeout waiting for queue time %lld\n", elapsed_usec);
        return -ETIME;
      }
    }
  }
  return 0;
}



static int data_transfer(struct data_device_structure *dev, sector_t sect, unsigned long long nsect, unsigned long long tsect, char *buffer, int write, int blockio)
{
  long long elapsed_usec = 0;
  unsigned long long offset = sect * KERNEL_SECTOR_SIZE;
  unsigned long long nbytes = nsect * KERNEL_SECTOR_SIZE;

  #if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
  struct timeval tv1, tv2;
  #else
  struct timespec64 tv1, tv2;
  #endif

  //printk(KERN_INFO "hddsuperdrive: request %lld buffer sect %lld count %lld total %lld active %d stop %d\n", request_number, (unsigned long long)sect, nsect, tsect, data_drive_active, stop_signal);    //debug

  if (io_scsi_only && !blockio)
  {
    memset(buffer, 0, nbytes);
    return 0;
  }

  if (!data_drive_active)
  {
    if (blockio)
    {
      memset(block_io_buffer, 0, nbytes);
      if (copy_to_user(buffer, block_io_buffer, nbytes))
      {
        printk (KERN_WARNING "hddsuperdrive: failed to copy user data\n");
      }
    }
    else
    {
      memset(buffer, 0, nbytes);
    }
    return 0;
  }
  if (stop_signal || read_ctrl_data(CTRL_STOP_SIGNAL))
  {
    if (blockio)
    {
      memset(block_io_buffer, 0, nbytes);
      if (copy_to_user(buffer, block_io_buffer, nbytes))
      {
        printk (KERN_WARNING "hddsuperdrive: failed to copy user data\n");
      }
    }
    else
    {
      memset(buffer, 0, nbytes);
    }
    return -EIO;
  }
  if (lockup_detected && !read_ctrl_data(CTRL_RESET_READ_TIMER))
  {
    if (blockio)
    {
      memset(block_io_buffer, 0, nbytes);
      if (copy_to_user(buffer, block_io_buffer, nbytes))
      {
        printk (KERN_WARNING "hddsuperdrive: failed to copy user data\n");
      }
    }
    else
    {
      memset(buffer, 0, nbytes);
    }
    return -EAGAIN;
  }
  if ((offset + nbytes) > dev->size)
  {
    printk (KERN_NOTICE "hddsuperdrive: Beyond end of device (%lld %lld)\n", offset, nbytes);
    return -EINVAL;
  }
  if (write)
  {
    printk(KERN_NOTICE "hddsuperdrive: Writing not allowed sect %lld count %lld\n", (unsigned long long)sect, nsect);
    return -EROFS;
  }

  // check if the chunk is in the current buffer
  if (read_ctrl_data(CTRL_DATA_VALID) && sect >= read_ctrl_data(CTRL_KSECTOR_START) && (sect + nsect) <= (read_ctrl_data(CTRL_KSECTOR_START) + read_ctrl_data(CTRL_KSECTOR_COUNT)) )
  {
    // if the mode is set for fast return to get domain, then just return without any other processing to speed things up
    if (read_ctrl_data(CTRL_RETURN_NO_DATA_FAST))
    {
      return 0;
    }

    if (!check_io_error_bitmap(sect - read_ctrl_data(CTRL_KSECTOR_START), nsect))
    {
      //printk(KERN_INFO "hddsuperdrive: request %lld buffer sect %lld count %lld total %lld\n", request_number, (unsigned long long)sect, nsect, tsect);    //debug
      if (blockio)
      {
        if (copy_to_user(buffer, transfer_buffer + ((sect - read_ctrl_data(CTRL_KSECTOR_START)) * KERNEL_SECTOR_SIZE), nbytes))
        {
          printk (KERN_WARNING "hddsuperdrive: failed to copy user data\n");
        }
      }
      else
      {
        memcpy(buffer, transfer_buffer + ((sect - read_ctrl_data(CTRL_KSECTOR_START)) * KERNEL_SECTOR_SIZE), nbytes);
      }
    }
    else
    {
      printk(KERN_INFO "hddsuperdrive: IO error sect %lld count %lld total %lld\n", (unsigned long long)sect, nsect, tsect);  //debug???
      if (return_zeros_on_error == 1)
      {
        if (blockio)
        {
          memset(block_io_buffer, 0, nbytes);
          if (copy_to_user(buffer, block_io_buffer, nbytes))
          {
            printk (KERN_WARNING "hddsuperdrive: failed to copy user data\n");
          }
        }
        else
        {
          memset(buffer, 0, nbytes);
        }
        return 0;
      }
      else if (return_zeros_on_error == 2)
      {
        unsigned long long n = 0;
        char *message = "HDDSUPERFILLMARK";
        u64 quad_word = 0xffeeeebbddaaeedd;
        while (n < nbytes)
        {
          if (blockio)
          {
            if (copy_to_user(buffer + n, message, 16))
            {
              printk (KERN_WARNING "hddsuperdrive: failed to copy user data\n");
            }
            n += 16;
            if (copy_to_user(buffer + n, &quad_word, 8))
            {
              printk (KERN_WARNING "hddsuperdrive: failed to copy user data\n");
            }
            n += 8;
            if (copy_to_user(buffer + n, &quad_word, 8))
            {
              printk (KERN_WARNING "hddsuperdrive: failed to copy user data\n");
            }
            n += 8;
          }
          else
          {
            memcpy(buffer + n, message, 16);
            n += 16;
            memcpy(buffer + n, &quad_word, 8);
            n += 8;
            memcpy(buffer + n, &quad_word, 8);
            n += 8;
          }
        }
      }
      else
      {
        if (blockio)
        {
          if (copy_to_user(buffer, transfer_buffer + ((sect - read_ctrl_data(CTRL_KSECTOR_START)) * KERNEL_SECTOR_SIZE), nbytes))
          {
            printk (KERN_WARNING "hddsuperdrive: failed to copy user data\n");
          }
        }
        else
        {
          memcpy(buffer, transfer_buffer + ((sect - read_ctrl_data(CTRL_KSECTOR_START)) * KERNEL_SECTOR_SIZE), nbytes);
        }
        return -EIO;
      }
    }
  }
  // if not in current buffer then make a request
  else
  {
    read_number++;
    //printk(KERN_INFO "hddsuperdrive: request %lld asking sect %lld count %lld total %lld\n", request_number, (unsigned long long)sect, nsect, tsect);    //debug
    write_ctrl_data(CTRL_DATA_VALID, 0);
    write_ctrl_data(CTRL_KSECTOR_START, sect);
    write_ctrl_data(CTRL_KSECTOR_COUNT, nsect);
    write_ctrl_data(CTRL_KSECTORS_REQUESTED, tsect);
    write_ctrl_data(CTRL_ACK_REQUEST, 0);
    write_ctrl_data(CTRL_DATA_READY, 0);
    write_ctrl_data(CTRL_DATA_REQUEST, 1);
    #if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
    do_gettimeofday(&tv1);
    #else
    ktime_get_real_ts64(&tv1);
    #endif
    while (!read_ctrl_data(CTRL_DATA_READY))
    {
      if (!read_ctrl_data(CTRL_NO_DRIVER_SLEEP))
      {
        usleep_range(1, 1);
      }
      if (read_ctrl_data(CTRL_RESET_READ_TIMER))
      {
        #if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
        do_gettimeofday(&tv1);
        #else
        ktime_get_real_ts64(&tv1);
        #endif
        write_ctrl_data(CTRL_RESET_READ_TIMER, 0);
        lockup_detected = 0;
      }
      #if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
      do_gettimeofday(&tv2);
      elapsed_usec = ((long long)tv2.tv_usec + (1000000 * (long long)tv2.tv_sec)) - ((long long)tv1.tv_usec + (1000000 * (long long)tv1.tv_sec));
      #else
      ktime_get_real_ts64(&tv2);
      elapsed_usec = (((long long)tv2.tv_nsec / 1000) + (1000000 * (long long)tv2.tv_sec)) - (((long long)tv1.tv_nsec / 1000) + (1000000 * (long long)tv1.tv_sec));
      #endif
      if (elapsed_usec > read_ctrl_data(CTRL_READ_TIMEOUT) )
      {
        printk(KERN_NOTICE "hddsuperdrive: timeout reading sect %lld count %lld time %lld\n", (unsigned long long)sect, nsect, elapsed_usec);
        write_ctrl_data(CTRL_DATA_REQUEST, 0);
        lockup_detected = 1;
        return -EAGAIN;
      }
      if (!read_ctrl_data(CTRL_ACK_REQUEST) && elapsed_usec > read_ctrl_data(CTRL_ACK_TIMEOUT))
      {
        printk(KERN_NOTICE "hddsuperdrive: no ack reading sect %lld count %lld time %lld\n", (unsigned long long)sect, nsect, elapsed_usec);
        write_ctrl_data(CTRL_DATA_REQUEST, 0);
        lockup_detected = 1;
        return -EAGAIN;
      }
      if (stop_signal || read_ctrl_data(CTRL_STOP_SIGNAL))
      {
        printk(KERN_NOTICE "hddsuperdrive: request stop\n");
        return -EIO;
      }
    }
    write_ctrl_data(CTRL_DATA_REQUEST, 0);
    write_ctrl_data(CTRL_DATA_READY, 0);

    // if the mode is set for fast return to get domain, then just return without any other processing to speed things up
    if (read_ctrl_data(CTRL_RETURN_NO_DATA_FAST))
    {
      return 0;
    }

    // check for io error and if chunk matches what we are looking for
    if (read_ctrl_data(CTRL_DATA_VALID) && sect >= read_ctrl_data(CTRL_KSECTOR_START) && (sect + nsect) <= (read_ctrl_data(CTRL_KSECTOR_START) + read_ctrl_data(CTRL_KSECTOR_COUNT)))
    {
      if (!check_io_error_bitmap(sect - read_ctrl_data(CTRL_KSECTOR_START), nsect))
      {
        //printk(KERN_INFO "hddsuperdrive: request %lld reading sect %lld count %lld total %lld\n", request_number, (unsigned long long)sect, nsect, tsect);    //debug
        if (blockio)
        {
          if (copy_to_user(buffer, transfer_buffer + ((sect - read_ctrl_data(CTRL_KSECTOR_START)) * KERNEL_SECTOR_SIZE), nbytes))
          {
            printk (KERN_WARNING "hddsuperdrive: failed to copy user data\n");
          }
        }
        else
        {
          memcpy(buffer, transfer_buffer + ((sect - read_ctrl_data(CTRL_KSECTOR_START)) * KERNEL_SECTOR_SIZE), nbytes);
        }
      }
      else
      {
        printk(KERN_INFO "hddsuperdrive: IO error sect %lld count %lld total %lld\n", (unsigned long long)sect, nsect, tsect);  //debug???
        // copy the buffer no matter what, zero filling and marking is being done in the main program
        if (blockio)
        {
          if (copy_to_user(buffer, transfer_buffer + ((sect - read_ctrl_data(CTRL_KSECTOR_START)) * KERNEL_SECTOR_SIZE), nbytes))
          {
            printk (KERN_WARNING "hddsuperdrive: failed to copy user data\n");
          }
        }
        else
        {
          memcpy(buffer, transfer_buffer + ((sect - read_ctrl_data(CTRL_KSECTOR_START)) * KERNEL_SECTOR_SIZE), nbytes);
        }
        if (return_zeros_on_error == 1)
        {
          // zero fill of unread data is already done in main program, so no need to zero here
          //memset(buffer, 0, nbytes);
          return 0;
        }
        else if (return_zeros_on_error == 2)
        {
          // marking of unread data is now done in main program, so no need to mark here
          //unsigned long long n = 0;
          //char *message = "HDDSUPERFILLMARK";
          //u64 quad_word = 0xffeeeebbddaaeedd;
          //while (n < nbytes)
          //{
          //  memcpy(buffer + n, message, 16);
          //  n += 16;
          //  memcpy(buffer + n, &quad_word, 8);
          //  n += 8;
          //  memcpy(buffer + n, &quad_word, 8);
          //  n += 8;
          //}
          return 0;
        }
        else
        {
          //memcpy(buffer, transfer_buffer + ((sect - read_ctrl_data(CTRL_KSECTOR_START)) * KERNEL_SECTOR_SIZE), nbytes);
          return -EIO;
        }
      }
    }
    else
    {
      printk(KERN_INFO "hddsuperdrive: out of range or invalid IO error sect %lld count %lld total %lld\n", (unsigned long long)sect, nsect, tsect);  //debug???
      if (blockio)
      {
        memset(block_io_buffer, 0, nbytes);
        if (copy_to_user(buffer, block_io_buffer, nbytes))
        {
          printk (KERN_WARNING "hddsuperdrive: failed to copy user data\n");
        }
      }
      else
      {
        memset(buffer, 0, nbytes);
      }
      return -EIO;
    }
  }
  return 0;
}



#if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
static void main_data_request(struct request_queue *q)
{
  int blockio = 0;
  long long elapsed_usec = 0;
  int ret = 0;
  struct request *req;
  struct timeval tv1, tv2;

  ret = wait_for_queue(request_queue);
  if (ret < 0)
  {
    next_queue();
    return;
  }

  request_number++;
  //printk(KERN_INFO "hddsuperdrive: request %lld\n", request_number);  //debug
  req = blk_fetch_request(q);
  do_gettimeofday(&tv1);
  while (req != NULL)
  {
    //printk(KERN_NOTICE "hddsuperdrive: pos=%lld cur=%d sect=%d\n", blk_rq_pos(req), blk_rq_cur_sectors(req), blk_rq_sectors(req));  //debug
    ret = data_transfer(&data_device, blk_rq_pos(req), blk_rq_cur_sectors(req), blk_rq_sectors(req), bio_data(req->bio), rq_data_dir(req), blockio);
    if ( ! __blk_end_request_cur(req, ret) )
    {
      req = blk_fetch_request(q);
    }
    if (read_ctrl_data(CTRL_RESET_REQUEST_TIMER))
    {
      do_gettimeofday(&tv1);
      write_ctrl_data(CTRL_RESET_REQUEST_TIMER, 0);
    }
    do_gettimeofday(&tv2);
    elapsed_usec = ((long long)tv2.tv_usec + (1000000 * (long long)tv2.tv_sec)) - ((long long)tv1.tv_usec + (1000000 * (long long)tv1.tv_sec));
    if (elapsed_usec > read_ctrl_data(CTRL_REQUEST_TIMEOUT))
    {
      stop_signal = 1;
      write_ctrl_data(CTRL_STOP_SIGNAL, 1);
      printk(KERN_NOTICE "hddsuperdrive: request timeout, stop request\n");
    }
  }
  //printk(KERN_INFO "hddsuperdrive: request %lld\n", request_number);  //debug
  next_queue();
}

#else
static blk_status_t main_data_request(struct blk_mq_hw_ctx *hctx, const struct blk_mq_queue_data* bd)
{
  int blockio = 0;
  long long elapsed_usec = 0;
  int ret = 0;
  int fail = 0;
  struct request *req = bd->rq;
  struct timespec64 tv1, tv2;
  struct bio_vec bvec;
  struct req_iterator iter;
  blk_status_t status = BLK_STS_OK;

  long long pos = blk_rq_pos(req);
  long long sect = blk_rq_sectors(req);

  ret = wait_for_queue(request_queue);
  if (ret < 0)
  {
    next_queue();
    return ret;
  }


  request_number++;
  //printk(KERN_INFO "hddsuperdrive: request %lld\n", request_number);  //debug

  blk_mq_start_request(req);
  ktime_get_real_ts64(&tv1);

  //printk(KERN_NOTICE "hddsuperdrive: request pos=%ld cur=%d sec=%d \n", blk_rq_pos(req), blk_rq_cur_sectors(req), blk_rq_sectors(req));  //debug

  rq_for_each_segment(bvec, req, iter)
  {
    unsigned long s_len = bvec.bv_len >> SECTOR_SHIFT;

    void* b_buf = page_address(bvec.bv_page) + bvec.bv_offset;

    //printk(KERN_NOTICE "hddsuperdrive: pos=%lld s_len=%ld sect=%lld\n", pos, s_len, sect);  //debug
    ret = data_transfer(&data_device, pos, s_len, sect, b_buf, rq_data_dir(req), blockio);
    if (ret)
    {
      fail = ret;
    }
    //printk(KERN_NOTICE "pos %lld len %ld\n", pos, s_len);  //debug
    pos += s_len;
    sect -= s_len;

    if (read_ctrl_data(CTRL_RESET_REQUEST_TIMER))
    {
      ktime_get_real_ts64(&tv1);
      write_ctrl_data(CTRL_RESET_REQUEST_TIMER, 0);
    }
    ktime_get_real_ts64(&tv2);
    elapsed_usec = (((long long)tv2.tv_nsec / 1000) + (1000000 * (long long)tv2.tv_sec)) - (((long long)tv1.tv_nsec / 1000) + (1000000 * (long long)tv1.tv_sec));
    if (elapsed_usec > read_ctrl_data(CTRL_REQUEST_TIMEOUT))
    {
      stop_signal = 1;
      write_ctrl_data(CTRL_STOP_SIGNAL, 1);
      printk(KERN_NOTICE "hddsuperdrive: request timeout, stop request\n");
    }
  }
  if (fail)
  {
    if (fail == -EAGAIN)
    {
      status = BLK_STS_AGAIN;
    }
    else
    {
      status = BLK_STS_IOERR;
    }
  }
  blk_mq_end_request(req, status);
  next_queue();
  return BLK_STS_OK;    //always return ok?
}
#endif



static int hddscbd_getgeo(struct block_device * block_device, struct hd_geometry * geo)
{
  geo->cylinders = data_device.chs_cylinders;
  geo->heads = data_device.chs_heads;
  geo->sectors = data_device.chs_sectors;
  geo->start = 0;
  return 0;
}





static void set_sense_buffer(const int transferred, struct sg_io_hdr *sgio_obj)
{
  int real_len = 18;
  unsigned char sbuf[18];
  unsigned char skey = 0;
  unsigned char asc = 0;
  unsigned char ascq = 0;
  memset (sbuf, 0, sizeof(sbuf));
  if (sgio_obj->mx_sb_len < real_len)
  {
    real_len = sgio_obj->mx_sb_len;
  }
  sgio_obj->host_status = 0;    // SG_ERR_DID_OK
  sgio_obj->driver_status = 0;    // SG_ERR_DRIVER_OK
  if (transferred >= 0)
  {
    sgio_obj->status = 0;    // SAM_STAT_GOOD
    sgio_obj->masked_status = 0;    // GOOD
    sgio_obj->info = 0;    // SG_INFO_OK
    sgio_obj->resid = sgio_obj->dxfer_len - transferred;
    sgio_obj->sb_len_wr = 0;
  }
  else
  {
    sgio_obj->status = 0x02;    // SAM_STAT_CHECK_CONDITION
    sgio_obj->masked_status = 0x01;    // CHECK_CONDITION
    sgio_obj->info = 0x01;    // SG_INFO_CHECK
    sgio_obj->resid = sgio_obj->dxfer_len;
    sgio_obj->sb_len_wr = real_len;
    sbuf[0] = 0x70;
    sbuf[7] = (18 - 7);
    if (transferred == -EIO)    // 3 11 00 Medium Error - unrecovered read error
    {
      skey = 0x3;
      asc = 0x11;
      ascq = 0x00;
    }
    else if (transferred == -EINVAL)    // 5 20 00 Illegal Request - invalid/unsupported command code
    {
      skey = 0x5;
      asc = 0x20;
      ascq = 0x00;
    }
    else if (transferred == -EBUSY)    //  2 04 00 Not Ready - Cause not reportable.
    {
      skey = 0x2;
      asc = 0x04;
      ascq = 0x00;
    }
    else if (transferred == -ETIME)    //  B 08 01 Aborted Command - LUN communication timeout
    {
      skey = 0xb;
      asc = 0x08;
      ascq = 0x01;
    }
    else if (transferred == -EAGAIN)    //  2 04 01 Not Ready - becoming ready
    {
      skey = 0x2;
      asc = 0x04;
      ascq = 0x01;
    }
    else if (transferred == -EROFS)    //   7 27 00 Write Protect - command not allowed
    {
      skey = 0x7;
      asc = 0x27;
      ascq = 0x00;
    }
    else if (transferred < 0)    // B 00 00 Aborted Command - no additional sense code
    {
      skey = 0xb;
      asc = 0x00;
      ascq = 0x00;
    }
  }
  sbuf[2] = skey;
  sbuf[12] = asc;
  sbuf[13] = ascq;
  if (copy_to_user (sgio_obj->sbp, sbuf, real_len))
  {
    printk (KERN_WARNING "hddsuperdrive: failed to copy user data\n");
  }
}



static void set_sense_buffer_v4(const int transferred, struct sg_io_v4 *sgio4_obj)
{
  int real_len = 18;
  unsigned char sbuf[18];
  unsigned char skey = 0;
  unsigned char asc = 0;
  unsigned char ascq = 0;
  memset (sbuf, 0, sizeof(sbuf));
  if (sgio4_obj->max_response_len < real_len)
  {
    real_len = sgio4_obj->max_response_len;
  }
  sgio4_obj->transport_status = 0;    // SG_ERR_DID_OK
  sgio4_obj->driver_status = 0;    // SG_ERR_DRIVER_OK
  sgio4_obj->dout_resid = sgio4_obj->dout_xfer_len;
  if (transferred >= 0)
  {
    sgio4_obj->device_status = 0;    // SAM_STAT_GOOD
    sgio4_obj->info = 0;    // SG_INFO_OK
    sgio4_obj->din_resid = sgio4_obj->din_xfer_len - transferred;
    sgio4_obj->response_len = 0;
  }
  else
  {
    sgio4_obj->device_status = 0x02;    //SAM_STAT_CHECK_CONDITION
    sgio4_obj->info = 0x01;    // SG_INFO_CHECK
    sgio4_obj->din_resid = sgio4_obj->din_xfer_len;
    sgio4_obj->response_len = real_len;
    sbuf[0] = 0x70;
    sbuf[7] = (18 - 7);
    if (transferred == -EIO)    // 3 11 00 Medium Error - unrecovered read error
    {
      skey = 0x3;
      asc = 0x11;
      ascq = 0x00;
    }
    else if (transferred == -EINVAL)    // 5 20 00 Illegal Request - invalid/unsupported command code
    {
      skey = 0x5;
      asc = 0x20;
      ascq = 0x00;
    }
    else if (transferred == -EBUSY)    //  2 04 00 Not Ready - Cause not reportable.
    {
      skey = 0x2;
      asc = 0x04;
      ascq = 0x00;
    }
    else if (transferred == -ETIME)    //  B 08 01 Aborted Command - LUN communication timeout
    {
      skey = 0xb;
      asc = 0x08;
      ascq = 0x01;
    }
    else if (transferred == -EAGAIN)    //  2 04 01 Not Ready - becoming ready
    {
      skey = 0x2;
      asc = 0x04;
      ascq = 0x01;
    }
    else if (transferred == -EROFS)    //   7 27 00 Write Protect - command not allowed
    {
      skey = 0x7;
      asc = 0x27;
      ascq = 0x00;
    }
    else if (transferred < 0)    // B 00 00 Aborted Command - no additional sense code
    {
      skey = 0xb;
      asc = 0x00;
      ascq = 0x00;
    }
  }
  sbuf[2] = skey;
  sbuf[12] = asc;
  sbuf[13] = ascq;
  if (copy_to_user ((void*)(uintptr_t)sgio4_obj->response, sbuf, real_len))
  {
    printk (KERN_WARNING "hddsuperdrive: failed to copy user data\n");
  }
}



static int process_inquiry(const unsigned char* cdb, unsigned char *buffer, const int max_len)
{
  int real_len = 44;
  unsigned char ibuf[44];
  memset (ibuf, 0, sizeof(ibuf));
  if (cdb[1]) // EVPD and CMDDT
  {
    return -EINVAL;
  }
  if (max_len < real_len)
  {
    real_len = max_len;
  }
  strncpy (ibuf + 8, "hmodel", 16);
  strncpy (ibuf + 36, "hserial", 8);
  //strncpy (ibuf + 8, data_device.device_name, 16);
  //strncpy (ibuf + 36, data_device.device_name, 8);
  if (copy_to_user (buffer, ibuf, real_len))
  {
    printk (KERN_WARNING "hddsuperdrive: failed to copy user data\n");
  }
  return real_len;
}



static int process_readcapacity10(const unsigned char* cdb, unsigned char *buffer, const int max_len)
{
  int real_len = 8;
  unsigned char cbuf[8];
  long long lastblock = data_device.sectors - 1;
  if (max_len < real_len)
  {
    return -EINVAL;
  }
  cbuf[0] = lastblock >> 24;
  cbuf[1] = lastblock >> 16;
  cbuf[2] = lastblock >> 8;
  cbuf[3] = lastblock;
  cbuf[4] = data_device.block_size >> 24;
  cbuf[5] = data_device.block_size >> 16;
  cbuf[6] = data_device.block_size >> 8;
  cbuf[7] = data_device.block_size;
  if (lastblock > 0xffffffff)
  {
    cbuf[0] = 0xff;
    cbuf[1] = 0xff;
    cbuf[2] = 0xff;
    cbuf[3] = 0xff;
  }
  if (copy_to_user (buffer, cbuf, real_len))
  {
    printk (KERN_WARNING "hddsuperdrive: failed to copy user data\n");
  }
  return real_len;
}



static int process_readcapacity16(const unsigned char* cdb, unsigned char *buffer, const int max_len)
{
  unsigned char cbuf[32];
  unsigned int allocate_len = 0;
  int real_len = 32;
  long long lastblock = data_device.sectors - 1;
  allocate_len = allocate_len + cdb[13];
  allocate_len = allocate_len + ((long long)cdb[12] << 8);
  allocate_len = allocate_len + ((long long)cdb[11] << 16);
  allocate_len = allocate_len + ((long long)cdb[10] << 24);
  if (allocate_len < real_len)
  {
    real_len = allocate_len;
  }
  cbuf[0] = lastblock >> 56;
  cbuf[1] = lastblock >> 48;
  cbuf[2] = lastblock >> 40;
  cbuf[3] = lastblock >> 32;
  cbuf[4] = lastblock >> 24;
  cbuf[5] = lastblock >> 16;
  cbuf[6] = lastblock >> 8;
  cbuf[7] = lastblock;
  cbuf[8] = data_device.block_size >> 24;
  cbuf[9] = data_device.block_size >> 16;
  cbuf[10] = data_device.block_size >> 8;
  cbuf[11] = data_device.block_size;
  if (copy_to_user (buffer, cbuf, real_len))
  {
    printk (KERN_WARNING "hddsuperdrive: failed to copy user data\n");
  }
  return real_len;
}



static int process_read10(const unsigned char* cdb, unsigned char *buffer, const int max_len)
{
  int blockio = 1;
  int write = 0;
  unsigned long long position = 0;
  unsigned long long size = 0;
  int multiplier = data_device.block_size / KERNEL_SECTOR_SIZE;
  position = position + cdb[5];
  position = position + (cdb[4] << 8);
  position = position + (cdb[3] << 16);
  position = position + (cdb[2] << 24);
  size = size + cdb[8];
  size = size + (cdb[7] << 8);
  position = position * multiplier;
  size = size * multiplier;
  if ( (size * KERNEL_SECTOR_SIZE) > DRIVER_MAIN_DATA_BUFFER_SIZE)
  {
    return -EINVAL;
  }
  if ( (size * KERNEL_SECTOR_SIZE) > max_len)
  {
    return -EINVAL;
  }
  //printk(KERN_INFO "hddsc_read10 position %lld size %lld blocksize %lld\n", position, size, data_device.block_size);    //debug
  return data_transfer(&data_device, position, size, size, buffer, write, blockio);
}



static int process_read16(const unsigned char* cdb, unsigned char *buffer, const int max_len)
{
  int blockio = 1;
  int write = 0;
  unsigned long long position = 0;
  unsigned long long size = 0;
  int multiplier = data_device.block_size / KERNEL_SECTOR_SIZE;
  position = position + cdb[9];
  position = position + ((long long)cdb[8] << 8);
  position = position + ((long long)cdb[7] << 16);
  position = position + ((long long)cdb[6] << 24);
  position = position + ((long long)cdb[5] << 32);
  position = position + ((long long)cdb[4] << 40);
  position = position + ((long long)cdb[3] << 48);
  position = position + ((long long)cdb[2] << 56);
  size = size + cdb[13];
  size = size + ((long long)cdb[12] << 8);
  size = size + ((long long)cdb[11] << 16);
  size = size + ((long long)cdb[10] << 24);
  position = position * multiplier;
  size = size * multiplier;
  if ( (size * KERNEL_SECTOR_SIZE) > DRIVER_MAIN_DATA_BUFFER_SIZE)
  {
    return -EINVAL;
  }
  if ( (size * KERNEL_SECTOR_SIZE) > max_len)
  {
    return -EINVAL;
  }
  return data_transfer(&data_device, position, size, size, buffer, write, blockio);
}





static int block_device_open(struct block_device * block_device, fmode_t mode)
{
  //printk(KERN_INFO "hddsc_block_device_open mode %d\n", mode);    //debug
  return 0;
}

static void block_device_release(struct gendisk * gendisk, fmode_t mode)
{
  //printk(KERN_INFO "hddsc_block_device_release mode %d\n", mode);    //debug
}

static int block_device_ioctl(struct block_device * block_device, fmode_t mode, unsigned cmd, unsigned long arg)
{
  int ret = 0;
  int return_value = 0;
  //printk(KERN_INFO "hddsc_block_device_ioctl mode %d cmd %x arg %lx\n", mode, cmd, arg);    //debug

  if (cmd == SG_GET_VERSION_NUM)
  {
    void __user *p = (void __user *)arg;
    int __user *ip = p;
    //printk(KERN_INFO "hddsc_block_device ioctl SG_GET_VERSION_NUM request\n");    //debug
    return put_user(sg_version_num, ip);
  }

  if (cmd == SG_IO)
  {
    struct sg_io_hdr *sgio_obj;
    struct sg_io_v4 *sgio4_obj;
    unsigned char cdb[16];

    ret = wait_for_queue(request_queue);
    if (ret < 0)
    {
      next_queue();
      return ret;
    }

    //printk(KERN_INFO "hddsc_block_device ioctl SG_IO request\n");    //debug
    sgio_obj= kmalloc(sizeof(struct sg_io_hdr), GFP_KERNEL);
    sgio4_obj= kmalloc(sizeof(struct sg_io_v4), GFP_KERNEL);
    if (copy_from_user(sgio_obj, (void *)arg, sizeof(struct sg_io_hdr)))
    {
      printk (KERN_WARNING "hddsuperdrive: failed to copy user data\n");
    }
    // SG_IO v4
    if (sgio_obj->interface_id == 'Q')
    {
      if (copy_from_user(sgio4_obj, (void *)arg, sizeof(struct sg_io_v4)))
      {
        printk (KERN_WARNING "hddsuperdrive: failed to copy user data\n");
      }
      //printk(KERN_INFO "hddsc_block_device interface_id %c\n", sgio4_obj->guard);    //debug
      //printk(KERN_INFO "hddsc_block_device cmd_len %d\n", sgio4_obj->request_len);    //debug
      if (copy_from_user (cdb, (const void*)(uintptr_t)sgio4_obj->request, sgio4_obj->request_len))
      {
        printk (KERN_WARNING "hddsuperdrive: failed to copy user data\n");
      }
      if (0)
      {
        if (sgio4_obj->request_len == 6)
        {
          printk(KERN_INFO "hddsc_block_device cdb %02x %02x %02x %02x %02x %02x\n", cdb[0], cdb[1], cdb[2], cdb[3], cdb[4], cdb[5]);    //debug
        }
        else if (sgio4_obj->request_len == 10)
        {
          printk(KERN_INFO "hddsc_block_device cdb %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", cdb[0], cdb[1], cdb[2], cdb[3], cdb[4], cdb[5], cdb[6], cdb[7], cdb[8], cdb[9]);    //debug
        }
        else if (sgio4_obj->request_len == 12)
        {
          printk(KERN_INFO "hddsc_block_device cdb %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", cdb[0], cdb[1], cdb[2], cdb[3], cdb[4], cdb[5], cdb[6], cdb[7], cdb[8], cdb[9], cdb[10], cdb[11]);    //debug
        }
        else
        {
          printk(KERN_INFO "hddsc_block_device cdb %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", cdb[0], cdb[1], cdb[2], cdb[3], cdb[4], cdb[5], cdb[6], cdb[7], cdb[8], cdb[9], cdb[10], cdb[11], cdb[12], cdb[13], cdb[14], cdb[15]);    //debug
        }
      }
      //printk(KERN_INFO "hddsc_block_device din_xfer_len %d\n", sgio4_obj->din_xfer_len);    //debug
      //printk(KERN_INFO "hddsc_block_device dout_xfer_len %d\n", sgio4_obj->dout_xfer_len);    //debug
      //printk(KERN_INFO "hddsc_block_device max_response_len %d\n", sgio4_obj->max_response_len);    //debug
      //printk(KERN_INFO "hddsc_block_device timeout %d\n", sgio4_obj->timeout);    //debug
      //printk(KERN_INFO "hddsc_block_device flags 0x%x\n", sgio4_obj->flags);    //debug
      //printk(KERN_INFO "hddsc_block_device din_iovec_count %d\n", sgio4_obj->din_iovec_count);    //debug
      //printk(KERN_INFO "hddsc_block_device dout_iovec_count %d\n", sgio4_obj->dout_iovec_count);    //debug
      //printk(KERN_INFO "hddsc_block_device %d\n", sgio4_obj->);    //debug

      if (cdb[0] == SCSI_INQUIRY)
      {
        ret = process_inquiry(cdb, (unsigned char*)(uintptr_t)sgio4_obj->din_xferp, sgio4_obj->din_xfer_len);
        set_sense_buffer_v4(ret, sgio4_obj);
      }
      else if (cdb[0] == SCSI_READCAPACITY10)
      {
        ret = process_readcapacity10(cdb, (unsigned char*)(uintptr_t)sgio4_obj->din_xferp, sgio4_obj->din_xfer_len);
        set_sense_buffer_v4(ret, sgio4_obj);
      }
      else if (cdb[0] == SCSI_READCAPACITY16 && (cdb[1] & 0x1f) == SCSI_READCAPACITY16SERVICEACTION)
      {
        ret = process_readcapacity16(cdb, (unsigned char*)(uintptr_t)sgio4_obj->din_xferp, sgio4_obj->din_xfer_len);
        set_sense_buffer_v4(ret, sgio4_obj);
      }
      else if (cdb[0] == SCSI_READ10)
      {
        ret = process_read10(cdb, (unsigned char*)(uintptr_t)sgio4_obj->din_xferp, sgio4_obj->din_xfer_len);
        set_sense_buffer_v4(ret, sgio4_obj);
      }
      else if (cdb[0] == SCSI_READ16)
      {
        ret = process_read16(cdb, (unsigned char*)(uintptr_t)sgio4_obj->din_xferp, sgio4_obj->din_xfer_len);
        set_sense_buffer_v4(ret, sgio4_obj);
      }
      else if (cdb[0] == SCSI_WRITE10)
      {
        set_sense_buffer_v4(-EROFS, sgio4_obj);
      }
      else if (cdb[0] == SCSI_WRITE16)
      {
        set_sense_buffer_v4(-EROFS, sgio4_obj);
      }
      else
      {
        printk(KERN_INFO "hddsuperdrive: unsupported SCSI command 0x%02x\n", cdb[0]);
        set_sense_buffer_v4(-EINVAL, sgio4_obj);
      }

      if (copy_to_user((void *)arg, sgio4_obj, sizeof(struct sg_io_v4)))
      {
        printk (KERN_WARNING "hddsuperdrive: failed to copy user data\n");
      }
    }
    // SG_IO v3
    else if (sgio_obj->interface_id == 'S')
    {
      //printk(KERN_INFO "hddsc_block_device interface_id %c\n", sgio_obj->interface_id);    //debug
      //printk(KERN_INFO "hddsc_block_device cmd_len %d\n", sgio_obj->cmd_len);    //debug
      if (copy_from_user (cdb, sgio_obj->cmdp, sgio_obj->cmd_len))
      {
        printk (KERN_WARNING "hddsuperdrive: failed to copy user data\n");
      }
      if (0)
      {
        if (sgio_obj->cmd_len == 6)
        {
          printk(KERN_INFO "hddsc_block_device cdb %02x %02x %02x %02x %02x %02x\n", cdb[0], cdb[1], cdb[2], cdb[3], cdb[4], cdb[5]);    //debug
        }
        else if (sgio_obj->cmd_len == 10)
        {
          printk(KERN_INFO "hddsc_block_device cdb %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", cdb[0], cdb[1], cdb[2], cdb[3], cdb[4], cdb[5], cdb[6], cdb[7], cdb[8], cdb[9]);    //debug
        }
        else if (sgio_obj->cmd_len == 12)
        {
          printk(KERN_INFO "hddsc_block_device cdb %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", cdb[0], cdb[1], cdb[2], cdb[3], cdb[4], cdb[5], cdb[6], cdb[7], cdb[8], cdb[9], cdb[10], cdb[11]);    //debug
        }
        else
        {
          printk(KERN_INFO "hddsc_block_device cdb %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", cdb[0], cdb[1], cdb[2], cdb[3], cdb[4], cdb[5], cdb[6], cdb[7], cdb[8], cdb[9], cdb[10], cdb[11], cdb[12], cdb[13], cdb[14], cdb[15]);    //debug
        }
      }
      //printk(KERN_INFO "hddsc_block_device dxfer_direction %d\n", sgio_obj->dxfer_direction);    //debug
      //printk(KERN_INFO "hddsc_block_device dxfer_len %d\n", sgio_obj->dxfer_len);    //debug
      //printk(KERN_INFO "hddsc_block_device mx_sb_len %d\n", sgio_obj->mx_sb_len);    //debug
      //printk(KERN_INFO "hddsc_block_device timeout %d\n", sgio_obj->timeout);    //debug
      //printk(KERN_INFO "hddsc_block_device flags 0x%x\n", sgio_obj->flags);    //debug
      //printk(KERN_INFO "hddsc_block_device iovec_count %d\n", sgio_obj->iovec_count);    //debug
      //printk(KERN_INFO "hddsc_block_device %d\n", sgio_obj->);    //debug

      if (cdb[0] == SCSI_INQUIRY)
      {
        ret = process_inquiry(cdb, sgio_obj->dxferp, sgio_obj->dxfer_len);
        set_sense_buffer(ret, sgio_obj);
      }
      else if (cdb[0] == SCSI_READCAPACITY10)
      {
        ret = process_readcapacity10(cdb, sgio_obj->dxferp, sgio_obj->dxfer_len);
        set_sense_buffer(ret, sgio_obj);
      }
      else if (cdb[0] == SCSI_READCAPACITY16 && (cdb[1] & 0x1f) == SCSI_READCAPACITY16SERVICEACTION)
      {
        ret = process_readcapacity16(cdb, sgio_obj->dxferp, sgio_obj->dxfer_len);
        set_sense_buffer(ret, sgio_obj);
      }
      else if (cdb[0] == SCSI_READ10)
      {
        ret = process_read10(cdb, sgio_obj->dxferp, sgio_obj->dxfer_len);
        set_sense_buffer(ret, sgio_obj);
      }
      else if (cdb[0] == SCSI_READ16)
      {
        ret = process_read16(cdb, sgio_obj->dxferp, sgio_obj->dxfer_len);
        set_sense_buffer(ret, sgio_obj);
      }
      else if (cdb[0] == SCSI_WRITE10)
      {
        set_sense_buffer(-EROFS, sgio_obj);
      }
      else if (cdb[0] == SCSI_WRITE16)
      {
        set_sense_buffer(-EROFS, sgio_obj);
      }
      else
      {
        printk(KERN_INFO "hddsuperdrive: unsupported SCSI command 0x%02x\n", cdb[0]);
        set_sense_buffer(-EINVAL, sgio_obj);
      }

      if (copy_to_user((void *)arg, sgio_obj, sizeof(struct sg_io_hdr)))
      {
        printk (KERN_WARNING "hddsuperdrive: failed to copy user data\n");
      }
    }
    // SG_IO bad version
    else
    {
      printk(KERN_INFO "hddsuperdrive: ioctl invalid SG_IO version %c\n", sgio_obj->interface_id);
      kfree(sgio_obj);
      kfree(sgio4_obj);
      next_queue();
      return -EINVAL;
    }

    next_queue();
    return return_value;
  }
  printk(KERN_INFO "hddsuperdrive: unsupported ioctl command 0x%04x\n", cmd);    //debug???
  return -ENOIOCTLCMD;
}








static struct block_device_operations data_operations =
{
  .owner = THIS_MODULE,
  .getgeo = hddscbd_getgeo,
  .open = block_device_open,
  .release = block_device_release,
  .ioctl = block_device_ioctl
};


#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,0,0)
static struct blk_mq_ops _mq_ops = {
    .queue_rq = main_data_request,
};
#endif



static void unregister_data_drive(void)
{
  del_gendisk(data_device.gd);
  put_disk(data_device.gd);
  unregister_blkdev(data_major_num, data_device.device_name);
  data_major_num = 0;
  blk_cleanup_queue(data_queue);
  #if LINUX_VERSION_CODE >= KERNEL_VERSION(5,0,0)
  blk_mq_free_tag_set(&data_device.tag_set);
  #endif
}



static void unregister_data_file(void)
{
  unregister_chrdev(data_major_num, data_device.device_name);
}



static int device_open(struct inode *inode, struct file *file)
{
  //printk(KERN_INFO "hddsc_device_open\n");    //debug
  if (device_is_open)
  {
    printk(KERN_INFO "hddsuperdrive: device already open\n");
    return -EBUSY;
  }
  device_is_open = 1;
  return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
  //printk(KERN_INFO "hddsc_device_release\n");    //debug
  if (!device_is_open)
  {
    printk(KERN_INFO "hddsuperdrive: device was not open\n");
  }
  device_is_open = 0;
  return 0;
}

static loff_t device_seek(struct file *file, loff_t offset, int type)
{
  printk(KERN_INFO "hddsc_device_seek offset=%lld type=%d\n", offset, type);    //debug
  if (type == SEEK_END)
  {
    //printk(KERN_INFO "hddsc_device_seek END\n");    //debug
    current_offset = data_device.size + offset;
    return current_offset;
  }
  if (type == SEEK_SET)
  {
    //printk(KERN_INFO "hddsc_device_seek SET\n");    //debug
    current_offset = offset;
    return current_offset;
  }
  if (type == SEEK_CUR)
  {
    //printk(KERN_INFO "hddsc_device_seek CUR\n");    //debug
    current_offset = current_offset + offset;
    return current_offset;
  }
  return -1;
}

static ssize_t device_read(struct file *file, char __user *buffer, size_t size, loff_t *offset)
{
  int blockio = 0;
  int write = 0;
  int ret = 0;
  long long sectors = 0;
  long long position = 0;
  if (operation_in_progress)
  {
    printk(KERN_INFO "hddsuperdrive: attempted operation while in use\n");
    return -EBUSY;
  }
  sectors = size / KERNEL_SECTOR_SIZE;
  position = current_offset / KERNEL_SECTOR_SIZE;
  //printk(KERN_INFO "hddsc_device_read size=%d offset=%lld\n", (int) size, *offset);    //debug
  operation_in_progress = 1;
  ret = data_transfer(&data_device, position, sectors, sectors, buffer, write, blockio);
  operation_in_progress = 0;
  if (ret < 0)
  {
    return ret;
  }
  return size;
}

static ssize_t device_write(struct file *file, const char __user *buffer, size_t size, loff_t *offset)
{
  int blockio = 0;
  int write = 1;
  int ret = 0;
  long long sectors = 0;
  long long position = 0;
  if (operation_in_progress)
  {
    printk(KERN_INFO "hddsuperdrive: attempted operation while in use\n");
    return -EBUSY;
  }
  sectors = size / KERNEL_SECTOR_SIZE;
  position = current_offset / KERNEL_SECTOR_SIZE;
  //printk(KERN_INFO "hddsc_device_write size=%d offset=%lld\n", (int) size, *offset);    //debug
  operation_in_progress = 1;
  ret = data_transfer(&data_device, position, sectors, sectors, (char*) buffer, write, blockio);
  operation_in_progress = 0;
  if (ret < 0)
  {
    return ret;
  }
  return size;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0)
static const struct proc_ops device_fops = {
  .proc_open = device_open,
  .proc_release = device_release,
  .proc_lseek = device_seek,
  .proc_read = device_read,
  .proc_write = device_write
};
static const struct file_operations device_ffops = {
  .open = device_fops.proc_open,
  .release = device_fops.proc_release,
  .llseek = device_fops.proc_lseek,
  .read = device_fops.proc_read,
  .write = device_fops.proc_write,
  .owner = THIS_MODULE
};
#else
static const struct file_operations device_fops = {
  .open = device_open,
  .release = device_release,
  .llseek = device_seek,
  .read = device_read,
  .write = device_write,
  .owner = THIS_MODULE
};
#endif




static long process_ioctl(struct file *f, const unsigned cmd, const unsigned long  arg)
{
  if (cmd == IOCTL_CMD_HDDSC)
  {
    struct control_data *control_obj= kmalloc(sizeof(struct control_data), GFP_KERNEL);

    if (copy_from_user(control_obj, (void *)arg, sizeof(struct control_data)))
    {
      printk (KERN_WARNING "hddsuperdrive: failed to copy user data\n");
    }

    if (control_obj->command == START_DRIVE_COMMAND)
    {
      stop_signal = 1;
      if (data_drive_active)
      {
        msleep(100);
        unregister_data_drive();
        data_drive_active = 0;
      }
      stop_signal = 0;
      write_ctrl_data(CTRL_STOP_SIGNAL, 0);
      write_ctrl_data(CTRL_READ_TIMEOUT, control_obj->read_timeout);
      write_ctrl_data(CTRL_REQUEST_TIMEOUT, control_obj->request_timeout);
      write_ctrl_data(CTRL_ACK_TIMEOUT, control_obj->ack_timeout);
      return_zeros_on_error = control_obj->return_zeros_on_error;
      io_scsi_only = control_obj->io_scsi_only;
      process_id = control_obj->process_id;
      data_device.size = control_obj->total_logical_sectors * control_obj->logical_block_size;
      data_device.sectors = control_obj->total_logical_sectors;
      data_device.block_size = control_obj->logical_block_size;
      data_device.chs_heads = control_obj->chs_heads;
      data_device.chs_sectors = control_obj->chs_sectors;
      data_device.chs_cylinders = control_obj->chs_cylinders;
      strncpy(data_device.device_name, control_obj->name, sizeof(data_device.device_name) - 1);
      working_queue = 0;
      request_queue = 0;
      queue_count = 0;

      #if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
      spin_lock_init(&data_device.lock);
      data_queue = blk_init_queue(main_data_request, &data_device.lock);
      if (data_queue == NULL)
      {
        goto out;
      }
      #else
      //configure tag_set
      data_device.tag_set.ops = &_mq_ops;
      data_device.tag_set.nr_hw_queues = 1;
      data_device.tag_set.queue_depth = 1;
      data_device.tag_set.numa_node = NUMA_NO_NODE;
      data_device.tag_set.cmd_size = sizeof(sblkdev_cmd_t);
      data_device.tag_set.flags = BLK_MQ_F_SHOULD_MERGE;
      //data_device.tag_set.driver_data = data_device;

      if (blk_mq_alloc_tag_set(&data_device.tag_set))
      {
        printk(KERN_WARNING "hddsuperdrive: unable to allocate tag set\n");
        goto out;
      }

      //configure queue
      data_queue = blk_mq_init_queue(&data_device.tag_set);
      if (IS_ERR(data_queue))
      {
        printk(KERN_WARNING "hddsuperdrive: Failed to allocate queue\n");
        goto out;
      }
      #endif

      blk_queue_logical_block_size(data_queue, control_obj->logical_block_size);
      blk_queue_physical_block_size(data_queue, control_obj->physical_block_size);
      blk_queue_max_hw_sectors(data_queue, DRIVER_TRANSFER_BUFFER_SIZE / KERNEL_SECTOR_SIZE);
      blk_queue_max_segment_size(data_queue, DRIVER_TRANSFER_BUFFER_SIZE);

      data_major_num = register_blkdev(data_major_num, data_device.device_name);
      if (data_major_num < 0)
      {
        printk(KERN_WARNING "hddsuperdrive: unable to get major number\n");
        data_major_num = 0;
        goto out;
      }
#if 0
      data_device.gd = alloc_disk(16);
#else
      data_device.gd = __alloc_disk_node(data_queue, NUMA_NO_NODE, &super_bio_compl_lkclass);
#endif
      if (!data_device.gd)
      {
        goto out_unregister;
      }
      data_device.gd->major = data_major_num;
      data_device.gd->first_minor = 0;
      data_device.gd->fops = &data_operations;
      data_device.gd->private_data = &data_device;
      strcpy(data_device.gd->disk_name, data_device.device_name);
      set_capacity(data_device.gd, (data_device.size / KERNEL_SECTOR_SIZE));
      data_device.gd->queue = data_queue;
      add_disk(data_device.gd);

      data_drive_active = 0;
      write_ctrl_data(CTRL_DATA_DRIVE_ACTIVE, 0);
      kfree(control_obj);
      return 0;

      out_unregister:
      unregister_blkdev(data_major_num, data_device.device_name);
      data_major_num = 0;
      out:
      kfree(control_obj);
      return -ENOMEM;
    }

    else if (control_obj->command == STOP_DRIVE_COMMAND)
    {
      if (data_drive_active)
      {
        stop_signal = 1;
        write_ctrl_data(CTRL_STOP_SIGNAL, 1);
        unregister_data_drive();
        data_drive_active = 0;
        write_ctrl_data(CTRL_DATA_DRIVE_ACTIVE, 0);
        process_id = 0;
        kfree(control_obj);
        return 0;
      }
    }

    else if (control_obj->command == START_FILE_COMMAND)
    {
      stop_signal = 1;
      if (data_drive_active)
      {
        msleep(100);
        unregister_data_file();
        data_drive_active = 0;
      }
      stop_signal = 0;
      write_ctrl_data(CTRL_STOP_SIGNAL, 0);
      write_ctrl_data(CTRL_READ_TIMEOUT, control_obj->read_timeout);
      write_ctrl_data(CTRL_REQUEST_TIMEOUT, control_obj->request_timeout);
      write_ctrl_data(CTRL_ACK_TIMEOUT, control_obj->ack_timeout);
      return_zeros_on_error = control_obj->return_zeros_on_error;
      io_scsi_only = control_obj->io_scsi_only;
      process_id = control_obj->process_id;
      data_device.size = control_obj->total_logical_sectors * control_obj->logical_block_size;
      data_device.block_size = control_obj->logical_block_size;
      data_device.chs_heads = control_obj->chs_heads;
      data_device.chs_sectors = control_obj->chs_sectors;
      data_device.chs_cylinders = control_obj->chs_cylinders;
      strncpy(data_device.device_name, control_obj->name, sizeof(data_device.device_name) - 1);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0)
      data_major_num = register_chrdev(0, data_device.device_name, &device_ffops);
#else
      data_major_num = register_chrdev(0, data_device.device_name, &device_fops);
#endif

      if (data_major_num < 0)
      {
        printk(KERN_ALERT "hddsuperdrive: Registering char device failed with %d\n", data_major_num);
        return data_major_num;
      }

      proc_create(data_device.device_name, 0, NULL, &device_fops);

      data_drive_active = 0;
      write_ctrl_data(CTRL_DATA_DRIVE_ACTIVE, 0);
      kfree(control_obj);
      return data_major_num;
    }

    else if (control_obj->command == STOP_FILE_COMMAND)
    {
      if (data_drive_active)
      {
        stop_signal = 1;
        write_ctrl_data(CTRL_STOP_SIGNAL, 1);
        unregister_data_file();
        remove_proc_entry(data_device.device_name, NULL);
        data_drive_active = 0;
        write_ctrl_data(CTRL_DATA_DRIVE_ACTIVE, 0);
        process_id = 0;
        kfree(control_obj);
        return 0;
      }
    }

    else if (control_obj->command == ACTIVATE_DRIVE_COMMAND)
    {
      if (!data_drive_active)
      {
        data_drive_active = 1;
        write_ctrl_data(CTRL_DATA_DRIVE_ACTIVE, 1);
        kfree(control_obj);
        return 0;
      }
    }

    else if (control_obj->command == READ_MEMORY_DRIVE_COMMAND)
    {
      unsigned char * mapbuffer;
      //long long physmap;
      //long long busmap;
      //physmap = phys_to_virt(control_obj->read_timeout);
      //busmap = bus_to_virt(control_obj->read_timeout);
      printk(KERN_INFO "hddsuperdebug: memory location %llx size %llx\n", control_obj->read_timeout, control_obj->request_timeout);    //debug
      //printk(KERN_INFO "hddsuperdebug: physmap %llx\n", physmap);    //debug
      //printk(KERN_INFO "hddsuperdebug: busmap %llx\n", busmap);    //debug
      mapbuffer = phys_to_virt(control_obj->read_timeout);
      //mapbuffer = bus_to_virt(control_obj->read_timeout);
      memcpy(control_obj->buffer, mapbuffer, control_obj->request_timeout);
      mapbuffer = NULL;
      if (copy_to_user((void *)arg, control_obj, sizeof(struct control_data)))
      {
        printk (KERN_WARNING "hddsuperdrive: failed to copy user data\n");
      }
      kfree(control_obj);
      return 0;
    }

    else
    {
      printk(KERN_INFO "%s: unrecognized command %lld\n", filename_ioctl, control_obj->command);
      kfree(control_obj);
      return -EINVAL;
    }

  }
  else
  {
    printk(KERN_INFO "%s: rejected ioctl cmd=0x%x arg=0x%lx\n", filename_ioctl, cmd, arg);
    return -EINVAL;
  }

  return 0;
}







struct mmap_info
{
  char *data;
  int reference;
};



// main control memory mapping
static void mmap_open_m(struct vm_area_struct *vma)
{
  struct mmap_info *info = (struct mmap_info *)vma->vm_private_data;
  info->reference++;
  //printk("mmap_open\n");
}
static void mmap_close_m(struct vm_area_struct *vma)
{
  struct mmap_info *info = (struct mmap_info *)vma->vm_private_data;
  info->reference--;
  //printk("mmap_close\n");
}
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,11,0)
static int mmap_fault_m(struct vm_area_struct *vma, struct vm_fault *vmf)
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(5,3,0)
static vm_fault_t mmap_fault_m(struct vm_fault *vmf)
#else
static int mmap_fault_m(struct vm_fault *vmf)
#endif
{
  struct page *page;
  struct mmap_info *info;
  #if LINUX_VERSION_CODE < KERNEL_VERSION(4,11,0)
  info = (struct mmap_info *)vma->vm_private_data;
  #else
  info = (struct mmap_info *)vmf->vma->vm_private_data;
  #endif
  if (!info->data)
  {
    printk("No data\n");
    return 0;
  }
  page = virt_to_page(info->data + (vmf->pgoff*PAGE_SIZE));
  get_page(page);
  vmf->page = page;
  //printk("mmap_fault\n");
  return 0;
}
static struct vm_operations_struct mmap_vm_ops_m =
{
  .open =     mmap_open_m,
  .close =    mmap_close_m,
  .fault =    mmap_fault_m,
};
static int op_mmap_m(struct file *filp, struct vm_area_struct *vma)
{
  vma->vm_ops = &mmap_vm_ops_m;
  vma->vm_flags |= VM_RESERVED;
  vma->vm_private_data = filp->private_data;
  mmap_open_m(vma);
  //printk("op_mmap\n");
  return 0;
}
static int mmapfop_close_m(struct inode *inode, struct file *filp)
{
  struct mmap_info *info = filp->private_data;
  main_buffer = NULL;
  free_page((unsigned long)info->data);
  kfree(info);
  filp->private_data = NULL;
  //printk("mmapfop_close\n");
  return 0;
}
static int mmapfop_open_m(struct inode *inode, struct file *filp)
{
  struct mmap_info *info = kmalloc(sizeof(struct mmap_info), GFP_KERNEL);
  info->data = (char *)__get_free_pages(GFP_DMA32, 5);    // make sure to match with buffer size
  if (!info->data)
  {
    return -ENOMEM;
  }
  memset(info->data, 0, DRIVER_CONTROL_BUFFER_SIZE);
  main_buffer = info->data;
  filp->private_data = info;
  //printk("mmapfop_open\n");
  return 0;
}







// transfer buffer memory mapping
static void mmap_open_tb(struct vm_area_struct *vma)
{
  struct mmap_info *info = (struct mmap_info *)vma->vm_private_data;
  info->reference++;
  //printk("mmap_open\n");
}
static void mmap_close_tb(struct vm_area_struct *vma)
{
  struct mmap_info *info = (struct mmap_info *)vma->vm_private_data;
  info->reference--;
  //printk("mmap_close\n");
}
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,11,0)
static int mmap_fault_tb(struct vm_area_struct *vma, struct vm_fault *vmf)
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(5,3,0)
static vm_fault_t mmap_fault_tb(struct vm_fault *vmf)
#else
static int mmap_fault_tb(struct vm_fault *vmf)
#endif
{
  struct page *page;
  struct mmap_info *info;
  #if LINUX_VERSION_CODE < KERNEL_VERSION(4,11,0)
  info = (struct mmap_info *)vma->vm_private_data;
  #else
  info = (struct mmap_info *)vmf->vma->vm_private_data;
  #endif
  if (!info->data)
  {
    printk("No data\n");
    return 0;
  }
  page = virt_to_page(info->data + (vmf->pgoff*PAGE_SIZE));
  get_page(page);
  vmf->page = page;
  //printk("mmap_fault\n");
  return 0;
}
static struct vm_operations_struct mmap_vm_ops_tb =
{
  .open =     mmap_open_tb,
  .close =    mmap_close_tb,
  .fault =    mmap_fault_tb,
};
static int op_mmap_tb(struct file *filp, struct vm_area_struct *vma)
{
  vma->vm_ops = &mmap_vm_ops_tb;
  vma->vm_flags |= VM_RESERVED;
  vma->vm_private_data = filp->private_data;
  mmap_open_tb(vma);
  //printk("op_mmap\n");
  return 0;
}
static int mmapfop_close_tb(struct inode *inode, struct file *filp)
{
  struct mmap_info *info = filp->private_data;
  transfer_buffer = NULL;
  free_page((unsigned long)info->data);
  kfree(info);
  filp->private_data = NULL;
  //printk("mmapfop_close\n");
  return 0;
}
static int mmapfop_open_tb(struct inode *inode, struct file *filp)
{
  struct mmap_info *info = kmalloc(sizeof(struct mmap_info), GFP_KERNEL);
  info->data = (char *)__get_free_pages(GFP_DMA32, 10);    // make sure to match with buffer size
  if (!info->data)
  {
    return -ENOMEM;
  }
  memset(info->data, 0, DRIVER_TRANSFER_BUFFER_SIZE);
  transfer_buffer = info->data;
  filp->private_data = info;
  //printk("mmapfop_open\n");
  return 0;
}







// main data buffer memory mapping
static void mmap_open_mdb(struct vm_area_struct *vma)
{
  struct mmap_info *info = (struct mmap_info *)vma->vm_private_data;
  info->reference++;
  //printk("mmap_open\n");
}
static void mmap_close_mdb(struct vm_area_struct *vma)
{
  struct mmap_info *info = (struct mmap_info *)vma->vm_private_data;
  info->reference--;
  //printk("mmap_close\n");
}
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,11,0)
static int mmap_fault_mdb(struct vm_area_struct *vma, struct vm_fault *vmf)
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(5,3,0)
static vm_fault_t mmap_fault_mdb(struct vm_fault *vmf)
#else
static int mmap_fault_mdb(struct vm_fault *vmf)
#endif
{
  struct page *page;
  struct mmap_info *info;
  #if LINUX_VERSION_CODE < KERNEL_VERSION(4,11,0)
  info = (struct mmap_info *)vma->vm_private_data;
  #else
  info = (struct mmap_info *)vmf->vma->vm_private_data;
  #endif
  if (!info->data)
  {
    printk("No data\n");
    return 0;
  }
  page = virt_to_page(info->data + (vmf->pgoff*PAGE_SIZE));
  get_page(page);
  vmf->page = page;
  //printk("mmap_fault\n");
  return 0;
}
static struct vm_operations_struct mmap_vm_ops_mdb =
{
  .open =     mmap_open_mdb,
  .close =    mmap_close_mdb,
  .fault =    mmap_fault_mdb,
};
int op_mmap_mdb(struct file *filp, struct vm_area_struct *vma)
{
  vma->vm_ops = &mmap_vm_ops_mdb;
  vma->vm_flags |= VM_RESERVED;
  vma->vm_private_data = filp->private_data;
  mmap_open_mdb(vma);
  //printk("op_mmap\n");
  return 0;
}
static int mmapfop_close_mdb(struct inode *inode, struct file *filp)
{
  struct mmap_info *info = filp->private_data;
  main_data_buffer = NULL;
  free_page((unsigned long)info->data);
  kfree(info);
  filp->private_data = NULL;
  //printk("mmapfop_close\n");
  return 0;
}
static int mmapfop_open_mdb(struct inode *inode, struct file *filp)
{
  struct mmap_info *info = kmalloc(sizeof(struct mmap_info), GFP_KERNEL);
  info->data = (char *)__get_free_pages(GFP_DMA32, 10);    // make sure to match with buffer size
  if (!info->data)
  {
    return -ENOMEM;
  }
  memset(info->data, 0, DRIVER_MAIN_DATA_BUFFER_SIZE);
  main_data_buffer = info->data;
  filp->private_data = info;
  //printk("mmapfop_open\n");
  return 0;
}







#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0)
static const struct proc_ops mmap_fops_m = {
  .proc_open = mmapfop_open_m,
  .proc_release = mmapfop_close_m,
  .proc_mmap = op_mmap_m
};
#else
static const struct file_operations mmap_fops_m = {
  .open = mmapfop_open_m,
  .release = mmapfop_close_m,
  .mmap = op_mmap_m,
  .owner = THIS_MODULE
};
#endif


#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0)
static const struct proc_ops mmap_fops_tb = {
  .proc_open = mmapfop_open_tb,
  .proc_release = mmapfop_close_tb,
  .proc_mmap = op_mmap_tb
};
#else
static const struct file_operations mmap_fops_tb = {
  .open = mmapfop_open_tb,
  .release = mmapfop_close_tb,
  .mmap = op_mmap_tb,
  .owner = THIS_MODULE
};
#endif


#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0)
static const struct proc_ops mmap_fops_mdb = {
  .proc_open = mmapfop_open_mdb,
  .proc_release = mmapfop_close_mdb,
  .proc_mmap = op_mmap_mdb
};
#else
static const struct file_operations mmap_fops_mdb = {
  .open = mmapfop_open_mdb,
  .release = mmapfop_close_mdb,
  .mmap = op_mmap_mdb,
  .owner = THIS_MODULE
};
#endif


#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0)
static const struct proc_ops ioctl_fops = {
  .proc_ioctl = process_ioctl
};
#else
static const struct file_operations ioctl_fops = {
  .unlocked_ioctl = process_ioctl,
  .owner = THIS_MODULE
};
#endif


static int __init initialize_driver(void)
{
  printk(KERN_INFO "hddsuperdrive version %s\n", DRIVER_VERSION);
  main_buffer = NULL;
  transfer_buffer = NULL;
  main_data_buffer = NULL;
  block_io_buffer = vmalloc(DRIVER_MAIN_DATA_BUFFER_SIZE);
  proc_create(filename_mmap_m, 0, NULL, &mmap_fops_m);
  proc_create(filename_mmap_tb, 0, NULL, &mmap_fops_tb);
  proc_create(filename_mmap_mdb, 0, NULL, &mmap_fops_mdb);
  proc_create(filename_ioctl, 0, NULL, &ioctl_fops);

  return 0;
}

static void __exit exit_driver(void)
{
  if (data_drive_active)
  {
    stop_signal = 1;
    unregister_data_drive();
    data_drive_active = 0;
  }
  vfree (block_io_buffer);

  remove_proc_entry(filename_ioctl, NULL);
  remove_proc_entry(filename_mmap_m, NULL);
  remove_proc_entry(filename_mmap_tb, NULL);
  remove_proc_entry(filename_mmap_mdb, NULL);
}

module_init(initialize_driver);
module_exit(exit_driver);
