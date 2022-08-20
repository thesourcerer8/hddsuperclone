// Copyright (C) 2015-2022 Scott Dwyer.
// You may use/distribute/modify this freely, under the terms of
// the GNU General Public License version 2 or later version.
// This software is distributed WITHOUT ANY WARRANTY.


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include "common.h"
#include "io.h"
#include "clone_gui_common.h"

unsigned char io_byte_ccc[16];
uint8_t io_singlebyte_ccc;
uint16_t io_word_ccc;
uint32_t io_doubleword_ccc;
uint64_t io_quadword_ccc;
uint8_t start_bit_ccc;
int command_status_ccc;
int identify_flag_ccc;
char error_string_ccc[255];
int lsblk_ccc = 1;
void *hba_virt_addr_ccc;
int performing_reset_ccc = 0;
int did_hard_reset_ccc = 0;
int did_power_cycle_ccc = 0;
int words_per_logical_sector_ccc = 256;
int bytes_per_logical_sector_ccc = 512;
char driver_device_name_ccc[64];
bool check_message_ccc;
char *title_ccc;
char virtual_driver_name_ccc[MAX_DEVICE_NAME_LENGTH];

struct sg_io_hdr io_hdr;

bool use_fpdma_ccc;
bool wait_for_ds_bit_ccc;
int driver_running_ccc;
#define MAXLANGLENGTH 2048
#define LANGCOUNT 427
char curlang_ccc[LANGCOUNT][MAXLANGLENGTH];


char *version_number_ccc;







// function to prepare CDB
int prepare_cdb_ccc(void)
{
  memset(&io_hdr, 0, sizeof(struct sg_io_hdr));
  io_hdr.interface_id = 'S';
  io_hdr.cmd_len = command_length_ccc;
  io_hdr.cmdp = passthrough_ccc.scsi_cmd;
  io_hdr.dxfer_direction = passthrough_ccc.sg_xfer_direction;
  io_hdr.dxfer_len = ccc_main_buffer_size_ccc;
  io_hdr.dxferp = ccc_buffer_ccc;
  io_hdr.mx_sb_len = sizeof(sense_buf_ccc);
  io_hdr.iovec_count = 0;
  io_hdr.sbp = sense_buf_ccc;
  io_hdr.timeout = general_timeout_ccc / 1000; // timeout in ms
  io_hdr.flags = SG_FLAG_DIRECT_IO;
  io_hdr.pack_id = 0;
  io_hdr.usr_ptr = 0;

  sense_key_ccc = 0;
  asc_ccc = 0;
  ascq_ccc = 0;


  if (verbose_ccc & DEBUG7)
  {
    fprintf (stdout, "CDB: ");
    int i;
    for (i = 0; i < command_length_ccc; i++)
    {
      fprintf (stdout, "%02x ", passthrough_ccc.scsi_cmd[i]);
    }
    fprintf (stdout, "\n");
  }
  if (debug_ccc & DEBUG7)
  {
    fprintf (debug_file_ccc, "CDB: ");
    int i;
    for (i = 0; i < command_length_ccc; i++)
    {
      fprintf (debug_file_ccc, "%02x ", passthrough_ccc.scsi_cmd[i]);
    }
    fprintf (debug_file_ccc, "\n");
  }

  return (0);
}



#define sensebuf ((char*)io_hdr.sbp)




// function post ioctl
int post_ioctl_ccc(void)
{
  int fixed = 0;
  int descriptor = 0;
  // if there was sense data written then check for errors
  if (io_hdr.sb_len_wr != 0)
  {
    switch (sensebuf[0])
    {
      case 0xF0:
      case 0xF1:
        sense_key_ccc = sensebuf[2] & 0x0f;
        asc_ccc = sensebuf[12];
        ascq_ccc = sensebuf[13];
        fixed = 1;
        break;

      case 0x70:
      case 0x71:
        sense_key_ccc = sensebuf[2] & 0x0f;
        asc_ccc = sensebuf[12];
        ascq_ccc = sensebuf[13];
        break;

      case 0x72:
      case 0x73:
        sense_key_ccc = sensebuf[1] & 0x0f;
        asc_ccc = sensebuf[2];
        ascq_ccc = sensebuf[3];
        descriptor = 1;
        break;

      default:
        sprintf (tempmessage_ccc, "\nbad (SG_IO) sense buffer response code from input device\n");
        if (superclone_ccc)
        {
          message_error_ccc(tempmessage_ccc);
        }
        else
        {
          message_now_ccc(tempmessage_ccc);
        }
        sprintf (tempmessage_ccc, "raw sense buffer: ");
        if (superclone_ccc)
        {
          message_error_ccc(tempmessage_ccc);
        }
        else
        {
          message_now_ccc(tempmessage_ccc);
        }
        int i;
        for (i = 0; i < io_hdr.sb_len_wr; i++)
        {
          sprintf (tempmessage_ccc, "%02x ", sensebuf[i]);
          if (superclone_ccc)
          {
            message_error_ccc(tempmessage_ccc);
          }
          else
          {
            message_now_ccc(tempmessage_ccc);
          }
        }
        sprintf (tempmessage_ccc, "\n");
        if (superclone_ccc)
        {
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
          clear_error_message_ccc();
        }
        else
        {
          message_now_ccc(tempmessage_ccc);
        }
        return INPUT_DEVICE_ERROR_RETURN_CODE;
    }
  }

  set_number_variable_value_ccc("$io_sense_key", sense_key_ccc);
  set_number_variable_value_ccc("$io_asc", asc_ccc);
  set_number_variable_value_ccc("$io_ascq", ascq_ccc);

  set_number_variable_value_ccc("$io_status", io_hdr.status);
  set_number_variable_value_ccc("$io_masked_status", io_hdr.masked_status);
  set_number_variable_value_ccc("$io_msg_status", io_hdr.msg_status);
  set_number_variable_value_ccc("$io_sb_len_wr", io_hdr.sb_len_wr);
  set_number_variable_value_ccc("$io_host_status", io_hdr.host_status);
  set_number_variable_value_ccc("$io_driver_status", io_hdr.driver_status);
  set_number_variable_value_ccc("$io_resid", io_hdr.resid);
  set_number_variable_value_ccc("$io_duration", io_hdr.duration);
  set_number_variable_value_ccc("$io_info", io_hdr.info);

  io_status_ccc = io_hdr.status;
  io_masked_status_ccc = io_hdr.masked_status;
  msg_status_ccc = io_hdr.msg_status;
  io_sb_len_wr_ccc = io_hdr.sb_len_wr;
  io_host_status_ccc = io_hdr.host_status;
  io_driver_status_ccc = io_hdr.driver_status;
  io_resid_ccc = io_hdr.resid;
  io_duration_ccc = io_hdr.duration;
  io_info_ccc = io_hdr.info;

  memset(sensebuffer_ccc, 0, sensebuffer_size_ccc);
  memcpy(sensebuffer_ccc, io_hdr.sbp, io_hdr.sb_len_wr);

  if (descriptor && io_hdr.sb_len_wr > 21 && sensebuf[8] == 0x9 && sensebuf[9] == 0xc)
  {
    int additional_sense_length = sensebuf[7];
    unsigned char ata_return_descriptor[additional_sense_length];
    int i;
    for (i = 0; i < additional_sense_length; i ++)
    {
      memcpy(&ata_return_descriptor[i], &sensebuf[i+8], 1);
    }

    set_number_variable_value_ccc("$ata_return_error", ata_return_descriptor[3]);
    int count = (ata_return_descriptor[4] << 8) + ata_return_descriptor[5];
    set_number_variable_value_ccc("$ata_return_count", count);
    long long lba = ((unsigned long long)ata_return_descriptor[10] << 48) + ((unsigned long long)ata_return_descriptor[8] << 32) + ((unsigned long long)ata_return_descriptor[6] << 24) + ((unsigned long long)ata_return_descriptor[11] << 16) + ((unsigned long long)ata_return_descriptor[9] << 8) + ata_return_descriptor[7];
    set_number_variable_value_ccc("$ata_return_lba", lba);
    set_number_variable_value_ccc("$ata_return_device", ata_return_descriptor[12]);
    set_number_variable_value_ccc("$ata_return_status", ata_return_descriptor[13]);
    memcpy(&ata_error_ccc, &ata_return_descriptor[3], 1);
    memcpy(&ata_status_ccc, &ata_return_descriptor[13], 1);
    memcpy(&ata_device_ccc, &ata_return_descriptor[12], 1);
    ata_lba_ccc = lba;
    ata_count_ccc = count;
    ata_return_valid_ccc = 1;
  }
  else if (fixed && io_hdr.sb_len_wr > 17)
  {
    unsigned char information[4];
    int i;
    for (i = 0; i < 4; i ++)
    {
      memcpy(&information[i], &sensebuf[i+3], 1);
    }
    unsigned char command_specific_information[4];
    for (i = 0; i < 4; i ++)
    {
      memcpy(&command_specific_information[i], &sensebuf[i+8], 1);
    }

    set_number_variable_value_ccc("$ata_return_error", information[0]);
    set_number_variable_value_ccc("$ata_return_status", information[1]);
    set_number_variable_value_ccc("$ata_return_device", information[2]);
    set_number_variable_value_ccc("$ata_return_count", information[3]);
    long long lba = ((unsigned long long)command_specific_information[1] << 16) + ((unsigned long long)command_specific_information[2] << 8) + command_specific_information[3];
    set_number_variable_value_ccc("$ata_return_lba", lba);
    memcpy(&ata_error_ccc, &information[0], 1);
    memcpy(&ata_status_ccc, &information[1], 1);
    memcpy(&ata_device_ccc, &information[2], 1);
    memcpy(&ata_count_ccc, &information[3], 1);
    ata_lba_ccc = lba;
    ata_return_valid_ccc = 1;
  }
  else
  {
    set_number_variable_value_ccc("$ata_return_error", 0);
    set_number_variable_value_ccc("$ata_return_count", 0);
    set_number_variable_value_ccc("$ata_return_lba", 0);
    set_number_variable_value_ccc("$ata_return_device", 0);
    set_number_variable_value_ccc("$ata_return_status", 0);
    ata_error_ccc = 0;
    ata_status_ccc = 0;
    ata_lba_ccc = 0;
    ata_count_ccc = 0;
    ata_device_ccc = 0;
    ata_return_valid_ccc = 0;
  }
  //fprintf(stdout, "error=%02x status=%02x sensekey=%02x asc=%02x ascq=%02x\n", ata_error_ccc, ata_status_ccc, sense_key_ccc, asc_ccc, ascq_ccc);    //debug
  if (0)
  {
    int i;
    for (i = 0; i < io_hdr.sb_len_wr; i++)
    {
      fprintf (stdout, "%02x ", sensebuf[i]);
    }
    fprintf (stdout, "\n");
    fprintf (stdout, "io_status_ccc %d\n", io_status_ccc);
    fprintf (stdout, "io_masked_status_ccc %d\n", io_masked_status_ccc);
    fprintf (stdout, "msg_status_ccc %d\n", msg_status_ccc);
    fprintf (stdout, "io_sb_len_wr_ccc %d\n", io_sb_len_wr_ccc);
    fprintf (stdout, "io_host_status_ccc %d\n", io_host_status_ccc);
    fprintf (stdout, "io_driver_status_ccc %d\n", io_driver_status_ccc);
    fprintf (stdout, "io_resid_ccc %d\n", io_resid_ccc);
    fprintf (stdout, "io_duration_ccc %d\n", io_duration_ccc);
    fprintf (stdout, "io_info_ccc %d\n", io_info_ccc);
  }

  return (0);
}








int do_scsi_cmd_ccc(int disk_fd)
{
  if (usb_mode_ccc)
  {
    unsigned long long start_time = get_elapsed_usec_ccc();
    unsigned long long elapsed_time;
    int usb_timeout_bak = usb_timeout_ccc;
    usb_timeout_ccc = soft_reset_time_ccc / 1000;
    int need_reset = 0;
    int inquiry_good = 0;
    int fail_level = 0;
    int ret = do_usb_cmd_ccc();
    if (stop_signal_ccc)
    {
      usb_timeout_ccc = usb_timeout_bak;
      return STOP_SIGNAL_RETURN_CODE;
    }
    elapsed_time = get_elapsed_usec_ccc();
    if (elapsed_time > start_time + general_timeout_ccc)
    {
      // if it exceeds general timeout then quit
      usb_timeout_ccc = usb_timeout_bak;
      return (INPUT_DEVICE_ERROR_RETURN_CODE);
    }
    usb_read_residue_ccc = usb_csw_residue_ccc;

    // good read
    if (ret == (int)ccc_main_buffer_size_ccc && usb_csw_valid_ccc && !usb_csw_residue_ccc && !usb_csw_status_ccc)
    {
      usb_timeout_ccc = usb_timeout_bak;
      return 0;
    }
    // bad read
    else if (usb_csw_valid_ccc && usb_csw_status_ccc == 1)
    {
      if (usb_get_sense_ccc() != USB_SENSE_BUFFER_SIZE)
      {
        if (stop_signal_ccc)
        {
          usb_timeout_ccc = usb_timeout_bak;
          return STOP_SIGNAL_RETURN_CODE;
        }
        elapsed_time = get_elapsed_usec_ccc();
        if (elapsed_time > start_time + general_timeout_ccc)
        {
          // if it exceeds general timeout then quit
          usb_timeout_ccc = usb_timeout_bak;
          return (INPUT_DEVICE_ERROR_RETURN_CODE);
        }
        fail_level = 0x1;
        need_reset = 1;
      }
      else
      {
        if (sense_key_ccc > 1)
        {
          usb_timeout_ccc = usb_timeout_bak;
          return 0;
        }
        else
        {
          usb_timeout_ccc = usb_timeout_bak;
          return 1;
        }
      }
    }

    // csw status phase error 0x02 or other unknown condition
    if (usb_csw_valid_ccc && usb_csw_status_ccc > 1)
    {
      fail_level = 0x10;
      if (!usb_reset_recovery_ccc(usb_timeout_ccc))
      {
        if (stop_signal_ccc)
        {
          usb_timeout_ccc = usb_timeout_bak;
          return STOP_SIGNAL_RETURN_CODE;
        }
        elapsed_time = get_elapsed_usec_ccc();
        if (elapsed_time > start_time + general_timeout_ccc)
        {
          // if it exceeds general timeout then quit
          usb_timeout_ccc = usb_timeout_bak;
          return (INPUT_DEVICE_ERROR_RETURN_CODE);
        }
        if (!usb_inquiry_ccc(usb_timeout_ccc) && !usb_check_capacity_ccc(usb_timeout_ccc))
        {
          if (stop_signal_ccc)
          {
            usb_timeout_ccc = usb_timeout_bak;
            return STOP_SIGNAL_RETURN_CODE;
          }
          elapsed_time = get_elapsed_usec_ccc();
          if (elapsed_time > start_time + general_timeout_ccc)
          {
            // if it exceeds general timeout then quit
            usb_timeout_ccc = usb_timeout_bak;
            return (INPUT_DEVICE_ERROR_RETURN_CODE);
          }
          inquiry_good = 1;
        }
        else
        {
          need_reset = 1;
        }
      }
      else
      {
        need_reset = 1;
      }
    }

    // other read conditions
    if (ret < 0)
    {
      // check if timeout
      if (ret == -110)
      {
        fail_level = 0x30;
        need_reset = 1;
      }
      else
      {
        fail_level = 0x40;
        need_reset = 1;
      }
    }
    else if (!usb_csw_valid_ccc)
    {
      fail_level = 0x50;
      if (usb_get_sense_ccc() != USB_SENSE_BUFFER_SIZE)
      {
        if (stop_signal_ccc)
        {
          usb_timeout_ccc = usb_timeout_bak;
          return STOP_SIGNAL_RETURN_CODE;
        }
        elapsed_time = get_elapsed_usec_ccc();
        if (elapsed_time > start_time + general_timeout_ccc)
        {
          // if it exceeds general timeout then quit
          usb_timeout_ccc = usb_timeout_bak;
          return (INPUT_DEVICE_ERROR_RETURN_CODE);
        }
        need_reset = 1;
      }
    }
    else if (ret != (int)ccc_main_buffer_size_ccc || usb_csw_residue_ccc)
    {
      if (usb_get_sense_ccc() != USB_SENSE_BUFFER_SIZE)
      {
        if (stop_signal_ccc)
        {
          usb_timeout_ccc = usb_timeout_bak;
          return STOP_SIGNAL_RETURN_CODE;
        }
        elapsed_time = get_elapsed_usec_ccc();
        if (elapsed_time > start_time + general_timeout_ccc)
        {
          // if it exceeds general timeout then quit
          usb_timeout_ccc = usb_timeout_bak;
          return (INPUT_DEVICE_ERROR_RETURN_CODE);
        }
        fail_level = 0x20;
        need_reset = 1;
      }
      else
      {
        inquiry_good = 1;
      }
      if ((ccc_main_buffer_size_ccc - ret) != usb_csw_residue_ccc)
      {
        fail_level = 0x21;
      }
    }

    if (need_reset)
    {
      fprintf (stdout, "usb reset needed %02x\n", fail_level);
      int rret;
      int reset_time = hard_reset_time_ccc / 1000;
      if (!usb_port_reset_enabled_ccc && !usb_hard_reset_enabled_ccc && !usb_soft_reset_enabled_ccc)
      {
        reset_time = reset_wait_time_ccc / 1000;
      }
      if (!inquiry_good && usb_bulk_reset_enabled_ccc)
      {
        if (!usb_reset_recovery_ccc(reset_time))
        {
          if (stop_signal_ccc)
          {
            usb_timeout_ccc = usb_timeout_bak;
            return STOP_SIGNAL_RETURN_CODE;
          }
          elapsed_time = get_elapsed_usec_ccc();
          if (elapsed_time > start_time + general_timeout_ccc)
          {
            // if it exceeds general timeout then quit
            usb_timeout_ccc = usb_timeout_bak;
            return (INPUT_DEVICE_ERROR_RETURN_CODE);
          }
          if (!usb_inquiry_ccc(reset_time) && !usb_check_capacity_ccc(usb_timeout_ccc))
          {
            if (stop_signal_ccc)
            {
              usb_timeout_ccc = usb_timeout_bak;
              return STOP_SIGNAL_RETURN_CODE;
            }
            elapsed_time = get_elapsed_usec_ccc();
            if (elapsed_time > start_time + general_timeout_ccc)
            {
              // if it exceeds general timeout then quit
              usb_timeout_ccc = usb_timeout_bak;
              return (INPUT_DEVICE_ERROR_RETURN_CODE);
            }
            inquiry_good = 1;
          }
        }
      }
      if (!usb_port_reset_enabled_ccc && !usb_hard_reset_enabled_ccc)
      {
        reset_time = reset_wait_time_ccc / 1000;
      }
      if (!inquiry_good && usb_soft_reset_enabled_ccc && usb_allow_ata_ccc)
      {
        if (!usb_reset_recovery_ccc(reset_time))
        {
          if (stop_signal_ccc)
          {
            usb_timeout_ccc = usb_timeout_bak;
            return STOP_SIGNAL_RETURN_CODE;
          }
          elapsed_time = get_elapsed_usec_ccc();
          if (elapsed_time > start_time + general_timeout_ccc)
          {
            // if it exceeds general timeout then quit
            usb_timeout_ccc = usb_timeout_bak;
            return (INPUT_DEVICE_ERROR_RETURN_CODE);
          }
          rret = usb_soft_hard_reset_ccc(USB_RESET_TYPE_SOFT, reset_time);
          if (rret == 0)
          {
            if (!usb_inquiry_ccc(reset_time) && !usb_check_capacity_ccc(usb_timeout_ccc))
            {
              if (stop_signal_ccc)
              {
                usb_timeout_ccc = usb_timeout_bak;
                return STOP_SIGNAL_RETURN_CODE;
              }
              elapsed_time = get_elapsed_usec_ccc();
              if (elapsed_time > start_time + general_timeout_ccc)
              {
                // if it exceeds general timeout then quit
                usb_timeout_ccc = usb_timeout_bak;
                return (INPUT_DEVICE_ERROR_RETURN_CODE);
              }
              inquiry_good = 1;
            }
          }
          else if (rret == 5)  // invalid command
          {
            need_reset = 0;
          }
        }
      }
      if (!usb_port_reset_enabled_ccc)
      {
        reset_time = reset_wait_time_ccc / 1000;
      }
      if (!inquiry_good && usb_hard_reset_enabled_ccc && usb_allow_ata_ccc)
      {
        if (need_reset)
        {
          rret = usb_reset_recovery_ccc(reset_time);
          if (stop_signal_ccc)
          {
            usb_timeout_ccc = usb_timeout_bak;
            return STOP_SIGNAL_RETURN_CODE;
          }
          elapsed_time = get_elapsed_usec_ccc();
          if (elapsed_time > start_time + general_timeout_ccc)
          {
            // if it exceeds general timeout then quit
            usb_timeout_ccc = usb_timeout_bak;
            return (INPUT_DEVICE_ERROR_RETURN_CODE);
          }
        }
        else
        {
          rret = 0;
        }
        if (!rret)
        {
          rret = usb_soft_hard_reset_ccc(USB_RESET_TYPE_HARD, reset_time);
          if (stop_signal_ccc)
          {
            usb_timeout_ccc = usb_timeout_bak;
            return STOP_SIGNAL_RETURN_CODE;
          }
          elapsed_time = get_elapsed_usec_ccc();
          if (elapsed_time > start_time + general_timeout_ccc)
          {
            // if it exceeds general timeout then quit
            usb_timeout_ccc = usb_timeout_bak;
            return (INPUT_DEVICE_ERROR_RETURN_CODE);
          }
          if (rret == 0)
          {
            if (!usb_inquiry_ccc(reset_time) && !usb_check_capacity_ccc(usb_timeout_ccc))
            {
              if (stop_signal_ccc)
              {
                usb_timeout_ccc = usb_timeout_bak;
                return STOP_SIGNAL_RETURN_CODE;
              }
              elapsed_time = get_elapsed_usec_ccc();
              if (elapsed_time > start_time + general_timeout_ccc)
              {
                // if it exceeds general timeout then quit
                usb_timeout_ccc = usb_timeout_bak;
                return (INPUT_DEVICE_ERROR_RETURN_CODE);
              }
              inquiry_good = 1;
            }
          }
        }
      }
      reset_time = reset_wait_time_ccc / 1000;
      if (!inquiry_good && usb_port_reset_enabled_ccc)
      {
        if (!do_usb_reset_ccc())
        {
          if (!usb_reset_recovery_ccc(reset_time))
          {
            if (stop_signal_ccc)
            {
              usb_timeout_ccc = usb_timeout_bak;
              return STOP_SIGNAL_RETURN_CODE;
            }
            elapsed_time = get_elapsed_usec_ccc();
            if (elapsed_time > start_time + general_timeout_ccc)
            {
              // if it exceeds general timeout then quit
              usb_timeout_ccc = usb_timeout_bak;
              return (INPUT_DEVICE_ERROR_RETURN_CODE);
            }
            if (!usb_inquiry_ccc(reset_time) && !usb_check_capacity_ccc(usb_timeout_ccc))
            {
              if (stop_signal_ccc)
              {
                usb_timeout_ccc = usb_timeout_bak;
                return STOP_SIGNAL_RETURN_CODE;
              }
              elapsed_time = get_elapsed_usec_ccc();
              if (elapsed_time > start_time + general_timeout_ccc)
              {
                // if it exceeds general timeout then quit
                usb_timeout_ccc = usb_timeout_bak;
                return (INPUT_DEVICE_ERROR_RETURN_CODE);
              }
              inquiry_good = 1;
            }
          }
        }
      }
    }
    if (!inquiry_good)
    {
      fail_level = fail_level + 0x0f;
    }
    usb_timeout_ccc = usb_timeout_bak;
    if (0)
    {
      fprintf (stdout, "usb read return = %d (%02x)\n", fail_level, fail_level);
    }
    return fail_level;
  }

  else
  {
    prepare_cdb_ccc();

    int ioctl_ret = ioctl(disk_fd, SG_IO, &io_hdr);
    if (ioctl_ret < 0)
    {
      sprintf(tempmessage_ccc, "Error performing IO on input device (%s).\n", strerror(errno));
      if (superclone_ccc)
      {
        check_message_ccc = true;
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
      }
      else
      {
        message_now_ccc(tempmessage_ccc);
      }
      return (INPUT_DEVICE_ERROR_RETURN_CODE);
    }

    int ret = post_ioctl_ccc();

    return ret;
  }
  return 0;
}





int do_usb_cmd_ccc(void)
{
  // clear any possible passthrough data
  sense_key_ccc = 0;
  asc_ccc = 0;
  ascq_ccc = 0;
  ata_error_ccc = 0;
  ata_status_ccc = 0;

  int cbw_ret = 0;
  int raw_ret = 0;
  int csw_ret = 0;
  usb_csw_status_ccc = 0;
  usb_csw_residue_ccc = 0;
  usb_csw_valid_ccc = 0;
  usb_tag_ccc++;
  memset (usbcbwbuffer_ccc, 0, USBCBW_BUFFER_SIZE);
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[71], passthrough_ccc.scsi_cmd, command_length_ccc);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[72], &command_length_ccc, 1);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[73], &usb_transfer_direction_ccc, 1);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[73] + 1, &usb_lun_ccc, 1);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[74], &usb_tag_ccc, 4);    //potential superbyte
  usb_transfer_length_ccc = ccc_main_buffer_size_ccc;
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[75], &usb_transfer_length_ccc, 4);    //potential superbyte
  uint32_t cbwsig = 0x43425355;
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[76], &cbwsig, 4);    //potential superbyte
  cbw_timeout_ccc = usb_timeout_ccc;
  cbw_ret = do_usb_send_cbw_ccc(cbw_timeout_ccc);
  if (cbw_ret != USBCBW_BUFFER_SIZE)
  {
    fprintf (stdout, "USB command send cbw short return or error %d (%s)\n", cbw_ret, strerror(abs(cbw_ret)));
    if (cbw_ret < 0)
    {
      return cbw_ret;
    }
    // reset and try again
    usb_reset_recovery_ccc(usb_timeout_ccc);
    cbw_ret = do_usb_send_cbw_ccc(cbw_timeout_ccc);
    if (cbw_ret != USBCBW_BUFFER_SIZE)
    {
      fprintf (stdout, "USB command send cbw short return or error %d (%s)\n", cbw_ret, strerror(abs(cbw_ret)));
      return INPUT_DEVICE_ERROR_RETURN_CODE;
    }
  }

  if (usb_transfer_length_ccc > 0)
  {
    if (usb_transfer_direction_ccc == USB_DIRECTION_IN)
    {
      raw_ret = usb_read_data_ccc(usb_timeout_ccc);
    }
    else if (usb_transfer_direction_ccc == USB_DIRECTION_OUT)
    {
      raw_ret = usb_write_data_ccc(usb_timeout_ccc);
    }
  }
  if (raw_ret != (int)usb_transfer_length_ccc)
  {
    fprintf (stdout, "USB command data transfer short return or error %d (%s)\n", raw_ret, strerror(abs(raw_ret)));
    if (raw_ret < 0)
    {
      return raw_ret;
    }
    //usb_auto_endpoint_reset(csw_timeout_ccc);
  }

  csw_timeout_ccc = usb_timeout_ccc;
  csw_ret = do_usb_receive_csw_ccc(csw_timeout_ccc);
  if (csw_ret == USBCSW_BUFFER_SIZE)
  {
    usb_csw_valid_ccc = 1;
  }
  else
  {
    fprintf (stdout, "USB command receive csw short return or error %d (%s)\n", csw_ret, strerror(abs(csw_ret)));
    if (csw_ret < 0)
    {
      //return csw_ret;
    }
    //usb_auto_endpoint_reset(csw_timeout_ccc);
    csw_ret = do_usb_receive_csw_ccc(csw_timeout_ccc);
    if (csw_ret == USBCSW_BUFFER_SIZE)
    {
    usb_csw_valid_ccc = 1;
    }
    else
    {
      fprintf (stdout, "USB command receive csw short return or error %d (%s)\n", csw_ret, strerror(abs(csw_ret)));
      //return INPUT_DEVICE_ERROR_RETURN_CODE;
    }
  }
  if (usb_csw_valid_ccc)
  {
    uint32_t cswsig;
    memcpy (&cswsig, usbcswbuffer_ccc + 0, 4);
    if (cswsig != 0x53425355)
    {
      fprintf (stdout, "USB command CSW wrong signature\n");
      usb_csw_valid_ccc = 0;
    }
    uint32_t tag;
    memcpy (&tag, usbcswbuffer_ccc + 4, 4);
    if (tag != usb_tag_ccc)
    {
      fprintf (stdout, "USB command CSW tag mismatch\n");
      usb_csw_valid_ccc = 0;
    }
    memcpy (&usb_csw_residue_ccc, usbcswbuffer_ccc + 8, 4);
    if (usb_csw_residue_ccc != 0)
    {
      fprintf (stdout, "USB command CSW data residue %d\n", usb_csw_residue_ccc);
    }
    memcpy (&usb_csw_status_ccc, usbcswbuffer_ccc + 12, 1);
    if (usb_csw_status_ccc != 0)
    {
      fprintf (stdout, "USB command CSW status failed %d\n", usb_csw_status_ccc);
    }
  }

  return raw_ret;
}





int usb_get_sense_ccc(void)
{
  // clear any possible passthrough data
  sense_key_ccc = 0;
  asc_ccc = 0;
  ascq_ccc = 0;
  ata_error_ccc = 0;
  ata_status_ccc = 0;
  ata_lba_ccc = 0;
  ata_count_ccc = 0;
  ata_device_ccc = 0;
  ata_return_valid_ccc = 0;
  int descriptor = 0;
  int fixed = 0;

  if (enable_process_chunk_ccc && 0)
  {
    sensebuffer_size_ccc = USB_SENSE_BUFFER_DESC_SIZE;
  }
  else
  {
    sensebuffer_size_ccc = USB_SENSE_BUFFER_SIZE;
  }

  usb_transfer_length_ccc = sensebuffer_size_ccc;
  passthrough_ccc.scsi_cmd[0] = 0x03; // request sense
  passthrough_ccc.scsi_cmd[1] = 0;
  passthrough_ccc.scsi_cmd[2] = 0;
  passthrough_ccc.scsi_cmd[3] = 0;
  passthrough_ccc.scsi_cmd[4] = usb_transfer_length_ccc; // size of data requested back
  passthrough_ccc.scsi_cmd[5] = 0;
  command_length_ccc = 6;
  passthrough_ccc.sg_xfer_direction = SG_DXFER_FROM_DEV;
  usb_transfer_direction_ccc = USB_DIRECTION_IN;

  int cbw_ret = 0;
  int raw_ret = 0;
  int csw_ret = 0;
  unsigned char csw_status = 0;
  uint32_t csw_residue = 0;
  usb_tag_ccc++;
  memset (usbcbwbuffer_ccc, 0, USBCBW_BUFFER_SIZE);
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[71], passthrough_ccc.scsi_cmd, command_length_ccc);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[72], &command_length_ccc, 1);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[73], &usb_transfer_direction_ccc, 1);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[73] + 1, &usb_lun_ccc, 1);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[74], &usb_tag_ccc, 4);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[75], &usb_transfer_length_ccc, 4);    //potential superbyte
  uint32_t cbwsig = 0x43425355;
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[76], &cbwsig, 4);    //potential superbyte
  cbw_timeout_ccc = usb_timeout_ccc;
  cbw_ret = do_usb_send_cbw_ccc(cbw_timeout_ccc);
  if (cbw_ret != USBCBW_BUFFER_SIZE)
  {
    fprintf (stdout, "USB sense send cbw short return or error %d (%s)\n", cbw_ret, strerror(abs(cbw_ret)));
    if (cbw_ret < 0)
    {
      return cbw_ret;
    }
    // reset and try again
    usb_reset_recovery_ccc(usb_timeout_ccc);
    cbw_ret = do_usb_send_cbw_ccc(cbw_timeout_ccc);
    if (cbw_ret != USBCBW_BUFFER_SIZE)
    {
      fprintf (stdout, "USB sense send cbw short return or error %d (%s)\n", cbw_ret, strerror(abs(cbw_ret)));
      return INPUT_DEVICE_ERROR_RETURN_CODE;
    }
  }

  set_sense_buffer_ccc();
  raw_ret = do_usb_read_sense_ccc(usb_timeout_ccc);
  if (raw_ret != (int)usb_transfer_length_ccc)
  {
    fprintf (stdout, "USB sense data transfer short return or error %d (%s)\n", raw_ret, strerror(abs(raw_ret)));
    if (raw_ret < 0)
    {
      return raw_ret;
    }
    //reset_usb_endpoint_in_ccc();
  }

  csw_timeout_ccc = usb_timeout_ccc;
  csw_ret = do_usb_receive_csw_ccc(csw_timeout_ccc);
  if (csw_ret != USBCSW_BUFFER_SIZE)
  {
    fprintf (stdout, "USB sense receive csw short return or error %d (%s)\n", csw_ret, strerror(abs(csw_ret)));
    if (csw_ret < 0)
    {
      return csw_ret;
    }
    //reset_usb_endpoint_in_ccc();
    csw_ret = do_usb_receive_csw_ccc(usb_timeout_ccc);
    if (csw_ret != USBCSW_BUFFER_SIZE)
    {
      fprintf (stdout, "USB sense receive csw short return or error %d (%s)\n", csw_ret, strerror(abs(csw_ret)));
      return INPUT_DEVICE_ERROR_RETURN_CODE;
    }
  }
  uint32_t cswsig;
  memcpy (&cswsig, usbcswbuffer_ccc + 0, 4);
  if (cswsig != 0x53425355)
  {
    fprintf (stdout, "USB sense CSW wrong signature\n");
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  uint32_t tag;
  memcpy (&tag, usbcswbuffer_ccc + 4, 4);
  if (tag != usb_tag_ccc)
  {
    fprintf (stdout, "USB sense CSW tag mismatch\n");
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  memcpy (&csw_residue, usbcswbuffer_ccc + 8, 4);
  if (csw_residue != 0)
  {
    fprintf (stdout, "USB sense CSW data residue %d\n", csw_residue);
  }
  memcpy (&csw_status, usbcswbuffer_ccc + 12, 1);
  if (csw_status != 0)
  {
    fprintf (stdout, "USB sense CSW status failed %d\n", csw_status);
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }

  unsigned char byte;
  unsigned char code;
  memcpy (&code, sensebuffer_ccc+0,1);
  switch (code)
  {
    case 0x70:
    case 0x71:
      memcpy (&byte, sensebuffer_ccc+2, 1);
      sense_key_ccc = byte & 0x0f;
      memcpy (&byte, sensebuffer_ccc+12, 1);
      asc_ccc = byte;
      memcpy (&byte, sensebuffer_ccc+13, 1);
      ascq_ccc = byte;
      break;

    case 0xF0:
    case 0xF1:
      memcpy (&byte, sensebuffer_ccc+2, 1);
      sense_key_ccc = byte & 0x0f;
      memcpy (&byte, sensebuffer_ccc+12, 1);
      asc_ccc = byte;
      memcpy (&byte, sensebuffer_ccc+13, 1);
      ascq_ccc = byte;
      fixed = 1;
      break;

    case 0x72:
    case 0x73:
      memcpy (&byte, sensebuffer_ccc+1, 1);
      sense_key_ccc = byte & 0x0f;
      memcpy (&byte, sensebuffer_ccc+2, 1);
      asc_ccc = byte;
      memcpy (&byte, sensebuffer_ccc+3, 1);
      ascq_ccc = byte;
      descriptor = 1;
      break;

    default:
      fprintf (stdout, "bad sense code %x\n", code);
      return INPUT_DEVICE_ERROR_RETURN_CODE;
  }

  unsigned char raw_sense[sensebuffer_size_ccc];
  unsigned int n;
  for (n = 0; n < sensebuffer_size_ccc; n++)
  {
    memcpy(&raw_sense, sensebuffer_ccc, 1);
  }

  if (0 && descriptor && sensebuffer_size_ccc > 21 && raw_sense[8] == 0x9 && raw_sense[9] == 0xc)
  {
    int additional_sense_length = raw_sense[7];
    unsigned char ata_return_descriptor[additional_sense_length];
    int i;
    for (i = 0; i < additional_sense_length; i ++)
    {
      memcpy(&ata_return_descriptor[i], &raw_sense[i+8], 1);
    }

    int count = (ata_return_descriptor[4] << 8) + ata_return_descriptor[5];
    long long lba = ((unsigned long long)ata_return_descriptor[10] << 48) + ((unsigned long long)ata_return_descriptor[8] << 32) + ((unsigned long long)ata_return_descriptor[6] << 24) + ((unsigned long long)ata_return_descriptor[11] << 16) + ((unsigned long long)ata_return_descriptor[9] << 8) + ata_return_descriptor[7];
    memcpy(&ata_error_ccc, &ata_return_descriptor[3], 1);
    memcpy(&ata_status_ccc, &ata_return_descriptor[13], 1);
    memcpy(&ata_device_ccc, &ata_return_descriptor[12], 1);
    ata_lba_ccc = lba;
    ata_count_ccc = count;
    ata_return_valid_ccc = 1;
  }
  else if (0 && fixed && sensebuffer_size_ccc > 17)
  {
    unsigned char information[4];
    int i;
    for (i = 0; i < 4; i ++)
    {
      memcpy(&information[i], &raw_sense[i+3], 1);
    }
    unsigned char command_specific_information[4];
    for (i = 0; i < 4; i ++)
    {
      memcpy(&command_specific_information[i], &raw_sense[i+8], 1);
    }

    long long lba = ((unsigned long long)command_specific_information[1] << 16) + ((unsigned long long)command_specific_information[2] << 8) + command_specific_information[3];
    memcpy(&ata_error_ccc, &information[0], 1);
    memcpy(&ata_status_ccc, &information[1], 1);
    memcpy(&ata_device_ccc, &information[2], 1);
    memcpy(&ata_count_ccc, &information[3], 1);
    ata_lba_ccc = lba;
    ata_return_valid_ccc = 1;
  }
  else
  {
    ata_error_ccc = 0;
    ata_status_ccc = 0;
    ata_lba_ccc = 0;
    ata_count_ccc = 0;
    ata_device_ccc = 0;
    ata_return_valid_ccc = 0;
  }

  if (1)
  {
    fprintf (stdout, "sense=%02x asc=%02x ascq=%02x\n", sense_key_ccc, asc_ccc, ascq_ccc);
    if (ata_return_valid_ccc)
    {
      fprintf (stdout, "error=%02x status=%02x device=%02x count=%02x lba=%02llx\n", ata_error_ccc, ata_status_ccc, ata_device_ccc, ata_count_ccc, ata_lba_ccc);
    }
  }

  return raw_ret;
}





int usb_soft_hard_reset_ccc(unsigned char reset_type, int timeout)
{
  if (superclone_ccc)
  {
    if (reset_type == USB_RESET_TYPE_SOFT)
    {
      sprintf (tempmessage_ccc, "usb soft reset\n");
    }
    else
    {
      sprintf (tempmessage_ccc, "usb hard reset\n");
    }
    message_console_log_ccc(tempmessage_ccc, 0);
  }
  // clear any possible passthrough data
  sense_key_ccc = 0;
  asc_ccc = 0;
  ascq_ccc = 0;

  usb_transfer_length_ccc = 0;
  passthrough_ccc.scsi_cmd[0] = 0xa1;  // ata12 passthrough
  passthrough_ccc.scsi_cmd[1] = reset_type;  // soft = 2, hard = 0
  passthrough_ccc.scsi_cmd[2] = 0;
  passthrough_ccc.scsi_cmd[3] = 0;
  passthrough_ccc.scsi_cmd[4] = 0;
  passthrough_ccc.scsi_cmd[5] = 0;
  passthrough_ccc.scsi_cmd[6] = 0;
  passthrough_ccc.scsi_cmd[7] = 0;
  passthrough_ccc.scsi_cmd[8] = 0;
  passthrough_ccc.scsi_cmd[9] = 0;
  passthrough_ccc.scsi_cmd[10] = 0;
  passthrough_ccc.scsi_cmd[11] = 0;
  command_length_ccc = 12;
  passthrough_ccc.sg_xfer_direction = SG_DXFER_FROM_DEV;
  usb_transfer_direction_ccc = USB_DIRECTION_IN;

  int cbw_ret = 0;
  int csw_ret = 0;
  unsigned char csw_status = 0;
  uint32_t csw_residue = 0;
  usb_tag_ccc++;
  memset (usbcbwbuffer_ccc, 0, USBCBW_BUFFER_SIZE);
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[71], passthrough_ccc.scsi_cmd, command_length_ccc);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[72], &command_length_ccc, 1);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[73], &usb_transfer_direction_ccc, 1);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[73] + 1, &usb_lun_ccc, 1);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[74], &usb_tag_ccc, 4);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[75], &usb_transfer_length_ccc, 4);    //potential superbyte
  uint32_t cbwsig = 0x43425355;
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[76], &cbwsig, 4);    //potential superbyte
  cbw_timeout_ccc = usb_timeout_ccc;
  cbw_ret = do_usb_send_cbw_ccc(cbw_timeout_ccc);
  if (cbw_ret != USBCBW_BUFFER_SIZE)
  {
    fprintf (stdout, "USB soft/hard reset send cbw short return or error %d (%s)\n", cbw_ret, strerror(abs(cbw_ret)));
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }

  csw_ret = do_usb_receive_csw_ccc(timeout);
  if (csw_ret != USBCSW_BUFFER_SIZE)
  {
    fprintf (stdout, "USB soft/hard reset receive csw short return or error %d (%s)\n", csw_ret, strerror(abs(csw_ret)));
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  uint32_t cswsig;
  memcpy (&cswsig, usbcswbuffer_ccc + 0, 4);
  if (cswsig != 0x53425355)
  {
    fprintf (stdout, "USB soft/hard reset CSW wrong signature\n");
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  uint32_t tag;
  memcpy (&tag, usbcswbuffer_ccc + 4, 4);
  if (tag != usb_tag_ccc)
  {
    fprintf (stdout, "USB soft/hard reset CSW tag mismatch\n");
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  memcpy (&csw_residue, usbcswbuffer_ccc + 8, 4);
  if (csw_residue != 0)
  {
    fprintf (stdout, "USB soft/hard reset CSW data residue %d\n", csw_residue);
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  memcpy (&csw_status, usbcswbuffer_ccc + 12, 1);
  if (csw_status != 0)
  {
    fprintf (stdout, "USB soft/hard reset CSW status failed %d\n", csw_status);
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  if (usb_get_sense_ccc() != USB_SENSE_BUFFER_SIZE)
  {
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  else if (sense_key_ccc > 1)
  {
    if (superclone_ccc && sense_key_ccc == 5)
    {
      sprintf (tempmessage_ccc, "soft/hard reset illegal request\n");
      message_console_log_ccc(tempmessage_ccc, 0);
    }
    return sense_key_ccc;
  }
  return 0;
}





int usb_inquiry_ccc(int timeout)
{
  // clear any possible passthrough data
  sense_key_ccc = 0;
  asc_ccc = 0;
  ascq_ccc = 0;

  ccc_main_usbbuffer_size_ccc = 36;    // 36 is the minimum inquiry return size
  usb_transfer_length_ccc = ccc_main_usbbuffer_size_ccc;
  passthrough_ccc.scsi_cmd[0] = 0x12; // inquiry
  passthrough_ccc.scsi_cmd[1] = 0;
  passthrough_ccc.scsi_cmd[2] = 0;
  passthrough_ccc.scsi_cmd[3] = 0;
  passthrough_ccc.scsi_cmd[4] = usb_transfer_length_ccc; // size of data requested back
  passthrough_ccc.scsi_cmd[5] = 0;
  command_length_ccc = 6;
  passthrough_ccc.sg_xfer_direction = SG_DXFER_FROM_DEV;
  usb_transfer_direction_ccc = USB_DIRECTION_IN;

  int cbw_ret = 0;
  int raw_ret = 0;
  int csw_ret = 0;
  unsigned char csw_status = 0;
  uint32_t csw_residue = 0;
  usb_tag_ccc++;
  memset (usbcbwbuffer_ccc, 0, USBCBW_BUFFER_SIZE);
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[71], passthrough_ccc.scsi_cmd, command_length_ccc);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[72], &command_length_ccc, 1);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[73], &usb_transfer_direction_ccc, 1);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[73] + 1, &usb_lun_ccc, 1);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[74], &usb_tag_ccc, 4);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[75], &usb_transfer_length_ccc, 4);    //potential superbyte
  uint32_t cbwsig = 0x43425355;
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[76], &cbwsig, 4);    //potential superbyte
  cbw_timeout_ccc = usb_timeout_ccc;
  cbw_ret = do_usb_send_cbw_ccc(cbw_timeout_ccc);
  if (cbw_ret != USBCBW_BUFFER_SIZE)
  {
    fprintf (stdout, "USB inquiry send cbw short return or error %d (%s)\n", cbw_ret, strerror(abs(cbw_ret)));
    if (cbw_ret < 0)
    {
      return cbw_ret;
    }
    // reset and try again
    usb_reset_recovery_ccc(usb_timeout_ccc);
    cbw_ret = do_usb_send_cbw_ccc(cbw_timeout_ccc);
    if (cbw_ret != USBCBW_BUFFER_SIZE)
    {
      fprintf (stdout, "USB inquiry send cbw short return or error %d (%s)\n", cbw_ret, strerror(abs(cbw_ret)));
      return INPUT_DEVICE_ERROR_RETURN_CODE;
    }
  }

  set_main_usb_buffer_ccc();
  raw_ret = do_usb_raw_read_ccc(timeout);
  if (raw_ret < 36)    // 36 is the minimum inquiry return size
  {
    fprintf (stdout, "USB inquiry data transfer short return or error %d (%s)\n", raw_ret, strerror(abs(raw_ret)));
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }

  csw_timeout_ccc = usb_timeout_ccc;
  csw_ret = do_usb_receive_csw_ccc(csw_timeout_ccc);
  if (csw_ret != USBCSW_BUFFER_SIZE)
  {
    fprintf (stdout, "USB inquiry receive csw short return or error %d (%s)\n", csw_ret, strerror(abs(csw_ret)));
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  uint32_t cswsig;
  memcpy (&cswsig, usbcswbuffer_ccc + 0, 4);
  if (cswsig != 0x53425355)
  {
    fprintf (stdout, "USB inquiry CSW wrong signature\n");
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  uint32_t tag;
  memcpy (&tag, usbcswbuffer_ccc + 4, 4);
  if (tag != usb_tag_ccc)
  {
    fprintf (stdout, "USB inquiry CSW tag mismatch\n");
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  memcpy (&csw_residue, usbcswbuffer_ccc + 8, 4);
  if (csw_residue != 0)
  {
    fprintf (stdout, "USB inquiry CSW data residue %d\n", csw_residue);
    if (csw_residue > 8)    // asking for 44 but might only get 36
    {
      return INPUT_DEVICE_ERROR_RETURN_CODE;
    }
  }
  memcpy (&csw_status, usbcswbuffer_ccc + 12, 1);
  if (csw_status != 0)
  {
    fprintf (stdout, "USB inquiry CSW status failed %d\n", csw_status);
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  return 0;
}





int usb_identify_ccc(int timeout)
{
  // clear any possible passthrough data
  sense_key_ccc = 0;
  asc_ccc = 0;
  ascq_ccc = 0;

  ccc_main_usbbuffer_size_ccc = IDENTIFY_BUFFER_SIZE;
  usb_transfer_length_ccc = ccc_main_usbbuffer_size_ccc;

  passthrough_ccc.multiple_count = 0;
  passthrough_ccc.transfer_length = 2;    // sector field
  passthrough_ccc.byte_block = 1;    // block
  passthrough_ccc.protocol = 4;    // pio-data-in
  passthrough_ccc.check_condition = 0;    // check condition
  passthrough_ccc.direction = 2;    // from
  passthrough_ccc.transfer_direction = 1;    // from

  passthrough_ccc.scsi_cmd[0] = 0xA1; // ata passthrough 12 command
  passthrough_ccc.scsi_cmd[1] = (passthrough_ccc.multiple_count << 5) + (passthrough_ccc.protocol << 1);
  passthrough_ccc.scsi_cmd[2] = (passthrough_ccc.off_line << 6) + (passthrough_ccc.check_condition << 5) + (passthrough_ccc.transfer_direction << 3) + ( passthrough_ccc.byte_block << 2) + (passthrough_ccc.transfer_length);
  passthrough_ccc.scsi_cmd[3] = 0;
  passthrough_ccc.scsi_cmd[4] = 1;
  passthrough_ccc.scsi_cmd[5] = 0;
  passthrough_ccc.scsi_cmd[6] = 0;
  passthrough_ccc.scsi_cmd[7] = 0;
  passthrough_ccc.scsi_cmd[8] = 0xa0;
  passthrough_ccc.scsi_cmd[9] = 0xec;
  passthrough_ccc.scsi_cmd[10] = 0;
  passthrough_ccc.scsi_cmd[11] = 0;
  command_length_ccc = 12;
  passthrough_ccc.sg_xfer_direction = SG_DXFER_FROM_DEV;
  usb_transfer_direction_ccc = USB_DIRECTION_IN;

  int cbw_ret = 0;
  int raw_ret = 0;
  int csw_ret = 0;
  unsigned char csw_status = 0;
  uint32_t csw_residue = 0;
  usb_tag_ccc++;
  memset (usbcbwbuffer_ccc, 0, USBCBW_BUFFER_SIZE);
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[71], passthrough_ccc.scsi_cmd, command_length_ccc);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[72], &command_length_ccc, 1);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[73], &usb_transfer_direction_ccc, 1);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[73] + 1, &usb_lun_ccc, 1);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[74], &usb_tag_ccc, 4);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[75], &usb_transfer_length_ccc, 4);    //potential superbyte
  uint32_t cbwsig = 0x43425355;
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[76], &cbwsig, 4);    //potential superbyte
  cbw_timeout_ccc = usb_timeout_ccc;
  cbw_ret = do_usb_send_cbw_ccc(cbw_timeout_ccc);
  if (cbw_ret != USBCBW_BUFFER_SIZE)
  {
    fprintf (stdout, "USB identify send cbw short return or error %d (%s)\n", cbw_ret, strerror(abs(cbw_ret)));
    if (cbw_ret < 0)
    {
      return cbw_ret;
    }
    // reset and try again
    usb_reset_recovery_ccc(usb_timeout_ccc);
    cbw_ret = do_usb_send_cbw_ccc(cbw_timeout_ccc);
    if (cbw_ret != USBCBW_BUFFER_SIZE)
    {
      fprintf (stdout, "USB identify send cbw short return or error %d (%s)\n", cbw_ret, strerror(abs(cbw_ret)));
      return INPUT_DEVICE_ERROR_RETURN_CODE;
    }
  }

  set_main_usb_buffer_ccc();
  raw_ret = do_usb_raw_read_ccc(timeout);
  if (raw_ret != (int)usb_transfer_length_ccc)
  {
    fprintf (stdout, "USB identify data transfer short return or error %d (%s)\n", raw_ret, strerror(abs(raw_ret)));
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }

  csw_timeout_ccc = usb_timeout_ccc;
  csw_ret = do_usb_receive_csw_ccc(csw_timeout_ccc);
  if (csw_ret != USBCSW_BUFFER_SIZE)
  {
    fprintf (stdout, "USB identify receive csw short return or error %d (%s)\n", csw_ret, strerror(abs(csw_ret)));
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  uint32_t cswsig;
  memcpy (&cswsig, usbcswbuffer_ccc + 0, 4);
  if (cswsig != 0x53425355)
  {
    fprintf (stdout, "USB identify CSW wrong signature\n");
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  uint32_t tag;
  memcpy (&tag, usbcswbuffer_ccc + 4, 4);
  if (tag != usb_tag_ccc)
  {
    fprintf (stdout, "USB identify CSW tag mismatch\n");
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  memcpy (&csw_residue, usbcswbuffer_ccc + 8, 4);
  if (csw_residue != 0)
  {
    fprintf (stdout, "USB identify CSW data residue %d\n", csw_residue);
    if (csw_residue > 8)    // asking for 44 but might only get 36
    {
      return INPUT_DEVICE_ERROR_RETURN_CODE;
    }
  }
  memcpy (&csw_status, usbcswbuffer_ccc + 12, 1);
  if (csw_status != 0)
  {
    fprintf (stdout, "USB identify CSW status failed %d\n", csw_status);
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  if (usb_get_sense_ccc() != USB_SENSE_BUFFER_SIZE)
  {
    fprintf (stdout, "USB identify error getting sense\n");
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  else if (sense_key_ccc > 1)
  {
    fprintf (stdout, "USB identify illegal request sense=%02x\n", sense_key_ccc);
    return sense_key_ccc;
  }
  return 0;
}





int usb_read_smart_ccc(int timeout, unsigned char reg[7])
{
  // clear any possible passthrough data
  sense_key_ccc = 0;
  asc_ccc = 0;
  ascq_ccc = 0;

  ccc_main_usbbuffer_size_ccc = IDENTIFY_BUFFER_SIZE;
  usb_transfer_length_ccc = ccc_main_usbbuffer_size_ccc;

  passthrough_ccc.multiple_count = 0;
  passthrough_ccc.transfer_length = 2;    // sector field
  passthrough_ccc.byte_block = 1;    // block
  passthrough_ccc.protocol = 4;    // pio-data-in
  passthrough_ccc.check_condition = 0;    // check condition
  passthrough_ccc.direction = 2;    // from
  passthrough_ccc.transfer_direction = 1;    // from

  passthrough_ccc.scsi_cmd[0] = 0xA1; // ata passthrough 12 command
  passthrough_ccc.scsi_cmd[1] = (passthrough_ccc.multiple_count << 5) + (passthrough_ccc.protocol << 1);
  passthrough_ccc.scsi_cmd[2] = (passthrough_ccc.off_line << 6) + (passthrough_ccc.check_condition << 5) + (passthrough_ccc.transfer_direction << 3) + ( passthrough_ccc.byte_block << 2) + (passthrough_ccc.transfer_length);
  passthrough_ccc.scsi_cmd[3] = reg[0];
  passthrough_ccc.scsi_cmd[4] = reg[1];
  passthrough_ccc.scsi_cmd[5] = reg[2];
  passthrough_ccc.scsi_cmd[6] = reg[3];
  passthrough_ccc.scsi_cmd[7] = reg[4];
  passthrough_ccc.scsi_cmd[8] = reg[5];
  passthrough_ccc.scsi_cmd[9] = reg[6];
  passthrough_ccc.scsi_cmd[10] = 0;
  passthrough_ccc.scsi_cmd[11] = 0;
  command_length_ccc = 12;
  passthrough_ccc.sg_xfer_direction = SG_DXFER_FROM_DEV;
  usb_transfer_direction_ccc = USB_DIRECTION_IN;

  int cbw_ret = 0;
  int raw_ret = 0;
  int csw_ret = 0;
  unsigned char csw_status = 0;
  uint32_t csw_residue = 0;
  usb_tag_ccc++;
  memset (usbcbwbuffer_ccc, 0, USBCBW_BUFFER_SIZE);
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[71], passthrough_ccc.scsi_cmd, command_length_ccc);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[72], &command_length_ccc, 1);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[73], &usb_transfer_direction_ccc, 1);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[73] + 1, &usb_lun_ccc, 1);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[74], &usb_tag_ccc, 4);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[75], &usb_transfer_length_ccc, 4);    //potential superbyte
  uint32_t cbwsig = 0x43425355;
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[76], &cbwsig, 4);    //potential superbyte
  cbw_timeout_ccc = usb_timeout_ccc;
  cbw_ret = do_usb_send_cbw_ccc(cbw_timeout_ccc);
  if (cbw_ret != USBCBW_BUFFER_SIZE)
  {
    fprintf (stdout, "USB identify send cbw short return or error %d (%s)\n", cbw_ret, strerror(abs(cbw_ret)));
    if (cbw_ret < 0)
    {
      return cbw_ret;
    }
    // reset and try again
    usb_reset_recovery_ccc(usb_timeout_ccc);
    cbw_ret = do_usb_send_cbw_ccc(cbw_timeout_ccc);
    if (cbw_ret != USBCBW_BUFFER_SIZE)
    {
      fprintf (stdout, "USB identify send cbw short return or error %d (%s)\n", cbw_ret, strerror(abs(cbw_ret)));
      return INPUT_DEVICE_ERROR_RETURN_CODE;
    }
  }

  set_main_usb_buffer_ccc();
  raw_ret = do_usb_raw_read_ccc(timeout);
  if (raw_ret != (int)usb_transfer_length_ccc)
  {
    fprintf (stdout, "USB identify data transfer short return or error %d (%s)\n", raw_ret, strerror(abs(raw_ret)));
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }

  csw_timeout_ccc = usb_timeout_ccc;
  csw_ret = do_usb_receive_csw_ccc(csw_timeout_ccc);
  if (csw_ret != USBCSW_BUFFER_SIZE)
  {
    fprintf (stdout, "USB identify receive csw short return or error %d (%s)\n", csw_ret, strerror(abs(csw_ret)));
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  uint32_t cswsig;
  memcpy (&cswsig, usbcswbuffer_ccc + 0, 4);
  if (cswsig != 0x53425355)
  {
    fprintf (stdout, "USB identify CSW wrong signature\n");
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  uint32_t tag;
  memcpy (&tag, usbcswbuffer_ccc + 4, 4);
  if (tag != usb_tag_ccc)
  {
    fprintf (stdout, "USB identify CSW tag mismatch\n");
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  memcpy (&csw_residue, usbcswbuffer_ccc + 8, 4);
  if (csw_residue != 0)
  {
    fprintf (stdout, "USB identify CSW data residue %d\n", csw_residue);
    if (csw_residue > 8)    // asking for 44 but might only get 36
    {
      return INPUT_DEVICE_ERROR_RETURN_CODE;
    }
  }
  memcpy (&csw_status, usbcswbuffer_ccc + 12, 1);
  if (csw_status != 0)
  {
    fprintf (stdout, "USB identify CSW status failed %d\n", csw_status);
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  if (usb_get_sense_ccc() != USB_SENSE_BUFFER_SIZE)
  {
    fprintf (stdout, "USB identify error getting sense\n");
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  else if (sense_key_ccc > 1)
  {
    fprintf (stdout, "USB identify illegal request sense=%02x\n", sense_key_ccc);
    return sense_key_ccc;
  }
  return 0;
}





int usb_check_capacity_ccc(int timeout)
{
  // clear any possible passthrough data
  sense_key_ccc = 0;
  asc_ccc = 0;
  ascq_ccc = 0;

  if (extended_support_ccc)
  {
    ccc_main_usbbuffer_size_ccc = 32;
    usb_transfer_length_ccc = ccc_main_usbbuffer_size_ccc;
    passthrough_ccc.scsi_cmd[0] = 0x9e; // read capacity 16
    passthrough_ccc.scsi_cmd[1] = 0x10;  // service action
    passthrough_ccc.scsi_cmd[2] = 0;
    passthrough_ccc.scsi_cmd[3] = 0;
    passthrough_ccc.scsi_cmd[4] = 0;
    passthrough_ccc.scsi_cmd[5] = 0;
    passthrough_ccc.scsi_cmd[6] = 0;
    passthrough_ccc.scsi_cmd[7] = 0;
    passthrough_ccc.scsi_cmd[8] = 0;
    passthrough_ccc.scsi_cmd[9] = 0;
    passthrough_ccc.scsi_cmd[10] = 0;
    passthrough_ccc.scsi_cmd[11] = 0;
    passthrough_ccc.scsi_cmd[12] = 0;
    passthrough_ccc.scsi_cmd[13] = 32;  // lenth of return data
    passthrough_ccc.scsi_cmd[14] = 0;
    passthrough_ccc.scsi_cmd[15] = 0;
    command_length_ccc = 16;
    passthrough_ccc.sg_xfer_direction = SG_DXFER_FROM_DEV;
    usb_transfer_direction_ccc = USB_DIRECTION_IN;
  }
  else
  {
    ccc_main_usbbuffer_size_ccc = 8;
    usb_transfer_length_ccc = ccc_main_usbbuffer_size_ccc;
    passthrough_ccc.scsi_cmd[0] = 0x25; // read capacity 10
    passthrough_ccc.scsi_cmd[1] = 0;
    passthrough_ccc.scsi_cmd[2] = 0;
    passthrough_ccc.scsi_cmd[3] = 0;
    passthrough_ccc.scsi_cmd[4] = 0;
    passthrough_ccc.scsi_cmd[5] = 0;
    passthrough_ccc.scsi_cmd[6] = 0;
    passthrough_ccc.scsi_cmd[7] = 0;
    passthrough_ccc.scsi_cmd[8] = 0;
    passthrough_ccc.scsi_cmd[9] = 0;
    command_length_ccc = 10;
    passthrough_ccc.sg_xfer_direction = SG_DXFER_FROM_DEV;
    usb_transfer_direction_ccc = USB_DIRECTION_IN;
  }

  int cbw_ret = 0;
  int raw_ret = 0;
  int csw_ret = 0;
  unsigned char csw_status = 0;
  uint32_t csw_residue = 0;
  usb_tag_ccc++;
  memset (usbcbwbuffer_ccc, 0, USBCBW_BUFFER_SIZE);
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[71], passthrough_ccc.scsi_cmd, command_length_ccc);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[72], &command_length_ccc, 1);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[73], &usb_transfer_direction_ccc, 1);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[73] + 1, &usb_lun_ccc, 1);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[74], &usb_tag_ccc, 4);    //potential superbyte
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[75], &usb_transfer_length_ccc, 4);    //potential superbyte
  uint32_t cbwsig = 0x43425355;
  memcpy (usbcbwbuffer_ccc + superbyte_ccc[76], &cbwsig, 4);    //potential superbyte
  cbw_timeout_ccc = usb_timeout_ccc;
  cbw_ret = do_usb_send_cbw_ccc(cbw_timeout_ccc);
  if (cbw_ret != USBCBW_BUFFER_SIZE)
  {
    fprintf (stdout, "USB readcapacity send cbw short return or error %d (%s)\n", cbw_ret, strerror(abs(cbw_ret)));
    if (cbw_ret < 0)
    {
      return cbw_ret;
    }
    // reset and try again
    usb_reset_recovery_ccc(usb_timeout_ccc);
    cbw_ret = do_usb_send_cbw_ccc(cbw_timeout_ccc);
    if (cbw_ret != USBCBW_BUFFER_SIZE)
    {
      fprintf (stdout, "USB readcapacity send cbw short return or error %d (%s)\n", cbw_ret, strerror(abs(cbw_ret)));
      return INPUT_DEVICE_ERROR_RETURN_CODE;
    }
  }

  set_main_usb_buffer_ccc();
  raw_ret = do_usb_raw_read_ccc(timeout);
  if (raw_ret != (int)usb_transfer_length_ccc)
  {
    fprintf (stdout, "USB readcapacity data transfer short return or error %d (%s)\n", raw_ret, strerror(abs(raw_ret)));
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }

  csw_timeout_ccc = usb_timeout_ccc;
  csw_ret = do_usb_receive_csw_ccc(csw_timeout_ccc);
  if (csw_ret != USBCSW_BUFFER_SIZE)
  {
    fprintf (stdout, "USB readcapacity receive csw short return or error %d (%s)\n", csw_ret, strerror(abs(csw_ret)));
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  uint32_t cswsig;
  memcpy (&cswsig, usbcswbuffer_ccc + 0, 4);
  if (cswsig != 0x53425355)
  {
    fprintf (stdout, "USB readcapacity CSW wrong signature\n");
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  uint32_t tag;
  memcpy (&tag, usbcswbuffer_ccc + 4, 4);
  if (tag != usb_tag_ccc)
  {
    fprintf (stdout, "USB readcapacity CSW tag mismatch\n");
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  memcpy (&csw_residue, usbcswbuffer_ccc + 8, 4);
  if (csw_residue != 0)
  {
    fprintf (stdout, "USB readcapacity CSW data residue %d\n", csw_residue);
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  memcpy (&csw_status, usbcswbuffer_ccc + 12, 1);
  if (csw_status != 0)
  {
    fprintf (stdout, "USB readcapacity CSW status failed %d\n", csw_status);
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }

  uint32_t double_word;
  uint64_t quad_word;
  long long capacity;
  if (extended_support_ccc)
  {
    memcpy(&quad_word, ccc_usbbuffer_ccc + 0, 8);
    capacity = __bswap_64 (quad_word);
  }
  else
  {
    memcpy(&double_word, ccc_usbbuffer_ccc + 0, 4);
    capacity = __bswap_32 (double_word);
  }
  capacity = capacity + 1;
  if (capacity != source_total_size_ccc)
  {
    fprintf (stdout, "drive capacity changed\n");
    return 1;
  }

  return 0;
}







int do_ata28_cmd_ccc(int disk_fd)
{
  // perform direct IO
  if (direct_mode_ccc)
  {
    // clear any possible passthrough data
    sense_key_ccc = 0;
    asc_ccc = 0;
    ascq_ccc = 0;
    //fprintf (stdout, "direction=%d protocol=%d\n", passthrough_ccc.direction, passthrough_ccc.protocol);

    if (passthrough_ccc.direction == 2 && passthrough_ccc.protocol == 4)
    {
      return_value_ccc = do_ata_pio_read_ccc(28);
    }
    else if (passthrough_ccc.direction == 1 && passthrough_ccc.protocol == 5)
    {
      return_value_ccc = do_ata_pio_write_ccc(28);
    }
    else if (passthrough_ccc.direction == 0 && passthrough_ccc.protocol == 3)
    {
      return_value_ccc = do_ata_pio_read_ccc(28);
    }
    else if (passthrough_ccc.direction == 2 && passthrough_ccc.protocol == 6)
    {
      return_value_ccc = do_ata_dma_read_ccc(28);
    }
    else if (passthrough_ccc.direction == 1 && passthrough_ccc.protocol == 6)
    {
      return_value_ccc = do_ata_dma_write_ccc(28);
    }
    else
    {
      sprintf (tempmessage_ccc, "Error! Protocol and direction settings not correct for direct mode.\n");
      if (superclone_ccc)
      {
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
      }
      else
      {
        message_now_ccc(tempmessage_ccc);
      }
      return (-1);
    }
    return (return_value_ccc);
  }


  // perform passthrough
  else
  {
    passthrough_ccc.extended = false;

    passthrough_ccc.scsi_cmd[0] = 0xA1; // ata passthrough 12 command
    passthrough_ccc.scsi_cmd[1] = (passthrough_ccc.multiple_count << 5) + (passthrough_ccc.protocol << 1);
    passthrough_ccc.scsi_cmd[2] = (passthrough_ccc.off_line << 6) + (passthrough_ccc.check_condition << 5) + (passthrough_ccc.transfer_direction << 3) + ( passthrough_ccc.byte_block << 2) + (passthrough_ccc.transfer_length);
    passthrough_ccc.scsi_cmd[3] = passthrough_ccc.ata_cmd[0];
    passthrough_ccc.scsi_cmd[4] = passthrough_ccc.ata_cmd[1];
    passthrough_ccc.scsi_cmd[5] = passthrough_ccc.ata_cmd[2];
    passthrough_ccc.scsi_cmd[6] = passthrough_ccc.ata_cmd[3];
    passthrough_ccc.scsi_cmd[7] = passthrough_ccc.ata_cmd[4];
    passthrough_ccc.scsi_cmd[8] = passthrough_ccc.ata_cmd[5];
    passthrough_ccc.scsi_cmd[9] = passthrough_ccc.ata_cmd[6];
    passthrough_ccc.scsi_cmd[10] = 0;
    passthrough_ccc.scsi_cmd[11] = 0;

    prepare_cdb_ccc();

    int status = 0;
    int ioctl_ret = ioctl(disk_fd, SG_IO, &io_hdr);
    if (ioctl_ret < 0)
    {
      if (identify_flag_ccc)
      {
        sprintf(tempmessage_ccc, "Bad ioctl accessing input device(%s)", strerror(errno));
        if (superclone_ccc)
        {
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
          clear_error_message_ccc();
        }
        else
        {
          message_exit_ccc(tempmessage_ccc);
        }
        command_status_ccc = 1;
      }
      else
      {
        sprintf(tempmessage_ccc, "Error performing passthrough IO on input device (%s).\n", strerror(errno));
        if (superclone_ccc)
        {
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
          clear_error_message_ccc();
          status = INPUT_DEVICE_ERROR_RETURN_CODE;
        }
        else
        {
          message_now_ccc(tempmessage_ccc);
          status = INPUT_DEVICE_ERROR_RETURN_CODE;
        }
        set_number_variable_value_ccc("$data_transferred", 0);
        set_number_variable_value_ccc("$command_status", 1);
      }
    }
    else
    {
      if (identify_flag_ccc)
      {
        command_status_ccc = 0;
      }
      else
      {
        set_number_variable_value_ccc("$data_transferred", ccc_main_buffer_size_ccc);
        set_number_variable_value_ccc("$command_status", 0);
      }

    }

    int ret = post_ioctl_ccc();
    if (ret)
    {
      return ret;
    }
    return (status);

  }
  return (0);
}







int do_ata48_cmd_ccc(int disk_fd)
{
  // perform direct IO
  if (direct_mode_ccc)
  {
    // clear any possible passthrough data
    sense_key_ccc = 0;
    asc_ccc = 0;
    ascq_ccc = 0;
    //fprintf (stdout, "direction=%d protocol=%d\n", passthrough_ccc.direction, passthrough_ccc.protocol);

    if (passthrough_ccc.direction == 2 && passthrough_ccc.protocol == 4)
    {
      return_value_ccc = do_ata_pio_read_ccc(48);
    }
    else if (passthrough_ccc.direction == 1 && passthrough_ccc.protocol == 5)
    {
      return_value_ccc = do_ata_pio_write_ccc(48);
    }
    else if (passthrough_ccc.direction == 0 && passthrough_ccc.protocol == 3)
    {
      return_value_ccc = do_ata_pio_read_ccc(48);
    }
    else if (passthrough_ccc.direction == 2 && (passthrough_ccc.protocol == 6 || passthrough_ccc.protocol == 12))
    {
      return_value_ccc = do_ata_dma_read_ccc(48);
    }
    else if (passthrough_ccc.direction == 1 && (passthrough_ccc.protocol == 6 || passthrough_ccc.protocol == 12))
    {
      return_value_ccc = do_ata_dma_write_ccc(48);
    }
    else
    {
      sprintf (tempmessage_ccc, "Error! Protocol and direction settings not correct for direct mode.\n");
      if (superclone_ccc)
      {
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
      }
      else
      {
        message_now_ccc(tempmessage_ccc);
      }
      return (-1);
    }
    return (return_value_ccc);
  }


  // perform passthrough
  else
  {
    passthrough_ccc.extended = true;

    passthrough_ccc.scsi_cmd[0] = 0x85; // ata passthrough 16 command
    passthrough_ccc.scsi_cmd[1] = (passthrough_ccc.multiple_count << 5) + (passthrough_ccc.protocol << 1) + passthrough_ccc.extended;
    passthrough_ccc.scsi_cmd[2] = (passthrough_ccc.off_line << 6) + (passthrough_ccc.check_condition << 5) + (passthrough_ccc.transfer_direction << 3) + ( passthrough_ccc.byte_block << 2) + (passthrough_ccc.transfer_length);
    passthrough_ccc.scsi_cmd[3] = passthrough_ccc.ata_cmd[0];
    passthrough_ccc.scsi_cmd[4] = passthrough_ccc.ata_cmd[1];
    passthrough_ccc.scsi_cmd[5] = passthrough_ccc.ata_cmd[2];
    passthrough_ccc.scsi_cmd[6] = passthrough_ccc.ata_cmd[3];
    passthrough_ccc.scsi_cmd[7] = passthrough_ccc.ata_cmd[4];
    passthrough_ccc.scsi_cmd[8] = passthrough_ccc.ata_cmd[5];
    passthrough_ccc.scsi_cmd[9] = passthrough_ccc.ata_cmd[6];
    passthrough_ccc.scsi_cmd[10] = passthrough_ccc.ata_cmd[7];
    passthrough_ccc.scsi_cmd[11] = passthrough_ccc.ata_cmd[8];
    passthrough_ccc.scsi_cmd[12] = passthrough_ccc.ata_cmd[9];
    passthrough_ccc.scsi_cmd[13] = passthrough_ccc.ata_cmd[10];
    passthrough_ccc.scsi_cmd[14] = passthrough_ccc.ata_cmd[11];
    passthrough_ccc.scsi_cmd[15] = 0;

    prepare_cdb_ccc();

    int status = 0;
    int ioctl_ret = ioctl(disk_fd, SG_IO, &io_hdr);
    if (ioctl_ret < 0)
    {
      sprintf(tempmessage_ccc, "Error performing passthrough IO on input device (%s).\n", strerror(errno));
      if (superclone_ccc)
      {
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        status = INPUT_DEVICE_ERROR_RETURN_CODE;
      }
      else
      {
        message_now_ccc(tempmessage_ccc);
        status = INPUT_DEVICE_ERROR_RETURN_CODE;
      }
      set_number_variable_value_ccc("$data_transferred", 0);
      set_number_variable_value_ccc("$command_status", 1);
    }
    else
    {
      set_number_variable_value_ccc("$data_transferred", ccc_main_buffer_size_ccc);
      set_number_variable_value_ccc("$command_status", 0);
    }

    int ret = post_ioctl_ccc();
    if (ret)
    {
      return ret;
    }
    return (status);

  }
  return (0);
}






int do_ata_pio_read_ccc(int command_type)
{
  // ahci
  if (ahci_mode_ccc)
  {
    return_value_ccc = ahci_rw_ccc(command_type, 0);
    return (return_value_ccc);
  }


  // direct
  else
  {
    set_number_variable_value_ccc("$data_transferred", 0);
    int fail_level = 0x0;
    int success = 1;

    // set device, this is important to do first to make sure the proper drive is selected
    set_device_ccc(reg_base_address_ccc, device_select_base_ccc);
    // tell device not to send interrupts
    outb(2, control_base_address_ccc);

    // wait for drive to be ready
    return_value_ccc = wait_not_busy_or_drq_ccc(initial_busy_wait_time_ccc, 0);
    if (stop_signal_ccc)
    {
      return STOP_SIGNAL_RETURN_CODE;
    }
    if (return_value_ccc != 0)
    {
      return_value_ccc = soft_reset_ccc(0);
    }
    if (return_value_ccc != 0)
    {
      success = 0;
      fail_level = 0x20 + return_value_ccc;
    }
    // do this again in case there was a reset event
    // set device, this is important to do first to make sure the proper drive is selected
    set_device_ccc(reg_base_address_ccc, device_select_base_ccc);
    // tell device not to send interrupts
    outb(2, control_base_address_ccc);


    if (success)
    {
      set_and_send_regs_ccc(command_type);

      // wait while busy, if timeout do soft reset
      return_value_ccc = wait_not_busy_ccc(soft_reset_time_ccc);
      if (stop_signal_ccc)
      {
        return STOP_SIGNAL_RETURN_CODE;
      }
      if (return_value_ccc != 0)
      {
        success = 0;
        // only do reset if soft reset time exceeded, but not if general timeout or fault
        if (return_value_ccc == 1)
        {
          fail_level = 0x30 + return_value_ccc;
          return_value_ccc = soft_reset_ccc(0);
          if (return_value_ccc != 0)
          {
            fail_level = 0x40 + return_value_ccc;
          }
        }
        else
        {
          fail_level = 0x50 + return_value_ccc;
        }
      }


      // get the data if drq is set and buffer size is not 0 and no reset
      if (ccc_main_buffer_size_ccc > 0 && success)
      {
        int drq = 0;
        int busy = 0;
        io_byte_ccc[7] = inb(reg_base_address_ccc+7);    //status
        //io_byte_ccc[8] = inb(control_base_address_ccc);    //altstatus
        //fprintf (stdout, "%02x ", io_byte_ccc[7]);
        //fprintf (stdout, "\n");
        drq = io_byte_ccc[7] & 0x08;    //status
        busy = io_byte_ccc[7] & 0x80;    //status
        //drq = io_byte_ccc[8] & 0x08;    //altstatus
        //busy = io_byte_ccc[8] & 0x80;    //altstatus
        if (drq && !busy)
        {
          unsigned int n;
          for (n = 0; n < ccc_main_buffer_size_ccc; n += 2)
          {
            io_word_ccc = inw(reg_base_address_ccc);
            memcpy(ccc_buffer_ccc + n, &io_word_ccc, 2);
            io_byte_ccc[7] = inb(reg_base_address_ccc+7);    //status
            //io_byte_ccc[8] = inb(control_base_address_ccc);    //altstatus
            memcpy(&ata_error_ccc, &io_byte_ccc[1], 1);
            memcpy(&ata_status_ccc, &io_byte_ccc[7], 1);    //status
            //memcpy(&ata_status_ccc, &io_byte_ccc[8], 1);    //altstatus
            update_display_status_buttons_ccc(DISPLAY_STATUS_UPDATE_TIME);
            drq = io_byte_ccc[7] & 0x08;    //status
            busy = io_byte_ccc[7] & 0x80;    //status
            //drq = io_byte_ccc[8] & 0x08;    //altstatus
            //busy = io_byte_ccc[8] & 0x80;    //altstatus
            unsigned long long start_time = get_elapsed_usec_ccc();
            while (!drq || busy)
            {
              io_byte_ccc[1] = inb(reg_base_address_ccc+1);
              io_byte_ccc[7] = inb(reg_base_address_ccc+7);    //status
              //io_byte_ccc[8] = inb(control_base_address_ccc);    //altstatus
              memcpy(&ata_error_ccc, &io_byte_ccc[1], 1);
              memcpy(&ata_status_ccc, &io_byte_ccc[7], 1);    //status
              //memcpy(&ata_status_ccc, &io_byte_ccc[8], 1);    //altstatus
              update_display_status_buttons_ccc(DISPLAY_STATUS_UPDATE_TIME);
              if (stop_signal_ccc)
              {
                break;
              }
              drq = io_byte_ccc[7] & 0x08;    //status
              busy = io_byte_ccc[7] & 0x80;    //status
              //drq = io_byte_ccc[8] & 0x08;    //altstatus
              //busy = io_byte_ccc[8] & 0x80;    //altstatus
              if (!busy & !drq)
              {
                // if busy and drq are not set the command is done
                break;
              }

              // anti loop timer
              unsigned long long elapsed_time = get_elapsed_usec_ccc();
              if (elapsed_time > start_time + soft_reset_time_ccc)
              {
                // if it times out then fail
                success = 0;
                fail_level = 0x60;
                return_value_ccc = soft_reset_ccc(0);
                if (return_value_ccc != 0)
                {
                  fail_level = 0x60 + return_value_ccc;
                }
                break;
              }
              if (elapsed_time > start_time + general_timeout_ccc)
              {
                // if it exceeds general timeout then quit
                success = 0;
                fail_level = 0x60;
                break;
              }

            }
            if (!success)
            {
              break;
            }
          }
          set_number_variable_value_ccc("$data_transferred", n);
        }
        else
        {
          fail_level = 0x60;
          success = 0;
        }
      }
    }

    // wait up to 500ms for drive to be ready after successful command so the status is correct
    if (success)
    {
      wait_not_busy_or_drq_ccc(500000, 0);
    }

    return_value_ccc = post_direct_ccc(command_type);
    // if success then check if still correct device
    if (return_value_ccc && success)
    {
      fail_level = 0x70 + return_value_ccc;
      success = 0;
    }

    set_number_variable_value_ccc("$command_status", fail_level);
    if (superclone_ccc)
    {
      return (fail_level);
    }
    else
    {
      return 0;
    }
  }
  return (0);
}





int ahci_rw_ccc(int command_type, int write_bit)
{
  if (superbyte_ccc[30] == 0xd9)
  {
    set_number_variable_value_ccc("$data_transferred", 0);
    int fail_level = 0x0;
    int success = 1;
    int what_changed = 0;

    // clear any interrupt bits
    ahci_interrupt_changed_ccc = true;
    memset(port_virt_addr_ccc + superbyte_ccc[7], 0xff, 4);    //potential superbyte

    // clear all error bits
    memset(port_virt_addr_ccc + superbyte_ccc[18], 0xff, 4);    //potential superbyte

    // wait for drive to be ready
    return_value_ccc = wait_not_busy_or_drq_ccc(initial_busy_wait_time_ccc, 0);
    //fprintf (stdout, "return from initial wait = %d\n", return_value_ccc);    //debug
    if (stop_signal_ccc)
    {
      return STOP_SIGNAL_RETURN_CODE;
    }
    if (return_value_ccc != 0)
    {
      int status = do_soft_reset_ccc(0);
      if (status)
      {
        success = 0;
        fail_level = 0x20 + return_value_ccc;
      }
    }

    if (success)
    {
      #ifdef DEBUG
      int i;
      for (i = 0; i < 0x80; i+=4)
      {
        memcpy(&io_doubleword_ccc, port_virt_addr_ccc + i, 4);
        //fprintf (stdout, "%x %08x\n", i, io_doubleword_ccc);  //debug
      }
      #endif

      // clear the command list
      memset(command_list_buffer_ccc, 0, command_list_size_ccc);
      // set the command FIS length, 5 dwords for h2d, also contains read/write bit
      io_singlebyte_ccc = 5;
      io_singlebyte_ccc = io_singlebyte_ccc | ((write_bit & 1) << 6);
      memcpy(command_list_buffer_ccc, &io_singlebyte_ccc, 1);

      set_and_send_regs_ccc(command_type);    // this does not send for ahci


      // set the start bit
      enable_start_ccc(START_BIT_TIME);

      if (wait_for_ds_bit_ccc)
      {
        // set the device status bit
        io_doubleword_ccc = 1;
        memcpy(port_virt_addr_ccc + superbyte_ccc[69], &io_doubleword_ccc, 4);    //potential superbyte
      }

      // set the command issue bit
      enable_command_issue_ccc(COMMAND_BIT_TIME);

      // wait while busy, if timeout do soft reset
      return_value_ccc = wait_not_busy_or_drq_ccc(soft_reset_time_ccc + first_read_time_ccc, 1);
      first_read_time_ccc = 0;
      //fprintf (stdout, "return from wait = %d\n", return_value_ccc);    //debug
      wait_for_ds_bit_ccc = false;
      if (stop_signal_ccc)
      {
        return STOP_SIGNAL_RETURN_CODE;
      }
      if (return_value_ccc != 0)
      {
        success = 0;
        // if err was set for fpdma command then read the ncq error log
        if (return_value_ccc == 4)
        {
          //fprintf (stdout, "port status before ncq read log = 0x%08x\n", get_ahci_status_ccc());    //debug
          //read_ncq_error_log_ccc();
          //fprintf (stdout, "port status after ncq read log = 0x%08x\n", get_ahci_status_ccc());    //debug
        }
        // only do reset if soft reset time exceeded, but not if general timeout or fault
        else if (return_value_ccc == 1)
        {
          fail_level = 0x30 + return_value_ccc;
          return_value_ccc = soft_reset_ccc(0);
          if (return_value_ccc != 0)
          {
            fail_level = 0x40 + return_value_ccc;
          }
        }
        // if it failed the change check and drive is hung then do reset
        else if (return_value_ccc == 8)
        {
          what_changed = check_for_unwanted_changes_ccc();
          return_value_ccc = wait_not_busy_or_drq_ccc(1000, 0);
          if (return_value_ccc)
          {
            return_value_ccc = soft_reset_ccc(0);
          }
          fail_level = 0x80 + return_value_ccc;
        }
        else
        {
          fail_level = 0x50 + return_value_ccc;
        }
      }

      // check for unwanted changes
      if (success)
      {
        what_changed = check_for_unwanted_changes_ccc();
        // get port status/error
        unsigned int port_error = get_ahci_error_ccc();
        if (port_error)
        {
          //what_changed = what_changed | 0x2000;
          //memcpy(&io_byte_ccc[1], port_virt_addr_ccc + superbyte_ccc[8], 1);    // status    //potential superbyte
          //memcpy(&io_byte_ccc[2], port_virt_addr_ccc + superbyte_ccc[9], 1);    // error    //potential superbyte
          //sprintf (tempmessage_ccc, "port ERR = 0x%08x s/e=0x%02x/0x%02x fail=0x%04x changed=0x%04x\n", port_error, io_byte_ccc[1], io_byte_ccc[2], fail_level, what_changed);
          sprintf (tempmessage_ccc, "port ERR = 0x%08x\n", port_error);
          message_console_log_ccc(tempmessage_ccc, 0);
          //fprintf (stdout, "port ERR = 0x%08x\n", port_error);
        }
        unsigned int port_status = get_ahci_status_ccc();
        if (port_status && !use_fpdma_ccc)
        {
          //what_changed = what_changed | 0x2000;
          //memcpy(&io_byte_ccc[1], port_virt_addr_ccc + superbyte_ccc[8], 1);    // status    //potential superbyte
          //memcpy(&io_byte_ccc[2], port_virt_addr_ccc + superbyte_ccc[9], 1);    // error    //potential superbyte
          //sprintf (tempmessage_ccc, "port DS = 0x%08x s/e=0x%02x/0x%02x fail=0x%04x changed=0x%04x\n", port_status, io_byte_ccc[1], io_byte_ccc[2], fail_level, what_changed);
          sprintf (tempmessage_ccc, "port DS = 0x%08x\n", port_status);
          message_console_log_ccc(tempmessage_ccc, 0);
          //fprintf (stdout, "port DS = 0x%08x\n", port_status);
        }
      }

    }
    fail_level = fail_level + what_changed;

    disable_start_ccc(START_BIT_TIME);

    // clear any interrupt bits
    ahci_interrupt_changed_ccc = true;
    memset(port_virt_addr_ccc + superbyte_ccc[7], 0xff, 4);    //potential superbyte

    // clear all error bits
    memset(port_virt_addr_ccc + superbyte_ccc[18], 0xff, 4);    //potential superbyte

    // get recent copy of status and error
    memcpy(&io_byte_ccc[1], port_virt_addr_ccc + superbyte_ccc[8], 1);    // status    //potential superbyte
    memcpy(&io_byte_ccc[2], port_virt_addr_ccc + superbyte_ccc[9], 1);    // error    //potential superbyte


    if (success)
    {
      set_number_variable_value_ccc("$data_transferred", ccc_main_buffer_size_ccc);
    }

    return_value_ccc = post_direct_ccc(command_type);

    set_number_variable_value_ccc("$command_status", fail_level);
    if (superclone_ccc)
    {
      return (fail_level);
    }
    else
    {
      return 0;
    }
  }
  else
  {
    command_type = command_type;
    write_bit = write_bit;
  }
  return 0;
}





int do_ata_dma_read_ccc(int command_type)
{
  if (superbyte_ccc[31] == 0x9c)
  {
    if (ccc_main_buffer_size_ccc > max_dma_size_ccc)
    {
      sprintf (tempmessage_ccc, "ERROR: Maximum DMA buffer size (%lld) exceeded.\n", max_dma_size_ccc);
      if (superclone_ccc)
      {
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 0);
        clear_error_message_ccc();
      }
      else
      {
        message_now_ccc(tempmessage_ccc);
      }
      return (-1);
    }
    // ahci
    if (ahci_mode_ccc)
    {
      return_value_ccc = ahci_rw_ccc(command_type, 0);
      return (return_value_ccc);
    }


    // direct
    else
    {
      set_number_variable_value_ccc("$data_transferred", 0);
      int fail_level = 0x0;
      int success = 1;
      unsigned char c;

      // set device, this is important to do first to make sure the proper drive is selected
      set_device_ccc(reg_base_address_ccc, device_select_base_ccc);
      // tell device not to send interrupts
      outb(2, control_base_address_ccc);

      // wait for drive to be ready
      return_value_ccc = wait_not_busy_or_drq_ccc(initial_busy_wait_time_ccc, 0);
      if (stop_signal_ccc)
      {
        return STOP_SIGNAL_RETURN_CODE;
      }
      if (return_value_ccc != 0)
      {
        return_value_ccc = soft_reset_ccc(0);
      }
      if (return_value_ccc != 0)
      {
        success = 0;
        fail_level = 0x20 + return_value_ccc;
      }
      // do this again in case there was a reset event
      // set device, this is important to do first to make sure the proper drive is selected
      set_device_ccc(reg_base_address_ccc, device_select_base_ccc);
      // tell device not to send interrupts
      outb(2, control_base_address_ccc);

      if (success)
      {
        // put table address into controler
        set_table_address_ccc();

        // Clear the Interrupt bit and Error bit in the Status register.
        c = superbyte_ccc[1];    //potential superbyte
        outb(c, bus_base_address_ccc+2);

        // set the read/write bit to 1 (read)
        c = superbyte_ccc[2];    //potential superbyte
        outb(c, bus_base_address_ccc);

        set_and_send_regs_ccc(command_type);

        // set the start/stop bit
        c = superbyte_ccc[3];    //potential superbyte
        outb(c, bus_base_address_ccc);

        // wait while busy or drq, if timeout do soft reset
        return_value_ccc = wait_not_busy_or_drq_ccc(soft_reset_time_ccc + first_read_time_ccc, 0);
        first_read_time_ccc = 0;
        if (stop_signal_ccc)
        {
          return STOP_SIGNAL_RETURN_CODE;
        }
        if (return_value_ccc != 0)
        {
          success = 0;
          // only do reset if soft reset time exceeded, but not if general timeout or fault
          if (return_value_ccc == 1)
          {
            fail_level = 0x30 + return_value_ccc;
            return_value_ccc = soft_reset_ccc(0);
            if (return_value_ccc != 0)
            {
              fail_level = 0x40 + return_value_ccc;
            }
          }
          else
          {
            fail_level = 0x50 + return_value_ccc;
          }
        }

        // clear the start/stop bit
        c = superbyte_ccc[2];    //potential superbyte
        outb(c, bus_base_address_ccc);

        // read the bus master status byte to flush cache and finalize transfer
        io_byte_ccc[8] = inb(bus_base_address_ccc+2);

        // Clear the Interrupt bit and Error bit in the Status register.
        c = superbyte_ccc[1];    //potential superbyte
        outb(c, bus_base_address_ccc+2);

        //fprintf (stdout, "%02x\n", io_byte_ccc[8]);
        set_number_variable_value_ccc("$bus_master_status", io_byte_ccc[8]);
      }


      if (success)
      {
        set_number_variable_value_ccc("$data_transferred", ccc_main_buffer_size_ccc);
      }

      return_value_ccc = post_direct_ccc(command_type);
      // if success then check if still correct device
      if (return_value_ccc && success)
      {
        fail_level = 0x70 + return_value_ccc;
        success = 0;
      }

      set_number_variable_value_ccc("$command_status", fail_level);
      if (superclone_ccc)
      {
        return (fail_level);
      }
      else
      {
        return 0;
      }
    }
  }
  else
  {
    command_type = command_type;
    sprintf (tempmessage_ccc, "ERROR: DMA not allowed in free version.\n");
    if (superclone_ccc)
    {
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
    }
    else
    {
      message_now_ccc(tempmessage_ccc);
    }
    return (-1);
  }
  return (0);
}





int do_ata_pio_write_ccc(int command_type)
{
  // ahci
  if (ahci_mode_ccc)
  {
    return_value_ccc = ahci_rw_ccc(command_type, 1);
    return (return_value_ccc);
  }


  // direct
  else
  {
    set_number_variable_value_ccc("$data_transferred", 0);
    int fail_level = 0x0;
    int success = 1;

    // set device, this is important to do first to make sure the proper drive is selected
    set_device_ccc(reg_base_address_ccc, device_select_base_ccc);
    // tell device not to send interrupts
    outb(2, control_base_address_ccc);

    // wait for drive to be ready
    return_value_ccc = wait_not_busy_or_drq_ccc(initial_busy_wait_time_ccc, 0);
    if (stop_signal_ccc)
    {
      return STOP_SIGNAL_RETURN_CODE;
    }
    if (return_value_ccc != 0)
    {
      return_value_ccc = soft_reset_ccc(0);
    }
    if (return_value_ccc != 0)
    {
      success = 0;
      fail_level = 0x20 + return_value_ccc;
    }
    // do this again in case there was a reset event
    // set device, this is important to do first to make sure the proper drive is selected
    set_device_ccc(reg_base_address_ccc, device_select_base_ccc);
    // tell device not to send interrupts
    outb(2, control_base_address_ccc);


    if (success)
    {
      set_and_send_regs_ccc(command_type);

      // wait while busy, if timeout do soft reset
      return_value_ccc = wait_not_busy_ccc(soft_reset_time_ccc);
      if (stop_signal_ccc)
      {
        return STOP_SIGNAL_RETURN_CODE;
      }
      if (return_value_ccc != 0)
      {
        success = 0;
        // only do reset if soft reset time exceeded, but not if general timeout
        if (return_value_ccc == 1)
        {
          fail_level = 0x30 + return_value_ccc;
          return_value_ccc = soft_reset_ccc(0);
          if (return_value_ccc != 0)
          {
            fail_level = 0x40 + return_value_ccc;
          }
        }
        else
        {
          fail_level = 0x50 + return_value_ccc;
        }
      }


      // write the data if drq is set and buffer size is not 0 and no reset
      if (ccc_main_buffer_size_ccc > 0 && success)
      {
        int drq = 0;
        int busy = 0;
        io_byte_ccc[7] = inb(reg_base_address_ccc+7);    //status
        //io_byte_ccc[8] = inb(control_base_address_ccc);    //altstatus
        //fprintf (stdout, "%02x ", io_byte_ccc[7]);
        //fprintf (stdout, "\n");
        drq = io_byte_ccc[7] & 0x08;    //status
        busy = io_byte_ccc[7] & 0x80;    //status
        //drq = io_byte_ccc[8] & 0x08;    //altstatus
        //busy = io_byte_ccc[8] & 0x80;    //altstatus
        if (drq && !busy)
        {
          unsigned int n;
          for (n = 0; n < ccc_main_buffer_size_ccc; n += 2)
          {
            memcpy(&io_word_ccc, ccc_buffer_ccc + n, 2);
            outw(io_word_ccc, reg_base_address_ccc);
            io_byte_ccc[7] = inb(reg_base_address_ccc+7);    //status
            //io_byte_ccc[8] = inb(control_base_address_ccc);    //altstatus
            drq = io_byte_ccc[7] & 0x08;    //status
            busy = io_byte_ccc[7] & 0x80;    //status
            //drq = io_byte_ccc[8] & 0x08;    //altstatus
            //busy = io_byte_ccc[8] & 0x80;    //altstatus
            unsigned long long start_time = get_elapsed_usec_ccc();
            while (!drq || busy)
            {
              io_byte_ccc[7] = inb(reg_base_address_ccc+7);    //status
              //io_byte_ccc[8] = inb(control_base_address_ccc);    //altstatus
              drq = io_byte_ccc[7] & 0x08;    //status
              busy = io_byte_ccc[7] & 0x80;    //status
              //drq = io_byte_ccc[8] & 0x08;    //altstatus
              //busy = io_byte_ccc[8] & 0x80;    //altstatus
              if (!busy & !drq)
              {
                // if busy and drq are not set the command is done
                break;
              }

              // anti loop timer
              unsigned long long elapsed_time = get_elapsed_usec_ccc();
              if (elapsed_time > start_time + soft_reset_time_ccc)
              {
                // if it times out then fail
                success = 0;
                fail_level = 0x60;
                break;
              }
              if (elapsed_time > start_time + general_timeout_ccc)
              {
                // if it exceeds general timeout then quit
                success = 0;
                fail_level = 0x60;
                break;
              }

            }
            if (!success)
            {
              break;
            }
          }
          set_number_variable_value_ccc("$data_transferred", n);
        }
        else
        {
          fail_level = 0x60;
          success = 0;
        }
      }
    }

    // wait up to 500ms for drive to be ready after successful command so the status is correct
    if (success)
    {
      wait_not_busy_or_drq_ccc(500000, 0);
    }

    return_value_ccc = post_direct_ccc(command_type);
    // if success then check if still correct device
    if (return_value_ccc && success)
    {
      fail_level = 0x70 + return_value_ccc;
      success = 0;
    }

    set_number_variable_value_ccc("$command_status", fail_level);
    if (superclone_ccc)
    {
      return (fail_level);
    }
    else
    {
      return 0;
    }
  }
  return (0);
}





int do_ata_dma_write_ccc(int command_type)
{
  if (superbyte_ccc[32] == 0x83)
  {
    if (ccc_main_buffer_size_ccc > max_dma_size_ccc)
    {
      sprintf (tempmessage_ccc, "ERROR: Maximum DMA buffer size (%lld) exceeded.\n", max_dma_size_ccc);
      if (superclone_ccc)
      {
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
      }
      else
      {
        message_now_ccc(tempmessage_ccc);
      }
      return (-1);
    }
    // ahci
    if (ahci_mode_ccc)
    {
      return_value_ccc = ahci_rw_ccc(command_type, 1);
      return (return_value_ccc);
    }


    // ide
    else
    {
      set_number_variable_value_ccc("$data_transferred", 0);
      int fail_level = 0x0;
      int success = 1;
      unsigned char c;

      // set device, this is important to do first to make sure the proper drive is selected
      set_device_ccc(reg_base_address_ccc, device_select_base_ccc);
      // tell device not to send interrupts
      outb(2, control_base_address_ccc);

      // wait for drive to be ready
      return_value_ccc = wait_not_busy_or_drq_ccc(initial_busy_wait_time_ccc, 0);
      if (stop_signal_ccc)
      {
        return STOP_SIGNAL_RETURN_CODE;
      }
      if (return_value_ccc != 0)
      {
        return_value_ccc = soft_reset_ccc(0);
      }
      if (return_value_ccc != 0)
      {
        success = 0;
        fail_level = 0x10 + return_value_ccc;
      }
      // do this again in case there was a reset event
      // set device, this is important to do first to make sure the proper drive is selected
      set_device_ccc(reg_base_address_ccc, device_select_base_ccc);
      // tell device not to send interrupts
      outb(2, control_base_address_ccc);

      if (success)
      {
        // put table address into controler
        set_table_address_ccc();

        // Clear the Interrupt bit and Error bit in the Status register.
        c = superbyte_ccc[1];    //potential superbyte
        outb(c, bus_base_address_ccc+2);

        // set the read/write bit to 0 (write)
        c = superbyte_ccc[4];    //potential superbyte
        outb(c, bus_base_address_ccc);

        set_and_send_regs_ccc(command_type);

        // set the start/stop bit
        c = superbyte_ccc[5];    //potential superbyte
        outb(c, bus_base_address_ccc);

        // wait while busy or drq, if timeout do soft reset
        return_value_ccc = wait_not_busy_or_drq_ccc(soft_reset_time_ccc + first_read_time_ccc, 0);
        first_read_time_ccc = 0;
        if (stop_signal_ccc)
        {
          return STOP_SIGNAL_RETURN_CODE;
        }
        if (return_value_ccc != 0)
        {
          success = 0;
          // only do reset if soft reset time exceeded, but not if general timeout or fault
          if (return_value_ccc == 1)
          {
            fail_level = 0x30 + return_value_ccc;
            return_value_ccc = soft_reset_ccc(0);
            if (return_value_ccc != 0)
            {
              fail_level = 0x40 + return_value_ccc;
            }
          }
          else
          {
            fail_level = 0x50 + return_value_ccc;
          }
        }

        // clear the start/stop bit
        c = superbyte_ccc[4];    //potential superbyte
        outb(c, bus_base_address_ccc);

        // read the bus master status byte to flush cache and finalize transfer
        io_byte_ccc[8] = inb(bus_base_address_ccc+2);

        // Clear the Interrupt bit and Error bit in the Status register.
        c = superbyte_ccc[1];    //potential superbyte
        outb(c, bus_base_address_ccc+2);

        //fprintf (stdout, "%02x\n", io_byte_ccc[8]);
        set_number_variable_value_ccc("$bus_master_status", io_byte_ccc[8]);
      }

      if (success)
      {
        set_number_variable_value_ccc("$data_transferred", ccc_main_buffer_size_ccc);
      }

      return_value_ccc = post_direct_ccc(command_type);
      // if success then check if still correct device
      if (return_value_ccc && success)
      {
        fail_level = 0x70 + return_value_ccc;
        success = 0;
      }

      set_number_variable_value_ccc("$command_status", fail_level);
      if (superclone_ccc)
      {
        return (fail_level);
      }
      else
      {
        return 0;
      }
    }
  }
  else
  {
    command_type = command_type;
    sprintf (tempmessage_ccc, "ERROR: DMA not allowed in free version.\n");
    if (superclone_ccc)
    {
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
    }
    else
    {
      message_now_ccc(tempmessage_ccc);
    }
    return (-1);
  }
  return (0);
}








int set_table_address_ccc(void)
{
  if (superbyte_ccc[33] == 0xb0)
  {
    // save current table address
    table_address_backup_ccc[0] = inb(bus_base_address_ccc+0+superbyte_ccc[0]);    //potential superbyte
    table_address_backup_ccc[1] = inb(bus_base_address_ccc+1+superbyte_ccc[0]);    //potential superbyte
    table_address_backup_ccc[2] = inb(bus_base_address_ccc+2+superbyte_ccc[0]);    //potential superbyte
    table_address_backup_ccc[3] = inb(bus_base_address_ccc+3+superbyte_ccc[0]);    //potential superbyte
    table_address_changed_ccc = true;

    // put table address into controler
    unsigned char c;
    c = table_physical_address_ccc;
    outb(c, bus_base_address_ccc+0+superbyte_ccc[0]);    //potential superbyte
    c = table_physical_address_ccc >> 8;
    outb(c, bus_base_address_ccc+1+superbyte_ccc[0]);    //potential superbyte
    c = table_physical_address_ccc >> 16;
    outb(c, bus_base_address_ccc+2+superbyte_ccc[0]);    //potential superbyte
    c = table_physical_address_ccc >> 24;
    outb(c, bus_base_address_ccc+3+superbyte_ccc[0]);    //potential superbyte

    #ifdef DEBUG
    if (debug_ccc & DEBUG34)
    {
      fprintf (stdout, "table_physical_address=0x%llx\n", table_physical_address_ccc);
      int size = 512;
      int i;
      for (i = 0; i < size; i+=16)
      {
        fprintf (stdout, "%x: ", i);
        unsigned char *c;
        int n;
        for (n=0; n < 16 && i+n < size; n++)
        {
          c = (unsigned char *)table_buffer_ccc+i+n;
          fprintf (stdout, "%02x ", *c);
        }
        fprintf (stdout, "   ");
        for (n=0; n < 16 && i+n < size; n++)
        {
          c = (unsigned char *)table_buffer_ccc+i+n;
          fprintf (stdout, "%c", isprint(*c) ? *c : '.');
        }
        fprintf (stdout, "\n");
      }
    }
    #endif
  }

  return (0);
}





int restore_table_address_ccc(void)
{
  if (superbyte_ccc[34] == 0x51)
  {
    outb(table_address_backup_ccc[0], bus_base_address_ccc+0+superbyte_ccc[0]);    //potential superbyte
    outb(table_address_backup_ccc[1], bus_base_address_ccc+1+superbyte_ccc[0]);    //potential superbyte
    outb(table_address_backup_ccc[2], bus_base_address_ccc+2+superbyte_ccc[0]);    //potential superbyte
    outb(table_address_backup_ccc[3], bus_base_address_ccc+3+superbyte_ccc[0]);    //potential superbyte
    table_address_changed_ccc = false;
  }
  return (0);
}





int do_soft_reset_ccc(int disk_fd)
{
  set_number_variable_value_ccc("$data_transferred", 0);
  int status = 0;
  if (direct_mode_ccc)
  {
    if (ahci_mode_ccc)
    {
      // ahci
      status = soft_reset_ccc(0);
      set_number_variable_value_ccc("$command_status", status);
    }

    else
    {
      // ide
      status = soft_reset_ccc(0);
      set_number_variable_value_ccc("$command_status", status);
    }
  }
  else
  {
    status = soft_reset_ccc(disk_fd);
  }
  return (status);
}





int do_hard_reset_ccc(int disk_fd)
{
  set_number_variable_value_ccc("$data_transferred", 0);
  int status = 0;
  if (direct_mode_ccc)
  {
    if (ahci_mode_ccc)
    {
      // ahci
      unsigned long long hard_reset_time_ccc_backup = hard_reset_time_ccc;
      hard_reset_time_ccc = 0;
      status = soft_reset_ccc(0);
      hard_reset_time_ccc = hard_reset_time_ccc_backup;
      set_number_variable_value_ccc("$command_status", status);
    }

    else
    {
      // ide
      int status = soft_reset_ccc(0);
      set_number_variable_value_ccc("$command_status", status);
    }
  }
  else
  {
    status = hard_reset_ccc(disk_fd);
  }
  return (status);
}





int do_hba_reset_ccc (void)
{
  int ret = -1;
  if (ahci_mode_ccc && enable_hba_reset_on_connect_source_ccc)
  {
    ret = hba_reset_ccc();
  }
  else if (!ahci_mode_ccc)
  {
    fprintf (stdout, "not ahci mode, hba reset not performed\n");
    strcpy (tempmessage_ccc, "not ahci mode, hba reset not performed\n");
    message_error_ccc(tempmessage_ccc);
  }
  else if (!enable_hba_reset_on_connect_source_ccc)
  {
    fprintf (stdout, "hba reset not enabled, hba reset not performed\n");
    strcpy (tempmessage_ccc, "hba reset not enabled, hba reset not performed\n");
    message_error_ccc(tempmessage_ccc);
  }
  return ret;
}







int soft_reset_ccc(int disk_fd)
{
  if (performing_reset_ccc)
  {
    return 1;
  }
  did_hard_reset_ccc = 0;
  did_reset_ccc = 1;
  if (direct_mode_ccc)
  {
    if (superclone_ccc)
    {
      sprintf (tempmessage_ccc, "soft reset\n");
      message_console_log_ccc(tempmessage_ccc, 0);
    }
    if (ahci_mode_ccc)
    {
      if (superbyte_ccc[35] == 0x26)
      {
        // ahci
        #ifdef DEBUG
        int i;
        if (debug_ccc & DEBUG35)
        {
          //fprintf (stdout, "soft reset\n");  //debug
          for (i = 0; i < 0x80; i+=4)
          {
            break;                                           //debug
            memcpy(&io_doubleword_ccc, port_virt_addr_ccc + i, 4);
            fprintf (stdout, "%x %08x ", i, io_doubleword_ccc);  //debug
            int n;
            for (n = 0; n < 32; n++)
            {
              if ( !(n % 8) )
              {
                fprintf (stdout, " ");
              }
              if ((io_doubleword_ccc<<n) & 0x80000000)
                fprintf (stdout, "1");
              else
                fprintf (stdout, "0");
            }
            fprintf (stdout, "\n");
          }
        }
        #endif

        // clear the start bit
        uint8_t byte;
        memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[10], 1);    //potential superbyte
        byte = byte & 0xfe;
        memcpy(port_virt_addr_ccc + superbyte_ccc[10], &byte, 1);    //potential superbyte

        // set the buffer size to 0 for no data transfer
        unsigned long long backup_buffer_size = ccc_main_buffer_size_ccc;
        ccc_main_buffer_size_ccc = 0;
        set_main_buffer_ccc();

        int status = 0;

        if (hard_reset_time_ccc == 0)
        {
          // if forced then do hard reset without soft reset
          status = hard_reset_ccc(disk_fd);
          did_hard_reset_ccc = 1;
        }
        else
        {
          // try soft reset first
          // clear the command list
          memset(command_list_buffer_ccc, 0, command_list_size_ccc);
          // set the command FIS length, 5 dwords for h2d, also contains read/write bit
          io_singlebyte_ccc = 5;
          io_singlebyte_ccc = io_singlebyte_ccc | ((1) << 6);
          memcpy(command_list_buffer_ccc, &io_singlebyte_ccc, 1);
          // set the reset bit in the command fis
          memset(command_list_buffer_ccc+1, 1, 1);

          // clear the fis buffer
          memset(fis_buffer_ccc, 0, fis_size_ccc);
          // clear the command FIS
          memset(table_buffer_ccc, 0, 0x80);
          // set FIS type
          memset(table_buffer_ccc, REG_H2D, 1);
          // set type as control
          memset(table_buffer_ccc+1, 0x00, 1);
          // set the table entry length
          uint16_t word = table_entry_count_ccc;
          memcpy(command_list_buffer_ccc+2, &word, 2);
          // set the command table address
          uint64_t qword = table_physical_address_ccc;
          memcpy(command_list_buffer_ccc+8, &qword, 8);

          // set the soft reset control bit
          memset(table_buffer_ccc+15, 4, 1);

          // set the command list override bit
          byte = byte | 8;
          memcpy(port_virt_addr_ccc + superbyte_ccc[10], &byte, 1);    //potential superbyte
          // wait for clo bit to be clear
          int check_bit = 1;
          unsigned long long start_time = get_elapsed_usec_ccc();
          while (check_bit)
          {
            unsigned long long elapsed_time = get_elapsed_usec_ccc();
            if (elapsed_time > start_time + COMMAND_LIST_OVERRIDE_TIME)
            {
              // if it exceeds general timeout then quit
              break;
            }
            memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[10], 1);    //potential superbyte
            check_bit = byte & 0x08;
            // give the cpu a chance to do something else so we are not using 100%
            do_nanosleep_ccc(1);
          }

          // make sure fis is running
          enable_fis_ccc(FIS_BIT_TIME);

          // set the start bit
          enable_start_ccc(START_BIT_TIME);

          // set the command issue bit
          enable_command_issue_ccc(COMMAND_BIT_TIME);

          do_nanosleep_ccc(500);

          // clear the soft reset control bit
          memset(table_buffer_ccc+15, 0, 1);

          // clear the start bit
          disable_start_ccc(START_BIT_TIME);

          // set the command list override bit
          byte = byte | 8;
          memcpy(port_virt_addr_ccc + superbyte_ccc[10], &byte, 1);    //potential superbyte
          // wait for clo bit to be clear
          check_bit = 1;
          start_time = get_elapsed_usec_ccc();
          while (check_bit)
          {
            unsigned long long elapsed_time = get_elapsed_usec_ccc();
            if (elapsed_time > start_time + COMMAND_LIST_OVERRIDE_TIME)
            {
              // if it exceeds general timeout then quit
              break;
            }
            memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[10], 1);    //potential superbyte
            check_bit = byte & 0x08;
            // give the cpu a chance to do something else so we are not using 100%
            do_nanosleep_ccc(1);
          }

          // set the start bit
          enable_start_ccc(START_BIT_TIME);

          // set the command issue bit
          enable_command_issue_ccc(COMMAND_BIT_TIME);

          // wait for 1ms
          do_nanosleep_ccc(1000000);

          // clear the start bit
          disable_start_ccc(START_BIT_TIME);

          // wait for 1ms
          do_nanosleep_ccc(1000000);

          // reset the fis to make sure it updates
          disable_fis_ccc(FIS_BIT_TIME);

          // wait for 1ms
          do_nanosleep_ccc(1000000);

          // set addresses
          uint64_t command_list_address = command_list_physical_address_ccc;
          memcpy(port_virt_addr_ccc + superbyte_ccc[11], &command_list_address, 8);    //potential superbyte
          uint64_t fis_address = fis_physical_address_ccc;
          memcpy(port_virt_addr_ccc + superbyte_ccc[12], &fis_address, 8);    //potential superbyte
          // wait for 1ms
          do_nanosleep_ccc(1000000);

          // make sure fis is running
          enable_fis_ccc(FIS_BIT_TIME);

          // wait for 1ms
          do_nanosleep_ccc(1000000);

          // if always wait for reset time then wait
          if (always_wait_for_reset_timers_ccc)
          {
            unsigned long long rstart_time = get_elapsed_usec_ccc();
            unsigned long long relapsed_time = get_elapsed_usec_ccc();
            while ( (relapsed_time - rstart_time) < hard_reset_time_ccc)
            {
              do_nanosleep_ccc(1000000);
              relapsed_time = get_elapsed_usec_ccc();
              update_display_status_buttons_ccc(DISPLAY_STATUS_UPDATE_TIME);
            }
          }

          // wait for device to be ready
          status = wait_not_busy_or_drq_ccc(hard_reset_time_ccc, 0);
          if (stop_signal_ccc)
          {
            return STOP_SIGNAL_RETURN_CODE;
          }
          if (status == 1)
          {
            sprintf (tempmessage_ccc, "timeout after soft reset\n");
            message_console_log_ccc(tempmessage_ccc, 0);
            // if it timed out then do hard reset
            performing_reset_ccc = 1;
            status = hard_reset_ccc(disk_fd);
            performing_reset_ccc = 0;
            did_hard_reset_ccc = 1;
          }
        }

        // clear any interrupt bits
        ahci_interrupt_changed_ccc = true;
        memset(port_virt_addr_ccc + superbyte_ccc[7], 0xff, 4);    //potential superbyte

        // get recent copy of status and error
        memcpy(&io_byte_ccc[1], port_virt_addr_ccc + superbyte_ccc[8], 1);    // status    //potential superbyte
        memcpy(&io_byte_ccc[2], port_virt_addr_ccc + superbyte_ccc[9], 1);    // error    //potential superbyte

        post_direct_ccc(28);

        // put the buffer size back
        ccc_main_buffer_size_ccc = backup_buffer_size;
        set_main_buffer_ccc();

        if (1)
        {
          // perform steps to help with recovery after reset
          // turn interupts off
          memset(port_virt_addr_ccc + superbyte_ccc[13], 0x0, 4);    //potential superbyte
          // wait for 1ms
          do_nanosleep_ccc(1000000);
          // clear the start bit
          disable_start_ccc(START_BIT_TIME);
          // wait for 1ms
          do_nanosleep_ccc(1000000);
          // clear the fis enable bit
          disable_fis_ccc(FIS_BIT_TIME);
          // wait for 1ms
          do_nanosleep_ccc(1000000);
          // set new addresses
          uint64_t command_list_address = command_list_physical_address_ccc;
          memcpy(port_virt_addr_ccc + superbyte_ccc[11], &command_list_address, 8);    //potential superbyte
          uint64_t fis_address = fis_physical_address_ccc;
          memcpy(port_virt_addr_ccc + superbyte_ccc[12], &fis_address, 8);    //potential superbyte
          // wait for 1ms
          do_nanosleep_ccc(1000000);
          // set the fis enable bit
          enable_fis_ccc(FIS_BIT_TIME);
          // wait for 1ms
          do_nanosleep_ccc(1000000);
        }

        // if it fails to identify after then bad status
        performing_reset_ccc = 1;
        if (!did_hard_reset_ccc && identify_device_ccc())
        {
          sprintf (tempmessage_ccc, "identify failed after soft reset\n");
          message_console_log_ccc(tempmessage_ccc, 0);
          if (did_hard_reset_ccc)
          {
            status = status | 4;
          }
          else
          {
            status = hard_reset_ccc(disk_fd);
          }
        }
        performing_reset_ccc = 0;
        return (status);
      }
    }


    else
    {
      // ide
      //fprintf (stdout, "reset\n");
      int x;
      for (x = 1; x < 8; x++)
      {
        io_byte_ccc[x] = inb(reg_base_address_ccc+x);
        //fprintf (stdout, "%02x ", io_byte_ccc[x]);
      }
      //fprintf (stdout, "\n");
      outb(6, control_base_address_ccc);

      do_nanosleep_ccc(500);

      outb(2, control_base_address_ccc);
      for (x = 1; x < 8; x++)
      {
        io_byte_ccc[x] = inb(reg_base_address_ccc+x);
        //fprintf (stdout, "%02x ", io_byte_ccc[x]);
      }
      //fprintf (stdout, "\n");
      // set the device after the reset
      set_device_ccc(reg_base_address_ccc, device_select_base_ccc);

      // if always wait for reset time then wait
      if (always_wait_for_reset_timers_ccc)
      {
        unsigned long long rstart_time = get_elapsed_usec_ccc();
        unsigned long long relapsed_time = get_elapsed_usec_ccc();
        while ( (relapsed_time - rstart_time) < hard_reset_time_ccc)
        {
          do_nanosleep_ccc(1000000);
          relapsed_time = get_elapsed_usec_ccc();
          update_display_status_buttons_ccc(DISPLAY_STATUS_UPDATE_TIME);
        }
      }

      int status = wait_not_busy_or_drq_ccc(reset_wait_time_ccc, 0);
      if (stop_signal_ccc)
      {
        return STOP_SIGNAL_RETURN_CODE;
      }

      for (x = 1; x < 8; x++)
      {
        io_byte_ccc[x] = inb(reg_base_address_ccc+x);
        //fprintf (stdout, "%02x ", io_byte_ccc[x]);
      }
      //fprintf (stdout, "\n");

      if (superclone_ccc && status)
      {
        status = call_command_on_power_cycle_ccc();
        if (status)
        {
          return status;
        }
      }

      // if it fails to identify after then bad status
      performing_reset_ccc = 1;
      if (identify_device_ccc())
      {
        status = status | 4;
      }
      performing_reset_ccc = 0;
      return (status);
    }
  }

  else if (usb_mode_ccc)
  {
    int reset_time = reset_wait_time_ccc / 1000;
    usb_reset_recovery_ccc(reset_time);
    if (usb_soft_reset_enabled_ccc && usb_allow_ata_ccc)
    {
      usb_soft_hard_reset_ccc(USB_RESET_TYPE_SOFT, reset_time);
      usb_reset_recovery_ccc(reset_time);
    }
  }

  else
  {
    if (superclone_ccc)
    {
      int status = disk_reopen_ccc();
      if (status)
      {
        return status;
      }
    }
    else
    {
      // this normally does not work
      // it just gives sense code of invalid field in CDB
      int protocol = passthrough_ccc.protocol;
      passthrough_ccc.protocol = 1;    // soft reset
      do_ata28_cmd_ccc(disk_fd);
      passthrough_ccc.protocol = protocol;
    }
  }

  // if it fails to identify after then bad status
  int status = 0;
  performing_reset_ccc = 1;
  if (identify_device_ccc())
  {
    status = status | 4;
  }
  performing_reset_ccc = 0;
  return status;
}





int hard_reset_ccc(int disk_fd)
{
  did_power_cycle_ccc = 0;
  int status = 0;
  if (direct_mode_ccc)
  {
    if (reset_wait_time_ccc == 0)
    {
      // if forced then do power cylcle without hard reset
      status = call_command_on_power_cycle_ccc();
      if (status)
      {
        return status;
      }
    }
    else
    {
      if (superbyte_ccc[36] == 0x6f)
      {
        if (ahci_mode_ccc)
        {
          // ahci
          if (superclone_ccc)
          {
            sprintf (tempmessage_ccc, "hard reset\n");
            message_console_log_ccc(tempmessage_ccc, 0);
          }
          #ifdef DEBUG
          int i;
          if (debug_ccc & DEBUG35)
          {
            fprintf (stdout, "hard reset\n");
            for (i = 0; i < 0x80; i+=4)
            {
              memcpy(&io_doubleword_ccc, port_virt_addr_ccc + i, 4);
              fprintf (stdout, "%x %08x ", i, io_doubleword_ccc);  //debug
              int n;
              for (n = 0; n < 32; n++)
              {
                if ( !(n % 8) )
                {
                  fprintf (stdout, " ");
                }
                if ((io_doubleword_ccc<<n) & 0x80000000)
                  fprintf (stdout, "1");
                else
                  fprintf (stdout, "0");
              }
              fprintf (stdout, "\n");
            }
          }
          #endif

          // clear the start bit
          uint8_t byte;
          memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[10], 1);    //potential superbyte
          byte = byte & 0xfe;
          memcpy(port_virt_addr_ccc + superbyte_ccc[10], &byte, 1);    //potential superbyte

          // set the buffer size to 0 for no data transfer
          unsigned long long backup_buffer_size = ccc_main_buffer_size_ccc;
          ccc_main_buffer_size_ccc = 0;
          set_main_buffer_ccc();

          // clear the command list
          memset(command_list_buffer_ccc, 0, command_list_size_ccc);
          // clear the fis buffer
          memset(fis_buffer_ccc, 0, fis_size_ccc);
          // clear the command FIS
          memset(table_buffer_ccc, 0, 0x80);

          // set the comreset bit in the Device Detection Initialization field
          memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[16], 1);
          byte = byte & 0xf0;
          byte = byte | 0x01;
          memcpy(port_virt_addr_ccc + superbyte_ccc[16], &byte, 1);

          // wait for 1ms
          do_nanosleep_ccc(1000000);

          // clear the comreset bit
          byte = byte & 0xf0;
          memcpy(port_virt_addr_ccc + superbyte_ccc[16], &byte, 1);

          // wait for device to connect
          memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[17], 1);
          byte = byte & 0x0f;
          unsigned long long start_time = get_elapsed_usec_ccc();
          while (byte != 3)
          {
            unsigned long long elapsed_time = get_elapsed_usec_ccc();
            if (elapsed_time > start_time + reset_wait_time_ccc)
            {
              // if it exceeds the reset timeout then quit
              break;
            }
            memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[17], 1);
            byte = byte & 0x0f;
            // give the cpu a chance to do something else so we are not using 100%
            do_nanosleep_ccc(1);
          }

          // clear all error bits
          memset(port_virt_addr_ccc + superbyte_ccc[18], 0xff, 4);

          // clear the start bit
          memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[10], 1);
          byte = byte & 0xfe;
          memcpy(port_virt_addr_ccc + superbyte_ccc[10], &byte, 1);

          // wait for 1ms
          do_nanosleep_ccc(1000000);

          // reset the fis to make sure it updates
          disable_fis_ccc(FIS_BIT_TIME);

          // wait for 1ms
          do_nanosleep_ccc(1000000);

          // set addresses
          uint64_t command_list_address = command_list_physical_address_ccc;
          memcpy(port_virt_addr_ccc + superbyte_ccc[11], &command_list_address, 8);    //potential superbyte
          uint64_t fis_address = fis_physical_address_ccc;
          memcpy(port_virt_addr_ccc + superbyte_ccc[12], &fis_address, 8);    //potential superbyte

          // wait for 1ms
          do_nanosleep_ccc(1000000);

          // make sure fis is running
          enable_fis_ccc(FIS_BIT_TIME);

          // wait for 1ms
          do_nanosleep_ccc(1000000);

          // if always wait for reset time then wait
          if (always_wait_for_reset_timers_ccc)
          {
            unsigned long long rstart_time = get_elapsed_usec_ccc();
            unsigned long long relapsed_time = get_elapsed_usec_ccc();
            while ( (relapsed_time - rstart_time) < reset_wait_time_ccc)
            {
              do_nanosleep_ccc(1000000);
              relapsed_time = get_elapsed_usec_ccc();
              update_display_status_buttons_ccc(DISPLAY_STATUS_UPDATE_TIME);
            }
          }

          // wait for device to be ready
          int status = wait_not_busy_or_drq_ccc(reset_wait_time_ccc, 0);
          if (stop_signal_ccc)
          {
            return STOP_SIGNAL_RETURN_CODE;
          }

          if (superclone_ccc && status)
          {
            sprintf (tempmessage_ccc, "timeout after hard reset\n");
            message_console_log_ccc(tempmessage_ccc, 0);
            status = call_command_on_power_cycle_ccc();
            did_power_cycle_ccc = 1;
            if (status)
            {
              return status;
            }
          }

          // put the buffer size back
          ccc_main_buffer_size_ccc = backup_buffer_size;
          set_main_buffer_ccc();

          #ifdef DEBUG
          for (i = 0; i < 0x80; i+=4)
          {
            memcpy(&io_doubleword_ccc, port_virt_addr_ccc + i, 4);
            //fprintf (stdout, "%x %08x\n", i, io_doubleword_ccc);  //debug
          }
          #endif

          if (1)
          {
            // perform steps to help with recovery after reset
            // turn interupts off
            memset(port_virt_addr_ccc + superbyte_ccc[13], 0x0, 4);    //potential superbyte
            // wait for 1ms
            do_nanosleep_ccc(1000000);
            // clear the start bit
            disable_start_ccc(START_BIT_TIME);
            // wait for 1ms
            do_nanosleep_ccc(1000000);
            // clear the fis enable bit
            disable_fis_ccc(FIS_BIT_TIME);
            // wait for 1ms
            do_nanosleep_ccc(1000000);
            // set new addresses
            uint64_t command_list_address = command_list_physical_address_ccc;
            memcpy(port_virt_addr_ccc + superbyte_ccc[11], &command_list_address, 8);    //potential superbyte
            uint64_t fis_address = fis_physical_address_ccc;
            memcpy(port_virt_addr_ccc + superbyte_ccc[12], &fis_address, 8);    //potential superbyte
            // wait for 1ms
            do_nanosleep_ccc(1000000);
            // set the fis enable bit
            enable_fis_ccc(FIS_BIT_TIME);
            // wait for 1ms
            do_nanosleep_ccc(1000000);
          }

          // if it fails to identify after then bad status
          performing_reset_ccc = 1;
          if (identify_device_ccc())
          {
            sprintf (tempmessage_ccc, "identify failed after hard reset\n");
            message_console_log_ccc(tempmessage_ccc, 0);
            performing_reset_ccc = 0;
            if (did_power_cycle_ccc)
            {
              status = status | 4;
            }
            else
            {
              status = call_command_on_power_cycle_ccc();
              did_power_cycle_ccc = 1;
              if (status)
              {
                return status;
              }
            }
          }
          performing_reset_ccc = 0;
          return (status);
        }


        else
        {
          // ide
          // not available so return status of 1
          return (1);
        }
      }
    }
  }

  else if (usb_mode_ccc)
  {
    int reset_time = reset_wait_time_ccc / 1000;
    do_usb_reset_ccc();
    usb_reset_recovery_ccc(reset_time);
    if (usb_hard_reset_enabled_ccc && usb_allow_ata_ccc)
    {
      usb_soft_hard_reset_ccc(USB_RESET_TYPE_HARD, reset_time);
      usb_reset_recovery_ccc(reset_time);
    }
  }

  else
  {
    if (superclone_ccc)
    {
      int status = disk_reopen_ccc();
      if (status)
      {
        return status;
      }
    }
    else
    {
      // this normally does not work
      // it just gives sense code of invalid field in CDB
      int protocol = passthrough_ccc.protocol;
      passthrough_ccc.protocol = 0;    // hard reset
      do_ata28_cmd_ccc(disk_fd);
      passthrough_ccc.protocol = protocol;
    }
  }

  // if it fails to identify after then bad status
  performing_reset_ccc = 1;
  if (identify_device_ccc())
  {
    status = status | 4;
  }
  performing_reset_ccc = 0;
  return (status);
}





int hba_reset_ccc(void)
{
  int ret = 0;
  if (direct_mode_ccc)
  {
    if (ahci_mode_ccc)
    {
      if (superbyte_ccc[42] == 0x20)
      {
        if (hba_reset_address_ccc == 0)
        {
          fprintf (stderr, "hba address not valid, try choosing a source first\n");
          strcpy (tempmessage_ccc, "hba address not valid, try choosing a source first\n");
          message_error_ccc(tempmessage_ccc);
          return (-1);
        }
        FILE *hba_debug_reset_file = fopen("hba_reset_debug.log", "w");
        fprintf (hba_debug_reset_file, "%s %s hba debug reset file created at ", title_ccc, version_number_ccc);
        time_t mytime;
        mytime = time(NULL);
        fprintf (hba_debug_reset_file, "%s", ctime(&mytime));

        sprintf (tempmessage_ccc, "HBA Reset\n");
        message_console_log_ccc(tempmessage_ccc, 0);

        unsigned char hba_backup[0x30];
        unsigned char hba_after[0x30];
        unsigned char port_backup[32][0x80];
        unsigned char port_after[32][0x80];
        unsigned char temp_hba_data[0x30];
        unsigned char temp_port_data[0x80];

        uint32_t page_mask, page_size;

        hba_mem_dev_ccc = open("/dev/mem", O_RDWR | O_SYNC);
        if(hba_mem_dev_ccc == -1)
        {
          fprintf (stderr, "unable to open /dev/mem\n");
          return (-1);
        }
        const uint32_t hba_mem_address = hba_reset_address_ccc;
        const uint32_t hba_mem_size = 128;

        page_size = pagesize_ccc;
        hba_alloc_mem_size_ccc = (((hba_mem_size / page_size) + 1) * page_size);
        page_mask = (page_size - 1);

        hba_mem_pointer_ccc = mmap(NULL,
                                   hba_alloc_mem_size_ccc,
                                   PROT_READ | PROT_WRITE,
                                   MAP_SHARED,
                                   hba_mem_dev_ccc,
                                   (hba_mem_address & ~page_mask)
        );

        if(hba_mem_pointer_ccc == MAP_FAILED)
        {
          fprintf (stderr, "HBA mem map failed\n");
          return (-1);
        }
        hba_virt_addr_ccc = (hba_mem_pointer_ccc + (hba_mem_address & page_mask));
        memory_mapped_ccc = true;
        memcpy(hba_backup, hba_virt_addr_ccc, 0x30);
        uint32_t ports_implemented;
        memcpy(&ports_implemented, hba_virt_addr_ccc + 0x0c, 4);

        // backup ports
        int x;
        for (x = 0; x < 32; x++)
        {
          int port_used = (ports_implemented >> x) & 1;
          if (port_used)
          {
            port_mem_dev_ccc = open("/dev/mem", O_RDWR | O_SYNC);
            if(port_mem_dev_ccc == -1)
            {
              fprintf (stderr, "unable to open /dev/mem\n");
              return (-1);
            }
            const uint32_t port_mem_address = (hba_reset_address_ccc + 0x100) + (0x80 * x);
            const uint32_t port_mem_size = 128;

            page_size = pagesize_ccc;
            port_alloc_mem_size_ccc = (((port_mem_size / page_size) + 1) * page_size);
            page_mask = (page_size - 1);

            port_mem_pointer_ccc = mmap(NULL,
                                        port_alloc_mem_size_ccc,
                                        PROT_READ | PROT_WRITE,
                                        MAP_SHARED,
                                        port_mem_dev_ccc,
                                        (port_mem_address & ~page_mask)
            );

            if(port_mem_pointer_ccc == MAP_FAILED)
            {
              fprintf (stderr, "Port mem map failed\n");
              return (-1);
            }
            port_virt_addr_ccc = (port_mem_pointer_ccc + (port_mem_address & page_mask));

            memcpy(port_backup[x], port_virt_addr_ccc, 0x80);

            munmap(port_mem_pointer_ccc, port_alloc_mem_size_ccc);
            close(port_mem_dev_ccc);
          }
        }

        if (1)
        {
          memcpy(temp_hba_data, hba_backup, 0x30);
          fprintf (stdout, "hba before hba reset\n");
          fprintf (hba_debug_reset_file, "hba before hba reset\n");
          dump_hba_data_to_file_ccc(stdout, temp_hba_data, 0x30);
          dump_hba_data_to_file_ccc(hba_debug_reset_file, temp_hba_data, 0x30);
        }
        if (1)
        {
          int x;
          for (x = 0; x < 32; x++)
          {
            int port_used = (ports_implemented >> x) & 1;
            if (port_used)
            {
              fprintf (stdout, "port %d before hba reset\n", x);
              fprintf (hba_debug_reset_file, "port %d before hba reset\n", x);
              if (port_reset_address_ccc == (hba_reset_address_ccc + 0x100) + (0x80 * x))
              {
                fprintf (stdout, "this is chosen port\n");
                fprintf (hba_debug_reset_file, "this is chosen port\n");
              }
              memcpy(temp_port_data, port_backup[x], 0x80);
              dump_hba_data_to_file_ccc(stdout, temp_port_data, 0x80);
              dump_hba_data_to_file_ccc(hba_debug_reset_file, temp_port_data, 0x80);
            }
          }
        }


        if (1) // choose to do the actual reset or not
        {
          fprintf (stdout, "performing actual hba reset now\n");
          fprintf (hba_debug_reset_file, "performing actual hba reset now\n");
          // set the hba reset bit
          uint8_t byte;
          memcpy(&byte, hba_virt_addr_ccc + superbyte_ccc[68], 1);    // superbyte
          byte = byte | 0x01;
          memcpy(hba_virt_addr_ccc + superbyte_ccc[68], &byte, 1);    // superbyte

          // wait for hba to reset
          memcpy(&byte, hba_virt_addr_ccc + superbyte_ccc[68], 1);    // superbyte
          byte = byte & 0x01;
          unsigned long long start_time = get_elapsed_usec_ccc();
          while (byte != 0)
          {
            unsigned long long elapsed_time = get_elapsed_usec_ccc();
            if (elapsed_time > start_time + HBA_RESET_TIME)
            {
              // if it exceeds the hba timeout then quit
              fprintf (stderr, "HBA appears to be hung\n");
              ret = -1;
              break;
            }
            memcpy(&byte, hba_virt_addr_ccc + superbyte_ccc[68], 1);    // superbyte
            byte = byte & 0x01;
            // give the cpu a chance to do something else so we are not using 100%
            do_nanosleep_ccc(1);
          }

          // now put some things back the way they were before the computer knows what happened
          // first enable ahci
          memcpy(&byte, hba_virt_addr_ccc + superbyte_ccc[70], 1);    // superbyte
          byte = byte | 0x80;
          memcpy(hba_virt_addr_ccc + superbyte_ccc[70], &byte, 1);    // superbyte
          // enable hba interupts
          memcpy(&byte, hba_virt_addr_ccc + superbyte_ccc[68], 1);    // superbyte
          byte = byte | 0x02;
          memcpy(hba_virt_addr_ccc + superbyte_ccc[68], &byte, 1);    // superbyte

          // now cycle through the ports
          int x;
          for (x = 0; x < 32; x++)
          {
            int port_used = (ports_implemented >> x) & 1;
            if (port_used)
            {
              port_mem_dev_ccc = open("/dev/mem", O_RDWR | O_SYNC);
              if(port_mem_dev_ccc == -1)
              {
                fprintf (stderr, "unable to open /dev/mem\n");
                return (-1);
              }
              const uint32_t port_mem_address = (hba_reset_address_ccc + 0x100) + (0x80 * x);
              const uint32_t port_mem_size = 128;
              page_size = pagesize_ccc;
              port_alloc_mem_size_ccc = (((port_mem_size / page_size) + 1) * page_size);
              page_mask = (page_size - 1);
              port_mem_pointer_ccc = mmap(NULL,
                                          port_alloc_mem_size_ccc,
                                          PROT_READ | PROT_WRITE,
                                          MAP_SHARED,
                                          port_mem_dev_ccc,
                                          (port_mem_address & ~page_mask)
              );
              if(port_mem_pointer_ccc == MAP_FAILED)
              {
                fprintf (stderr, "Port mem map failed\n");
                return (-1);
              }
              port_virt_addr_ccc = (port_mem_pointer_ccc + (port_mem_address & page_mask));


              // clear the start bit in case it is not clear
              if (disable_start_ccc(START_BIT_TIME))
              {
                fprintf (stdout, "port %d command list did not stop in time\n", x);
                fprintf (hba_debug_reset_file, "port %d command list did not stop in time\n", x);
              }
              // clear port interrupt status
              uint32_t doubleword = 0;
              memcpy(port_virt_addr_ccc + superbyte_ccc[7], &doubleword, 4);    // superbyte
              // clear error status
              memcpy(port_virt_addr_ccc + superbyte_ccc[18], &doubleword, 4);    // superbyte
              // restore transitions and allowed speed (whole register is ok)
              memcpy(&doubleword, port_backup[x] + superbyte_ccc[16], 4);    // superbyte
              memcpy(port_virt_addr_ccc + superbyte_ccc[16], &doubleword, 4);    // superbyte
              // restore fis enable bit
              byte = port_backup[x][superbyte_ccc[10]] & 0x10;    // superbyte
              if (byte)
              {
                if (enable_fis_ccc(FIS_BIT_TIME))
                {
                  fprintf (stdout, "port %d fis did not start in time\n", x);
                  fprintf (hba_debug_reset_file, "port %d fis did not start in time\n", x);
                }
                else
                {
                  fprintf (stdout, "port %d fis running\n", x);
                  fprintf (hba_debug_reset_file, "port %d fis running\n", x);
                }
              }
              else
              {
                fprintf (stdout, "port %d fis was not enabled\n", x);
                fprintf (hba_debug_reset_file, "port %d fis was not enabled\n", x);
              }



              // if there was a device connected before reset, perform a com reset
              memcpy(&byte, port_backup[x] + superbyte_ccc[17], 1);
              byte = byte & 0x0f;
              if (byte == 3)
              {
                fprintf (stdout, "port %d com reset\n", x);
                fprintf (hba_debug_reset_file, "port %d com reset\n", x);
                // clear the start bit in case it is not clear
                memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[10], 1);    //potential superbyte
                byte = byte & 0xfe;
                memcpy(port_virt_addr_ccc + superbyte_ccc[10], &byte, 1);    //potential superbyte

                // set the comreset bit in the Device Detection Initialization field
                memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[16], 1);
                byte = byte | 0x01;
                memcpy(port_virt_addr_ccc + superbyte_ccc[16], &byte, 1);

                // wait for 1ms
                do_nanosleep_ccc(1000000);

                // clear the comreset bit
                byte = byte & 0xfe;
                memcpy(port_virt_addr_ccc + superbyte_ccc[16], &byte, 1);

                // wait for device to connect
                memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[17], 1);
                byte = byte & 0x0f;
                unsigned long long start_time = get_elapsed_usec_ccc();
                while (byte != 3)
                {
                  unsigned long long elapsed_time = get_elapsed_usec_ccc();
                  if (elapsed_time > start_time + reset_wait_time_ccc)
                  {
                    // if it exceeds the reset timeout then quit
                    fprintf (stdout, "port %d took too long to connect\n", x);
                    fprintf (hba_debug_reset_file, "port %d took too long to connect\n", x);
                    break;
                  }
                  memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[17], 1);
                  byte = byte & 0x0f;
                  // give the cpu a chance to do something else so we are not using 100%
                  do_nanosleep_ccc(1);
                }

                // clear all error bits
                memset(port_virt_addr_ccc + superbyte_ccc[18], 0xff, 4);

                // clear the start bit
                if (disable_start_ccc(START_BIT_TIME))
                {
                  fprintf (stdout, "port %d command list did not stop in time\n", x);
                  fprintf (hba_debug_reset_file, "port %d command list did not stop in time\n", x);
                }
              }


              // clear port interrupt status
              doubleword = 0;
              memcpy(port_virt_addr_ccc + superbyte_ccc[7], &doubleword, 4);    // superbyte
              // clear error status
              memcpy(port_virt_addr_ccc + superbyte_ccc[18], &doubleword, 4);    // superbyte
              // restore transitions and allowed speed (whole register is ok)
              memcpy(&doubleword, port_backup[x] + superbyte_ccc[16], 4);    // superbyte
              memcpy(port_virt_addr_ccc + superbyte_ccc[16], &doubleword, 4);    // superbyte
              // restore fis enable bit
              byte = port_backup[x][superbyte_ccc[10]] & 0x10;    // superbyte
              if (byte)
              {
                if (enable_fis_ccc(FIS_BIT_TIME))
                {
                  fprintf (stdout, "port %d fis did not start in time\n", x);
                  fprintf (hba_debug_reset_file, "port %d fis did not start in time\n", x);
                }
                else
                {
                  fprintf (stdout, "port %d fis running\n", x);
                  fprintf (hba_debug_reset_file, "port %d fis running\n", x);
                }
              }
              else
              {
                fprintf (stdout, "port %d fis was not enabled\n", x);
                fprintf (hba_debug_reset_file, "port %d fis was not enabled\n", x);
              }
              // restore start bit
              byte = port_backup[x][superbyte_ccc[10]] & 0x01;    // superbyte
              if (byte)
              {
                if (enable_start_ccc(START_BIT_TIME))
                {
                  fprintf (stdout, "port %d command list did not start in time\n", x);
                  fprintf (hba_debug_reset_file, "port %d command list did not start in time\n", x);
                }
                else
                {
                  fprintf (stdout, "port %d command list running\n", x);
                  fprintf (hba_debug_reset_file, "port %d command list running\n", x);
                }
              }
              else
              {
                fprintf (stdout, "port %d start bit was not enabled\n", x);
                fprintf (hba_debug_reset_file, "port %d start bit was not enabled\n", x);
              }
              // restore interrupt enable bits
              memcpy(&doubleword, port_backup[x] + superbyte_ccc[13], 4);    // superbyte
              memcpy(port_virt_addr_ccc + superbyte_ccc[13], &doubleword, 4);    // superbyte

              // if it is the chosen port, then wait for it to be not busy
              if (0 && port_reset_address_ccc == (hba_reset_address_ccc + 0x100) + (0x80 * x))
              {
                fprintf (stdout, "wating for chosen port not busy\n");
                fprintf (hba_debug_reset_file, "wating for chosen port not busy\n");
                // wait for drive to be ready
                int busy = 1;
                unsigned long long start_time = get_elapsed_usec_ccc();
                while (busy)
                {
                  unsigned long long elapsed_time = get_elapsed_usec_ccc();
                  if (elapsed_time > start_time + reset_wait_time_ccc)
                  {
                    // if it times out then fail
                    fprintf (stdout, "port stayed busy through reset wait time\n");
                    fprintf (hba_debug_reset_file, "port stayed busy through reset wait time\n");
                    break;
                  }
                  memcpy(&io_doubleword_ccc, port_virt_addr_ccc + superbyte_ccc[8], 4);
                  //fprintf (stdout, "%08x\n", io_doubleword_ccc);  //debug
                  busy = io_doubleword_ccc & 0x80;
                  // give the cpu a chance to do something else so we are not using 100%
                  //do_nanosleep_ccc(1);
                }
              }

              munmap(port_mem_pointer_ccc, port_alloc_mem_size_ccc);
              close(port_mem_dev_ccc);
            }
          }
          fprintf (stdout, "hba reset complete, hope everything still works and doesn't hang\n");
          fprintf (hba_debug_reset_file, "hba reset complete, hope everything still works and doesn't hang\n");
        }


        // wait for all ports to not be busy
        for (x = 0; x < 32; x++)
        {
          int port_used = (ports_implemented >> x) & 1;
          if (port_used)
          {
            port_mem_dev_ccc = open("/dev/mem", O_RDWR | O_SYNC);
            if(port_mem_dev_ccc == -1)
            {
              fprintf (stderr, "unable to open /dev/mem\n");
              return (-1);
            }
            const uint32_t port_mem_address = (hba_reset_address_ccc + 0x100) + (0x80 * x);
            const uint32_t port_mem_size = 128;
            page_size = pagesize_ccc;
            port_alloc_mem_size_ccc = (((port_mem_size / page_size) + 1) * page_size);
            page_mask = (page_size - 1);
            port_mem_pointer_ccc = mmap(NULL,
                                        port_alloc_mem_size_ccc,
                                        PROT_READ | PROT_WRITE,
                                        MAP_SHARED,
                                        port_mem_dev_ccc,
                                        (port_mem_address & ~page_mask)
            );
            if(port_mem_pointer_ccc == MAP_FAILED)
            {
              fprintf (stderr, "Port mem map failed\n");
              return (-1);
            }
            port_virt_addr_ccc = (port_mem_pointer_ccc + (port_mem_address & page_mask));
            fprintf (stdout, "wating for port %d not busy\n", x);
            fprintf (hba_debug_reset_file, "wating for port %d not busy\n", x);
            // wait for drive to be ready
            int busy = 1;
            unsigned long long start_time = get_elapsed_usec_ccc();
            while (busy)
            {
              unsigned long long elapsed_time = get_elapsed_usec_ccc();
              if (elapsed_time > start_time + reset_wait_time_ccc)
              {
                // if it times out then fail
                fprintf (stdout, "port %d stayed busy through reset wait time\n", x);
                fprintf (hba_debug_reset_file, "port %d stayed busy through reset wait time\n", x);
                break;
              }
              memcpy(&io_doubleword_ccc, port_virt_addr_ccc + superbyte_ccc[8], 4);
              //fprintf (stdout, "%08x\n", io_doubleword_ccc);  //debug
              busy = io_doubleword_ccc & 0x80;
              // give the cpu a chance to do something else so we are not using 100%
              //do_nanosleep_ccc(1);
            }
            munmap(port_mem_pointer_ccc, port_alloc_mem_size_ccc);
            close(port_mem_dev_ccc);
          }
        }






        // backup hba and ports after reset for display
        memcpy(hba_after, hba_virt_addr_ccc, 0x30);
        for (x = 0; x < 32; x++)
        {
          int port_used = (ports_implemented >> x) & 1;
          if (port_used)
          {
            port_mem_dev_ccc = open("/dev/mem", O_RDWR | O_SYNC);
            if(port_mem_dev_ccc == -1)
            {
              fprintf (stderr, "unable to open /dev/mem\n");
              return (-1);
            }
            const uint32_t port_mem_address = (hba_reset_address_ccc + 0x100) + (0x80 * x);
            const uint32_t port_mem_size = 128;

            page_size = pagesize_ccc;
            port_alloc_mem_size_ccc = (((port_mem_size / page_size) + 1) * page_size);
            page_mask = (page_size - 1);

            port_mem_pointer_ccc = mmap(NULL,
                                        port_alloc_mem_size_ccc,
                                        PROT_READ | PROT_WRITE,
                                        MAP_SHARED,
                                        port_mem_dev_ccc,
                                        (port_mem_address & ~page_mask)
            );

            if(port_mem_pointer_ccc == MAP_FAILED)
            {
              fprintf (stderr, "Port mem map failed\n");
              return (-1);
            }
            port_virt_addr_ccc = (port_mem_pointer_ccc + (port_mem_address & page_mask));

            memcpy(port_after[x], port_virt_addr_ccc, 0x80);

            munmap(port_mem_pointer_ccc, port_alloc_mem_size_ccc);
            close(port_mem_dev_ccc);
          }
        }

        if (1)
        {
          memcpy(temp_hba_data, hba_after, 0x30);
          fprintf (stdout, "hba after hba reset\n");
          fprintf (hba_debug_reset_file, "hba after hba reset\n");
          dump_hba_data_to_file_ccc(stdout, temp_hba_data, 0x30);
          dump_hba_data_to_file_ccc(hba_debug_reset_file, temp_hba_data, 0x30);
        }
        if (1)
        {
          int x;
          for (x = 0; x < 32; x++)
          {
            int port_used = (ports_implemented >> x) & 1;
            if (port_used)
            {
              fprintf (stdout, "port %d after hba reset\n", x);
              fprintf (hba_debug_reset_file, "port %d after hba reset\n", x);
              if (port_reset_address_ccc == (hba_reset_address_ccc + 0x100) + (0x80 * x))
              {
                fprintf (stdout, "this is chosen port\n");
                fprintf (hba_debug_reset_file, "this is chosen port\n");
              }
              memcpy(temp_port_data, port_after[x], 0x80);
              dump_hba_data_to_file_ccc(stdout, temp_port_data, 0x80);
              dump_hba_data_to_file_ccc(hba_debug_reset_file, temp_port_data, 0x80);
            }
          }
        }


        // close mapped memory
        munmap(hba_mem_pointer_ccc, hba_alloc_mem_size_ccc);
        close(hba_mem_dev_ccc);
        memory_mapped_ccc = false;

        fclose (hba_debug_reset_file);
      }
    }
  }

  return ret;
}





void dump_hba_data_to_file_ccc(FILE *file, unsigned char *data, int size)
{
  if (size % 4)
  {
    fprintf (file, "warning, size of data not evenly dividable by 4\n");
  }
  int i;
  uint32_t doubleword;
  for (i = 0; i < size; i+=4)
  {
    memcpy(&doubleword, data + i, 4);
    fprintf (file, "%x %08x ", i, doubleword);
    int n;
    for (n = 0; n < 32; n++)
    {
      if ( !(n % 8) )
      {
        fprintf (file, " ");
      }
      if ((doubleword<<n) & 0x80000000)
        fprintf (file, "1");
      else
        fprintf (file, "0");
    }
    fprintf (file, "\n");
  }
  fflush(file);
  int ptf = fileno(file);
  fsync(ptf);
}





void dump_data_to_file_ccc(FILE *file, unsigned char *data, int size)
{
  if (size % 16)
  {
    fprintf (file, "warning, size of data not evenly dividable by 16\n");
  }
  int i;
  for (i = 0; i < size; i+=16)
  {
    fprintf (file, "%x: ", i);
    unsigned char *c;
    int n;
    for (n=0; n < 16 && i+n < size; n++)
    {
      c = (unsigned char *)data+i+n;
      fprintf (file, "%02x ", *c);
    }
    fprintf (file, "   ");
    for (n=0; n < 16 && i+n < size; n++)
    {
      c = (unsigned char *)data+i+n;
      fprintf (file, "%c", isprint(*c) ? *c : '.');
    }
    fprintf (file, "\n");
  }
  fflush(file);
  int ptf = fileno(file);
  fsync(ptf);
}





int dump_hba_port_fis_command_data_ccc(unsigned long long hba_address, unsigned long long port_address, int hba, int port, int fis, int cdata, int ctable)
{
  if (!driver_installed_ccc)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGMISSINGDRIVER]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return -1;
  }

  char *dump_filename = data_dump_filename_ccc;
  unsigned char temp_hba_data[0x30];
  unsigned char temp_port_data[0x80];
  unsigned char temp_data[65536];
  char temp_string[1024];

  uint32_t page_mask, page_size;
  int hba_mem_dev;
  uint32_t hba_alloc_mem_size;
  void *hba_mem_pointer;
  void *hba_virt_addr;
  void *port_virt_addr;
  int port_mem_dev;
  uint32_t port_alloc_mem_size;
  void *port_mem_pointer;

  hba_mem_dev = open("/dev/mem", O_RDWR | O_SYNC);
  if(hba_mem_dev == -1)
  {
    fprintf (stderr, "unable to open /dev/mem\n");
    return (-1);
  }
  const uint32_t hba_mem_address = hba_address;
  const uint32_t hba_mem_size = 128;
  page_size = pagesize_ccc;
  hba_alloc_mem_size = (((hba_mem_size / page_size) + 1) * page_size);
  page_mask = (page_size - 1);

  hba_mem_pointer = mmap(NULL,
                             hba_alloc_mem_size,
                             PROT_READ | PROT_WRITE,
                             MAP_SHARED,
                             hba_mem_dev,
                             (hba_mem_address & ~page_mask)
  );
  if(hba_mem_pointer == MAP_FAILED)
  {
    fprintf (stderr, "HBA mem map failed\n");
    return (-1);
  }
  hba_virt_addr = (hba_mem_pointer + (hba_mem_address & page_mask));

  port_mem_dev = open("/dev/mem", O_RDWR | O_SYNC);
  if(port_mem_dev == -1)
  {
    fprintf (stderr, "unable to open /dev/mem\n");
    return (-1);
  }
  const uint32_t port_mem_address = port_address;
  const uint32_t port_mem_size = 128;
  page_size = pagesize_ccc;
  port_alloc_mem_size = (((port_mem_size / page_size) + 1) * page_size);
  page_mask = (page_size - 1);
  port_mem_pointer = mmap(NULL,
                              port_alloc_mem_size,
                              PROT_READ | PROT_WRITE,
                              MAP_SHARED,
                              port_mem_dev,
                              (port_mem_address & ~page_mask)
  );
  if(port_mem_pointer == MAP_FAILED)
  {
    fprintf (stderr, "Port mem map failed\n");
    return (-1);
  }
  port_virt_addr = (port_mem_pointer + (port_mem_address & page_mask));

  if (hba)
  {
    fprintf (stdout, "hba data, address=%08llx\n", hba_address);
    sprintf (temp_string, "hba data, address=%08llx\n", hba_address);
    dump_info_to_filename_ccc (dump_filename, temp_string);
    memcpy(temp_hba_data, hba_virt_addr, 0x30);
    dump_hba_data_to_file_ccc(stdout, temp_hba_data, 0x30);
    dump_data_to_filename_binary_ccc(dump_filename, temp_hba_data, 0x30, "hba data");
  }
  if (port)
  {
    fprintf (stdout, "port data, address=%08llx\n", port_address);
    sprintf (temp_string, "port data, address=%08llx\n", port_address);
    dump_info_to_filename_ccc (dump_filename, temp_string);
    memcpy(temp_port_data, port_virt_addr, 0x80);
    dump_hba_data_to_file_ccc(stdout, temp_port_data, 0x80);
    dump_data_to_filename_binary_ccc(dump_filename, temp_port_data, 0x80, "port data");
  }

  if (fis)
  {
    // fis data
    int data_size = 0x100;
    uint64_t address;
    memcpy(&address, port_virt_addr + superbyte_ccc[12], 8);    //potential superbyte
    fprintf (stdout, "fis data, address=%08llx\n", (unsigned long long)address);
    sprintf (temp_string, "fis data, address=%08llx\n", (unsigned long long)address);
    dump_info_to_filename_ccc (dump_filename, temp_string);

    //driver_buffer_ccc = malloc(DRIVER_TRANSFER_BUFFER_SIZE);
    memset(driver_control_data_ccc.buffer, 0, sizeof(driver_control_data_ccc.buffer));
    //driver_control_data_ccc.buffer = driver_buffer_ccc;

    sprintf (driver_device_name_ccc, "/proc/%s%d", MAIN_DRIVER_IOCTL_NAME, process_id_ccc);
    if (main_driver_fd_ccc > 0)
    {
      close(main_driver_fd_ccc);
    }
    main_driver_fd_ccc = open(driver_device_name_ccc, O_RDWR);
    if (main_driver_fd_ccc == -1)
    {
      check_message_ccc = true;
      strcpy (tempmessage_ccc, curlang_ccc[LANGUERROROPENINGDRIVER]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " %s (%s)\n", driver_device_name_ccc, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return -1;
    }

    driver_control_data_ccc.command = READ_MEMORY_DRIVE_COMMAND;
    unsigned long request = IOCTL_CMD_HDDSC;
    //strcpy(driver_control_data_ccc.buffer, virtual_driver_name_ccc);
    driver_control_data_ccc.read_timeout = address;
    driver_control_data_ccc.request_timeout = data_size;
    int ret = ioctl(main_driver_fd_ccc, request, &driver_control_data_ccc);
    if (ret < 0)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGERRORSTARTINGDRIVER]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " %s (%s)\n", driver_device_name_ccc, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      return -1;
    }

    memcpy (temp_data, driver_control_data_ccc.buffer, data_size);
    dump_data_to_file_ccc(stdout, temp_data, data_size);
    dump_data_to_filename_ccc(dump_filename, temp_data, data_size, "FIS data");

    close(main_driver_fd_ccc);
    //free(driver_buffer_ccc);
  }

  unsigned char command_data[0x400];
  if(cdata)
  {
    // command data
    int data_size = 0x400;
    uint64_t address;
    memcpy(&address, port_virt_addr + superbyte_ccc[11], 8);    //potential superbyte
    fprintf (stdout, "command list data, address=%08llx\n", (unsigned long long)address);
    sprintf (temp_string, "command list data, address=%08llx\n", (unsigned long long)address);
    dump_info_to_filename_ccc (dump_filename, temp_string);

    //driver_buffer_ccc = malloc(DRIVER_TRANSFER_BUFFER_SIZE);
    memset(driver_control_data_ccc.buffer, 0, sizeof(driver_control_data_ccc.buffer));
    //driver_control_data_ccc.buffer = driver_buffer_ccc;

    sprintf (driver_device_name_ccc, "/proc/%s%d", MAIN_DRIVER_IOCTL_NAME, process_id_ccc);
    if (main_driver_fd_ccc > 0)
    {
      close(main_driver_fd_ccc);
    }
    main_driver_fd_ccc = open(driver_device_name_ccc, O_RDWR);
    if (main_driver_fd_ccc == -1)
    {
      check_message_ccc = true;
      strcpy (tempmessage_ccc, curlang_ccc[LANGUERROROPENINGDRIVER]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " %s (%s)\n", driver_device_name_ccc, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return -1;
    }

    driver_control_data_ccc.command = READ_MEMORY_DRIVE_COMMAND;
    unsigned long request = IOCTL_CMD_HDDSC;
    //strcpy(driver_control_data_ccc.buffer, virtual_driver_name_ccc);
    driver_control_data_ccc.read_timeout = address;
    driver_control_data_ccc.request_timeout = data_size;
    int ret = ioctl(main_driver_fd_ccc, request, &driver_control_data_ccc);
    if (ret < 0)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGERRORSTARTINGDRIVER]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " %s (%s)\n", driver_device_name_ccc, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      return -1;
    }

    memcpy (temp_data, driver_control_data_ccc.buffer, data_size);
    dump_data_to_file_ccc(stdout, temp_data, data_size);
    dump_data_to_filename_ccc(dump_filename, temp_data, data_size, "Command data");

    memcpy (command_data, temp_data, data_size);

    close(main_driver_fd_ccc);
    //free(driver_buffer_ccc);
  }

  if(ctable)
  {
    // command table data
    int i;
    for (i = 0; i < 32; i++)
    {
      uint16_t table_length;
      memcpy (&table_length, command_data + (0x20 * i) + 2, 2);
      uint64_t table_address;
      memcpy (&table_address, command_data + ((0x20 * i) + 8), 8);
      if (table_length == 0 || table_address == 0)
      {
        fprintf (stdout, "command table data %d is zero\n", i);
        sprintf (temp_string, "command table data %d is zero\n", i);
        dump_info_to_filename_ccc (dump_filename, temp_string);
      }
      else
      {
        int data_size = 0x80 + (table_length * 0x10);
        if (data_size > 256)
        {
          fprintf (stdout, "command table data size is too big %d\n", data_size);
          sprintf (temp_string, "command table data size is too big %d\n", data_size);
          dump_info_to_filename_ccc (dump_filename, temp_string);
          data_size = 256;
        }
        uint64_t address;
        address = table_address;
        fprintf (stdout, "command table data %d, address=%08llx\n", i, (unsigned long long)address);
        sprintf (temp_string, "command table data %d, address=%08llx\n", i, (unsigned long long)address);
        dump_info_to_filename_ccc (dump_filename, temp_string);

        //driver_buffer_ccc = malloc(DRIVER_TRANSFER_BUFFER_SIZE);
        memset(driver_control_data_ccc.buffer, 0, sizeof(driver_control_data_ccc.buffer));
        //driver_control_data_ccc.buffer = driver_buffer_ccc;

        sprintf (driver_device_name_ccc, "/proc/%s%d", MAIN_DRIVER_IOCTL_NAME, process_id_ccc);
        if (main_driver_fd_ccc > 0)
        {
          close(main_driver_fd_ccc);
        }
        main_driver_fd_ccc = open(driver_device_name_ccc, O_RDWR);
        if (main_driver_fd_ccc == -1)
        {
          check_message_ccc = true;
          strcpy (tempmessage_ccc, curlang_ccc[LANGUERROROPENINGDRIVER]);
          message_error_ccc(tempmessage_ccc);
          sprintf (tempmessage_ccc, " %s (%s)\n", driver_device_name_ccc, strerror(errno));
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
          clear_error_message_ccc();
          return -1;
        }

        driver_control_data_ccc.command = READ_MEMORY_DRIVE_COMMAND;
        unsigned long request = IOCTL_CMD_HDDSC;
        driver_control_data_ccc.read_timeout = address;
        driver_control_data_ccc.request_timeout = data_size;
        int ret = ioctl(main_driver_fd_ccc, request, &driver_control_data_ccc);
        if (ret < 0)
        {
          strcpy (tempmessage_ccc, curlang_ccc[LANGERRORSTARTINGDRIVER]);
          message_error_ccc(tempmessage_ccc);
          sprintf (tempmessage_ccc, " %s (%s)\n", driver_device_name_ccc, strerror(errno));
          message_error_ccc(tempmessage_ccc);
          return -1;
        }

        memcpy (temp_data, driver_control_data_ccc.buffer, data_size);
        dump_data_to_file_ccc(stdout, temp_data, data_size);
        dump_data_to_filename_ccc(dump_filename, temp_data, data_size, "Command data table");

        close(main_driver_fd_ccc);
        //free(driver_buffer_ccc);
      }
    }
  }
  // close mapped memory
  munmap(hba_mem_pointer, hba_alloc_mem_size);
  close(hba_mem_dev);
  munmap(port_mem_pointer, port_alloc_mem_size);
  close(port_mem_dev);
  return 0;
}





int hba_test_ccc(void)
{
  FILE *hba_debug_file = fopen("hba_debug.log", "w");
  fprintf (hba_debug_file, "%s %s hba debug file created at ", title_ccc, version_number_ccc);
  time_t mytime;
  mytime = time(NULL);
  fprintf (hba_debug_file, "%s", ctime(&mytime));
  fprintf (stdout, "driver control data size %d\n", (int)sizeof(driver_control_data_ccc));
  fprintf (stderr, "driver control data size %d\n", (int)sizeof(driver_control_data_ccc));
  fprintf (hba_debug_file, "driver control data size %d\n", (int)sizeof(driver_control_data_ccc));
  fflush(hba_debug_file);
  int ptf = fileno(hba_debug_file);
  fsync(ptf);
  // ahci
  uint32_t page_mask, page_size;

  hba_mem_dev_ccc = open("/dev/mem", O_RDWR | O_SYNC);
  if(hba_mem_dev_ccc == -1)
  {
    fprintf (stderr, "unable to open /dev/mem\n");
    return (-1);
  }
  const uint32_t hba_mem_address = hba_base_address_ccc;
  const uint32_t hba_mem_size = 128;

  page_size = pagesize_ccc;
  hba_alloc_mem_size_ccc = (((hba_mem_size / page_size) + 1) * page_size);
  page_mask = (page_size - 1);

  hba_mem_pointer_ccc = mmap(NULL,
                             hba_alloc_mem_size_ccc,
                             PROT_READ | PROT_WRITE,
                             MAP_SHARED,
                             hba_mem_dev_ccc,
                             (hba_mem_address & ~page_mask)
  );

  if(hba_mem_pointer_ccc == MAP_FAILED)
  {
    fprintf (stderr, "HBA mem map failed\n");
    return (-1);
  }
  hba_virt_addr_ccc = (hba_mem_pointer_ccc + (hba_mem_address & page_mask));
  memory_mapped_ccc = true;


  port_mem_dev_ccc = open("/dev/mem", O_RDWR | O_SYNC);
  if(port_mem_dev_ccc == -1)
  {
    fprintf (stderr, "unable to open /dev/mem\n");
    return (-1);
  }
  const uint32_t port_mem_address = port_base_address_ccc;
  const uint32_t port_mem_size = 128;

  page_size = pagesize_ccc;
  port_alloc_mem_size_ccc = (((port_mem_size / page_size) + 1) * page_size);
  page_mask = (page_size - 1);

  port_mem_pointer_ccc = mmap(NULL,
                              port_alloc_mem_size_ccc,
                              PROT_READ | PROT_WRITE,
                              MAP_SHARED,
                              port_mem_dev_ccc,
                              (port_mem_address & ~page_mask)
  );

  if(port_mem_pointer_ccc == MAP_FAILED)
  {
    fprintf (stderr, "Port mem map failed\n");
    return (-1);
  }
  port_virt_addr_ccc = (port_mem_pointer_ccc + (port_mem_address & page_mask));
  memory_mapped_ccc = true;

  unsigned char temp_hba_data[0x30];
  unsigned char temp_port_data[0x80];
  unsigned char temp_data[65536];
  uint8_t byte;

  if (1)
  {
    fprintf (stdout, "hba data at start, address=%08llx\n", hba_base_address_ccc);
    fprintf (hba_debug_file, "hba data at start, address=%08llx\n", hba_base_address_ccc);
    fflush(hba_debug_file);
    ptf = fileno(hba_debug_file);
    fsync(ptf);
    memcpy(temp_hba_data, hba_virt_addr_ccc, 0x30);
    dump_hba_data_to_file_ccc(stdout, temp_hba_data, 0x30);
    dump_hba_data_to_file_ccc(hba_debug_file, temp_hba_data, 0x30);
  }
  if (1)
  {
    fprintf (stdout, "port data at start, address=%08llx\n", port_base_address_ccc);
    fprintf (hba_debug_file, "port data at start, address=%08llx\n", port_base_address_ccc);
    fflush(hba_debug_file);
    ptf = fileno(hba_debug_file);
    fsync(ptf);
    memcpy(temp_port_data, port_virt_addr_ccc, 0x80);
    dump_hba_data_to_file_ccc(stdout, temp_port_data, 0x80);
    dump_hba_data_to_file_ccc(hba_debug_file, temp_port_data, 0x80);
  }





  if(1)
  {
    // fis data
    int data_size = 0x100;
    uint64_t address;
    memcpy(&address, port_virt_addr_ccc + superbyte_ccc[12], 8);    //potential superbyte
    fprintf (stdout, "fis data at start, address=%08llx\n", (unsigned long long)address);
    fprintf (hba_debug_file, "fis data at start, address=%08llx\n", (unsigned long long)address);
    fflush(hba_debug_file);
    ptf = fileno(hba_debug_file);
    fsync(ptf);

    if (!driver_installed_ccc)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGMISSINGDRIVER]);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return -1;
    }

    //driver_buffer_ccc = malloc(DRIVER_TRANSFER_BUFFER_SIZE);
    memset(driver_control_data_ccc.buffer, 0, sizeof(driver_control_data_ccc.buffer));
    //driver_control_data_ccc.buffer = driver_buffer_ccc;

    sprintf (driver_device_name_ccc, "/proc/%s%d", MAIN_DRIVER_IOCTL_NAME, process_id_ccc);
    if (main_driver_fd_ccc > 0)
    {
      close(main_driver_fd_ccc);
    }
    main_driver_fd_ccc = open(driver_device_name_ccc, O_RDWR);
    if (main_driver_fd_ccc == -1)
    {
      check_message_ccc = true;
      strcpy (tempmessage_ccc, curlang_ccc[LANGUERROROPENINGDRIVER]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " %s (%s)\n", driver_device_name_ccc, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return -1;
    }

    driver_control_data_ccc.command = READ_MEMORY_DRIVE_COMMAND;
    unsigned long request = IOCTL_CMD_HDDSC;
    //strcpy(driver_control_data_ccc.buffer, virtual_driver_name_ccc);
    driver_control_data_ccc.read_timeout = address;
    driver_control_data_ccc.request_timeout = data_size;
    int ret = ioctl(main_driver_fd_ccc, request, &driver_control_data_ccc);
    if (ret < 0)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGERRORSTARTINGDRIVER]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " %s (%s)\n", driver_device_name_ccc, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      return -1;
    }

    memcpy (temp_data, driver_control_data_ccc.buffer, data_size);
    dump_data_to_file_ccc(stdout, temp_data, data_size);
    dump_data_to_file_ccc(hba_debug_file, temp_data, data_size);

    close(main_driver_fd_ccc);
    //free(driver_buffer_ccc);
  }



  unsigned char command_data[0x400];
  if(1)
  {
    // command data
    int data_size = 0x400;
    uint64_t address;
    memcpy(&address, port_virt_addr_ccc + superbyte_ccc[11], 8);    //potential superbyte
    fprintf (stdout, "command list data at start, address=%08llx\n", (unsigned long long)address);
    fprintf (hba_debug_file, "command list data at start, address=%08llx\n", (unsigned long long)address);
    fflush(hba_debug_file);
    ptf = fileno(hba_debug_file);
    fsync(ptf);

    if (!driver_installed_ccc)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGMISSINGDRIVER]);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return -1;
    }

    //driver_buffer_ccc = malloc(DRIVER_TRANSFER_BUFFER_SIZE);
    memset(driver_control_data_ccc.buffer, 0, sizeof(driver_control_data_ccc.buffer));
    //driver_control_data_ccc.buffer = driver_buffer_ccc;

    sprintf (driver_device_name_ccc, "/proc/%s%d", MAIN_DRIVER_IOCTL_NAME, process_id_ccc);
    if (main_driver_fd_ccc > 0)
    {
      close(main_driver_fd_ccc);
    }
    main_driver_fd_ccc = open(driver_device_name_ccc, O_RDWR);
    if (main_driver_fd_ccc == -1)
    {
      check_message_ccc = true;
      strcpy (tempmessage_ccc, curlang_ccc[LANGUERROROPENINGDRIVER]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " %s (%s)\n", driver_device_name_ccc, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return -1;
    }

    driver_control_data_ccc.command = READ_MEMORY_DRIVE_COMMAND;
    unsigned long request = IOCTL_CMD_HDDSC;
    //strcpy(driver_control_data_ccc.buffer, virtual_driver_name_ccc);
    driver_control_data_ccc.read_timeout = address;
    driver_control_data_ccc.request_timeout = data_size;
    int ret = ioctl(main_driver_fd_ccc, request, &driver_control_data_ccc);
    if (ret < 0)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGERRORSTARTINGDRIVER]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " %s (%s)\n", driver_device_name_ccc, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      return -1;
    }

    memcpy (temp_data, driver_control_data_ccc.buffer, data_size);
    dump_data_to_file_ccc(stdout, temp_data, data_size);
    dump_data_to_file_ccc(hba_debug_file, temp_data, data_size);

    memcpy (command_data, temp_data, data_size);

    close(main_driver_fd_ccc);
    //free(driver_buffer_ccc);
  }



  if(1)
  {
    // command table data
    int i;
    for (i = 0; i < 32; i++)
    {
      uint16_t table_length;
      memcpy (&table_length, command_data + (0x20 * i) + 2, 2);
      uint64_t table_address;
      memcpy (&table_address, command_data + ((0x20 * i) + 8), 8);
      if (table_length == 0 || table_address == 0)
      {
        fprintf (stdout, "command table data at start %d is zero\n", i);
        fprintf (hba_debug_file, "command table data at start %d is zero\n", i);
        fflush(hba_debug_file);
        ptf = fileno(hba_debug_file);
        fsync(ptf);
      }
      else
      {
        int data_size = 0x80 + (table_length * 0x10);
        if (data_size > 256)
        {
          fprintf (stdout, "command table data size is too big %d\n", data_size);
          fprintf (hba_debug_file, "command table data size is too big %d\n", data_size);
          fflush(hba_debug_file);
          ptf = fileno(hba_debug_file);
          fsync(ptf);
          data_size = 256;
        }
        uint64_t address;
        address = table_address;
        fprintf (stdout, "command table data at start %d, address=%08llx\n", i, (unsigned long long)address);
        fprintf (hba_debug_file, "command table data at start %d, address=%08llx\n", i, (unsigned long long)address);
        fflush(hba_debug_file);
        ptf = fileno(hba_debug_file);
        fsync(ptf);

        if (!driver_installed_ccc)
        {
          strcpy (tempmessage_ccc, curlang_ccc[LANGMISSINGDRIVER]);
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
          clear_error_message_ccc();
          return -1;
        }

        //driver_buffer_ccc = malloc(DRIVER_TRANSFER_BUFFER_SIZE);
        memset(driver_control_data_ccc.buffer, 0, sizeof(driver_control_data_ccc.buffer));
        //driver_control_data_ccc.buffer = driver_buffer_ccc;

        sprintf (driver_device_name_ccc, "/proc/%s%d", MAIN_DRIVER_IOCTL_NAME, process_id_ccc);
        if (main_driver_fd_ccc > 0)
        {
          close(main_driver_fd_ccc);
        }
        main_driver_fd_ccc = open(driver_device_name_ccc, O_RDWR);
        if (main_driver_fd_ccc == -1)
        {
          check_message_ccc = true;
          strcpy (tempmessage_ccc, curlang_ccc[LANGUERROROPENINGDRIVER]);
          message_error_ccc(tempmessage_ccc);
          sprintf (tempmessage_ccc, " %s (%s)\n", driver_device_name_ccc, strerror(errno));
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
          clear_error_message_ccc();
          return -1;
        }

        driver_control_data_ccc.command = READ_MEMORY_DRIVE_COMMAND;
        unsigned long request = IOCTL_CMD_HDDSC;
        //strcpy(driver_control_data_ccc.buffer, virtual_driver_name_ccc);
        driver_control_data_ccc.read_timeout = address;
        driver_control_data_ccc.request_timeout = data_size;
        int ret = ioctl(main_driver_fd_ccc, request, &driver_control_data_ccc);
        if (ret < 0)
        {
          strcpy (tempmessage_ccc, curlang_ccc[LANGERRORSTARTINGDRIVER]);
          message_error_ccc(tempmessage_ccc);
          sprintf (tempmessage_ccc, " %s (%s)\n", driver_device_name_ccc, strerror(errno));
          message_error_ccc(tempmessage_ccc);
          return -1;
        }

        memcpy (temp_data, driver_control_data_ccc.buffer, data_size);
        dump_data_to_file_ccc(stdout, temp_data, data_size);
        dump_data_to_file_ccc(hba_debug_file, temp_data, data_size);

        close(main_driver_fd_ccc);
        //free(driver_buffer_ccc);
      }
    }
  }






  // backup current addresses
  memcpy(&command_list_address_backup_ccc, port_virt_addr_ccc + superbyte_ccc[11], 8);    //potential superbyte
  memcpy(&fis_address_backup_ccc, port_virt_addr_ccc + superbyte_ccc[12], 8);    //potential superbyte

  // backup the interrupt settings
  memcpy(&interrupt_backup_ccc, port_virt_addr_ccc + superbyte_ccc[13], 4);    //potential superbyte


  // turn interupts off
  ahci_interrupt_changed_ccc = true;
  memset(port_virt_addr_ccc + superbyte_ccc[13], 0x0, 4);    //potential superbyte

  // wait for 1ms
  do_nanosleep_ccc(1000000);

  // clear the start bit
  disable_start_ccc(START_BIT_TIME);

  // wait for 1ms
  do_nanosleep_ccc(1000000);

  // clear the fis enable bit
  disable_fis_ccc(FIS_BIT_TIME);

  // wait for 1ms
  do_nanosleep_ccc(1000000);

  // set new addresses
  ahci_address_changed_ccc = true;
  uint64_t command_list_address = command_list_physical_address_ccc;
  memcpy(port_virt_addr_ccc + superbyte_ccc[11], &command_list_address, 8);    //potential superbyte
  uint64_t fis_address = fis_physical_address_ccc;
  memcpy(port_virt_addr_ccc + superbyte_ccc[12], &fis_address, 8);    //potential superbyte

  // wait for 1ms
  do_nanosleep_ccc(1000000);

  // set the fis enable bit
  enable_fis_ccc(FIS_BIT_TIME);

  // wait for 1ms
  do_nanosleep_ccc(1000000);

  if (1)
  {
    fprintf (stdout, "port data after address change\n");
    fprintf (hba_debug_file, "port data after address change\n");
    fflush(hba_debug_file);
    ptf = fileno(hba_debug_file);
    fsync(ptf);
    memcpy(temp_port_data, port_virt_addr_ccc, 0x80);
    dump_hba_data_to_file_ccc(stdout, temp_port_data, 0x80);
    dump_hba_data_to_file_ccc(hba_debug_file, temp_port_data, 0x80);
  }



  int soft_timeout = 0;
  if (1)
  {
    fprintf (stdout, "port soft reset\n");
    fprintf (hba_debug_file, "port soft reset\n");
    fflush(hba_debug_file);
    ptf = fileno(hba_debug_file);
    fsync(ptf);
    // clear the start bit
    uint8_t byte;
    memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[10], 1);    //potential superbyte
    byte = byte & 0xfe;
    memcpy(port_virt_addr_ccc + superbyte_ccc[10], &byte, 1);    //potential superbyte

    // set the buffer size to 0 for no data transfer
    unsigned long long backup_buffer_size = ccc_main_buffer_size_ccc;
    ccc_main_buffer_size_ccc = 0;
    set_main_buffer_ccc();

    int status = 0;

    // clear the command list
    memset(command_list_buffer_ccc, 0, command_list_size_ccc);
    // set the command FIS length, 5 dwords for h2d, also contains read/write bit
    io_singlebyte_ccc = 5;
    io_singlebyte_ccc = io_singlebyte_ccc | ((1) << 6);
    memcpy(command_list_buffer_ccc, &io_singlebyte_ccc, 1);
    // set the reset bit in the command fis
    memset(command_list_buffer_ccc+1, 1, 1);

    // clear the fis buffer
    memset(fis_buffer_ccc, 0, fis_size_ccc);
    // clear the command FIS
    memset(table_buffer_ccc, 0, 0x80);
    // set FIS type
    memset(table_buffer_ccc, REG_H2D, 1);
    // set type as control
    memset(table_buffer_ccc+1, 0x00, 1);
    // set the table entry length
    uint16_t word = table_entry_count_ccc;
    memcpy(command_list_buffer_ccc+2, &word, 2);
    // set the command table address
    uint64_t qword = table_physical_address_ccc;
    memcpy(command_list_buffer_ccc+8, &qword, 8);

    // set the soft reset control bit
    memset(table_buffer_ccc+15, 4, 1);

    // set the command list override bit
    byte = byte | 8;
    memcpy(port_virt_addr_ccc + superbyte_ccc[10], &byte, 1);    //potential superbyte
    // wait for clo bit to be clear
    int check_bit = 1;
    unsigned long long start_time = get_elapsed_usec_ccc();
    while (check_bit)
    {
      unsigned long long elapsed_time = get_elapsed_usec_ccc();
      if (elapsed_time > start_time + COMMAND_LIST_OVERRIDE_TIME)
      {
        // if it exceeds general timeout then quit
        break;
      }
      memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[10], 1);    //potential superbyte
      check_bit = byte & 0x08;
      // give the cpu a chance to do something else so we are not using 100%
      do_nanosleep_ccc(1);
    }

    // make sure fis is running
    enable_fis_ccc(FIS_BIT_TIME);

    // set the start bit
    enable_start_ccc(START_BIT_TIME);

    // set the command issue bit
    enable_command_issue_ccc(COMMAND_BIT_TIME);

    do_nanosleep_ccc(500);

    // clear the soft reset control bit
    memset(table_buffer_ccc+15, 0, 1);

    // clear the start bit
    disable_start_ccc(START_BIT_TIME);

    // set the command list override bit
    byte = byte | 8;
    memcpy(port_virt_addr_ccc + superbyte_ccc[10], &byte, 1);    //potential superbyte
    // wait for clo bit to be clear
    check_bit = 1;
    start_time = get_elapsed_usec_ccc();
    while (check_bit)
    {
      unsigned long long elapsed_time = get_elapsed_usec_ccc();
      if (elapsed_time > start_time + COMMAND_LIST_OVERRIDE_TIME)
      {
        // if it exceeds general timeout then quit
        break;
      }
      memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[10], 1);    //potential superbyte
      check_bit = byte & 0x08;
      // give the cpu a chance to do something else so we are not using 100%
      do_nanosleep_ccc(1);
    }

    // set the start bit
    enable_start_ccc(START_BIT_TIME);

    // set the command issue bit
    enable_command_issue_ccc(COMMAND_BIT_TIME);

    // wait for 1ms
    do_nanosleep_ccc(1000000);

    // clear the start bit
    disable_start_ccc(START_BIT_TIME);

    // make sure fis is running
    enable_fis_ccc(FIS_BIT_TIME);

    // wait for device to be ready
    status = wait_not_busy_or_drq_ccc(reset_wait_time_ccc, 0);
    if (status == 1)
    {
      fprintf (stdout, "did not become ready after soft reset\n");
      fprintf (hba_debug_file, "did not become ready after soft reset\n");
      fflush(hba_debug_file);
      ptf = fileno(hba_debug_file);
      fsync(ptf);
      soft_timeout = 1;
    }

    // clear any interrupt bits
    ahci_interrupt_changed_ccc = true;
    memset(port_virt_addr_ccc + superbyte_ccc[7], 0xff, 4);    //potential superbyte

    // put the buffer size back
    ccc_main_buffer_size_ccc = backup_buffer_size;
    set_main_buffer_ccc();
  }







  if (soft_timeout)
  {
    fprintf (stdout, "port com reset\n");
    fprintf (hba_debug_file, "port com reset\n");
    fflush(hba_debug_file);
    ptf = fileno(hba_debug_file);
    fsync(ptf);
    // clear the start bit in case it is not clear
    memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[10], 1);    //potential superbyte
    byte = byte & 0xfe;
    memcpy(port_virt_addr_ccc + superbyte_ccc[10], &byte, 1);    //potential superbyte

    // set the comreset bit in the Device Detection Initialization field
    memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[16], 1);
    byte = byte | 0x01;
    memcpy(port_virt_addr_ccc + superbyte_ccc[16], &byte, 1);

    // wait for 1ms
    do_nanosleep_ccc(1000000);

    // clear the comreset bit
    byte = byte & 0xfe;
    memcpy(port_virt_addr_ccc + superbyte_ccc[16], &byte, 1);

    // wait for device to connect
    memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[17], 1);
    byte = byte & 0x0f;
    unsigned long long start_time = get_elapsed_usec_ccc();
    while (byte != 3)
    {
      unsigned long long elapsed_time = get_elapsed_usec_ccc();
      if (elapsed_time > start_time + reset_wait_time_ccc)
      {
        // if it exceeds the reset timeout then quit
        fprintf (stdout, "port took too long to connect\n");
        fprintf (hba_debug_file, "port took too long to connect\n");
        fflush(hba_debug_file);
        ptf = fileno(hba_debug_file);
        fsync(ptf);
        break;
      }
      memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[17], 1);
      byte = byte & 0x0f;
      // give the cpu a chance to do something else so we are not using 100%
      do_nanosleep_ccc(1);
    }

    // clear all error bits
    memset(port_virt_addr_ccc + superbyte_ccc[18], 0xff, 4);

    // clear the start bit
    if (disable_start_ccc(START_BIT_TIME))
    {
      fprintf (stdout, "port command list did not stop in time\n");
      fprintf (hba_debug_file, "port command list did not stop in time\n");
      fflush(hba_debug_file);
      ptf = fileno(hba_debug_file);
      fsync(ptf);
    }

    // wait for drive to be ready
    int busy = 1;
    start_time = get_elapsed_usec_ccc();
    while (busy)
    {
      unsigned long long elapsed_time = get_elapsed_usec_ccc();
      if (elapsed_time > start_time + reset_wait_time_ccc)
      {
        // if it times out then fail
        fprintf (stdout, "port stayed busy through reset wait time\n");
        fprintf (hba_debug_file, "port stayed busy through reset wait time\n");
        fflush(hba_debug_file);
        ptf = fileno(hba_debug_file);
        fsync(ptf);
        break;
      }
      memcpy(&io_doubleword_ccc, port_virt_addr_ccc + superbyte_ccc[8], 4);
      busy = io_doubleword_ccc & 0x80;
      // give the cpu a chance to do something else so we are not using 100%
      //do_nanosleep_ccc(1);
    }
  }

  if (1)
  {
    fprintf (stdout, "port data after reset\n");
    fprintf (hba_debug_file, "port data after reset\n");
    fflush(hba_debug_file);
    ptf = fileno(hba_debug_file);
    fsync(ptf);
    memcpy(temp_port_data, port_virt_addr_ccc, 0x80);
    dump_hba_data_to_file_ccc(stdout, temp_port_data, 0x80);
    dump_hba_data_to_file_ccc(hba_debug_file, temp_port_data, 0x80);
  }

  if (1)
  {
    fprintf (stdout, "fis buffer after reset\n");
    fprintf (hba_debug_file, "fis buffer after reset\n");
    fflush(hba_debug_file);
    ptf = fileno(hba_debug_file);
    fsync(ptf);
    memcpy(temp_data, fis_buffer_ccc, 256);
    dump_data_to_file_ccc(stdout, temp_data, 256);
    dump_data_to_file_ccc(hba_debug_file, temp_data, 256);
  }



  int timeout = 0;
  if (1)
  {
    // identify device
    // wait for drive to become ready
    int busy = 1;
    int drq = 1;
    unsigned long long start_time = get_elapsed_usec_ccc();
    while (busy || drq)
    {
      unsigned long long elapsed_time = get_elapsed_usec_ccc();
      if (elapsed_time > start_time + initial_busy_wait_time_ccc)
      {
        timeout = 1;
        fprintf (stdout, "drive did not become ready after reset\n");
        fprintf (hba_debug_file, "drive did not become ready after reset\n");
        fflush(hba_debug_file);
        ptf = fileno(hba_debug_file);
        fsync(ptf);
        break;
      }
      memcpy(&io_doubleword_ccc, port_virt_addr_ccc + superbyte_ccc[8], 4);    //potential superbyte
      busy = io_doubleword_ccc & 0x80;
      drq = io_doubleword_ccc & 0x08;
      do_nanosleep_ccc(1);
    }

    if (!timeout)
    {
      ccc_main_buffer_size_ccc = 512;
      set_main_buffer_ccc();
      memset (ccc_buffer_ccc, 0, ccc_main_buffer_size_ccc);
      // clear the fis buffer
      memset(fis_buffer_ccc, 0, fis_size_ccc);
      // clear the command FIS
      memset(table_buffer_ccc, 0, 0x80);
      // set FIS type
      memset(table_buffer_ccc, REG_H2D, 1);
      // set type as command
      memset(table_buffer_ccc+1, 0x80, 1);
      // set the command register for identify device
      memset(table_buffer_ccc+2, 0xec, 1);
      if (enable_data_dump_ccc)
      {
        // set the count to 1
        memset(table_buffer_ccc+12, 1, 1);
        // set the control register value (something unknown)
        memset(table_buffer_ccc+15, 0x08, 1);
      }
      // clear the command list
      memset(command_list_buffer_ccc, 0, command_list_size_ccc);
      // set the command FIS length, 5 dwords for h2d
      memset(command_list_buffer_ccc, 5, 1);
      // set the table entry length
      uint16_t word = table_entry_count_ccc;
      memcpy(command_list_buffer_ccc+2, &word, 2);
      // set the command table address
      uint64_t qword = table_physical_address_ccc;
      memcpy(command_list_buffer_ccc+8, &qword, 8);

      // set the start bit
      enable_start_ccc(START_BIT_TIME);
      // wait for 1ms
      do_nanosleep_ccc(1000000);
      // set the command issue bit
      enable_command_issue_ccc(COMMAND_BIT_TIME);

      // wait for it to complete
      busy = 1;
      drq = 1;
      unsigned long long start_time = get_elapsed_usec_ccc();
      while (busy || drq)
      {
        unsigned long long elapsed_time = get_elapsed_usec_ccc();
        if (elapsed_time > start_time + initial_busy_wait_time_ccc)
        {
          timeout = 1;
          fprintf (stdout, "identify device command did not complete\n");
          fprintf (hba_debug_file, "identify device command did not complete\n");
          fflush(hba_debug_file);
          ptf = fileno(hba_debug_file);
          fsync(ptf);
          break;
        }
        memcpy(&io_doubleword_ccc, port_virt_addr_ccc + superbyte_ccc[8], 4);    //potential superbyte
        busy = io_doubleword_ccc & 0x80;
        drq = io_doubleword_ccc & 0x08;
        do_nanosleep_ccc(1);
      }

      // clear the start bit
      disable_start_ccc(START_BIT_TIME);
      // wait for 1ms
      do_nanosleep_ccc(1000000);

      if (1)
      {
        fprintf (stdout, "port data after identify device\n");
        fprintf (hba_debug_file, "port data after identify device\n");
        fflush(hba_debug_file);
        ptf = fileno(hba_debug_file);
        fsync(ptf);
        memcpy(temp_port_data, port_virt_addr_ccc, 0x80);
        dump_hba_data_to_file_ccc(stdout, temp_port_data, 0x80);
        dump_hba_data_to_file_ccc(hba_debug_file, temp_port_data, 0x80);
      }
      if (1)
      {
        fprintf (stdout, "fis buffer after identify device\n");
        fprintf (hba_debug_file, "fis buffer after identify device\n");
        fflush(hba_debug_file);
        ptf = fileno(hba_debug_file);
        fsync(ptf);
        memcpy(temp_data, fis_buffer_ccc, 256);
        dump_data_to_file_ccc(stdout, temp_data, 256);
        dump_data_to_file_ccc(hba_debug_file, temp_data, 256);
      }
      if (1)
      {
        fprintf (stdout, "table buffer after identify device\n");
        fprintf (hba_debug_file, "table buffer after identify device\n");
        fflush(hba_debug_file);
        ptf = fileno(hba_debug_file);
        fsync(ptf);
        memcpy(temp_data, table_buffer_ccc, 256);
        dump_data_to_file_ccc(stdout, temp_data, 256);
        dump_data_to_file_ccc(hba_debug_file, temp_data, 256);
      }
      if (1)
      {
        fprintf (stdout, "command list buffer after identify device\n");
        fprintf (hba_debug_file, "command list buffer after identify device\n");
        fflush(hba_debug_file);
        ptf = fileno(hba_debug_file);
        fsync(ptf);
        memcpy(temp_data, command_list_buffer_ccc, 128);
        dump_data_to_file_ccc(stdout, temp_data, 128);
        dump_data_to_file_ccc(hba_debug_file, temp_data, 128);
      }

      if (1)
      {
        fprintf (stdout, "identify device data\n");
        fprintf (hba_debug_file, "identify device data\n");
        fflush(hba_debug_file);
        ptf = fileno(hba_debug_file);
        fsync(ptf);
        memcpy(temp_data, ccc_buffer_ccc, 512);
        dump_data_to_file_ccc(stdout, ccc_buffer_ccc, 512);
        dump_data_to_file_ccc(hba_debug_file, ccc_buffer_ccc, 512);
      }

      if(1)
      {
        // fis data
        int data_size = 0x100;
        uint64_t address;
        memcpy(&address, port_virt_addr_ccc + superbyte_ccc[12], 8);    //potential superbyte
        fprintf (stdout, "fis data after identify device, address=%08llx\n", (unsigned long long)address);
        fprintf (hba_debug_file, "fis data after identify device, address=%08llx\n", (unsigned long long)address);
        fflush(hba_debug_file);
        ptf = fileno(hba_debug_file);
        fsync(ptf);

        if (!driver_installed_ccc)
        {
          strcpy (tempmessage_ccc, curlang_ccc[LANGMISSINGDRIVER]);
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
          clear_error_message_ccc();
          return -1;
        }

        //driver_buffer_ccc = malloc(DRIVER_TRANSFER_BUFFER_SIZE);
        memset(driver_control_data_ccc.buffer, 0, sizeof(driver_control_data_ccc.buffer));
        //driver_control_data_ccc.buffer = driver_buffer_ccc;

        sprintf (driver_device_name_ccc, "/proc/%s%d", MAIN_DRIVER_IOCTL_NAME, process_id_ccc);
        if (main_driver_fd_ccc > 0)
        {
          close(main_driver_fd_ccc);
        }
        main_driver_fd_ccc = open(driver_device_name_ccc, O_RDWR);
        if (main_driver_fd_ccc == -1)
        {
          check_message_ccc = true;
          strcpy (tempmessage_ccc, curlang_ccc[LANGUERROROPENINGDRIVER]);
          message_error_ccc(tempmessage_ccc);
          sprintf (tempmessage_ccc, " %s (%s)\n", driver_device_name_ccc, strerror(errno));
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
          clear_error_message_ccc();
          return -1;
        }

        driver_control_data_ccc.command = READ_MEMORY_DRIVE_COMMAND;
        unsigned long request = IOCTL_CMD_HDDSC;
        //strcpy(driver_control_data_ccc.buffer, virtual_driver_name_ccc);
        driver_control_data_ccc.read_timeout = address;
        driver_control_data_ccc.request_timeout = data_size;
        int ret = ioctl(main_driver_fd_ccc, request, &driver_control_data_ccc);
        if (ret < 0)
        {
          strcpy (tempmessage_ccc, curlang_ccc[LANGERRORSTARTINGDRIVER]);
          message_error_ccc(tempmessage_ccc);
          sprintf (tempmessage_ccc, " %s (%s)\n", driver_device_name_ccc, strerror(errno));
          message_error_ccc(tempmessage_ccc);
          return -1;
        }

        memcpy (temp_data, driver_control_data_ccc.buffer, data_size);
        dump_data_to_file_ccc(stdout, temp_data, data_size);
        dump_data_to_file_ccc(hba_debug_file, temp_data, data_size);

        close(main_driver_fd_ccc);
        //free(driver_buffer_ccc);
      }

      unsigned char command_data[0x400];
      if(1)
      {
        // command data
        int data_size = 0x400;
        uint64_t address;
        memcpy(&address, port_virt_addr_ccc + superbyte_ccc[11], 8);    //potential superbyte
        fprintf (stdout, "command list data after identify device, address=%08llx\n", (unsigned long long)address);
        fprintf (hba_debug_file, "command list data after identify device, address=%08llx\n", (unsigned long long)address);
        fflush(hba_debug_file);
        ptf = fileno(hba_debug_file);
        fsync(ptf);

        if (!driver_installed_ccc)
        {
          strcpy (tempmessage_ccc, curlang_ccc[LANGMISSINGDRIVER]);
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
          clear_error_message_ccc();
          return -1;
        }

        //driver_buffer_ccc = malloc(DRIVER_TRANSFER_BUFFER_SIZE);
        memset(driver_control_data_ccc.buffer, 0, sizeof(driver_control_data_ccc.buffer));
        //driver_control_data_ccc.buffer = driver_buffer_ccc;

        sprintf (driver_device_name_ccc, "/proc/%s%d", MAIN_DRIVER_IOCTL_NAME, process_id_ccc);
        if (main_driver_fd_ccc > 0)
        {
          close(main_driver_fd_ccc);
        }
        main_driver_fd_ccc = open(driver_device_name_ccc, O_RDWR);
        if (main_driver_fd_ccc == -1)
        {
          check_message_ccc = true;
          strcpy (tempmessage_ccc, curlang_ccc[LANGUERROROPENINGDRIVER]);
          message_error_ccc(tempmessage_ccc);
          sprintf (tempmessage_ccc, " %s (%s)\n", driver_device_name_ccc, strerror(errno));
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
          clear_error_message_ccc();
          return -1;
        }

        driver_control_data_ccc.command = READ_MEMORY_DRIVE_COMMAND;
        unsigned long request = IOCTL_CMD_HDDSC;
        //strcpy(driver_control_data_ccc.buffer, virtual_driver_name_ccc);
        driver_control_data_ccc.read_timeout = address;
        driver_control_data_ccc.request_timeout = data_size;
        int ret = ioctl(main_driver_fd_ccc, request, &driver_control_data_ccc);
        if (ret < 0)
        {
          strcpy (tempmessage_ccc, curlang_ccc[LANGERRORSTARTINGDRIVER]);
          message_error_ccc(tempmessage_ccc);
          sprintf (tempmessage_ccc, " %s (%s)\n", driver_device_name_ccc, strerror(errno));
          message_error_ccc(tempmessage_ccc);
          return -1;
        }

        memcpy (temp_data, driver_control_data_ccc.buffer, data_size);
        dump_data_to_file_ccc(stdout, temp_data, data_size);
        dump_data_to_file_ccc(hba_debug_file, temp_data, data_size);

        memcpy (command_data, temp_data, data_size);

        close(main_driver_fd_ccc);
        //free(driver_buffer_ccc);
      }

      if(1)
      {
        // command table data
        int i;
        for (i = 0; i < 32; i++)
        {
          uint16_t table_length;
          memcpy (&table_length, command_data + (0x20 * i) + 2, 2);
          uint64_t table_address;
          memcpy (&table_address, command_data + ((0x20 * i) + 8), 8);
          if (table_length == 0 || table_address == 0)
          {
            fprintf (stdout, "command table data after identify device %d is zero\n", i);
            fprintf (hba_debug_file, "command table data after identify device %d is zero\n", i);
            fflush(hba_debug_file);
            ptf = fileno(hba_debug_file);
            fsync(ptf);
          }
          else
          {
            int data_size = 0x80 + (table_length * 0x10);
            if (data_size > 256)
            {
              fprintf (stdout, "command table data size is too big %d\n", data_size);
              fprintf (hba_debug_file, "command table data size is too big %d\n", data_size);
              fflush(hba_debug_file);
              ptf = fileno(hba_debug_file);
              fsync(ptf);
              data_size = 256;
            }
            uint64_t address;
            address = table_address;
            fprintf (stdout, "command table data after identify device %d, address=%08llx\n", i, (unsigned long long)address);
            fprintf (hba_debug_file, "command table data after identify device %d, address=%08llx\n", i, (unsigned long long)address);
            fflush(hba_debug_file);
            ptf = fileno(hba_debug_file);
            fsync(ptf);

            if (!driver_installed_ccc)
            {
              strcpy (tempmessage_ccc, curlang_ccc[LANGMISSINGDRIVER]);
              message_error_ccc(tempmessage_ccc);
              print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
              clear_error_message_ccc();
              return -1;
            }

            //driver_buffer_ccc = malloc(DRIVER_TRANSFER_BUFFER_SIZE);
            memset(driver_control_data_ccc.buffer, 0, sizeof(driver_control_data_ccc.buffer));
            //driver_control_data_ccc.buffer = driver_buffer_ccc;

            sprintf (driver_device_name_ccc, "/proc/%s%d", MAIN_DRIVER_IOCTL_NAME, process_id_ccc);
            if (main_driver_fd_ccc > 0)
            {
              close(main_driver_fd_ccc);
            }
            main_driver_fd_ccc = open(driver_device_name_ccc, O_RDWR);
            if (main_driver_fd_ccc == -1)
            {
              check_message_ccc = true;
              strcpy (tempmessage_ccc, curlang_ccc[LANGUERROROPENINGDRIVER]);
              message_error_ccc(tempmessage_ccc);
              sprintf (tempmessage_ccc, " %s (%s)\n", driver_device_name_ccc, strerror(errno));
              message_error_ccc(tempmessage_ccc);
              print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
              clear_error_message_ccc();
              return -1;
            }

            driver_control_data_ccc.command = READ_MEMORY_DRIVE_COMMAND;
            unsigned long request = IOCTL_CMD_HDDSC;
            //strcpy(driver_control_data_ccc.buffer, virtual_driver_name_ccc);
            driver_control_data_ccc.read_timeout = address;
            driver_control_data_ccc.request_timeout = data_size;
            int ret = ioctl(main_driver_fd_ccc, request, &driver_control_data_ccc);
            if (ret < 0)
            {
              strcpy (tempmessage_ccc, curlang_ccc[LANGERRORSTARTINGDRIVER]);
              message_error_ccc(tempmessage_ccc);
              sprintf (tempmessage_ccc, " %s (%s)\n", driver_device_name_ccc, strerror(errno));
              message_error_ccc(tempmessage_ccc);
              return -1;
            }

            memcpy (temp_data, driver_control_data_ccc.buffer, data_size);
            dump_data_to_file_ccc(stdout, temp_data, data_size);
            dump_data_to_file_ccc(hba_debug_file, temp_data, data_size);

            close(main_driver_fd_ccc);
            //free(driver_buffer_ccc);
          }
        }
      }
    }
  }



  if (enable_hba_reset_on_connect_source_ccc)
  {
    // reset hba and try again
    if (ahci_address_changed_ccc)
    {
      // restore addresses
      memcpy(port_virt_addr_ccc + superbyte_ccc[11], &command_list_address_backup_ccc, 8);    //potential superbyte
      memcpy(port_virt_addr_ccc + superbyte_ccc[12], &fis_address_backup_ccc, 8);    //potential superbyte
      ahci_address_changed_ccc = false;
    }
    if (ahci_interrupt_changed_ccc)
    {
      // restore the interrupt settings
      memcpy(port_virt_addr_ccc + superbyte_ccc[13], &interrupt_backup_ccc, 4);    //potential superbyte
      ahci_interrupt_changed_ccc = false;
    }
    if (memory_mapped_ccc)
    {
      // close mapped memory
      munmap(hba_mem_pointer_ccc, hba_alloc_mem_size_ccc);
      close(hba_mem_dev_ccc);
      munmap(port_mem_pointer_ccc, port_alloc_mem_size_ccc);
      close(port_mem_dev_ccc);
      memory_mapped_ccc = false;
    }

    hba_reset_ccc();

    hba_mem_dev_ccc = open("/dev/mem", O_RDWR | O_SYNC);
    if(hba_mem_dev_ccc == -1)
    {
      fprintf (stderr, "unable to open /dev/mem\n");
      return (-1);
    }
    page_size = pagesize_ccc;
    hba_alloc_mem_size_ccc = (((hba_mem_size / page_size) + 1) * page_size);
    page_mask = (page_size - 1);

    hba_mem_pointer_ccc = mmap(NULL,
                               hba_alloc_mem_size_ccc,
                               PROT_READ | PROT_WRITE,
                               MAP_SHARED,
                               hba_mem_dev_ccc,
                               (hba_mem_address & ~page_mask)
    );
    if(hba_mem_pointer_ccc == MAP_FAILED)
    {
      fprintf (stderr, "HBA mem map failed\n");
      return (-1);
    }
    hba_virt_addr_ccc = (hba_mem_pointer_ccc + (hba_mem_address & page_mask));
    memory_mapped_ccc = true;

    port_mem_dev_ccc = open("/dev/mem", O_RDWR | O_SYNC);
    if(port_mem_dev_ccc == -1)
    {
      fprintf (stderr, "unable to open /dev/mem\n");
      return (-1);
    }
    page_size = pagesize_ccc;
    port_alloc_mem_size_ccc = (((port_mem_size / page_size) + 1) * page_size);
    page_mask = (page_size - 1);
    port_mem_pointer_ccc = mmap(NULL,
                                port_alloc_mem_size_ccc,
                                PROT_READ | PROT_WRITE,
                                MAP_SHARED,
                                port_mem_dev_ccc,
                                (port_mem_address & ~page_mask)
    );
    if(port_mem_pointer_ccc == MAP_FAILED)
    {
      fprintf (stderr, "Port mem map failed\n");
      return (-1);
    }
    port_virt_addr_ccc = (port_mem_pointer_ccc + (port_mem_address & page_mask));
    memory_mapped_ccc = true;

    if (1)
    {
      fprintf (stdout, "hba data after hba reset, address=%08llx\n", hba_base_address_ccc);
      fprintf (hba_debug_file, "hba data after hba reset, address=%08llx\n", hba_base_address_ccc);
      fflush(hba_debug_file);
      ptf = fileno(hba_debug_file);
      fsync(ptf);
      memcpy(temp_hba_data, hba_virt_addr_ccc, 0x30);
      dump_hba_data_to_file_ccc(stdout, temp_hba_data, 0x30);
      dump_hba_data_to_file_ccc(hba_debug_file, temp_hba_data, 0x30);
    }
    if (1)
    {
      fprintf (stdout, "port data after hba reset, address=%08llx\n", port_base_address_ccc);
      fprintf (hba_debug_file, "port data after hba reset, address=%08llx\n", port_base_address_ccc);
      fflush(hba_debug_file);
      ptf = fileno(hba_debug_file);
      fsync(ptf);
      memcpy(temp_port_data, port_virt_addr_ccc, 0x80);
      dump_hba_data_to_file_ccc(stdout, temp_port_data, 0x80);
      dump_hba_data_to_file_ccc(hba_debug_file, temp_port_data, 0x80);
    }

    // backup current addresses
    memcpy(&command_list_address_backup_ccc, port_virt_addr_ccc + superbyte_ccc[11], 8);    //potential superbyte
    memcpy(&fis_address_backup_ccc, port_virt_addr_ccc + superbyte_ccc[12], 8);    //potential superbyte

    // backup the interrupt settings
    memcpy(&interrupt_backup_ccc, port_virt_addr_ccc + superbyte_ccc[13], 4);    //potential superbyte


    // turn interupts off
    ahci_interrupt_changed_ccc = true;
    memset(port_virt_addr_ccc + superbyte_ccc[13], 0x0, 4);    //potential superbyte

    // wait for 1ms
    do_nanosleep_ccc(1000000);

    // clear the start bit
    disable_start_ccc(START_BIT_TIME);

    // wait for 1ms
    do_nanosleep_ccc(1000000);

    // clear the fis enable bit
    disable_fis_ccc(FIS_BIT_TIME);

    // wait for 1ms
    do_nanosleep_ccc(1000000);

    // set new addresses
    ahci_address_changed_ccc = true;
    uint64_t command_list_address = command_list_physical_address_ccc;
    memcpy(port_virt_addr_ccc + superbyte_ccc[11], &command_list_address, 8);    //potential superbyte
    uint64_t fis_address = fis_physical_address_ccc;
    memcpy(port_virt_addr_ccc + superbyte_ccc[12], &fis_address, 8);    //potential superbyte

    // wait for 1ms
    do_nanosleep_ccc(1000000);

    // set the fis enable bit
    enable_fis_ccc(FIS_BIT_TIME);

    // wait for 1ms
    do_nanosleep_ccc(1000000);

    if (1)
    {
      fprintf (stdout, "port data after address change\n");
      fprintf (hba_debug_file, "port data after address change\n");
      fflush(hba_debug_file);
      ptf = fileno(hba_debug_file);
      fsync(ptf);
      memcpy(temp_port_data, port_virt_addr_ccc, 0x80);
      dump_hba_data_to_file_ccc(stdout, temp_port_data, 0x80);
      dump_hba_data_to_file_ccc(hba_debug_file, temp_port_data, 0x80);
    }

    timeout = 0;
    if (1)
    {
      // identify device
      // wait for drive to become ready
      int busy = 1;
      int drq = 1;
      unsigned long long start_time = get_elapsed_usec_ccc();
      while (busy || drq)
      {
        unsigned long long elapsed_time = get_elapsed_usec_ccc();
        if (elapsed_time > start_time + initial_busy_wait_time_ccc)
        {
          timeout = 1;
          fprintf (stdout, "drive did not become ready after hba reset\n");
          fprintf (hba_debug_file, "drive did not become ready after hba reset\n");
          fflush(hba_debug_file);
          ptf = fileno(hba_debug_file);
          fsync(ptf);
          break;
        }
        memcpy(&io_doubleword_ccc, port_virt_addr_ccc + superbyte_ccc[8], 4);    //potential superbyte
        busy = io_doubleword_ccc & 0x80;
        drq = io_doubleword_ccc & 0x08;
        do_nanosleep_ccc(1);
      }

      if (!timeout)
      {
        ccc_main_buffer_size_ccc = 512;
        set_main_buffer_ccc();
        memset (ccc_buffer_ccc, 0, ccc_main_buffer_size_ccc);
        // clear the fis buffer
        memset(fis_buffer_ccc, 0, fis_size_ccc);
        // clear the command FIS
        memset(table_buffer_ccc, 0, 0x80);
        // set FIS type
        memset(table_buffer_ccc, REG_H2D, 1);
        // set type as command
        memset(table_buffer_ccc+1, 0x80, 1);
        // set the command register for identify device
        memset(table_buffer_ccc+2, 0xec, 1);
        if (enable_data_dump_ccc)
        {
          // set the count to 1
          memset(table_buffer_ccc+12, 1, 1);
          // set the control register value (something unknown)
          memset(table_buffer_ccc+15, 0x08, 1);
        }
        // clear the command list
        memset(command_list_buffer_ccc, 0, command_list_size_ccc);
        // set the command FIS length, 5 dwords for h2d
        memset(command_list_buffer_ccc, 5, 1);
        // set the table entry length
        uint16_t word = table_entry_count_ccc;
        memcpy(command_list_buffer_ccc+2, &word, 2);
        // set the command table address
        uint64_t qword = table_physical_address_ccc;
        memcpy(command_list_buffer_ccc+8, &qword, 8);

        // set the start bit
        enable_start_ccc(START_BIT_TIME);
        // wait for 1ms
        do_nanosleep_ccc(1000000);
        // set the command issue bit
        enable_command_issue_ccc(COMMAND_BIT_TIME);

        // wait for it to complete
        busy = 1;
        drq = 1;
        unsigned long long start_time = get_elapsed_usec_ccc();
        while (busy || drq)
        {
          unsigned long long elapsed_time = get_elapsed_usec_ccc();
          if (elapsed_time > start_time + initial_busy_wait_time_ccc)
          {
            timeout = 1;
            fprintf (stdout, "identify device command did not complete\n");
            fprintf (hba_debug_file, "identify device command did not complete\n");
            fflush(hba_debug_file);
            ptf = fileno(hba_debug_file);
            fsync(ptf);
            break;
          }
          memcpy(&io_doubleword_ccc, port_virt_addr_ccc + superbyte_ccc[8], 4);    //potential superbyte
          busy = io_doubleword_ccc & 0x80;
          drq = io_doubleword_ccc & 0x08;
          do_nanosleep_ccc(1);
        }

        // clear the start bit
        disable_start_ccc(START_BIT_TIME);
        // wait for 1ms
        do_nanosleep_ccc(1000000);

        if (1)
        {
          fprintf (stdout, "port data after identify device\n");
          fprintf (hba_debug_file, "port data after identify device\n");
          fflush(hba_debug_file);
          ptf = fileno(hba_debug_file);
          fsync(ptf);
          memcpy(temp_port_data, port_virt_addr_ccc, 0x80);
          dump_hba_data_to_file_ccc(stdout, temp_port_data, 0x80);
          dump_hba_data_to_file_ccc(hba_debug_file, temp_port_data, 0x80);
        }
        if (1)
        {
          fprintf (stdout, "fis buffer after identify device\n");
          fprintf (hba_debug_file, "fis buffer after identify device\n");
          fflush(hba_debug_file);
          ptf = fileno(hba_debug_file);
          fsync(ptf);
          memcpy(temp_data, fis_buffer_ccc, 256);
          dump_data_to_file_ccc(stdout, temp_port_data, 256);
          dump_data_to_file_ccc(hba_debug_file, temp_port_data, 256);
        }
        if (1)
        {
          fprintf (stdout, "table buffer after identify device\n");
          fprintf (hba_debug_file, "table buffer after identify device\n");
          fflush(hba_debug_file);
          ptf = fileno(hba_debug_file);
          fsync(ptf);
          memcpy(temp_data, table_buffer_ccc, 256);
          dump_data_to_file_ccc(stdout, temp_port_data, 256);
          dump_data_to_file_ccc(hba_debug_file, temp_port_data, 256);
        }
        if (1)
        {
          fprintf (stdout, "command list buffer after identify device\n");
          fprintf (hba_debug_file, "command list buffer after identify device\n");
          fflush(hba_debug_file);
          ptf = fileno(hba_debug_file);
          fsync(ptf);
          memcpy(temp_data, command_list_buffer_ccc, 128);
          dump_data_to_file_ccc(stdout, temp_port_data, 128);
          dump_data_to_file_ccc(hba_debug_file, temp_port_data, 128);
        }

        if (!timeout)
        {
          fprintf (stdout, "identify device data\n");
          fprintf (hba_debug_file, "identify device data\n");
          fflush(hba_debug_file);
          ptf = fileno(hba_debug_file);
          fsync(ptf);
          memcpy(temp_data, ccc_buffer_ccc, 512);
          dump_data_to_file_ccc(stdout, ccc_buffer_ccc, 512);
          dump_data_to_file_ccc(hba_debug_file, ccc_buffer_ccc, 512);
        }
      }
    }
  }






  if (ahci_address_changed_ccc)
  {
    // restore addresses
    memcpy(port_virt_addr_ccc + superbyte_ccc[11], &command_list_address_backup_ccc, 8);    //potential superbyte
    memcpy(port_virt_addr_ccc + superbyte_ccc[12], &fis_address_backup_ccc, 8);    //potential superbyte
    ahci_address_changed_ccc = false;
  }

  if (ahci_interrupt_changed_ccc)
  {
    // restore the interrupt settings
    memcpy(port_virt_addr_ccc + superbyte_ccc[13], &interrupt_backup_ccc, 4);    //potential superbyte
    ahci_interrupt_changed_ccc = false;
  }

  if (memory_mapped_ccc)
  {
    // close mapped memory
    munmap(hba_mem_pointer_ccc, hba_alloc_mem_size_ccc);
    close(hba_mem_dev_ccc);
    munmap(port_mem_pointer_ccc, port_alloc_mem_size_ccc);
    close(port_mem_dev_ccc);
    memory_mapped_ccc = false;
  }

  fclose (hba_debug_file);
  return 0;
}





int post_direct_ccc(int command_type)
{
  // clear passthrough sense data so it doesn't cause scripting problems
  set_number_variable_value_ccc("$io_sense_key", 0);
  set_number_variable_value_ccc("$io_asc", 0);
  set_number_variable_value_ccc("$io_ascq", 0);

  ata_return_valid_ccc = 1;

  int wrong_drive = 0;

  if (ahci_mode_ccc)
  {
    if (superbyte_ccc[37] == 0xf5)
    {
      // ahci
      // clear return data
      set_number_variable_value_ccc("$ata_return_error", 0);
      set_number_variable_value_ccc("$ata_return_count", 0);
      set_number_variable_value_ccc("$ata_return_lba", 0);
      set_number_variable_value_ccc("$ata_return_device", 0);
      set_number_variable_value_ccc("$ata_return_status", 0);
      set_number_variable_value_ccc("$ata_alternate_status", 0);

      // set error and status to current values
      set_number_variable_value_ccc("$ata_return_status", io_byte_ccc[1]);
      set_number_variable_value_ccc("$ata_alternate_status", 0);
      set_number_variable_value_ccc("$ata_return_error", io_byte_ccc[2]);
      memcpy(&ata_error_ccc, &io_byte_ccc[2], 1);
      memcpy(&ata_status_ccc, &io_byte_ccc[1], 1);

      // check if there is a d2h fis
      // if there is then get that data but keep current error and status
      memcpy(&io_singlebyte_ccc, fis_buffer_ccc + 0x40, 1);
      if (io_singlebyte_ccc == REG_D2H)
      {
        if (command_type == 28)
        {
          memcpy(&io_byte_ccc[1], fis_buffer_ccc + 0x42, 1);    // status
          memcpy(&io_byte_ccc[2], fis_buffer_ccc + 0x43, 1);    // error
          memcpy(&io_byte_ccc[3], fis_buffer_ccc + 0x44, 1);    // lba low
          memcpy(&io_byte_ccc[4], fis_buffer_ccc + 0x45, 1);    // lba mid
          memcpy(&io_byte_ccc[5], fis_buffer_ccc + 0x46, 1);    // lba high
          memcpy(&io_byte_ccc[6], fis_buffer_ccc + 0x47, 1);    // device
          memcpy(&io_byte_ccc[7], fis_buffer_ccc + 0x4c, 1);    // count

          unsigned long long lba = io_byte_ccc[3] + (io_byte_ccc[4] << 8) + (io_byte_ccc[5] << 16) + ((io_byte_ccc[5] & 0x0f) << 24);
          //set_number_variable_value_ccc("$ata_return_error", io_byte_ccc[2]);
          set_number_variable_value_ccc("$ata_return_count", io_byte_ccc[7]);
          set_number_variable_value_ccc("$ata_return_lba", lba);
          set_number_variable_value_ccc("$ata_return_device", io_byte_ccc[6]);
          set_number_variable_value_ccc("$ata_alternate_status", io_byte_ccc[1]);
          ata_lba_ccc = lba;
          ata_count_ccc = io_byte_ccc[7];
          memcpy(&ata_device_ccc, &io_byte_ccc[6], 1);
        }
        else if (command_type == 48)
        {
          memcpy(&io_byte_ccc[1], fis_buffer_ccc + 0x42, 1);    // status
          memcpy(&io_byte_ccc[2], fis_buffer_ccc + 0x43, 1);    // error
          memcpy(&io_byte_ccc[3], fis_buffer_ccc + 0x44, 1);    // lba1 7:0
          memcpy(&io_byte_ccc[4], fis_buffer_ccc + 0x45, 1);    // lba2 15:8
          memcpy(&io_byte_ccc[5], fis_buffer_ccc + 0x46, 1);    // lba3 23:16
          memcpy(&io_byte_ccc[6], fis_buffer_ccc + 0x47, 1);    // device
          memcpy(&io_byte_ccc[7], fis_buffer_ccc + 0x48, 1);    // lba4 31:24
          memcpy(&io_byte_ccc[8], fis_buffer_ccc + 0x49, 1);    // lba5 39:32
          memcpy(&io_byte_ccc[9], fis_buffer_ccc + 0x4a, 1);    // lba6 47:40
          memcpy(&io_byte_ccc[10], fis_buffer_ccc + 0x4c, 1);    // count low
          memcpy(&io_byte_ccc[11], fis_buffer_ccc + 0x4d, 1);    // count high

          unsigned long long lba = io_byte_ccc[3] + (io_byte_ccc[4] << 8) + (io_byte_ccc[5] << 16) + (io_byte_ccc[7] << 24) + ((unsigned long long)io_byte_ccc[8] << 32) + ((unsigned long long)io_byte_ccc[9] << 40);
          unsigned int count = io_byte_ccc[10] + (io_byte_ccc[11] << 8);
          //set_number_variable_value_ccc("$ata_return_error", io_byte_ccc[2]);
          set_number_variable_value_ccc("$ata_return_count", count);
          set_number_variable_value_ccc("$ata_return_lba", lba);
          set_number_variable_value_ccc("$ata_return_device", io_byte_ccc[6]);
          set_number_variable_value_ccc("$ata_alternate_status", io_byte_ccc[1]);
          ata_lba_ccc = lba;
          ata_count_ccc = count;
          memcpy(&ata_device_ccc, &io_byte_ccc[6], 1);
        }
      }

      #ifdef DEBUG
      if (debug_ccc & DEBUG36)
      {
        int i;
        for (i = 0; i < 256; i+=16)
        {
          fprintf (stdout, "%x: ", i);
          unsigned char *c;
          int n;
          for (n=0; n < 16 && i+n < 512; n++)
          {
            c = (unsigned char *)table_buffer_ccc+i+n;
            fprintf (stdout, "%02x ", *c);
          }
          fprintf (stdout, "   ");
          for (n=0; n < 16 && i+n < 512; n++)
          {
            c = (unsigned char *)table_buffer_ccc+i+n;
            fprintf (stdout, "%c", isprint(*c) ? *c : '.');
          }
          fprintf (stdout, "\n");
        }
      }

      if (debug_ccc & DEBUG36)
      {
        int i;
        for (i = 0; i < 256; i+=16)
        {
          fprintf (stdout, "%x: ", i);
          unsigned char *c;
          int n;
          for (n=0; n < 16 && i+n < 512; n++)
          {
            c = (unsigned char *)fis_buffer_ccc+i+n;
            fprintf (stdout, "%02x ", *c);
          }
          fprintf (stdout, "   ");
          for (n=0; n < 16 && i+n < 512; n++)
          {
            c = (unsigned char *)fis_buffer_ccc+i+n;
            fprintf (stdout, "%c", isprint(*c) ? *c : '.');
          }
          fprintf (stdout, "\n");
        }
      }

      if (debug_ccc & DEBUG36)
      {
        int i;
        for (i = 0; i < 128; i+=16)
        {
          fprintf (stdout, "%x: ", i);
          unsigned char *c;
          int n;
          for (n=0; n < 16 && i+n < 512; n++)
          {
            c = (unsigned char *)command_list_buffer_ccc+i+n;
            fprintf (stdout, "%02x ", *c);
          }
          fprintf (stdout, "   ");
          for (n=0; n < 16 && i+n < 512; n++)
          {
            c = (unsigned char *)command_list_buffer_ccc+i+n;
            fprintf (stdout, "%c", isprint(*c) ? *c : '.');
          }
          fprintf (stdout, "\n");
        }
      }
      #endif
    }
  }

  else
  {
    // ide
    // check if still working with the correct device
    io_byte_ccc[6] = inb(reg_base_address_ccc+6);
    if ( ((io_byte_ccc[6] & 0x10) >> 4) != device_select_base_ccc )
    {
      wrong_drive = 1;
    }

    if (command_type == 28)
    {
      io_byte_ccc[1] = inb(reg_base_address_ccc+1);
      io_byte_ccc[2] = inb(reg_base_address_ccc+2);
      io_byte_ccc[3] = inb(reg_base_address_ccc+3);
      io_byte_ccc[4] = inb(reg_base_address_ccc+4);
      io_byte_ccc[5] = inb(reg_base_address_ccc+5);
      io_byte_ccc[6] = inb(reg_base_address_ccc+6);
      io_byte_ccc[7] = inb(reg_base_address_ccc+7);
      io_byte_ccc[8] = inb(control_base_address_ccc);

      //fprintf (stdout, "return28= %02x %02x %02x %02x %02x %02x %02x %02x\n", io_byte_ccc[1], io_byte_ccc[2], io_byte_ccc[3], io_byte_ccc[4], io_byte_ccc[5], io_byte_ccc[6], io_byte_ccc[7], io_byte_ccc[8]);    //debug

      unsigned long long lba = io_byte_ccc[3] + (io_byte_ccc[4] << 8) + (io_byte_ccc[5] << 16) + ((io_byte_ccc[6] & 0x0f) << 24);
      set_number_variable_value_ccc("$ata_return_error", io_byte_ccc[1]);
      set_number_variable_value_ccc("$ata_return_count", io_byte_ccc[2]);
      set_number_variable_value_ccc("$ata_return_lba", lba);
      set_number_variable_value_ccc("$ata_return_device", io_byte_ccc[6]);
      set_number_variable_value_ccc("$ata_return_status", io_byte_ccc[7]);
      set_number_variable_value_ccc("$ata_alternate_status", io_byte_ccc[8]);
      memcpy(&ata_error_ccc, &io_byte_ccc[1], 1);
      memcpy(&ata_status_ccc, &io_byte_ccc[7], 1);    //status
      //memcpy(&ata_status_ccc, &io_byte_ccc[8], 1);    //altstatus
      memcpy(&ata_device_ccc, &io_byte_ccc[6], 1);
      ata_lba_ccc = lba;
      ata_count_ccc = io_byte_ccc[2];
    }

    else if (command_type == 48)
    {
      // read low registers
      io_byte_ccc[6] = inb(reg_base_address_ccc+1);
      io_byte_ccc[7] = inb(reg_base_address_ccc+2);
      io_byte_ccc[8] = inb(reg_base_address_ccc+3);
      io_byte_ccc[9] = inb(reg_base_address_ccc+4);
      io_byte_ccc[10] = inb(reg_base_address_ccc+5);
      io_byte_ccc[11] = inb(reg_base_address_ccc+6);
      io_byte_ccc[12] = inb(reg_base_address_ccc+7);

      // set high order bit to read high registers and keep inturrupts off
      outb(0x82, control_base_address_ccc);
      io_byte_ccc[1] = inb(reg_base_address_ccc+1);
      io_byte_ccc[2] = inb(reg_base_address_ccc+2);
      io_byte_ccc[3] = inb(reg_base_address_ccc+3);
      io_byte_ccc[4] = inb(reg_base_address_ccc+4);
      io_byte_ccc[5] = inb(reg_base_address_ccc+5);

      // clear high order bit and keep inturrupts off then read alt status reg to clear any inturrupts
      outb(2, control_base_address_ccc);
      io_byte_ccc[13] = inb(control_base_address_ccc);

      //fprintf (stdout, "return48= %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", io_byte_ccc[1], io_byte_ccc[2], io_byte_ccc[3], io_byte_ccc[4], io_byte_ccc[5], io_byte_ccc[6], io_byte_ccc[7], io_byte_ccc[8], io_byte_ccc[9], io_byte_ccc[10], io_byte_ccc[11], io_byte_ccc[12], io_byte_ccc[13]);    //debug

      unsigned long long lba = io_byte_ccc[8] + (io_byte_ccc[9] << 8) + (io_byte_ccc[10] << 16) + (io_byte_ccc[3] << 24) + ((unsigned long long)io_byte_ccc[4] << 32) + ((unsigned long long)io_byte_ccc[5] << 40);
      unsigned int count = io_byte_ccc[7] + (io_byte_ccc[2] << 8);
      set_number_variable_value_ccc("$ata_return_error", io_byte_ccc[6]);
      set_number_variable_value_ccc("$ata_return_count", count);
      set_number_variable_value_ccc("$ata_return_lba", lba);
      set_number_variable_value_ccc("$ata_return_device", io_byte_ccc[11]);
      set_number_variable_value_ccc("$ata_return_status", io_byte_ccc[12]);
      set_number_variable_value_ccc("$ata_alternate_status", io_byte_ccc[13]);
      memcpy(&ata_error_ccc, &io_byte_ccc[6], 1);
      memcpy(&ata_status_ccc, &io_byte_ccc[12], 1);    //status
      //memcpy(&ata_status_ccc, &io_byte_ccc[13], 1);    //altstatus
      memcpy(&ata_device_ccc, &io_byte_ccc[11], 1);
      ata_lba_ccc = lba;
      ata_count_ccc = count;
    }

    else
    {
      sprintf (tempmessage_ccc, "\n\nInternal error, post direct command type not valid\n\n");
      if (superclone_ccc)
      {
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
      }
      else
      {
        message_now_ccc(tempmessage_ccc);
      }
      exitcode_ccc = INTERNAL_ERROR_EXIT_CODE;
      return -1;
    }
  }

  return (wrong_drive);
}






int refresh_status_ccc(int disk_fd)
{
  if (direct_mode_ccc)
  {
    if (ahci_mode_ccc)
    {
      if (superbyte_ccc[38] == 0x50)
      {
        // ahci
        #ifdef DEBUG
        if (debug_ccc & DEBUG37)
        {
          int i;
          fprintf (stdout, "HBA memory\n");
          for (i = 0; i < 0x80; i+=4)
          {
            memcpy(&io_doubleword_ccc, hba_virt_addr_ccc + i, 4);
            fprintf (stdout, "%x %08x ", i, io_doubleword_ccc);  //debug
            int n;
            for (n = 0; n < 32; n++)
            {
              if ( !(n % 8) )
              {
                fprintf (stdout, " ");
              }
              if ((io_doubleword_ccc<<n) & 0x80000000)
                fprintf (stdout, "1");
              else
                fprintf (stdout, "0");
            }
            fprintf (stdout, "\n");
          }

          fprintf (stdout, "Port memory\n");
          for (i = 0; i < 0x80; i+=4)
          {
            memcpy(&io_doubleword_ccc, port_virt_addr_ccc + i, 4);
            fprintf (stdout, "%x %08x ", i, io_doubleword_ccc);  //debug
            int n;
            for (n = 0; n < 32; n++)
            {
              if ( !(n % 8) )
              {
                fprintf (stdout, " ");
              }
              if ((io_doubleword_ccc<<n) & 0x80000000)
                fprintf (stdout, "1");
              else
                fprintf (stdout, "0");
            }
            fprintf (stdout, "\n");
          }
        }
        #endif

        // get recent copy of status and error
        memcpy(&io_byte_ccc[1], port_virt_addr_ccc + superbyte_ccc[8], 1);    // status
        memcpy(&io_byte_ccc[2], port_virt_addr_ccc + superbyte_ccc[9], 1);    // error
        set_number_variable_value_ccc("$ata_return_error", io_byte_ccc[2]);
        set_number_variable_value_ccc("$ata_return_status", io_byte_ccc[1]);
        set_number_variable_value_ccc("$ata_alternate_status", 0);
        memcpy(&ata_error_ccc, &io_byte_ccc[2], 1);
        memcpy(&ata_status_ccc, &io_byte_ccc[1], 1);
      }
    }

    else
    {
      // ide
      // set device, this is important to do first to make sure the proper drive is selected
      set_device_ccc(reg_base_address_ccc, device_select_base_ccc);

      io_byte_ccc[1] = inb(reg_base_address_ccc+1);
      io_byte_ccc[7] = inb(reg_base_address_ccc+7);
      io_byte_ccc[8] = inb(control_base_address_ccc);

      set_number_variable_value_ccc("$ata_return_error", io_byte_ccc[1]);
      set_number_variable_value_ccc("$ata_return_status", io_byte_ccc[7]);
      set_number_variable_value_ccc("$ata_alternate_status", io_byte_ccc[8]);
      memcpy(&ata_error_ccc, &io_byte_ccc[1], 1);
      memcpy(&ata_status_ccc, &io_byte_ccc[7], 1);    //status
      //memcpy(&ata_status_ccc, &io_byte_ccc[8], 1);    //altstatus
    }
  }
  else
  {
    // this normally does not work
    // it just gives sense code of invalid field in CDB
    int protocol = passthrough_ccc.protocol;
    passthrough_ccc.protocol = 15;    // return response information

    do_ata28_cmd_ccc(disk_fd);

    passthrough_ccc.protocol = protocol;
  }

  return (0);
}







int find_all_devices_ccc(void)
{
  if (usb_mode_ccc)
  {
    find_all_usb_devices_ccc();
    return 0;
  }
  sprintf (tempmessage_ccc, "Finding devices\n");
  message_now_ccc(tempmessage_ccc);
  // clear the list
  int i;
  for (i = 0; i < MAX_DEVICES; i++)
  {
    reg_address_ccc[i] = 0;
    control_address_ccc[i] = 0;
    bus_address_ccc[i] = 0;
    device_select_ccc[i] = 0;
    device_present_ccc[i] = 0;
    device_visable_ccc[i] = 0;
    hba_address_ccc[i] = 0;
    port_address_ccc[i] = 0;
    port_number_ccc[i] = 0;
    port_status_ccc[i] = 0;
    port_signature_ccc[i] = 0;
    drive_size_ccc[i] = 0;
    device_type_ccc[i] = 0;
    strcpy (device_reference_ccc[i], "");
    strcpy (device_name_ccc[i], "");
    strcpy (device_driver_ccc[i], "");
    strcpy (device_bus_ccc[i], "");
    strcpy (model_ccc[i], "");
    strcpy (serial_ccc[i], "");
    strcpy (drive_list_ccc[i], "");
  }

  device_count_ccc = 0;
  if (direct_mode_ccc)
  {
    // direct
    // list possible drivers
    char command[255];
    if (ahci_mode_ccc)
    {
      strcpy (command, "ls /sys/bus/pci/drivers | grep ahci");
    }
    else
    {
      strcpy (command, "ls /sys/bus/pci/drivers | grep ata_");
    }
    FILE *fp = popen(command, "r");
    int rows = MAX_DRIVERS;
    int cols = 255;
    char list[cols][rows];
    int i;
    char line[cols];
    i = 0;
    while (fgets(line, sizeof line, fp))
    {
      int x;
      for (x = 0; x < cols; x++)
      {
        if (line[x] == '\n')
        {
          line[x] = '\0';
          break;
        }
      }
      strcpy (list[i], line);
      //fprintf (stdout, "%s\n", list[i]);  //debug
      find_bus_devices_ccc(list[i]);
      i++;
      if (i >= MAX_DRIVERS)
      {
        break;
      }
    }
    pclose(fp);

    // get all device info
    for (i = 0; i < device_count_ccc; i++)
    {
      if (ahci_mode_ccc)
      {
        identify_device_ahci_ccc(i);
      }
      else
      {
        identify_device_direct_ccc(reg_address_ccc[i], device_select_ccc[i], i);
      }
    }
  }

  else if (scsi_passthrough_ccc || ata_passthrough_ccc)
  {
    // passthrough
    // list possible drives
    char command[255];
    if (enable_scsi_write_ccc)
    {
      strcpy (command, "ls /dev | grep -w sg.");
    }
    else
    {
      strcpy (command, "ls /dev | grep -w sd.");
    }
    FILE *fp = popen(command, "r");
    int cols = 255;
    int i;
    char line[cols];
    i = 0;
    while (fgets(line, sizeof line, fp))
    {
      int x;
      for (x = 0; x < cols; x++)
      {
        if (line[x] == '\n')
        {
          line[x] = '\0';
          break;
        }
      }

      if ( (strncmp(line, "sd", 2) == 0 || strncmp(line, "sg", 2) == 0) && strlen(line) < 4 )
      {
        strcpy (drive_list_ccc[device_count_ccc], "/dev/");
        strcat (drive_list_ccc[device_count_ccc], line);
        device_count_ccc++;
      }

      i++;
      if (i >= MAX_DEVICES)
      {
        break;
      }
    }
    pclose(fp);

    // get all device info
    for (i = 0; i < device_count_ccc; i++)
    {
      //fprintf (stdout, "%s\n", drive_list_ccc[i]);
      if (ata_passthrough_ccc)
      {
        identify_device_passthrough_ccc(drive_list_ccc[i], i);
      }
      else
      {
        identify_device_scsi_ccc(drive_list_ccc[i], i);
        if (auto_passthrough_ccc && (strncmp(model_ccc[i], "ATA", 3) == 0))
        {
          identify_device_passthrough_ccc(drive_list_ccc[i], i);
        }
      }
    }
  }

  else
  {
    // generic
    // list possible drives
    char command[255];
    strcpy (command, "lsblk -d -n -b -o NAME,SIZE,LOG-SEC,MODEL");
    FILE *fp = popen(command, "r");
    int cols = 255;
    int i;
    char line[cols];
    i = 0;
    while (fgets(line, sizeof line, fp))
    {
      int x;
      for (x = 0; x < cols; x++)
      {
        if (line[x] == '\n')
        {
          line[x] = '\0';
          break;
        }
      }

      char name[cols];
      char model[cols];
      char raw_size[cols];
      char raw_bytes_per_sec[cols];
      strcpy (name, "");
      strcpy (model, "");
      strcpy (raw_size, "");
      strcpy (raw_bytes_per_sec, "");
      sscanf(line, "%s %s %s %[^\n]", name, raw_size, raw_bytes_per_sec, model);
      strcpy (drive_list_ccc[device_count_ccc], "/dev/");
      strcat (drive_list_ccc[device_count_ccc], name);
      strncpy (model_ccc[device_count_ccc], model, sizeof(model_ccc));
      // get device info
      long long size = strtoull(raw_size, NULL, 0);
      long long bytes_per_sec = strtoull(raw_bytes_per_sec, NULL, 0);
      drive_size_ccc[device_count_ccc] = size;
      drive_bytes_per_sector_ccc[device_count_ccc] = bytes_per_sec;
      device_type_ccc[device_count_ccc] = DEVICE_TYPE_BLOCK;

      device_count_ccc++;

      i++;
      if (i >= MAX_DEVICES)
      {
        break;
      }
    }
    pclose(fp);
  }



  return (0);
}





int find_bus_devices_ccc(char *driver)
{
  // list possible bus devices
  char command[255];
  strcpy (command, "ls /sys/bus/pci/drivers/");
  strcat (command, driver);
  strcat (command, " | grep 0000:");
  //fprintf (stdout, "%s\n", command);  //debug
  FILE *fp = popen(command, "r");
  int rows = MAX_BUSES;
  int cols = 255;
  char list[cols][rows];
  int i;
  char line[cols];
  i = 0;
  while (fgets(line, sizeof line, fp))
  {
    int x;
    for (x = 0; x < cols; x++)
    {
      if (line[x] == '\n')
      {
        line[x] = '\0';
        break;
      }
    }
    strcpy (list[i], line);
    //fprintf (stdout, "  %s\n", list[i]);  //debug

    // get info from each device
    get_device_resources_ccc(list[i], driver);
    i++;
    if (i >= MAX_BUSES)
    {
      break;
    }
  }
  pclose(fp);
  return (0);
}






int get_device_resources_ccc(char *bus, char *driver)
{
  char command[255];
  strcpy (command, "cat /sys/bus/pci/devices/");
  strcat (command, bus);
  strcat (command, "/resource");
  //fprintf (stdout, "%s\n", command);  //debug

  FILE *fp = popen(command, "r");
  int rows = MAX_RESOURCES;
  int cols = 255;
  char line[cols];
  unsigned long long resource_start[rows];
  unsigned long long resource_end[rows];
  unsigned long long resource_flag[rows];
  int i = 0;
  while (fgets(line, sizeof line, fp))
  {
    //fprintf (stdout, "  %s", line);  //debug
    char start[cols];
    char end[cols];
    char flag[cols];
    char leftover[cols];
    int scanline = sscanf(line, "%s %s %s %[^\n]", start, end, flag, leftover);
    if (scanline != 3)
    {
      break;
    }
    else
    {
      resource_start[i] = strtoull(start, NULL, 0);
      resource_end[i] = strtoull(end, NULL, 0);
      resource_flag[i] = strtoull(flag, NULL, 0);
      i++;
      if (i >= MAX_RESOURCES)
      {
        break;
      }
    }
  }

  unsigned long long real_start[i];
  unsigned long long real_end[i];
  unsigned long long real_flag[i];
  int real_count = 0;
  int n;
  // new fixed method of memory mapping ports, list all memory ports for processing with this method
  int fixed_method = 1;
  for (n = 0; n < i; n++)
  {
    if (resource_start[n] != 0 || fixed_method)
    {
      //fprintf (stdout, "0x%llx 0x%llx 0x%llx\n", resource_start[n], resource_end[n], resource_flag[n]);  //debug
      real_start[real_count] = resource_start[n];
      real_end[real_count] = resource_end[n];
      real_flag[real_count] = resource_flag[n];
      real_count++;
    }
  }

  process_resources_ccc(real_start, real_end, real_flag, real_count, bus, driver);

  pclose(fp);
  return (0);
}






int process_resources_ccc(unsigned long long *start, unsigned long long *end, unsigned long long *flag, int count, char *bus, char *driver)
{
  if (ahci_mode_ccc)
  {
    if (superbyte_ccc[39] == 0x0e)
    {
      int i;
      if (verbose_ccc & DEBUG8)
      {
        for (i = 0; i < count; i++)
        {
          fprintf (stdout, "0x%llx 0x%llx 0x%llx\n", start[i], end[i], flag[i]);
        }
      }
      unsigned long long potential_hba_start;
      unsigned long long potential_hba_end;
      // new fixed method of memory mapping ports, the 6th in the list should be the hba
      int fixed_method = 1;
      if (fixed_method)
      {
        potential_hba_start = start[5];
        potential_hba_end = end[5];
      }
      else
      {
        int potential_count = 0;
        for (i = 0; i < count; i++)
        {
          if (end[i] - start[i] > 1024 && end[i] - start[i] < 16384)
          {
            potential_hba_start = start[i];
            potential_hba_end = end[i];
            potential_count++;
          }
        }
        if (potential_count == 0)
        {
          //fprintf (stdout, "fail1\n");  //debug
          return (0);
        }
        else if (potential_count > 1)
        {
          potential_count = 0;
          for (i = 0; i < count; i++)
          {
            if (end[i] - start[i] > 2047 && end[i] - start[i] < 8192 && flag[i] == 0x40200)
            {
              potential_hba_start = start[i];
              potential_hba_end = end[i];
              potential_count++;
            }
          }
          if (potential_count != 1)
          {
            //fprintf (stdout, "fail2\n");  //debug
            return (0);
          }
        }
      }
      //fprintf (stdout, "0x%llx 0x%llx\n", potential_hba_start, potential_hba_end);  //debug

      int port_mem_dev_ccc = open("/dev/mem", O_RDWR | O_SYNC);

      if(port_mem_dev_ccc == -1)
      {
        sprintf (tempmessage_ccc, "unable to open /dev/mem\n");
        message_now_ccc(tempmessage_ccc);
        return (0);
      }
      const uint32_t mem_address = potential_hba_start;
      const uint32_t mem_size = potential_hba_end - potential_hba_start +1;

      uint32_t port_alloc_mem_size_ccc, page_mask, page_size;
      void *port_mem_pointer_ccc, *port_virt_addr_ccc;

      page_size = pagesize_ccc;
      port_alloc_mem_size_ccc = (((mem_size / page_size) + 1) * page_size);
      page_mask = (page_size - 1);

      port_mem_pointer_ccc = mmap(NULL,
                                  port_alloc_mem_size_ccc,
                                  PROT_READ | PROT_WRITE,
                                  MAP_SHARED,
                                  port_mem_dev_ccc,
                                  (mem_address & ~page_mask)
      );

      if(port_mem_pointer_ccc == MAP_FAILED)
      {
        sprintf (tempmessage_ccc, "mem map failed\n");
        message_now_ccc(tempmessage_ccc);
        return (0);
      }
      port_virt_addr_ccc = (port_mem_pointer_ccc + (mem_address & page_mask));

      memcpy(&io_doubleword_ccc, port_virt_addr_ccc + superbyte_ccc[19], 4);
      unsigned int ports_implemented = io_doubleword_ccc;
      //fprintf (stdout, "ports_implemented= 0x%x\n", ports_implemented);  //debug

      for (i = 0; i < 32; i++)
      {
        int possible_device = ports_implemented & 1;
        if (possible_device)
        {
          get_device_information_ccc(driver, bus, i, 0, device_count_ccc);
          strcpy (device_driver_ccc[device_count_ccc], driver);
          strcpy (device_bus_ccc[device_count_ccc], bus);
          hba_address_ccc[device_count_ccc] = potential_hba_start;
          port_number_ccc[device_count_ccc] = i;
          port_address_ccc[device_count_ccc] = potential_hba_start + 0x100 + (i*0x80);
          memcpy(&io_doubleword_ccc, port_virt_addr_ccc + 0x128 + (i*0x80), 4);
          port_status_ccc[device_count_ccc] = io_doubleword_ccc;
          if (port_status_ccc == 0)
          {
            device_present_ccc[device_count_ccc] = false;
          }
          else
          {
            device_present_ccc[device_count_ccc] = true;
          }
          //fprintf (stdout, "port_status_ccc= 0x%x\n", port_status_ccc);  //debug
          memcpy(&io_doubleword_ccc, port_virt_addr_ccc + 0x124 + (i*0x80), 4);
          port_signature_ccc[device_count_ccc] = io_doubleword_ccc;
          //fprintf (stdout, "port_signature_ccc= 0x%x\n", port_signature_ccc);  //debug
          //fprintf (stdout, "%s %s %llx %d %llx %x %x\n", driver, bus, potential_hba_start, port_number_ccc[device_count_ccc], port_address_ccc[device_count_ccc], port_status_ccc[device_count_ccc], port_signature_ccc[device_count_ccc]);  //debug
          device_count_ccc++;
        }
        ports_implemented = ports_implemented >> 1;
      }

      munmap(port_mem_pointer_ccc, port_alloc_mem_size_ccc);
      close(port_mem_dev_ccc);
    }
  }

  else
  {
    unsigned long long potential_chan0_reg;
    unsigned long long potential_chan0_con;
    unsigned long long potential_chan0_bus;
    unsigned long long potential_chan1_reg;
    unsigned long long potential_chan1_con;
    unsigned long long potential_chan1_bus;
    int i;
    if (verbose_ccc & DEBUG8)
    {
      for (i = 0; i < count; i++)
      {
        fprintf (stdout, "0x%llx 0x%llx 0x%llx\n", start[i], end[i], flag[i]);
      }
    }
    int success = 1;
    // new fixed method of memory mapping ports
    int fixed_method = 1;
    if (fixed_method)
    {
      potential_chan0_reg = start[0];
      potential_chan0_con = start[1];
      potential_chan1_reg = start[2];
      potential_chan1_con = start[3];
      potential_chan0_bus = start[4];
      potential_chan1_bus = start[4] + 8;
    }
    else
    {
      // we expect a io_byte_ccc size pattern of 8, 1 or 4, 8, 1 or 4, 16
      i = 0;
      if (end[i] - start[i] == 7)
      {
        potential_chan0_reg = start[i];
      }
      else
      {
        //fprintf (stdout, "fail1\n");
        success = 0;
      }

      i = 1;
      if (end[i] - start[i] == 0)
      {
        potential_chan0_con = start[i];
      }
      else if (end[i] - start[i] == 3)
      {
        potential_chan0_con = start[i] + 2;
      }
      else
      {
        //fprintf (stdout, "fail2\n");
        success = 0;
      }

      i = 2;
      if (end[i] - start[i] == 7)
      {
        potential_chan1_reg = start[i];
      }
      else
      {
        //fprintf (stdout, "fail3\n");
        success = 0;
      }

      i = 3;
      if (end[i] - start[i] == 0)
      {
        potential_chan1_con = start[i];
      }
      else if (end[i] - start[i] == 3)
      {
        potential_chan1_con = start[i] + 2;
      }
      else
      {
        //fprintf (stdout, "fail4\n");
        success = 0;
      }

      i = 4;
      if (end[i] - start[i] == 15)
      {
        potential_chan0_bus = start[i];
        potential_chan1_bus = start[i] + 8;
      }
      else
      {
        //fprintf (stdout, "fail5\n");
        success = 0;
      }
    }

    if (success)
    {
      get_device_information_ccc(driver, bus, 0, 0, device_count_ccc);
      strcpy (device_driver_ccc[device_count_ccc], driver);
      strcpy (device_bus_ccc[device_count_ccc], bus);
      reg_address_ccc[device_count_ccc] = potential_chan0_reg;
      control_address_ccc[device_count_ccc] = potential_chan0_con;
      bus_address_ccc[device_count_ccc] = potential_chan0_bus;
      device_select_ccc[device_count_ccc] = 0;
      port_number_ccc[device_count_ccc] = 0;

      device_count_ccc++;
      get_device_information_ccc(driver, bus, 0, 1, device_count_ccc);
      strcpy (device_driver_ccc[device_count_ccc], driver);
      strcpy (device_bus_ccc[device_count_ccc], bus);
      reg_address_ccc[device_count_ccc] = potential_chan0_reg;
      control_address_ccc[device_count_ccc] = potential_chan0_con;
      bus_address_ccc[device_count_ccc] = potential_chan0_bus;
      device_select_ccc[device_count_ccc] = 1;
      port_number_ccc[device_count_ccc] = 1;

      device_count_ccc++;
      get_device_information_ccc(driver, bus, 1, 0, device_count_ccc);
      strcpy (device_driver_ccc[device_count_ccc], driver);
      strcpy (device_bus_ccc[device_count_ccc], bus);
      reg_address_ccc[device_count_ccc] = potential_chan1_reg;
      control_address_ccc[device_count_ccc] = potential_chan1_con;
      bus_address_ccc[device_count_ccc] = potential_chan1_bus;
      device_select_ccc[device_count_ccc] = 0;
      port_number_ccc[device_count_ccc] = 2;

      device_count_ccc++;
      get_device_information_ccc(driver, bus, 1, 1, device_count_ccc);
      strcpy (device_driver_ccc[device_count_ccc], driver);
      strcpy (device_bus_ccc[device_count_ccc], bus);
      reg_address_ccc[device_count_ccc] = potential_chan1_reg;
      control_address_ccc[device_count_ccc] = potential_chan1_con;
      bus_address_ccc[device_count_ccc] = potential_chan1_bus;
      device_select_ccc[device_count_ccc] = 1;
      port_number_ccc[device_count_ccc] = 3;

      device_count_ccc++;
    }
  }

  return (0);
}





int read_capacity_10_ccc(void)
{
  int ret = 0;
  unsigned char tempbuf[512];
  unsigned long long buffersize_backup = ccc_main_buffer_size_ccc;
  memcpy(tempbuf, ccc_buffer_ccc, 512);
  memset(ccc_buffer_ccc, 0, 512);
  memset(read_capacity_buffer_ccc, 0, 512);
  ccc_main_buffer_size_ccc = 8;
  set_main_buffer_ccc();

  passthrough_ccc.scsi_cmd[0] = 0x25; // read capacity 10
  passthrough_ccc.scsi_cmd[1] = 0;
  passthrough_ccc.scsi_cmd[2] = 0;
  passthrough_ccc.scsi_cmd[3] = 0;
  passthrough_ccc.scsi_cmd[4] = 0;
  passthrough_ccc.scsi_cmd[5] = 0;
  passthrough_ccc.scsi_cmd[6] = 0;
  passthrough_ccc.scsi_cmd[7] = 0;
  passthrough_ccc.scsi_cmd[8] = 0;
  passthrough_ccc.scsi_cmd[9] = 0;
  command_length_ccc = 10;
  passthrough_ccc.sg_xfer_direction = SG_DXFER_FROM_DEV;

  unsigned long long soft_reset_time_bak = soft_reset_time_ccc;
  soft_reset_time_ccc = initial_busy_wait_time_ccc;
  ret = do_scsi_cmd_ccc(current_disk_ccc);
  soft_reset_time_ccc = soft_reset_time_bak;

  memcpy(read_capacity_buffer_ccc, ccc_buffer_ccc, 512);
  ccc_main_buffer_size_ccc = buffersize_backup;
  set_main_buffer_ccc();
  memcpy(ccc_buffer_ccc, tempbuf, 512);

  if (0)
  {
    int i;
    for (i = 0; i < 512; i+=16)
    {
      fprintf (stdout, "%x: ", i);
      unsigned char *c;
      int n;
      for (n=0; n < 16 && i+n < 512; n++)
      {
        c = (unsigned char *)read_capacity_buffer_ccc+i+n;
        fprintf (stdout, "%02x ", *c);
      }
      fprintf (stdout, "   ");
      for (n=0; n < 16 && i+n < 512; n++)
      {
        c = (unsigned char *)read_capacity_buffer_ccc+i+n;
        fprintf (stdout, "%c", isprint(*c) ? *c : '.');
      }
      fprintf (stdout, "\n");
    }
  }
  //fprintf(stdout, "identify return= %d\n", ret);    //debug

  return ret;
}





int read_capacity_16_ccc(void)
{
  int ret = 0;
  unsigned char tempbuf[512];
  unsigned long long buffersize_backup = ccc_main_buffer_size_ccc;
  memcpy(tempbuf, ccc_buffer_ccc, 512);
  memset(ccc_buffer_ccc, 0, 512);
  memset(read_capacity_buffer_ccc, 0, 512);
  ccc_main_buffer_size_ccc = 32;
  set_main_buffer_ccc();

  passthrough_ccc.scsi_cmd[0] = 0x9e; // read capacity 16
  passthrough_ccc.scsi_cmd[1] = 0x10;  // service action
  passthrough_ccc.scsi_cmd[2] = 0;
  passthrough_ccc.scsi_cmd[3] = 0;
  passthrough_ccc.scsi_cmd[4] = 0;
  passthrough_ccc.scsi_cmd[5] = 0;
  passthrough_ccc.scsi_cmd[6] = 0;
  passthrough_ccc.scsi_cmd[7] = 0;
  passthrough_ccc.scsi_cmd[8] = 0;
  passthrough_ccc.scsi_cmd[9] = 0;
  passthrough_ccc.scsi_cmd[10] = 0;
  passthrough_ccc.scsi_cmd[11] = 0;
  passthrough_ccc.scsi_cmd[12] = 0;
  passthrough_ccc.scsi_cmd[13] = 32;  // lenth of return data
  passthrough_ccc.scsi_cmd[14] = 0;
  passthrough_ccc.scsi_cmd[15] = 0;
  command_length_ccc = 16;
  passthrough_ccc.sg_xfer_direction = SG_DXFER_FROM_DEV;

  usb_transfer_direction_ccc = USB_DIRECTION_IN;

  unsigned long long soft_reset_time_bak = soft_reset_time_ccc;
  soft_reset_time_ccc = initial_busy_wait_time_ccc;
  ret = do_scsi_cmd_ccc(current_disk_ccc);
  soft_reset_time_ccc = soft_reset_time_bak;

  memcpy(read_capacity_buffer_ccc, ccc_buffer_ccc, 512);
  ccc_main_buffer_size_ccc = buffersize_backup;
  set_main_buffer_ccc();
  memcpy(ccc_buffer_ccc, tempbuf, 512);

  if (0)
  {
    int i;
    for (i = 0; i < 512; i+=16)
    {
      fprintf (stdout, "%x: ", i);
      unsigned char *c;
      int n;
      for (n=0; n < 16 && i+n < 512; n++)
      {
        c = (unsigned char *)read_capacity_buffer_ccc+i+n;
        fprintf (stdout, "%02x ", *c);
      }
      fprintf (stdout, "   ");
      for (n=0; n < 16 && i+n < 512; n++)
      {
        c = (unsigned char *)read_capacity_buffer_ccc+i+n;
        fprintf (stdout, "%c", isprint(*c) ? *c : '.');
      }
      fprintf (stdout, "\n");
    }
  }
  //fprintf(stdout, "identify return= %d\n", ret);    //debug

  return ret;
}





int set_dma_mode_ccc(unsigned char mode)
{
  int ret = 0;
  unsigned char reg[7];
  reg[0] = 0x03;    //set transfer mode
  reg[1] = mode;    // mode
  reg[2] = 0;
  reg[3] = 0;
  reg[4] = 0;
  reg[5] = 0;
  reg[6] = 0xef;    // set features
  unsigned long long buffersize_backup = ccc_main_buffer_size_ccc;
  ccc_main_buffer_size_ccc = 0;
  set_main_buffer_ccc();

  passthrough_ccc.transfer_length = 2;    // sector field
  passthrough_ccc.byte_block = 1;    // block
  passthrough_ccc.protocol = 3;    // non-data
  passthrough_ccc.check_condition = 1;    // check condition true
  passthrough_ccc.direction = 0;    // none
  passthrough_ccc.transfer_direction = 1;    // from
  passthrough_ccc.sg_xfer_direction = SG_DXFER_NONE;

  int cmd_length = 7;
  int i;
  for (i = 0; i < cmd_length; i++)
  {
    passthrough_ccc.ata_cmd[i] = reg[i];
  }

  command_length_ccc = 12;
  ret = do_ata28_cmd_ccc(current_disk_ccc);

  ccc_main_buffer_size_ccc = buffersize_backup;
  set_main_buffer_ccc();
  return ret;
}






int identify_device_ccc(void)
{
  if (generic_mode_ccc)
  {
    // just return 0 since nothing is done here for generic
    memset(identify_buffer_ccc, 0, IDENTIFY_BUFFER_SIZE);
    ata_status_ccc = 0;
    ata_error_ccc = 0;
    return 0;
  }
  if (superclone_ccc)
  {
    if (performing_reset_ccc)
    {
      //fprintf (stdout, "identify device no reset\n");    //debug
    }
    else
    {
      //fprintf (stdout, "identify device with reset\n");    //debug
    }
  }
  int ret = 0;
  unsigned char tempbuf[IDENTIFY_BUFFER_SIZE];
  unsigned long long buffersize_backup = ccc_main_buffer_size_ccc;
  memcpy(tempbuf, ccc_buffer_ccc, IDENTIFY_BUFFER_SIZE);
  memset(ccc_buffer_ccc, 0, IDENTIFY_BUFFER_SIZE);
  memset(identify_buffer_ccc, 0, IDENTIFY_BUFFER_SIZE);
  ccc_main_buffer_size_ccc = IDENTIFY_BUFFER_SIZE;
  set_main_buffer_ccc();
  unsigned long long initial_busy_wait_time_backup = initial_busy_wait_time_ccc;
  initial_busy_wait_time_ccc = busy_wait_time_ccc;

  ata_check_ccc = 0;
  int inquiresize = 44;
  if (usb_mode_ccc)
  {
    inquiresize = 36;
  }
  if ((scsi_passthrough_ccc && !ata_passthrough_ccc) || usb_mode_ccc)
  {
    ccc_main_buffer_size_ccc = inquiresize;
    set_main_buffer_ccc();
    passthrough_ccc.scsi_cmd[0] = 0x12; // inquiry command
    passthrough_ccc.scsi_cmd[1] = 0;
    passthrough_ccc.scsi_cmd[2] = 0;
    passthrough_ccc.scsi_cmd[3] = 0;
    passthrough_ccc.scsi_cmd[4] = inquiresize; // size of data requested back
    passthrough_ccc.scsi_cmd[5] = 0;
    command_length_ccc = 6;
    passthrough_ccc.sg_xfer_direction = SG_DXFER_FROM_DEV;

    usb_transfer_direction_ccc = USB_DIRECTION_IN;

    unsigned long long soft_reset_time_bak = soft_reset_time_ccc;
    soft_reset_time_ccc = initial_busy_wait_time_ccc;
    ret = do_scsi_cmd_ccc(current_disk_ccc);
    soft_reset_time_ccc = soft_reset_time_bak;

    int i;
    char temp_model[10] = "";
    for (i = 0; i < 8; i ++)
    {
      unsigned char *c = (unsigned char *)ccc_buffer_ccc+8+i;
      if ( isprint(*c) )
      {
        temp_model[i] = *c;
      }
      else
      {
        break;
      }
    }
    if (auto_passthrough_ccc && (strncmp(temp_model, "ATA", 3) == 0))
    {
      ata_check_ccc = 1;
      ccc_main_buffer_size_ccc = IDENTIFY_BUFFER_SIZE;
      memset(ccc_buffer_ccc, 0, IDENTIFY_BUFFER_SIZE);
      //fprintf (stdout, "device is ata\n");    //debug
    }
  }

  if (ata_passthrough_ccc || direct_mode_ccc || ata_check_ccc)
  {
    unsigned char reg[7] = {0, 0, 0, 0, 0, 0xa0, 0xec};
    if (enable_data_dump_ccc)
    {
      // set the count to 1
      reg[1] = 1;
    }

    passthrough_ccc.transfer_length = 2;    // sector field
    passthrough_ccc.byte_block = 1;    // block
    passthrough_ccc.protocol = 4;    // pio-data-in
    passthrough_ccc.check_condition = 1;    // check condition true
    passthrough_ccc.direction = 2;    // from
    passthrough_ccc.transfer_direction = 1;    // from
    passthrough_ccc.sg_xfer_direction = SG_DXFER_FROM_DEV;

    int cmd_length = 7;
    int i;
    for (i = 0; i < cmd_length; i++)
    {
      passthrough_ccc.ata_cmd[i] = reg[i];
    }

    command_length_ccc = 12;
    ret = do_ata28_cmd_ccc(current_disk_ccc);
  }

  memcpy(identify_buffer_ccc, ccc_buffer_ccc, IDENTIFY_BUFFER_SIZE);
  ccc_main_buffer_size_ccc = buffersize_backup;
  set_main_buffer_ccc();
  memcpy(ccc_buffer_ccc, tempbuf, IDENTIFY_BUFFER_SIZE);
  initial_busy_wait_time_ccc = initial_busy_wait_time_backup;

  if (0)
  {
    int i;
    for (i = 0; i < 512; i+=16)
    {
      fprintf (stdout, "%x: ", i);
      unsigned char *c;
      int n;
      for (n=0; n < 16 && i+n < 512; n++)
      {
        c = (unsigned char *)identify_buffer_ccc+i+n;
        fprintf (stdout, "%02x ", *c);
      }
      fprintf (stdout, "   ");
      for (n=0; n < 16 && i+n < 512; n++)
      {
        c = (unsigned char *)identify_buffer_ccc+i+n;
        fprintf (stdout, "%c", isprint(*c) ? *c : '.');
      }
      fprintf (stdout, "\n");
    }
  }
  //fprintf(stdout, "identify return= %d\n", ret);    //debug

  return ret;
}






int identify_device_direct_ccc(unsigned long long address, int select, int count)
{
  memset (ccc_buffer_ccc, 0, ccc_main_buffer_size_ccc);
  ioperm (address, 8, 1);
  int i;
  //fprintf (stdout, "identify\n");
  int busy;
  int drdy;
  int drq;
  device_present_ccc[count] = true;

  // set device, this is important to do first to make sure the proper drive is selected
  set_device_ccc(address, select);
  // wait some extra time switching devices just in case
  do_nanosleep_ccc(10000);

  io_byte_ccc[7] = inb(address+7);
  //fprintf (stdout, "%02x\n", io_byte_ccc[7]);
  busy = io_byte_ccc[7] & 0x80;
  drdy = io_byte_ccc[7] & 0x40;
  drq = io_byte_ccc[7] & 0x08;
  if (busy || drq)
  {
    io_byte_ccc[0] = inb(address);
    //fprintf (stdout, "busy or drq\n");
    strcpy (model_ccc[count], "busy or drq");
    device_present_ccc[count] = false;
    char str[32];
    sprintf(str, "s=%02x e=%02x", io_byte_ccc[7], io_byte_ccc[0]);
    strcpy (serial_ccc[count], str);
    ioperm (address, 8, 0);
    return (1);
  }
  else if (enable_dont_identify_on_choose_source_ccc)
  {
    strcpy (model_ccc[count], "error/status");
    if (drdy)
    {
      strcpy (model_ccc[count], "ready");
    }
    char str[32];
    sprintf(str, "s=%02x e=%02x", io_byte_ccc[7], io_byte_ccc[0]);
    strcpy (serial_ccc[count], str);
    ioperm (address, 8, 0);
    return (1);
  }

  io_byte_ccc[1] = 0x00;
  io_byte_ccc[2] = 0x00;
  io_byte_ccc[3] = 0x00;
  io_byte_ccc[4] = 0x00;
  io_byte_ccc[5] = 0x00;
  io_byte_ccc[6] = 0xa0 | (select << 4);
  io_byte_ccc[7] = 0xec;
  if (enable_data_dump_ccc)
  {
    // set the count to 1
    io_byte_ccc[2] = 0x01;
  }

  for (i = 1; i < 8; i++)
  {
    outb(io_byte_ccc[i], address+i);
  }

  io_byte_ccc[7] = inb(address+7);
  //fprintf (stdout, "%02x\n", io_byte_ccc[7]);
  drq = io_byte_ccc[7] & 0x08;
  busy = io_byte_ccc[7] & 0x80;

  unsigned long long start_time = get_elapsed_usec_ccc();
  while (!drq && busy)
  {
    unsigned long long elapsed_time = get_elapsed_usec_ccc();
    if (elapsed_time > start_time + initial_busy_wait_time_ccc)
    {
      break;
    }
    io_byte_ccc[7] = inb(address+7);
    //fprintf (stdout, "%02x\n", io_byte_ccc[7]);
    drq = io_byte_ccc[7] & 0x08;
    busy = io_byte_ccc[7] & 0x80;
    // give the cpu a chance to do something else so we are not using 100%
    do_nanosleep_ccc(1);

  }

  if (drq)
  {
    #if defined(__i386__) || defined(__x86_64__)
      insw(address, ccc_buffer_ccc, IDENTIFY_BUFFER_SIZE);
    #else
      fprintf(stderr,"This insw needs to be ported to other architectures, perhaps with memcpy\n");
    #endif
  }
  else
  {
    io_byte_ccc[4] = inb(address+4);
    io_byte_ccc[5] = inb(address+5);
    //fprintf (stdout, "no drq\n");
    char str[32];
    sprintf(str, "no drq s=%02x lm=%02x lh=%02x", io_byte_ccc[7], io_byte_ccc[4], io_byte_ccc[5]);
    strcpy (model_ccc[count], str);
    if (io_byte_ccc[4] == 0x0 && io_byte_ccc[5] == 0x0)
    {
      strcpy (serial_ccc[count], "no device");
      device_present_ccc[count] = false;
    }
    else if (io_byte_ccc[4] == 0x14 && io_byte_ccc[5] == 0xeb)
    {
      strcpy (serial_ccc[count], "patapi device");
    }
    else if (io_byte_ccc[4] == 0x69 && io_byte_ccc[5] == 0x96)
    {
      strcpy (serial_ccc[count], "satapi device");
    }
    else
    {
      strcpy (serial_ccc[count], "unknown device");
    }
    ioperm (address, 8, 0);
    return (2);
  }

  if (verbose_ccc & DEBUG9)
  {
    for (i = 0; i < 512; i+=16)
    {
      fprintf (stdout, "%x: ", i);
      unsigned char *c;
      int n;
      for (n=0; n < 16 && i+n < 512; n++)
      {
        c = (unsigned char *)ccc_buffer_ccc+i+n;
        fprintf (stdout, "%02x ", *c);
      }
      fprintf (stdout, "   ");
      for (n=0; n < 16 && i+n < 512; n++)
      {
        c = (unsigned char *)ccc_buffer_ccc+i+n;
        fprintf (stdout, "%c", isprint(*c) ? *c : '.');
      }
      fprintf (stdout, "\n");
    }
  }

  char temp_model[41] = "";
  char temp_serial[21] = "";
  for (i = 0; i < 40; i += 2)
  {
    unsigned char *c = (unsigned char *)ccc_buffer_ccc+54+i+1;
    unsigned char *d = (unsigned char *)ccc_buffer_ccc+54+i;
    if ( isprint(*c) )
    {
      temp_model[i] = *c;
    }
    else
    {
      temp_model[i] = '\0';
    }
    if ( isprint(*d) )
    {
      temp_model[i+1] = *d;
    }
    else
    {
      temp_model[i+1] = '\0';
    }
  }
  temp_model[40] = '\0';
  for (i = 0; i < 20; i += 2)
  {
    unsigned char *c = (unsigned char *)ccc_buffer_ccc+20+i+1;
    unsigned char *d = (unsigned char *)ccc_buffer_ccc+20+i;
    if ( isprint(*c) )
    {
      temp_serial[i] = *c;
    }
    else
    {
      temp_serial[i] = '\0';
    }
    if ( isprint(*d) )
    {
      temp_serial[i+1] = *d;
    }
    else
    {
      temp_serial[i+1] = '\0';
    }
  }
  temp_serial[20] = '\0';

  char *found_model = trim_white_space_ccc(temp_model);
  char *found_serial = trim_white_space_ccc(temp_serial);

  strcpy (model_ccc[count], found_model);
  strcpy (serial_ccc[count], found_serial);

  uint16_t word;
  uint32_t double_word;
  uint64_t quad_word;
  words_per_logical_sector_ccc = 256;
  bytes_per_logical_sector_ccc = 512;
  memcpy(&word, ccc_buffer_ccc+212, 2);
  //check if word data is valid
  if ((word & 0xc000) == 0x4000)
  {
    // check if sector size greater than 256 words
    if (word & 0x1000)
    {
      memcpy(&double_word, ccc_buffer_ccc+234, 4);
      words_per_logical_sector_ccc = double_word;
    }
  }
  bytes_per_logical_sector_ccc = words_per_logical_sector_ccc * 2;
  memcpy(&double_word, ccc_buffer_ccc+120, 4);
  memcpy(&quad_word, ccc_buffer_ccc+200, 8);
  unsigned char c;
  // pick based on if extend 48 bit supported
  memcpy(&c, ccc_buffer_ccc+167, 1);
  if (c & 4)
  {
    drive_size_ccc[count] = (quad_word * bytes_per_logical_sector_ccc);
  }
  else
  {
    drive_size_ccc[count] = (double_word * bytes_per_logical_sector_ccc);
  }

  // check if lba is supported and if not get data from chs
  memcpy(&c, ccc_buffer_ccc+99, 1);
  if ( !(c & 2) )
  {
    bytes_per_logical_sector_ccc = 512;    // CHS should always be 512 bytes per sector
    uint16_t word;
    memcpy(&word, ccc_buffer_ccc+106, 2);
    if (word & 1)
    {
      memcpy(&word, ccc_buffer_ccc+114, 2);
      drive_size_ccc[count] = word << 16;
      memcpy(&word, ccc_buffer_ccc+116, 2);
      drive_size_ccc[count] = drive_size_ccc[count] + word;
      drive_size_ccc[count] = drive_size_ccc[count] * bytes_per_logical_sector_ccc;
    }
    else
    {
      memcpy(&word, ccc_buffer_ccc+2, 2);
      drive_size_ccc[count] = word;
      memcpy(&word, ccc_buffer_ccc+6, 2);
      drive_size_ccc[count] = drive_size_ccc[count] * word;
      memcpy(&word, ccc_buffer_ccc+12, 2);
      drive_size_ccc[count] = drive_size_ccc[count] * word;
      drive_size_ccc[count] = drive_size_ccc[count] * bytes_per_logical_sector_ccc;
    }
  }
  drive_bytes_per_sector_ccc[count] = bytes_per_logical_sector_ccc;

  device_type_ccc[count] = DEVICE_TYPE_IDE;

  memset (ccc_buffer_ccc, 0, ccc_main_buffer_size_ccc);
  ioperm (address, 8, 0);
  return (0);
}






int identify_device_ahci_ccc(int count)
{
  if (superbyte_ccc[40] == 0x05)
  {
    memset (ccc_buffer_ccc, 0, ccc_main_buffer_size_ccc);

    strcpy (model_ccc[count], "");
    strcpy (serial_ccc[count], "");

    if (enable_data_dump_ccc)    // TODO this is for hba debug testing
    {
      char temp_string[256];
      sprintf (temp_string, "dump before identify for port number %d\n", port_number_ccc[count]);
      dump_info_to_filename_ccc (data_dump_filename_ccc, temp_string);
      int ret = dump_hba_port_fis_command_data_ccc(hba_address_ccc[count], port_address_ccc[count], 1, 1, 1, 1, 1);
      if (ret)
      {
        strcpy (model_ccc[count], "error with data dump");
        return (1);
      }
    }

    if (port_signature_ccc[count] == 0xffffffff)
    {
      strcpy (model_ccc[count], "no device");
      return (1);
    }

    else if (port_signature_ccc[count] == SATA_SIG_ATA)
    {
      int busy;
      int drq;
      int drdy;
      strcpy (model_ccc[count], "sata device");
      int port_mem_dev_ccc = open("/dev/mem", O_RDWR | O_SYNC);
      if(port_mem_dev_ccc == -1)
      {
        sprintf (tempmessage_ccc, "unable to open /dev/mem\n");
        message_now_ccc(tempmessage_ccc);
        return (-1);
      }
      const uint32_t mem_address = port_address_ccc[count];
      const uint32_t mem_size = 128;

      uint32_t port_alloc_mem_size_ccc, page_mask, page_size;
      void *port_mem_pointer_ccc;

      page_size = pagesize_ccc;
      port_alloc_mem_size_ccc = (((mem_size / page_size) + 1) * page_size);
      page_mask = (page_size - 1);

      port_mem_pointer_ccc = mmap(NULL,
                                  port_alloc_mem_size_ccc,
                                  PROT_READ | PROT_WRITE,
                                  MAP_SHARED,
                                  port_mem_dev_ccc,
                                  (mem_address & ~page_mask)
      );

      if(port_mem_pointer_ccc == MAP_FAILED)
      {
        sprintf (tempmessage_ccc, "mem map failed\n");
        message_now_ccc(tempmessage_ccc);
        return (0);
      }
      port_virt_addr_ccc = (port_mem_pointer_ccc + (mem_address & page_mask));

      if (enable_data_dump_ccc)    // TODO this is for hba debug testing
      {
        char temp_string[256];
        sprintf (temp_string, "dump 1 for port number %d\n", port_number_ccc[count]);
        dump_info_to_filename_ccc (data_dump_filename_ccc, temp_string);
        unsigned char temp_buffer[0x80];
        memcpy(temp_buffer, port_virt_addr_ccc, 0x80);
        dump_data_to_filename_binary_ccc(data_dump_filename_ccc, temp_buffer, 0x80, "Port data");
      }

      // check for busy or drq
      memcpy(&io_doubleword_ccc, port_virt_addr_ccc + superbyte_ccc[8], 4);
      busy = io_doubleword_ccc & 0x80;
      drdy = io_doubleword_ccc & 0x40;
      drq = io_doubleword_ccc & 0x08;
      if (busy || drq)
      {
        strcpy (model_ccc[count], "busy or drq");
        device_present_ccc[count] = false;
        char str[32];
        sprintf(str, "e/s=%04x", io_doubleword_ccc);
        strcpy (serial_ccc[count], str);
        // close mapped memory
        munmap(port_mem_pointer_ccc, port_alloc_mem_size_ccc);
        close(port_mem_dev_ccc);
        return (1);
      }
      else if (enable_dont_identify_on_choose_source_ccc)
      {
        strcpy (model_ccc[count], "error/status");
        if (drdy)
        {
          strcpy (model_ccc[count], "ready");
        }
        char str[32];
        sprintf(str, "e/s=%04x", io_doubleword_ccc);
        strcpy (serial_ccc[count], str);
        // close mapped memory
        munmap(port_mem_pointer_ccc, port_alloc_mem_size_ccc);
        close(port_mem_dev_ccc);
        return (1);
      }

      #ifdef DEBUG
      int i;
      for (i = 0; i < 0x80; i+=4)
      {
        memcpy(&io_doubleword_ccc, port_virt_addr_ccc + i, 4);
        //fprintf (stdout, "%x %08x\n", i, io_doubleword_ccc);  //debug
      }
      #endif

      // backup the interrupt settings
      memcpy(&interrupt_backup_ccc, port_virt_addr_ccc + superbyte_ccc[13], 4);    //potential superbyte
      ahci_interrupt_changed_ccc = true;

      // backup current addresses
      memcpy(&command_list_address_backup_ccc, port_virt_addr_ccc + superbyte_ccc[11], 8);    //potential superbyte
      memcpy(&fis_address_backup_ccc, port_virt_addr_ccc + superbyte_ccc[12], 8);    //potential superbyte
      ahci_address_changed_ccc = true;
      //fprintf (stdout, "command_list_address_backup_ccc= %016llx\n", command_list_address_backup_ccc);  //debug
      //fprintf (stdout, "fis_address_backup_ccc= %016llx\n", fis_address_backup_ccc);  //debug

      // clear the fis buffer
      memset(fis_buffer_ccc, 0, fis_size_ccc);
      // clear the command FIS
      memset(table_buffer_ccc, 0, 0x80);
      // set FIS type
      memset(table_buffer_ccc, REG_H2D, 1);
      // set type as command
      memset(table_buffer_ccc+1, 0x80, 1);
      // set the command register for identify device
      memset(table_buffer_ccc+2, 0xec, 1);
      if (enable_data_dump_ccc)
      {
        // set the count to 1
        memset(table_buffer_ccc+12, 1, 1);
        // set the control register value (something unknown)
        memset(table_buffer_ccc+15, 0x08, 1);
      }
      // clear the command list
      memset(command_list_buffer_ccc, 0, command_list_size_ccc);
      // set the command FIS length, 5 dwords for h2d
      memset(command_list_buffer_ccc, 5, 1);
      // set the table entry length
      uint16_t word = table_entry_count_ccc;
      memcpy(command_list_buffer_ccc+2, &word, 2);
      // set the command table address
      uint64_t qword = table_physical_address_ccc;
      memcpy(command_list_buffer_ccc+8, &qword, 8);

      // turn interupts off
      memset(port_virt_addr_ccc + superbyte_ccc[13], 0x0, 4);    //potential superbyte

      // wait for 1ms
      do_nanosleep_ccc(1000000);

      if (enable_data_dump_ccc)    // TODO this is for hba debug testing
      {
        char temp_string[256];
        sprintf (temp_string, "dump 2 for port number %d\n", port_number_ccc[count]);
        dump_info_to_filename_ccc (data_dump_filename_ccc, temp_string);
        unsigned char temp_buffer[0x80];
        memcpy(temp_buffer, port_virt_addr_ccc, 0x80);
        dump_data_to_filename_binary_ccc(data_dump_filename_ccc, temp_buffer, 0x80, "Port data");
      }

      // backup the start bit
      uint8_t byte;
      memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[10], 1);    //potential superbyte
      start_bit_ccc = byte & 1;

      // clear the start bit
      disable_start_ccc(START_BIT_TIME);

      // wait for 1ms
      do_nanosleep_ccc(1000000);

      if (enable_data_dump_ccc)    // TODO this is for hba debug testing
      {
        char temp_string[256];
        sprintf (temp_string, "dump 3 for port number %d\n", port_number_ccc[count]);
        dump_info_to_filename_ccc (data_dump_filename_ccc, temp_string);
        unsigned char temp_buffer[0x80];
        memcpy(temp_buffer, port_virt_addr_ccc, 0x80);
        dump_data_to_filename_binary_ccc(data_dump_filename_ccc, temp_buffer, 0x80, "Port data");
      }

      // clear the fis enable bit
      disable_fis_ccc(FIS_BIT_TIME);

      // wait for 1ms
      do_nanosleep_ccc(1000000);

      if (enable_data_dump_ccc)    // TODO this is for hba debug testing
      {
        char temp_string[256];
        sprintf (temp_string, "dump 4 for port number %d\n", port_number_ccc[count]);
        dump_info_to_filename_ccc (data_dump_filename_ccc, temp_string);
        unsigned char temp_buffer[0x80];
        memcpy(temp_buffer, port_virt_addr_ccc, 0x80);
        dump_data_to_filename_binary_ccc(data_dump_filename_ccc, temp_buffer, 0x80, "Port data");
      }

      // set new addresses
      uint64_t command_list_address = command_list_physical_address_ccc;
      memcpy(port_virt_addr_ccc + superbyte_ccc[11], &command_list_address, 8);    //potential superbyte
      uint64_t fis_address = fis_physical_address_ccc;
      memcpy(port_virt_addr_ccc + superbyte_ccc[12], &fis_address, 8);    //potential superbyte

      // wait for 1ms
      do_nanosleep_ccc(1000000);

      if (enable_data_dump_ccc)    // TODO this is for hba debug testing
      {
        char temp_string[256];
        sprintf (temp_string, "dump 5 for port number %d\n", port_number_ccc[count]);
        dump_info_to_filename_ccc (data_dump_filename_ccc, temp_string);
        unsigned char temp_buffer[0x80];
        memcpy(temp_buffer, port_virt_addr_ccc, 0x80);
        dump_data_to_filename_binary_ccc(data_dump_filename_ccc, temp_buffer, 0x80, "Port data");
      }

      // set the fis enable bit
      enable_fis_ccc(FIS_BIT_TIME);

      // wait for 1ms
      do_nanosleep_ccc(1000000);

      if (enable_data_dump_ccc)    // TODO this is for hba debug testing
      {
        char temp_string[256];
        sprintf (temp_string, "dump 6 for port number %d\n", port_number_ccc[count]);
        dump_info_to_filename_ccc (data_dump_filename_ccc, temp_string);
        unsigned char temp_buffer[0x80];
        memcpy(temp_buffer, port_virt_addr_ccc, 0x80);
        dump_data_to_filename_binary_ccc(data_dump_filename_ccc, temp_buffer, 0x80, "Port data");
      }

      // set the start bit
      enable_start_ccc(START_BIT_TIME);

      // wait for 1ms
      do_nanosleep_ccc(1000000);

      if (enable_data_dump_ccc)    // TODO this is for hba debug testing
      {
        char temp_string[256];
        sprintf (temp_string, "dump 7 for port number %d\n", port_number_ccc[count]);
        dump_info_to_filename_ccc (data_dump_filename_ccc, temp_string);
        unsigned char temp_buffer[0x80];
        memcpy(temp_buffer, port_virt_addr_ccc, 0x80);
        dump_data_to_filename_binary_ccc(data_dump_filename_ccc, temp_buffer, 0x80, "Port data");
      }

      // set the command issue bit
      enable_command_issue_ccc(COMMAND_BIT_TIME);

      if (enable_data_dump_ccc)    // TODO this is for hba debug testing
      {
        char temp_string[256];
        sprintf (temp_string, "dump 8 for port number %d\n", port_number_ccc[count]);
        dump_info_to_filename_ccc (data_dump_filename_ccc, temp_string);
        unsigned char temp_buffer[0x80];
        memcpy(temp_buffer, port_virt_addr_ccc, 0x80);
        dump_data_to_filename_binary_ccc(data_dump_filename_ccc, temp_buffer, 0x80, "Port data");
      }

      // wait for it to complete
      busy = 1;
      drq = 1;
      unsigned long long start_time = get_elapsed_usec_ccc();
      bool timeout = false;
      while (busy || drq)
      {
        unsigned long long elapsed_time = get_elapsed_usec_ccc();
        if (elapsed_time > start_time + initial_busy_wait_time_ccc)
        {
          timeout = true;
          break;
        }
        memcpy(&io_doubleword_ccc, port_virt_addr_ccc + superbyte_ccc[8], 4);    //potential superbyte
        busy = io_doubleword_ccc & 0x80;
        drq = io_doubleword_ccc & 0x08;
        //fprintf (stdout, "e/s=%x\n", io_doubleword_ccc);  //debug
        // give the cpu a chance to do something else so we are not using 100%
        do_nanosleep_ccc(1);
      }

      if (enable_data_dump_ccc)
      {
        char temp_string[256];
        sprintf (temp_string, "dump during identify for port number %d\n", port_number_ccc[count]);
        dump_info_to_filename_ccc (data_dump_filename_ccc, temp_string);
        dump_hba_port_fis_command_data_ccc(hba_address_ccc[count], port_address_ccc[count], 1, 1, 1, 1, 1);
      }

      // clear the start bit
      disable_start_ccc(START_BIT_TIME);

      // wait for 1ms
      do_nanosleep_ccc(1000000);

      if (enable_data_dump_ccc)    // TODO this is for hba debug testing
      {
        char temp_string[256];
        sprintf (temp_string, "dump 9 for port number %d\n", port_number_ccc[count]);
        dump_info_to_filename_ccc (data_dump_filename_ccc, temp_string);
        unsigned char temp_buffer[0x80];
        memcpy(temp_buffer, port_virt_addr_ccc, 0x80);
        dump_data_to_filename_binary_ccc(data_dump_filename_ccc, temp_buffer, 0x80, "Port data");
      }

      // clear the fis enable bit
      disable_fis_ccc(FIS_BIT_TIME);

      // wait for 1ms
      do_nanosleep_ccc(1000000);

      if (enable_data_dump_ccc)    // TODO this is for hba debug testing
      {
        char temp_string[256];
        sprintf (temp_string, "dump 10 for port number %d\n", port_number_ccc[count]);
        dump_info_to_filename_ccc (data_dump_filename_ccc, temp_string);
        unsigned char temp_buffer[0x80];
        memcpy(temp_buffer, port_virt_addr_ccc, 0x80);
        dump_data_to_filename_binary_ccc(data_dump_filename_ccc, temp_buffer, 0x80, "Port data");
      }

      // restore addresses
      memcpy(port_virt_addr_ccc + superbyte_ccc[11], &command_list_address_backup_ccc, 8);    //potential superbyte
      memcpy(port_virt_addr_ccc + superbyte_ccc[12], &fis_address_backup_ccc, 8);    //potential superbyte
      ahci_address_changed_ccc = false;

      // wait for 1ms
      do_nanosleep_ccc(1000000);

      if (enable_data_dump_ccc)    // TODO this is for hba debug testing
      {
        char temp_string[256];
        sprintf (temp_string, "dump 11 for port number %d\n", port_number_ccc[count]);
        dump_info_to_filename_ccc (data_dump_filename_ccc, temp_string);
        unsigned char temp_buffer[0x80];
        memcpy(temp_buffer, port_virt_addr_ccc, 0x80);
        dump_data_to_filename_binary_ccc(data_dump_filename_ccc, temp_buffer, 0x80, "Port data");
      }

      // set the fis enable bit
      enable_fis_ccc(FIS_BIT_TIME);

      // wait for 1ms
      do_nanosleep_ccc(1000000);

      if (enable_data_dump_ccc)    // TODO this is for hba debug testing
      {
        char temp_string[256];
        sprintf (temp_string, "dump 12 for port number %d\n", port_number_ccc[count]);
        dump_info_to_filename_ccc (data_dump_filename_ccc, temp_string);
        unsigned char temp_buffer[0x80];
        memcpy(temp_buffer, port_virt_addr_ccc, 0x80);
        dump_data_to_filename_binary_ccc(data_dump_filename_ccc, temp_buffer, 0x80, "Port data");
      }

      // restore the start bit
      if (start_bit_ccc)
      {
        enable_start_ccc(START_BIT_TIME);
      }

      // wait for 1ms
      do_nanosleep_ccc(1000000);

      if (enable_data_dump_ccc)    // TODO this is for hba debug testing
      {
        char temp_string[256];
        sprintf (temp_string, "dump 13 for port number %d\n", port_number_ccc[count]);
        dump_info_to_filename_ccc (data_dump_filename_ccc, temp_string);
        unsigned char temp_buffer[0x80];
        memcpy(temp_buffer, port_virt_addr_ccc, 0x80);
        dump_data_to_filename_binary_ccc(data_dump_filename_ccc, temp_buffer, 0x80, "Port data");
      }

      // restore the interrupt settings
      memcpy(port_virt_addr_ccc + superbyte_ccc[13], &interrupt_backup_ccc, 4);    //potential superbyte
      ahci_interrupt_changed_ccc = false;

      // wait for 1ms
      do_nanosleep_ccc(1000000);

      if (enable_data_dump_ccc)    // TODO this is for hba debug testing
      {
        char temp_string[256];
        sprintf (temp_string, "dump 14 for port number %d\n", port_number_ccc[count]);
        dump_info_to_filename_ccc (data_dump_filename_ccc, temp_string);
        unsigned char temp_buffer[0x80];
        memcpy(temp_buffer, port_virt_addr_ccc, 0x80);
        dump_data_to_filename_binary_ccc(data_dump_filename_ccc, temp_buffer, 0x80, "Port data");
      }

      // close mapped memory
      munmap(port_mem_pointer_ccc, port_alloc_mem_size_ccc);
      close(port_mem_dev_ccc);

      #ifdef DEBUG
      if (debug_ccc & DEBUG36)
      {
        for (i = 0; i < 256; i+=16)
        {
          fprintf (stdout, "%x: ", i);
          unsigned char *c;
          int n;
          for (n=0; n < 16 && i+n < 512; n++)
          {
            c = (unsigned char *)fis_buffer_ccc+i+n;
            fprintf (stdout, "%02x ", *c);
          }
          fprintf (stdout, "   ");
          for (n=0; n < 16 && i+n < 512; n++)
          {
            c = (unsigned char *)fis_buffer_ccc+i+n;
            fprintf (stdout, "%c", isprint(*c) ? *c : '.');
          }
          fprintf (stdout, "\n");
        }
      }

      if (debug_ccc & DEBUG36)
      {
        for (i = 0; i < 256; i+=16)
        {
          fprintf (stdout, "%x: ", i);
          unsigned char *c;
          int n;
          for (n=0; n < 16 && i+n < 512; n++)
          {
            c = (unsigned char *)table_buffer_ccc+i+n;
            fprintf (stdout, "%02x ", *c);
          }
          fprintf (stdout, "   ");
          for (n=0; n < 16 && i+n < 512; n++)
          {
            c = (unsigned char *)table_buffer_ccc+i+n;
            fprintf (stdout, "%c", isprint(*c) ? *c : '.');
          }
          fprintf (stdout, "\n");
        }
      }

      if (debug_ccc & DEBUG36)
      {
        for (i = 0; i < 128; i+=16)
        {
          fprintf (stdout, "%x: ", i);
          unsigned char *c;
          int n;
          for (n=0; n < 16 && i+n < 512; n++)
          {
            c = (unsigned char *)command_list_buffer_ccc+i+n;
            fprintf (stdout, "%02x ", *c);
          }
          fprintf (stdout, "   ");
          for (n=0; n < 16 && i+n < 512; n++)
          {
            c = (unsigned char *)command_list_buffer_ccc+i+n;
            fprintf (stdout, "%c", isprint(*c) ? *c : '.');
          }
          fprintf (stdout, "\n");
        }
      }
      #endif

      if (timeout)
      {
        strcpy (model_ccc[count], "timeout");
        char str[32];
        sprintf(str, "e/s=%04x", io_doubleword_ccc);
        strcpy (serial_ccc[count], str);
        return (1);
      }
      else
      {
        // check for error
        if (io_doubleword_ccc & 0x0001)
        {
          strcpy (model_ccc[count], "error");
          char str[32];
          sprintf(str, "e/s=%04x", io_doubleword_ccc);
          strcpy (serial_ccc[count], str);
          return (1);
        }
      }

    }



    else if (port_signature_ccc[count] == SATA_SIG_ATAPI)
    {
      strcpy (model_ccc[count], "satapi device");
      return (2);
    }
    else if (port_signature_ccc[count] == SATA_SIG_SEMB)
    {
      strcpy (model_ccc[count], "enclosure management bridge");
      return (2);
    }
    else if (port_signature_ccc[count] == SATA_SIG_PM)
    {
      strcpy (model_ccc[count], "port multiplier");
      return (2);
    }
    else
    {
      strcpy (model_ccc[count], "unknown device");
      return (2);
    }


    int i;
    if (verbose_ccc & DEBUG9)
    {
      for (i = 0; i < 512; i+=16)
      {
        fprintf (stdout, "%x: ", i);
        unsigned char *c;
        int n;
        for (n=0; n < 16 && i+n < 512; n++)
        {
          c = (unsigned char *)ccc_buffer_ccc+i+n;
          fprintf (stdout, "%02x ", *c);
        }
        fprintf (stdout, "   ");
        for (n=0; n < 16 && i+n < 512; n++)
        {
          c = (unsigned char *)ccc_buffer_ccc+i+n;
          fprintf (stdout, "%c", isprint(*c) ? *c : '.');
        }
        fprintf (stdout, "\n");
      }
    }

    char temp_model[41] = "";
    char temp_serial[21] = "";
    for (i = 0; i < 40; i += 2)
    {
      unsigned char *c = (unsigned char *)ccc_buffer_ccc+54+i+1;
      unsigned char *d = (unsigned char *)ccc_buffer_ccc+54+i;
      if ( isprint(*c) )
      {
        temp_model[i] = *c;
      }
      else
      {
        temp_model[i] = '\0';
      }
      if ( isprint(*d) )
      {
        temp_model[i+1] = *d;
      }
      else
      {
        temp_model[i+1] = '\0';
      }
    }
    temp_model[40] = '\0';
    for (i = 0; i < 20; i += 2)
    {
      unsigned char *c = (unsigned char *)ccc_buffer_ccc+20+i+1;
      unsigned char *d = (unsigned char *)ccc_buffer_ccc+20+i;
      if ( isprint(*c) )
      {
        temp_serial[i] = *c;
      }
      else
      {
        temp_serial[i] = '\0';
      }
      if ( isprint(*d) )
      {
        temp_serial[i+1] = *d;
      }
      else
      {
        temp_serial[i+1] = '\0';
      }
    }
    temp_serial[20] = '\0';

    char *found_model = trim_white_space_ccc(temp_model);
    char *found_serial = trim_white_space_ccc(temp_serial);

    strcpy (model_ccc[count], found_model);
    strcpy (serial_ccc[count], found_serial);

    uint16_t word;
    uint32_t double_word;
    uint64_t quad_word;
    words_per_logical_sector_ccc = 256;
    bytes_per_logical_sector_ccc = 512;
    memcpy(&word, ccc_buffer_ccc+212, 2);
    //check if word data is valid
    if ((word & 0xc000) == 0x4000)
    {
      // check if sector size greater than 256 words
      if (word & 0x1000)
      {
        memcpy(&double_word, ccc_buffer_ccc+234, 4);
        words_per_logical_sector_ccc = double_word;
      }
    }
    bytes_per_logical_sector_ccc = words_per_logical_sector_ccc * 2;
    memcpy(&double_word, ccc_buffer_ccc+120, 4);
    memcpy(&quad_word, ccc_buffer_ccc+200, 8);
    unsigned char c;
    // pick based on if extend 48 bit supported
    memcpy(&c, ccc_buffer_ccc+167, 1);
    if (c & 4)
    {
      drive_size_ccc[count] = (quad_word * bytes_per_logical_sector_ccc);
    }
    else
    {
      drive_size_ccc[count] = (double_word * bytes_per_logical_sector_ccc);
    }

    // check if lba is supported and if not get data from chs
    memcpy(&c, ccc_buffer_ccc+99, 1);
    if ( !(c & 2) )
    {
      bytes_per_logical_sector_ccc = 512;    // CHS should always be 512 bytes per sector
      uint16_t word;
      memcpy(&word, ccc_buffer_ccc+106, 2);
      if (word & 1)
      {
        memcpy(&word, ccc_buffer_ccc+114, 2);
        drive_size_ccc[count] = word << 16;
        memcpy(&word, ccc_buffer_ccc+116, 2);
        drive_size_ccc[count] = drive_size_ccc[count] + word;
        drive_size_ccc[count] = drive_size_ccc[count] * bytes_per_logical_sector_ccc;
      }
      else
      {
        memcpy(&word, ccc_buffer_ccc+2, 2);
        drive_size_ccc[count] = word;
        memcpy(&word, ccc_buffer_ccc+6, 2);
        drive_size_ccc[count] = drive_size_ccc[count] * word;
        memcpy(&word, ccc_buffer_ccc+12, 2);
        drive_size_ccc[count] = drive_size_ccc[count] * word;
        drive_size_ccc[count] = drive_size_ccc[count] * bytes_per_logical_sector_ccc;
      }
    }
    drive_bytes_per_sector_ccc[count] = bytes_per_logical_sector_ccc;

    device_type_ccc[count] = DEVICE_TYPE_AHCI;

    if (enable_data_dump_ccc)
    {
      char temp_string[256];
      sprintf (temp_string, "dump after identify for port number %d\n", port_number_ccc[count]);
      dump_info_to_filename_ccc (data_dump_filename_ccc, temp_string);
      dump_hba_port_fis_command_data_ccc(hba_address_ccc[count], port_address_ccc[count], 1, 1, 1, 1, 1);
      dump_data_to_filename_ccc(data_dump_filename_ccc, ccc_buffer_ccc, ccc_main_buffer_size_ccc, "Main buffer data");
    }

    memset (ccc_buffer_ccc, 0, ccc_main_buffer_size_ccc);
  }
  else
  {
    count = count;
  }
  return (0);
}






int identify_device_passthrough_ccc(char *name, int count)
{
  memset (ccc_buffer_ccc, 0, ccc_main_buffer_size_ccc);
  passthrough_ccc.ata_cmd[0] = 0;
  passthrough_ccc.ata_cmd[1] = 0;
  passthrough_ccc.ata_cmd[2] = 0;
  passthrough_ccc.ata_cmd[3] = 0;
  passthrough_ccc.ata_cmd[4] = 0;
  passthrough_ccc.ata_cmd[5] = 0xa0;
  passthrough_ccc.ata_cmd[6] = 0xec;    // identify device
  command_length_ccc = 12;
  passthrough_ccc.protocol = 4;    // pio-data-in
  passthrough_ccc.check_condition = 1;    // check condition true
  passthrough_ccc.direction = 2;    // from
  passthrough_ccc.transfer_direction = 1;    // from
  passthrough_ccc.sg_xfer_direction = SG_DXFER_FROM_DEV;
  if (enable_data_dump_ccc)
  {
    // set the count to 1
    passthrough_ccc.ata_cmd[1] = 1;
  }

  int disk_fd = open (name, O_RDWR | O_NONBLOCK);
  // if there is an error try with read only
  if (disk_fd == -1)
  {
    disk_fd = open (name, O_RDONLY | O_NONBLOCK);
  }
  if (disk_fd == -1)
  {
    sprintf(error_string_ccc, "Error(%s)", strerror(errno));
    strncpy(model_ccc[count], error_string_ccc, 40);
    return (1);
  }

  do_ata28_cmd_ccc(disk_fd);

  if (command_status_ccc != 0)
  {
    strncpy(model_ccc[count], error_string_ccc, 40);
    return (2);
  }

  if (sense_key_ccc > 1)
  {
    // check for usb result
    if (sense_key_ccc != 4 || asc_ccc != 0 || ascq_ccc != 0)
    {
      sprintf(error_string_ccc, "sense-data %02x %02x %02x", sense_key_ccc, asc_ccc, ascq_ccc);
      strncpy(model_ccc[count], error_string_ccc, 40);
      return (3);
    }
  }

  close (disk_fd);

  int i;
  if (verbose_ccc & DEBUG9)
  {
    for (i = 0; i < 512; i+=16)
    {
      fprintf (stdout, "%x: ", i);
      unsigned char *c;
      int n;
      for (n=0; n < 16 && i+n < 512; n++)
      {
        c = (unsigned char *)ccc_buffer_ccc+i+n;
        fprintf (stdout, "%02x ", *c);
      }
      fprintf (stdout, "   ");
      for (n=0; n < 16 && i+n < 512; n++)
      {
        c = (unsigned char *)ccc_buffer_ccc+i+n;
        fprintf (stdout, "%c", isprint(*c) ? *c : '.');
      }
      fprintf (stdout, "\n");
    }
  }

  char temp_model[41] = "";
  char temp_serial[21] = "";
  for (i = 0; i < 40; i += 2)
  {
    unsigned char *c = (unsigned char *)ccc_buffer_ccc+54+i+1;
    unsigned char *d = (unsigned char *)ccc_buffer_ccc+54+i;
    if ( isprint(*c) )
    {
      temp_model[i] = *c;
    }
    else
    {
      temp_model[i] = '\0';
    }
    if ( isprint(*d) )
    {
      temp_model[i+1] = *d;
    }
    else
    {
      temp_model[i+1] = '\0';
    }
  }
  temp_model[40] = '\0';
  for (i = 0; i < 20; i += 2)
  {
    unsigned char *c = (unsigned char *)ccc_buffer_ccc+20+i+1;
    unsigned char *d = (unsigned char *)ccc_buffer_ccc+20+i;
    if ( isprint(*c) )
    {
      temp_serial[i] = *c;
    }
    else
    {
      temp_serial[i] = '\0';
    }
    if ( isprint(*d) )
    {
      temp_serial[i+1] = *d;
    }
    else
    {
      temp_serial[i+1] = '\0';
    }
  }
  temp_serial[20] = '\0';

  char *found_model = trim_white_space_ccc(temp_model);
  char *found_serial = trim_white_space_ccc(temp_serial);

  strcpy (model_ccc[count], found_model);
  strcpy (serial_ccc[count], found_serial);

  uint16_t word;
  uint32_t double_word;
  uint64_t quad_word;
  words_per_logical_sector_ccc = 256;
  bytes_per_logical_sector_ccc = 512;
  memcpy(&word, ccc_buffer_ccc+212, 2);
  //check if word data is valid
  if ((word & 0xc000) == 0x4000)
  {
    // check if sector size greater than 256 words
    if (word & 0x1000)
    {
      memcpy(&double_word, ccc_buffer_ccc+234, 4);
      words_per_logical_sector_ccc = double_word;
    }
  }
  bytes_per_logical_sector_ccc = words_per_logical_sector_ccc * 2;
  memcpy(&double_word, ccc_buffer_ccc+120, 4);
  memcpy(&quad_word, ccc_buffer_ccc+200, 8);
  unsigned char c;
  // pick based on if extend 48 bit supported
  memcpy(&c, ccc_buffer_ccc+167, 1);
  if (c & 4)
  {
    drive_size_ccc[count] = (quad_word * bytes_per_logical_sector_ccc);
  }
  else
  {
    drive_size_ccc[count] = (double_word * bytes_per_logical_sector_ccc);
  }

  // check if lba is supported and if not get data from chs
  memcpy(&c, ccc_buffer_ccc+99, 1);
  if ( !(c & 2) )
  {
    bytes_per_logical_sector_ccc = 512;    // CHS should always be 512 bytes per sector
    uint16_t word;
    memcpy(&word, ccc_buffer_ccc+106, 2);
    if (word & 1)
    {
      memcpy(&word, ccc_buffer_ccc+114, 2);
      drive_size_ccc[count] = word << 16;
      memcpy(&word, ccc_buffer_ccc+116, 2);
      drive_size_ccc[count] = drive_size_ccc[count] + word;
      drive_size_ccc[count] = drive_size_ccc[count] * bytes_per_logical_sector_ccc;
    }
    else
    {
      memcpy(&word, ccc_buffer_ccc+2, 2);
      drive_size_ccc[count] = word;
      memcpy(&word, ccc_buffer_ccc+6, 2);
      drive_size_ccc[count] = drive_size_ccc[count] * word;
      memcpy(&word, ccc_buffer_ccc+12, 2);
      drive_size_ccc[count] = drive_size_ccc[count] * word;
      drive_size_ccc[count] = drive_size_ccc[count] * bytes_per_logical_sector_ccc;
    }
  }
  drive_bytes_per_sector_ccc[count] = bytes_per_logical_sector_ccc;

  device_type_ccc[count] = DEVICE_TYPE_ATA;

  memset (ccc_buffer_ccc, 0, ccc_main_buffer_size_ccc);
  return (0);
}





int identify_device_scsi_ccc(char *name, int count)
{
  memset (ccc_buffer_ccc, 0, ccc_main_buffer_size_ccc);
  passthrough_ccc.scsi_cmd[0] = 0x12; // inquiry command
  passthrough_ccc.scsi_cmd[1] = 0;
  passthrough_ccc.scsi_cmd[2] = 0;
  passthrough_ccc.scsi_cmd[3] = 0;
  passthrough_ccc.scsi_cmd[4] = 44; // size of data requested back
  passthrough_ccc.scsi_cmd[5] = 0;
  command_length_ccc = 6;
  passthrough_ccc.sg_xfer_direction = SG_DXFER_FROM_DEV;

  usb_transfer_direction_ccc = USB_DIRECTION_IN;

  int disk_fd = open (name, O_RDWR | O_NONBLOCK);
  // if there is an error try with read only
  if (disk_fd == -1)
  {
    disk_fd = open (name, O_RDONLY | O_NONBLOCK);
  }
  if (disk_fd == -1)
  {
    sprintf(error_string_ccc, "Error(%s)", strerror(errno));
    strncpy(model_ccc[count], error_string_ccc, 40);
    return (1);
  }

  int buffersizebak = ccc_main_buffer_size_ccc;
  ccc_main_buffer_size_ccc = 44;
  do_scsi_cmd_ccc(disk_fd);
  ccc_main_buffer_size_ccc = buffersizebak;

  if (command_status_ccc != 0)
  {
    strncpy(model_ccc[count], error_string_ccc, 40);
    close (disk_fd);
    return (2);
  }

  if (sense_key_ccc > 1)
  {
    sprintf(error_string_ccc, "sense-data %02x %02x %02x", sense_key_ccc, asc_ccc, ascq_ccc);
    strncpy(model_ccc[count], error_string_ccc, 40);
    close (disk_fd);
    return (3);
  }

  close (disk_fd);

  int i;
  if (verbose_ccc & DEBUG9)
  {
    for (i = 0; i < 44; i+=16)
    {
      fprintf (stdout, "%x: ", i);
      unsigned char *c;
      int n;
      for (n=0; n < 16 && i+n < 512; n++)
      {
        c = (unsigned char *)ccc_buffer_ccc+i+n;
        fprintf (stdout, "%02x ", *c);
      }
      fprintf (stdout, "   ");
      for (n=0; n < 16 && i+n < 512; n++)
      {
        c = (unsigned char *)ccc_buffer_ccc+i+n;
        fprintf (stdout, "%c", isprint(*c) ? *c : '.');
      }
      fprintf (stdout, "\n");
    }
  }

  char temp_model[41] = "";
  char temp_serial[21] = "";
  for (i = 0; i < 8; i ++)
  {
    unsigned char *c = (unsigned char *)ccc_buffer_ccc+8+i;
    if ( isprint(*c) )
    {
      temp_model[i] = *c;
    }
    else
    {
      break;
    }
  }
  int n;
  for (n = 0; n < 16; n++)
  {
    unsigned char *d = (unsigned char *)ccc_buffer_ccc+16+n;
    if ( isprint(*d) )
    {
      temp_model[n+i] = *d;
    }
    else
    {
      temp_model[n+i] = '\0';
    }
  }
  temp_model[24] = '\0';
  for (i = 0; i < 8; i ++)
  {
    unsigned char *c = (unsigned char *)ccc_buffer_ccc+36+i;
    if ( isprint(*c) )
    {
      temp_serial[i] = *c;
    }
    else
    {
      temp_serial[i] = '\0';
    }
  }
  temp_serial[8] = '\0';

  char *found_model = trim_white_space_ccc(temp_model);
  char *found_serial = trim_white_space_ccc(temp_serial);

  strcpy (model_ccc[count], found_model);
  strcpy (serial_ccc[count], found_serial);


  // get drive size
  uint32_t double_word;
  uint64_t quad_word;
  unsigned long long capacity = 0;
  unsigned int blocksize = 0;
  memset (ccc_buffer_ccc, 0, ccc_main_buffer_size_ccc);
  passthrough_ccc.scsi_cmd[0] = 0x25; // read capacity 10
  passthrough_ccc.scsi_cmd[1] = 0;
  passthrough_ccc.scsi_cmd[2] = 0;
  passthrough_ccc.scsi_cmd[3] = 0;
  passthrough_ccc.scsi_cmd[4] = 0;
  passthrough_ccc.scsi_cmd[5] = 0;
  passthrough_ccc.scsi_cmd[6] = 0;
  passthrough_ccc.scsi_cmd[7] = 0;
  passthrough_ccc.scsi_cmd[8] = 0;
  passthrough_ccc.scsi_cmd[9] = 0;
  command_length_ccc = 10;
  passthrough_ccc.sg_xfer_direction = SG_DXFER_FROM_DEV;
  usb_transfer_direction_ccc = USB_DIRECTION_IN;
  disk_fd = open (name, O_RDWR | O_NONBLOCK);
  // if there is an error try with read only
  if (disk_fd == -1)
  {
    disk_fd = open (name, O_RDONLY | O_NONBLOCK);
  }
  if (disk_fd == -1)
  {
    drive_size_ccc[count] = -1;
    return (1);
  }
  buffersizebak = ccc_main_buffer_size_ccc;
  ccc_main_usbbuffer_size_ccc = 8;
  do_scsi_cmd_ccc(disk_fd);
  ccc_main_buffer_size_ccc = buffersizebak;
  if (command_status_ccc != 0)
  {
    drive_size_ccc[count] = -2;
    close (disk_fd);
    return (2);
  }
  if (sense_key_ccc > 1)
  {
    drive_size_ccc[count] = -3;
    close (disk_fd);
    return (3);
  }
  close (disk_fd);
  memcpy(&double_word, ccc_buffer_ccc + 0, 4);
  capacity = __bswap_32 (double_word);
  memcpy(&double_word, ccc_buffer_ccc + 4, 4);
  blocksize = __bswap_32 (double_word);

  if (capacity == 0xffffffff)
  {
    passthrough_ccc.scsi_cmd[0] = 0x9e; // read capacity 16
    passthrough_ccc.scsi_cmd[1] = 0x10;  // service action
    passthrough_ccc.scsi_cmd[2] = 0;
    passthrough_ccc.scsi_cmd[3] = 0;
    passthrough_ccc.scsi_cmd[4] = 0;
    passthrough_ccc.scsi_cmd[5] = 0;
    passthrough_ccc.scsi_cmd[6] = 0;
    passthrough_ccc.scsi_cmd[7] = 0;
    passthrough_ccc.scsi_cmd[8] = 0;
    passthrough_ccc.scsi_cmd[9] = 0;
    passthrough_ccc.scsi_cmd[10] = 0;
    passthrough_ccc.scsi_cmd[11] = 0;
    passthrough_ccc.scsi_cmd[12] = 0;
    passthrough_ccc.scsi_cmd[13] = 32;  // lenth of return data
    passthrough_ccc.scsi_cmd[14] = 0;
    passthrough_ccc.scsi_cmd[15] = 0;
    command_length_ccc = 16;
    passthrough_ccc.sg_xfer_direction = SG_DXFER_FROM_DEV;
    usb_transfer_direction_ccc = USB_DIRECTION_IN;
    disk_fd = open (name, O_RDWR | O_NONBLOCK);
    // if there is an error try with read only
    if (disk_fd == -1)
    {
      disk_fd = open (name, O_RDONLY | O_NONBLOCK);
    }
    if (disk_fd == -1)
    {
      drive_size_ccc[count] = -1;
      return (1);
    }
    int buffersizebak = ccc_main_buffer_size_ccc;
    ccc_main_usbbuffer_size_ccc = 32;
    do_scsi_cmd_ccc(disk_fd);
    ccc_main_buffer_size_ccc = buffersizebak;
    if (command_status_ccc != 0)
    {
      drive_size_ccc[count] = -2;
      close (disk_fd);
      return (2);
    }
    if (sense_key_ccc > 1)
    {
      drive_size_ccc[count] = -3;
      close (disk_fd);
      return (3);
    }
    close (disk_fd);
    memcpy(&quad_word, ccc_buffer_ccc + 0, 8);
    capacity = __bswap_64 (quad_word);
    memcpy(&double_word, ccc_buffer_ccc + 8, 4);
    blocksize =__bswap_32 (double_word);
  }
  drive_size_ccc[count] = (capacity + 1) * blocksize;
  drive_bytes_per_sector_ccc[count] = blocksize;
  device_type_ccc[count] = DEVICE_TYPE_SCSI;

  memset (ccc_buffer_ccc, 0, ccc_main_buffer_size_ccc);
  return (0);
}







int choose_device_ccc(void)
{
  if (usb_mode_ccc)
  {
    int ret = choose_usb_device_ccc();
    return ret;
  }
  //fprintf (stdout, "choose_device_ccc\n");    //debug
  char check_serial[21] = "";
  int check_choice = 0;
  strcpy (current_source_model_ccc, "");
  strcpy (current_source_serial_ccc, "");

  if (ahci_mode_ccc)
  {
    if (superbyte_ccc[41] == 0xd4)
    {
      // ahci
      char input_text[32];
      char raw_value[32];
      int choice = -1;
      int keeptrying = 1;
      if (drive_serial_ccc == NULL && !superclone_ccc)
      {
        while (keeptrying)
        {
          strcpy(raw_value, "");
          int i;
          fprintf (stdout, "\nQ) Quit\n");
          fprintf (stdout, "R) Refresh drive list\n");
          for (i = 0; i < device_count_ccc; i++)
          {
            if (verbose_ccc & DEBUG6)
            {
              fprintf (stdout, "%d) %s %s %s %s %llx %d %llx (%lld) %s %s\n", i+1, device_driver_ccc[i], device_bus_ccc[i], device_reference_ccc[i], device_name_ccc[i], hba_address_ccc[i], port_number_ccc[i], port_address_ccc[i], drive_size_ccc[i], model_ccc[i], serial_ccc[i]);
            }
            else if (verbose_ccc & DEBUG5)
            {
              fprintf (stdout, "%d) %s %s %llx %d %llx (%lld) %s %s\n", i+1, device_reference_ccc[i], device_name_ccc[i], hba_address_ccc[i], port_number_ccc[i], port_address_ccc[i], drive_size_ccc[i], model_ccc[i], serial_ccc[i]);
            }
            else
            {
              fprintf (stdout, "%d) %s %s (%lld) %s %s\n", i+1, device_reference_ccc[i], device_name_ccc[i], drive_size_ccc[i], model_ccc[i], serial_ccc[i]);
            }
          }

          fprintf (stdout, "Choose which drive > ");

          fflush(stdout);

          if ( fgets(input_text, sizeof input_text, stdin) != NULL )
          {
            sscanf(input_text, "%s", raw_value);
            if (strcasecmp(raw_value, "Q") == 0)
            {
              return (-1);
            }
            else if (strcasecmp(raw_value, "R") == 0)
            {
              find_all_devices_ccc();
            }
            else
            {
              char* endptr;
              choice = strtol(raw_value, &endptr, 0);
              if (*endptr)
              {
                fprintf (stderr, "Error! Choice \'%s\' was unable to processed.\n", raw_value);
              }
              else if (choice < 1 || choice > device_count_ccc)
              {
                fprintf (stderr, "Error! Choice %d is out of range.\n", choice);
              }
              else
              {
                keeptrying = 0;
              }
            }
          }
        }
      }

      else
      {
        choice = new_source_ccc + 1;
      }


      if (choice == -1)
      {
        fprintf (stdout, "Error: Drive serial not found\n");
        return (-2);
      }

      int i = choice - 1;
      if (verbose_ccc & DEBUG6)
      {
        fprintf (stdout, "%d) %s %s %s %s %llx %d %llx (%lld) %s %s\n", i+1, device_driver_ccc[i], device_bus_ccc[i], device_reference_ccc[i], device_name_ccc[i], hba_address_ccc[i], port_number_ccc[i], port_address_ccc[i], drive_size_ccc[i], model_ccc[i], serial_ccc[i]);
      }

      check_choice = i;
      strcpy(check_serial, serial_ccc[i]);
      hba_base_address_ccc = hba_address_ccc[i];
      port_base_address_ccc = port_address_ccc[i];
      port_number_base_ccc = port_number_ccc[i];
      strcpy(device_driver_base_ccc, device_driver_ccc[i]);
      strcpy(device_bus_base_ccc, device_bus_ccc[i]);
      strcpy (current_source_model_ccc, model_ccc[i]);
      strcpy (current_source_serial_ccc, serial_ccc[i]);
      current_device_type_ccc = device_type_ccc[i];
      disk_1_ccc = malloc (sizeof device_reference_ccc[i]);
      memcpy (disk_1_ccc, device_reference_ccc[i], sizeof device_reference_ccc[i]);
      if (drive_bytes_per_sector_ccc[i] > 0)
      {
        source_total_size_ccc = drive_size_ccc[i] / drive_bytes_per_sector_ccc[i];
        bytes_per_sector_ccc = drive_bytes_per_sector_ccc[i];
      }
      else
      {
        source_total_size_ccc = 0;
        bytes_per_sector_ccc = 0;
      }
    }
  }



  else if (direct_mode_ccc)
  {
    // direct
    char input_text[32];
    char raw_value[32];
    int choice = -1;
    int keeptrying = 1;
    if (drive_serial_ccc == NULL && !superclone_ccc)
    {
      while (keeptrying)
      {
        strcpy(raw_value, "");
        int i;
        fprintf (stdout, "\nQ) Quit\n");
        fprintf (stdout, "R) Refresh drive list\n");
        for (i = 0; i < device_count_ccc; i++)
        {
          if (verbose_ccc & DEBUG6)
          {
            fprintf (stdout, "%d) %s %s %s %s %d %llx %llx %llx %d (%lld) %s %s\n", i+1, device_driver_ccc[i], device_bus_ccc[i], device_reference_ccc[i], device_name_ccc[i], device_visable_ccc[i], reg_address_ccc[i], control_address_ccc[i], bus_address_ccc[i], device_select_ccc[i], drive_size_ccc[i], model_ccc[i], serial_ccc[i]);
          }
          else if (verbose_ccc & DEBUG5)
          {
            fprintf (stdout, "%d) %s %s %llx %llx %llx %d (%lld) %s %s\n", i+1, device_reference_ccc[i], device_name_ccc[i], reg_address_ccc[i], control_address_ccc[i], bus_address_ccc[i], device_select_ccc[i], drive_size_ccc[i], model_ccc[i], serial_ccc[i]);
          }
          else
          {
            fprintf (stdout, "%d) %s %s (%lld) %s %s\n", i+1, device_reference_ccc[i], device_name_ccc[i], drive_size_ccc[i], model_ccc[i], serial_ccc[i]);
          }
        }

        fprintf (stdout, "Choose which drive > ");

        fflush(stdout);

        if ( fgets(input_text, sizeof input_text, stdin) != NULL )
        {
          sscanf(input_text, "%s", raw_value);
          if (strcasecmp(raw_value, "Q") == 0)
          {
            return (-1);
          }
          else if (strcasecmp(raw_value, "R") == 0)
          {
            find_all_devices_ccc();
          }
          else
          {
            char* endptr;
            choice = strtol(raw_value, &endptr, 0);
            if (*endptr)
            {
              fprintf (stderr, "Error! Choice \'%s\' was unable to processed.\n", raw_value);
            }
            else if (choice < 1 || choice > device_count_ccc)
            {
              fprintf (stderr, "Error! Choice %d is out of range.\n", choice);
            }
            else
            {
              keeptrying = 0;
            }
          }
        }
      }
    }

    else
    {
      choice = new_source_ccc + 1;
    }

    if (choice == -1)
    {
      fprintf (stdout, "Error: Drive serial not found\n");
      return (-2);
    }

    int i = choice - 1;
    if (verbose_ccc & DEBUG6)
    {
      fprintf (stdout, "choice=%d %s %s %llx %llx %llx %d (%lld) %s %s\n", i+1, device_driver_ccc[i], device_bus_ccc[i], reg_address_ccc[i], control_address_ccc[i], bus_address_ccc[i], device_select_ccc[i], drive_size_ccc[i], model_ccc[i], serial_ccc[i]);
    }

    check_choice = i;
    strcpy(check_serial, serial_ccc[i]);
    reg_base_address_ccc = reg_address_ccc[i];
    control_base_address_ccc = control_address_ccc[i];
    bus_base_address_ccc = bus_address_ccc[i];
    device_select_base_ccc = device_select_ccc[i];
    strcpy (current_source_model_ccc, model_ccc[i]);
    strcpy (current_source_serial_ccc, serial_ccc[i]);
    current_device_type_ccc = device_type_ccc[i];
    disk_1_ccc = malloc (sizeof device_reference_ccc[i]);
    memcpy (disk_1_ccc, device_reference_ccc[i], sizeof device_reference_ccc[i]);
    if (drive_bytes_per_sector_ccc[i] > 0)
    {
      source_total_size_ccc = drive_size_ccc[i] / drive_bytes_per_sector_ccc[i];
      bytes_per_sector_ccc = drive_bytes_per_sector_ccc[i];
    }
    else
    {
      source_total_size_ccc = 0;
      bytes_per_sector_ccc = 0;
    }
  }



  else if (scsi_passthrough_ccc || ata_passthrough_ccc || generic_mode_ccc)
  {
    // passthrough
    char input_text[32];
    char raw_value[32];
    int choice = -1;
    int keeptrying = 1;
    if (drive_serial_ccc == NULL && !superclone_ccc)
    {
      while (keeptrying)
      {
        strcpy(raw_value, "");
        int i;
        fprintf (stdout, "\nQ) Quit\n");
        fprintf (stdout, "R) Refresh drive list\n");
        for (i = 0; i < device_count_ccc; i++)
        {
          fprintf (stdout, "%d) %s (%lld) %s %s\n", i+1, drive_list_ccc[i], drive_size_ccc[i], model_ccc[i], serial_ccc[i]);
        }

        fprintf (stdout, "Choose which drive > ");

        fflush(stdout);

        if ( fgets(input_text, sizeof input_text, stdin) != NULL )
        {
          sscanf(input_text, "%s", raw_value);
          if (strcasecmp(raw_value, "Q") == 0)
          {
            return (-1);
          }
          else if (strcasecmp(raw_value, "R") == 0)
          {
            find_all_devices_ccc();
          }
          else
          {
            char* endptr;
            choice = strtol(raw_value, &endptr, 0);
            if (*endptr)
            {
              fprintf (stderr, "Error! Choice \'%s\' was unable to processed.\n", raw_value);
            }
            else if (choice < 1 || choice > device_count_ccc)
            {
              fprintf (stderr, "Error! Choice %d is out of range.\n", choice);
            }
            else
            {
              keeptrying = 0;
            }
          }
        }
      }
    }

    else
    {
      choice = new_source_ccc + 1;
    }

    if (choice == -1)
    {
      fprintf (stdout, "Error: Drive serial not found\n");
      return (-2);
    }

    int i = choice - 1;
    if (verbose_ccc & DEBUG6)
    {
      fprintf (stdout, "choice=%d %s (%lld) %s %s\n", i+1, drive_list_ccc[i], drive_size_ccc[i], model_ccc[i], serial_ccc[i]);
    }

    check_choice = i;
    strcpy(check_serial, serial_ccc[i]);
    disk_1_ccc = malloc (sizeof drive_list_ccc[i]);
    memcpy (disk_1_ccc, drive_list_ccc[i], sizeof drive_list_ccc[i]);
    strcpy (current_source_model_ccc, model_ccc[i]);
    strcpy (current_source_serial_ccc, serial_ccc[i]);
    current_device_type_ccc = device_type_ccc[i];
    if (superclone_ccc)
    {
      if (drive_bytes_per_sector_ccc[i] > 0)
      {
        source_total_size_ccc = drive_size_ccc[i] / drive_bytes_per_sector_ccc[i];
        bytes_per_sector_ccc = drive_bytes_per_sector_ccc[i];
      }
      else
      {
        source_total_size_ccc = 0;
        bytes_per_sector_ccc = 0;
      }
    }
  }

  // if direct mode then do safety checks
  if (direct_mode_ccc)
  {
    if (!ahci_mode_ccc)
    {
      // first check for another drive on same controller
      int same_controller = 0;
      // if odd
      if (device_select_ccc[check_choice])
      {
        if (device_present_ccc[check_choice-1] || device_visable_ccc[check_choice-1])
        {
          same_controller = 1;
        }
      }
      // if even
      else
      {
        if (device_present_ccc[check_choice+1] || device_visable_ccc[check_choice+1])
        {
          same_controller = 1;
        }
      }
      if (same_controller)
      {
        char input_text[32];
        char raw_value[32];
        if (superclone_ccc)
        {
          sprintf (tempmessage_ccc, "%s", curlang_ccc[LANGDRIVESAMECONTROLLER]);
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGWARN], 0);
          clear_error_message_ccc();
          if ( !force_danger_ccc )
          {
            return (-100);
          }
        }
        else
        {
          fprintf (stdout, "WARNING: There is possibly another drive on the same controller!\n");
          fprintf (stdout, "You should NEVER work with a drive with another one on the same controller!\n");
          fprintf (stdout, "That could be very dangerous, and could result in data loss (or worse)!\n");
          if ( !force_danger_ccc )
          {
            fprintf (stdout, "If you still wish to continue, type the word DANGEROUS > ");
            fflush(stdout);
            if ( fgets(input_text, sizeof input_text, stdin) != NULL )
            {
              sscanf(input_text, "%s", raw_value);
              if (strcmp(raw_value, "DANGEROUS") != 0)
              {
                fprintf (stdout, "You chose not to live dangerously, now exiting...\n");
                return (-1);
              }
            }
          }
        }
      }

      // check if device is a slave
      if (device_select_ccc[check_choice])
      {
        char input_text[32];
        char raw_value[32];
        if (superclone_ccc)
        {
          sprintf (tempmessage_ccc, "%s", curlang_ccc[LANGDRIVEISSLAVE]);
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGWARN], 0);
          clear_error_message_ccc();
          if ( !force_danger_ccc )
          {
            return (-100);
          }
        }
        else
        {
          fprintf (stdout, "WARNING: The selected device is a slave on the controller!\n");
          fprintf (stdout, "This does not always work as expected!\n");
          fprintf (stdout, "That could be very dangerous, and could result in data loss (or worse)!\n");
          fprintf (stdout, "It is recommended for the device to be master on the controller!\n");
          if ( !force_danger_ccc )
          {
            fprintf (stdout, "If you still wish to continue, type the word DANGEROUS > ");
            fflush(stdout);
            if ( fgets(input_text, sizeof input_text, stdin) != NULL )
            {
              sscanf(input_text, "%s", raw_value);
              if (strcmp(raw_value, "DANGEROUS") != 0)
              {
                fprintf (stdout, "You chose not to live dangerously, now exiting...\n");
                return (-1);
              }
            }
          }
        }
      }
    }

    // check if drive is visable to OS
    if (device_visable_ccc[check_choice])
    {
      if (superclone_ccc)
      {
        sprintf (tempmessage_ccc, "%s", curlang_ccc[LANGDRIVEVISABLEOS]);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGWARN], 0);
        clear_error_message_ccc();
        if ( !force_stupid_ccc )
        {
          return (-200);
        }
      }
      else
      {
        fprintf (stdout, "WARNING: This drive is also available to the OS!\n");
        fprintf (stdout, "That is too dangerous, and would likely result in a computer crash!\n");
        fprintf (stdout, "Please make sure to follow the instrutions on how to hide the drive!\n");
        if ( !force_stupid_ccc )
        {
          fprintf (stdout, "Now exiting...\n");
          return (-1);
        }
      }
    }
    if (0)
    {
      // now check to see if this drive is listed in the OS
      // temporarally turn off direct mode to populate the passthrough drive list
      direct_mode_ccc = false;
      scsi_passthrough_ccc = true;
      find_all_devices_ccc();
      scsi_passthrough_ccc = false;
      direct_mode_ccc = true;
      int i;
      for (i = 0; i < device_count_ccc; i++)
      {
        if ( ( (strcmp(check_serial, "") != 0) && strcmp(check_serial, serial_ccc[i]) == 0))
        {
          if (superclone_ccc)
          {
            sprintf (tempmessage_ccc, "%s", curlang_ccc[LANGDRIVEVISABLEOS]);
            message_error_ccc(tempmessage_ccc);
            print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGWARN], 0);
            clear_error_message_ccc();
            if ( !force_stupid_ccc )
            {
              return (-200);
            }
          }
          else
          {
            fprintf (stdout, "WARNING: This drive is also available to the OS!\n");
            fprintf (stdout, "That is too dangerous, and would likely result in a computer crash!\n");
            fprintf (stdout, "Please make sure to follow the instrutions on how to hide the drive!\n");
            if ( !force_stupid_ccc )
            {
              fprintf (stdout, "Now exiting...\n");
              return (-1);
            }
          }
          break;
        }
      }
    }
  }


  // if direct mode then setup io and backup some information
  if (direct_mode_ccc && !superclone_ccc)
  {
    int ret = connect_source_disk_ccc();
    if (ret != 0)
    {
      return ret;
    }
  }

  return (0);
}




int connect_source_disk_ccc(void)
{
  //fprintf (stdout, "connect_source_disk_ccc\n");    //debug
  // if direct mode then setup io and backup some information
  if (direct_mode_ccc)
  {
    if (ahci_mode_ccc)
    {
      if (superbyte_ccc[42] == 0x20)
      {
        if (enable_hba_test_on_connect_source_ccc)    // TODO this is debug for troubleshooting the hba and port
        {
          hba_test_ccc();
          return -1;
        }
        else
        {
          // ahci
          uint32_t page_mask, page_size;

          hba_mem_dev_ccc = open("/dev/mem", O_RDWR | O_SYNC);
          if(hba_mem_dev_ccc == -1)
          {
            fprintf (stderr, "unable to open /dev/mem\n");
            return (-1);
          }
          const uint32_t hba_mem_address = hba_base_address_ccc;
          const uint32_t hba_mem_size = 128;

          page_size = pagesize_ccc;
          hba_alloc_mem_size_ccc = (((hba_mem_size / page_size) + 1) * page_size);
          page_mask = (page_size - 1);

          hba_mem_pointer_ccc = mmap(NULL,
                                     hba_alloc_mem_size_ccc,
                                     PROT_READ | PROT_WRITE,
                                     MAP_SHARED,
                                     hba_mem_dev_ccc,
                                     (hba_mem_address & ~page_mask)
          );

          if(hba_mem_pointer_ccc == MAP_FAILED)
          {
            fprintf (stderr, "HBA mem map failed\n");
            return (-1);
          }
          hba_virt_addr_ccc = (hba_mem_pointer_ccc + (hba_mem_address & page_mask));
          memory_mapped_ccc = true;


          port_mem_dev_ccc = open("/dev/mem", O_RDWR | O_SYNC);
          if(port_mem_dev_ccc == -1)
          {
            fprintf (stderr, "unable to open /dev/mem\n");
            return (-1);
          }
          const uint32_t port_mem_address = port_base_address_ccc;
          const uint32_t port_mem_size = 128;

          page_size = pagesize_ccc;
          port_alloc_mem_size_ccc = (((port_mem_size / page_size) + 1) * page_size);
          page_mask = (page_size - 1);

          port_mem_pointer_ccc = mmap(NULL,
                                      port_alloc_mem_size_ccc,
                                      PROT_READ | PROT_WRITE,
                                      MAP_SHARED,
                                      port_mem_dev_ccc,
                                      (port_mem_address & ~page_mask)
          );

          if(port_mem_pointer_ccc == MAP_FAILED)
          {
            fprintf (stderr, "Port mem map failed\n");
            return (-1);
          }
          port_virt_addr_ccc = (port_mem_pointer_ccc + (port_mem_address & page_mask));
          memory_mapped_ccc = true;

          int i;
          if (verbose_ccc & DEBUG39)
          {
            fprintf (stdout, "hba data, address=%08llx\n", hba_base_address_ccc);
            for (i = 0; i < 0x80; i+=4)
            {
              memcpy(&io_doubleword_ccc, hba_virt_addr_ccc + i, 4);
              fprintf (stdout, "%x %08x ", i, io_doubleword_ccc);  //debug
              int n;
              for (n = 0; n < 32; n++)
              {
                if ( !(n % 8) )
                {
                  fprintf (stdout, " ");
                }
                if ((io_doubleword_ccc<<n) & 0x80000000)
                  fprintf (stdout, "1");
                else
                  fprintf (stdout, "0");
              }
              fprintf (stdout, "\n");
            }
          }
          if (verbose_ccc & DEBUG39)
          {
            fprintf (stdout, "port data, address=%08llx\n", port_base_address_ccc);
            for (i = 0; i < 0x80; i+=4)
            {
              memcpy(&io_doubleword_ccc, port_virt_addr_ccc + i, 4);
              fprintf (stdout, "%x %08x ", i, io_doubleword_ccc);  //debug
              int n;
              for (n = 0; n < 32; n++)
              {
                if ( !(n % 8) )
                {
                  fprintf (stdout, " ");
                }
                if ((io_doubleword_ccc<<n) & 0x80000000)
                  fprintf (stdout, "1");
                else
                  fprintf (stdout, "0");
              }
              fprintf (stdout, "\n");
            }
          }
          if (debug_ccc & DEBUG39)
          {
            fprintf (debug_file_ccc, "hba data, address=%08llx\n", hba_base_address_ccc);
            for (i = 0; i < 0x80; i+=4)
            {
              memcpy(&io_doubleword_ccc, hba_virt_addr_ccc + i, 4);
              fprintf (debug_file_ccc, "%x %08x ", i, io_doubleword_ccc);  //debug
              int n;
              for (n = 0; n < 32; n++)
              {
                if ( !(n % 8) )
                {
                  fprintf (debug_file_ccc, " ");
                }
                if ((io_doubleword_ccc<<n) & 0x80000000)
                  fprintf (debug_file_ccc, "1");
                else
                  fprintf (debug_file_ccc, "0");
              }
              fprintf (debug_file_ccc, "\n");
            }
          }
          if (debug_ccc & DEBUG39)
          {
            fprintf (debug_file_ccc, "port data, address=%08llx\n", port_base_address_ccc);
            for (i = 0; i < 0x80; i+=4)
            {
              memcpy(&io_doubleword_ccc, port_virt_addr_ccc + i, 4);
              fprintf (debug_file_ccc, "%x %08x ", i, io_doubleword_ccc);  //debug
              int n;
              for (n = 0; n < 32; n++)
              {
                if ( !(n % 8) )
                {
                  fprintf (debug_file_ccc, " ");
                }
                if ((io_doubleword_ccc<<n) & 0x80000000)
                  fprintf (debug_file_ccc, "1");
                else
                  fprintf (debug_file_ccc, "0");
              }
              fprintf (debug_file_ccc, "\n");
            }
          }

          // backup current addresses
          memcpy(&command_list_address_backup_ccc, port_virt_addr_ccc + superbyte_ccc[11], 8);    //potential superbyte
          memcpy(&fis_address_backup_ccc, port_virt_addr_ccc + superbyte_ccc[12], 8);    //potential superbyte

          // backup the interrupt settings
          memcpy(&interrupt_backup_ccc, port_virt_addr_ccc + superbyte_ccc[13], 4);    //potential superbyte


          // turn interupts off
          ahci_interrupt_changed_ccc = true;
          memset(port_virt_addr_ccc + superbyte_ccc[13], 0x0, 4);    //potential superbyte

          // wait for 1ms
          do_nanosleep_ccc(1000000);

          // clear the start bit
          disable_start_ccc(START_BIT_TIME);

          // wait for 1ms
          do_nanosleep_ccc(1000000);

          // clear the fis enable bit
          disable_fis_ccc(FIS_BIT_TIME);

          // wait for 1ms
          do_nanosleep_ccc(1000000);

          // set new addresses
          ahci_address_changed_ccc = true;
          uint64_t command_list_address = command_list_physical_address_ccc;
          memcpy(port_virt_addr_ccc + superbyte_ccc[11], &command_list_address, 8);    //potential superbyte
          uint64_t fis_address = fis_physical_address_ccc;
          memcpy(port_virt_addr_ccc + superbyte_ccc[12], &fis_address, 8);    //potential superbyte

          // wait for 1ms
          do_nanosleep_ccc(1000000);

          // set the fis enable bit
          enable_fis_ccc(FIS_BIT_TIME);

          // wait for 1ms
          do_nanosleep_ccc(1000000);

          if (use_rebuild_assist_ccc)
          {
            if (set_rebuild_assist_enabled_ccc())
            {
              fprintf (stderr, "Enable rebuild assist failed\n");
            }
          }
        }
      }
    }

    else
    {
      // ide
      ioperm (reg_base_address_ccc, 8, 1);
      ioperm (control_base_address_ccc, 1, 1);
      ioperm (bus_base_address_ccc, 8, 1);
      io_permissions_ccc = true;

      // turn interrupts off, but make sure correct device first
      set_device_ccc(reg_base_address_ccc, device_select_base_ccc);
      outb(2, control_base_address_ccc);

      // put table address into controler
      set_table_address_ccc();
    }
  }
  else if (usb_mode_ccc)
  {
    int ret = connect_usbd1_ccc();
    if (ret != 0)
    {
      return -1;
    }
    read_capacity_10_ccc();    // do read capacity commands to clear possible unit attention
    read_capacity_10_ccc();
  }
  // if passthrough and gui then open source
  else if (superclone_ccc)
  {
    if (open_source_disk_ccc())
    {
      return -1;
    }
  }

  return (0);
}





int choose_target_ccc(void)
{
  bool direct_mode_bak = direct_mode_ccc;
  bool ahci_mode_bak = ahci_mode_ccc;
  bool ata_passthrough_bak = ata_passthrough_ccc;
  bool scsi_passthrough_bak = scsi_passthrough_ccc;

  direct_mode_ccc = false;
  ahci_mode_ccc = false;
  ata_passthrough_ccc = false;
  scsi_passthrough_ccc = !generic_mode_ccc;
  find_all_devices_ccc();
  direct_mode_ccc = direct_mode_bak;
  ahci_mode_ccc = ahci_mode_bak;
  ata_passthrough_ccc = ata_passthrough_bak;
  scsi_passthrough_ccc = scsi_passthrough_bak;

  strcpy (current_destination_model_ccc, "");
  strcpy (current_destination_serial_ccc, "");

  // passthrough
  char input_text[256];
  char raw_value[256];
  int choice = -1;
  int keeptrying = 1;

  if (!superclone_ccc)
  {
    while (keeptrying)
    {
      strcpy(raw_value, "");
      int i;
      fprintf (stdout, "\nQ) Quit\n");
      fprintf (stdout, "R) Refresh drive list\n");
      fprintf (stdout, "I) Image file\n");
      fprintf (stdout, "N) /dev/null\n");
      for (i = 0; i < device_count_ccc; i++)
      {
        fprintf (stdout, "%d) %s %s %s\n", i+1, drive_list_ccc[i], model_ccc[i], serial_ccc[i]);
      }

      fprintf (stdout, "WARNING: ALL DATA ON THE TARGET WILL BE DESTROYED!\n");
      fprintf (stdout, "Choose target (destination) drive > ");
      fflush(stdout);

      if ( fgets(input_text, sizeof input_text, stdin) != NULL )
      {
        sscanf(input_text, "%s", raw_value);
        if (strcasecmp(raw_value, "Q") == 0)
        {
          return (-1);
        }
        else if (strcasecmp(raw_value, "R") == 0)
        {
          direct_mode_ccc = false;
          ahci_mode_ccc = false;
          ata_passthrough_ccc = false;
          scsi_passthrough_ccc = true;
          find_all_devices_ccc();
          direct_mode_ccc = direct_mode_bak;
          ahci_mode_ccc = ahci_mode_bak;
          ata_passthrough_ccc = ata_passthrough_bak;
          scsi_passthrough_ccc = scsi_passthrough_bak;
        }
        else if (strcasecmp(raw_value, "N") == 0)
        {
          disk_2_ccc = "/dev/null";
          break;
        }
        else if (strcasecmp(raw_value, "I") == 0)
        {
          fprintf (stdout, "Enter image file > ");
          fflush(stdout);
          if ( fgets(input_text, sizeof input_text, stdin) != NULL )
          {
            disk_2_ccc = malloc (sizeof input_text);
            memset (disk_2_ccc, 0, sizeof input_text);
            sscanf(input_text, "%[^\n]", disk_2_ccc);
          }
          break;
        }
        else
        {
          char* endptr;
          choice = strtol(raw_value, &endptr, 0);
          if (*endptr)
          {
            fprintf (stderr, "Error! Choice \'%s\' was unable to processed.\n", raw_value);
          }
          else if (choice < 1 || choice > device_count_ccc)
          {
            fprintf (stderr, "Error! Choice %d is out of range.\n", choice);
          }
          else
          {
            keeptrying = 0;
          }
        }
      }
    }
  }
  else
  {
    choice = new_destination_ccc + 1;
  }

  int i = choice - 1;
  if ((verbose_ccc & DEBUG6) && choice > 0)
  {
    fprintf (stdout, "choice=%d %s %s %s\n", i+1, drive_list_ccc[i], model_ccc[i], serial_ccc[i]);
  }

  if (choice > 0)
  {
    free (disk_2_ccc);
    disk_2_ccc = malloc (sizeof drive_list_ccc[i]);
    memcpy (disk_2_ccc, drive_list_ccc[i], sizeof drive_list_ccc[i]);
    strcpy (current_destination_model_ccc, model_ccc[i]);
    strcpy (current_destination_serial_ccc, serial_ccc[i]);
  }

  return (0);
}





int get_device_information_ccc(char *driver, char *bus, int bus_count, int device, int current_device_count)
{
  // check to see if the device is available to the OS
  strcpy (device_reference_ccc[current_device_count], "---");
  strcpy (device_name_ccc[current_device_count], "---");
  char command[255];
  strcpy (command, "ls -p /sys/bus/pci/drivers/");
  strcat (command, driver);
  strcat (command, "/");
  strcat (command, bus);
  strcat (command, " |grep '/' | grep ata | sort -n -k1.4");

  char check_location[255];
  int found_location = 0;
  FILE *fp = popen(command, "r");
  int cols = 255;
  int i;
  char line[cols];
  i = 0;
  while (fgets(line, sizeof line, fp))
  {
    int x;
    for (x = 0; x < cols; x++)
    {
      if (line[x] == '\n')
      {
        line[x] = '\0';
        break;
      }
    }

    if (i == bus_count)
    {
      //fprintf (stdout, "%s\n", line);  //debug
      strcpy (check_location, line);
      int n;
      for (n = 0; n < 16; n++)
      {
        if (line[n] == '/')
        {
          break;
        }
        device_reference_ccc[current_device_count][n] = line[n];
      }
      device_reference_ccc[current_device_count][n] = '\0';
      found_location = 1;
      break;
    }

    i++;
    if (i >= MAX_DEVICES * 2)
    {
      break;
    }
  }
  pclose(fp);

  int host_found = 0;
  char check_host[255];
  if (found_location)
  {
    char command[255];
    strcpy (command, "ls -p /sys/bus/pci/drivers/");
    strcat (command, driver);
    strcat (command, "/");
    strcat (command, bus);
    strcat (command, "/");
    strcat (command, check_location);
    strcat (command, " | grep host");

    FILE *fp = popen(command, "r");
    int cols = 255;
    char line[cols];
    while (fgets(line, sizeof line, fp))
    {
      int x;
      for (x = 0; x < cols; x++)
      {
        if (line[x] == '\n')
        {
          line[x] = '\0';
          break;
        }
      }
      //fprintf (stdout, "%s\n", line);  //debug
      if (strcmp(line, "") != 0)
      {
        //fprintf (stdout, "found host\n");  //debug
        strcpy(check_host, line);
        host_found = 1;
      }
      break;
    }
    pclose(fp);
  }

  int target_found = 0;
  char check_target[255];
  if (host_found)
  {
    char command[255];
    strcpy (command, "ls -p /sys/bus/pci/drivers/");
    strcat (command, driver);
    strcat (command, "/");
    strcat (command, bus);
    strcat (command, "/");
    strcat (command, check_location);
    strcat (command, check_host);
    strcat (command, " | grep target");

    FILE *fp = popen(command, "r");
    int cols = 255;
    char line[cols];
    while (fgets(line, sizeof line, fp))
    {
      int x;
      for (x = 0; x < cols; x++)
      {
        if (line[x] == '\n')
        {
          line[x] = '\0';
          break;
        }
      }
      //fprintf (stdout, "%s\n", line);  //debug
      if (strcmp(line, "") != 0)
      {
        //fprintf (stdout, "found target %s\n", line);  //debug
        int n;
        for (n = 0; n < 16; n++)
        {
          if (line[n] == '\0')
          {
            break;
          }
        }
        char temp = line[n-2];
        int device_compare = strtol(&temp, NULL, 0);
        if (device == device_compare)
        {
          //fprintf (stdout, "found target %s %s %c %d %d \n", check_location, line, temp, device, device_compare);  //debug
          strcpy (check_target, line);
          strcat (device_reference_ccc[current_device_count], ".0");
          strncat (device_reference_ccc[current_device_count], &temp, 1);
          device_visable_ccc[current_device_count] = true;
          target_found = 1;
          break;
        }
      }
    }
    pclose(fp);
  }
  if (!target_found)
  {
    strcat (device_reference_ccc[current_device_count], "   ");
  }

  int zeros_found = 0;
  char check_zeros[255];
  if (target_found)
  {
    char command[255];
    strcpy (command, "ls -p /sys/bus/pci/drivers/");
    strcat (command, driver);
    strcat (command, "/");
    strcat (command, bus);
    strcat (command, "/");
    strcat (command, check_location);
    strcat (command, check_host);
    strcat (command, check_target);
    strcat (command, " | grep :");

    FILE *fp = popen(command, "r");
    int cols = 255;
    char line[cols];
    while (fgets(line, sizeof line, fp))
    {
      int x;
      for (x = 0; x < cols; x++)
      {
        if (line[x] == '\n')
        {
          line[x] = '\0';
          break;
        }
      }
      //fprintf (stdout, "%s\n", line);  //debug
      if (strcmp(line, "") != 0)
      {
        //fprintf (stdout, "found zeros\n");  //debug
        strcpy(check_zeros, line);
        zeros_found = 1;
      }
      break;
    }
    pclose(fp);
  }

  if (zeros_found)
  {
    char command[255];
    strcpy (command, "ls /sys/bus/pci/drivers/");
    strcat (command, driver);
    strcat (command, "/");
    strcat (command, bus);
    strcat (command, "/");
    strcat (command, check_location);
    strcat (command, check_host);
    strcat (command, check_target);
    strcat (command, check_zeros);
    strcat (command, "block");

    FILE *fp = popen(command, "r");
    int cols = 255;
    char line[cols];
    while (fgets(line, sizeof line, fp))
    {
      int x;
      for (x = 0; x < cols; x++)
      {
        if (line[x] == '\n')
        {
          line[x] = '\0';
          break;
        }
      }
      //fprintf (stdout, "%s\n", line);  //debug
      if (strcmp(line, "") != 0)
      {
        //fprintf (stdout, "found bkock\n");  //debug
        strcpy(device_name_ccc[current_device_count], line);
      }
      break;
    }
    pclose(fp);
  }






  return (0);
}






int wait_not_busy_ccc(unsigned long long time)
{
  // wait for drive to be ready
  int timeout = 0;
  int busy = 1;
  unsigned long long start_time = get_elapsed_usec_ccc();
  while (busy)
  {
    unsigned long long elapsed_time = get_elapsed_usec_ccc();
    if (elapsed_time > start_time + time)
    {
      // if it times out then fail
      timeout = 1;
      break;
    }
    if (elapsed_time > start_time + general_timeout_ccc)
    {
      // if it exceeds general timeout then quit
      timeout = 2;
      break;
    }
    io_byte_ccc[1] = inb(reg_base_address_ccc+1);
    io_byte_ccc[7] = inb(reg_base_address_ccc+7);    //status
    //io_byte_ccc[8] = inb(control_base_address_ccc);    //altstatus
    memcpy(&ata_error_ccc, &io_byte_ccc[1], 1);
    memcpy(&ata_status_ccc, &io_byte_ccc[7], 1);    //status
    //memcpy(&ata_status_ccc, &io_byte_ccc[8], 1);    //altstatus
    //fprintf (stdout, "%02x ", io_byte_ccc[7]);
    //fprintf (stdout, "\r");
    busy = io_byte_ccc[7] & 0x80;    //status
    //busy = io_byte_ccc[8] & 0x80;    //altstatus
    // give the cpu a chance to do something else so we are not using 100%
    do_nanosleep_ccc(1);
    update_display_status_buttons_ccc(DISPLAY_STATUS_UPDATE_TIME);
    if (stop_signal_ccc)
    {
      break;
    }
  }
  //fprintf (stdout, "\n");
  return (timeout);
}






int wait_not_busy_or_drq_ccc(unsigned long long time, int check)
{
  if (!dont_wait_for_drdy_ccc)
  {
    return (wait_drdy_not_busy_or_drq_ccc(time, check));
  }
  // wait for drive to be ready
  int timeout = 0;
  int busy = 1;
  int drq = 1;
  int err = 0;
  int ncq = 0;
  long long busycount = 0;
  long long drqcount = 0;
  long long errcount = 0;
  long long ncqcount = 0;
  long long dscount = 0;
  if (use_fpdma_ccc && wait_for_ds_bit_ccc && !performing_reset_ccc)
  {
    ncq = 1;
  }
  unsigned long long start_time = get_elapsed_usec_ccc();
  while (busy || drq || ncq)
  {
    if (direct_mode_ccc && check)
    {
      if ( check_for_unwanted_changes_ccc() )
      {
        timeout = 8;
        break;
      }
    }
    unsigned long long elapsed_time = get_elapsed_usec_ccc();
    if (elapsed_time > start_time + time)
    {
      // if it times out then fail
      timeout = 1;
      break;
    }
    if (elapsed_time > start_time + general_timeout_ccc)
    {
      // if it exceeds general timeout then quit
      timeout = 2;
      break;
    }
    if (ahci_mode_ccc)
    {
      if (superbyte_ccc[43] == 0x3d)
      {
        // ahci
        memcpy(&io_doubleword_ccc, port_virt_addr_ccc + superbyte_ccc[8], 4);
        //fprintf (stdout, "%08x\n", io_doubleword_ccc);  //debug
        busy = io_doubleword_ccc & 0x80;
        drq = io_doubleword_ccc & 0x08;
        err = io_doubleword_ccc & 0x01;
        unsigned char current_status = io_doubleword_ccc;
        unsigned char current_error = io_doubleword_ccc >> 8;
        memcpy(&ata_error_ccc, &current_error, 1);
        memcpy(&ata_status_ccc, &current_status, 1);
        memcpy(&io_singlebyte_ccc, port_virt_addr_ccc + superbyte_ccc[69], 1);    //potential superbyte
        if (io_singlebyte_ccc)
        {
          dscount++;
        }
        if (busy)
        {
          busycount++;
        }
        if (drq)
        {
          drqcount++;
        }
        if (err)
        {
          errcount++;
        }
        if (use_fpdma_ccc && wait_for_ds_bit_ccc && !performing_reset_ccc)
        {
          ncq = io_singlebyte_ccc & 1;
          if (ncq)
          {
            ncqcount++;
          }
          if (err)
          {
            //timeout = 4;
            break;
          }
        }
        //fprintf (stdout, "busy=%d drq=%d err=%d ncq=%d\n", busy, drq, err, ncq);
      }
    }
    else
    {
      // ide
      io_byte_ccc[1] = inb(reg_base_address_ccc+1);
      io_byte_ccc[7] = inb(reg_base_address_ccc+7);    //status
      //io_byte_ccc[8] = inb(control_base_address_ccc);    //altstatus
      memcpy(&ata_error_ccc, &io_byte_ccc[1], 1);
      memcpy(&ata_status_ccc, &io_byte_ccc[7], 1);    //status
      //memcpy(&ata_status_ccc, &io_byte_ccc[8], 1);    //altstatus
      //fprintf (stdout, "%02x ", io_byte_ccc[7]);
      //fprintf (stdout, "\r");
      busy = io_byte_ccc[7] & 0x80;    //status
      drq = io_byte_ccc[7] & 0x08;    //status
      //busy = io_byte_ccc[8] & 0x80;    //altstatus
      //drq = io_byte_ccc[8] & 0x08;    //altstatus
    }

    // give the cpu a chance to do something else so we are not using 100%
    do_nanosleep_ccc(1);
    update_display_status_buttons_ccc(DISPLAY_STATUS_UPDATE_TIME);
    if (stop_signal_ccc)
    {
      break;
    }
  }
  //fprintf (stdout, "busycount=%lld drqcount=%lld errcount=%lld ncqcount=%lld dscount=%lld error=0x%02x status=0x%02x\n", busycount, drqcount, errcount, ncqcount, dscount, ata_error_ccc, ata_status_ccc);  //debug
  //fprintf (stdout, "\n");
  return (timeout);
}





int wait_drdy_not_busy_or_drq_ccc(unsigned long long time, int check)
{
  // wait for drive to be ready
  int timeout = 0;
  int busy = 1;
  int drdy = 0;
  int drq = 1;
  int err = 0;
  int ncq = 0;
  long long busycount = 0;
  long long drdycount = 0;
  long long drqcount = 0;
  long long errcount = 0;
  long long ncqcount = 0;
  long long dscount = 0;
  if (use_fpdma_ccc && wait_for_ds_bit_ccc && !performing_reset_ccc)
  {
    ncq = 1;
  }
  unsigned long long start_time = get_elapsed_usec_ccc();
  while (busy || drq || !drdy || ncq)
  {
    if (direct_mode_ccc && check)
    {
      if ( check_for_unwanted_changes_ccc() )
      {
        timeout = 8;
        break;
      }
    }
    unsigned long long elapsed_time = get_elapsed_usec_ccc();
    if (elapsed_time > start_time + time)
    {
      // if it times out then fail
      timeout = 1;
      break;
    }
    if (elapsed_time > start_time + general_timeout_ccc)
    {
      // if it exceeds general timeout then quit
      timeout = 2;
      break;
    }
    if (ahci_mode_ccc)
    {
      if (superbyte_ccc[43] == 0x3d)
      {
        // ahci
        memcpy(&io_doubleword_ccc, port_virt_addr_ccc + superbyte_ccc[8], 4);
        //fprintf (stdout, "%08x\n", io_doubleword_ccc);  //debug
        busy = io_doubleword_ccc & 0x80;
        drdy = io_doubleword_ccc & 0x40;
        drq = io_doubleword_ccc & 0x08;
        err = io_doubleword_ccc & 0x01;
        unsigned char current_status = io_doubleword_ccc;
        unsigned char current_error = io_doubleword_ccc >> 8;
        memcpy(&ata_error_ccc, &current_error, 1);
        memcpy(&ata_status_ccc, &current_status, 1);
        memcpy(&io_singlebyte_ccc, port_virt_addr_ccc + superbyte_ccc[69], 1);    //potential superbyte
        if (io_singlebyte_ccc)
        {
          dscount++;
        }
        if (busy)
        {
          busycount++;
        }
        if (!drdy)
        {
          drdycount++;
        }
        if (drq)
        {
          drqcount++;
        }
        if (err)
        {
          errcount++;
        }
        if (use_fpdma_ccc && wait_for_ds_bit_ccc && !performing_reset_ccc)
        {
          ncq = io_singlebyte_ccc & 1;
          if (ncq)
          {
            ncqcount++;
          }
          if (err)
          {
            //timeout = 4;
            break;
          }
        }
        //fprintf (stdout, "busy=%d drdy=%d drq=%d err=%d ncq=%d\n", busy, drdy, drq, err, ncq);
      }
    }
    else
    {
      // ide
      io_byte_ccc[1] = inb(reg_base_address_ccc+1);
      io_byte_ccc[7] = inb(reg_base_address_ccc+7);    //status
      //io_byte_ccc[8] = inb(control_base_address_ccc);    //altstatus
      memcpy(&ata_error_ccc, &io_byte_ccc[1], 1);
      memcpy(&ata_status_ccc, &io_byte_ccc[7], 1);    //status
      //memcpy(&ata_status_ccc, &io_byte_ccc[8], 1);    //altstatus
      //fprintf (stdout, "%02x ", io_byte_ccc[7]);
      //fprintf (stdout, "\r");
      busy = io_byte_ccc[7] & 0x80;    //status
      drdy = io_byte_ccc[7] & 0x40;    //status
      drq = io_byte_ccc[7] & 0x08;    //status
      //busy = io_byte_ccc[8] & 0x80;    //altstatus
      //drq = io_byte_ccc[8] & 0x08;    //altstatus
    }

    // give the cpu a chance to do something else so we are not using 100%
    do_nanosleep_ccc(1);
    update_display_status_buttons_ccc(DISPLAY_STATUS_UPDATE_TIME);
    if (stop_signal_ccc)
    {
      break;
    }
  }
  //fprintf (stdout, "busycount=%lld drqcount=%lld errcount=%lld ncqcount=%lld dscount=%lld error=0x%02x status=0x%02x\n", busycount, drqcount, errcount, ncqcount, dscount, ata_error_ccc, ata_status_ccc);  //debug
  //fprintf (stdout, "\n");
  return (timeout);
}





int set_device_ccc(unsigned long long address, int device)
{
  // set device
  io_byte_ccc[6] = device << 4;
  outb(io_byte_ccc[6], address+6);
  do_nanosleep_ccc(400);
  return (0);
}






int set_and_send_regs_ccc(int command_type)
{
  // leave this here for reference even though it does not do anything
  if (0 && superbyte_ccc[44] == 0x88 && passthrough_ccc.direction == 1)
  {
    // check serial byte 9 at random
  }

  // ahci
  if (ahci_mode_ccc)
  {
    if (superbyte_ccc[45] == 0x33)
    {
      // ahci
      // clear the fis buffer
      memset(fis_buffer_ccc, 0, fis_size_ccc);
      // clear the command FIS
      memset(table_buffer_ccc, 0, 0x80);
      // set FIS type
      memset(table_buffer_ccc, REG_H2D, 1);
      // set type as command
      memset(table_buffer_ccc+1, 0x80, 1);
      // set the table entry length
      uint16_t word = table_entry_count_ccc;
      memcpy(command_list_buffer_ccc+2, &word, 2);
      // set the command table address
      uint64_t qword = table_physical_address_ccc;
      memcpy(command_list_buffer_ccc+8, &qword, 8);

      if (enable_data_dump_ccc)
      {
        // set the control register value (something unknown)
        memset(table_buffer_ccc+15, 0x08, 1);
      }

      if (command_type == 28)
      {
        // set command
        memset(table_buffer_ccc+2, passthrough_ccc.ata_cmd[6], 1);    // command
        memset(table_buffer_ccc+3, passthrough_ccc.ata_cmd[0], 1);    // feature
        memset(table_buffer_ccc+4, passthrough_ccc.ata_cmd[2], 1);    // lba low
        memset(table_buffer_ccc+5, passthrough_ccc.ata_cmd[3], 1);    // lba mid
        memset(table_buffer_ccc+6, passthrough_ccc.ata_cmd[4], 1);    // lba high
        passthrough_ccc.ata_cmd[5] = passthrough_ccc.ata_cmd[5] & 0xef;    // strip out the device select bit
        passthrough_ccc.ata_cmd[5] = passthrough_ccc.ata_cmd[5] | (device_select_base_ccc << 4);    // set proper select bit
        memset(table_buffer_ccc+7, passthrough_ccc.ata_cmd[5], 1);    // device
        memset(table_buffer_ccc+12, passthrough_ccc.ata_cmd[1], 1);    // count
      }

      else if (command_type == 48)
      {
        // set command
        memset(table_buffer_ccc+2, passthrough_ccc.ata_cmd[11], 1);    // command
        memset(table_buffer_ccc+3, passthrough_ccc.ata_cmd[1], 1);    // feature low
        memset(table_buffer_ccc+4, passthrough_ccc.ata_cmd[5], 1);    // LBA LL 7:0
        memset(table_buffer_ccc+5, passthrough_ccc.ata_cmd[7], 1);    // LBA LH 15:8
        memset(table_buffer_ccc+6, passthrough_ccc.ata_cmd[9], 1);    // LBA ML 23:16
        passthrough_ccc.ata_cmd[10] = passthrough_ccc.ata_cmd[10] & 0xef;    // strip out the device select bit
        passthrough_ccc.ata_cmd[10] = passthrough_ccc.ata_cmd[10] | (device_select_base_ccc << 4);    // set proper select bit
        memset(table_buffer_ccc+7, passthrough_ccc.ata_cmd[10], 1);    // device
        memset(table_buffer_ccc+8, passthrough_ccc.ata_cmd[4], 1);    // LBA MH 31:24
        memset(table_buffer_ccc+9, passthrough_ccc.ata_cmd[6], 1);    // LBA HL 39:32
        memset(table_buffer_ccc+10, passthrough_ccc.ata_cmd[8], 1);    // LBA HH 47:40
        memset(table_buffer_ccc+11, passthrough_ccc.ata_cmd[0], 1);    // feature high
        memset(table_buffer_ccc+12, passthrough_ccc.ata_cmd[3], 1);    // sector count low
        memset(table_buffer_ccc+13, passthrough_ccc.ata_cmd[2], 1);    // sector count high
      }

      else
      {
        sprintf (tempmessage_ccc, "\n\nInternal error, send regs command type not valid\n\n");
        if (superclone_ccc)
        {
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
          clear_error_message_ccc();
        }
        else
        {
          message_now_ccc(tempmessage_ccc);
          message_exit_ccc(tempmessage_ccc);
          exitcode_ccc = INTERNAL_ERROR_EXIT_CODE;
        }
        return -1;
      }


      #ifdef DEBUG
      if (debug_ccc & DEBUG36)
      {
        int i;
        for (i = 0; i < 256; i+=16)
        {
          fprintf (stdout, "%x: ", i);
          unsigned char *c;
          int n;
          for (n=0; n < 16 && i+n < 512; n++)
          {
            c = (unsigned char *)table_buffer_ccc+i+n;
            fprintf (stdout, "%02x ", *c);
          }
          fprintf (stdout, "   ");
          for (n=0; n < 16 && i+n < 512; n++)
          {
            c = (unsigned char *)table_buffer_ccc+i+n;
            fprintf (stdout, "%c", isprint(*c) ? *c : '.');
          }
          fprintf (stdout, "\n");
        }
      }

      if (debug_ccc & DEBUG36)
      {
        int i;
        for (i = 0; i < 256; i+=16)
        {
          fprintf (stdout, "%x: ", i);
          unsigned char *c;
          int n;
          for (n=0; n < 16 && i+n < 512; n++)
          {
            c = (unsigned char *)fis_buffer_ccc+i+n;
            fprintf (stdout, "%02x ", *c);
          }
          fprintf (stdout, "   ");
          for (n=0; n < 16 && i+n < 512; n++)
          {
            c = (unsigned char *)fis_buffer_ccc+i+n;
            fprintf (stdout, "%c", isprint(*c) ? *c : '.');
          }
          fprintf (stdout, "\n");
        }
      }

      if (debug_ccc & DEBUG36)
      {
        int i;
        for (i = 0; i < 128; i+=16)
        {
          fprintf (stdout, "%x: ", i);
          unsigned char *c;
          int n;
          for (n=0; n < 16 && i+n < 512; n++)
          {
            c = (unsigned char *)command_list_buffer_ccc+i+n;
            fprintf (stdout, "%02x ", *c);
          }
          fprintf (stdout, "   ");
          for (n=0; n < 16 && i+n < 512; n++)
          {
            c = (unsigned char *)command_list_buffer_ccc+i+n;
            fprintf (stdout, "%c", isprint(*c) ? *c : '.');
          }
          fprintf (stdout, "\n");
        }
      }
      #endif
    }
  }


  else
  {
    // ide
    if (command_type == 28)
    {
      // set command
      io_byte_ccc[1] = passthrough_ccc.ata_cmd[0];    // feature
      io_byte_ccc[2] = passthrough_ccc.ata_cmd[1];    // count
      io_byte_ccc[3] = passthrough_ccc.ata_cmd[2];    // lba low
      io_byte_ccc[4] = passthrough_ccc.ata_cmd[3];    // lba mid
      io_byte_ccc[5] = passthrough_ccc.ata_cmd[4];    // lba high
      passthrough_ccc.ata_cmd[5] = passthrough_ccc.ata_cmd[5] & 0xef;    // strip out the device select bit
      io_byte_ccc[6] = passthrough_ccc.ata_cmd[5] | (device_select_base_ccc << 4);    // device with proper select bit
      io_byte_ccc[7] = passthrough_ccc.ata_cmd[6];    // command

      //fprintf (stdout, "send28= %02x %02x %02x %02x %02x %02x %02x\n", io_byte_ccc[1], io_byte_ccc[2], io_byte_ccc[3], io_byte_ccc[4], io_byte_ccc[5], io_byte_ccc[6], io_byte_ccc[7]);

      // send command
      outb(io_byte_ccc[6], reg_base_address_ccc+6);    // device, supposed to do this first even though we already set the device
      outb(io_byte_ccc[1], reg_base_address_ccc+1);    // feature
      outb(io_byte_ccc[2], reg_base_address_ccc+2);    // count
      outb(io_byte_ccc[3], reg_base_address_ccc+3);    // lba low
      outb(io_byte_ccc[4], reg_base_address_ccc+4);    // lba mid
      outb(io_byte_ccc[5], reg_base_address_ccc+5);    // lba high
      outb(io_byte_ccc[7], reg_base_address_ccc+7);    // command, must be last
    }

    else if (command_type == 48)
    {
      // set command
      io_byte_ccc[1] = passthrough_ccc.ata_cmd[0];    // feature high
      io_byte_ccc[2] = passthrough_ccc.ata_cmd[1];    // feature low
      io_byte_ccc[3] = passthrough_ccc.ata_cmd[2];    // sector count high
      io_byte_ccc[4] = passthrough_ccc.ata_cmd[3];    // sector count low
      io_byte_ccc[5] = passthrough_ccc.ata_cmd[4];    // LBA MH 31:24
      io_byte_ccc[6] = passthrough_ccc.ata_cmd[5];    // LBA LL 7:0
      io_byte_ccc[7] = passthrough_ccc.ata_cmd[6];    // LBA HL 39:32
      io_byte_ccc[8] = passthrough_ccc.ata_cmd[7];    // LBA LH 15:8
      io_byte_ccc[9] = passthrough_ccc.ata_cmd[8];    // LBA HH 47:40
      io_byte_ccc[10] = passthrough_ccc.ata_cmd[9];    // LBA ML 23:16
      passthrough_ccc.ata_cmd[10] = passthrough_ccc.ata_cmd[10] & 0xef;    // strip out the device select bit
      io_byte_ccc[11] = passthrough_ccc.ata_cmd[10] | (device_select_base_ccc << 4);    // device with proper select bit
      io_byte_ccc[12] = passthrough_ccc.ata_cmd[11];   // Command

      //fprintf (stdout, "send48= %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", io_byte_ccc[1], io_byte_ccc[2], io_byte_ccc[3], io_byte_ccc[4], io_byte_ccc[5], io_byte_ccc[6], io_byte_ccc[7], io_byte_ccc[8], io_byte_ccc[9], io_byte_ccc[10], io_byte_ccc[11], io_byte_ccc[12]);

      // send command
      outb(io_byte_ccc[11], reg_base_address_ccc+6);    // device, supposed to do this first even though we already set the device
      outb(io_byte_ccc[1], reg_base_address_ccc+1);    // feature high
      outb(io_byte_ccc[3], reg_base_address_ccc+2);    // count high
      outb(io_byte_ccc[5], reg_base_address_ccc+3);    // lba4 31:24
      outb(io_byte_ccc[7], reg_base_address_ccc+4);    // lba5 39:32
      outb(io_byte_ccc[9], reg_base_address_ccc+5);    // lba6 47:40
      outb(io_byte_ccc[2], reg_base_address_ccc+1);    // feature low
      outb(io_byte_ccc[4], reg_base_address_ccc+2);    // count low
      outb(io_byte_ccc[6], reg_base_address_ccc+3);    // lba1 7:0
      outb(io_byte_ccc[8], reg_base_address_ccc+4);    // lba2 15:8
      outb(io_byte_ccc[10], reg_base_address_ccc+5);    // lba3 23:16
      outb(io_byte_ccc[12], reg_base_address_ccc+7);    // command, must be last
    }

    else
    {
      sprintf (tempmessage_ccc, "\n\nInternal error, send regs command type not valid\n\n");
      if (superclone_ccc)
      {
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
      }
      else
      {
        message_now_ccc(tempmessage_ccc);
        message_exit_ccc(tempmessage_ccc);
        exitcode_ccc = INTERNAL_ERROR_EXIT_CODE;
      }
      return -1;
    }
  }
  return (0);
}





int check_for_unwanted_changes_ccc(void)
{
  int changed_status = 0;

  if (ahci_mode_ccc)
  {
    if (superbyte_ccc[46] == 0x9b)
    {
      uint32_t wtf;
      // check that interrupts are still turned off
      memcpy(&wtf, port_virt_addr_ccc + superbyte_ccc[13], 4);    //potential superbyte
      if (wtf)
      {
        changed_status = changed_status + 0x0100;
      }

      // check that start bit and fis enable are still on
      memcpy(&wtf, port_virt_addr_ccc + superbyte_ccc[10], 4);     //potential superbyte
      if ( !(wtf & 0x01) )
      {
        changed_status = changed_status + 0x0200;
      }
      if ( !(wtf & 0x10) )
      {
        changed_status = changed_status + 0x0400;
      }

      // check addresses
      uint64_t wtf2;
      uint64_t wtf3;
      memcpy(&wtf2, port_virt_addr_ccc + superbyte_ccc[11], 8);    // command list address
      memcpy(&wtf3, port_virt_addr_ccc + superbyte_ccc[12], 8);    // fis address
      if (command_list_physical_address_ccc != wtf2)
      {
        changed_status = changed_status + 0x0800;
      }
      if (fis_physical_address_ccc != wtf3)
      {
        changed_status = changed_status + 0x1000;
      }

      #ifdef DEBUG
      if (changed_status && (debug_ccc & DEBUG38))
      {
        fprintf (stdout, "status changed\n");  //debug
        int i;
        for (i = 0; i < 0x80; i+=4)
        {
          break;                                           //debug
          memcpy(&io_doubleword_ccc, port_virt_addr_ccc + i, 4);
          fprintf (stdout, "%x %08x ", i, io_doubleword_ccc);  //debug
          int n;
          for (n = 0; n < 32; n++)
          {
            if ( !(n % 8) )
            {
              fprintf (stdout, " ");
            }
            if ((io_doubleword_ccc<<n) & 0x80000000)
              fprintf (stdout, "1");
            else
              fprintf (stdout, "0");
          }
          fprintf (stdout, "\n");
        }
      }
      #endif
    }
  }

  else
  {
    // ide
    unsigned char ta1 = inb(bus_base_address_ccc+0+superbyte_ccc[0]);    //potential superbyte
    unsigned char ta2 = inb(bus_base_address_ccc+1+superbyte_ccc[0]);    //potential superbyte
    unsigned char ta3 = inb(bus_base_address_ccc+2+superbyte_ccc[0]);    //potential superbyte
    unsigned char ta4 = inb(bus_base_address_ccc+3+superbyte_ccc[0]);    //potential superbyte
    unsigned char tpa1 = table_physical_address_ccc;
    unsigned char tpa2 = table_physical_address_ccc >> 8;
    unsigned char tpa3 = table_physical_address_ccc >> 16;
    unsigned char tpa4 = table_physical_address_ccc >> 24;
    if (ta1 != tpa1 || ta2 != tpa2 || ta3 != tpa3 || ta4 != tpa4)
    {
      changed_status = 0x0800;
    }

  }

  return (changed_status);
}





unsigned int get_ahci_error_ccc(void)
{
  if (ahci_mode_ccc)
  {
    if (superbyte_ccc[47] == 0x41)
    {
      uint32_t port_error;
      memcpy(&port_error, port_virt_addr_ccc + superbyte_ccc[18], 4);    // port error
      return port_error;
    }
  }
  return 0;
}





unsigned int get_ahci_status_ccc(void)
{
  if (ahci_mode_ccc)
  {
    if (superbyte_ccc[47] == 0x41)
    {
      uint64_t port_status;
      memcpy(&port_status, port_virt_addr_ccc + superbyte_ccc[18] + 4, 4);    // port status
      return port_status;
    }
  }
  return 0;
}





int disable_fis_ccc(unsigned long long time)
{
  int timeout = 0;
  if (superbyte_ccc[48] == 0x0a)
  {
    uint8_t byte;
    timeout = 0;
    // clear the fis enable bit
    memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[10], 1);    //potential superbyte
    byte = byte & 0xef;
    memcpy(port_virt_addr_ccc + superbyte_ccc[10], &byte, 1);

    // wait for fis running bit to clear
    memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[14], 1);
    byte = byte & 0x40;
    unsigned long long start_time = get_elapsed_usec_ccc();
    while (byte)
    {
      unsigned long long elapsed_time = get_elapsed_usec_ccc();
      if (elapsed_time > start_time + time)
      {
        // if it exceeds general timeout then quit
        timeout = 1;
        break;
      }
      memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[14], 1);
      byte = byte & 0x40;
      // give the cpu a chance to do something else so we are not using 100%
      do_nanosleep_ccc(1);
    }
  }
  else
  {
    time = time;
    timeout = 0;
  }
  return (timeout);
}





int enable_fis_ccc(unsigned long long time)
{
  int timeout = 0;
  if (superbyte_ccc[49] == 0xd9)
  {
    uint8_t byte;
    timeout = 0;
    // set the fis enable bit
    memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[10], 1);    //potential superbyte
    byte = byte | 0x10;
    memcpy(port_virt_addr_ccc + superbyte_ccc[10], &byte, 1);    //potential superbyte

    // wait for fis running bit to set
    memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[14], 1);    //potential superbyte
    byte = byte & 0x40;
    unsigned long long start_time = get_elapsed_usec_ccc();
    while (!byte)
    {
      unsigned long long elapsed_time = get_elapsed_usec_ccc();
      if (elapsed_time > start_time + time)
      {
        // if it exceeds general timeout then quit
        timeout = 1;
        break;
      }
      memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[14], 1);    //potential superbyte
      byte = byte & 0x40;
      // give the cpu a chance to do something else so we are not using 100%
      do_nanosleep_ccc(1);
    }
  }
  else
  {
    time = time;
    timeout = 0;
  }
  return (timeout);
}





int disable_start_ccc(unsigned long long time)
{
  int timeout = 0;
  if (superbyte_ccc[50] == 0x22)
  {
    uint8_t byte;
    timeout = 0;
    // clear the start bit
    memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[10], 1);    //potential superbyte
    byte = byte & 0xfe;
    memcpy(port_virt_addr_ccc + superbyte_ccc[10], &byte, 1);    //potential superbyte

    // wait for command list running bit to clear
    memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[14], 1);    //potential superbyte
    byte = byte & 0x80;
    unsigned long long start_time = get_elapsed_usec_ccc();
    while (byte)
    {
      unsigned long long elapsed_time = get_elapsed_usec_ccc();
      if (elapsed_time > start_time + time)
      {
        // if it exceeds general timeout then quit
        timeout = 1;
        break;
      }
      memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[14], 1);    //potential superbyte
      byte = byte & 0x80;
      // give the cpu a chance to do something else so we are not using 100%
      do_nanosleep_ccc(1);
    }
  }
  else
  {
    time = time;
    timeout = 0;
  }
  return (timeout);
}





int enable_start_ccc(unsigned long long time)
{
  int timeout = 0;
  if (superbyte_ccc[51] == 0x4b)
  {
    uint8_t byte;
    timeout = 0;
    // set the start bit
    memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[10], 1);    //potential superbyte
    byte = byte | 0x01;
    memcpy(port_virt_addr_ccc + superbyte_ccc[10], &byte, 1);    //potential superbyte

    // wait for command list running bit to set
    memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[14], 1);    //potential superbyte
    byte = byte & 0x80;
    unsigned long long start_time = get_elapsed_usec_ccc();
    while (!byte)
    {
      unsigned long long elapsed_time = get_elapsed_usec_ccc();
      if (elapsed_time > start_time + time)
      {
        // if it exceeds general timeout then quit
        timeout = 1;
        break;
      }
      memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[14], 1);    //potential superbyte
      byte = byte & 0x80;
      // give the cpu a chance to do something else so we are not using 100%
      do_nanosleep_ccc(1);
    }
  }
  else
  {
    time = time;
    timeout = 0;
  }
  return (timeout);
}





int enable_command_issue_ccc(unsigned long long time)
{
  int timeout = 0;
  if (superbyte_ccc[51] == 0x4b)
  {
    uint8_t byte;
    timeout = 0;
    // set the command issue bit
    io_doubleword_ccc = 1;
    memcpy(port_virt_addr_ccc + superbyte_ccc[6], &io_doubleword_ccc, 4);    //potential superbyte

    // wait for busy bit to set
    memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[8], 1);    //potential superbyte
    byte = byte & 0x80;
    unsigned long long start_time = get_elapsed_usec_ccc();
    while (!byte)
    {
      unsigned long long elapsed_time = get_elapsed_usec_ccc();
      if (elapsed_time > start_time + time)
      {
        // if it exceeds general timeout then quit
        timeout = 1;
        break;
      }
      memcpy(&byte, port_virt_addr_ccc + superbyte_ccc[14], 1);    //potential superbyte
      byte = byte & 0x80;
      // give the cpu a chance to do something else so we are not using 100%
      do_nanosleep_ccc(1);
    }
  }
  else
  {
    time = time;
    timeout = 0;
  }
  return (timeout);
}





int read_ncq_error_log_ccc (void)
{
  void* temp_buffer;
  temp_buffer = malloc(ccc_main_buffer_size_ccc);
  memcpy (temp_buffer, ccc_buffer_ccc, ccc_main_buffer_size_ccc);
  unsigned long long ccc_main_buffer_size_bak = ccc_main_buffer_size_ccc;
  ccc_main_buffer_size_ccc = 512;
  set_main_buffer_ccc();
  memset (ccc_buffer_ccc, 0, ccc_main_buffer_size_ccc);

  passthrough_ccc.transfer_length = 2;    // sector field
  passthrough_ccc.byte_block = 1;    // block
  passthrough_ccc.protocol = 4;    // pio
  passthrough_ccc.check_condition = 1;    // check condition true
  passthrough_ccc.direction = 2;    // from
  passthrough_ccc.transfer_direction = 1;    // from
  passthrough_ccc.sg_xfer_direction = SG_DXFER_FROM_DEV;

  passthrough_ccc.ata_cmd[0] = 0;        // feature high, 0
  passthrough_ccc.ata_cmd[1] = 0;      // feature low, 0
  passthrough_ccc.ata_cmd[2] = 0;        // sector count high, 0
  passthrough_ccc.ata_cmd[3] = 1;      // sector count low, 1 to read one page
  passthrough_ccc.ata_cmd[4] = 0;      // LBA4 31:24, 0
  passthrough_ccc.ata_cmd[5] = 0x10;     // LBA1 7:0, 0x10 ncq error log page
  passthrough_ccc.ata_cmd[6] = 0;      // LBA5 39:32, 0
  passthrough_ccc.ata_cmd[7] = 0;       // LBA2 15:8, 0
  passthrough_ccc.ata_cmd[8] = 0;      // LBA6 47:40, 0
  passthrough_ccc.ata_cmd[9] = 0;      // LBA3 23:16, 0
  passthrough_ccc.ata_cmd[10] = 0;    // Device, 0
  passthrough_ccc.ata_cmd[11] = 0x2f;    // Command read log pio ext
  command_length_ccc = 16;
  int ret = do_ata48_cmd_ccc(current_disk_ccc);

  if (0)
  {
    int i;
    for (i = 0; i < 512; i+=16)
    {
      fprintf (stdout, "%x: ", i);
      unsigned char *c;
      int n;
      for (n=0; n < 16 && i+n < 512; n++)
      {
        c = (unsigned char *)ccc_buffer_ccc+i+n;
        fprintf (stdout, "%02x ", *c);
      }
      fprintf (stdout, "   ");
      for (n=0; n < 16 && i+n < 512; n++)
      {
        c = (unsigned char *)ccc_buffer_ccc+i+n;
        fprintf (stdout, "%c", isprint(*c) ? *c : '.');
      }
      fprintf (stdout, "\n");
    }
  }
  //fprintf(stdout, "ncq log return= %d\n", ret);    //debug

  unsigned char ncq_buffer[512];
  memcpy (&ncq_buffer, ccc_buffer_ccc, 512);
  ncq_error_log_data_ccc.flags = ncq_buffer[0];
  ncq_error_log_data_ccc.status = ncq_buffer[2];
  ncq_error_log_data_ccc.error = ncq_buffer[3];
  ncq_error_log_data_ccc.device = ncq_buffer[7];
  ncq_error_log_data_ccc.sense_key = ncq_buffer[14];
  ncq_error_log_data_ccc.sense_asc = ncq_buffer[15];
  ncq_error_log_data_ccc.sense_ascq = ncq_buffer[16];
  ncq_error_log_data_ccc.count = ncq_buffer[12] + ((int)ncq_buffer[13] << 8);
  ncq_error_log_data_ccc.lba = ncq_buffer[4] + ((long long)ncq_buffer[5] << 8) + ((long long)ncq_buffer[6] << 16) + ((long long)ncq_buffer[8] << 24) + ((long long)ncq_buffer[9] << 32) + ((long long)ncq_buffer[10] << 40);
  ncq_error_log_data_ccc.final_lba = ncq_buffer[17] + ((long long)ncq_buffer[18] << 8) + ((long long)ncq_buffer[19] << 16) + ((long long)ncq_buffer[20] << 24) + ((long long)ncq_buffer[21] << 32) + ((long long)ncq_buffer[22] << 40);

  if (0)
  {
    fprintf (stdout, "flags=%02x, status=%02x, error=%02x, device=%02x, key=%02x, asc=%02x, ascq=%02x\n", ncq_error_log_data_ccc.flags, ncq_error_log_data_ccc.status, ncq_error_log_data_ccc.error, ncq_error_log_data_ccc.device, ncq_error_log_data_ccc.sense_key, ncq_error_log_data_ccc.sense_asc, ncq_error_log_data_ccc.sense_ascq);    //debug
    fprintf (stdout, "count=%04x, lba=%08llx, final=%08llx\n", ncq_error_log_data_ccc.count, ncq_error_log_data_ccc.lba, ncq_error_log_data_ccc.final_lba);    //debug
  }

  ccc_main_buffer_size_ccc = ccc_main_buffer_size_bak;
  set_main_buffer_ccc();
  memcpy (ccc_buffer_ccc, temp_buffer, ccc_main_buffer_size_ccc);
  free(temp_buffer);

  return ret;
}





int read_rebuild_assist_log_ccc (void)
{
  unsigned long long ccc_main_buffer_size_bak = ccc_main_buffer_size_ccc;
  ccc_main_buffer_size_ccc = LOG_PAGE_SIZE;
  set_main_buffer_ccc();
  memset (ccc_buffer_ccc, 0, ccc_main_buffer_size_ccc);

  passthrough_ccc.transfer_length = 2;    // sector field
  passthrough_ccc.byte_block = 1;    // block
  passthrough_ccc.protocol = 4;    // pio
  passthrough_ccc.check_condition = 1;    // check condition true
  passthrough_ccc.direction = 2;    // from
  passthrough_ccc.transfer_direction = 1;    // from
  passthrough_ccc.sg_xfer_direction = SG_DXFER_FROM_DEV;

  passthrough_ccc.ata_cmd[0] = 0;        // feature high, 0
  passthrough_ccc.ata_cmd[1] = 0;      // feature low, 0
  passthrough_ccc.ata_cmd[2] = 0;        // sector count high, 0
  passthrough_ccc.ata_cmd[3] = 1;      // sector count low, 1 to read one page
  passthrough_ccc.ata_cmd[4] = 0;      // LBA4 31:24, 0
  passthrough_ccc.ata_cmd[5] = 0x15;     // LBA1 7:0, 0x15 rebuild assist log page
  passthrough_ccc.ata_cmd[6] = 0;      // LBA5 39:32, 0
  passthrough_ccc.ata_cmd[7] = 0;       // LBA2 15:8, 0
  passthrough_ccc.ata_cmd[8] = 0;      // LBA6 47:40, 0
  passthrough_ccc.ata_cmd[9] = 0;      // LBA3 23:16, 0
  passthrough_ccc.ata_cmd[10] = 0;    // Device, 0
  passthrough_ccc.ata_cmd[11] = 0x2f;    // Command read log pio ext
  command_length_ccc = 16;
  int ret = do_ata48_cmd_ccc(current_disk_ccc);

  if (0)
  {
    int i;
    int readsize = 16;
    for (i = 0; i < readsize; i+=16)
    {
      fprintf (stdout, "%x: ", i);
      unsigned char *c;
      int n;
      for (n=0; n < 16 && i+n < readsize; n++)
      {
        c = (unsigned char *)ccc_buffer_ccc+i+n;
        fprintf (stdout, "%02x ", *c);
      }
      fprintf (stdout, "   ");
      for (n=0; n < 16 && i+n < readsize; n++)
      {
        c = (unsigned char *)ccc_buffer_ccc+i+n;
        fprintf (stdout, "%c", isprint(*c) ? *c : '.');
      }
      fprintf (stdout, "\n");
    }
  }
  //fprintf(stdout, "ncq log return= %d\n", ret);    //debug

  unsigned char rebuild_assist_buffer[LOG_PAGE_SIZE];
  memcpy (&rebuild_assist_buffer, ccc_buffer_ccc, LOG_PAGE_SIZE);
  rebuild_assist_log_data_ccc.flags = rebuild_assist_buffer[0];
  rebuild_assist_log_data_ccc.length = rebuild_assist_buffer[7];
  memset(rebuild_assist_log_data_ccc.mask, 0, sizeof(rebuild_assist_log_data_ccc.mask));
  memset(rebuild_assist_log_data_ccc.disabled, 0, sizeof(rebuild_assist_log_data_ccc.disabled));
  int i;
  for (i = 0; i < rebuild_assist_log_data_ccc.length; i++)
  {
    rebuild_assist_log_data_ccc.mask[i] = rebuild_assist_buffer[8 + i];
  }
  for (i = 0; i < rebuild_assist_log_data_ccc.length; i++)
  {
    rebuild_assist_log_data_ccc.disabled[i] = rebuild_assist_buffer[8 + rebuild_assist_log_data_ccc.length + i];
  }

  if (0)
  {
    fprintf (stdout, "flags=%02x length=%02x mask=", rebuild_assist_log_data_ccc.flags, rebuild_assist_log_data_ccc.length);
    for (i = 0; i < rebuild_assist_log_data_ccc.length; i++)
    {
      fprintf (stdout, "%02x", rebuild_assist_log_data_ccc.mask[i]);
    }
    fprintf (stdout, " disabled=");
    for (i = 0; i < rebuild_assist_log_data_ccc.length; i++)
    {
      fprintf (stdout, "%02x", rebuild_assist_log_data_ccc.disabled[i]);
    }
    fprintf (stdout, "\n");
  }

  ccc_main_buffer_size_ccc = ccc_main_buffer_size_bak;
  set_main_buffer_ccc();
  memset (ccc_buffer_ccc, 0, ccc_main_buffer_size_ccc);

  return ret;
}





int write_rebuild_assist_log_ccc (unsigned char data[LOG_PAGE_SIZE])
{
  unsigned long long ccc_main_buffer_size_bak = ccc_main_buffer_size_ccc;
  ccc_main_buffer_size_ccc = LOG_PAGE_SIZE;
  set_main_buffer_ccc();
  memset (ccc_buffer_ccc, 0, ccc_main_buffer_size_ccc);

  memcpy (ccc_buffer_ccc, data, LOG_PAGE_SIZE);

  if (0)
  {
    int i;
    int readsize = 16;
    for (i = 0; i < readsize; i+=16)
    {
      fprintf (stdout, "%x: ", i);
      unsigned char *c;
      int n;
      for (n=0; n < 16 && i+n < readsize; n++)
      {
        c = (unsigned char *)ccc_buffer_ccc+i+n;
        fprintf (stdout, "%02x ", *c);
      }
      fprintf (stdout, "   ");
      for (n=0; n < 16 && i+n < readsize; n++)
      {
        c = (unsigned char *)ccc_buffer_ccc+i+n;
        fprintf (stdout, "%c", isprint(*c) ? *c : '.');
      }
      fprintf (stdout, "\n");
    }
  }

  passthrough_ccc.transfer_length = 2;    // sector field
  passthrough_ccc.byte_block = 1;    // block
  passthrough_ccc.protocol = 5;    // pio-data-out
  passthrough_ccc.check_condition = 1;    // check condition true
  passthrough_ccc.direction = 1;    // to
  passthrough_ccc.transfer_direction = 0;    // to
  passthrough_ccc.sg_xfer_direction = SG_DXFER_TO_DEV;

  passthrough_ccc.ata_cmd[0] = 0;        // feature high, 0
  passthrough_ccc.ata_cmd[1] = 0;      // feature low, 0
  passthrough_ccc.ata_cmd[2] = 0;        // sector count high, 0
  passthrough_ccc.ata_cmd[3] = 1;      // sector count low, 1 to read one page
  passthrough_ccc.ata_cmd[4] = 0;      // LBA4 31:24, 0
  passthrough_ccc.ata_cmd[5] = 0x15;     // LBA1 7:0, 0x15 rebuild assist log page
  passthrough_ccc.ata_cmd[6] = 0;      // LBA5 39:32, 0
  passthrough_ccc.ata_cmd[7] = 0;       // LBA2 15:8, 0
  passthrough_ccc.ata_cmd[8] = 0;      // LBA6 47:40, 0
  passthrough_ccc.ata_cmd[9] = 0;      // LBA3 23:16, 0
  passthrough_ccc.ata_cmd[10] = 0;    // Device, 0
  passthrough_ccc.ata_cmd[11] = 0x3f;    // Command write log pio ext
  command_length_ccc = 16;
  int ret = do_ata48_cmd_ccc(current_disk_ccc);

  ccc_main_buffer_size_ccc = ccc_main_buffer_size_bak;
  set_main_buffer_ccc();
  memset (ccc_buffer_ccc, 0, ccc_main_buffer_size_ccc);

  return ret;
}





int start_driver_ccc (void)
{
  if (superbyte_ccc[57] == 0xcc)
  {
    sprintf (driver_device_name_ccc, "/proc/%s%d", MAIN_DRIVER_IOCTL_NAME, process_id_ccc);
    if (main_driver_fd_ccc > 0)
    {
      close(main_driver_fd_ccc);
    }
    main_driver_fd_ccc = open(driver_device_name_ccc, O_RDWR);
    if (main_driver_fd_ccc == -1)
    {
      check_message_ccc = true;
      strcpy (tempmessage_ccc, curlang_ccc[LANGUERROROPENINGDRIVER]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " %s (%s)\n", driver_device_name_ccc, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return -1;
    }

    if (charater_device_driver_mode_ccc)
    {
      driver_control_data_ccc.command = START_FILE_COMMAND;
    }
    else
    {
      driver_control_data_ccc.command = START_DRIVE_COMMAND;
    }
    unsigned long request = IOCTL_CMD_HDDSC;
    //strcpy(driver_control_data_ccc.buffer, virtual_driver_name_ccc);
    strncpy(driver_control_data_ccc.name, virtual_driver_name_ccc, sizeof(driver_control_data_ccc.name));
    //fprintf (stdout, "sizeofstructdata = %d\n", sizeof(driver_control_data_ccc));    // debug
    int ret = ioctl(main_driver_fd_ccc, request, &driver_control_data_ccc);
    if (ret < 0)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGERRORSTARTINGDRIVER]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " %s (%s)\n", driver_device_name_ccc, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      return -1;
    }

    if (charater_device_driver_mode_ccc)
    {
      // install the node
      char command[256];
      sprintf (command, "mknod /dev/%s c %d 0", virtual_driver_name_ccc, ret);
      system(command);
    }

    // sleep for a few seconds before activating the driver to give it time to settle in
    do_nanosleep_ccc(DRIVER_WAIT_TO_ACTIVATE_TIME);

    driver_control_data_ccc.command = ACTIVATE_DRIVE_COMMAND;
    request = IOCTL_CMD_HDDSC;
    ret = ioctl(main_driver_fd_ccc, request, &driver_control_data_ccc);
    if (ret < 0)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGERRORSTARTINGDRIVER]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " %s (%s)\n", driver_device_name_ccc, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      return -1;
    }

    close(main_driver_fd_ccc);
    driver_running_ccc = 1;
  }
  return 0;
}





int stop_driver_ccc (void)
{
  if (superbyte_ccc[58] == 0x74)
  {
    sprintf (driver_device_name_ccc, "/proc/%s%d", MAIN_DRIVER_IOCTL_NAME, process_id_ccc);
    if (main_driver_fd_ccc > 0)
    {
      close(main_driver_fd_ccc);
    }
    main_driver_fd_ccc = open(driver_device_name_ccc, O_RDWR);
    if (main_driver_fd_ccc == -1)
    {
      check_message_ccc = true;
      strcpy (tempmessage_ccc, curlang_ccc[LANGUERROROPENINGDRIVER]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " %s (%s)\n", driver_device_name_ccc, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return -1;
    }
    int error = 0;
    if (charater_device_driver_mode_ccc)
    {
      driver_control_data_ccc.command = STOP_FILE_COMMAND;
    }
    else
    {
      driver_control_data_ccc.command = STOP_DRIVE_COMMAND;
    }
    unsigned long request = IOCTL_CMD_HDDSC;
    int ret = ioctl(main_driver_fd_ccc, request, &driver_control_data_ccc);
    if (ret < 0)
    {
      error = 1;
      strcpy (tempmessage_ccc, curlang_ccc[LANGERRORSTOPINGDRIVER]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " %s (%s)\n", driver_device_name_ccc, strerror(errno));
      message_error_ccc(tempmessage_ccc);
    }

    if (charater_device_driver_mode_ccc)
    {
      // remove the node
      char command[256];
      sprintf (command, "rm -f /dev/%s", virtual_driver_name_ccc);
      system(command);
    }

    ret = close(main_driver_fd_ccc);
    if (ret < 0)
    {
      error = 1;
      strcpy (tempmessage_ccc, curlang_ccc[LANGERRORCLOSINGINGDRIVER]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " %s (%s)\n", driver_device_name_ccc, strerror(errno));
      message_error_ccc(tempmessage_ccc);
    }
    if (error)
    {
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return -1;
    }
    driver_running_ccc = 0;
  }
  return 0;
}





int reset_driver_timers_ccc (void)
{
  if (driver_memory_mapped_ccc)
  {
    write_ctrl_data_ccc(CTRL_RESET_READ_TIMER, 1);
    write_ctrl_data_ccc(CTRL_RESET_REQUEST_TIMER, 1);
  }
  return 0;
}




int check_driver_ccc (void)
{
  char name[256];
  sprintf (name, "/proc/%s%d", MAIN_DRIVER_IOCTL_NAME, process_id_ccc);
  int fd = open(name, O_RDWR);
  if (fd == -1)
  {
    return -1;
  }
  close (fd);
  return 0;
}




int map_driver_memory_ccc (void)
{
  if (superbyte_ccc[59] == 0x0c)
  {
    unmap_driver_memory_ccc();
    int configfd;
    driver_control_address_ccc = NULL;
    driver_error_bitmap_address_ccc = NULL;
    driver_transfer_buffer_address_ccc = NULL;
    driver_memory_mapped_ccc = 0;

    char name[256];
    sprintf (name, "/proc/%s%d", MAIN_DRIVER_MMAP_NAME, process_id_ccc);
    configfd = open(name, O_RDWR);
    if(configfd < 0)
    {
      sprintf (tempmessage_ccc, "Opening driver mapping file failed (%s)\n", strerror(errno));
      message_error_ccc(tempmessage_ccc);
      return -1;
    }

    driver_control_address_ccc = mmap(NULL, pagesize_ccc, PROT_READ|PROT_WRITE, MAP_SHARED, configfd, 0);
    if (driver_control_address_ccc == MAP_FAILED)
    {
      sprintf (tempmessage_ccc, "Mapping driver memory failed (%s)\n", strerror(errno));
      message_error_ccc(tempmessage_ccc);
      return -1;
    }

    driver_error_bitmap_address_ccc = mmap(NULL, pagesize_ccc*2, PROT_READ|PROT_WRITE, MAP_SHARED, configfd, pagesize_ccc*1);
    if (driver_error_bitmap_address_ccc == MAP_FAILED)
    {
      sprintf (tempmessage_ccc, "Mapping driver memory failed (%s)\n", strerror(errno));
      message_error_ccc(tempmessage_ccc);
      return -1;
    }

    driver_table_buffer_ccc = mmap(NULL, pagesize_ccc*4, PROT_READ|PROT_WRITE, MAP_SHARED, configfd, pagesize_ccc*3);
    if (driver_table_buffer_ccc == MAP_FAILED)
    {
      sprintf (tempmessage_ccc, "Mapping driver memory failed (%s)\n", strerror(errno));
      message_error_ccc(tempmessage_ccc);
      return -1;
    }

    driver_command_list_buffer_ccc = mmap(NULL, pagesize_ccc, PROT_READ|PROT_WRITE, MAP_SHARED, configfd, pagesize_ccc*7);
    if (driver_command_list_buffer_ccc == MAP_FAILED)
    {
      sprintf (tempmessage_ccc, "Mapping driver memory failed (%s)\n", strerror(errno));
      message_error_ccc(tempmessage_ccc);
      return -1;
    }

    driver_fis_buffer_ccc = mmap(NULL, pagesize_ccc, PROT_READ|PROT_WRITE, MAP_SHARED, configfd, pagesize_ccc*8);
    if (driver_fis_buffer_ccc == MAP_FAILED)
    {
      sprintf (tempmessage_ccc, "Mapping driver memory failed (%s)\n", strerror(errno));
      message_error_ccc(tempmessage_ccc);
      return -1;
    }

    close(configfd);



    sprintf (name, "/proc/%s%d", MAIN_DRIVER_MMAPTB_NAME, process_id_ccc);
    configfd = open(name, O_RDWR);
    if(configfd < 0)
    {
      sprintf (tempmessage_ccc, "Opening driver mapping file failed (%s)\n", strerror(errno));
      message_error_ccc(tempmessage_ccc);
      return -1;
    }

    driver_transfer_buffer_address_ccc = mmap(NULL, DRIVER_TRANSFER_BUFFER_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, configfd, 0);
    if (driver_transfer_buffer_address_ccc == MAP_FAILED)
    {
      sprintf (tempmessage_ccc, "Mapping driver memory failed (%s)\n", strerror(errno));
      message_error_ccc(tempmessage_ccc);
      return -1;
    }
    close(configfd);



    sprintf (name, "/proc/%s%d", MAIN_DRIVER_MMAPMDB_NAME, process_id_ccc);
    configfd = open(name, O_RDWR);
    if(configfd < 0)
    {
      sprintf (tempmessage_ccc, "Opening driver mapping file failed (%s)\n", strerror(errno));
      message_error_ccc(tempmessage_ccc);
      return -1;
    }

    driver_main_data_buffer_address_ccc = mmap(NULL, DRIVER_MAIN_DATA_BUFFER_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, configfd, 0);
    if (driver_main_data_buffer_address_ccc == MAP_FAILED)
    {
      sprintf (tempmessage_ccc, "Mapping driver memory failed (%s)\n", strerror(errno));
      message_error_ccc(tempmessage_ccc);
      return -1;
    }
    close(configfd);

    driver_memory_mapped_ccc = 1;
  }
  return 0;
}




int unmap_driver_memory_ccc (void)
{
  if (superbyte_ccc[60] == 0xaa)
  {
    if (driver_control_address_ccc)
    {
      munmap(driver_control_address_ccc, pagesize_ccc);
      driver_control_address_ccc = NULL;
    }
    if (driver_error_bitmap_address_ccc)
    {
      munmap(driver_error_bitmap_address_ccc, pagesize_ccc*2);
      driver_error_bitmap_address_ccc = NULL;
    }
    if (driver_table_buffer_ccc)
    {
      munmap(driver_table_buffer_ccc, pagesize_ccc*4);
      driver_table_buffer_ccc = NULL;
      table_buffer_ccc = NULL;
    }
    if (driver_command_list_buffer_ccc)
    {
      munmap(driver_command_list_buffer_ccc, pagesize_ccc);
      driver_command_list_buffer_ccc = NULL;
      command_list_buffer_ccc = NULL;
    }
    if (driver_fis_buffer_ccc)
    {
      munmap(driver_fis_buffer_ccc, pagesize_ccc);
      driver_fis_buffer_ccc = NULL;
      fis_buffer_ccc = NULL;
    }
    if (driver_transfer_buffer_address_ccc)
    {
      munmap(driver_transfer_buffer_address_ccc, DRIVER_TRANSFER_BUFFER_SIZE);
      driver_transfer_buffer_address_ccc = NULL;
    }
    if (driver_main_data_buffer_address_ccc)
    {
      munmap(driver_main_data_buffer_address_ccc, DRIVER_TRANSFER_BUFFER_SIZE);
      driver_main_data_buffer_address_ccc = NULL;
      ccc_buffer_ccc = NULL;
    }
    driver_memory_mapped_ccc = 0;
  }
  return 0;
}




unsigned long long read_ctrl_data_ccc(int item)
{
  unsigned long long value = 0;
  memcpy(&value, driver_control_address_ccc + (item * superbyte_ccc[64]), superbyte_ccc[65]);    //potential superbyte
  return value;
}




void write_ctrl_data_ccc(int item, unsigned long long value)
{
  memcpy(driver_control_address_ccc + (item * superbyte_ccc[66]), &value, superbyte_ccc[67]);    //potential superbyte
}




unsigned char read_ctrl_error_bitmap_ccc(int offset)
{
  unsigned char value = 0;
  memcpy(&value, driver_error_bitmap_address_ccc + offset, 1);
  return value;
}




void write_ctrl_error_bitmap_ccc(int offset, unsigned char value)
{
  memcpy(driver_error_bitmap_address_ccc + offset, &value, 1);
}








