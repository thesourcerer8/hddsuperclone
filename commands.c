// Copyright (C) 2015-2022 Scott Dwyer.
// You may use/distribute/modify this freely, under the terms of
// the GNU General Public License version 2 or later version.
// This software is distributed WITHOUT ANY WARRANTY.


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// version 0.1 20141018

#include "common.h"
#include "commands.h"

#include "strncpy_wrapper.h"
#include "util.h"


// function to either check or execute a command
int execute_line_ccc(bool perform_check, unsigned int line_number, char *command, char *rest_of_line)
{
  if (verbose_ccc & DEBUG3)
  {
    if (perform_check)
    {
      fprintf (stdout, "checking line %d\n", line_number-1);
    }
    else
    {
      fprintf (stdout, "executing line %d\n", line_number-1);
    }
  }
  if (debug_ccc & DEBUG3)
  {
    if (perform_check)
    {
      fprintf (debug_file_ccc, "checking line %d\n", line_number-1);
    }
    else
    {
      fprintf (debug_file_ccc, "executing line %d\n", line_number-1);
    }
  }
  return_value_ccc = -1;
  if (command[0] == '#')
  {
    return_value_ccc = 0;
  }

  else if ( (strcasecmp(command, "echo") == 0) )
  {
    return_value_ccc = echo_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "seti") == 0) )
  {
    return_value_ccc = seti_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "sets") == 0) )
  {
    return_value_ccc = sets_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "exit") == 0) )
  {
    return_value_ccc = exit_with_code_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "end") == 0) )
  {
    return_value_ccc = -100;
  }

  else if ( (strcasecmp(command, "hex") == 0) )
  {
    if (!perform_check)
    {
      print_hex_ccc = true;
    }
    return_value_ccc = 0;
  }

  else if ( (strcasecmp(command, "decimal") == 0) )
  {
    if (!perform_check)
    {
      print_hex_ccc = false;
    }
    return_value_ccc = 0;
  }

  else if ( (strcasecmp(command, "printbuffer") == 0) )
  {
    return_value_ccc = printbuffer_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "setbuffer") == 0) )
  {
    return_value_ccc = setmainbuffer_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "endbuffer") == 0) )
  {
    return_value_ccc = endmainbuffer_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "clearbuffer") == 0) )
  {
    memset (ccc_buffer_ccc, 0, ccc_main_buffer_size_ccc);
    return (0);
  }

  else if ( (strcasecmp(command, "buffersize") == 0) )
  {
    return_value_ccc = change_main_buffer_size_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "writebuffer") == 0) )
  {
    return_value_ccc = write_buffer_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "readbuffer") == 0) )
  {
    return_value_ccc = read_buffer_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "direction") == 0) )
  {
    return_value_ccc = set_direction_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "scsi6cmd") == 0) )
  {
    return_value_ccc = scsi_6_cmd_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "scsi10cmd") == 0) )
  {
    return_value_ccc = scsi_10_cmd_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "scsi12cmd") == 0) )
  {
    return_value_ccc = scsi_12_cmd_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "scsi16cmd") == 0) )
  {
    return_value_ccc = scsi_16_cmd_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "protocol") == 0) )
  {
    return_value_ccc = set_protocol_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "tlengthfield") == 0) )
  {
    return_value_ccc = set_tlength_field_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "transferunit") == 0) )
  {
    return_value_ccc = set_byte_block_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "checkcondition") == 0) )
  {
    return_value_ccc = set_check_condition_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "ata28cmd") == 0) )
  {
    return_value_ccc = ata_28_cmd_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "ata48cmd") == 0) )
  {
    return_value_ccc = ata_48_cmd_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "writelog") == 0) )
  {
    return_value_ccc = write_log_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "gettime") == 0) )
  {
    return_value_ccc = get_time_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "stringtobuffer") == 0) )
  {
    return_value_ccc = string_to_buffer_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "printsensebuffer") == 0) )
  {
    return_value_ccc = print_sense_buffer_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "reopendisk") == 0) )
  {
    if (!perform_check && !direct_mode_ccc)
    {
      return_value_ccc = disk_reopen_ccc();
    }
    return (0);
  }

  else if ( (strcasecmp(command, "setreadpio") == 0) )
  {
    return_value_ccc = setup_pio_read_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "setreaddma") == 0) )
  {
    return_value_ccc = setup_dma_read_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "setwritepio") == 0) )
  {
    return_value_ccc = setup_pio_write_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "setwritedma") == 0) )
  {
    return_value_ccc = setup_dma_write_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "getstatus") == 0) )
  {
    return_value_ccc = get_status_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "softreset") == 0) )
  {
    return_value_ccc = perform_soft_reset_ccc(perform_check, line_number, rest_of_line);
  }

    else if ( (strcasecmp(command, "hardreset") == 0) )
  {
    return_value_ccc = perform_hard_reset_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "usleep") == 0) )
  {
    return_value_ccc = u_sleep_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "loadscript") == 0) )
  {
    return_value_ccc = load_script_file_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "previousscript") == 0) )
  {
    return_value_ccc = previous_script_file_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "include") == 0) )
  {
    return_value_ccc = include_script_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "stringtoscratchpad") == 0) )
  {
    return_value_ccc = string_to_scratchpad_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "gosub") == 0) )
  {
    return_value_ccc = gosub_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "subroutine") == 0) )
  {
    return_value_ccc = ccc_subroutine_ccc(perform_check, line_number);
  }

  else if ( (strcasecmp(command, "endsubroutine") == 0) )
  {
    return_value_ccc = endsubroutine_ccc(perform_check, line_number);
  }

  else if ( (strcasecmp(command, "returnsub") == 0) )
  {
    return_value_ccc = returnsub_ccc(perform_check, line_number);
  }

  else if ( (strcasecmp(command, "if") == 0) )
  {
    return_value_ccc = ifstate_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "endif") == 0) )
  {
    return_value_ccc = ccc_endif_ccc(perform_check, line_number);
  }

  else if ( (strcasecmp(command, "else") == 0) )
  {
    return_value_ccc = elsestate_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "elseif") == 0) )
  {
    return_value_ccc = ccc_elseif_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "while") == 0) )
  {
    return_value_ccc = dowhile_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "done") == 0) )
  {
    return_value_ccc = ccc_done_ccc(perform_check, line_number);
  }

  else if ( (strcasecmp(command, "break") == 0) )
  {
    return_value_ccc = dobreak_ccc(perform_check, line_number);
  }

  else if ( (strcasecmp(command, "wordflipscratchpad") == 0) )
  {
    return_value_ccc = wordflip_scratchpad_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "wordflipbuffer") == 0) )
  {
    return_value_ccc = wordflip_buffer_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "getfilesize") == 0) )
  {
    return_value_ccc = get_file_size_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "writescratchpad") == 0) )
  {
    return_value_ccc = write_scratchpad_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "readscratchpad") == 0) )
  {
    return_value_ccc = read_scratchpad_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "deletefile") == 0) )
  {
    return_value_ccc = delete_file_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "callcommand") == 0) )
  {
    return_value_ccc = call_command_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "userinput") == 0) )
  {
    return_value_ccc = user_input_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "printscratchpad") == 0) )
  {
    return_value_ccc = printscratchpad_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "setscratchpad") == 0) )
  {
    return_value_ccc = setmainscratchpad_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "endscratchpad") == 0) )
  {
    return_value_ccc = endmainscratchpad_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "clearscratchpad") == 0) )
  {
    memset (ccc_scratchpad_ccc, 0, ccc_main_scratchpad_size_ccc);
    return (0);
  }

  else if ( (strcasecmp(command, "scratchpadsize") == 0) )
  {
    return_value_ccc = change_main_scratchpad_size_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "copyscratchpadtobuffer") == 0) )
  {
    return_value_ccc = copy_scratch_to_buffer_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "copybuffertoscratchpad") == 0) )
  {
    return_value_ccc = copy_buffer_to_scratch_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "variablecheck") == 0) )
  {
    return_value_ccc = variable_check_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "upline") == 0) )
  {
    if (!perform_check)
    {
      fprintf (stdout, "\x1b[A");
    }
    return (0);
  }

  else if ( (strcasecmp(command, "clearusbbuffer") == 0) )
  {
    memset (ccc_usbbuffer_ccc, 0, ccc_main_usbbuffer_size_ccc);
    return (0);
  }

  else if ( (strcasecmp(command, "stringtousbbuffer") == 0) )
  {
    return_value_ccc = string_to_usbbuffer_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "wordflipusbbuffer") == 0) )
  {
    return_value_ccc = wordflip_usbbuffer_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "writeusbbuffer") == 0) )
  {
    return_value_ccc = write_usbbuffer_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "readusbbuffer") == 0) )
  {
    return_value_ccc = read_usbbuffer_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "printusbbuffer") == 0) )
  {
    return_value_ccc = print_usbbuffer_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "setusbbuffer") == 0) )
  {
    return_value_ccc = set_usbbuffer_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "endusbbuffer") == 0) )
  {
    return_value_ccc = end_usbbuffer_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "usbbuffersize") == 0) )
  {
    return_value_ccc = change_usbbuffer_size_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "copyusbtobuffer") == 0) )
  {
    return_value_ccc = copy_usbbuffer_to_buffer_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "copybuffertousb") == 0) )
  {
    return_value_ccc = copy_buffer_to_usbbuffer_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "copyusbtoscratchpad") == 0) )
  {
    return_value_ccc = copy_usbbuffer_to_scratch_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "copyscratchpadtousb") == 0) )
  {
    return_value_ccc = copy_scratch_to_usbbuffer_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "sendusbcontrolmsg") == 0) )
  {
    return_value_ccc = send_usb_control_msg_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "usbrawread") == 0) )
  {
    return_value_ccc = usb_raw_read_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "usbrawwrite") == 0) )
  {
    return_value_ccc = usb_raw_write_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "usbreset") == 0) )
  {
    return_value_ccc = usb_reset_ccc(perform_check, line_number, rest_of_line);
  }





  else if ( (strcasecmp(command, "softtimeout") == 0) )
  {
    return_value_ccc = softtimeout_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "hardtimeout") == 0) )
  {
    return_value_ccc = hardtimeout_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "busytimeout") == 0) )
  {
    return_value_ccc = busytimeout_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "resettimeout") == 0) )
  {
    return_value_ccc = resettimeout_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "generaltimeout") == 0) )
  {
    return_value_ccc = generaltimeout_ccc(perform_check, line_number, rest_of_line);
  }

  else if ( (strcasecmp(command, "usbtimeout") == 0) )
  {
    return_value_ccc = usbtimeout_ccc(perform_check, line_number, rest_of_line);
  }







  else // default:
  {
    if (!setting_buffer_ccc && !setting_scratchpad_ccc && !setting_usbbuffer_ccc)
    {
      fprintf (stdout, "ERROR! Command \'%s\' on line \'%d\' not a valid command.\n", command, line_number-1);
      return (-1);
    }
    else
    {
      return (0);
    }
  }

  return (return_value_ccc);
}




// command echo
// print whatever is on the line after the echo command
int echo_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int i;
  int length = strlen(rest_of_line);
  char var_name[length+1];
  bool print = false;
  bool dquote;
  char *error_string = "Error processing ECHO command on line";
  for (i = 0; i < length; i++)
  {
    //fprintf (stderr, "%c", rest_of_line[i]);
    if (!print)
    {
      if (rest_of_line[i] == '$')
      {
        int scanline = sscanf(rest_of_line +i+1, "%s [^\n]", var_name);
        if (scanline != 1)
        {
          fprintf (stderr, "\nError processing variable,\n");
          fprintf (stderr, "%s %d.\n", error_string, line_number-1);
          return (-1);
        }
        //fprintf (stdout, "\nvar_name= %s\n", var_name);
        return_value_ccc = check_variable_ccc(var_name);
        if (return_value_ccc < 1)
        {
          fprintf (stderr, "\nError: Variable \'%s\' not found,\n", var_name);
          fprintf (stderr, "%s %d.\n", error_string, line_number-1);
          return (-2);
        }

        else if (return_value_ccc == 1)
        {
          int var_num = find_number_variable_ccc(var_name);
          if (!perform_check)
          {
            if (print_hex_ccc)
            {
              fprintf (stdout, "%llx", number_variable_buffer_ccc[var_num]);
            }
            else
            {
              fprintf (stdout, "%lld", number_variable_buffer_ccc[var_num]);
            }
          }
          i += strlen(var_name);
        }

        else if (return_value_ccc == 2)
        {
          int var_num = find_string_variable_ccc(var_name);
          if (!perform_check)
          {
            fprintf (stdout, "%s", string_variable_pointer_ccc[var_num]);
          }
          i += strlen(var_name);
        }
      }
      else if (rest_of_line[i] == '"' && rest_of_line[i+1] == '"')
      {
        i += 2;
      }
      else if (rest_of_line[i] == '\'' && rest_of_line[i+1] == '\'')
      {
        i += 2;
      }
      else if (rest_of_line[i] == '"')
      {
        print = true;
        dquote = true;
        i++;
      }
      else if (rest_of_line[i] == '\'')
      {
        print = true;
        dquote = false;
        i++;
      }
      else if (rest_of_line[i] == ' ')
      {
        i++;
        i--;
      }
      else
      {
        fprintf (stderr, "\nError: Unknown characters outside of quotes,\n");
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-2);
      }
    }
    else if (rest_of_line[i] == '"' && dquote)
    {
      print = false;
    }
    else
    {
      if (rest_of_line[i] == '\'' && !dquote)
        print = false;
    }

    if (!perform_check && print)
    {
      fprintf (stdout, "%c", rest_of_line[i]);
    }
  }
  if (!perform_check)
  {
    fprintf (stdout, "\n");
  }

  return (0);
}




// command seti
// set a number variable to a value
int seti_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char var_name[length+1];
  char value[length+1];
  char *error_string = "Error processing SETI command on line";
  int scanline = sscanf(rest_of_line, "%s = %[^\n]", var_name, value);
  if (scanline < 2)
  {
    fprintf (stderr, "\nError processing variable,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if ( (strncmp(value, "buffer", 6) == 0) || (strncmp(value, "BUFFER", 6) == 0) )
  {
    char raw_offset[length];
    char raw_value_type[length];
    char leftover[length];
    int value_type = 1;    //byte
    unsigned int offset;
    int scanline = sscanf(value+6, "%s %s %[^\n]", raw_offset, raw_value_type, leftover);
    if (scanline != 1 && scanline != 2)
    {
      if (scanline < 1)
      {
        fprintf (stderr, "\nError: Not enough arguments,\n");
      }
      if (scanline > 2)
      {
        fprintf (stderr, "\nError: Too many arguments,\n");
      }
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

    if (scanline == 2)
    {
      if (strcmp(raw_value_type, "b") == 0)
      {
        value_type = 1;
      }
      else if (strcmp(raw_value_type, "w") == 0)
      {
        value_type = 2;
      }
      else if (strcmp(raw_value_type, "dw") == 0)
      {
        value_type = 4;
      }
      else if (strcmp(raw_value_type, "qw") == 0)
      {
        value_type = 8;
      }
      else
      {
        fprintf (stderr, "\nError: Invalid type,\n");
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
    }

    if (raw_offset[0] == '$')
    {
      offset = get_number_variable_value_ccc(raw_offset);
      if (return_value_ccc < 0)
      {
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
    }
    else
    {
      char* endptr;
      offset = strtoll(raw_offset, &endptr, 0);
      if (*endptr)
      {
        fprintf (stderr, "Error! Offset \'%s\' was unable to be completely processed.\n", raw_offset);
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
    }
    if (!perform_check && offset+value_type-1 >= ccc_main_buffer_size_ccc)
    {
      fprintf (stderr, "\nError: Buffer offset %s higher than current buffer size,\n", value);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

    if (value_type == 1)
    {
      unsigned char *c = (unsigned char *)ccc_buffer_ccc+offset;
      snprintf(value, sizeof(value), "0x%x", (unsigned char)*c);
    }
    else if (value_type == 2)
    {
      uint16_t w;
      memcpy(&w, ccc_buffer_ccc+offset, 2);
      snprintf(value, sizeof(value), "0x%x", w);
    }
    else if (value_type == 4)
    {
      uint32_t dw;
      memcpy(&dw, ccc_buffer_ccc+offset, 4);
      snprintf(value, sizeof(value), "0x%x", dw);
    }
    else if (value_type == 8)
    {
      uint64_t qw;
      memcpy(&qw, ccc_buffer_ccc+offset, 8);
      unsigned long long qw64 = qw;
      snprintf(value, sizeof(value), "0x%llx", qw64);
    }
    else
    {
      fprintf (stderr, "\nInternal Error! (invlaid value type),\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  else if ( (strncmp(value, "usbbuffer", 9) == 0) || (strncmp(value, "USBBUFFER", 9) == 0) )
  {
    char raw_offset[length];
    char raw_value_type[length];
    char leftover[length];
    int value_type = 1;    //byte
    unsigned int offset;
    int scanline = sscanf(value+10, "%s %s %[^\n]", raw_offset, raw_value_type, leftover);
    if (scanline != 1 && scanline != 2)
    {
      if (scanline < 1)
      {
        fprintf (stderr, "\nError: Not enough arguments,\n");
      }
      if (scanline > 2)
      {
        fprintf (stderr, "\nError: Too many arguments,\n");
      }
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

    if (scanline == 2)
    {
      if (strcmp(raw_value_type, "b") == 0)
      {
        value_type = 1;
      }
      else if (strcmp(raw_value_type, "w") == 0)
      {
        value_type = 2;
      }
      else if (strcmp(raw_value_type, "dw") == 0)
      {
        value_type = 4;
      }
      else if (strcmp(raw_value_type, "qw") == 0)
      {
        value_type = 8;
      }
      else
      {
        fprintf (stderr, "\nError: Invalid type,\n");
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
    }

    if (raw_offset[0] == '$')
    {
      offset = get_number_variable_value_ccc(raw_offset);
      if (return_value_ccc < 0)
      {
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
    }
    else
    {
      char* endptr;
      offset = strtoll(raw_offset, &endptr, 0);
      if (*endptr)
      {
        fprintf (stderr, "Error! Offset \'%s\' was unable to be completely processed.\n", raw_offset);
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
    }
    if (!perform_check && offset+value_type-1 >= ccc_main_scratchpad_size_ccc)
    {
      fprintf (stderr, "\nError: USBbuffer offset %s higher than current usbbuffer size,\n", value);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

    if (value_type == 1)
    {
      unsigned char *c = (unsigned char *)ccc_usbbuffer_ccc+offset;
      snprintf(value, sizeof(value), "0x%x", (unsigned char)*c);
    }
    else if (value_type == 2)
    {
      uint16_t w;
      memcpy(&w, ccc_usbbuffer_ccc+offset, 2);
      snprintf(value, sizeof(value), "0x%x", w);
    }
    else if (value_type == 4)
    {
      uint32_t dw;
      memcpy(&dw, ccc_usbbuffer_ccc+offset, 4);
      snprintf(value, sizeof(value), "0x%x", dw);
    }
    else if (value_type == 8)
    {
      uint64_t qw;
      memcpy(&qw, ccc_usbbuffer_ccc+offset, 8);
      unsigned long long qw64 = qw;
      snprintf(value, sizeof(value), "0x%llx", qw64);
    }
    else
    {
      fprintf (stderr, "\nInternal Error! (invlaid value type),\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

  }

  else if ( (strncmp(value, "scratchpad", 10) == 0) || (strncmp(value, "SCRATCHPAD", 10) == 0) )
  {
    char raw_offset[length];
    char raw_value_type[length];
    char leftover[length];
    int value_type = 1;    //byte
    unsigned int offset;
    int scanline = sscanf(value+10, "%s %s %[^\n]", raw_offset, raw_value_type, leftover);
    if (scanline != 1 && scanline != 2)
    {
      if (scanline < 1)
      {
        fprintf (stderr, "\nError: Not enough arguments,\n");
      }
      if (scanline > 2)
      {
        fprintf (stderr, "\nError: Too many arguments,\n");
      }
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

    if (scanline == 2)
    {
      if (strcmp(raw_value_type, "b") == 0)
      {
        value_type = 1;
      }
      else if (strcmp(raw_value_type, "w") == 0)
      {
        value_type = 2;
      }
      else if (strcmp(raw_value_type, "dw") == 0)
      {
        value_type = 4;
      }
      else if (strcmp(raw_value_type, "qw") == 0)
      {
        value_type = 8;
      }
      else
      {
        fprintf (stderr, "\nError: Invalid type,\n");
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
    }

    if (raw_offset[0] == '$')
    {
      offset = get_number_variable_value_ccc(raw_offset);
      if (return_value_ccc < 0)
      {
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
    }
    else
    {
      char* endptr;
      offset = strtoll(raw_offset, &endptr, 0);
      if (*endptr)
      {
        fprintf (stderr, "Error! Offset \'%s\' was unable to be completely processed.\n", raw_offset);
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
    }
    if (!perform_check && offset+value_type-1 >= ccc_main_scratchpad_size_ccc)
    {
      fprintf (stderr, "\nError: Scratchpad offset %s higher than current scratchpad size,\n", value);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

    if (value_type == 1)
    {
      unsigned char *c = (unsigned char *)ccc_scratchpad_ccc+offset;
      snprintf(value, sizeof(value), "0x%x", (unsigned char)*c);
    }
    else if (value_type == 2)
    {
      uint16_t w;
      memcpy(&w, ccc_scratchpad_ccc+offset, 2);
      snprintf(value, sizeof(value), "0x%x", w);
    }
    else if (value_type == 4)
    {
      uint32_t dw;
      memcpy(&dw, ccc_scratchpad_ccc+offset, 4);
      snprintf(value, sizeof(value), "0x%x", dw);
    }
    else if (value_type == 8)
    {
      uint64_t qw;
      memcpy(&qw, ccc_scratchpad_ccc+offset, 8);
      unsigned long long qw64 = qw;
      snprintf(value, sizeof(value), "0x%llx", qw64);
    }
    else
    {
      fprintf (stderr, "\nInternal Error! (invlaid value type),\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

  }
  else if ( (strncmp(value, "sensebuffer", 11) == 0) || (strncmp(value, "SENSEBUFFER", 11) == 0) )
  {
    char raw_offset[length];
    char leftover[length];
    unsigned int offset;
    int scanline = sscanf(value+11, "%s %[^\n]", raw_offset, leftover);
    if (scanline != 1)
    {
      if (scanline < 1)
      {
        fprintf (stderr, "\nError: Not enough arguments,\n");
      }
      if (scanline > 1)
      {
        fprintf (stderr, "\nError: Too many arguments,\n");
      }
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

    if (raw_offset[0] == '$')
    {
      offset = get_number_variable_value_ccc(raw_offset);
      if (return_value_ccc < 0)
      {
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
    }
    else
    {
      char* endptr;
      offset = strtoll(raw_offset, &endptr, 0);
      if (*endptr)
      {
        fprintf (stderr, "Error! Offset \'%s\' was unable to be completely processed.\n", raw_offset);
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
    }
    if (!perform_check && offset >= sensebuffer_size_ccc)
    {
      fprintf (stderr, "\nError: Sensebuffer offset %s higher than current sensebuffer size,\n", value);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

    unsigned char *c = (unsigned char *)sensebuffer_ccc+offset;
    snprintf(value, sizeof(value), "0x%x", (unsigned char)*c);
  }

  if (var_name[0] != '$')
  {
    fprintf (stderr, "\nError: Variable name must start with \'$\',\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  scanline = sscanf(var_name+1, "%s", var_name);
  if (scanline != 1)
  {
    fprintf (stderr, "\nError processing variable,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  return_value_ccc = create_number_variable_ccc(var_name);
  if (return_value_ccc != 0)
  {
    if (return_value_ccc == -1)
    {
      fprintf (stderr, "Error: Variable \'%s\' has been declared more than once,\n", var_name);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
    else if (return_value_ccc != 1)
    {
      fprintf (stderr, "Error: Variable \'%s\' has been declared previously as a different type,\n", var_name);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (!perform_check)
  {
    int var_num = find_number_variable_ccc(var_name);
    return_value_ccc = set_number_variable_ccc(var_num, value);
    if (return_value_ccc == -1)
    {
      fprintf (stderr, "Error! Variable \'%s\' value \'%s\' was unable to be completely processed,\n", var_name, value);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  return (0);
}




// command sets
// set a string variable to a value
int sets_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char var_name[length+1];
  char value[length+1];
  char temp_var[MAX_VARIABLE_LENGTH * 4];
  memset(&temp_var, 0, sizeof(temp_var));
  char *error_string = "Error processing SETS command on line";
  int scanline = sscanf(rest_of_line, "%s = %[^\n]", var_name, value);
  if (scanline != 2)
  {
    fprintf (stderr, "\nError processing variable,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (var_name[0] != '$')
  {
    fprintf (stderr, "\nError: Variable name must start with \'$\',\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  scanline = sscanf(var_name+1, "%s", var_name);
  if (scanline != 1)
  {
    fprintf (stderr, "\nError processing variable,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  return_value_ccc = create_string_variable_ccc(var_name);
  if (return_value_ccc != 0)
  {
    if (return_value_ccc == -1)
    {
      fprintf (stderr, "Error: Variable \'%s\' has been declared more than once,\n", var_name);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
    else if (return_value_ccc != 2)
    {
      fprintf (stderr, "Error: Variable \'%s\' has been declared previously as a different type,\n", var_name);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if ( (strncmp(value, "buffer", 6) == 0) || (strncmp(value, "BUFFER", 6) == 0) )
  {
    char raw_offset[length];
    char raw_size[length];
    char leftover[length];
    unsigned int offset;
    unsigned int size;
    int scanline = sscanf(value+6, "%s %s %[^\n]", raw_offset, raw_size, leftover);
    if (scanline != 2)
    {
      if (scanline < 2)
      {
        fprintf (stderr, "\nError: Not enough arguments,\n");
      }
      if (scanline > 2)
      {
        fprintf (stderr, "\nError: Too many arguments,\n");
      }
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

    if (raw_offset[0] == '$')
    {
      offset = get_number_variable_value_ccc(raw_offset);
      if (return_value_ccc < 0)
      {
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
    }
    else
    {
      char* endptr;
      offset = strtoll(raw_offset, &endptr, 0);
      if (*endptr)
      {
        fprintf (stderr, "Error! Offset \'%s\' was unable to be completely processed.\n", raw_offset);
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
    }
    if (raw_size[0] == '$')
    {
      size = get_number_variable_value_ccc(raw_size);
      if (return_value_ccc < 0)
      {
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
    }
    else
    {
      char* endptr;
      size = strtoll(raw_size, &endptr, 0);
      if (*endptr)
      {
        fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
    }
    if (!perform_check && size + offset > ccc_main_buffer_size_ccc)
    {
      fprintf (stderr, "ERROR: Attempt to copy past end of buffer,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

    if (size > MAX_VARIABLE_LENGTH -2)
    {
      fprintf (stderr, "ERROR: Size is bigger than max variable length,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

    if (!perform_check)
    {
      strncpy(temp_var, ccc_buffer_ccc+offset, size);
    }
  }

  else if ( (strncmp(value, "scratchpad", 6) == 0) || (strncmp(value, "SCRATCHPAD", 6) == 0) )
  {
    char raw_offset[length];
    char raw_size[length];
    char leftover[length];
    unsigned int offset;
    unsigned int size;
    int scanline = sscanf(value+10, "%s %s %[^\n]", raw_offset, raw_size, leftover);
    if (scanline != 2)
    {
      if (scanline < 2)
      {
        fprintf (stderr, "\nError: Not enough arguments,\n");
      }
      if (scanline > 2)
      {
        fprintf (stderr, "\nError: Too many arguments,\n");
      }
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

    if (raw_offset[0] == '$')
    {
      offset = get_number_variable_value_ccc(raw_offset);
      if (return_value_ccc < 0)
      {
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
    }
    else
    {
      char* endptr;
      offset = strtoll(raw_offset, &endptr, 0);
      if (*endptr)
      {
        fprintf (stderr, "Error! Offset \'%s\' was unable to be completely processed.\n", raw_offset);
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
    }
    if (raw_size[0] == '$')
    {
      size = get_number_variable_value_ccc(raw_size);
      if (return_value_ccc < 0)
      {
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
    }
    else
    {
      char* endptr;
      size = strtoll(raw_size, &endptr, 0);
      if (*endptr)
      {
        fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
    }
    if (!perform_check && size + offset > ccc_main_scratchpad_size_ccc)
    {
      fprintf (stderr, "ERROR: Attempt to copy past end of scratchpad,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

    if (size > MAX_VARIABLE_LENGTH -2)
    {
      fprintf (stderr, "ERROR: Size is bigger than max variable length,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

    if (!perform_check)
    {
      strncpy(temp_var, ccc_scratchpad_ccc+offset, size);
    }
  }

  else
  {
    int i;
    strcpy(temp_var, "");
    length = strlen(value);
    bool print = false;
    bool dquote;
    for (i = 0; i < length; i++)
    {
      char var_name[length+1];
      if (!print)
      {
        if (value[i] == '$')
        {
          int scanline = sscanf(value +i+1, "%s [^\n]", var_name);
          if (scanline != 1)
          {
            fprintf (stderr, "\nError processing variable,\n");
            fprintf (stderr, "%s %d.\n", error_string, line_number-1);
            return (-1);
          }
          //fprintf (stdout, "\nvar_name= %s\n", var_name);
          return_value_ccc = check_variable_ccc(var_name);
          if (return_value_ccc < 1)
          {
            fprintf (stderr, "\nError: Variable \'%s\' not found,\n", var_name);
            fprintf (stderr, "%s %d.\n", error_string, line_number-1);
            return (-2);
          }

          else if (return_value_ccc == 1)
          {
            int var_num = find_number_variable_ccc(var_name);
            if (!perform_check)
            {
              char str[32];
              if (print_hex_ccc)
              {
                snprintf(str, sizeof(str), "%llx", number_variable_buffer_ccc[var_num]);
              }
              else
              {
                snprintf(str, sizeof(str), "%lld", number_variable_buffer_ccc[var_num]);
              }
              strcat(temp_var, str);
            }
            i += strlen(var_name);
          }

          else if (return_value_ccc == 2)
          {
            int var_num = find_string_variable_ccc(var_name);
            if (!perform_check)
            {
              strcat(temp_var, string_variable_pointer_ccc[var_num]);
            }
            i += strlen(var_name);
          }
        }
        else if (value[i] == '"' && value[i+1] == '"')
        {
          i += 2;
        }
        else if (value[i] == '\'' && value[i+1] == '\'')
        {
          i += 2;
        }
        else if (value[i] == '"')
        {
          print = true;
          dquote = true;
          i++;
        }
        else if (value[i] == '\'')
        {
          print = true;
          dquote = false;
          i++;
        }
        else if (value[i] == ' ')
        {
          i++;
          i--;
        }
        else
        {
          fprintf (stderr, "\nError: Unknown characters outside of quotes,\n");
          fprintf (stderr, "%s %d.\n", error_string, line_number-1);
          return (-2);
        }
      }
      else if (value[i] == '"' && dquote)
      {
        print = false;
      }
      else
      {
        if (value[i] == '\'' && !dquote)
          print = false;
      }

      if (!perform_check && print)
      {
        char temp[2];
        temp[0] = value[i];
        temp[1] = '\0';
        strcat(temp_var, temp);
      }

      int total_length = strlen(temp_var);
      if (total_length > MAX_VARIABLE_LENGTH -1)
      {
        fprintf (stderr, "\nError: String variable too long,\n");
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-2);
      }
    }
  }

  if (!perform_check)
  {
    int var_num = find_string_variable_ccc(var_name);
    set_string_variable_ccc(var_num, temp_var);
  }

  return (0);
}







int printbuffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_offset[length];
  char raw_size[length];
  char leftover[length];
  unsigned int offset;
  unsigned int size;
  char *error_string = "Error processing PRINTBUFFER command on line";
  int scanline = sscanf(rest_of_line, "%s %s %[^\n]", raw_offset, raw_size, leftover);
  if (scanline != 2)
  {
    if (scanline < 2)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 2)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (raw_offset[0] == '$')
  {
    offset = get_number_variable_value_ccc(raw_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    char* endptr;
    offset = strtoll(raw_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Offset \'%s\' was unable to be completely processed.\n", raw_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  if (raw_size[0] == '$')
  {
    size = get_number_variable_value_ccc(raw_size);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    char* endptr;
    size = strtoll(raw_size, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  if (!perform_check && size + offset > ccc_main_buffer_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to print past end of buffer,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check)
  {
    unsigned int i;
    for (i = 0; i < size; i+=16)
    {
      fprintf (stdout, "%x: ", i + offset);
      unsigned char *c;
      int n;
      for (n=0; n < 16 && i+n < size; n++)
      {
        c = (unsigned char *)ccc_buffer_ccc+i+n+offset;
        fprintf (stdout, "%02x ", *c);
      }
      fprintf (stdout, "   ");
      for (n=0; n < 16 && i+n < size; n++)
      {
        c = (unsigned char *)ccc_buffer_ccc+i+n+offset;
        fprintf (stdout, "%c", isprint(*c) ? *c : '.');
      }
      fprintf (stdout, "\n");
    }
  }

  return (0);
}






int setmainbuffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  //int length = strlen(rest_of_line);
  char raw_offset[MAX_LINE_LENGTH];
  char leftover[MAX_LINE_LENGTH];
  char* endptr;
  unsigned long long offset;
  char *error_string = "Error processing SETBUFFER command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", raw_offset, leftover);
  if (scanline != 1)
  {
    fprintf (stderr, "\nError processing offset argument,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (raw_offset[0] == '$')
  {
    offset = get_number_variable_value_ccc(raw_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    offset = strtoll(raw_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Offset \'%s\' was unable to be completely processed.\n", raw_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  if (perform_check)
  {
    setting_buffer_ccc = true;
    int find_next_setbuf = find_command_ccc("setbuffer", line_number+1);
    int find_next_endbuf = find_command_ccc("endbuffer", line_number+1);
    if (find_next_endbuf < 1)
    {
      fprintf (stderr, "\nError, SETBUFFER without ENDBUFFER,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
    if (find_next_setbuf > 1 && find_next_setbuf < find_next_endbuf)
    {
      fprintf (stderr, "\nError: Next SETBUFFER found before ENDBUFFER on line %d,\n", find_next_setbuf+1);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  int current_line = line_number+1;
  char *full_line;
  char input_text[MAX_LINE_LENGTH];
  memset(input_text,0,sizeof(input_text));
  if (command_line_ccc)
  {
    fprintf (stdout, "setbuffer> ");
    fflush(stdout);
    full_line = fgets(input_text,sizeof(input_text)-1, stdin);
  }
  else
  {
    full_line = get_full_line_ccc(current_line, input_text);
  }
  char line[MAX_LINE_LENGTH];
  strcpy (line, full_line);
  char raw_byte[MAX_LINE_LENGTH];
  unsigned int byte_count = 0;
  unsigned int byte_offset = 0;
  while (1)
  {
    //fprintf (stdout, "line=%s\n", line);
    strcpy (raw_byte, "");
    strcpy (leftover, "");
    sscanf(line, "%s %[^\n]", raw_byte, leftover);
    //fprintf (stdout, "raw_byte=%s  leftover=%s\n", raw_byte, leftover);
    if ( (strcmp(raw_byte, "endbuffer") == 0) || (strcmp(raw_byte, "ENDBUFFER") == 0) )
    {
      break;
    }
    int length = strlen(raw_byte);
    if (length <= 0)
    {
      current_line++;
      char *full_line;
      char input_text[MAX_LINE_LENGTH];
      input_text[0]='\0';
      if (command_line_ccc)
      {
        fprintf (stdout, "setbuffer> ");
        fflush(stdout);
        full_line = fgets(input_text, sizeof input_text, stdin);
      }
      else
      {
        full_line = get_full_line_ccc(current_line, input_text);
      }
      strcpy (line, full_line);

    }
    else if (raw_byte[length-1] == ':')
    {
      byte_offset = strtoll(raw_byte, NULL, 16);
      strcpy (line, leftover);
      byte_count = 0;
    }
    else
    {
      if (!perform_check &&  byte_count +1 + byte_offset + offset > ccc_main_buffer_size_ccc)
      {
        fprintf (stderr, "Error! End of buffer reached at \'%s\' on line %d.\n", raw_byte, current_line+1);
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
      unsigned long long value;
      if (raw_byte[0] == '$')
      {
        value = get_number_variable_value_ccc(raw_byte);
        if (return_value_ccc < 0)
        {
          fprintf (stderr, "%s %d.\n", error_string, line_number-1);
          return (-1);
        }
      }
      else
      {
        char* endptr;
        value = strtoull(raw_byte, &endptr, 16);
        if (*endptr)
        {
          fprintf (stderr, "Error! Byte \'%s\' on line %d could not be processed.\n", raw_byte, current_line+1);
          fprintf (stderr, "%s %d.\n", error_string, line_number-1);
          return (-1);
        }
      }
      if (value > 255)
      {
        fprintf (stderr, "Error! Byte \'%s\' on line %d is too big (>255).\n", raw_byte, current_line+1);
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
      if (!perform_check)
      {
        unsigned char c = value;
        memcpy(ccc_buffer_ccc + byte_offset + offset + byte_count, &c, 1);
      }
      strcpy (line, leftover);
      byte_count++;
    }
  }
  if (!perform_check)
  {
    return (current_line-1);
  }
  return (0);
}






int endmainbuffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  char *error_string = "Error processing ENDBUFFER command on line";
  if (perform_check)
  {
    if (!setting_buffer_ccc)
    {
      fprintf (stderr, "\nError, ENDBUFFER without SETBUFFER,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
    int length = strlen(rest_of_line);
    if (length > 0)
    {
      fprintf (stderr, "\nError, there should be no arguments after the ENDBUFFER command,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
    setting_buffer_ccc = false;
  }
  return (0);
}






int change_main_buffer_size_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  //int length = strlen(rest_of_line);
  char raw_size[MAX_LINE_LENGTH];
  char leftover[MAX_LINE_LENGTH];
  char* endptr;
  unsigned long long size;
  bool size_is_variable = false;
  char *error_string = "Error processing BUFFERSIZE command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", raw_size, leftover);
  if (scanline != 1)
  {
    fprintf (stderr, "\nError processing size argument,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (raw_size[0] == '$')
  {
    size_is_variable = true;
    size = get_number_variable_value_ccc(raw_size);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    size = strtoll(raw_size, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (perform_check && size_is_variable && !direct_mode_ccc)
  {
    ccc_main_buffer_size_ccc = real_buffer_size_ccc;
  }
  else if (perform_check && size_is_variable && direct_mode_ccc)
  {
    ccc_main_buffer_size_ccc = max_dma_size_ccc;
  }
  else
  {
    ccc_main_buffer_size_ccc = size;
  }
  return_value_ccc = set_main_buffer_ccc();
  if (return_value_ccc != 0)
  {
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
  }
  return (return_value_ccc);
}







int print_sense_buffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_offset[length];
  char raw_size[length];
  char leftover[length];
  unsigned int offset;
  unsigned int size;
  char *error_string = "Error processing PRINTSENSEBUFFER command on line";

  int scanline = sscanf(rest_of_line, "%s %s %[^\n]", raw_offset, raw_size, leftover);
  if (scanline != 2)
  {
    if (scanline < 2)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 2)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (raw_offset[0] == '$')
  {
    offset = get_number_variable_value_ccc(raw_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    char* endptr;
    offset = strtoll(raw_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Offset \'%s\' was unable to be completely processed.\n", raw_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  if (raw_size[0] == '$')
  {
    size = get_number_variable_value_ccc(raw_size);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    char* endptr;
    size = strtoll(raw_size, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  if (size + offset > sensebuffer_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to print past end of sensebuffer,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check)
  {
    unsigned int i;
    for (i = 0; i < size; i+=16)
    {
      fprintf (stdout, "%x: ", i + offset);
      unsigned char *c;
      int n;
      for (n=0; n < 16 && i+n < size; n++)
      {
        c = (unsigned char *)sensebuffer_ccc+i+n+offset;
        fprintf (stdout, "%02x ", *c);
      }
      fprintf (stdout, "\n");
    }
  }

  return (0);
}








// function to write buffer to file
int write_buffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_file_name[length];
  char raw_buffer_offset[length];
  char raw_file_offset[length];
  char raw_size[length];
  char leftover[length];
  char* endptr;
  char *file_name;
  unsigned long long buffer_offset;
  unsigned long long file_offset;
  unsigned long long size;
  char *error_string = "Error processing WRITEBUFFER command on line";
  int scanline = sscanf(rest_of_line, "%s %s %s %s %[^\n]", raw_file_name, raw_buffer_offset, raw_file_offset, raw_size, leftover);
  if (scanline != 4)
  {
    if (scanline < 4)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 4)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_file_name[0] == '$')
  {
    file_name = get_string_variable_value_ccc(raw_file_name);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    file_name = raw_file_name;
  }

  if (!perform_check && strlen(file_name) == 0)
  {
    fprintf (stderr, "\nError: File name is blank,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_buffer_offset[0] == '$')
  {
    buffer_offset = get_number_variable_value_ccc(raw_buffer_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    buffer_offset = strtoll(raw_buffer_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Buffer offset \'%s\' was unable to be completely processed.\n", raw_buffer_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_file_offset[0] == '$')
  {
    file_offset = get_number_variable_value_ccc(raw_file_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    file_offset = strtoll(raw_file_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! File offset \'%s\' was unable to be completely processed.\n", raw_file_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_size[0] == '$')
  {
    size = get_number_variable_value_ccc(raw_size);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    size = strtoll(raw_size, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (!perform_check && size + buffer_offset > ccc_main_buffer_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to copy past end of buffer,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }


  if (!perform_check)
  {
    FILE *createfile = fopen(file_name, "ab");
    fclose (createfile);
    FILE *writefile = fopen(file_name, "rb+");
    if (writefile == NULL)
    {
      fprintf(stderr, "Cannot open %s for writing (%s).\nAborting...\n", file_name, strerror(errno));
      return (-1);
    }
    if (fseeko(writefile, file_offset, SEEK_SET) != 0 )
    {
      fprintf(stderr, "Error seeking %s (%s).\n", file_name, strerror(errno));
      fclose(writefile);
      return (-1);
    }
    if (fwrite(ccc_buffer_ccc + buffer_offset, 1, size, writefile) != size )
    {
      fprintf(stderr, "Error writing to %s (%s).\nAborting...\n", file_name, strerror(errno));
      fclose(writefile);
      return (-1);
    }

    fclose (writefile);
  }

  return (0);
}









// function to read buffer from file
int read_buffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_file_name[length];
  char raw_buffer_offset[length];
  char raw_file_offset[length];
  char raw_size[length];
  char leftover[length];
  char* endptr;
  char *file_name;
  unsigned int buffer_offset;
  unsigned int file_offset;
  unsigned int size;
  char *error_string = "Error processing READBUFFER command on line";
  int scanline = sscanf(rest_of_line, "%s %s %s %s %[^\n]", raw_file_name, raw_buffer_offset, raw_file_offset, raw_size, leftover);
  if (scanline != 4)
  {
    if (scanline < 4)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 4)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_file_name[0] == '$')
  {
    file_name = get_string_variable_value_ccc(raw_file_name);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    file_name = raw_file_name;
  }

  if (!perform_check && strlen(file_name) == 0)
  {
    fprintf (stderr, "\nError: File name is blank,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_buffer_offset[0] == '$')
  {
    buffer_offset = get_number_variable_value_ccc(raw_buffer_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    buffer_offset = strtoll(raw_buffer_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Buffer offset \'%s\' was unable to be completely processed.\n", raw_buffer_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_file_offset[0] == '$')
  {
    file_offset = get_number_variable_value_ccc(raw_file_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    file_offset = strtoll(raw_file_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! File offset \'%s\' was unable to be completely processed.\n", raw_file_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_size[0] == '$')
  {
    size = get_number_variable_value_ccc(raw_size);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    size = strtoll(raw_size, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (!perform_check && size + buffer_offset > ccc_main_buffer_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to copy past end of buffer,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }


  if (!perform_check)
  {
    FILE *readfile = fopen(file_name, "rb");
    if (readfile == NULL)
    {
      fprintf(stderr, "Cannot open %s for reading (%s).\nAborting...\n", file_name, strerror(errno));
      return (-1);
    }
    if (fseeko(readfile, 0, SEEK_END) != 0 )
    {
      fprintf(stderr, "Error seeking %s (%s).\n", file_name, strerror(errno));
      fclose(readfile);
      return (-1);
    }
    long long file_size = ftello(readfile);
    if (size + buffer_offset > file_size)
    {
      fprintf (stderr, "ERROR: Attempt to read past end of file,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      fclose (readfile);
      return (-1);
    }

    if (fseeko(readfile, file_offset, SEEK_SET) != 0 )
    {
      fprintf(stderr, "Error seeking %s (%s).\n", file_name, strerror(errno));
      fclose(readfile);
      return (-1);
    }
    if (fread(ccc_buffer_ccc + buffer_offset, 1, size, readfile) != size )
    {
      fprintf(stderr, "Error reading from %s (%s).\nAborting...\n", file_name, strerror(errno));
      fclose(readfile);
      return (-1);
    }

    fclose (readfile);
  }

  return (0);
}









// function to set data transfer direction
int set_direction_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  //int length = strlen(rest_of_line);
  char raw_data[MAX_LINE_LENGTH];
  char leftover[MAX_LINE_LENGTH];
  char *error_string = "Error processing DIRECTION command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", raw_data, leftover);
  if (scanline != 1)
  {
    if (scanline < 1)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 1)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if ( (strcmp(raw_data, "none") == 0) || (strcmp(raw_data, "NONE") == 0) )
  {
    if(!perform_check)
    {
      passthrough_ccc.direction = 0;
      passthrough_ccc.transfer_direction = 1;
      passthrough_ccc.sg_xfer_direction = SG_DXFER_NONE;
    }
  }

  else if ( (strcmp(raw_data, "to") == 0) || (strcmp(raw_data, "TO") == 0) )
  {
    if(!perform_check)
    {
      passthrough_ccc.direction = 1;
      passthrough_ccc.transfer_direction = 0;
      passthrough_ccc.sg_xfer_direction = SG_DXFER_TO_DEV;
    }
  }

  else if ( (strcmp(raw_data, "from") == 0) || (strcmp(raw_data, "FROM") == 0) )
  {
    if(!perform_check)
    {
      passthrough_ccc.direction = 2;
      passthrough_ccc.transfer_direction = 1;
      passthrough_ccc.sg_xfer_direction = SG_DXFER_FROM_DEV;
    }
  }

  else if ( (strcmp(raw_data, "tofrom") == 0) || (strcmp(raw_data, "TOFROM") == 0) )
  {
    if(!perform_check)
    {
      passthrough_ccc.direction = 2;
      passthrough_ccc.transfer_direction = 1;
      passthrough_ccc.sg_xfer_direction = SG_DXFER_TO_FROM_DEV;
    }
  }

  else
  {
    fprintf (stderr, "\nError: \'%s\' is not a valid direction,\n", raw_data);
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  return (0);
}








int scsi_6_cmd_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int cmd_length = 6;
  int length = strlen(rest_of_line);
  char raw_b[cmd_length][length];
  char leftover[length];
  char* endptr;
  char *error_string = "Error processing SCSI6CMD command on line";

  int scanline = sscanf(rest_of_line, "%s %s %s %s %s %s %[^\n]", raw_b[0], raw_b[1], raw_b[2], raw_b[3], raw_b[4], raw_b[5], leftover);
  if (scanline != cmd_length)
  {
    if (scanline < cmd_length)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > cmd_length)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }


  int i;
  for (i = 0; i < cmd_length; i++)
  {
    if (raw_b[i][0] == '$')
    {
      unsigned long long temp = get_number_variable_value_ccc(raw_b[i]);
      if (return_value_ccc < 0)
      {
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
      passthrough_ccc.scsi_cmd[i] = temp;
    }
    else
    {
      unsigned long long temp = strtoll(raw_b[i], &endptr, 0);
      if (*endptr)
      {
        fprintf (stderr, "Error! Register \'%s\' was unable to be completely processed.\n", raw_b[i]);
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
      passthrough_ccc.scsi_cmd[i] = temp;
    }
  }


  if (!perform_check)
  {
    command_length_ccc = cmd_length;
    return_value_ccc = do_scsi_cmd_ccc(current_disk_ccc);
    return (return_value_ccc);
  }

  return (0);
}







int scsi_10_cmd_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int cmd_length = 10;
  int length = strlen(rest_of_line);
  char raw_b[cmd_length][length];
  char leftover[length];
  char* endptr;
  char *error_string = "Error processing SCSI10CMD command on line";

  int scanline = sscanf(rest_of_line, "%s %s %s %s %s %s %s %s %s %s %[^\n]", raw_b[0], raw_b[1], raw_b[2], raw_b[3], raw_b[4], raw_b[5], raw_b[6], raw_b[7], raw_b[8], raw_b[9], leftover);
  if (scanline != cmd_length)
  {
    if (scanline < cmd_length)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > cmd_length)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }


  int i;
  for (i = 0; i < cmd_length; i++)
  {
    if (raw_b[i][0] == '$')
    {
      unsigned long long temp = get_number_variable_value_ccc(raw_b[i]);
      if (return_value_ccc < 0)
      {
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
      passthrough_ccc.scsi_cmd[i] = temp;
    }
    else
    {
      unsigned long long temp = strtoll(raw_b[i], &endptr, 0);
      if (*endptr)
      {
        fprintf (stderr, "Error! Register \'%s\' was unable to be completely processed.\n", raw_b[i]);
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
      passthrough_ccc.scsi_cmd[i] = temp;
    }
  }


  if (!perform_check)
  {
    command_length_ccc = cmd_length;
    return_value_ccc = do_scsi_cmd_ccc(current_disk_ccc);
    return (return_value_ccc);
  }

  return (0);
}







int scsi_12_cmd_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int cmd_length = 12;
  int length = strlen(rest_of_line);
  char raw_b[cmd_length][length];
  char leftover[length];
  char* endptr;
  char *error_string = "Error processing SCSI12CMD command on line";

  int scanline = sscanf(rest_of_line, "%s %s %s %s %s %s %s %s %s %s %s %s %[^\n]", raw_b[0], raw_b[1], raw_b[2], raw_b[3], raw_b[4], raw_b[5], raw_b[6], raw_b[7], raw_b[8], raw_b[9], raw_b[10], raw_b[11], leftover);
  if (scanline != cmd_length)
  {
    if (scanline < cmd_length)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > cmd_length)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }


  int i;
  for (i = 0; i < cmd_length; i++)
  {
    if (raw_b[i][0] == '$')
    {
      unsigned long long temp = get_number_variable_value_ccc(raw_b[i]);
      if (return_value_ccc < 0)
      {
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
      passthrough_ccc.scsi_cmd[i] = temp;
    }
    else
    {
      unsigned long long temp = strtoll(raw_b[i], &endptr, 0);
      if (*endptr)
      {
        fprintf (stderr, "Error! Register \'%s\' was unable to be completely processed.\n", raw_b[i]);
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
      passthrough_ccc.scsi_cmd[i] = temp;
    }
  }


  if (!perform_check)
  {
    command_length_ccc = cmd_length;
    return_value_ccc = do_scsi_cmd_ccc(current_disk_ccc);
    return (return_value_ccc);
  }

  return (0);
}







int scsi_16_cmd_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int cmd_length = 16;
  int length = strlen(rest_of_line);
  char raw_b[cmd_length][length];
  char leftover[length];
  char* endptr;
  char *error_string = "Error processing SCSI16CMD command on line";

  int scanline = sscanf(rest_of_line, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %[^\n]", raw_b[0], raw_b[1], raw_b[2], raw_b[3], raw_b[4], raw_b[5], raw_b[6], raw_b[7], raw_b[8], raw_b[9], raw_b[10], raw_b[11], raw_b[12], raw_b[13], raw_b[14], raw_b[15], leftover);
  if (scanline != cmd_length)
  {
    if (scanline < cmd_length)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > cmd_length)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }


  int i;
  for (i = 0; i < cmd_length; i++)
  {
    if (raw_b[i][0] == '$')
    {
      unsigned long long temp = get_number_variable_value_ccc(raw_b[i]);
      if (return_value_ccc < 0)
      {
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
      passthrough_ccc.scsi_cmd[i] = temp;
    }
    else
    {
      unsigned long long temp = strtoll(raw_b[i], &endptr, 0);
      if (*endptr)
      {
        fprintf (stderr, "Error! Register \'%s\' was unable to be completely processed.\n", raw_b[i]);
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
      passthrough_ccc.scsi_cmd[i] = temp;
    }
  }


  if (!perform_check)
  {
    command_length_ccc = cmd_length;
    return_value_ccc = do_scsi_cmd_ccc(current_disk_ccc);
    return (return_value_ccc);
  }

  return (0);
}







// function to set protocol
int set_protocol_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_data[length];
  char leftover[length];
  char *error_string = "Error processing PROTOCOL command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", raw_data, leftover);
  if (scanline != 1)
  {
    if (scanline < 1)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 1)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if ( (strcmp(raw_data, "hard-reset") == 0) || (strcmp(raw_data, "HARD-RESET") == 0) )
  {
    if(!perform_check)
    {
      passthrough_ccc.protocol = 0;
    }
  }

  else if ( (strcmp(raw_data, "soft-reset") == 0) || (strcmp(raw_data, "SOFT-RESET") == 0) )
  {
    if(!perform_check)
    {
      passthrough_ccc.protocol = 1;
    }
  }

  else if ( (strcmp(raw_data, "non-data") == 0) || (strcmp(raw_data, "NON-DATA") == 0) )
  {
    if(!perform_check)
    {
      passthrough_ccc.protocol = 3;
    }
  }

  else if ( (strcmp(raw_data, "pio-data-in") == 0) || (strcmp(raw_data, "PIO-DATA-IN") == 0) )
  {
    if(!perform_check)
    {
      passthrough_ccc.protocol = 4;
    }
  }

  else if ( (strcmp(raw_data, "pio-data-out") == 0) || (strcmp(raw_data, "PIO-DATA-OUT") == 0) )
  {
    if(!perform_check)
    {
      passthrough_ccc.protocol = 5;
    }
  }

  else if ( (strcmp(raw_data, "dma") == 0) || (strcmp(raw_data, "DMA") == 0) )
  {
    if(!perform_check)
    {
      passthrough_ccc.protocol = 6;
    }
  }

  else if ( (strcmp(raw_data, "dma-queued") == 0) || (strcmp(raw_data, "DMA-QUEUED") == 0) )
  {
    if(!perform_check)
    {
      passthrough_ccc.protocol = 7;
    }
  }

  else if ( (strcmp(raw_data, "device-diagnostic") == 0) || (strcmp(raw_data, "DEVICE-DIAGNOSTIC") == 0) )
  {
    if(!perform_check)
    {
      passthrough_ccc.protocol = 8;
    }
  }

  else if ( (strcmp(raw_data, "device-reset") == 0) || (strcmp(raw_data, "DEVICE-RESET") == 0) )
  {
    if(!perform_check)
    {
      passthrough_ccc.protocol = 9;
    }
  }

  else if ( (strcmp(raw_data, "udma-data-in") == 0) || (strcmp(raw_data, "UDMA-DATA-IN") == 0) )
  {
    if(!perform_check)
    {
      passthrough_ccc.protocol = 10;
    }
  }

  else if ( (strcmp(raw_data, "udma-data-out") == 0) || (strcmp(raw_data, "UDMA-DATA-OUT") == 0) )
  {
    if(!perform_check)
    {
      passthrough_ccc.protocol = 11;
    }
  }

  else if ( (strcmp(raw_data, "fpdma") == 0) || (strcmp(raw_data, "FPDMA") == 0) )
  {
    if(!perform_check)
    {
      passthrough_ccc.protocol = 12;
    }
  }

  else if ( (strcmp(raw_data, "return-response-info") == 0) || (strcmp(raw_data, "RETURN-RESPONSE-INFO") == 0) )
  {
    if(!perform_check)
    {
      passthrough_ccc.protocol = 15;
    }
  }

  else
  {
    fprintf (stderr, "\nError: \'%s\' is not a valid protocol,\n", raw_data);
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  return (0);
}







// function to set transfer length field
int set_tlength_field_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_data[length];
  char leftover[length];
  char *error_string = "Error processing TLENGTHFIELD command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", raw_data, leftover);
  if (scanline != 1)
  {
    if (scanline < 1)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 1)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if ( (strcmp(raw_data, "none") == 0) || (strcmp(raw_data, "NONE") == 0) )
  {
    if(!perform_check)
    {
      passthrough_ccc.transfer_length = 0;
    }
  }

  else if ( (strcmp(raw_data, "features") == 0) || (strcmp(raw_data, "FEATURES") == 0) )
  {
    if(!perform_check)
    {
      passthrough_ccc.transfer_length = 1;
    }
  }

  else if ( (strcmp(raw_data, "sectorcount") == 0) || (strcmp(raw_data, "SECTORCOUNT") == 0) )
  {
    if(!perform_check)
    {
      passthrough_ccc.transfer_length = 2;
    }
  }

  else if ( (strcmp(raw_data, "stpsiu") == 0) || (strcmp(raw_data, "STPSIU") == 0) )
  {
    if(!perform_check)
    {
      passthrough_ccc.transfer_length = 3;
    }
  }

  else
  {
    fprintf (stderr, "\nError: \'%s\' is not a valid field,\n", raw_data);
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  return (0);
}







// function to set byte or block
int set_byte_block_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_data[length];
  char leftover[length];
  char *error_string = "Error processing TRANSFERUNIT command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", raw_data, leftover);
  if (scanline != 1)
  {
    if (scanline < 1)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 1)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if ( (strcmp(raw_data, "byte") == 0) || (strcmp(raw_data, "BYTE") == 0) )
  {
    if(!perform_check)
    {
      passthrough_ccc.byte_block = 0;
    }
  }

  else if ( (strcmp(raw_data, "block") == 0) || (strcmp(raw_data, "BLOCK") == 0) )
  {
    if(!perform_check)
    {
      passthrough_ccc.byte_block = 1;
    }
  }


  else
  {
    fprintf (stderr, "\nError: \'%s\' is not a valid unit,\n", raw_data);
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  return (0);
}







// function to set check condition
int set_check_condition_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_data[length];
  char leftover[length];
  char *error_string = "Error processing CHECKCONDITION command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", raw_data, leftover);
  if (scanline != 1)
  {
    if (scanline < 1)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 1)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if ( (strcmp(raw_data, "false") == 0) || (strcmp(raw_data, "FALSE") == 0) )
  {
    if(!perform_check)
    {
      passthrough_ccc.check_condition = 0;
    }
  }

  else if ( (strcmp(raw_data, "true") == 0) || (strcmp(raw_data, "TRUE") == 0) )
  {
    if(!perform_check)
    {
      passthrough_ccc.check_condition = 1;
    }
  }


  else
  {
    fprintf (stderr, "\nError: \'%s\' is not a valid option,\n", raw_data);
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  return (0);
}









int ata_28_cmd_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int cmd_length = 7;
  int length = strlen(rest_of_line);
  char raw_b[cmd_length][length];
  char leftover[length];
  char* endptr;
  char *error_string = "Error processing ATA28CMD command on line";

  int scanline = sscanf(rest_of_line, "%s %s %s %s %s %s %s %[^\n]", raw_b[0], raw_b[1], raw_b[2], raw_b[3], raw_b[4], raw_b[5], raw_b[6], leftover);
  if (scanline != cmd_length)
  {
    if (scanline < cmd_length)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > cmd_length)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }


  int i;
  for (i = 0; i < cmd_length; i++)
  {
    if (raw_b[i][0] == '$')
    {
      unsigned long long temp = get_number_variable_value_ccc(raw_b[i]);
      if (return_value_ccc < 0)
      {
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
      passthrough_ccc.ata_cmd[i] = temp;
    }
    else
    {
      unsigned long long temp = strtoll(raw_b[i], &endptr, 0);
      if (*endptr)
      {
        fprintf (stderr, "Error! Register \'%s\' was unable to be completely processed.\n", raw_b[i]);
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
      passthrough_ccc.ata_cmd[i] = temp;
    }
  }


  if (!perform_check)
  {
    command_length_ccc = 12;
    return_value_ccc = do_ata28_cmd_ccc(current_disk_ccc);
    return (return_value_ccc);
  }

  return (0);
}





int ata_48_cmd_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int cmd_length = 7;
  int length = strlen(rest_of_line);
  char raw_b[cmd_length][length];
  char leftover[length];
  char* endptr;
  char *error_string = "Error processing ATA48CMD command on line";

  int scanline = sscanf(rest_of_line, "%s %s %s %s %s %s %s %[^\n]", raw_b[0], raw_b[1], raw_b[2], raw_b[3], raw_b[4], raw_b[5], raw_b[6], leftover);
  if (scanline != cmd_length)
  {
    if (scanline < cmd_length)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > cmd_length)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }


  int i;
  unsigned int ata_word[cmd_length];
  for (i = 0; i < cmd_length; i++)
  {
    if (raw_b[i][0] == '$')
    {
      unsigned long long temp = get_number_variable_value_ccc(raw_b[i]);
      if (return_value_ccc < 0)
      {
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
      ata_word[i] = temp;
    }
    else
    {
      unsigned long long temp = strtoll(raw_b[i], &endptr, 0);
      if (*endptr)
      {
        fprintf (stderr, "Error! Register \'%s\' was unable to be completely processed.\n", raw_b[i]);
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
      ata_word[i] = temp;
      //fprintf (stdout, "temp=%llx ataword=%x\n", temp, ata_word[i]);
    }
  }


  if (!perform_check)
  {
    passthrough_ccc.ata_cmd[0] = ata_word[0] >> 8;        // feature high
    passthrough_ccc.ata_cmd[1] = ata_word[0] & 0xff;     // feature low
    passthrough_ccc.ata_cmd[2] = ata_word[1] >> 8;        // sector count high
    passthrough_ccc.ata_cmd[3] = ata_word[1] & 0xff;     // sector count low
    passthrough_ccc.ata_cmd[4] = ata_word[3] >> 8;        // LBA MH 31:24
    passthrough_ccc.ata_cmd[5] = ata_word[4] & 0xff;     // LBA LL 7:0
    passthrough_ccc.ata_cmd[6] = ata_word[2] & 0xff;      // LBA HL 39:32
    passthrough_ccc.ata_cmd[7] = ata_word[4] >> 8;        // LBA LH 15:8
    passthrough_ccc.ata_cmd[8] = ata_word[2] >> 8;        // LBA HH 47:40
    passthrough_ccc.ata_cmd[9] = ata_word[3] & 0xff;     // LBA ML 23:16
    passthrough_ccc.ata_cmd[10] = ata_word[5] & 0xff;    // Device
    passthrough_ccc.ata_cmd[11] = ata_word[6] & 0xff;    // Command

    //fprintf (stdout, "ataword=%x\n", ata_word[5]);
    //fprintf (stdout, "cmd11=%02x\n", passthrough_ccc.ata_cmd[11]);

    command_length_ccc = 16;
    return_value_ccc = do_ata48_cmd_ccc(current_disk_ccc);
    return (return_value_ccc);
  }

  return (0);
}






int write_log_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_file_name[length];
  char raw_variable_name[length];
  char leftover[length];
  char *file_name;
  char var_name[length];
  char *error_string = "Error processing WRITELOG command on line";
  int scanline = sscanf(rest_of_line, "%s %s %[^\n]", raw_file_name, raw_variable_name, leftover);
  if (scanline != 2)
  {
    if (scanline < 2)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 2)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_file_name[0] == '$')
  {
    file_name = get_string_variable_value_ccc(raw_file_name);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    file_name = raw_file_name;
  }

  if (!perform_check && strlen(file_name) == 0)
  {
    fprintf (stderr, "\nError: File name is blank,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_variable_name[0] != '$')
  {
    fprintf (stderr, "\nError: Variable name must start with \'$\',\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  scanline = sscanf(raw_variable_name+1, "%s", var_name);
  if (scanline != 1)
  {
    fprintf (stderr, "\nError processing variable,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  return_value_ccc = check_variable_ccc(var_name);
  if (return_value_ccc < 1)
  {
    fprintf (stderr, "\nError: Variable \'%s\' not found,\n", var_name);
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  else if (return_value_ccc != 2)
  {
    fprintf (stderr, "\nError: Expecting string variable but found \'%s\' to be wrong type,\n", raw_variable_name);
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }


  if (!perform_check)
  {
    char *value = get_string_variable_value_ccc(raw_variable_name);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

    FILE *writefile = fopen(file_name, "a");
    if (writefile == NULL)
    {
      fprintf(stderr, "Cannot open %s for writing (%s).\nAborting...\n", file_name, strerror(errno));
      return (-1);
    }

    if (fprintf(writefile, "%s\n", value) < 0 )
    {
      fprintf(stderr, "Error writing to %s (%s).\nAborting...\n", file_name, strerror(errno));
      fclose (writefile);
      return (-1);
    }

    fclose (writefile);
  }

  return (0);
}





int get_time_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char *error_string = "Error processing GETTIME command on line";

  if (length > 0)
  {
    fprintf (stderr, "rest_of_line= %s\n", rest_of_line);
    fprintf (stderr, "\nError: Too many arguments,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check)
  {
    return_value_ccc = get_the_time_ccc();
    if (return_value_ccc != 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  return (0);
}








int exit_with_code_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  char raw_exitcode[MAX_LINE_LENGTH];
  char leftover[MAX_LINE_LENGTH];
  unsigned char tempcode;
  char* endptr;
  char *error_string = "Error processing EXIT command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", raw_exitcode, leftover);
  if (scanline != 1)
  {
    fprintf (stderr, "\nError processing exitcode argument,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (raw_exitcode[0] == '$')
  {
    tempcode = get_number_variable_value_ccc(raw_exitcode);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    tempcode = strtoll(raw_exitcode, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Exitcode \'%s\' was unable to be completely processed.\n", raw_exitcode);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (!perform_check)
  {
    exitcode_ccc = tempcode;
    return (-101);
  }
  return (0);
}








int string_to_buffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  unsigned int length = strlen(rest_of_line);
  char raw_buffer_offset[length];
  char raw_variable_name[length];
  char raw_size[length];
  char leftover[length];
  char* endptr;
  unsigned int buffer_offset;
  char extracted_var[MAX_LINE_LENGTH];
  unsigned int size;
  char *error_string = "Error processing STRINGTOBUFFER command on line";

  int scanline = sscanf(rest_of_line, "%s %s %s %[^\n]", raw_buffer_offset, raw_size, raw_variable_name, leftover);
  if (scanline != 3)
  {
    if (scanline < 3)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 3)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_buffer_offset[0] == '$')
  {
    buffer_offset = get_number_variable_value_ccc(raw_buffer_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    buffer_offset = strtoll(raw_buffer_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Buffer offset \'%s\' was unable to be completely processed.\n", raw_buffer_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_size[0] == '$')
  {
    size = get_number_variable_value_ccc(raw_size);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    size = strtoll(raw_size, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_variable_name[0] == '$')
  {
    char var_name[MAX_VARIABLE_NAME_LENGTH];
    sscanf(raw_variable_name+1, "%s", var_name);
    return_value_ccc = check_variable_ccc(var_name);
    if (return_value_ccc < 1)
    {
      fprintf (stderr, "\nError: Variable \'%s\' not found,\n", raw_variable_name);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
    else if (return_value_ccc != 2)
    {
      fprintf (stderr, "\nError: Variable \'%s\' is wrong type (expecting string variable),\n", raw_variable_name);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
    else
    {
      int var_num = find_string_variable_ccc(var_name);
      strcpy(extracted_var, string_variable_pointer_ccc[var_num]);
      length = strlen(extracted_var);
    }
  }
  else
  {
    fprintf (stderr, "\nError: Variable name must start with \'$\',\n");
    fprintf (stderr, "Error processing \'%s\'\n", raw_variable_name);
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }


  if (size + buffer_offset > ccc_main_buffer_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to write past end of buffer,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check)
  {
    unsigned int i;
    for (i = 0; i < size && i < length; i++)
    {
      unsigned char c = extracted_var[i];
      memcpy(ccc_buffer_ccc + buffer_offset + i, &c, 1);
    }
  }

  return (0);
}






int setup_pio_read_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char *error_string = "Error processing SETREADPIO command on line";
  if (length > 0)
  {
    fprintf (stderr, "rest_of_line= %s\n", rest_of_line);
    fprintf (stderr, "\nError: Too many arguments,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check)
  {
    passthrough_ccc.transfer_length = 2;    // sector field
    passthrough_ccc.byte_block = 1;    // block
    passthrough_ccc.protocol = 4;    // pio-data-in
    passthrough_ccc.check_condition = 1;    // check condition true
    // set directions for read unless buffer is 0
    if (ccc_main_buffer_size_ccc > 0)
    {
      passthrough_ccc.direction = 2;    // from
      passthrough_ccc.transfer_direction = 1;    // from
      passthrough_ccc.sg_xfer_direction = SG_DXFER_FROM_DEV;
    }
    else
    {
      passthrough_ccc.direction = 0;    // none
      passthrough_ccc.protocol = 3;    // none-data
      passthrough_ccc.transfer_direction = 1;    // from
      passthrough_ccc.sg_xfer_direction = SG_DXFER_NONE;
    }
  }
  return (0);
}






int setup_dma_read_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char *error_string = "Error processing SETREADDMA command on line";
  if (length > 0)
  {
    fprintf (stderr, "rest_of_line= %s\n", rest_of_line);
    fprintf (stderr, "\nError: Too many arguments,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check)
  {
    passthrough_ccc.transfer_length = 2;    // sector field
    passthrough_ccc.byte_block = 1;    // block
    passthrough_ccc.protocol = 6;    // dma
    passthrough_ccc.check_condition = 1;    // check condition true
    passthrough_ccc.direction = 2;    // from
    passthrough_ccc.transfer_direction = 1;    // from
    passthrough_ccc.sg_xfer_direction = SG_DXFER_FROM_DEV;
  }
  return (0);
}






int setup_pio_write_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char *error_string = "Error processing SETWRITEPIO command on line";
  if (length > 0)
  {
    fprintf (stderr, "rest_of_line= %s\n", rest_of_line);
    fprintf (stderr, "\nError: Too many arguments,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check)
  {
    passthrough_ccc.transfer_length = 2;    // sector field
    passthrough_ccc.byte_block = 1;    // block
    passthrough_ccc.protocol = 5;    // pio-data-out
    passthrough_ccc.check_condition = 1;    // check condition true
    // set directions for write unless buffer is 0
    if (ccc_main_buffer_size_ccc > 0)
    {
      passthrough_ccc.direction = 1;    // to
      passthrough_ccc.transfer_direction = 0;    // to
      passthrough_ccc.sg_xfer_direction = SG_DXFER_TO_DEV;
    }
    else
    {
      passthrough_ccc.direction = 0;    // none
      passthrough_ccc.protocol = 3;    // none-data
      passthrough_ccc.transfer_direction = 0;    // to
      passthrough_ccc.sg_xfer_direction = SG_DXFER_NONE;
    }
  }
  return (0);
}






int setup_dma_write_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char *error_string = "Error processing SETWRITEDMA command on line";
  if (length > 0)
  {
    fprintf (stderr, "rest_of_line= %s\n", rest_of_line);
    fprintf (stderr, "\nError: Too many arguments,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check)
  {
    passthrough_ccc.transfer_length = 2;    // sector field
    passthrough_ccc.byte_block = 1;    // block
    passthrough_ccc.protocol = 6;    // dma
    passthrough_ccc.check_condition = 1;    // check condition true
    passthrough_ccc.direction = 1;    // to
    passthrough_ccc.transfer_direction = 0;    // to
    passthrough_ccc.sg_xfer_direction = SG_DXFER_TO_DEV;
  }
  return (0);
}






int get_status_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char *error_string = "Error processing GETSTATUS command on line";
  if (length > 0)
  {
    fprintf (stderr, "rest_of_line= %s\n", rest_of_line);
    fprintf (stderr, "\nError: Too many arguments,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check)
  {
    refresh_status_ccc(current_disk_ccc);
  }
  return (0);
}





int perform_soft_reset_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char *error_string = "Error processing SOFTRESET command on line";
  if (length > 0)
  {
    fprintf (stderr, "rest_of_line= %s\n", rest_of_line);
    fprintf (stderr, "\nError: Too many arguments,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check)
  {
    do_soft_reset_ccc(current_disk_ccc);
  }
  return (0);
}





int perform_hard_reset_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char *error_string = "Error processing HARDRESET command on line";
  if (length > 0)
  {
    fprintf (stderr, "rest_of_line= %s\n", rest_of_line);
    fprintf (stderr, "\nError: Too many arguments,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check)
  {
    do_hard_reset_ccc(current_disk_ccc);
  }
  return (0);
}





int u_sleep_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_time[length];
  char leftover[length];
  char* endptr;
  unsigned long long time;
  char *error_string = "Error processing SLEEP command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", raw_time, leftover);
  if (scanline != 1)
  {
    fprintf (stderr, "\nError processing time argument,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (raw_time[0] == '$')
  {
    time = get_number_variable_value_ccc(raw_time);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    time = strtoull(raw_time, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Time \'%s\' was unable to be completely processed.\n", raw_time);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (!perform_check)
  {
    time = time * 1000;
    do_nanosleep_ccc(time);
  }

  return (0);
}





int busytimeout_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_time[length];
  char leftover[length];
  char* endptr;
  unsigned long long time;
  char *error_string = "Error processing BUSYTIMEOUT command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", raw_time, leftover);
  if (scanline != 1)
  {
    fprintf (stderr, "\nError processing time argument,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (raw_time[0] == '$')
  {
    time = get_number_variable_value_ccc(raw_time);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    time = strtoull(raw_time, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Time \'%s\' was unable to be completely processed.\n", raw_time);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (!perform_check)
  {
    busy_wait_time_ccc = time;
  }

  return (0);
}






int resettimeout_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_time[length];
  char leftover[length];
  char* endptr;
  unsigned long long time;
  char *error_string = "Error processing RESETTIMEOUT command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", raw_time, leftover);
  if (scanline != 1)
  {
    fprintf (stderr, "\nError processing time argument,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (raw_time[0] == '$')
  {
    time = get_number_variable_value_ccc(raw_time);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    time = strtoull(raw_time, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Time \'%s\' was unable to be completely processed.\n", raw_time);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (!perform_check)
  {
    reset_wait_time_ccc = time;
  }

  return (0);
}






int softtimeout_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  //int length = strlen(rest_of_line);
  char raw_time[MAX_LINE_LENGTH];
  char leftover[MAX_LINE_LENGTH];
  char* endptr;
  unsigned long long time;
  char *error_string = "Error processing SOFTTIMEOUT command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", raw_time, leftover);
  if (scanline != 1)
  {
    fprintf (stderr, "\nError processing time argument,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (raw_time[0] == '$')
  {
    time = get_number_variable_value_ccc(raw_time);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    time = strtoull(raw_time, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Time \'%s\' was unable to be completely processed.\n", raw_time);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (!perform_check)
  {
    soft_reset_time_ccc = time;
  }

  return (0);
}





int hardtimeout_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  //int length = strlen(rest_of_line);
  char raw_time[MAX_LINE_LENGTH];
  char leftover[MAX_LINE_LENGTH];
  char* endptr;
  unsigned long long time;
  char *error_string = "Error processing HARDTIMEOUT command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", raw_time, leftover);
  if (scanline != 1)
  {
    fprintf (stderr, "\nError processing time argument,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (raw_time[0] == '$')
  {
    time = get_number_variable_value_ccc(raw_time);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    time = strtoull(raw_time, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Time \'%s\' was unable to be completely processed.\n", raw_time);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (!perform_check)
  {
    hard_reset_time_ccc = time;
  }

  return (0);
}





int generaltimeout_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_time[length];
  char leftover[length];
  char* endptr;
  unsigned long long time;
  char *error_string = "Error processing GENERALTIMEOUT command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", raw_time, leftover);
  if (scanline != 1)
  {
    fprintf (stderr, "\nError processing time argument,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (raw_time[0] == '$')
  {
    time = get_number_variable_value_ccc(raw_time);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    time = strtoull(raw_time, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Time \'%s\' was unable to be completely processed.\n", raw_time);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (!perform_check)
  {
    general_timeout_ccc = time;
  }

  return (0);
}






int load_script_file_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  //int length = strlen(rest_of_line);
  char raw_data[MAX_LINE_LENGTH];
  char leftover[MAX_LINE_LENGTH];
  char *error_string = "Error processing LOADSCRIPT command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", raw_data, leftover);
  if (scanline != 1)
  {
    if (scanline < 1)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 1)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if(!perform_check)
  {
    //fprintf (stdout, "next, count=%d script=%s\n", script_count_ccc, script_file_ccc);
    // copy the current script name to list
    if (script_count_ccc < MAX_SCRIPT_DEPTH)
    {
      strcpy (script_list_ccc[script_count_ccc], current_script_ccc);
      script_count_ccc++;
    }
    else
    {
      fprintf (stderr, "\nError: Script level exceeded maximum (%d),\n", MAX_SCRIPT_DEPTH);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

    // set the new script file name to load
    strcpy (current_script_ccc, raw_data);
    script_file_ccc = current_script_ccc;
    //fprintf (stdout, "next, count=%d script=%s\n", script_count_ccc, script_file_ccc);
    int i;
    for (i = 0; i < script_count_ccc; i++)
      {
        //fprintf (stdout, "count=%d script=%s\n", i, script_list_ccc[i]);
      }

    // clear the stack counters
    subroutine_stack_counter_ccc = 0;
    subroutine_count_ccc = 0;
    return_count_ccc = 0;
    endif_stack_counter_ccc = 0;
    while_stack_counter_ccc = 0;

    // return with value that will cause new script to be loaded
    return (-200);
  }

  return (0);
}






int previous_script_file_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char *error_string = "Error processing PREVIOUSSCRIPT command on line";
  if (length > 0)
  {
    fprintf (stderr, "\nError: Too many arguments,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if(!perform_check)
  {
    script_count_ccc--;
    if (script_count_ccc >= 0)
    {
      // set the previous script file name to load
      strcpy (current_script_ccc, script_list_ccc[script_count_ccc]);
      script_file_ccc = current_script_ccc;
      //fprintf (stdout, "previous, count=%d script=%s\n", script_count_ccc, script_file_ccc);
      int i;
      for (i = 0; i < script_count_ccc; i++)
      {
        //fprintf (stdout, "count=%d script=%s\n", i, script_list_ccc[i]);
      }

      // clear the stack counters
      subroutine_stack_counter_ccc = 0;
      subroutine_count_ccc = 0;
      return_count_ccc = 0;
      endif_stack_counter_ccc = 0;
      while_stack_counter_ccc = 0;

      // return with value that will cause new script to be loaded
      return (-200);
    }
    else
    {
      fprintf (stdout, "No previous script to load.\n");
      return (-102);
    }
  }

  return (0);
}





int include_script_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  //int length = strlen(rest_of_line);
  char raw_data[MAX_LINE_LENGTH];
  char leftover[MAX_LINE_LENGTH];
  char *error_string = "Error processing INCLUDE command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", raw_data, leftover);
  if (scanline != 1)
  {
    if (scanline < 1)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 1)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if(perform_check)
  {
    append_script_ccc = 1;
    read_script_file_ccc(raw_data);
  }

  return (0);
}






int string_to_scratchpad_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  unsigned int length = strlen(rest_of_line);
  char raw_scratchpad_offset[length];
  char raw_variable_name[length];
  char raw_size[length];
  char leftover[length];
  char* endptr;
  unsigned int scratchpad_offset;
  char extracted_var[MAX_LINE_LENGTH];
  unsigned int size;
  char *error_string = "Error processing STRINGTOBUFFER command on line";

  int scanline = sscanf(rest_of_line, "%s %s %s %[^\n]", raw_scratchpad_offset, raw_size, raw_variable_name, leftover);
  if (scanline != 3)
  {
    if (scanline < 3)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 3)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_scratchpad_offset[0] == '$')
  {
    scratchpad_offset = get_number_variable_value_ccc(raw_scratchpad_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    scratchpad_offset = strtoll(raw_scratchpad_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Buffer offset \'%s\' was unable to be completely processed.\n", raw_scratchpad_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_size[0] == '$')
  {
    size = get_number_variable_value_ccc(raw_size);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    size = strtoll(raw_size, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_variable_name[0] == '$')
  {
    char var_name[MAX_VARIABLE_NAME_LENGTH];
    sscanf(raw_variable_name+1, "%s", var_name);
    return_value_ccc = check_variable_ccc(var_name);
    if (return_value_ccc < 1)
    {
      fprintf (stderr, "\nError: Variable \'%s\' not found,\n", raw_variable_name);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
    else if (return_value_ccc != 2)
    {
      fprintf (stderr, "\nError: Variable \'%s\' is wrong type (expecting string variable),\n", raw_variable_name);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
    else
    {
      int var_num = find_string_variable_ccc(var_name);
      strcpy(extracted_var, string_variable_pointer_ccc[var_num]);
      length = strlen(extracted_var);
    }
  }
  else
  {
    fprintf (stderr, "\nError: Variable name must start with \'$\',\n");
    fprintf (stderr, "Error processing \'%s\'\n", raw_variable_name);
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }


  if (size + scratchpad_offset > ccc_main_scratchpad_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to write past end of scratchpad,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check)
  {
    unsigned int i;
    for (i = 0; i < size && i < length; i++)
    {
      unsigned char c = extracted_var[i];
      memcpy(ccc_scratchpad_ccc + scratchpad_offset + i, &c, 1);
    }
  }

  return (0);
}







int wordflip_buffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_offset[length];
  char raw_size[length];
  char leftover[length];
  unsigned int offset;
  unsigned int size;
  char *error_string = "Error processing WORDFLIPBUFFER command on line";
  int scanline = sscanf(rest_of_line, "%s %s %[^\n]", raw_offset, raw_size, leftover);
  if (scanline != 2)
  {
    if (scanline < 2)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 2)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (raw_offset[0] == '$')
  {
    offset = get_number_variable_value_ccc(raw_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    char* endptr;
    offset = strtoll(raw_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Offset \'%s\' was unable to be completely processed.\n", raw_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  if (raw_size[0] == '$')
  {
    size = get_number_variable_value_ccc(raw_size);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    char* endptr;
    size = strtoll(raw_size, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  if (size + offset > ccc_main_buffer_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to flip past end of buffer,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check)
  {
    unsigned int i;
    for (i = 0; i < size-1; i+=2)
    {
      unsigned char a;
      unsigned char b;
      memcpy(&a, ccc_buffer_ccc+i+offset, 1);
      memcpy(&b, ccc_buffer_ccc+i+offset+1, 1);
      memcpy(ccc_buffer_ccc+i+offset, &b, 1);
      memcpy(ccc_buffer_ccc+i+offset+1, &a, 1);
    }
  }

  return (0);
}






int wordflip_scratchpad_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_offset[length];
  char raw_size[length];
  char leftover[length];
  unsigned int offset;
  unsigned int size;
  char *error_string = "Error processing WORDFLIPSCRATCHPAD command on line";
  int scanline = sscanf(rest_of_line, "%s %s %[^\n]", raw_offset, raw_size, leftover);
  if (scanline != 2)
  {
    if (scanline < 2)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 2)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (raw_offset[0] == '$')
  {
    offset = get_number_variable_value_ccc(raw_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    char* endptr;
    offset = strtoll(raw_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Offset \'%s\' was unable to be completely processed.\n", raw_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  if (raw_size[0] == '$')
  {
    size = get_number_variable_value_ccc(raw_size);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    char* endptr;
    size = strtoll(raw_size, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  if (size + offset > ccc_main_scratchpad_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to flip past end of scratchpad,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check)
  {
    unsigned int i;
    for (i = 0; i < size-1; i+=2)
    {
      unsigned char a;
      unsigned char b;
      memcpy(&a, ccc_scratchpad_ccc+i+offset, 1);
      memcpy(&b, ccc_scratchpad_ccc+i+offset+1, 1);
      memcpy(ccc_scratchpad_ccc+i+offset, &b, 1);
      memcpy(ccc_scratchpad_ccc+i+offset+1, &a, 1);
    }
  }

  return (0);
}








// function to get file size, which is placed into the variable error_level
int get_file_size_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  //int length = strlen(rest_of_line);
  char raw_file_name[MAX_LINE_LENGTH];
  char leftover[MAX_LINE_LENGTH];
  char *file_name;
  char *error_string = "Error processing GETFILESIZE command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", raw_file_name, leftover);
  if (scanline != 1)
  {
    if (scanline < 1)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 1)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_file_name[0] == '$')
  {
    file_name = get_string_variable_value_ccc(raw_file_name);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    file_name = raw_file_name;
  }

  if (!perform_check && strlen(file_name) == 0)
  {
    fprintf (stderr, "\nError: File name is blank,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }


  if (!perform_check)
  {
    int var_num = find_number_variable_ccc("error_level");
    if (var_num < 0)
    {
      fprintf (stderr, "\nError: Unable to locate internal variable \'error_level\',\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

    FILE *readfile = fopen(file_name, "rb");
    if (readfile == NULL)
    {
      //fprintf(stderr, "Cannot open %s for reading (%s).\n", file_name, strerror(errno));
      number_variable_buffer_ccc[var_num] = -1;
    }
    else if (fseeko(readfile, 0, SEEK_END) != 0 )
    {
      //fprintf(stderr, "Error seeking %s (%s).\n", file_name, strerror(errno));
      number_variable_buffer_ccc[var_num] = -1;
      fclose (readfile);
    }
    else
    {
      number_variable_buffer_ccc[var_num] = ftello(readfile);
      fclose (readfile);
    }

    //fprintf (stdout, "error_level=%lld\n", error_level);
  }

  return (0);
}








// function to write scratchpad to file
int write_scratchpad_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_file_name[length];
  char raw_buffer_offset[length];
  char raw_file_offset[length];
  char raw_size[length];
  char leftover[length];
  char* endptr;
  char *file_name;
  unsigned long long buffer_offset;
  unsigned long long file_offset;
  unsigned long long size;
  char *error_string = "Error processing WRITESCRATCHPAD command on line";
  int scanline = sscanf(rest_of_line, "%s %s %s %s %[^\n]", raw_file_name, raw_buffer_offset, raw_file_offset, raw_size, leftover);
  if (scanline != 4)
  {
    if (scanline < 4)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 4)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_file_name[0] == '$')
  {
    file_name = get_string_variable_value_ccc(raw_file_name);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    file_name = raw_file_name;
  }

  if (!perform_check && strlen(file_name) == 0)
  {
    fprintf (stderr, "\nError: File name is blank,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_buffer_offset[0] == '$')
  {
    buffer_offset = get_number_variable_value_ccc(raw_buffer_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    buffer_offset = strtoll(raw_buffer_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Buffer offset \'%s\' was unable to be completely processed.\n", raw_buffer_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_file_offset[0] == '$')
  {
    file_offset = get_number_variable_value_ccc(raw_file_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    file_offset = strtoll(raw_file_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! File offset \'%s\' was unable to be completely processed.\n", raw_file_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_size[0] == '$')
  {
    size = get_number_variable_value_ccc(raw_size);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    size = strtoll(raw_size, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (!perform_check && size + buffer_offset > ccc_main_scratchpad_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to copy past end of scratchpad,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }


  if (!perform_check)
  {
    FILE *createfile = fopen(file_name, "ab");
    fclose (createfile);
    FILE *writefile = fopen(file_name, "rb+");
    if (writefile == NULL)
    {
      fprintf(stderr, "Cannot open \'%s\' for writing (%s).\n", file_name, strerror(errno));
      return (-1);
    }
    if (fseeko(writefile, file_offset, SEEK_SET) != 0 )
    {
      fprintf(stderr, "Error seeking \'%s\' (%s).\n", file_name, strerror(errno));
      fclose(writefile);
      return (-1);
    }
    if (fwrite(ccc_scratchpad_ccc + buffer_offset, 1, size, writefile) != size )
    {
      fprintf(stderr, "Error writing to \'%s\' (%s).\n", file_name, strerror(errno));
      fclose(writefile);
      return (-1);
    }

    fclose (writefile);
  }

  return (0);
}









// function to read scratchpad from file
int read_scratchpad_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_file_name[length];
  char raw_buffer_offset[length];
  char raw_file_offset[length];
  char raw_size[length];
  char leftover[length];
  char* endptr;
  char *file_name;
  unsigned int buffer_offset;
  unsigned int file_offset;
  unsigned int size;
  char *error_string = "Error processing READSCRATCHPAD command on line";
  int scanline = sscanf(rest_of_line, "%s %s %s %s %[^\n]", raw_file_name, raw_buffer_offset, raw_file_offset, raw_size, leftover);
  if (scanline != 4)
  {
    if (scanline < 4)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 4)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_file_name[0] == '$')
  {
    file_name = get_string_variable_value_ccc(raw_file_name);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    file_name = raw_file_name;
  }

  if (!perform_check && strlen(file_name) == 0)
  {
    fprintf (stderr, "\nError: File name is blank,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_buffer_offset[0] == '$')
  {
    buffer_offset = get_number_variable_value_ccc(raw_buffer_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    buffer_offset = strtoll(raw_buffer_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Buffer offset \'%s\' was unable to be completely processed.\n", raw_buffer_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_file_offset[0] == '$')
  {
    file_offset = get_number_variable_value_ccc(raw_file_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    file_offset = strtoll(raw_file_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! File offset \'%s\' was unable to be completely processed.\n", raw_file_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_size[0] == '$')
  {
    size = get_number_variable_value_ccc(raw_size);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    size = strtoll(raw_size, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (!perform_check && size + buffer_offset > ccc_main_scratchpad_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to copy past end of scratchpad,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }


  if (!perform_check)
  {
    FILE *readfile = fopen(file_name, "rb");
    if (readfile == NULL)
    {
      fprintf(stderr, "Cannot open %s for reading (%s).\nAborting...\n", file_name, strerror(errno));
      return (-1);
    }
    if (fseeko(readfile, 0, SEEK_END) != 0 )
    {
      fprintf(stderr, "Error seeking %s (%s).\n", file_name, strerror(errno));
      fclose(readfile);
      return (-1);
    }
    long long file_size = ftello(readfile);
    if (size + buffer_offset > file_size)
    {
      fprintf (stderr, "ERROR: Attempt to read past end of file,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      fclose (readfile);
      return (-1);
    }

    if (fseeko(readfile, file_offset, SEEK_SET) != 0 )
    {
      fprintf(stderr, "Error seeking \'%s\' (%s).\n", file_name, strerror(errno));
      fclose(readfile);
      return (-1);
    }
    if (fread(ccc_scratchpad_ccc + buffer_offset, 1, size, readfile) != size )
    {
      fprintf(stderr, "Error reading from \'%s\' (%s).\n", file_name, strerror(errno));
      fclose(readfile);
      return (-1);
    }

    fclose (readfile);
  }

  return (0);
}








// function to delete file
int delete_file_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  //int length = strlen(rest_of_line);
  char raw_file_name[MAX_LINE_LENGTH];
  char leftover[MAX_LINE_LENGTH];
  char *file_name;
  char *error_string = "Error processing DELETEFILE command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", raw_file_name, leftover);
  if (scanline != 1)
  {
    if (scanline < 1)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 1)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_file_name[0] == '$')
  {
    file_name = get_string_variable_value_ccc(raw_file_name);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    file_name = raw_file_name;
  }

  if (!perform_check && strlen(file_name) == 0)
  {
    fprintf (stderr, "\nError: File name is blank,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }


  if (!perform_check)
  {
    remove (file_name);
  }

  return (0);
}








// function to perform an external command
int call_command_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  //int length = strlen(rest_of_line);
  char raw_command[MAX_LINE_LENGTH];
  char leftover[MAX_LINE_LENGTH];
  char *command;
  char *error_string = "Error processing CALLCOMMAND command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", raw_command, leftover);
  if (scanline != 1)
  {
    if (scanline < 1)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 1)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_command[0] == '$')
  {
    command = get_string_variable_value_ccc(raw_command);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    command = raw_command;
  }
  int var_num = find_number_variable_ccc("error_level");
  if (var_num < 0)
  {
    fprintf (stderr, "\nError: Unable to locate internal variable \'error_level\',\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (!perform_check)
  {
    number_variable_buffer_ccc[var_num] = WEXITSTATUS(system (command));
  }

  return (0);
}








// function to get user input
int user_input_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  //int length = strlen(rest_of_line);
  char raw_variable_name[MAX_LINE_LENGTH];
  char leftover[MAX_LINE_LENGTH];
  char *error_string = "Error processing USERINPUT command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", raw_variable_name, leftover);
  if (scanline != 1)
  {
    if (scanline < 1)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 1)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_variable_name[0] == '$')
  {
    return_value_ccc = create_string_variable_ccc(raw_variable_name+1);
    if (return_value_ccc != 0)
    {
      if (return_value_ccc == -1)
      {
        fprintf (stderr, "Error: Variable \'%s\' has been declared more than once,\n", raw_variable_name);
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
      else if (return_value_ccc != 2)
      {
        fprintf (stderr, "Error: Variable \'%s\' has been declared previously as a different type,\n", raw_variable_name);
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
    }

    int var_num = find_string_variable_ccc(raw_variable_name+1);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "Error: Variable \'%s\' not found,\n", raw_variable_name);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
    if (!perform_check)
    {
      fprintf (stdout, "> ");
      char input_text[MAX_VARIABLE_LENGTH];
      fflush(stdout);
      if ( fgets(input_text, sizeof input_text, stdin) != NULL )
      {
        char *newline = strchr(input_text, '\n'); // search for newline character
        if ( newline != NULL )
        {
          *newline = '\0'; // overwrite trailing newline
        }
      }
      strcpy(string_variable_pointer_ccc[var_num], input_text);
      fprintf (stdout, "%s\n", input_text);
    }
  }

  else
  {
    fprintf (stderr, "\nError: Expecting string variable but found \'%s\',\n", raw_variable_name);
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  return (0);
}






int setmainscratchpad_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  //int length = strlen(rest_of_line);
  char raw_offset[MAX_LINE_LENGTH];
  char leftover[MAX_LINE_LENGTH];
  char* endptr;
  unsigned long long offset;
  char *error_string = "Error processing SETSCRATCHPAD command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", raw_offset, leftover);
  if (scanline != 1)
  {
    fprintf (stderr, "\nError processing offset argument,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (raw_offset[0] == '$')
  {
    offset = get_number_variable_value_ccc(raw_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    offset = strtoll(raw_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Offset \'%s\' was unable to be completely processed.\n", raw_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  if (perform_check)
  {
    setting_scratchpad_ccc = true;
    int find_next_setscratch = find_command_ccc("setscratchpad", line_number+1);
    int find_next_endscratch = find_command_ccc("endscratchpad", line_number+1);
    if (find_next_endscratch < 1)
    {
      fprintf (stderr, "\nError, SETSCRATCHPAD without ENDSCRATCHPAD,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
    if (find_next_setscratch > 1 && find_next_setscratch < find_next_endscratch)
    {
      fprintf (stderr, "\nError: Next SETSCRATCHPAD found before ENDSCRATCHPAD on line %d,\n", find_next_setscratch+1);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  int current_line = line_number+1;
  char *full_line;
  char input_text[MAX_LINE_LENGTH];
  memset(input_text,0,sizeof(input_text));
  if (command_line_ccc)
  {
    fprintf (stdout, "setscratchpad> ");
    fflush(stdout);
    full_line = fgets(input_text, sizeof input_text, stdin);
  }
  else
  {
    full_line = get_full_line_ccc(current_line, input_text);
  }
  char line[MAX_LINE_LENGTH];
  strcpy (line, full_line);
  char raw_byte[MAX_LINE_LENGTH];
  unsigned int byte_count = 0;
  unsigned int byte_offset = 0;
  while (1)
  {
    //fprintf (stdout, "line=%s\n", line);
    strcpy (raw_byte, "");
    strcpy (leftover, "");
    sscanf(line, "%s %[^\n]", raw_byte, leftover);
    //fprintf (stdout, "raw_byte=%s  leftover=%s\n", raw_byte, leftover);
    if ( (strcmp(raw_byte, "endscratchpad") == 0) || (strcmp(raw_byte, "ENDSCRATCHPAD") == 0) )
    {
      break;
    }
    int length = strlen(raw_byte);
    if (length <= 0)
    {
      current_line++;
      char *full_line;
      char input_text[MAX_LINE_LENGTH];
      input_text[0]='\0';
      if (command_line_ccc)
      {
        fprintf (stdout, "setscratchpad> ");
        fflush(stdout);
        full_line = fgets(input_text, sizeof input_text, stdin);
      }
      else
      {
        full_line = get_full_line_ccc(current_line, input_text);
      }
      strcpy (line, full_line);
    }
    else if (raw_byte[length-1] == ':')
    {
      byte_offset = strtoll(raw_byte, NULL, 16);
      strcpy (line, leftover);
      byte_count = 0;
    }
    else
    {
      if (!perform_check &&  byte_count +1 + byte_offset + offset > ccc_main_scratchpad_size_ccc)
      {
        fprintf (stderr, "Error! End of scratchpad reached at \'%s\' on line %d.\n", raw_byte, current_line+1);
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
      unsigned long long value;
      if (raw_byte[0] == '$')
      {
        value = get_number_variable_value_ccc(raw_byte);
        if (return_value_ccc < 0)
        {
          fprintf (stderr, "%s %d.\n", error_string, line_number-1);
          return (-1);
        }
      }
      else
      {
        char* endptr;
        value = strtoull(raw_byte, &endptr, 16);
        if (*endptr)
        {
          fprintf (stderr, "Error! Byte \'%s\' on line %d could not be processed.\n", raw_byte, current_line+1);
          fprintf (stderr, "%s %d.\n", error_string, line_number-1);
          return (-1);
        }
      }
      int high_value = 0;
      if (value > 255)
      {
        if ( byte_count + 8 + byte_offset + offset > ccc_main_scratchpad_size_ccc)
        {
          fprintf (stderr, "Error! End of scratchpad reached at \'%s\' on line %d.\n", raw_byte, current_line+1);
          fprintf (stderr, "%s %d.\n", error_string, line_number-1);
          return (-1);
        }
        high_value = 1;
      }
      if (!perform_check)
      {
        if (high_value)
        {
          unsigned long long c = value;
          memcpy(ccc_scratchpad_ccc + byte_offset + offset + byte_count, &c, 8);
        }
        else
        {
          unsigned char c = value;
          memcpy(ccc_scratchpad_ccc + byte_offset + offset + byte_count, &c, 1);
        }
      }
      strcpy (line, leftover);
      byte_count++;
    }
  }
  if (!perform_check)
  {
    return (current_line-1);
  }
  return (0);
}






int endmainscratchpad_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  char *error_string = "Error processing ENDSCRATCHPAD command on line";
  if (perform_check)
  {
    if (!setting_scratchpad_ccc)
    {
      fprintf (stderr, "\nError, ENDSCRATCHPAD without SETSCRATCHPAD,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
    int length = strlen(rest_of_line);
    if (length > 0)
    {
      fprintf (stderr, "\nError, there should be no arguments after the ENDSCRATCHPAD command,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
    setting_scratchpad_ccc = false;
  }

  return (0);
}






int change_main_scratchpad_size_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  //int length = strlen(rest_of_line);
  char raw_size[MAX_LINE_LENGTH];
  char leftover[MAX_LINE_LENGTH];
  char* endptr;
  unsigned long long size;
  bool size_is_variable = false;
  char *error_string = "Error processing SCRATCHPADSIZE command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", raw_size, leftover);
  if (scanline != 1)
  {
    fprintf (stderr, "\nError processing size argument,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (raw_size[0] == '$')
  {
    size_is_variable = true;
    size = get_number_variable_value_ccc(raw_size);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    size = strtoll(raw_size, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (perform_check && size_is_variable)
  {
    ccc_main_scratchpad_size_ccc = MAX_SCRATCHPAD_SIZE;
  }
  else
  {
    ccc_main_scratchpad_size_ccc = size;
  }

  return_value_ccc = set_main_scratchpad_ccc();
  return (return_value_ccc);
}






int copy_buffer_to_scratch_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_buffer_offset[length];
  char raw_scratch_offset[length];
  char raw_size[length];
  char leftover[length];
  char* endptr;
  unsigned int buffer_offset;
  unsigned int scratch_offset;
  unsigned int size;
  char *error_string = "Error processing COPYBUFFERTOSCRATCHPAD command on line";
  int scanline = sscanf(rest_of_line, "%s %s %s %[^\n]", raw_buffer_offset, raw_scratch_offset, raw_size, leftover);
  if (scanline != 3)
  {
    if (scanline < 3)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 3)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_buffer_offset[0] == '$')
  {
    buffer_offset = get_number_variable_value_ccc(raw_buffer_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    buffer_offset = strtoll(raw_buffer_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Buffer offset \'%s\' was unable to be completely processed.\n", raw_buffer_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_scratch_offset[0] == '$')
  {
    scratch_offset = get_number_variable_value_ccc(raw_scratch_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    scratch_offset = strtoll(raw_scratch_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Scratch offset \'%s\' was unable to be completely processed.\n", raw_scratch_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_size[0] == '$')
  {
    size = get_number_variable_value_ccc(raw_size);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    size = strtoll(raw_size, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }


  if (!perform_check && size + buffer_offset > ccc_main_buffer_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to copy past end of buffer,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (!perform_check && size + scratch_offset > ccc_main_scratchpad_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to copy past end of scratchpad,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check)
  {
    memcpy(ccc_scratchpad_ccc + scratch_offset, ccc_buffer_ccc + buffer_offset, size);
  }

  return (0);
}






int copy_scratch_to_buffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_buffer_offset[length];
  char raw_scratch_offset[length];
  char raw_size[length];
  char leftover[length];
  char* endptr;
  unsigned int buffer_offset;
  unsigned int scratch_offset;
  unsigned int size;
  char *error_string = "Error processing COPYSCRATCHPADTOBUFFER command on line";
  int scanline = sscanf(rest_of_line, "%s %s %s %[^\n]", raw_scratch_offset, raw_buffer_offset, raw_size, leftover);
  if (scanline != 3)
  {
    if (scanline < 3)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 3)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_scratch_offset[0] == '$')
  {
    scratch_offset = get_number_variable_value_ccc(raw_scratch_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    scratch_offset = strtoll(raw_scratch_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Scratch offset \'%s\' was unable to be completely processed.\n", raw_scratch_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_buffer_offset[0] == '$')
  {
    buffer_offset = get_number_variable_value_ccc(raw_buffer_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    buffer_offset = strtoll(raw_buffer_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Buffer offset \'%s\' was unable to be completely processed.\n", raw_buffer_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_size[0] == '$')
  {
    size = get_number_variable_value_ccc(raw_size);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    size = strtoll(raw_size, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (!perform_check && size + scratch_offset > ccc_main_scratchpad_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to copy past end of scratchpad,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check && size + buffer_offset > ccc_main_buffer_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to copy past end of buffer,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check)
  {
    memcpy(ccc_buffer_ccc + buffer_offset, ccc_scratchpad_ccc + scratch_offset, size);
  }

  return (0);
}






// function to check if variable exists and return the type in error_level
int variable_check_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char var_name[length+1];
  char leftover[length+1];
  char *error_string = "Error processing VARIABLECHECK command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", var_name, leftover);
  if (scanline != 1)
  {
    fprintf (stderr, "\nError processing variable,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (var_name[0] != '$')
  {
    fprintf (stderr, "\nError: Variable name must start with \'$\',\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  scanline = sscanf(var_name+1, "%s", var_name);
  if (scanline != 1)
  {
    fprintf (stderr, "\nError processing variable,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  int value = check_variable_ccc(var_name);
  int value2 = check_arguments_ccc(var_name);
  //fprintf (stdout, "value=%d value2=%d\n", value, value2);
  int var_num = find_number_variable_ccc("error_level");
  if (var_num < 0)
  {
    fprintf (stderr, "\nError: Unable to locate internal variable \'error_level\',\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (!perform_check)
  {
    int combined_value = value + (value2 << 4);
    number_variable_buffer_ccc[var_num] = combined_value;
  }
  return (0);
}







int printscratchpad_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_offset[length];
  char raw_size[length];
  char leftover[length];
  unsigned int offset;
  unsigned int size;
  char *error_string = "Error processing PRINTSCRATCHPAD command on line";
  int scanline = sscanf(rest_of_line, "%s %s %[^\n]", raw_offset, raw_size, leftover);
  if (scanline != 2)
  {
    if (scanline < 2)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 2)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (raw_offset[0] == '$')
  {
    offset = get_number_variable_value_ccc(raw_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    char* endptr;
    offset = strtoll(raw_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Offset \'%s\' was unable to be completely processed.\n", raw_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  if (raw_size[0] == '$')
  {
    size = get_number_variable_value_ccc(raw_size);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    char* endptr;
    size = strtoll(raw_size, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  if (!perform_check && size + offset > ccc_main_scratchpad_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to print past end of scratchpad,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check)
  {
    unsigned int i;
    for (i = 0; i < size; i+=16)
    {
      fprintf (stdout, "%x: ", i + offset);
      unsigned char *c;
      int n;
      for (n=0; n < 16 && i+n < size; n++)
      {
        c = (unsigned char *)ccc_scratchpad_ccc+i+n+offset;
        fprintf (stdout, "%02x ", *c);
      }
      for (n=0; n < 16 && i+n < size; n++)
      {
        c = (unsigned char *)ccc_scratchpad_ccc+i+n+offset;
        fprintf (stdout, "%c", isprint(*c) ? *c : '.');
      }
      fprintf (stdout, "\n");
    }
  }

  return (0);
}







int gosub_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char subroutine[length+1];
  char *error_string = "Error processing GOSUB command on line";
  int scanline = sscanf(rest_of_line, "%s[^\n]", subroutine);
  if (scanline != 1)
  {
    fprintf (stderr, "\nError processing argument of GOSUB command on line %d.\n", line_number-1);
    return (-1);
  }

  // check if subroutine exists and mark new line
  int new_line = find_command_plus1_ccc("subroutine", "SUBROUTINE", subroutine, 0);
  if (new_line < 0)
  {
    fprintf (stderr, "\nError: Subroutine \'%s\' not found,\n", subroutine);
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  else if (new_line == 0)
  {
    fprintf (stderr, "\nError: Subroutine \'%s\' is on line 1 which is not allowed,\n", subroutine);
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  // if we are checking then make sure there are no duplicates
  if (perform_check)
  {
    int double_check = find_command_plus1_ccc("subroutine", "SUBROUTINE", subroutine, new_line+1);
    if (double_check > 0)
    {
      fprintf (stderr, "\nError: Subroutine \'%s\' declared more than once,\n", subroutine);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  // if not checking then set the new line number
  if (!perform_check)
  {
    ccc_subroutine_stack_ccc[subroutine_stack_counter_ccc] = line_number;
    subroutine_while_stack_ccc[subroutine_stack_counter_ccc] = while_stack_counter_ccc;
    subroutine_endif_stack_ccc[subroutine_stack_counter_ccc] = endif_stack_counter_ccc;
    subroutine_stack_counter_ccc ++;
    if (subroutine_stack_counter_ccc > MAX_SUB_LEVEL)
    {
      fprintf (stderr, "\nError: Subroutine level exceeded maximum of %d,\n", MAX_SUB_LEVEL);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
    return (new_line);
  }

  return (0);
}





int ccc_subroutine_ccc(bool perform_check, unsigned int line_number)
{
  char *error_string = "Error processing SUBROUTINE command on line";
  if (perform_check)
  {
    subroutine_count_ccc++;
    if (subroutine_count_ccc != return_count_ccc+1)
    {
      fprintf (stderr, "\nError: SUBROUTINE count does not match ENDSUBROUTINE count,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
    int new_line = find_command_ccc("endsubroutine", line_number+1);
    if (new_line < 0)
    {
      fprintf (stderr, "\nError: SUBROUTINE without ENDSUBROUTINE,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  // skip to return line so that it can't be executed unless called
  if (!perform_check)
  {
    int new_line = find_command_ccc("endsubroutine", line_number+1);
    if (new_line < 0)
    {
      fprintf (stderr, "\nError: SUBROUTINE without ENDSUBROUTINE,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
    else return (new_line);
  }
  return (0);
}





int endsubroutine_ccc(bool perform_check, unsigned int line_number)
{
  char *error_string = "Error processing ENDSUBROUTINE command on line";
  if (perform_check)
  {
    return_count_ccc++;
    if (return_count_ccc != subroutine_count_ccc)
    {
      fprintf (stderr, "\nError: ENDSUBROUTINE count does not match SUBROUTINE count,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (!perform_check)
  {
    int new_line;
    subroutine_stack_counter_ccc --;
    if (subroutine_stack_counter_ccc < 0)
    {
      fprintf (stderr, "\nError: ENDSUBROUTINE without SUBROUTINE,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
    new_line = ccc_subroutine_stack_ccc[subroutine_stack_counter_ccc];
    return (new_line);
  }
  return (0);
}





int returnsub_ccc(bool perform_check, unsigned int line_number)
{
  char *error_string = "Error processing RETURNSUB command on line";
  if (perform_check)
  {
    if (subroutine_count_ccc < 1)
    {
      fprintf (stderr, "\nError: RETURNSUB without SUBROUTINE,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (!perform_check)
  {
    int new_line;
    subroutine_stack_counter_ccc --;
    if (subroutine_stack_counter_ccc < 0)
    {
      fprintf (stderr, "\nError: RETURNSUB without SUBROUTINE,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
    new_line = ccc_subroutine_stack_ccc[subroutine_stack_counter_ccc];
    while_stack_counter_ccc = subroutine_while_stack_ccc[subroutine_stack_counter_ccc];
    endif_stack_counter_ccc = subroutine_endif_stack_ccc[subroutine_stack_counter_ccc];
    return (new_line);
  }
  return (0);
}







int ifstate_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  // find matching endif and next elseif or else
  int current_line = line_number;
  int find_next_endif;
  int find_next_if;
  int find_next_else;
  int matching_else = -1;
  int find_next_elseif;
  int matching_elseif = -1;
  int matching_endif;
  int next_statement;
  int level = 1;
  char *error_string = "Error processing IF command on line";
  //fprintf (stdout, "processing if statement on line %d\n", line_number-1);
  while (1)
  {
    find_next_else = find_command_ccc("else", current_line+1);
    find_next_elseif = find_command_ccc("elseif", current_line+1);
    find_next_endif = find_command_ccc("endif", current_line+1);
    find_next_if = find_command_ccc("if", current_line+1);

    //fprintf (stdout, "if=%d else=%d elseif=%d endif=%d level=%d current_line=%d\n", find_next_if+1, find_next_else+1, find_next_elseif+1, find_next_endif+1, level, current_line+1);

    if (find_next_endif < 1)
    {
      fprintf (stderr, "\nError: IF without ENDIF,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

    if (level == 1 && matching_else > 1 && find_next_else > 1)
    {
      if ( (find_next_else < find_next_if || find_next_if < 1) && find_next_else < find_next_endif)
      {
        fprintf (stderr, "\nError: Too many ELSE for IF on lines %d and %d,\n", matching_else+1, find_next_else+1);
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
    }
    if (level == 1 && matching_else < 1 && find_next_else > 1)
    {
      if ( (find_next_else < find_next_if || find_next_if < 1) && find_next_else < find_next_endif)
      {
        matching_else = find_next_else;
        int find_duplicate = find_next_else = find_command_ccc("else", matching_else+1);
        if (find_duplicate > 1)
        {
          if ( (find_duplicate < find_next_if || find_next_if < 1) && find_duplicate < find_next_endif)
          {
            fprintf (stderr, "\nError: Too many ELSE for IF on lines %d and %d,\n", matching_else+1, find_duplicate+1);
            fprintf (stderr, "%s %d.\n", error_string, line_number-1);
            return (-1);
          }
        }
      }
    }

    if (level == 1 && matching_elseif < 1 && find_next_elseif > 1)
    {
      if ( (find_next_elseif < find_next_if || find_next_if < 1) && find_next_elseif < find_next_endif)
      {
        matching_elseif = find_next_elseif;
      }
    }

    if (find_next_if > 0 && find_next_if < find_next_endif)
    {
      level++;
      current_line = find_next_if;
    }
    else
    {
      level--;
      current_line = find_next_endif;
    }

    if (level < 0)
    {
      fprintf (stderr, "\nError: ENDIF without IF,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

    if (level == 0)
    {
      matching_endif = find_next_endif;
      break;
    }
  }

  //fprintf (stdout, "matchelse=%d matchelseif=%d matchendif=%d\n", matching_else+1, matching_elseif+1, matching_endif+1);


  if (matching_else > 1 && matching_elseif > 1 && matching_else < matching_elseif)
  {
    fprintf (stderr, "\nError: ELSE before ELSEIF on lines %d and %d,\n", matching_else+1, matching_elseif+1);
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (matching_elseif > 1)
  {
    next_statement = matching_elseif;
  }
  else if (matching_else > 1)
  {
    next_statement = matching_else;
  }
  else
  {
    next_statement = matching_endif;
  }



  if (perform_check)
  {
    if_statement_level_ccc++;
  }

  int statement = compare_ccc(perform_check, rest_of_line);
  if (statement < 0)
  {
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }


  if (!perform_check)
  {
    ccc_endif_stack_ccc[endif_stack_counter_ccc] = matching_endif;
    //fprintf (stdout, "endifstack=%d counter=%d\n", ccc_endif_stack_ccc[endif_stack_counter_ccc], endif_stack_counter_ccc);
    statement_condition_ccc[endif_stack_counter_ccc] = false;
    endif_stack_counter_ccc ++;
    if (endif_stack_counter_ccc > MAX_ENDIF_LEVEL)
    {
      fprintf (stderr, "\nError: IF statement level exceeded maximum of %d,\n", MAX_ENDIF_LEVEL);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

    if (statement == 0)
    {
      return (next_statement-1);
    }
    else if (statement == 1)
    {
      statement_condition_ccc[endif_stack_counter_ccc-1] = true;
      return (0);
    }
  }

  return (0);
}






int ccc_endif_ccc(bool perform_check, unsigned int line_number)
{
  char *error_string = "Error processing ENDIF command on line";
  if (perform_check)
  {
    if_statement_level_ccc--;
    if (if_statement_level_ccc < 0)
    {
      fprintf (stderr, "\nError: ENDIF without IF,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (!perform_check)
  {
    endif_stack_counter_ccc --;
    if (endif_stack_counter_ccc < 0)
    {
      fprintf (stderr, "\nError: ENDIF without IF,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  return (0);
}





int elsestate_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  char *error_string = "Error processing ELSE command on line";
  int length = strlen(rest_of_line);
  char temp[length+1];
  char leftover[length+1];
  int scanline = sscanf(rest_of_line, "%s %[^\n]", temp, leftover);
  if (scanline > 0)
  {
    fprintf (stderr, "\nUnknown data following statement,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (perform_check && if_statement_level_ccc < 1)
  {
    fprintf (stderr, "\nError: ELSE without IF,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check && endif_stack_counter_ccc < 1)
  {
    fprintf (stderr, "\nError: ELSE without IF,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check && statement_condition_ccc[endif_stack_counter_ccc-1])
  {
    //fprintf (stdout, "endifstack=%d\n", ccc_endif_stack_ccc[endif_stack_counter_ccc-1]-1);
    return (ccc_endif_stack_ccc[endif_stack_counter_ccc-1]-1);
  }
  return (0);
}







int ccc_elseif_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  char *error_string = "Error processing ELSEIF command on line";
  if (perform_check && if_statement_level_ccc < 1)
  {
    fprintf (stderr, "\nError: ELSEIF without IF,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check && endif_stack_counter_ccc < 1)
  {
    fprintf (stderr, "\nError: ELSEIF without IF,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  //TODO: endif_stack_counter_ccc can be 0, which means
  //  statement_condition_ccc can be indexed by -1 which
  //  is a memory bug, but the script parser breaks when
  //  I treat that as an error! quick hack return success:
  if (endif_stack_counter_ccc < 1) return 0;

  if (statement_condition_ccc[endif_stack_counter_ccc-1])
  {
    return (ccc_endif_stack_ccc[endif_stack_counter_ccc-1]-1);
  }

  // find matching endif and next elseif or else
  int current_line = line_number;
  int find_next_endif;
  int find_next_if;
  int find_next_else;
  int matching_else = -1;
  int find_next_elseif;
  int matching_elseif = -1;
  int matching_endif;
  int next_statement;
  int level = 1;
  //fprintf (stdout, "processing elseif statement on line %d\n", line_number-1);
  while (1)
  {
    find_next_else = find_command_ccc("else", current_line+1);
    find_next_elseif = find_command_ccc("elseif", current_line+1);
    find_next_endif = find_command_ccc("endif", current_line+1);
    find_next_if = find_command_ccc("if", current_line+1);

    //fprintf (stdout, "if=%d else=%d elseif=%d endif=%d level=%d current_line=%d\n", find_next_if+1, find_next_else+1, find_next_elseif+1, find_next_endif+1, level, current_line+1);

    if (find_next_endif < 1)
    {
      fprintf (stderr, "\nError: IF without ENDIF,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

    if (level == 1 && matching_else < 1)
    {
      if ( (find_next_else < find_next_if || find_next_if < 1) && find_next_else < find_next_endif)
      {
        matching_else = find_next_else;
      }
    }
    if (level == 1 && matching_elseif < 1)
    {

      if ( (find_next_elseif < find_next_if || find_next_if < 1) && find_next_elseif < find_next_endif)
      {
        matching_elseif = find_next_elseif;
      }
    }

    if (find_next_if > 1 && find_next_if < find_next_endif)
    {
      level++;
      current_line = find_next_if;
    }
    else
    {
      level--;
      current_line = find_next_endif;
    }

    if (level < 0)
    {
      fprintf (stderr, "\nError: ENDIF without IF,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

    if (level == 0)
    {
      matching_endif = find_next_endif;
      break;
    }
  }

  //fprintf (stdout, "matchelse=%d matchelseif=%d matchendif=%d\n", matching_else+1, matching_elseif+1, matching_endif+1);


  if (matching_else > 1 && matching_elseif > 1 && matching_else < matching_elseif)
  {
    fprintf (stderr, "\nError: ELSE before ELSEIF on lines %d and %d,\n", matching_else+1, matching_elseif+1);
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (matching_elseif > 1)
  {
    next_statement = matching_elseif;
  }
  else if (matching_else > 1)
  {
    next_statement = matching_else;
  }
  else
  {
    next_statement = matching_endif;
  }


  int statement = compare_ccc(perform_check, rest_of_line);
  if (statement < 0)
  {
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }


  if (!perform_check)
  {
    statement_condition_ccc[endif_stack_counter_ccc] = false;

    if (statement == 0)
    {
      return (next_statement-1);
    }
    else if (statement == 1)
    {
      statement_condition_ccc[endif_stack_counter_ccc-1] = true;
      return (0);
    }
  }
  return (0);
}









int dowhile_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  //fprintf (stdout, "whilestackcount=%d\n", while_stack_counter_ccc);

  char *error_string = "Error processing WHILE command on line";
  if (!perform_check && while_stack_counter_ccc > 0 && while_condition_ccc[while_stack_counter_ccc-1] == true && ccc_while_stack_ccc[while_stack_counter_ccc-1] == line_number)
  {
    int statement = compare_ccc(perform_check, rest_of_line);
    if (statement < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
    else if (statement == 0)
    {
      while_condition_ccc[while_stack_counter_ccc-1] = false;
      while_stack_counter_ccc--;
      return (done_stack_ccc[while_stack_counter_ccc]);
    }
    else if (statement == 1)
    {
      return (0);
    }
  }


  // find matching done
  int current_line = line_number;
  int find_next_done;
  int find_next_while;
  int matching_done;
  int level = 1;
  while (1)
  {
    find_next_done = find_command_ccc("done", current_line+1);
    find_next_while = find_command_ccc("while", current_line+1);

    if (find_next_done < 1)
    {
      fprintf (stderr, "\nError: WHILE without DONE,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

    if (find_next_while > 0 && find_next_while < find_next_done)
    {
      level++;
      current_line = find_next_while;
    }
    else
    {
      level--;
      current_line = find_next_done;
    }

    if (level < 0)
    {
      fprintf (stderr, "\nError: DONE without WHILE,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

    if (level == 0)
    {
      matching_done = find_next_done;
      break;
    }
  }


  if (perform_check)
  {
    while_statement_level_ccc++;
  }

  int statement = compare_ccc(perform_check, rest_of_line);
  if (statement < 0)
  {
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }


  if (!perform_check)
  {
    done_stack_ccc[while_stack_counter_ccc] = matching_done;
    ccc_while_stack_ccc[while_stack_counter_ccc] = line_number;
    while_condition_ccc[while_stack_counter_ccc] = false;
    while_endif_stack_ccc[while_stack_counter_ccc] = endif_stack_counter_ccc;
    while_stack_counter_ccc ++;

    //fprintf (stdout, "while count=%d condition=%d whilestack=%d donestack=%d\n", while_stack_counter_ccc-1, while_condition_ccc[while_stack_counter_ccc-1], ccc_while_stack_ccc[while_stack_counter_ccc-1]+1, done_stack_ccc[while_stack_counter_ccc-1]+1);

    if (while_stack_counter_ccc > MAX_WHILE_LEVEL)
    {
      fprintf (stderr, "\nError: WHILE statement level exceeded maximum of %d,\n", MAX_WHILE_LEVEL);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }

    if (statement == 0)
    {
      return (matching_done-1);
    }
    else if (statement == 1)
    {
      while_condition_ccc[while_stack_counter_ccc-1] = true;
      return (0);
    }
  }

  return (0);
}







int ccc_done_ccc(bool perform_check, unsigned int line_number)
{
  char *error_string = "Error processing DONE command on line";
  if (perform_check)
  {
    while_statement_level_ccc--;
    if (while_statement_level_ccc < 0)
    {
      fprintf (stderr, "\nError: DONE without WHILE,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (!perform_check)
  {
    //fprintf (stdout, "done count=%d condition=%d whilestack=%d\n", while_stack_counter_ccc-1, while_condition_ccc[while_stack_counter_ccc-1], ccc_while_stack_ccc[while_stack_counter_ccc-1]+1);

    if (while_condition_ccc[while_stack_counter_ccc-1] == true)
    {
      return (ccc_while_stack_ccc[while_stack_counter_ccc-1]-1);
    }

    while_stack_counter_ccc --;
    if (while_stack_counter_ccc < 0)
    {
      fprintf (stderr, "\nError: DONE without WHILE,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  return (0);
}






int dobreak_ccc(bool perform_check, unsigned int line_number)
{
  char *error_string = "Error processing BREAK command on line";
  if (perform_check)
  {
    if (while_statement_level_ccc < 1)
    {
      fprintf (stderr, "\nError: BREAK without WHILE,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (!perform_check)
  {
    while_stack_counter_ccc --;
    if (while_stack_counter_ccc < 0)
    {
      fprintf (stderr, "\nError: BREAK without WHILE,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
    else
    {
      endif_stack_counter_ccc = while_endif_stack_ccc[while_stack_counter_ccc];
      return (done_stack_ccc[while_stack_counter_ccc]);
    }
  }

  return (0);
}





int string_to_usbbuffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  unsigned int length = strlen(rest_of_line);
  char raw_usbbuffer_offset[length];
  char raw_variable_name[length];
  char raw_size[length];
  char leftover[length];
  char* endptr;
  unsigned int usbbuffer_offset;
  char extracted_var[MAX_LINE_LENGTH];
  unsigned int size;
  char *error_string = "Error processing STRINGTOUSBBUFFER command on line";

  int scanline = sscanf(rest_of_line, "%s %s %s %[^\n]", raw_usbbuffer_offset, raw_size, raw_variable_name, leftover);
  if (scanline != 3)
  {
    if (scanline < 3)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 3)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_usbbuffer_offset[0] == '$')
  {
    usbbuffer_offset = get_number_variable_value_ccc(raw_usbbuffer_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    usbbuffer_offset = strtoll(raw_usbbuffer_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Buffer offset \'%s\' was unable to be completely processed.\n", raw_usbbuffer_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_size[0] == '$')
  {
    size = get_number_variable_value_ccc(raw_size);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    size = strtoll(raw_size, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_variable_name[0] == '$')
  {
    char var_name[MAX_VARIABLE_NAME_LENGTH];
    sscanf(raw_variable_name+1, "%s", var_name);
    return_value_ccc = check_variable_ccc(var_name);
    if (return_value_ccc < 1)
    {
      fprintf (stderr, "\nError: Variable \'%s\' not found,\n", raw_variable_name);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
    else if (return_value_ccc != 2)
    {
      fprintf (stderr, "\nError: Variable \'%s\' is wrong type (expecting string variable),\n", raw_variable_name);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
    else
    {
      int var_num = find_string_variable_ccc(var_name);
      strcpy(extracted_var, string_variable_pointer_ccc[var_num]);
      length = strlen(extracted_var);
    }
  }
  else
  {
    fprintf (stderr, "\nError: Variable name must start with \'$\',\n");
    fprintf (stderr, "Error processing \'%s\'\n", raw_variable_name);
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }


  if (size + usbbuffer_offset > ccc_main_usbbuffer_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to write past end of usb buffer,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check)
  {
    unsigned int i;
    for (i = 0; i < size && i < length; i++)
    {
      unsigned char c = extracted_var[i];
      memcpy(ccc_usbbuffer_ccc + usbbuffer_offset + i, &c, 1);
    }
  }
  return 0;
}





int wordflip_usbbuffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_offset[length];
  char raw_size[length];
  char leftover[length];
  unsigned int offset;
  unsigned int size;
  char *error_string = "Error processing WORDFLIPUSBBUFFER command on line";
  int scanline = sscanf(rest_of_line, "%s %s %[^\n]", raw_offset, raw_size, leftover);
  if (scanline != 2)
  {
    if (scanline < 2)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 2)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (raw_offset[0] == '$')
  {
    offset = get_number_variable_value_ccc(raw_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    char* endptr;
    offset = strtoll(raw_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Offset \'%s\' was unable to be completely processed.\n", raw_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  if (raw_size[0] == '$')
  {
    size = get_number_variable_value_ccc(raw_size);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    char* endptr;
    size = strtoll(raw_size, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  if (size + offset > ccc_main_usbbuffer_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to flip past end of usb buffer,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check)
  {
    unsigned int i;
    for (i = 0; i < size-1; i+=2)
    {
      unsigned char a;
      unsigned char b;
      memcpy(&a, ccc_usbbuffer_ccc+i+offset, 1);
      memcpy(&b, ccc_usbbuffer_ccc+i+offset+1, 1);
      memcpy(ccc_usbbuffer_ccc+i+offset, &b, 1);
      memcpy(ccc_usbbuffer_ccc+i+offset+1, &a, 1);
    }
  }
  return 0;
}





int write_usbbuffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_file_name[length];
  char raw_buffer_offset[length];
  char raw_file_offset[length];
  char raw_size[length];
  char leftover[length];
  char* endptr;
  char *file_name;
  unsigned long long buffer_offset;
  unsigned long long file_offset;
  unsigned long long size;
  char *error_string = "Error processing WRITEUSBBUFFER command on line";
  int scanline = sscanf(rest_of_line, "%s %s %s %s %[^\n]", raw_file_name, raw_buffer_offset, raw_file_offset, raw_size, leftover);
  if (scanline != 4)
  {
    if (scanline < 4)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 4)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_file_name[0] == '$')
  {
    file_name = get_string_variable_value_ccc(raw_file_name);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    file_name = raw_file_name;
  }

  if (!perform_check && strlen(file_name) == 0)
  {
    fprintf (stderr, "\nError: File name is blank,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_buffer_offset[0] == '$')
  {
    buffer_offset = get_number_variable_value_ccc(raw_buffer_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    buffer_offset = strtoll(raw_buffer_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Buffer offset \'%s\' was unable to be completely processed.\n", raw_buffer_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_file_offset[0] == '$')
  {
    file_offset = get_number_variable_value_ccc(raw_file_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    file_offset = strtoll(raw_file_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! File offset \'%s\' was unable to be completely processed.\n", raw_file_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_size[0] == '$')
  {
    size = get_number_variable_value_ccc(raw_size);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    size = strtoll(raw_size, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (!perform_check && size + buffer_offset > ccc_main_usbbuffer_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to copy past end of usb buffer,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }


  if (!perform_check)
  {
    FILE *createfile = fopen(file_name, "ab");
    fclose (createfile);
    FILE *writefile = fopen(file_name, "rb+");
    if (writefile == NULL)
    {
      fprintf(stderr, "Cannot open \'%s\' for writing (%s).\n", file_name, strerror(errno));
      return (-1);
    }
    if (fseeko(writefile, file_offset, SEEK_SET) != 0 )
    {
      fprintf(stderr, "Error seeking \'%s\' (%s).\n", file_name, strerror(errno));
      fclose(writefile);
      return (-1);
    }
    if (fwrite(ccc_usbbuffer_ccc + buffer_offset, 1, size, writefile) != size )
    {
      fprintf(stderr, "Error writing to \'%s\' (%s).\n", file_name, strerror(errno));
      fclose(writefile);
      return (-1);
    }

    fclose (writefile);
  }
  return 0;
}





int read_usbbuffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_file_name[length];
  char raw_buffer_offset[length];
  char raw_file_offset[length];
  char raw_size[length];
  char leftover[length];
  char* endptr;
  char *file_name;
  unsigned int buffer_offset;
  unsigned int file_offset;
  unsigned int size;
  char *error_string = "Error processing READUSBBUFFER command on line";
  int scanline = sscanf(rest_of_line, "%s %s %s %s %[^\n]", raw_file_name, raw_buffer_offset, raw_file_offset, raw_size, leftover);
  if (scanline != 4)
  {
    if (scanline < 4)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 4)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_file_name[0] == '$')
  {
    file_name = get_string_variable_value_ccc(raw_file_name);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    file_name = raw_file_name;
  }

  if (!perform_check && strlen(file_name) == 0)
  {
    fprintf (stderr, "\nError: File name is blank,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_buffer_offset[0] == '$')
  {
    buffer_offset = get_number_variable_value_ccc(raw_buffer_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    buffer_offset = strtoll(raw_buffer_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Buffer offset \'%s\' was unable to be completely processed.\n", raw_buffer_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_file_offset[0] == '$')
  {
    file_offset = get_number_variable_value_ccc(raw_file_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    file_offset = strtoll(raw_file_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! File offset \'%s\' was unable to be completely processed.\n", raw_file_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_size[0] == '$')
  {
    size = get_number_variable_value_ccc(raw_size);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    size = strtoll(raw_size, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (!perform_check && size + buffer_offset > ccc_main_usbbuffer_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to copy past end of usb buffer,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }


  if (!perform_check)
  {
    FILE *readfile = fopen(file_name, "rb");
    if (readfile == NULL)
    {
      fprintf(stderr, "Cannot open %s for reading (%s).\nAborting...\n", file_name, strerror(errno));
      return (-1);
    }
    if (fseeko(readfile, 0, SEEK_END) != 0 )
    {
      fprintf(stderr, "Error seeking %s (%s).\n", file_name, strerror(errno));
      fclose(readfile);
      return (-1);
    }
    long long file_size = ftello(readfile);
    if (size + buffer_offset > file_size)
    {
      fprintf (stderr, "ERROR: Attempt to read past end of file,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      fclose (readfile);
      return (-1);
    }

    if (fseeko(readfile, file_offset, SEEK_SET) != 0 )
    {
      fprintf(stderr, "Error seeking \'%s\' (%s).\n", file_name, strerror(errno));
      fclose(readfile);
      return (-1);
    }
    if (fread(ccc_usbbuffer_ccc + buffer_offset, 1, size, readfile) != size )
    {
      fprintf(stderr, "Error reading from \'%s\' (%s).\n", file_name, strerror(errno));
      fclose(readfile);
      return (-1);
    }

    fclose (readfile);
  }
  return 0;
}





int print_usbbuffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_offset[length];
  char raw_size[length];
  char leftover[length];
  unsigned int offset;
  unsigned int size;
  char *error_string = "Error processing PRINTUSBBUFFER command on line";
  int scanline = sscanf(rest_of_line, "%s %s %[^\n]", raw_offset, raw_size, leftover);
  if (scanline != 2)
  {
    if (scanline < 2)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 2)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (raw_offset[0] == '$')
  {
    offset = get_number_variable_value_ccc(raw_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    char* endptr;
    offset = strtoll(raw_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Offset \'%s\' was unable to be completely processed.\n", raw_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  if (raw_size[0] == '$')
  {
    size = get_number_variable_value_ccc(raw_size);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    char* endptr;
    size = strtoll(raw_size, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  if (!perform_check && size + offset > ccc_main_usbbuffer_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to print past end of usb buffer,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check)
  {
    unsigned int i;
    for (i = 0; i < size; i+=16)
    {
      fprintf (stdout, "%x: ", i + offset);
      unsigned char *c;
      int n;
      for (n=0; n < 16 && i+n < size; n++)
      {
        c = (unsigned char *)ccc_usbbuffer_ccc+i+n+offset;
        fprintf (stdout, "%02x ", *c);
      }
      for (n=0; n < 16 && i+n < size; n++)
      {
        c = (unsigned char *)ccc_usbbuffer_ccc+i+n+offset;
        fprintf (stdout, "%c", isprint(*c) ? *c : '.');
      }
      fprintf (stdout, "\n");
    }
  }
  return 0;
}





int set_usbbuffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_offset[length];
  char leftover[MAX_LINE_LENGTH];
  char* endptr;
  unsigned long long offset;
  char *error_string = "Error processing SETUSBBUFFER command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", raw_offset, leftover);
  if (scanline != 1)
  {
    fprintf (stderr, "\nError processing offset argument,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (raw_offset[0] == '$')
  {
    offset = get_number_variable_value_ccc(raw_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    offset = strtoll(raw_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Offset \'%s\' was unable to be completely processed.\n", raw_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  if (perform_check)
  {
    setting_usbbuffer_ccc = true;
    int find_next_setusb = find_command_ccc("setusbbuffer", line_number+1);
    int find_next_endusb = find_command_ccc("endusbbuffer", line_number+1);
    if (find_next_endusb < 1)
    {
      fprintf (stderr, "\nError, SETUSBBUFFER without ENDUSBBUFFER,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
    if (find_next_setusb > 1 && find_next_setusb < find_next_endusb)
    {
      fprintf (stderr, "\nError: Next SETUSBBUFFER found before ENDUSBBUFFER on line %d,\n", find_next_setusb+1);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  int current_line = line_number+1;
  char *full_line;
  char input_text[MAX_LINE_LENGTH];
  input_text[0]='\0';
  if (command_line_ccc)
  {
    fprintf (stdout, "setusbbuffer> ");
    fflush(stdout);
    full_line = fgets(input_text, sizeof input_text, stdin);
  }
  else
  {
    full_line = get_full_line_ccc(current_line, input_text);
  }
  char line[MAX_LINE_LENGTH];
  strcpy (line, full_line);
  char raw_byte[MAX_LINE_LENGTH];
  unsigned int byte_count = 0;
  unsigned int byte_offset = 0;
  while (1)
  {
    //fprintf (stdout, "line=%s\n", line);
    strcpy (raw_byte, "");
    strcpy (leftover, "");
    sscanf(line, "%s %[^\n]", raw_byte, leftover);
    //fprintf (stdout, "raw_byte=%s  leftover=%s\n", raw_byte, leftover);
    if ( (strcasecmp(raw_byte, "endusbbuffer") == 0) )
    {
      break;
    }
    int length = strlen(raw_byte);
    if (length <= 0)
    {
      current_line++;
      char *full_line;
      char input_text[MAX_LINE_LENGTH];
      memset(input_text,0,sizeof(input_text));
      if (command_line_ccc)
      {
        fprintf (stdout, "setusbbuffer> ");
        fflush(stdout);
        full_line=fgets(input_text, sizeof input_text, stdin);
      }
      else
      {
        full_line = get_full_line_ccc(current_line, input_text);
      }
      strcpy (line, full_line);
    }
    else if (raw_byte[length-1] == ':')
    {
      byte_offset = strtoll(raw_byte, NULL, 16);
      strcpy (line, leftover);
      byte_count = 0;
    }
    else
    {
      if (!perform_check &&  byte_count +1 + byte_offset + offset > ccc_main_usbbuffer_size_ccc)
      {
        fprintf (stderr, "Error! End of usb buffer reached at \'%s\' on line %d.\n", raw_byte, current_line+1);
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
      unsigned long long value;
      if (raw_byte[0] == '$')
      {
        value = get_number_variable_value_ccc(raw_byte);
        if (return_value_ccc < 0)
        {
          fprintf (stderr, "%s %d.\n", error_string, line_number-1);
          return (-1);
        }
      }
      else
      {
        char* endptr;
        value = strtoull(raw_byte, &endptr, 16);
        if (*endptr)
        {
          fprintf (stderr, "Error! Byte \'%s\' on line %d could not be processed.\n", raw_byte, current_line+1);
          fprintf (stderr, "%s %d.\n", error_string, line_number-1);
          return (-1);
        }
      }
      int high_value = 0;
      if (value > 255)
      {
        if ( byte_count + 8 + byte_offset + offset > ccc_main_usbbuffer_size_ccc)
        {
          fprintf (stderr, "Error! End of usb buffer reached at \'%s\' on line %d.\n", raw_byte, current_line+1);
          fprintf (stderr, "%s %d.\n", error_string, line_number-1);
          return (-1);
        }
        high_value = 1;
      }
      if (!perform_check)
      {
        if (high_value)
        {
          unsigned long long c = value;
          memcpy(ccc_usbbuffer_ccc + byte_offset + offset + byte_count, &c, 8);
        }
        else
        {
          unsigned char c = value;
          memcpy(ccc_usbbuffer_ccc + byte_offset + offset + byte_count, &c, 1);
        }
      }
      strcpy (line, leftover);
      byte_count++;
    }
  }
  if (!perform_check)
  {
    return (current_line-1);
  }
  return 0;
}





int end_usbbuffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  char *error_string = "Error processing ENDUSBBUFFER command on line";
  if (perform_check)
  {
    if (!setting_usbbuffer_ccc)
    {
      fprintf (stderr, "\nError, ENDUSBBUFFER without SETUSBBUFFER,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
    int length = strlen(rest_of_line);
    if (length > 0)
    {
      fprintf (stderr, "\nError, there should be no arguments after the ENDUSBBUFFER command,\n");
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
    setting_usbbuffer_ccc = false;
  }
  return 0;
}





int change_usbbuffer_size_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_size[length];
  char leftover[length];
  char* endptr;
  unsigned long long size;
  bool size_is_variable = false;
  char *error_string = "Error processing USBBUFFERSIZE command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", raw_size, leftover);
  if (scanline != 1)
  {
    fprintf (stderr, "\nError processing size argument,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (raw_size[0] == '$')
  {
    size_is_variable = true;
    size = get_number_variable_value_ccc(raw_size);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    size = strtoll(raw_size, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (perform_check && size_is_variable)
  {
    ccc_main_usbbuffer_size_ccc = MAX_USB_BUFFER_SIZE;
  }
  else
  {
    ccc_main_usbbuffer_size_ccc = size;
  }

  return_value_ccc = set_main_usb_buffer_ccc();
  return (return_value_ccc);
}





int copy_usbbuffer_to_buffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_buffer_offset[length];
  char raw_usb_offset[length];
  char raw_size[length];
  char leftover[length];
  char* endptr;
  unsigned int buffer_offset;
  unsigned int usb_offset;
  unsigned int size;
  char *error_string = "Error processing COPYUSBTOBUFFER command on line";
  int scanline = sscanf(rest_of_line, "%s %s %s %[^\n]", raw_usb_offset, raw_buffer_offset, raw_size, leftover);
  if (scanline != 3)
  {
    if (scanline < 3)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 3)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_usb_offset[0] == '$')
  {
    usb_offset = get_number_variable_value_ccc(raw_usb_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    usb_offset = strtoll(raw_usb_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! USB offset \'%s\' was unable to be completely processed.\n", raw_usb_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_buffer_offset[0] == '$')
  {
    buffer_offset = get_number_variable_value_ccc(raw_buffer_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    buffer_offset = strtoll(raw_buffer_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Buffer offset \'%s\' was unable to be completely processed.\n", raw_buffer_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_size[0] == '$')
  {
    size = get_number_variable_value_ccc(raw_size);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    size = strtoll(raw_size, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (!perform_check && size + usb_offset > ccc_main_usbbuffer_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to copy past end of usb buffer,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check && size + buffer_offset > ccc_main_buffer_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to copy past end of buffer,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check)
  {
    memcpy(ccc_buffer_ccc + buffer_offset, ccc_usbbuffer_ccc + usb_offset, size);
  }
  return 0;
}





int copy_buffer_to_usbbuffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_buffer_offset[length];
  char raw_usb_offset[length];
  char raw_size[length];
  char leftover[length];
  char* endptr;
  unsigned int buffer_offset;
  unsigned int usb_offset;
  unsigned int size;
  char *error_string = "Error processing COPYBUFFERTOUSB command on line";
  int scanline = sscanf(rest_of_line, "%s %s %s %[^\n]", raw_buffer_offset, raw_usb_offset, raw_size, leftover);
  if (scanline != 3)
  {
    if (scanline < 3)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 3)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_buffer_offset[0] == '$')
  {
    buffer_offset = get_number_variable_value_ccc(raw_buffer_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    buffer_offset = strtoll(raw_buffer_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Buffer offset \'%s\' was unable to be completely processed.\n", raw_buffer_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_usb_offset[0] == '$')
  {
    usb_offset = get_number_variable_value_ccc(raw_usb_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    usb_offset = strtoll(raw_usb_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! USB offset \'%s\' was unable to be completely processed.\n", raw_usb_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_size[0] == '$')
  {
    size = get_number_variable_value_ccc(raw_size);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    size = strtoll(raw_size, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }


  if (!perform_check && size + buffer_offset > ccc_main_buffer_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to copy past end of buffer,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (!perform_check && size + usb_offset > ccc_main_usbbuffer_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to copy past end of usb buffer,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check)
  {
    memcpy(ccc_usbbuffer_ccc + usb_offset, ccc_buffer_ccc + buffer_offset, size);
  }
  return 0;
}





int copy_usbbuffer_to_scratch_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_usb_offset[length];
  char raw_scratch_offset[length];
  char raw_size[length];
  char leftover[length];
  char* endptr;
  unsigned int usb_offset;
  unsigned int scratch_offset;
  unsigned int size;
  char *error_string = "Error processing COPYUSBTOSCRATCHPAD command on line";
  int scanline = sscanf(rest_of_line, "%s %s %s %[^\n]", raw_usb_offset, raw_scratch_offset, raw_size, leftover);
  if (scanline != 3)
  {
    if (scanline < 3)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 3)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_usb_offset[0] == '$')
  {
    usb_offset = get_number_variable_value_ccc(raw_usb_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    usb_offset = strtoll(raw_usb_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! USB offset \'%s\' was unable to be completely processed.\n", raw_usb_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_scratch_offset[0] == '$')
  {
    scratch_offset = get_number_variable_value_ccc(raw_scratch_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    scratch_offset = strtoll(raw_scratch_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Scratch offset \'%s\' was unable to be completely processed.\n", raw_scratch_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_size[0] == '$')
  {
    size = get_number_variable_value_ccc(raw_size);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    size = strtoll(raw_size, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }


  if (!perform_check && size + usb_offset > ccc_main_usbbuffer_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to copy past end of usb buffer,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (!perform_check && size + scratch_offset > ccc_main_scratchpad_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to copy past end of scratchpad,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check)
  {
    memcpy(ccc_scratchpad_ccc + scratch_offset, ccc_usbbuffer_ccc + usb_offset, size);
  }
  return 0;
}





int copy_scratch_to_usbbuffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_usb_offset[length];
  char raw_scratch_offset[length];
  char raw_size[length];
  char leftover[length];
  char* endptr;
  unsigned int usb_offset;
  unsigned int scratch_offset;
  unsigned int size;
  char *error_string = "Error processing COPYSCRATCHPADTOUSB command on line";
  int scanline = sscanf(rest_of_line, "%s %s %s %[^\n]", raw_scratch_offset, raw_usb_offset, raw_size, leftover);
  if (scanline != 3)
  {
    if (scanline < 3)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > 3)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (raw_scratch_offset[0] == '$')
  {
    scratch_offset = get_number_variable_value_ccc(raw_scratch_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    scratch_offset = strtoll(raw_scratch_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Scratch offset \'%s\' was unable to be completely processed.\n", raw_scratch_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_usb_offset[0] == '$')
  {
    usb_offset = get_number_variable_value_ccc(raw_usb_offset);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    usb_offset = strtoll(raw_usb_offset, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! USB offset \'%s\' was unable to be completely processed.\n", raw_usb_offset);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (raw_size[0] == '$')
  {
    size = get_number_variable_value_ccc(raw_size);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    size = strtoll(raw_size, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Size \'%s\' was unable to be completely processed.\n", raw_size);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (!perform_check && size + scratch_offset > ccc_main_scratchpad_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to copy past end of scratchpad,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check && size + usb_offset > ccc_main_usbbuffer_size_ccc)
  {
    fprintf (stderr, "ERROR: Attempt to copy past end of usb buffer,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  if (!perform_check)
  {
    memcpy(ccc_usbbuffer_ccc + usb_offset, ccc_scratchpad_ccc + scratch_offset, size);
  }
  return 0;
}





int usbtimeout_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_time[length];
  char leftover[length];
  char* endptr;
  unsigned long long time;
  char *error_string = "Error processing USBTIMEOUT command on line";
  int scanline = sscanf(rest_of_line, "%s %[^\n]", raw_time, leftover);
  if (scanline != 1)
  {
    fprintf (stderr, "\nError processing time argument,\n");
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }
  if (raw_time[0] == '$')
  {
    time = get_number_variable_value_ccc(raw_time);
    if (return_value_ccc < 0)
    {
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }
  else
  {
    time = strtoull(raw_time, &endptr, 0);
    if (*endptr)
    {
      fprintf (stderr, "Error! Time \'%s\' was unable to be completely processed.\n", raw_time);
      fprintf (stderr, "%s %d.\n", error_string, line_number-1);
      return (-1);
    }
  }

  if (!perform_check)
  {
    usb_timeout_ccc = time;
  }
  return 0;
}





int send_usb_control_msg_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  int cmd_length = 4;
  int length = strlen(rest_of_line);
  char raw_b[cmd_length][length];
  char leftover[length];
  char* endptr;
  char *error_string = "Error processing SENDUSBCONTROLMSG command on line";

  int scanline = sscanf(rest_of_line, "%s %s %s %s %[^\n]", raw_b[0], raw_b[1], raw_b[2], raw_b[3], leftover);
  if (scanline != cmd_length)
  {
    if (scanline < cmd_length)
    {
      fprintf (stderr, "\nError: Not enough arguments,\n");
    }
    if (scanline > cmd_length)
    {
      fprintf (stderr, "\nError: Too many arguments,\n");
    }
    fprintf (stderr, "%s %d.\n", error_string, line_number-1);
    return (-1);
  }

  int value[cmd_length];
  int i;
  for (i = 0; i < cmd_length; i++)
  {
    if (raw_b[i][0] == '$')
    {
      unsigned long long temp = get_number_variable_value_ccc(raw_b[i]);
      if (return_value_ccc < 0)
      {
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
      value[i] = temp;
    }
    else
    {
      unsigned long long temp = strtoll(raw_b[i], &endptr, 0);
      if (*endptr)
      {
        fprintf (stderr, "Error! Value \'%s\' was unable to be completely processed.\n", raw_b[i]);
        fprintf (stderr, "%s %d.\n", error_string, line_number-1);
        return (-1);
      }
      value[i] = temp;
    }
  }


  if (!perform_check)
  {
    return_value_ccc = do_send_usbd_control_msg_ccc(value[0], value[1], value[2], value[3], usb_timeout_ccc);
    //return (return_value_ccc);
  }

  return 0;
}





int usb_raw_read_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  (void) line_number;
  (void) rest_of_line;
  if (!perform_check)
  {
    return_value_ccc = do_usb_raw_read_ccc(usb_timeout_ccc);
    //return (return_value_ccc);
  }
  return 0;
}





int usb_raw_write_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  (void) line_number;
  (void) rest_of_line;
  if (!perform_check)
  {
    return_value_ccc = do_usb_raw_write_ccc(usb_timeout_ccc);
    //return (return_value_ccc);
  }
  return 0;
}





int usb_reset_ccc(bool perform_check, unsigned int line_number, char *rest_of_line)
{
  (void) line_number;
  (void) rest_of_line;
  if (!perform_check)
  {
    return_value_ccc = do_usb_reset_ccc();
    //return (return_value_ccc);
  }
  return 0;
}





























