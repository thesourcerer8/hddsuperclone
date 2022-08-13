/*
Copyright (C) 2016 Scott Dwyer

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef __linux__
#include <stdio.h>
#include <stdlib.h>
int main()
{
  fprintf (stderr, "This program only works on Linux\n");
  exit (0);
}
#else

#define _GNU_SOURCE
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/ioctl.h>
#include <scsi/sg.h>
#include "ddru_diskutility_help.h"

char *title = "ddru_diskutility";
char *version_number = "1.3 20141005";
int copyright_year = 2014;

int cleanup(void);

// Function to handle ctrl-c
void signal_callback_handler(int signum)
{
  printf("Terminated by user\n");
  cleanup();
  exit(signum);
}

// time functions
/* Return 1 if the difference is negative, otherwise 0.  */
int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
  long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
  result->tv_sec = diff / 1000000;
  result->tv_usec = diff % 1000000;

  return (diff<0);
}

void timeval_print(struct timeval *tv)
{
  char buffer[30];
  time_t curtime;

  // printf("%ld.%06ld", tv->tv_sec, tv->tv_usec);
  curtime = tv->tv_sec;
  strftime(buffer, 30, "%m-%d-%Y  %T", localtime(&curtime));
  printf("%s.%06ld\n", buffer, tv->tv_usec);
}


time_t start_time, end_time;

void help(void);
void version(void);

int i;
int n;
long long return_value;
bool direct = false;
bool sgpt = false;
bool ata = false;
bool read_error = false;
bool doata = false;
bool doinquiry = false;
bool doatainquiry = false;
bool doreadsector = false;
bool doreadlong28 = false;
bool doreadlong48 = false;
bool doreadlongbest = false;
bool dobinary = false;
bool error_recovery_control = false;
bool long_sector_access = false;
int verbosity = 0;

long long total_size = 0;
unsigned long long input_offset = 0;
long long input_position = 0;
long long sector_size = 512;
long long read_size = 0;
long long data_read = 0;
long long cluster_size = 0;
unsigned long long read_sector = 0;
unsigned long long error_timer = 0;
long long sector_start = 0;
long long sector_count = 0;
unsigned long long readlong_count = 0;

// stuff for sgpt
unsigned char sense_buf[64];
unsigned char scsi_cmd[16];
unsigned char sense_key = 0;
unsigned char asc = 0;
unsigned char ascq = 0;
int ioctl_ret;
struct sg_io_hdr io_hdr;
unsigned int buffer_align;
void* buffer;
int buffer_size = 0;
int ecc_size = 0;
int command_length = 0;

unsigned char scsi_inquiry_buffer[256];
unsigned char ata_inquiry_buffer[512];

long last_second = 0;
long last_usecond = 0;

int input_fd;
int output_fd;
char *source_disk;
char *destination_disk;
//FILE *read_rates;

int inquiry(void);

int scsi_inquiry(void);

int ata_inquiry(void);

int readsector(void);

int readlong28(void);

int readlong48(void);

int readlongbest(void);

int prepare_cdb(void);

int post_ioctl(void);

int main (int argc, char **argv)
{
  // Register ctrl-c signal and signal handler
  signal(SIGINT, signal_callback_handler);

  // begin processing command line arguments
  int command_line_argument;
  int arguments_required;	// required number of non-option arguments
  bool command_line_error = false;	// initialize error to false

  while (1)
  {
    static struct option long_options[] =
    {
      {"help",    	no_argument,	0, 'h'},
      {"version", 	no_argument,	0, 'v'},
      {"verbose",  	no_argument,	0, 'V'},
      {"inquiry",  	no_argument,	0, 'I'},
      {"binary",  	no_argument,	0, 'B'},
      {"direct",  	no_argument,	0, 'd'},
      {"sgpt",  	no_argument,	0, 'p'},
      {"sgptata",  	no_argument,	0, 'P'},
      {"readsector",    	required_argument, 0, 'r'},
      {"readlongbest",    	required_argument, 0, 'g'},
      {"readlong28",    	required_argument, 0, 'l'},
      {"readlong48",    	required_argument, 0, 'L'},
      {"sectorsize",    	required_argument, 0, 'b'},
      {"inputoffset",    	required_argument, 0, 'i'},
      {"readsize",    	required_argument, 0, 's'},
      {"clustersize",    	required_argument, 0, 'c'},
      {0, 0, 0, 0}
    };
    // getopt_long stores the option index here.
    int option_index = 0;

    command_line_argument = getopt_long (argc, argv, "hvVIBdpPr:g:l:L:b:i:s:c:",
					 long_options, &option_index);

    // Detect the end of the options.
    if (command_line_argument == -1)
      break;

    switch (command_line_argument)
    {
      case 'h':
	help();
	exit(0);

      case 'v':
	version();
	exit(0);

      case 'r':
	read_sector = strtoull(optarg, NULL, 0);
	doreadsector = true;
	break;

      case 'g':
	readlong_count = strtoul(optarg, NULL, 0);
	doreadlongbest = true;
	sgpt = true;
	break;

      case 'l':
	read_sector = strtoull(optarg, NULL, 0);
	doreadlong28 = true;
	sgpt = true;
	break;

      case 'L':
	read_sector = strtoull(optarg, NULL, 0);
	doreadlong48 = true;
	sgpt = true;
	break;

      case 'b':
	sector_size = strtoul(optarg, NULL, 0);
	break;

      case 'i':
	input_offset = strtoull(optarg, NULL, 0);
	break;

      case 's':
	read_size = strtoul(optarg, NULL, 0);
	break;

      case 'I':
	sgpt = true;
	doinquiry = true;
	break;

      case 'B':
	dobinary = true;
	break;

      case 'd':
	direct = true;
	break;

      case 'p':
	sgpt = true;
	break;

      case 'P':
	sgpt = true;
	doata = true;
	break;

      case 'V':
	verbosity++;
	break;

      case 'c':
	cluster_size = strtoul(optarg, NULL, 0);
	break;

      case '?':
	// getopt_long already printed an error message.
	command_line_error = true;
	break;

      default:
	fprintf (stderr, "catastrophic command line processing error\n");
	exit (1);
    }
  }

  arguments_required = 1;
  if ((argc - optind) != arguments_required)
  {
    command_line_error = true;
    if ((argc - optind) < arguments_required)
      fprintf (stderr, "Error: too few arguments\n");
    if ((argc - optind) > arguments_required)
      fprintf (stderr, "Error: too many arguments\n");
  }

  // get remaining arguments that are not options
  source_disk = argv[optind++];

  // exit on command line error
  if (command_line_error)
  {
    fprintf (stderr, "ERROR: command line input error\n");
    fprintf (stderr, "Usage: ddru_diskutility sourcedrive (options)\n");
    exit (1);
  }

  // end of command line processing

  fprintf (stderr, "%s %s\n", title, version_number);

  // open source input
  if (direct)
  {
    input_fd = open (source_disk, O_RDONLY | O_DIRECT);
    if (input_fd == -1)
    {
      perror ("open1");
      exit (2);
    }
  }
  else
  {
    input_fd = open (source_disk, O_RDONLY);
    if (input_fd == -1)
    {
      perror ("open2");
      exit (2);
    }
  }

  // get total size of input
  total_size = lseek( input_fd, 0, SEEK_END );
  if (total_size == -1)
  {
    perror ("lseek1");
    exit (2);
  }
  // set start position back to begining
  return_value = lseek( input_fd, 0, SEEK_SET );
  if (return_value == -1)
  {
    perror ("lseek2");
    exit (2);
  }

  if (input_offset%sector_size)
  {
    fprintf (stderr, "Error, input offset not a multiple of sector size\n");
    exit (1);
  }
  if (read_size%sector_size)
  {
    fprintf (stderr, "Error, read size not a multiple of sector size\n");
    exit (1);
  }
  if (read_size !=0 && cluster_size !=0)
  {
    fprintf (stderr, "Error, you can not supply both clustersize and readsize\n");
    exit (1);
  }
  if (read_size == 0 && cluster_size == 0)
  {
    cluster_size = 1;
  }

  sector_start = read_sector + (input_offset/sector_size);
  sector_count = cluster_size + (read_size/sector_size);




  // if passthrough then get size of buffer limit
  // also works to tell if whole drive or partition
  if (sgpt)
  {
    // extract last part of device name
    int i = 0;
    while (source_disk[i] != '\0')
      i++;
    while (source_disk[i] != '/')
    {
      i--;
      if (i < 0)
        break;
    }
    char device_name[20];
    i++;
    int n = 0;
    while (source_disk[i] != '\0')
    {
      device_name[n] = source_disk[i];
      n++;
      i++;
    }
    device_name[n] = '\0';

    // whether scsi or ata we need to get the buffer limit that the kernel uses
    // and we can also use this to find out if whole disk or partition
    char file_name[80] = "/sys/block/\0";
    strcat (file_name, device_name);
    strcat (file_name, "/queue/max_sectors_kb");

    FILE *file_pointer;
    file_pointer = fopen(file_name, "r");
    if (file_pointer == NULL)
    {
      fprintf(stderr, "Cannot open %s for reading (%s)\n", file_name, strerror(errno));
      fprintf(stderr, "Error! Input must be whole disk with passthrough option. Aborting...\n" );
      exit (1);
    }
    else
    {
      char line[20];
      while (fgets(line, sizeof line, file_pointer))
        break;

      long long max_sectors_kb = strtoull(line, NULL, 0);
      if (sector_size * sector_count > max_sectors_kb*1024)
      {
        fprintf(stderr, "Error! The reported cluster limit is %lld and you chose %lld. Aborting...\n", (max_sectors_kb*1024)/sector_size, sector_count);
        exit (1);
      }
      fclose(file_pointer);
    }
  }


  // limit sector count to 256 to try to limit screen output to a somewhat resonable level
  if (sector_count > 256)
  {
    fprintf (stderr, "Error, clustersize cannot be greater than 256\n");
    exit (1);
  }

  if (verbosity > 0)
  {
    printf ("total size of input device in bytes= %lld\n", total_size);
    printf ("total size of input device in sectors= %lld\n", total_size/sector_size);
    printf ("current position in bytes= %lld\n", sector_start*sector_size);
    printf ("current position in sectors= %lld\n", sector_start);
  }

  buffer_size = sector_size * sector_count;
  if (buffer_size < sysconf(_SC_PAGESIZE))
  {
    buffer_size = sysconf(_SC_PAGESIZE);
  }

  // create a buffer that is memory aligned with the pagesize
  buffer_align = sysconf(_SC_PAGESIZE);
  if (posix_memalign(&buffer, buffer_align, buffer_size))
  { perror("posix_memalign failed"); exit (EXIT_FAILURE); }

  buffer_size = sector_size * sector_count;

  // perform initial inquiry
  if (sgpt)
  {
    int inquiry_return;
    inquiry_return = scsi_inquiry();
    if (inquiry_return == 1)
    {
      ata_inquiry();
      ata = true;
    }
  }

  if (doinquiry)
  {
    inquiry();
    cleanup();
    exit (0);
  }

  if (doreadsector)
  {
    readsector();
    cleanup();
    exit (0);
  }

  if (doreadlong28)
  {
    if (doreadlongbest)
      readlongbest();
    else
      readlong28();
    cleanup();
    exit (0);
  }

  if (doreadlong48)
  {
    if (doreadlongbest)
      readlongbest();
    else
      readlong48();
    cleanup();
    exit (0);
  }


  cleanup();
  exit (0);
}
// end of main
//***********************************************************************



// function cleanup
int cleanup(void)
{
  free(buffer);
  close (input_fd);
  close (output_fd);
  return (0);
}




//function to process inquiry into output
int inquiry(void)
{
  int i;
  // scsi inquiry data
  fprintf (stdout, "SCSI inquiry results:\n");
  fprintf (stdout, "  Vendor ID= ");
  for (i = 8; i < 16; i++)
  {
    fprintf (stdout, "%c", isprint(scsi_inquiry_buffer[i]) ? scsi_inquiry_buffer[i] : '.');
  }
  fprintf (stdout, "\n");

  fprintf (stdout, "  Product ID= ");
  for (i = 16; i < 32; i++)
  {
    fprintf (stdout, "%c", isprint(scsi_inquiry_buffer[i]) ? scsi_inquiry_buffer[i] : '.');
  }
  fprintf (stdout, "\n");

  fprintf (stdout, "  Product Revision= ");
  for (i = 32; i < 36; i++)
  {
    fprintf (stdout, "%c", isprint(scsi_inquiry_buffer[i]) ? scsi_inquiry_buffer[i] : '.');
  }
  fprintf (stdout, "\n");

  fprintf (stdout, "  Serial Number= ");
  for (i = 36; i < 44; i++)
  {
    fprintf (stdout, "%c", isprint(scsi_inquiry_buffer[i]) ? scsi_inquiry_buffer[i] : '.');
  }
  fprintf (stdout, "\n");

  // ata inquiry data
  if (ata)
  {
    fprintf (stdout, "ATA indentify device results:\n");
    fprintf (stdout, "  Serial Number= ");
    for (i = 20; i < 40; i += 2)
    {
      fprintf (stdout, "%c", isprint(ata_inquiry_buffer[i+1]) ? ata_inquiry_buffer[i+1] : '.');
      fprintf (stdout, "%c", isprint(ata_inquiry_buffer[i]) ? ata_inquiry_buffer[i] : '.');
    }
    fprintf (stdout, "\n");

    fprintf (stdout, "  Firmware Revision= ");
    for (i = 46; i < 52; i += 2)
    {
      fprintf (stdout, "%c", isprint(ata_inquiry_buffer[i+1]) ? ata_inquiry_buffer[i+1] : '.');
      fprintf (stdout, "%c", isprint(ata_inquiry_buffer[i]) ? ata_inquiry_buffer[i] : '.');
    }
    fprintf (stdout, "\n");

    fprintf (stdout, "  Model Number= ");
    for (i = 54; i < 92; i += 2)
    {
      fprintf (stdout, "%c", isprint(ata_inquiry_buffer[i+1]) ? ata_inquiry_buffer[i+1] : '.');
      fprintf (stdout, "%c", isprint(ata_inquiry_buffer[i]) ? ata_inquiry_buffer[i] : '.');
    }
    fprintf (stdout, "\n");

    fprintf (stdout, "  Total Sectors= 0x");
    for (i = 123; i > 119; i--)
    {
      fprintf (stdout, "%02X", ata_inquiry_buffer[i]);
    }
    fprintf (stdout, "\n");

    fprintf (stdout, "  Total Sectors 48bit= 0x");
    for (i = 207; i > 199; i--)
    {
      fprintf (stdout, "%02X", ata_inquiry_buffer[i]);
    }
    fprintf (stdout, "\n");
/*
    fprintf (stdout, "  2x Physical Sectors per Logical (EXPERIMENTAL)= %d\n", (ata_inquiry_buffer[213] & 0x20));

    fprintf (stdout, "  Logical Sector Size in Words (EXPERIMENTAL)= 0x");
    for (i = 237; i > 233; i--)
    {
      fprintf (stdout, "%02X", ata_inquiry_buffer[i]);
    }
    fprintf (stdout, "\n");
*/
    fprintf (stdout, "  SCT Command Transport= 0x");
    for (i = 413; i > 411; i--)
    {
      fprintf (stdout, "%02X", ata_inquiry_buffer[i]);
    }
    fprintf (stdout, "\n");

    fprintf (stdout, "  SCT Error Recovery Control= %s\n", error_recovery_control ? "yes" : "no");
    fprintf (stdout, "  SCT Long Sector Access= %s\n", long_sector_access ? "yes" : "no");
  }

  return (0);
}


// funtion for scsi inquiry
int scsi_inquiry(void)
{
  scsi_cmd[0] = 0x12; // inquiry command
  scsi_cmd[1] = 0;
  scsi_cmd[2] = 0;
  scsi_cmd[3] = 0;
  scsi_cmd[4] = 0x2C; // size of data requested back
  scsi_cmd[5] = 0;

  buffer_size = 256;
  command_length = 6;
  prepare_cdb();
  io_hdr.dxfer_direction = SG_DXFER_FROM_DEV;

  if (verbosity > 0)
    fprintf (stdout, "Performing SCSI Inquiry command\n");

  errno = 0;
  ioctl_ret = ioctl(input_fd, SG_IO, &io_hdr);
  if (ioctl_ret < 0)
  {
    perror("SCSI inquiry error");
    exit (1);
  }
  post_ioctl();

  // copy to external buffer for later use
  unsigned char *c;
  for (i = 0; i < scsi_cmd[4]; i++)
  {
    c = (unsigned char *)buffer+i;
    scsi_inquiry_buffer[i] = *c;
  }

  if (strncmp((char *)buffer+8, "ATA     ", 8) == 0)
  {
    if (verbosity > 0)
      fprintf (stdout, "ATA device detected\n");
    return_value = 1;
  }
  else
  {
    if (verbosity > 0)
      fprintf (stdout, "Device is not ATA\n");
    return_value = 0;
  }

  return (return_value);
}



// function to do ata inquiry
int ata_inquiry(void)
{
  buffer_size = 512;
  command_length = 12;

  scsi_cmd[0] = 0xA1; // ata passthough 12 command
  scsi_cmd[1] = 0x08; // set protocol to 4, PIO Data-In
  scsi_cmd[2] = (1<<5)+(1<<3); // set check condition, direction (1 from device, 0 to device)
  scsi_cmd[3] = 0;
  scsi_cmd[4] = 0;
  scsi_cmd[5] = 0;
  scsi_cmd[6] = 0;
  scsi_cmd[7] = 0;
  scsi_cmd[8] = 0;
  scsi_cmd[9] = 0xEC; // identify device command
  scsi_cmd[10] = 0;
  scsi_cmd[11] = 0;

  prepare_cdb();
  io_hdr.dxfer_direction = SG_DXFER_FROM_DEV;

  if (verbosity > 0)
    fprintf (stdout, "Performing ATA Identify Device command\n");
  errno = 0;
  ioctl_ret = ioctl(input_fd, SG_IO, &io_hdr);
  if (ioctl_ret < 0)
  {
    perror("ATA inquiry error");
    exit (1);
  }
  doatainquiry = true;
  post_ioctl();

  // copy to external buffer for later use
  unsigned char *c;
  for (i = 0; i < buffer_size; i++)
  {
    c = (unsigned char *)buffer+i;
    ata_inquiry_buffer[i] = *c;
  }

  // set some flags for later use
  error_recovery_control = ata_inquiry_buffer[412] & 8;
  long_sector_access = ata_inquiry_buffer[412] & 2;

  return (0);
}




// function to read sector(s)
int readsector(void)
{
  // if ata passthough was chosen check if device is ata
  if (doata)
  {
    if (ata != true)
    {
      fprintf (stderr, "Unable to perform funtion, drive not ATA\n");
      exit (1);
    }
  }

  buffer_size = sector_size * sector_count;
  ssize_t ret_in;
  input_position = sector_start * sector_size;
  read_size = sector_size * sector_count;

  read_error = false;

  // if passthough
  if (sgpt)
  {
    command_length = 16;

    // if disk is ata and ata option also chosen then do ata commands
    if (ata && doata)
    {
      scsi_cmd[0] = 0x85; // ata 16 passthough
      scsi_cmd[1] = (0x04<<1)+1; // set protocol to 4 PIO Data-In, and set extended bit
      scsi_cmd[2] = (1<<5)+(1<<3)+2; // set check condition, direction (1 from device, 0 to device), and length to sector field
      scsi_cmd[3] = 0;
      scsi_cmd[4] = 0;
      scsi_cmd[5] = (unsigned char)((sector_count >> 8) & 0xff);
      scsi_cmd[6] = (unsigned char)(sector_count & 0xff);
      scsi_cmd[7] = (unsigned char)((sector_start >> 24) & 0xff);
      scsi_cmd[8] = (unsigned char)(sector_start & 0xff);
      scsi_cmd[9] = (unsigned char)((sector_start >> 32) & 0xff);
      scsi_cmd[10] = (unsigned char)((sector_start >> 8) & 0xff);
      scsi_cmd[11] = (unsigned char)((sector_start >> 40) & 0xff);
      scsi_cmd[12] = (unsigned char)((sector_start >> 16) & 0xff);
      scsi_cmd[13] = (1<<7) + (1<<6) + (1<<5) + (unsigned char)((sector_start >> 24) & 0xf);; // back-compat, set LBA bit, back-compat, high 4 bits of LBA
      if ((sector_start + (sector_count-1) > 0xFFFFFFF) || (sector_count-1 > 0xFF))
        scsi_cmd[14] = 0x24; // read sectors PIO ext
        else
          scsi_cmd[14] = 0x20; // read sectors PIO
          scsi_cmd[15] = 0;
    }

    // otherwise do scsi commands
    else
    {
      // if the highest LBA to be read is bigger than 32 bits then use read 16
      if (sector_start * sector_count > 0xFFFFFFFF)
	command_length = 16;
      else
	command_length = 10;

      switch (command_length)
      {
	case 10:
	  scsi_cmd[0] = 0x28; // read 10 command
	  scsi_cmd[1] = 0;
	  scsi_cmd[2] = (unsigned char)((sector_start >> 24) & 0xff);
	  scsi_cmd[3] = (unsigned char)((sector_start >> 16) & 0xff);
	  scsi_cmd[4] = (unsigned char)((sector_start>> 8) & 0xff);
	  scsi_cmd[5] = (unsigned char)(sector_start & 0xff);
	  scsi_cmd[6] = 0;
	  scsi_cmd[7] = (unsigned char)((sector_count >> 8) & 0xff);
	  scsi_cmd[8] = (unsigned char)(sector_count & 0xff);
	  scsi_cmd[9] = 0;
	  break;

	case 16:
	  scsi_cmd[0] = 0x88; // read 16 command
	  scsi_cmd[1] = 0;
	  scsi_cmd[2] = (unsigned char)((sector_start >> 56) & 0xff);
	  scsi_cmd[3] = (unsigned char)((sector_start >> 48) & 0xff);
	  scsi_cmd[4] = (unsigned char)((sector_start >> 40) & 0xff);
	  scsi_cmd[5] = (unsigned char)((sector_start >> 32) & 0xff);
	  scsi_cmd[6] = (unsigned char)((sector_start >> 24) & 0xff);
	  scsi_cmd[7] = (unsigned char)((sector_start >> 16) & 0xff);
	  scsi_cmd[8] = (unsigned char)((sector_start >> 8) & 0xff);
	  scsi_cmd[9] = (unsigned char)(sector_start & 0xff);
	  scsi_cmd[10] = (unsigned char)((sector_count >> 24) & 0xff);
	  scsi_cmd[11] = (unsigned char)((sector_count >> 16) & 0xff);
	  scsi_cmd[12] = (unsigned char)((sector_count >> 8) & 0xff);
	  scsi_cmd[13] = (unsigned char)(sector_count & 0xff);
	  scsi_cmd[14] = 0;
	  scsi_cmd[15] = 0;
	  break;

	default:
	  fprintf(stderr, "expected cdb size of 10 or 16 but got %d\n", command_length);
	  exit (1);
      }
    }

    // make sure buffer is filled with zeros before we start
    memset (buffer, 0, buffer_size);

    prepare_cdb();
    io_hdr.dxfer_direction = SG_DXFER_FROM_DEV;

    errno = 0;
    ioctl_ret = ioctl(input_fd, SG_IO, &io_hdr);
    if (ioctl_ret < 0)
    {
      perror("reading (SG_IO) on sg device, error");
      exit (1);
    }
    post_ioctl();

    if (errno != 0)
      read_error = true;

  }

  else // normal read
    {
      return_value = lseek( input_fd, input_position, SEEK_SET );
      if (return_value == -1)
      {
	perror ("lseek3");
	exit (2);
      }
      errno = 0;

      ret_in = read (input_fd, buffer, buffer_size);
      if (ret_in < buffer_size || errno != 0)
      {
	read_error = true;
	if (verbosity > 0)
	{
	  fprintf (stderr, "return read size=%d  ", (int)ret_in);
	  fprintf (stderr, "errno=%d\n", errno);
	  perror ("read error");
	}
      }
    }


    if (errno == 0)
    {
      fprintf (stdout, "sector=%lld\n", sector_start);
      fprintf (stdout, "count=%lld\n", sector_count);
      fprintf (stdout, "buffersize=%d\n", buffer_size);
      unsigned char *c;
      int i;
      for (i = 0; i < buffer_size; i+=16)
      {
	fprintf (stdout, "%llX: ", i+(sector_start*sector_size));
	int n;
	for (n=0; n < 16; n++)
	{
	  c = (unsigned char *)buffer+i+n;
	  if (dobinary)
	  {
            if (n == 8)
            fprintf (stdout, "\n%llX: ", i+n+(sector_start*sector_size));
	    int i;
	    for (i = 0; i < 8; i++)
	    {
	      if ((*c<<i) & 0x80)
		fprintf (stdout, "1");
	      else
		fprintf (stdout, "0");
	    }
	    fprintf (stdout, " ");
	  }
	  else
	    fprintf (stdout, "%02X ", *c);
	}
	for (n=0; n < 16; n++)
	{
	  c = (unsigned char *)buffer+i+n;
	  if (!dobinary)
	    fprintf (stdout, "%c", isprint(*c) ? *c : '.');
	}
	fprintf (stdout, "\n");
      }
      fprintf (stdout, "\n");
    }

    else
    {
      fprintf (stderr, "Read error, unable to read sector(s) from disk\n");
    }

    return (0);
}





// function to perform old 28 bit read long
int readlong28(void)
{
  if (ata != true)
  {
    fprintf (stderr, "Unable to perform funtion, drive not ATA\n");
    exit (1);
  }
  if ((sector_start*sector_size) > total_size)
    fprintf (stderr, "Warning, sector %lld is higher than reported maximum %lld\n", sector_start, total_size/sector_size);

  sector_count = 1;
  ecc_size = 52;
  read_size = 512;
  buffer_size = read_size + ecc_size;
  command_length = 16;

  scsi_cmd[0] = 0x85; // ata 16 passthough
  scsi_cmd[1] = (0x04<<1)+1; // set protocol to 4 PIO Data-In, and set extended bit
  scsi_cmd[2] = (1<<5)+(0<<3)+2; // set check condition, direction (1 from device, 0 to device), and length to sector field
  scsi_cmd[3] = 0;
  scsi_cmd[4] = 0;
  scsi_cmd[5] = (unsigned char)((sector_count >> 8) & 0xff);
  scsi_cmd[6] = (unsigned char)(sector_count & 0xff);
  scsi_cmd[7] = (unsigned char)((sector_start >> 24) & 0xff);
  scsi_cmd[8] = (unsigned char)(sector_start & 0xff);
  scsi_cmd[9] = (unsigned char)((sector_start >> 32) & 0xff);
  scsi_cmd[10] = (unsigned char)((sector_start >> 8) & 0xff);
  scsi_cmd[11] = (unsigned char)((sector_start >> 40) & 0xff);
  scsi_cmd[12] = (unsigned char)((sector_start >> 16) & 0xff);
  scsi_cmd[13] = (1<<7) + (1<<6) + (1<<5) + (unsigned char)((sector_start >> 24) & 0xf);; // back-compat, set LBA bit, back-compat, high 4 bits of LBA
  scsi_cmd[14] = 0x22; // read long PIO
  scsi_cmd[15] = 0;

  // make sure buffer is filled with zeros before we start
  memset (buffer, 0, buffer_size);

  prepare_cdb();
  io_hdr.dxfer_direction = SG_DXFER_FROM_DEV;

  errno = 0;
  ioctl_ret = ioctl(input_fd, SG_IO, &io_hdr);
  if (ioctl_ret < 0)
  {
    perror("reading (SG_IO) on sg device, error");
    exit (1);
  }
  post_ioctl();

  if (errno == 0)
  {
    fprintf (stdout, "sector=%lld\n", sector_start);
    fprintf (stdout, "count=%lld\n", sector_count);
    fprintf (stdout, "Data size=%lld\n", read_size);
    fprintf (stdout, "ECC size=%d\n", ecc_size);
    unsigned char *c;
    int i;
    for (i = 0; i < read_size; i+=16)
    {
      fprintf (stdout, "%d: ", i);
      int n;
      for (n=0; n < 16; n++)
      {
	c = (unsigned char *)buffer+i+n;
	if (i+n < read_size)
	{
	  if (dobinary)
	  {
            if (n == 8)
              fprintf (stdout, "\n%d: ", i+n);
	    int i;
	    for (i = 0; i < 8; i++)
	    {
	      if ((*c<<i) & 0x80)
		fprintf (stdout, "1");
	      else
		fprintf (stdout, "0");
	    }
	    fprintf (stdout, " ");
	  }
	  else
	    fprintf (stdout, "%02X ", *c);
	}
	else
	  fprintf (stdout, "   ");
      }
      for (n=0; n < 16; n++)
      {
	c = (unsigned char *)buffer+i+n;
	if (i+n < read_size)
	{
	  if (!dobinary)
	    fprintf (stdout, "%c", isprint(*c) ? *c : '.');
	}
      }
      fprintf (stdout, "\n");
    }
    fprintf (stdout, "\n");

    for (i = read_size; i < buffer_size; i+=16)
    {
      fprintf (stdout, "%d: ", i);
      int n;
      for (n=0; n < 16; n++)
      {
	c = (unsigned char *)buffer+i+n;
	if (i+n < buffer_size)
	  {
	  if (dobinary)
	  {
            if (n == 8)
              fprintf (stdout, "\n%d: ", i+n);
	    int i;
	    for (i = 0; i < 8; i++)
	    {
	      if ((*c<<i) & 0x80)
		fprintf (stdout, "1");
	      else
		fprintf (stdout, "0");
	    }
	    fprintf (stdout, " ");
	  }
	  else
	    fprintf (stdout, "%02X ", *c);
	}
	else
	  fprintf (stdout, "   ");
      }
      for (n=0; n < 16; n++)
      {
	c = (unsigned char *)buffer+i+n;
	if (i+n < buffer_size)
	{
	  if (!dobinary)
	    fprintf (stdout, "%c", isprint(*c) ? *c : '.');
	}
      }
      fprintf (stdout, "\n");
    }
    fprintf (stdout, "\n");
  }

  else
  {
    fprintf (stderr, "Read error, unable to read sector from disk\n");
  }

  return (0);
}



// function to perform 48 bit read long
int readlong48(void)
{
  if (ata != true)
  {
    fprintf (stderr, "Unable to perform funtion, drive not ATA\n");
    exit (1);
  }

  if (long_sector_access != true)
  {
    fprintf (stderr, "Unable to perform funtion, drive not capable of long sector access\n");
    exit (1);
  }

  if ((sector_start*sector_size) > total_size)
    fprintf (stderr, "Warning, sector %lld is higher than reported maximum %lld\n", sector_start, total_size/sector_size);

  buffer_size = 512;
  command_length = 16;

  scsi_cmd[0] = 0x85; // ata 16 passthough
  scsi_cmd[1] = (0x05<<1)+1; // set protocol to 5 PIO data out, and set extended bit
  scsi_cmd[2] = (1<<5)+(0<<3)+2; // set check condition, direction (1 from device, 0 to device), and length to sector field
  scsi_cmd[3] = 0; // 00H feature high
  scsi_cmd[4] = 0; // 00L feature low
  scsi_cmd[5] = 0; // 01H block count high
  scsi_cmd[6] = 1; // 01L block count low
  scsi_cmd[7] = 0; // 03MH
  scsi_cmd[8] = 0xE0; // 02LL  SCT page E0
  scsi_cmd[9] = 0; // 04HL
  scsi_cmd[10] = 0; // 02LH
  scsi_cmd[11] = 0; // 04HH
  scsi_cmd[12] = 0; // 03ML
  scsi_cmd[13] = 0; // 05H device
  scsi_cmd[14] = 0x3F; // 05L command write log ext
  scsi_cmd[15] = 0;

  // make sure buffer is filled with zeros before we start
  memset (buffer, 0, buffer_size);

  unsigned char c[12];

  c[0] = 1; // action code low, read/write long
  c[1] = 0; // action code high
  c[2] = 0x01; // function code low, read long
  c[3] = 0; // function code high
  c[4] = (unsigned char)(sector_start & 0xff);
  c[5] = (unsigned char)((sector_start >> 8) & 0xff);
  c[6] = (unsigned char)((sector_start >> 16) & 0xff);
  c[7] = (unsigned char)((sector_start >> 24) & 0xff);
  c[8] = (unsigned char)((sector_start >> 32) & 0xff);
  c[9] = (unsigned char)((sector_start >> 40) & 0xff);
  c[10] = (unsigned char)((sector_start >> 48) & 0xff);
  c[11] = (unsigned char)((sector_start >> 56) & 0xff);

  memcpy(buffer, &c, 12);

  if (verbosity > 1)
  {
    unsigned char *d;
    int i;
    fprintf (stdout, "SCT command: ");
    for (i = 0; i < 16; i++)
    {
      d = (unsigned char *)buffer+i;
      fprintf (stdout, "%02X ", *d);
    }
    fprintf (stdout, "\n");
  }

  prepare_cdb();
  io_hdr.dxfer_direction = SG_DXFER_TO_DEV;

  errno = 0;
  ioctl_ret = ioctl(input_fd, SG_IO, &io_hdr);
  if (ioctl_ret < 0)
  {
    perror("reading (SG_IO) on sg device, error");
    exit (1);
  }
  post_ioctl();

  buffer_size = 1024;

  scsi_cmd[1] = (0x04<<1)+1; // set protocol to 4 PIO data in, and set extended bit
  scsi_cmd[2] = (1<<5)+(1<<3)+2; // set check condition, direction (1 from device, 0 to device), and length to sector field
  scsi_cmd[3] = 0; // 00H feature high
  scsi_cmd[4] = 0; // 00L feature low
  scsi_cmd[5] = 0; // 01H block count high
  scsi_cmd[6] = 2; // 01L block count low
  scsi_cmd[7] = 0; // 03MH
  scsi_cmd[8] = 0xE1; // 02LL  SCT page E1
  scsi_cmd[9] = 0; // 04HL
  scsi_cmd[10] = 0; // 02LH
  scsi_cmd[11] = 0; // 04HH
  scsi_cmd[12] = 0; // 03ML
  scsi_cmd[13] = 0; // 05H device
  scsi_cmd[14] = 0x2F; // 05L command read log ext
  scsi_cmd[15] = 0;

  // make sure buffer is filled with zeros before we start
  memset (buffer, 0, buffer_size);

  prepare_cdb();
  io_hdr.dxfer_direction = SG_DXFER_FROM_DEV;

  errno = 0;
  ioctl_ret = ioctl(input_fd, SG_IO, &io_hdr);
  if (ioctl_ret < 0)
  {
    perror("reading (SG_IO) on sg device, error");
    exit (1);
  }
  post_ioctl();

  read_size = 512;
  ecc_size = io_hdr.sbp[8+5];
  if (errno == 0)
  {
    fprintf (stdout, "sector=%lld\n", sector_start);
    fprintf (stdout, "count=%lld\n", sector_count);
    fprintf (stdout, "Data size=%lld\n", read_size);
    fprintf (stdout, "ECC size=%d\n", ecc_size);
    unsigned char *c;
    int i;
    for (i = 0; i < read_size; i+=16)
    {
      fprintf (stdout, "%d: ", i);
      int n;
      for (n=0; n < 16; n++)
      {
	c = (unsigned char *)buffer+i+n;
	if (i+n < read_size)
	{
	  if (dobinary)
	  {
            if (n == 8)
              fprintf (stdout, "\n%d: ", i+n);
	    int i;
	    for (i = 0; i < 8; i++)
	    {
	      if ((*c<<i) & 0x80)
		fprintf (stdout, "1");
	      else
		fprintf (stdout, "0");
	    }
	    fprintf (stdout, " ");
	  }
	  else
	    fprintf (stdout, "%02X ", *c);
	}
	else
	  fprintf (stdout, "   ");
      }
      for (n=0; n < 16; n++)
      {
	c = (unsigned char *)buffer+i+n;
	if (i+n < read_size)
	{
	  if (!dobinary)
	    fprintf (stdout, "%c", isprint(*c) ? *c : '.');
	}
      }
      fprintf (stdout, "\n");
    }
    fprintf (stdout, "\n");

    for (i = read_size; i < read_size+ecc_size; i+=16)
    {
      fprintf (stdout, "%d: ", i);
      int n;
      for (n=0; n < 16; n++)
      {
	c = (unsigned char *)buffer+i+n;
	if (i+n < read_size+ecc_size)
	{
	  if (dobinary)
	  {
            if (n == 8)
              fprintf (stdout, "\n%d: ", i+n);
	    int i;
	    for (i = 0; i < 8; i++)
	    {
	      if ((*c<<i) & 0x80)
		fprintf (stdout, "1");
	      else
		fprintf (stdout, "0");
	    }
	    fprintf (stdout, " ");
	  }
	  else
	    fprintf (stdout, "%02X ", *c);
	}
	else
	  fprintf (stdout, "   ");
      }
      for (n=0; n < 16; n++)
      {
	c = (unsigned char *)buffer+i+n;
	if (i+n < read_size+ecc_size)
	{
	  if (!dobinary)
	    fprintf (stdout, "%c", isprint(*c) ? *c : '.');
	}
      }
      fprintf (stdout, "\n");
    }
    fprintf (stdout, "\n");
  }

  else
  {
    fprintf (stderr, "Read error, unable to read sector from disk\n");
  }


  return (0);
}



// function to prepare CDB
int prepare_cdb(void)
{
  if (verbosity > 1)
  {
    int i;
    fprintf (stdout, "\nscsi_cmd= ");
    for (i = 0; i < command_length; i++)
    {
      fprintf (stdout, "%02X ", scsi_cmd[i]);
    }
    fprintf (stdout, "\n");

    if (doata == true)
    {
      fprintf(stdout, "ATA command data:\n");
      fprintf(stdout, "features= %02X%02X\n", scsi_cmd[3], scsi_cmd[4]);
      fprintf(stdout, "count= %02X%02X\n", scsi_cmd[5], scsi_cmd[6]);
      fprintf(stdout, "LBAhigh= %02X%02X\n", scsi_cmd[11], scsi_cmd[9]);
      fprintf(stdout, "LBAmid= %02X%02X\n", scsi_cmd[7], scsi_cmd[12]);
      fprintf(stdout, "LBAlow= %02X%02X\n", scsi_cmd[10], scsi_cmd[8]);
      fprintf(stdout, "device= %02X\n", scsi_cmd[13]);
      fprintf(stdout, "command= %02X\n", scsi_cmd[14]);
    }
  }

  memset(&io_hdr, 0, sizeof(struct sg_io_hdr));
  io_hdr.interface_id = 'S';
	io_hdr.cmd_len = command_length;
	io_hdr.cmdp = scsi_cmd;
	io_hdr.dxfer_direction = SG_DXFER_FROM_DEV;
	io_hdr.dxfer_len = buffer_size;
	io_hdr.dxferp = buffer;
	io_hdr.mx_sb_len = sizeof(sense_buf);
	io_hdr.iovec_count = 0;
	io_hdr.sbp = sense_buf;
	io_hdr.timeout = 20000; // timeout in ms
	io_hdr.flags = SG_FLAG_DIRECT_IO;
	io_hdr.pack_id = 0;
	io_hdr.usr_ptr = 0;

	return (0);
}



// function to perform readlong multiple times and get best result
int readlongbest(void)
{
  if (readlong_count < 3)
    readlong_count = 3;
  if (readlong_count > 255)
    readlong_count = 255;
  unsigned char buff[512];
  unsigned char zero[512][8];
  unsigned char one[512][8];
  memset(&zero[0][0], 0, sizeof(zero));
  memset(&one[0][0], 0, sizeof(one));
  unsigned char *c;
  unsigned int count;
  int byte;
  int bit;
  for (count=0; count < readlong_count; count++)
  {
    fprintf (stdout, "\nRead number %d of %lld long reads-\n", count, readlong_count);
    if (doreadlong48)
      readlong48();
    else
      readlong28();
    for (byte=0; byte < 512; byte++)
    {
      c = (unsigned char *)buffer+byte;
      for (bit=0; bit < 8; bit++)
      {
	if (*c & 0x80)
	  one[byte][bit]++;
	else
	  zero[byte][bit]++;
	*c <<= 1;
      }
    }
  }

  for (byte=0; byte < 512; byte++)
  {
    *c = 0;
    for (bit=0; bit < 8; bit++)
    {
      if (one[byte][bit] > zero[byte][bit])
	*c += 1;
      if (bit < 7)
	*c <<= 1;
    }
    buff[byte] = *c;
  }

  fprintf (stdout, "\nBest average of %lld long reads-\n", readlong_count);
  read_size = 512;
  int i;
  for (i = 0; i < read_size; i+=16)
  {
    fprintf (stdout, "%d: ", i);
    int n;
    for (n=0; n < 16; n++)
    {
      *c = buff[i+n];
      if (i+n < read_size)
      {
	if (dobinary)
	{
          if (n == 8)
            fprintf (stdout, "\n%d: ", i+n);
	  int i;
	  for (i = 0; i < 8; i++)
	  {
	    if ((*c<<i) & 0x80)
	      fprintf (stdout, "1");
	    else
	      fprintf (stdout, "0");
	  }
	  fprintf (stdout, " ");
	}
	else
	  fprintf (stdout, "%02X ", *c);
      }
      else
	fprintf (stdout, "   ");
    }
    for (n=0; n < 16; n++)
    {
      *c = buff[i+n];
      if (i+n < read_size)
      {
	if (!dobinary)
	  fprintf (stdout, "%c", isprint(*c) ? *c : '.');
      }
    }
    fprintf (stdout, "\n");
  }
  fprintf (stdout, "\n");

  return (0);
}






// function post ioctl
int post_ioctl(void)
{
  // if there was sense data written then check for errors
  if (io_hdr.sb_len_wr != 0)
  {
    switch (io_hdr.sbp[0])
    {
      case 0x70:
      case 0x71:
	sense_key = io_hdr.sbp[2] & 0x0f;
	asc = io_hdr.sbp[12];
	ascq = io_hdr.sbp[13];
	break;

      case 0x72:
      case 0x73:
	sense_key = io_hdr.sbp[1] & 0x0f;
	asc = io_hdr.sbp[2];
	ascq = io_hdr.sbp[3];
	break;

      default:
	fprintf (stderr, "\nbad (SG_IO) sense buffer response code\n");
	exit (1);
    }

    if (sense_key != 0) // if not 0 then check further
      {
	if (sense_key != 1) // value of 1 means a recovered error so read was still good, else read failed
	      errno = 1;
      }

  }
  if (verbosity > 1)
  {
    fprintf(stdout, "status=%hhu, masked_status=%hhu, msg_status=%hhu, sb_len_wr=%hhu, host_status=%hu, driver_status=%hu, resid=%hhu, duration=%u, info=%u\n",
	    io_hdr.status,
	    io_hdr.masked_status,
	    io_hdr.msg_status,
	    io_hdr.sb_len_wr,
	    io_hdr.host_status,
	    io_hdr.driver_status,
	    io_hdr.resid,
	    io_hdr.duration,
	    io_hdr.info);

    // if sense data then show it also
    if (io_hdr.sb_len_wr != 0)
    {
      fprintf(stdout, "additional sense info: %02X %02X %02X\n", sense_key, asc, ascq);
      fprintf(stdout, "sense buffer, in hex:\n");
      unsigned int i;
      for (i = 0; i < io_hdr.sb_len_wr; i++)
	fprintf(stdout, "%02X ", io_hdr.sbp[i]);
      fprintf(stdout, "\n");
      if (doatainquiry == true)
      {
	int ata_shift = 8;
	fprintf(stdout, "ATA return data:\n");
	fprintf(stdout, "descriptor= %02X\n", io_hdr.sbp[ata_shift+0]);
	fprintf(stdout, "additional length= %02X\n", io_hdr.sbp[ata_shift+1]);
	fprintf(stdout, "extend= %02X\n", io_hdr.sbp[ata_shift+2]);
	fprintf(stdout, "error= %02X\n", io_hdr.sbp[ata_shift+3]);
	fprintf(stdout, "count= %02X%02X\n", io_hdr.sbp[ata_shift+4], io_hdr.sbp[ata_shift+5]);
	fprintf(stdout, "LBAhigh= %02X%02X\n", io_hdr.sbp[ata_shift+10], io_hdr.sbp[ata_shift+8]);
	fprintf(stdout, "LBAmid= %02X%02X\n", io_hdr.sbp[ata_shift+6], io_hdr.sbp[ata_shift+11]);
	fprintf(stdout, "LBAlow= %02X%02X\n", io_hdr.sbp[ata_shift+9], io_hdr.sbp[ata_shift+7]);
	fprintf(stdout, "device= %02X\n", io_hdr.sbp[ata_shift+12]);
	fprintf(stdout, "status= %02X\n", io_hdr.sbp[ata_shift+13]);
      }
    }

  }

  if ((verbosity > 2) && (doatainquiry == false))
  {
    fprintf (stdout, "sector=%lld\n", sector_start);
    fprintf (stdout, "count=%lld\n", sector_count);
    fprintf (stdout, "buffersize=%d\n", buffer_size);
    unsigned char *c;
    int i;
    for (i = 0; i < buffer_size; i+=16)
    {
      fprintf (stdout, "%d:", i);
      int n;
      for (n=0; n < 16; n++)
      {
        c = (unsigned char *)buffer+i+n;
        if (dobinary)
        {
          if (n == 8)
            fprintf (stdout, "\n%d:", i+n);
          int i;
          for (i = 0; i < 8; i++)
          {
            if ((*c<<i) & 0x80)
              fprintf (stdout, "1");
            else
              fprintf (stdout, "0");
          }
          fprintf (stdout, " ");
        }
        else
          fprintf (stdout, "%02X ", *c);
      }
      for (n=0; n < 16; n++)
      {
	c = (unsigned char *)buffer+i+n;
        if (!dobinary)
	  fprintf (stdout, "%c", isprint(*c) ? *c : '.');
      }
      fprintf (stdout, "\n");
    }
    fprintf (stdout, "\n");
  }

  // ata buffer words have bytes fliped
  if ((verbosity > 2) && (doatainquiry == true))
  {
    fprintf (stdout, "sector=%lld\n", sector_start);
    fprintf (stdout, "count=%lld\n", sector_count);
    fprintf (stdout, "buffersize=%d\n", buffer_size);
    unsigned char *c;
    unsigned char *d;
    int i;
    for (i = 0; i < buffer_size; i+=16)
    {
      fprintf (stdout, "%d:", i/2);
      int n;
      for (n=0; n < 16; n+=2)
      {
        c = (unsigned char *)buffer+i+n;
        d = (unsigned char *)buffer+i+n+1;
        if (dobinary)
        {
          if (n == 8)
            fprintf (stdout, "\n%d:", (i+n)/2);
          int i;
          for (i = 0; i < 8; i++)
          {
            if ((*d<<i) & 0x80)
              fprintf (stdout, "1");
            else
              fprintf (stdout, "0");
          }
          for (i = 0; i < 8; i++)
          {
            if ((*c<<i) & 0x80)
              fprintf (stdout, "1");
            else
              fprintf (stdout, "0");
          }
          fprintf (stdout, " ");
        }
        else
          fprintf (stdout, "%02X%02X ", *d, *c);
      }
      for (n=0; n < 16; n+=2)
      {
	c = (unsigned char *)buffer+i+n;
	d = (unsigned char *)buffer+i+n+1;
        if (!dobinary)
	  fprintf (stdout, "%c%c", isprint(*d) ? *d : '.', isprint(*c) ? *c : '.');
      }
      fprintf (stdout, "\n");
    }
    fprintf (stdout, "\n");
  }

  // if sense_key is not 0, 1, or 3 then something bad happened
  if (sense_key != 0 && sense_key != 1 && sense_key != 3)
  {
    fprintf (stderr, "\nscsi sense key reports the command failed,\n");
    fprintf (stderr, "the command my not be supported, or something else went wrong\n");
    fprintf(stderr, "additional sense info: %02X %02X %02X\n", sense_key, asc, ascq);
    if (doatainquiry == true)
      {
        int ata_shift = 8;
        fprintf(stderr, "ATA return data:\n");
        fprintf(stderr, "descriptor= %02X\n", io_hdr.sbp[ata_shift+0]);
        fprintf(stderr, "additional length= %02X\n", io_hdr.sbp[ata_shift+1]);
        fprintf(stderr, "extend= %02X\n", io_hdr.sbp[ata_shift+2]);
        fprintf(stderr, "error= %02X\n", io_hdr.sbp[ata_shift+3]);
        fprintf(stderr, "count= %02X%02X\n", io_hdr.sbp[ata_shift+4], io_hdr.sbp[ata_shift+5]);
        fprintf(stderr, "LBAhigh= %02X%02X\n", io_hdr.sbp[ata_shift+10], io_hdr.sbp[ata_shift+8]);
        fprintf(stderr, "LBAmid= %02X%02X\n", io_hdr.sbp[ata_shift+6], io_hdr.sbp[ata_shift+11]);
        fprintf(stderr, "LBAlow= %02X%02X\n", io_hdr.sbp[ata_shift+9], io_hdr.sbp[ata_shift+7]);
        fprintf(stderr, "device= %02X\n", io_hdr.sbp[ata_shift+12]);
        fprintf(stderr, "status= %02X\n", io_hdr.sbp[ata_shift+13]);
      }
    exit (1);
  }


  return (0);
}




// function to display version
void version(void)
{
  printf ("%s %s\n", title, version_number);
  printf ("Copyright (C) %d Scott Dwyer.\n", copyright_year);
  printf ("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n");
  printf ("This is free software: you are free to change and redistribute it.\n");
  printf ("There is NO WARRANTY, to the extent permitted by law.\n");
}

// function to display help
// function to display help
void help(void)
{
  unsigned int i;
  for (i = 0; i < ddru_diskutility_help_txt_len; i++)
  {
    printf ("%c", ddru_diskutility_help_txt[i]);
  }
}

#endif