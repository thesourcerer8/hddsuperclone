// Copyright (C) 2015-2022 Scott Dwyer.
// You may use/distribute/modify this freely, under the terms of
// the GNU General Public License version 2 or later version.
// This software is distributed WITHOUT ANY WARRANTY.


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "common.h"
#include "hddsupertool.h"
#include "hddsupertool_help.h"



char *title = "hddsupertool";
char *version_number = "1.10-1.8 20170129x";
int copyright_year = 2017;


char *debugfile_ccc = "hddsupertool.log";

//char *argument_ccc[MAX_ARGUMENTS];
//unsigned int argument_count_ccc = 0;
//bool check_only_ccc = false;
//bool do_indentation_ccc = false;
//bool quiet_ccc = false;
//int ccc_indent_ccc = 0;
//int spaces_ccc = 0;
unsigned int script_rows_ccc = 1000;
unsigned int number_variable_rows_ccc = 100;
unsigned int string_variable_rows_ccc = 100;
char *menufile1_ccc = "hddmenu";
char *menufile2_ccc = "hddscripts/hddmenu";
char *menufile3_ccc = "/usr/local/bin/hddscripts/hddmenu";
char script_directory1_ccc[PATH_MAX] = "";
char script_directory2_ccc[PATH_MAX] = "hddscripts/";
char script_directory3_ccc[PATH_MAX] = "/usr/local/bin/hddscripts/";
char script_directory4_ccc[PATH_MAX] = "";
char *current_script_directory_ccc = "";
char full_script_path_ccc[PATH_MAX];
char *script_line_buffer_ccc;
extern char **script_line_pointer_ccc;
unsigned int total_lines_ccc;
char *number_variable_name_buffer_ccc;
char **number_variable_name_pointer_ccc;
unsigned int total_number_variables_ccc;
char *string_variable_name_buffer_ccc;
char **string_variable_name_pointer_ccc;
char *string_variable_buffer_ccc;
unsigned int total_string_variables_ccc;


bool forced_exit_ccc=false;



// Function to handle ctrl-c
void signal_callback_handler_ccc(int signum)
{
  if (!critical_process_ccc)
  {
  fprintf(stderr, "\nTerminated by user\n");
  forced_exit_ccc = true;
  cleanup_ccc();
  exit(signum);
  }
  pending_exit_ccc = true;
}

int fail6_ccc=0;
int fail7_ccc=0;
int fail9_ccc=0;
int fail10_ccc=0;
int fail11_ccc=0;
int fail12_ccc=0;
int fail13_ccc=0;




int main (int argc, char **argv)
{
  // Register ctrl-c signal and signal handler
  signal(SIGINT, signal_callback_handler_ccc);

  //fprintf (stdout, "argument count= %d\n", argc);

  critical_process_ccc = false;
  pending_exit_ccc = false;


  // set start_time_ccc
  start_time_ccc = time(NULL);


  // set some variables
  superclone_ccc = false;
  force_stupid_ccc = false;
  verbose_ccc = 0;
  debug_ccc = 0;
  command_line_ccc = false;
  fail6_ccc = 0;
  fail7_ccc = 0;
  fail9_ccc = 0;
  fail10_ccc = 0;
  fail11_ccc = 0;
  fail12_ccc = 0;
  fail13_ccc = 0;
  check_command_ccc = 0;
  check_read_ccc = 0;
  check_write_ccc = 0;
  append_script_ccc = 0;
  exitcode_ccc = 0;
  pagesize_ccc = getpagesize();
  if (pagesize_ccc > 8192)
  {
    fprintf (stderr, "Internal Error, pagesize too great\n");
    exit (1);
  }
  direct_mode_ccc = false;
  ahci_mode_ccc = false;
  ata_passthrough_ccc = true;
  scsi_passthrough_ccc = false;
  table_address_changed_ccc = false;
  ahci_address_changed_ccc = false;
  ahci_interrupt_changed_ccc = false;
  memory_mapped_ccc = false;
  io_permissions_ccc = false;
  reg_base_address_ccc = 0;
  control_base_address_ccc = 0;
  bus_base_address_ccc = 0;
  hba_base_address_ccc = 0;
  port_base_address_ccc = 0;
  max_dma_size_ccc = pagesize_ccc;
  real_buffer_size_ccc = MAX_BUFFER_SIZE;
  soft_reset_time_ccc = SOFT_RESET_TIMER;
  hard_reset_time_ccc = HARD_RESET_TIMER;
  busy_wait_time_ccc = BUSY_WAIT_TIMER;
  reset_wait_time_ccc = RESET_WAIT_TIMER;
  general_timeout_ccc = GENERAL_TIMER;
  initial_busy_wait_time_ccc = 5000000; // 5 sec
  ata_return_valid_ccc = 1;



#ifdef FULLVERSION
  // get executable file name and current directory path
  if (getcwd(start_working_directory_ccc, sizeof(start_working_directory_ccc)) == NULL)
  {
    path_fail_ccc = true;
  }
  strncpy (called_name_ccc, argv[0], sizeof(called_name_ccc)-1);
  called_name_ccc[sizeof(called_name_ccc)-1] = '\0';
  if (called_name_ccc[0] == '/')
  {
    strcpy (full_exec_path_ccc, called_name_ccc);
    found_exec_ccc = true;
  }
  else if (called_name_ccc[0] != '.')
  {
    strcpy (full_exec_path_ccc, install_directory_ccc);
    strcat (full_exec_path_ccc, called_name_ccc);
    found_exec_ccc = true;
  }
  else if (!path_fail_ccc && called_name_ccc[1] == '/')
  {
    strcpy (full_exec_path_ccc, start_working_directory_ccc);
    strcat (full_exec_path_ccc, called_name_ccc+1);
    found_exec_ccc = true;
  }
  else
  {
    found_exec_ccc = false;
  }
#endif



  // begin processing command line arguments
  int command_line_argument;
  int arguments_required;       // required number of non-option arguments
  bool command_line_error = false;      // initialize error to false

  while (1)
  {
    static struct option long_options[] =
    {
      {"help",          no_argument,    0, 'h'},
      {"version",       no_argument,    0, 'v'},
      {"debug",         no_argument,    0, 'D'},
      {"verbose",       no_argument,    0, 'V'},
      {"check",         no_argument,    0, 'c'},
      {"command",       no_argument,    0, 'C'},
      {"direct",        no_argument,    0, 'd'},
      {"ahci",          no_argument,    0, 'a'},
      {"ata",           no_argument,    0, ATAMODE},
      {"scsi",          no_argument,    0, SCSIMODE},
      {"file",          required_argument, 0, 'f'},
      {"indent",        required_argument, 0, 'i'},
      {"target",        required_argument, 0, 't'},
      {"license",       required_argument, 0, 'L'},
      {"regaddress",    required_argument, 0, REGADDR},
      {"conaddress",    required_argument, 0, CONADDR},
      {"busaddress",    required_argument, 0, BUSADDR},
      {"device",        required_argument, 0, DEVICE},
      {"hbaaddress",    required_argument, 0, HBAADDR},
      {"portnumber",    required_argument, 0, PORTNUM},
      {"portaddress",   required_argument, 0, PORTADDR},
      {"serial",        required_argument, 0, 's'},
      {"quiet_ccc",         no_argument,    0, 'Q'},
      {"crash-computer-destroy-data",         no_argument,    0, CRASH},
      {0, 0, 0, 0}
    };
    // getopt_long stores the option index here.
    int option_index = 0;

    command_line_argument = getopt_long (argc, argv, "hvDVcCdaQf:i:t:L:s:",
                                         long_options, &option_index);

    // Detect the end of the options.
    if (command_line_argument == -1)
      break;

    switch (command_line_argument)
    {
      case 'h':
        help_ccc();
        exit(0);

      case 'v':
        version_ccc();
        exit(0);

      case 'D':
        debug_ccc++;
        break;

      case 'V':
        verbose_ccc++;
        break;

      case 'c':
        check_only_ccc = true;
        break;

      case 'C':
        command_line_ccc = true;
        break;

      case 'i':
        spaces_ccc = strtoul(optarg, NULL, 0);
        do_indentation_ccc = true;
        check_only_ccc = true;
        break;

      case REGADDR:
        reg_base_address_ccc = strtoul(optarg, NULL, 16);
        break;

      case CONADDR:
        control_base_address_ccc = strtoul(optarg, NULL, 16);
        break;

      case BUSADDR:
        bus_base_address_ccc = strtoul(optarg, NULL, 16);
        break;

      case DEVICE:
        device_select_base_ccc = strtoul(optarg, NULL, 0);
        break;

      case HBAADDR:
        hba_base_address_ccc = strtoul(optarg, NULL, 16);
        break;

      case PORTNUM:
        port_number_base_ccc = strtoul(optarg, NULL, 0);
        break;

      case PORTADDR:
        port_base_address_ccc = strtoul(optarg, NULL, 16);
        break;

      case 'd':
        direct_mode_ccc = true;
        break;

      case 'a':
        direct_mode_ccc = true;
        ahci_mode_ccc = true;
        break;

      case ATAMODE:
        ata_passthrough_ccc = true;
        scsi_passthrough_ccc = false;
        break;

      case SCSIMODE:
        ata_passthrough_ccc = false;
        scsi_passthrough_ccc = true;
        break;

      case 't':
        disk_1_ccc = optarg;
        break;

      case 'f':
        script_file_ccc = optarg;
        break;

      case 's':
        drive_serial_ccc = optarg;
        break;

      case 'L':
        version_ccc();
        exit(0);
        break;

      case 'Q':
        quiet_ccc = true;
        break;

      case CRASH:
        force_stupid_ccc = true;
        break;

      case '?':
        // getopt_long already printed an error message.
        command_line_error = true;
        break;

      default:
        fprintf (stderr, "Error processing command line options\n");
        exit (1);
    }
  }



  if (do_indentation_ccc)
  {
    arguments_required = 0;
    if ((argc - optind) != arguments_required)
    {
      command_line_error = true;
      if ((argc - optind) < arguments_required)
      {
        fprintf (stderr, "Error: too few arguments\n");
      }
      if ((argc - optind) > arguments_required)
      {
        fprintf (stderr, "Error: too many arguments\n");
      }
    }
  }

  else if (command_line_ccc)
  {
    arguments_required = 0;
    if ((argc - optind) < arguments_required)
    {
      command_line_error = true;
      if ((argc - optind) < arguments_required)
      {
        fprintf (stderr, "Error: too few arguments\n");
      }
      if ((argc - optind) > arguments_required)
      {
        fprintf (stderr, "Error: too many arguments\n");
      }
    }

    // get remaining arguments that are not options
    while ( (argc - optind) > 0)
    {
      argument_ccc[argument_count_ccc] = argv[optind++];
      argument_count_ccc ++;
    }

    if (argument_count_ccc > MAX_ARGUMENTS)
    {
      fprintf (stderr, "ERROR! Number of arguments exceeded the maximum of %d.\n", MAX_ARGUMENTS);
      exit (1);
    }

    if (verbose_ccc > 0)
    {
      unsigned int c;
      for (c = 0; c < argument_count_ccc; c++)
        fprintf (stdout, "argument%d= %s\n", c+1, argument_ccc[c]);
    }
    if (debug_ccc > 0)
    {
      unsigned int c;
      for (c = 0; c < argument_count_ccc; c++)
        fprintf (debug_file_ccc, "argument%d= %s\n", c+1, argument_ccc[c]);
    }
  }

  else
  {
    arguments_required = 0;
    if ((argc - optind) < arguments_required)
    {
      command_line_error = true;
      if ((argc - optind) < arguments_required)
      {
        fprintf (stderr, "Error: too few arguments\n");
      }
      if ((argc - optind) > arguments_required)
      {
        fprintf (stderr, "Error: too many arguments\n");
      }
    }

    // get remaining arguments that are not options
    while ( (argc - optind) > 0)
    {
      argument_ccc[argument_count_ccc] = argv[optind++];
      argument_count_ccc ++;
    }

    if (argument_count_ccc > MAX_ARGUMENTS)
    {
      fprintf (stderr, "ERROR! Number of arguments exceeded the maximum of %d.\n", MAX_ARGUMENTS);
      exit (1);
    }

    if (verbose_ccc > 0)
    {
      unsigned int c;
      for (c = 0; c < argument_count_ccc; c++)
        fprintf (stdout, "argument%d= %s\n", c+1, argument_ccc[c]);
    }
    if (debug_ccc > 0)
    {
      unsigned int c;
      for (c = 0; c < argument_count_ccc; c++)
        fprintf (debug_file_ccc, "argument%d= %s\n", c+1, argument_ccc[c]);
    }
  }



  // exit on command line error
  if (command_line_error)
  {
    fprintf (stderr, "%s: Both disk and script must be specified.\n", title);
    fprintf (stderr, "Try '%s --help' for more information.\n", title);
    exit (3);
  }

  //check if debug is turned on and open debug file if it is
  if (debug_ccc > 0)
  {
    debug_file_ccc = fopen(debugfile_ccc, "w");
    if (debug_file_ccc == NULL)
    {
      fprintf(stderr, "Cannot open %s for writing (%s).\nAborting...\n", debugfile_ccc, strerror(errno));
      exit (4);
    }
  }

  if (debug_ccc > 0)
  {
    fprintf (debug_file_ccc, "%s debug file created at ", title);
    time_t mytime;
    mytime = time(NULL);
    fprintf (debug_file_ccc, "%s", ctime(&mytime));
    fprintf (debug_file_ccc, "disk_1 = %s\n", disk_1_ccc);
    fprintf (debug_file_ccc, "script_file = %s\n", script_file_ccc);
  }

  // end of command line processing





  if (!quiet_ccc)
  {
    fprintf (stdout, "%s %s\n", title, version_number);
#ifdef DEBUG
    fprintf (stdout, "DEBUG version\n");
#endif
  }

  // create random data for later use
  memset (random_data_ccc, 0, sizeof(random_data_ccc));
  int i;
  for (i = 0; i < 1024; i += 4)
  {
    int n = get_random_value_ccc(100);
    random_data_ccc[i] = n;
    random_data_ccc[i+1] = n >> 8;
    random_data_ccc[i+2] = n >> 16;
    random_data_ccc[i+3] = n >> 24;
  }

#ifdef GODMODE
  superbyte_ccc[0] = 0x04;
  superbyte_ccc[1] = 0x06;
  superbyte_ccc[2] = 0x08;
  superbyte_ccc[3] = 0x09;
  superbyte_ccc[4] = 0x00;
  superbyte_ccc[5] = 0x01;
  superbyte_ccc[6] = 0x38;
  superbyte_ccc[7] = 0x10;
  superbyte_ccc[8] = 0x20;
  superbyte_ccc[9] = 0x21;
  superbyte_ccc[10] = 0x18;
  superbyte_ccc[11] = 0x00;
  superbyte_ccc[12] = 0x08;
  superbyte_ccc[13] = 0x14;
  superbyte_ccc[14] = 0x19;
  superbyte_ccc[15] = 0x80;
  superbyte_ccc[16] = 0x2c;
  superbyte_ccc[17] = 0x28;
  superbyte_ccc[18] = 0x30;
  superbyte_ccc[19] = 0x0c;

  license_type_ccc = 0xff;
  sprintf (tempmessage_ccc, "GOD MODE ACTIVE\n");
  message_now_ccc(tempmessage_ccc);
#else
#ifdef NOTFREE
  print_license_ccc(0);

  unsigned char sid[8];
  sid[0] = random_data_ccc[0];
  sid[1] = random_data_ccc[1];
  sid[2] = rotl8_ccc(rotl8_ccc(sid[0], 1) ^ rotl8_ccc(sid[1], 1) ^ rotl8_ccc(sbyte_ccc[0], 1) , 1);
  sid[3] = rotl8_ccc(rotl8_ccc(sid[0], 2) ^ rotl8_ccc(sid[1], 2) ^ rotl8_ccc(sbyte_ccc[1], 2) , 2);
  sid[4] = rotl8_ccc(rotl8_ccc(sid[0], 3) ^ rotl8_ccc(sid[1], 3) ^ rotl8_ccc(sbyte_ccc[2], 3) , 3);
  unsigned char temp = (sbyte_ccc[3] & 128) | license_information_ccc;
  sid[5] = rotl8_ccc(rotl8_ccc(sid[0], 4) ^ rotl8_ccc(sid[1], 4) ^ rotl8_ccc(temp, 4) , 4);
  sid[6] = rotl8_ccc(rotl8_ccc(sid[0], 5) ^ rotl8_ccc(sid[1], 5) ^ rotl8_ccc((expire_day_ccc >> 8) & 255, 5) , 5);
  sid[7] = rotl8_ccc(rotl8_ccc(sid[0], 6) ^ rotl8_ccc(sid[1], 6) ^ rotl8_ccc((expire_day_ccc & 255), 6) , 6);

  if (!quiet_ccc)
  {
    fprintf (stdout, "SessionID: %02x%02x%02x%02x%02x%02x%02x%02x\n", sid[0], sid[1], sid[2], sid[3], sid[4], sid[5], sid[6], sid[7]);
  }
#endif
#endif

  max_dma_size_ccc = (pagesize_ccc / 8) * pagesize_ccc;
  if (ahci_mode_ccc)
  {
    max_dma_size_ccc = ( (pagesize_ccc - 128) / 16 ) * pagesize_ccc;
  }

  return_value_ccc = initialize_memory_ccc();
  if (return_value_ccc != 0)
  {
    cleanup_ccc();
    exit (1);
  }

  return_value_ccc = process_arguments_ccc();
  if (return_value_ccc != 0)
  {
    cleanup_ccc();
    exit (1);
  }


  // if not just checking the script then make sure drive info is set
  if (!check_only_ccc)
  {
    // Check if root privilages
    if (geteuid())
    {
      fprintf (stderr, "Root privilages not detected! This program must be run as root.\n");
      cleanup_ccc();
      exit (1);
    }
    if (ahci_mode_ccc)
    {
      // if direct io ahci and addresses are not all set then find and choose device
      if (hba_base_address_ccc == 0 || port_base_address_ccc == 0)
      {
        find_all_devices_ccc();
        return_value_ccc = choose_device_ccc();
        if (return_value_ccc != 0)
        {
          cleanup_ccc();
          exit (1);
        }
      }
    }
    else if (direct_mode_ccc)
    {
      // if direct io ide and addresses are not all set then find and choose device
      if (reg_base_address_ccc == 0 || control_base_address_ccc == 0 || bus_base_address_ccc == 0)
      {
        find_all_devices_ccc();
        return_value_ccc = choose_device_ccc();
        if (return_value_ccc != 0)
        {
          cleanup_ccc();
          exit (1);
        }
      }
    }
    else
    {
      // if passthrough and disk is not set then find and choose device
      if (disk_1_ccc == NULL)
      {
        find_all_devices_ccc();
        return_value_ccc = choose_device_ccc();
        if (return_value_ccc != 0)
        {
          cleanup_ccc();
          exit (1);
        }
      }
    }


    if (direct_mode_ccc)
    {
      if (ahci_mode_ccc)
      {
        // if ahci then check to make sure addresses were all set
        if (hba_base_address_ccc == 0 || port_base_address_ccc == 0)
        {
          fprintf (stderr, "Error: Port addresses not set for AHCI mode.\n");
          cleanup_ccc();
          exit (1);
        }
        long long calculated = port_base_address_ccc - (hba_base_address_ccc + 0x100 + (port_number_base_ccc * 0x80));
        if (calculated != 0)
        {
          fprintf (stderr, "Error: Port number does not match address for AHCI mode.\n");
          cleanup_ccc();
          exit (1);
        }
      }

      else
      {
        // if direct io then check to make sure addresses were all set
        if (reg_base_address_ccc == 0 || control_base_address_ccc == 0 || bus_base_address_ccc == 0)
        {
          fprintf (stderr, "Error: Port addresses not set for direct IO mode.\n");
          cleanup_ccc();
          exit (1);
        }
#if 0
        if (device_select_base_ccc > 1)
        {
          fprintf (stderr, "Error: Device select can only be 0 or 1.\n");
          cleanup_ccc();
          exit (1);
        }
#endif
      }
    }

    else
    {
      // if passthrough then make sure the drive is chosen
      if (disk_1_ccc == NULL)
      {
        find_all_devices_ccc();
        fprintf (stderr, "Error: Target not set for passthrough mode.\n");
        cleanup_ccc();
        exit (1);
      }
    }
  }


  if (command_line_ccc)
  {
    process_command_line_ccc();
  }
  else
  {
    int script_status = 1;
    while (script_status == 1)
    {
      return_value_ccc = read_script_file_ccc(script_file_ccc);
      if (return_value_ccc != 0)
      {
        cleanup_ccc();
        exit (1);
      }
      // make sure timers are reset between scripts
      soft_reset_time_ccc = SOFT_RESET_TIMER;
      hard_reset_time_ccc = HARD_RESET_TIMER;
      busy_wait_time_ccc = BUSY_WAIT_TIMER;
      general_timeout_ccc = GENERAL_TIMER;

      script_status = process_lines_ccc();
      if (script_status < 0 || script_status > 1)
      {
        cleanup_ccc();
        exit (script_status);
      }
    }
  }

  if (do_indentation_ccc)
  {
    write_file_ccc(script_file_ccc);
  }




  // final cleanup at end of main
  cleanup_ccc();


  exit (exitcode_ccc);

}
// end of main
//******************************************************











// function to cleanup before exit
void cleanup_ccc(void)
{
#ifdef FULLVERSION

  int success = 1;
  unsigned int checksum = 0;
  unsigned long long file_size = 0;
  if (found_exec_ccc == true)
  {
    //fprintf (stdout, "Full path: %s\n", full_exec_path_ccc);

    FILE *readfile = fopen(full_exec_path_ccc, "rb");
    if (readfile == NULL)
    {
      //fprintf(stderr, "Cannot open %s for reading (%s).\nAborting...\n", full_exec_path_ccc, strerror(errno));
      success = 0;
    }
    if (fseeko(readfile, 0, SEEK_END) != 0 )
    {
      //fprintf(stderr, "Error seeking %s (%s).\n", full_exec_path_ccc, strerror(errno));
      success = 0;
    }
    else
    {
      file_size = ftello(readfile);
      fseeko(readfile, 0, SEEK_SET);
    }
    if (success)
    {
      unsigned char file_data[file_size];
      if (fread(file_data, 1, file_size, readfile) == file_size )
      {
        unsigned long long i;
        for (i = 0; i < file_size; i++)
        {
          //fprintf (stdout, "%02x", file_data[i]);
          checksum += file_data[i] ;
        }
        //fprintf (stdout, "\n");
      }
    }
    fclose (readfile);
  }


#endif


  if (debug_ccc > 0)
  {
    fclose(debug_file_ccc);
  }

  if (ahci_address_changed_ccc)
  {
    // restore addresses
    memcpy(port_virt_addr_ccc + superbyte_ccc[11], &command_list_address_backup_ccc, 8);
    memcpy(port_virt_addr_ccc + superbyte_ccc[12], &fis_address_backup_ccc, 8);
  }

  if (ahci_interrupt_changed_ccc)
  {
    // restore the interrupt settings
    memcpy(port_virt_addr_ccc + superbyte_ccc[13], &interrupt_backup_ccc, 4);
  }

  if (table_address_changed_ccc)
  {
    outb(table_address_backup_ccc[0], bus_base_address_ccc+0+superbyte_ccc[10]);
    outb(table_address_backup_ccc[1], bus_base_address_ccc+1+superbyte_ccc[10]);
    outb(table_address_backup_ccc[2], bus_base_address_ccc+2+superbyte_ccc[10]);
    outb(table_address_backup_ccc[3], bus_base_address_ccc+3+superbyte_ccc[10]);
  }

  if (memory_mapped_ccc)
  {
    // close mapped memory
    munmap(hba_mem_pointer_ccc, hba_alloc_mem_size_ccc);
    close(hba_mem_dev_ccc);
    munmap(port_mem_pointer_ccc, port_alloc_mem_size_ccc);
    close(port_mem_dev_ccc);
  }

  if (io_permissions_ccc)
  {
    // turn interrupts back on, but make sure correct device first
    set_device_ccc(reg_base_address_ccc, device_select_base_ccc);
    outb(0, control_base_address_ccc);
    // turn off io permissions
    ioperm (reg_base_address_ccc, 8, 0);
    ioperm (control_base_address_ccc, 1, 0);
    ioperm (bus_base_address_ccc, 8, 0);
  }

  close (disk1_fd_ccc);

  free (script_line_buffer_ccc);
  free (script_line_pointer_ccc);
  free (number_variable_name_buffer_ccc);
  free (number_variable_name_pointer_ccc);
  free (number_variable_buffer_ccc);
  free (string_variable_name_buffer_ccc);
  free (string_variable_name_pointer_ccc);
  free (string_variable_buffer_ccc);
  free (string_variable_pointer_ccc);
  free (ccc_buffer_ccc);
  free (ccc_scratchpad_ccc);
  free (table_buffer_ccc);

  // print time elapsed
  end_time_ccc = time(NULL);
  if (!quiet_ccc)
  {
    fprintf (stdout, "Total program run time = %ld seconds\n", end_time_ccc - start_time_ccc);
  }
}


// function to display help
void help_ccc(void)
{
  unsigned int i;
  for (i = 0; i < hddsupertool_help_txt_len; i++)
  {
    printf ("%c", hddsupertool_help_txt[i]);
  }
}

// function to display version
void version_ccc(void)
{
  printf ("%s %s\n", title, version_number);
  printf ("Copyright (C) %d Scott Dwyer.\n", copyright_year);
  printf ("License type: FREE SOFTWARE, GPLv2\n");
  printf ("There is NO WARRANTY, to the extent permitted by law.\n");
}





// function to initialize needed memory
int initialize_memory_ccc(void)
{
  // assign memory for number variables
  total_number_variables_ccc = 0;
  unsigned int cols = MAX_VARIABLE_LENGTH;
  unsigned int i;
  number_variable_name_buffer_ccc = malloc(number_variable_rows_ccc * cols * sizeof(*number_variable_name_buffer_ccc));
  if (number_variable_name_buffer_ccc == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (-1);
  }
  number_variable_name_pointer_ccc = malloc(number_variable_rows_ccc * sizeof(*number_variable_name_pointer_ccc));
  if (number_variable_name_pointer_ccc == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (-1);
  }
  for (i = 0; i < number_variable_rows_ccc; ++i)
  {
    number_variable_name_pointer_ccc[i] = &number_variable_name_buffer_ccc[i * cols];
  }

  number_variable_buffer_ccc = malloc(number_variable_rows_ccc * sizeof(*number_variable_buffer_ccc));
  if (number_variable_buffer_ccc == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (-1);
  }


  // assign memory for string variables
  total_string_variables_ccc = 0;
  string_variable_name_buffer_ccc = malloc(string_variable_rows_ccc * cols * sizeof(*string_variable_name_buffer_ccc));
  if (string_variable_name_buffer_ccc == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (-1);
  }
  string_variable_name_pointer_ccc = malloc(string_variable_rows_ccc * sizeof(*string_variable_name_pointer_ccc));
  if (string_variable_name_pointer_ccc == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (-1);
  }
  for (i = 0; i < string_variable_rows_ccc; ++i)
  {
    string_variable_name_pointer_ccc[i] = &string_variable_name_buffer_ccc[i * cols];
  }

  string_variable_buffer_ccc = malloc(string_variable_rows_ccc * cols * sizeof(*string_variable_buffer_ccc));
  if (string_variable_buffer_ccc == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (-1);
  }
  string_variable_pointer_ccc = malloc(string_variable_rows_ccc * sizeof(*string_variable_pointer_ccc));
  if (string_variable_pointer_ccc == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (-1);
  }
  for (i = 0; i < string_variable_rows_ccc; ++i)
  {
    string_variable_pointer_ccc[i] = &string_variable_buffer_ccc[i * cols];
  }

  if (!quiet_ccc)
  {
    fprintf (stdout, "Initializing memory\n");
  }

  // initialize the table buffer
  if (direct_mode_ccc)
  {
    return_value_ccc = set_table_buffer_ccc();
    if (return_value_ccc != 0)
    {
      fprintf (stderr, "Unable to get table buffer physical address in 32bit range\n");
      return (-1);
    }
  }


  // initialize the command list buffer
  if (ahci_mode_ccc)
  {
    return_value_ccc = set_command_list_buffer_ccc();
    if (return_value_ccc != 0)
    {
      fprintf (stderr, "Unable to get command list buffer physical address in 32bit range\n");
      return (-1);
    }
  }


  // initialize the FIS buffer
  if (ahci_mode_ccc)
  {
    return_value_ccc = set_fis_buffer_ccc();
    if (return_value_ccc != 0)
    {
      fprintf (stderr, "Unable to get FIS buffer physical address in 32bit range\n");
      return (-1);
    }
  }


  // initialize main buffer
  // create a buffer that is memory aligned with the pagesize
  if (direct_mode_ccc && fullversion_ccc)
  {
    return_value_ccc = get_buffer_physical_memory_locations_ccc();
    if (return_value_ccc != 0)
    {
      return (return_value_ccc);
    }
  }
  else
  {
    unsigned int align = pagesize_ccc;
    free (ccc_buffer_ccc);
    if (posix_memalign(&ccc_buffer_ccc, align, real_buffer_size_ccc))
    {
      perror("posix_memalign failed");
      return (-1);
    }
    memset (ccc_buffer_ccc, 0, real_buffer_size_ccc);
  }


  // set the starting main buffer size
  ccc_main_buffer_size_ccc = 512;
  return_value_ccc = set_main_buffer_ccc();
  if (return_value_ccc != 0)
  {
    return (-1);
  }

  // set the starting scratchpad size
  ccc_main_scratchpad_size_ccc = 512;
  return_value_ccc = set_main_scratchpad_ccc();
  if (return_value_ccc != 0)
  {
    return (-1);
  }

  // set the sensebuffer size
  sensebuffer_size_ccc = 256;
  return_value_ccc = set_sense_buffer_ccc();
  if (return_value_ccc != 0)
  {
    return (-1);
  }

  return (0);
}





// function to increase total number variable memory
int increase_number_variable_memory_ccc(void)
{
  // if used up allocated memory then increase it
  number_variable_rows_ccc += 100;
  unsigned int cols = MAX_VARIABLE_LENGTH;
  unsigned int i;

  number_variable_name_buffer_ccc = realloc(number_variable_name_buffer_ccc, number_variable_rows_ccc * cols * sizeof(*number_variable_name_buffer_ccc));
  if (number_variable_name_buffer_ccc == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (-1);
  }
  number_variable_name_pointer_ccc = realloc(number_variable_name_pointer_ccc, number_variable_rows_ccc * sizeof(*number_variable_name_pointer_ccc));
  if (number_variable_name_pointer_ccc == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (-1);
  }
  for (i = 0; i < number_variable_rows_ccc; ++i)
  {
    number_variable_name_pointer_ccc[i] = &number_variable_name_buffer_ccc[i * cols];
  }

  number_variable_buffer_ccc = realloc(number_variable_buffer_ccc, number_variable_rows_ccc * sizeof(*number_variable_buffer_ccc));
  if (number_variable_buffer_ccc == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (-1);
  }
  return (0);
}





// function to increase total string variable memory
int increase_string_variable_memory_ccc(void)
{
  // if used up allocated memory then increase it
  string_variable_rows_ccc += 100;
  unsigned int cols = MAX_VARIABLE_LENGTH;
  unsigned int i;

  string_variable_name_buffer_ccc = realloc(string_variable_name_buffer_ccc, string_variable_rows_ccc * cols * sizeof(*string_variable_name_buffer_ccc));
  if (string_variable_name_buffer_ccc == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (-1);
  }
  string_variable_name_pointer_ccc = realloc(string_variable_name_pointer_ccc, string_variable_rows_ccc * sizeof(*string_variable_name_pointer_ccc));
  if (string_variable_name_pointer_ccc == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (-1);
  }
  for (i = 0; i < string_variable_rows_ccc; ++i)
  {
    string_variable_name_pointer_ccc[i] = &string_variable_name_buffer_ccc[i * cols];
  }

  string_variable_buffer_ccc = realloc(string_variable_buffer_ccc, string_variable_rows_ccc * cols * sizeof(*string_variable_buffer_ccc));
  if (string_variable_buffer_ccc == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (-1);
  }
  string_variable_pointer_ccc = realloc(string_variable_pointer_ccc, string_variable_rows_ccc * sizeof(*string_variable_pointer_ccc));
  if (string_variable_pointer_ccc == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (-1);
  }
  for (i = 0; i < string_variable_rows_ccc; ++i)
  {
    string_variable_pointer_ccc[i] = &string_variable_buffer_ccc[i * cols];
  }

  return (0);
}





// function to set scratchpad buffer
int set_main_scratchpad_ccc(void)
{
  if (ccc_main_scratchpad_size_ccc > MAX_SCRATCHPAD_SIZE)
  {
    fprintf (stderr, "ERROR: Maximum scratchpad size exceeded.\n");
    return (-1);
  }
  free (ccc_scratchpad_ccc);
  ccc_scratchpad_ccc = calloc(ccc_main_scratchpad_size_ccc, 1);
  if (ccc_scratchpad_ccc == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (-1);
  }
  return (0);
}






// function to set sense buffer
int set_sense_buffer_ccc(void)
{
  if (sensebuffer_size_ccc > MAX_SENSEBUFFER_SIZE)
  {
    fprintf (stderr, "ERROR: Maximum sensebuffer size exceeded.\n");
    return (-1);
  }
  free (sensebuffer_ccc);
  sensebuffer_ccc = calloc(sensebuffer_size_ccc, 1);
  if (sensebuffer_ccc == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (-1);
  }
  return (0);
}





// function to process the remaining arguments into variables
int process_arguments_ccc(void)
{
  // create default variables
  create_number_variable_ccc("error_level");
  create_number_variable_ccc("io_sense_key");
  create_number_variable_ccc("io_asc");
  create_number_variable_ccc("io_ascq");
  create_number_variable_ccc("io_status");
  create_number_variable_ccc("io_masked_status");
  create_number_variable_ccc("io_msg_status");
  create_number_variable_ccc("io_sb_len_wr");
  create_number_variable_ccc("io_host_status");
  create_number_variable_ccc("io_driver_status");
  create_number_variable_ccc("io_resid");
  create_number_variable_ccc("io_duration");
  create_number_variable_ccc("io_info");
  create_number_variable_ccc("time");
  create_string_variable_ccc("date");
  create_number_variable_ccc("ata_return_error");
  create_number_variable_ccc("ata_return_count");
  create_number_variable_ccc("ata_return_lba");
  create_number_variable_ccc("ata_return_device");
  create_number_variable_ccc("ata_return_status");
  create_number_variable_ccc("ata_alternate_status");
  create_number_variable_ccc("data_transferred");
  create_number_variable_ccc("command_status");
  create_number_variable_ccc("bus_master_status");
  create_number_variable_ccc("direct_mode");
  create_number_variable_ccc("ahci_mode");

  // initially set direct mode variable if needed
  if (direct_mode_ccc)
  {
    set_number_variable_value_ccc("$direct_mode", 1);
  }
  if (ahci_mode_ccc)
  {
    set_number_variable_value_ccc("$ahci_mode", 1);
  }

  unsigned int c;
  for (c = 0; c < argument_count_ccc; c++)
  {
    if (verbose_ccc > 0)
    {
      fprintf (stdout, "argument%d= %s\n", c+1, argument_ccc[c]);
    }
    if (debug_ccc > 0)
    {
      fprintf (debug_file_ccc, "argument%d= %s\n", c+1, argument_ccc[c]);
    }
    char current_argument[MAX_VARIABLE_LENGTH];
    char var_name[MAX_VARIABLE_NAME_LENGTH];
    char variable[MAX_VARIABLE_LENGTH];
    strcpy (current_argument, argument_ccc[c]);
    int length = strlen(current_argument);
    int var_num;
    int i;
    int n;
    char var_type = '\0';
    for (i = 0; i < length; i++)
    {
      if (current_argument[i] == '=')
      {
        n = i;
        if (current_argument[i+1] == '=')
        {
          var_type = 's';
        }
        else
        {
          var_type = 'i';
        }

        strncpy (var_name, current_argument, n);
        var_name[n] = '\0';
        if (var_type == 's')
        {
          strcpy (variable, current_argument+n+2);
        }
        else
        {
          strcpy (variable, current_argument+n+1);
        }
        //fprintf (stdout, "var_name= %s\n", var_name);
        //fprintf (stdout, "variable= %s\n", variable);
        //fprintf (stdout, "type= %c\n", var_type);

        if (var_type == 'i')
        {
          return_value_ccc = check_variable_ccc(var_name);
          if (return_value_ccc != 0)
          {
            fprintf (stderr, "Error! Argument \'%s\' has been declared more than once in the command line.\n", var_name);
            cleanup_ccc();
            exit (1);
          }
          create_number_variable_ccc(var_name);
          var_num = find_number_variable_ccc(var_name);
          if (var_num == -1)
          {
            fprintf (stderr, "Error! Unable to find agrument variable \'%s\' after setting it.\n", var_name);
            cleanup_ccc();
            exit (1);
          }
          else
            return_value_ccc = set_number_variable_ccc(var_num, variable);
          if (return_value_ccc == -1)
          {
            fprintf (stderr, "Error! Number variable \'%s\' value \'%s\' was unable to be completely processed.\n", var_name, variable);
            cleanup_ccc();
            exit (1);
          }
          //fprintf (stdout, "%s = %lld\n", number_variable_name_pointer_ccc[var_num], number_variable_buffer_ccc[var_num]);
        }

        if (var_type == 's')
        {
          return_value_ccc = check_variable_ccc(var_name);
          if (return_value_ccc != 0)
          {
            fprintf (stderr, "Error! Argument \'%s\' has been declared more than once in the command line.\n", var_name);
            cleanup_ccc();
            exit (1);
          }
          create_string_variable_ccc(var_name);
          var_num = find_string_variable_ccc(var_name);
          if (var_num == -1)
          {
            fprintf (stderr, "Error! Unable to find agrument variable \'%s\' after setting it.\n", var_name);
            cleanup_ccc();
            exit (1);
          }
          else
            return_value_ccc = set_string_variable_ccc(var_num, variable);
          if (return_value_ccc == -1)
          {
            fprintf (stderr, "Error! Number variable \'%s\' value \'%s\' was unable to be completely processed.\n", var_name, variable);
            cleanup_ccc();
            exit (1);
          }
        }
        break;
      }
    }
    //fprintf (stdout, "\n");
    if (var_type == '\0')
    {
      fprintf (stderr, "Error processing argument \'%s\'\n", current_argument);
      cleanup_ccc();
      exit (1);
    }

  }
  return (0);
}





// function to read the script file into memory
int read_script_file_ccc(char *script_file_ccc)
{
  unsigned int cols = MAX_LINE_LENGTH;
  unsigned int i;

  if (append_script_ccc == 0)
  {
    // assign memory for the script
    free (script_line_buffer_ccc);
    free (script_line_pointer_ccc);
    script_rows_ccc = 1000;

    script_line_buffer_ccc = malloc(script_rows_ccc * cols * sizeof(*script_line_buffer_ccc));
    if (script_line_buffer_ccc == NULL)
    {
      fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
      return (-1);
    }
    script_line_pointer_ccc = malloc(script_rows_ccc * sizeof(*script_line_pointer_ccc));
    if (script_line_pointer_ccc == NULL)
    {
      fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
      return (-1);
    }
    for (i = 0; i < script_rows_ccc; ++i)
    {
      script_line_pointer_ccc[i] = &script_line_buffer_ccc[i * cols];
    }


    if (script_file_ccc == NULL)
    {
      if( access( menufile1_ccc, F_OK ) != -1 )
      {
        script_file_ccc = menufile1_ccc;
        strcpy (current_script_ccc, script_file_ccc);
        current_script_directory_ccc = script_directory1_ccc;
      }
      else if( access( menufile2_ccc, F_OK ) != -1 )
      {
        script_file_ccc = menufile1_ccc;
        strcpy (current_script_ccc, script_file_ccc);
        current_script_directory_ccc = script_directory2_ccc;
      }
      else if( access( menufile3_ccc, F_OK ) != -1 )
      {
        script_file_ccc = menufile1_ccc;
        strcpy (current_script_ccc, script_file_ccc);
        current_script_directory_ccc = script_directory3_ccc;
      }
      else
      {
        fprintf (stdout, "Error: No script file specified and hddmenu not found.\n");
        return (1);
      }
    }
    i = 0;
  }
  else
  {
    i = total_lines_ccc;
    // extract the folder path from the original so that included scipts can be loaded from same path
    int n;
    int line_size = strlen(full_script_path_ccc);
    for (n = line_size - 1; n >= 0; n--)
    {
      if (full_script_path_ccc[n] == '/')
      {
        strncpy (script_directory4_ccc, full_script_path_ccc, n+1);
        current_script_directory_ccc = script_directory4_ccc;
        break;
      }
    }
  }
  append_script_ccc = 0;

  strcpy (full_script_path_ccc, current_script_directory_ccc);
  strcat (full_script_path_ccc, script_file_ccc);


  if (verbose_ccc > 0)
  {
    fprintf (stdout, "Reading %s into memory...\n", full_script_path_ccc);
  }

  FILE *readfile;
  readfile = fopen(full_script_path_ccc, "r");
  if (readfile == NULL)
  {
    fprintf(stderr, "Cannot open %s for reading (%s).\n", full_script_path_ccc, strerror(errno));
    return (1);
  }

  char line[cols];

  // offest the script 2 lines to allow for while statement at top
  // make sure the first 2 lines are empty
  strcpy (script_line_pointer_ccc[i], "\n");
  strcpy (script_line_pointer_ccc[i+1], "\n");
  i += 2;
  while (fgets(line, sizeof line, readfile))
  {
    bool line_too_long = true;
    unsigned int n;
    for (n = 0; n < cols; n++)
    {
      // if we find end of line before the end then the line isn't too long
      if (line[n] == '\n')
      {
        line_too_long = false;
        break;
      }
    }
    for (n = 0; n < cols - 1; n++)
    {
      // if we find end of file before the end then the line isn't too long
      if (line[n] == '\0')
      {
        line_too_long = false;
        break;
      }
    }
    if (line_too_long)
    {
      fprintf (stderr, "ERROR! Line %d of %s is too long.\n", i-1, script_file_ccc);
      exit (1);
    }

    strcpy (script_line_pointer_ccc[i], line);

    i++;

    // if used up allocated memory then increase it
    if (i >= script_rows_ccc)
    {
      script_rows_ccc += 1000;
      unsigned int i;
      script_line_buffer_ccc = realloc(script_line_buffer_ccc, script_rows_ccc * cols * sizeof(*script_line_buffer_ccc));
      if (script_line_buffer_ccc == NULL)
      {
        fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
        return (3);
      }

      script_line_pointer_ccc = realloc(script_line_pointer_ccc, script_rows_ccc * sizeof(*script_line_pointer_ccc));
      if (script_line_pointer_ccc == NULL)
      {
        fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
        return (3);
      }

      for (i = 0; i < script_rows_ccc; ++i)
      {
        script_line_pointer_ccc[i] = &script_line_buffer_ccc[i * cols];
      }
    }
  }
  total_lines_ccc = i;

  if (verbose_ccc > 1)
  {
    unsigned int n;
    for (n = 2; n < total_lines_ccc; n++)
    {
      fprintf (stdout, "line%d= %s", n-1, script_line_pointer_ccc[n]);
    }
  }
  if (debug_ccc > 1)
  {
    unsigned int n;
    for (n = 2; n < total_lines_ccc; n++)
    {
      fprintf (debug_file_ccc, "line%d= %s", n-1, script_line_pointer_ccc[n]);
    }
  }
  //fprintf (stdout, "\n");

  fclose(readfile);
  return (0);
}





// function to write the script back to file after indentation cleanup
int write_file_ccc(char *script_file_ccc)
{
  fprintf (stdout, "Overwriting %s with new indentation...\n", script_file_ccc);

  FILE *writefile;
  writefile = fopen(script_file_ccc, "w");
  if (writefile == NULL)
  {
    fprintf(stderr, "Cannot open %s for writing (%s).\n", script_file_ccc, strerror(errno));
    return (1);
  }

  unsigned int line_number;
  for (line_number = 0; line_number < total_lines_ccc; line_number++)
  {
    fprintf (writefile, "%s", script_line_pointer_ccc[line_number]);
    //fprintf (stdout, "%s", script_line_pointer_ccc[line_number]);
  }

  fclose(writefile);
  return (0);
}





// function to process the script lines
int process_lines_ccc(void)
{
  int scanline;
  unsigned int line_number;
  char command[MAX_LINE_LENGTH];
  char rest_of_line[MAX_LINE_LENGTH];
  print_hex_ccc = false;
  if_statement_level_ccc = 0;

  bool perform_check = true;
  if (verbose_ccc > 0)
  {
    if (do_indentation_ccc)
    {
      fprintf (stdout, "Performing indentation cleanup...\n");
    }
    else
    {
      fprintf (stdout, "Checking script for errors...\n");
    }
  }
  for (line_number = 0; line_number < total_lines_ccc; line_number++)
  {
    strcpy (command, "");
    strcpy (rest_of_line, "");
    scanline = sscanf(script_line_pointer_ccc[line_number], "%s %[^\n]", command, rest_of_line);

    // if there is a command found
    if (scanline > 0)
    {
      if (verbose_ccc > 1)
      {
        fprintf (stdout, "command%d=%s  arguments=%s\n", line_number-1, command, rest_of_line);
      }
      if (debug_ccc > 1)
      {
        fprintf (debug_file_ccc, "command%d=%s  arguments=%s\n", line_number-1, command, rest_of_line);
      }

      // cleanup indentation
      if (do_indentation_ccc)
      {
        char new_line[MAX_LINE_LENGTH * 4] = "";
        int current_indent = ccc_indent_ccc;
        bool recess = false;
        if ( (strcmp(command, "subroutine") == 0) || (strcmp(command, "SUBROUTINE") == 0) )
        {
          ccc_indent_ccc++;
        }

        else if ( (strcmp(command, "endsubroutine") == 0) || (strcmp(command, "ENDSUBROUTINE") == 0) )
        {
          ccc_indent_ccc--;
          recess = true;
        }

        if ( (strcmp(command, "while") == 0) || (strcmp(command, "WHILE") == 0) )
        {
          ccc_indent_ccc++;
        }

        else if ( (strcmp(command, "done") == 0) || (strcmp(command, "DONE") == 0) )
        {
          ccc_indent_ccc--;
          recess = true;
        }

        else if ( (strcmp(command, "if") == 0) || (strcmp(command, "IF") == 0) )
        {
          ccc_indent_ccc++;
        }

        else if ( (strcmp(command, "endif") == 0) || (strcmp(command, "ENDIF") == 0) )
        {
          ccc_indent_ccc--;
          recess = true;
        }

        else if ( (strcmp(command, "else") == 0) || (strcmp(command, "ELSE") == 0) )
        {
          recess = true;
        }

        else if ( (strcmp(command, "elseif") == 0) || (strcmp(command, "ELSEIF") == 0) )
        {
          recess = true;
        }

        else if ( (strcmp(command, "setbuffer") == 0) || (strcmp(command, "SETBUFFER") == 0) )
        {
          ccc_indent_ccc++;
        }

        else if ( (strcmp(command, "endbuffer") == 0) || (strcmp(command, "ENDBUFFER") == 0) )
        {
          ccc_indent_ccc--;
          recess = true;
        }

        else if ( (strcmp(command, "setscratchpad") == 0) || (strcmp(command, "SETSCRATCHPAD") == 0) )
        {
          ccc_indent_ccc++;
        }

        else if ( (strcmp(command, "endscratchpad") == 0) || (strcmp(command, "ENDSCRATCHPAD") == 0) )
        {
          ccc_indent_ccc--;
          recess = true;
        }

        else
        {
          ccc_indent_ccc = ccc_indent_ccc;
        }

        if (recess)
        {
          current_indent--;
        }

        int i;
        for (i = 0; i < current_indent; i++)
        {
          int n;
          for (n = 0; n < spaces_ccc; n++)
          {
            if (i + n >= MAX_LINE_LENGTH)
            {
              fprintf (stderr, "Indentation exceeded maximum line length on line %d. Exiting without saving...\n", line_number+1);
              cleanup_ccc();
              exit (1);
            }
            strcat (new_line, " ");
          }
        }
        if (strlen(command) + strlen(rest_of_line) + strlen(new_line) +2 >= MAX_LINE_LENGTH)
        {
          fprintf (stderr, "Line %d is too long with new indentation. Exiting without saving...\n", line_number+1);
          cleanup_ccc();
          exit (1);
        }
        strcat (new_line, command);
        if (strcmp(rest_of_line, "") != 0)
        {
          strcat (new_line, " ");
          strcat (new_line, rest_of_line);
        }
        strcat (new_line, "\n");
        strcpy (script_line_pointer_ccc[line_number], new_line);
        //fprintf (stdout, "%s", new_line);
      }

      // perform sanity checks on the script
      else
      {
        return_value_ccc = execute_line_ccc(perform_check, line_number, command, rest_of_line);
        if (return_value_ccc < 0 && return_value_ccc > -100)
        {
          if (return_value_ccc == -99)
          {
            fprintf (stdout, "Command not supported by current license type. Exiting...\n");
            cleanup_ccc();
            exit (1);
          }
          else
          {
            fprintf (stdout, "%s", script_line_pointer_ccc[line_number]);
            fprintf (stdout, "There was an error processing the script \'%s\'. Exiting...\n", script_file_ccc);
            cleanup_ccc();
            exit (1);
          }
        }
      }
    }
  }

  if (!do_indentation_ccc && if_statement_level_ccc != 0)
  {
    fprintf (stderr, "\nError: IF count does not match ENDIF count (too many IF),\n");
    fprintf (stdout, "There was an error processing the script \'%s\'. Exiting...\n", script_file_ccc);
    cleanup_ccc();
    exit (1);
  }
  if (!do_indentation_ccc && while_statement_level_ccc != 0)
  {
    fprintf (stderr, "\nError: WHILE count does not match DONE count (too many WHILE),\n");
    fprintf (stdout, "There was an error processing the script \'%s\'. Exiting...\n", script_file_ccc);
    cleanup_ccc();
    exit (1);
  }

  // if it passed the sanity checks and not just checking or doing indentation then run the script
  if (!check_only_ccc)
  {
    if (verbose_ccc > 0)
    {
      fprintf (stdout, "Running script...\n");
    }
    // reset the starting main buffer size in case it was changed during checking
    ccc_main_buffer_size_ccc = 512;
    return_value_ccc = set_main_buffer_ccc();
    if (return_value_ccc != 0)
    {
      return (-1);
    }
    // reset the starting scratchpad size in case it was changed during checking
    ccc_main_scratchpad_size_ccc = 512;
    return_value_ccc = set_main_scratchpad_ccc();
    if (return_value_ccc != 0)
    {
      return (-1);
    }

    // open the disk for IO
    if (disk_1_ccc != NULL && !direct_mode_ccc)
    {
      disk1_fd_ccc = open (disk_1_ccc, O_RDWR | O_NONBLOCK);
      if (disk1_fd_ccc == -1)
      {
        fprintf(stderr, "Error: Unable to open %s (%s).\n", disk_1_ccc, strerror(errno));
        return (-1);
      }
    }

    current_disk_ccc = disk1_fd_ccc;

    // clear/initialize the passthrough structure
    memset(&passthrough_ccc, 0, sizeof(passthrough_ccc));

    perform_check = false;
    for (line_number = 0; line_number < total_lines_ccc; line_number++)
    {
      strcpy (command, "");
      strcpy (rest_of_line, "");
      scanline = sscanf(script_line_pointer_ccc[line_number], "%s %[^\n]", command, rest_of_line);

      // if there is a command found
      if (scanline > 0)
      {
        if (verbose_ccc > 1)
        {
          fprintf (stdout, "command%d= %s\n", line_number-1, command);
        }
        if (debug_ccc > 1)
        {
          fprintf (debug_file_ccc, "command%d= %s\n", line_number-1, command);
        }

        return_value_ccc = execute_line_ccc(perform_check, line_number, command, rest_of_line);
        if (return_value_ccc < 0)
        {
          // check if script hit a loadscript command
          if (return_value_ccc == -200)
          {
            // return 1 so it will load the next script
            return (1);
          }
          // check if script tried to load previous script when there wasn't one
          if (return_value_ccc == -102)
          {
	    if (!quiet_ccc)
            {
              fprintf (stdout, "Script reached PREVIOUSSCRIPT command at line %d, exiting normally...\n", line_number-1);
	    }
            return (0);
          }
          // check if script hit an exit or end command
          if (return_value_ccc == -101)
          {
	    if (!quiet_ccc)
            {
              fprintf (stdout, "Script reached EXIT command at line %d, exiting normally...\n", line_number-1);
	    }
            return (0);
          }
          else if (return_value_ccc == -100)
          {
	    if (!quiet_ccc)
            {
              fprintf (stdout, "Script reached END command at line %d, exiting normally...\n", line_number-1);
	    }
            return (0);
          }
          else if (return_value_ccc == -99)
          {
            fprintf (stdout, "Command not supported by current license type. Exiting...\n");
            cleanup_ccc();
            exit (1);
          }
          else
          {
            fprintf (stdout, "%s", script_line_pointer_ccc[line_number]);
            fprintf (stdout, "There was an error processing the script \'%s\'. Exiting...\n", script_file_ccc);
            cleanup_ccc();
            exit (1);
          }
        }

        // if return is positive then it means to jump to a new line number
        if (return_value_ccc > 0)
        {
          line_number = return_value_ccc;
        }
      }
    }
  }
  if (!do_indentation_ccc)
  {
    fprintf (stdout, "End of script reached, exiting normally...\n");
  }
  return (0);
}





// function to close and reopen the disk
int disk_reopen_ccc(void)
{
  if (current_disk_ccc == disk1_fd_ccc && disk_1_ccc != NULL && !direct_mode_ccc)
  {
    close (disk1_fd_ccc);
    disk1_fd_ccc = open (disk_1_ccc, O_RDWR | O_NONBLOCK);
    if (disk1_fd_ccc == -1)
    {
      fprintf(stderr, "Error: Unable to open %s (%s).\n", disk_1_ccc, strerror(errno));
      return (-1);
    }
    current_disk_ccc = disk1_fd_ccc;
  }
  return (0);
}





// function to process commands from a command line
int process_command_line_ccc(void)
{
  int scanline;
  unsigned int line_number = 0;
  total_lines_ccc = 0;
  char command[MAX_LINE_LENGTH];
  char rest_of_line[MAX_LINE_LENGTH];
  print_hex_ccc = false;
  fprintf (stdout, "\n");

  // open the disk for IO
  if (disk_1_ccc != NULL && !direct_mode_ccc)
  {
    disk1_fd_ccc = open (disk_1_ccc, O_RDWR | O_NONBLOCK);
    if (disk1_fd_ccc == -1)
    {
      fprintf(stderr, "Error: Unable to open %s (%s).\n", disk_1_ccc, strerror(errno));
      return (-1);
    }
  }

  current_disk_ccc = disk1_fd_ccc;

  // clear/initialize the passthrough structure
  memset(&passthrough_ccc, 0, sizeof(passthrough_ccc));
  bool perform_check = false;
  bool keep_running = true;
  while (keep_running)
  {
    return_value_ccc = 0;
    fprintf (stdout, "> ");
    char input_text[MAX_LINE_LENGTH];
    fflush(stdout);
    if ( fgets(input_text, sizeof input_text, stdin) != NULL )
    {
      strcpy (command, "");
      strcpy (rest_of_line, "");
      scanline = sscanf(input_text, "%s %[^\n]", command, rest_of_line);
      if (scanline > 0)
      {
        return_value_ccc = execute_line_ccc(perform_check, line_number, command, rest_of_line);
      }
    }


    if (return_value_ccc == -101 || return_value_ccc == -100)
    {
      break;
    }
  }
  fprintf (stdout, "\n");
  return (0);
}





// function to check if variable already exists and what type it is
// returns 0 if not found, 1 for number, 2 for string
// returns -1 if found more than one instance of the variable name
int check_variable_ccc(char *var_name)
{
  //fprintf (stdout, "var_name= %s\n", var_name);
  unsigned int i;
  int n = 0;
  bool number = false;
  bool string = false;
  // check number variables
  for (i = 0; i < total_number_variables_ccc; i++)
  {
    if (strcmp(var_name, number_variable_name_pointer_ccc[i]) == 0)
    {
      number = true;
      n++;
    }
  }

  // check string variables
  for (i = 0; i < total_string_variables_ccc; i++)
  {
    if (strcmp(var_name, string_variable_name_pointer_ccc[i]) == 0)
    {
      string = true;
      n++;
    }
  }

  if (n > 1)
  {
    return (-1);
  }

  else if (number)
  {
    return (1);
  }

  else if (string)
  {
    return (2);
  }

  return (0);
}





// function to check if variable was set on command line
// returns 0 if not found, 1 if found
int check_arguments_ccc(char *var_name)
{
  //fprintf (stdout, "var_name= %s\n", var_name);
  unsigned int i;

  int found_arg = 0;

  // check if it is a command line argument
  for (i = 0; i < total_string_variables_ccc; i++)
  {
    unsigned int c;
    for (c = 0; c < argument_count_ccc; c++)
    {
      if (verbose_ccc > 0)
      {
        fprintf (stdout, "argument%d= %s\n", c+1, argument_ccc[c]);
      }
      if (debug_ccc > 0)
      {
        fprintf (debug_file_ccc, "argument%d= %s\n", c+1, argument_ccc[c]);
      }
      char current_argument[MAX_VARIABLE_LENGTH];
      char name[MAX_VARIABLE_NAME_LENGTH];
      strcpy (current_argument, argument_ccc[c]);
      int length = strlen(current_argument);
      int i;
      for (i = 0; i < length; i++)
      {
        if (current_argument[i] == '=')
        {
          strncpy (name, current_argument, i);
          name[i] = '\0';
          //fprintf (stdout, "var_name= %s\n", var_name);
          if (strcmp(var_name, name) == 0)
          {
            found_arg = 1;
          }
          break;
        }
      }

    }
  }
  return (found_arg);
}




// function to create a number variable
// returns 0 if successful, otherwise returns the value from check_variable_ccc
int create_number_variable_ccc(char *var_name)
{
  return_value_ccc = check_variable_ccc(var_name);
  if (return_value_ccc == 0)
  {
    if (total_number_variables_ccc >= number_variable_rows_ccc)
    {
      int ret = increase_number_variable_memory_ccc();
      if (ret != 0)
      {
        return (-1);
      }
    }
    strcpy(number_variable_name_pointer_ccc[total_number_variables_ccc++], var_name);
    int var_num = find_number_variable_ccc(var_name);
    set_number_variable_ccc(var_num, "0");
  }

  else
  {
    return (return_value_ccc);
  }

  return (0);
}





// function to create a string variable
// returns 0 if successful, otherwise returns the value from check_variable_ccc
int create_string_variable_ccc(char *var_name)
{
  return_value_ccc = check_variable_ccc(var_name);
  if (return_value_ccc == 0)
  {
    if (total_string_variables_ccc >= string_variable_rows_ccc)
    {
      int ret = increase_string_variable_memory_ccc();
      if (ret != 0)
      {
        return (-1);
      }
    }
    strcpy(string_variable_name_pointer_ccc[total_string_variables_ccc++], var_name);
    int var_num = find_string_variable_ccc(var_name);
    set_string_variable_ccc(var_num, "");
  }

  else
  {
    return (return_value_ccc);
  }

  return (0);
}





// function to find a number variable
int find_number_variable_ccc(char *var_name)
{
  unsigned int i;
  for (i = 0; i < total_number_variables_ccc; i++)
  {
    if (strcmp(var_name, number_variable_name_pointer_ccc[i]) == 0)
    {
      return (i);
    }
  }
  return (-1);
}


// function to find a string variable
int find_string_variable_ccc(char *var_name)
{
  unsigned int i;
  for (i = 0; i < total_string_variables_ccc; i++)
  {
    if (strcmp(var_name, string_variable_name_pointer_ccc[i]) == 0)
    {
      return (i);
    }
  }
  return (-1);
}





// function to set a number variable
int set_number_variable_ccc(int var_num, char *raw_value)
{
  long long value = math_ccc(raw_value);

  int err_num = find_number_variable_ccc("error_level");
  if (err_num < 0)
  {
    fprintf (stderr, "\nError: Unable to locate internal variable \'error_level\',\n");
    return (-1);
  }
  number_variable_buffer_ccc[err_num] = error_level_ccc;

  if (error_level_ccc != 0)
  {
    if (error_level_ccc == 1)
    {
      return (-1);
    }
  }

  number_variable_buffer_ccc[var_num] = value;

  return (0);
}






// function to set a string variable
int set_string_variable_ccc(int var_num, char *raw_value)
{
  strcpy (string_variable_pointer_ccc[var_num], raw_value);

  //fprintf (stdout, "string_value= %s\n", string_variable_pointer_ccc[var_num]);

  return (0);
}






// function to find the next matching command and return the line number
// returns -1 if not found
int find_command3_ccc(char *search_command, char *search_command_cap, unsigned int start_line)
{
  int scanline;
  unsigned int line_number;
  char command[MAX_LINE_LENGTH];
  for (line_number = start_line; line_number < total_lines_ccc; line_number++)
  {
    scanline = sscanf(script_line_pointer_ccc[line_number], "%s[^\n]", command);

    // if there is a command found check to see if it matches
    if (scanline == 1)
    {
      //fprintf (stdout, "command= %s\n", command);
      if (strcmp(search_command, command) == 0 || strcmp(search_command_cap, command) == 0)
      {
        return (line_number);
      }
    }
  }
  return (-1);
}







// function to find the next matching command plus 1 item and return the line number
// returns -1 if not found
int find_command_plus1_ccc(char *search_command, char *search_command_cap, char *first_item, unsigned int start_line)
{
  int scanline;
  unsigned int line_number;
  char command[MAX_LINE_LENGTH];
  char first[MAX_LINE_LENGTH];

  for (line_number = start_line; line_number < total_lines_ccc; line_number++)
  {
    scanline = sscanf(script_line_pointer_ccc[line_number], "%s %s[^\n]", command, first);

    // if there is a command found check to see if it matches
    if (scanline == 2)
    {
      //fprintf (stdout, "command= %s\n", command);
      if (strcmp(search_command, command) == 0 || strcmp(search_command_cap, command) == 0)
      {
        if (strcmp(first_item, first) == 0)
        {
          return (line_number);
        }
      }
    }
  }

  return (-1);
}


// function to return a full line
char *get_full_line_ccc(unsigned int line_number, char* max_line_buffer)
{
  sscanf(script_line_pointer_ccc[line_number], "%[^\n]", max_line_buffer);
  return max_line_buffer;
}


// function to process comparison statement
// returns -1 if error, 0 if false, and 1 if true
int compare_ccc(bool perform_check, char *rest_of_line)
{
  int length = strlen(rest_of_line);
  char raw_variable1[length+1];
  char condition[length+1];
  char raw_variable2[length+1];
  char leftover[length+1];
  int scanline = sscanf(rest_of_line, "%s %s %s %[^\n]", raw_variable1, condition, raw_variable2, leftover);
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
    return (-1);
  }

  // set the operator, 0 not set, 1 equal, 2 not equal, 3 greater than, 4 less than, 5 greater or equal, 6 less or equal
  int operator = 0;
  if (strcmp(condition, "=") == 0)
  {
    operator = 1;
  }
  else if (strcmp(condition, "!=") == 0)
  {
    operator = 2;
  }
  else if (strcmp(condition, ">") == 0)
  {
    operator = 3;
  }
  else if (strcmp(condition, "<") == 0)
  {
    operator = 4;
  }
  else if (strcmp(condition, ">=") == 0)
  {
    operator = 5;
  }
  else if (strcmp(condition, "<=") == 0)
  {
    operator = 6;
  }
  else
  {
    fprintf (stderr, "\nError: Unknown condition operator \'%s\',\n", condition);
    return (-1);
  }

  bool string_check = false;
  int variable1_type = 0;
  char *string_variable1;
  char *string_variable2;
  long long variable1;
  long long variable2;
  if (raw_variable1[0] == '$')
  {
    int var_type = check_variable_ccc(raw_variable1+1);
    variable1_type = var_type;
    if (var_type == 0)
    {
      fprintf (stderr, "\nError: Variable \'%s\' not found,\n", raw_variable1);
      return (-1);
    }
    else if (var_type == 1)
    {
      int var_num = find_number_variable_ccc(raw_variable1+1);
      if (var_num < 0)
      {
        fprintf (stderr, "\nError: Variable \'%s\' not found,\n", raw_variable1);
        return (-1);
      }
      variable1 = number_variable_buffer_ccc[var_num];
    }
    else if (var_type == 2)
    {
      string_check = true;
      int var_num = find_string_variable_ccc(raw_variable1+1);
      if (var_num < 0)
      {
        fprintf (stderr, "\nError: Variable \'%s\' not found,\n", raw_variable1);
        return (-1);
      }
      string_variable1 = get_string_variable_value_ccc(raw_variable1);
    }
    else
    {
      fprintf (stderr, "\nError: Variable \'%s\' is wrong type,\n", raw_variable1);
      return (-1);
    }
  }
  else
  {
    char* endptr;
    variable1 = strtoll(raw_variable1, &endptr, 0);
    if (*endptr)
    {
      string_variable1 = raw_variable1;
      string_check = true;
    }
  }

  if (raw_variable2[0] == '$')
  {
    int var_type = check_variable_ccc(raw_variable2+1);
    if (var_type == 0)
    {
      fprintf (stderr, "\nError: Variable \'%s\' not found,\n", raw_variable2);
      return (-1);
    }
    else if (var_type == 1 && !string_check)
    {
      int var_num = find_number_variable_ccc(raw_variable2+1);
      if (var_num < 0)
      {
        fprintf (stderr, "\nError: Variable \'%s\' not found,\n", raw_variable2);
        return (-1);
      }
      variable2 = number_variable_buffer_ccc[var_num];
    }
    else if (var_type == 2 && string_check)
    {
      string_check = true;
      int var_num = find_string_variable_ccc(raw_variable2+1);
      if (var_num < 0)
      {
        fprintf (stderr, "\nError: Variable \'%s\' not found,\n", raw_variable2);
        return (-1);
      }
      string_variable2 = get_string_variable_value_ccc(raw_variable2);
    }
    else
    {
      fprintf (stderr, "\nError: Variable \'%s\' is wrong or different type,\n", raw_variable2);
      return (-1);
    }
  }
  else
  {
    if (!string_check)
    {
      char* endptr;
      variable2 = strtoll(raw_variable2, &endptr, 0);
      if (*endptr)
      {
        fprintf (stderr, "\nError: Variable \'%s\' is wrong or different type,\n", raw_variable2);
        return (-1);
      }
    }
    else
    {
      string_variable2 = raw_variable2;
      if (variable1_type != 2)
      {
        fprintf (stderr, "\nError: Variable \'%s\' is wrong or different type,\n", raw_variable2);
        return (-1);
      }
    }
  }

  if (string_check && (operator != 1 && operator != 2))
  {
    fprintf (stderr, "\nError: Improper operator '%s' for comparing strings,\n", condition);
    fprintf (stderr, "Only '=' or '!=' are allowed when comparing strings,\n");
    return (-1);
  }

  if (!perform_check)
  {
    bool statement = false;
    if (string_check && operator == 1)
    {
      //fprintf (stdout, "string1=%s  string2=%s\n", string_variable1, string_variable2);
      if (strcmp(string_variable1, string_variable2) == 0)
      {
        statement = true;
      }
    }
    else if (string_check && operator == 2)
    {
      //fprintf (stdout, "string1=%s  string2=%s\n", string_variable1, string_variable2);
      if (strcmp(string_variable1, string_variable2) != 0)
      {
        statement = true;
      }
    }
    else
    {
      switch (operator)
      {
        case 1:
          if (variable1 == variable2)
          {
            statement = true;
          }
          break;

        case 2:
          if (variable1 != variable2)
          {
            statement = true;
          }
          break;

        case 3:
          if (variable1 > variable2)
          {
            statement = true;
          }
          break;

        case 4:
          if (variable1 < variable2)
          {
            statement = true;
          }
          break;

        case 5:
          if (variable1 >= variable2)
          {
            statement = true;
          }
          break;

        case 6:
          if (variable1 <= variable2)
          {
            statement = true;
          }
          break;

        default:
          fprintf (stderr, "\nUnknown error processing conditional statement,\n");
          return (-1);
      }
    }
    if (statement)
    {
      return (1);
    }
  }
  return (0);
}





long long math_ccc(char *raw_value)
{
  error_level_ccc = 0;
  long long value;
  long long value1;
  long long value2;
  char part1[MAX_LINE_LENGTH];
  char part2[MAX_LINE_LENGTH];
  char part3[MAX_LINE_LENGTH];
  char extra[MAX_LINE_LENGTH];
  char* endptr;
  int scanline;

  scanline = sscanf(raw_value, "%s %s %s %[^\n]", part1, part2, part3, extra);

  // if only one word then convert to number
  if (scanline == 1)
  {
    if (part1[0] == '$')
    {
      char var_name[MAX_VARIABLE_NAME_LENGTH];
      sscanf(part1+1, "%s", var_name);
      return_value_ccc = check_variable_ccc(var_name);
      if (return_value_ccc < 1)
      {
        fprintf (stderr, "\nError: Variable \'%s\' not found,\n", part1);
        error_level_ccc = 1;
        return (0);
      }
      else if (return_value_ccc != 1)
      {
        fprintf (stderr, "\nError: Variable \'%s\' is wrong type (expecting number variable),\n", part1);
        error_level_ccc = 1;
        return (0);
      }
      else
      {
        int var_num = find_number_variable_ccc(var_name);
        value = number_variable_buffer_ccc[var_num];
        return (value);
      }

    }
    else
    {
      value = strtoull(part1, &endptr, 0);
      if (*endptr)
      {
        fprintf (stderr, "Error performing math on \'%s\'\n", part1);
        error_level_ccc = 1;
        return (0);
      }
      return (value);
    }
  }

  // if two words then expect a string variable followed by a value
  if (scanline == 2)
  {
    if (part1[0] == '$')
    {
      char var_name[MAX_VARIABLE_NAME_LENGTH];
      sscanf(part1+1, "%s", var_name);
      return_value_ccc = check_variable_ccc(var_name);
      if (return_value_ccc < 1)
      {
        fprintf (stderr, "\nError: Variable \'%s\' not found,\n", part1);
        error_level_ccc = 1;
        return (0);
      }
      else if (return_value_ccc != 2)
      {
        fprintf (stderr, "\nError: Variable \'%s\' is wrong type (expecting string variable),\n", part1);
        error_level_ccc = 1;
        return (0);
      }
      else
      {
        int var_num = find_string_variable_ccc(var_name);
        char temp_var[MAX_LINE_LENGTH];
        strcpy(temp_var, string_variable_pointer_ccc[var_num]);
        //fprintf (stdout, "temp_var=%s\n", temp_var);
        int string_count = 0;
        if (part2[0] == '$')
        {
          char var_name[MAX_VARIABLE_NAME_LENGTH];
          sscanf(part2+1, "%s", var_name);
          return_value_ccc = check_variable_ccc(var_name);
          if (return_value_ccc < 1)
          {
            fprintf (stderr, "\nError: Variable \'%s\' not found,\n", part2);
            error_level_ccc = 1;
            return (0);
          }
          else if (return_value_ccc != 1)
          {
            fprintf (stderr, "\nError: Variable \'%s\' is wrong type (expecting number variable),\n", part2);
            error_level_ccc = 1;
            return (0);
          }
          else
          {
            int var_num = find_number_variable_ccc(var_name);
            string_count = number_variable_buffer_ccc[var_num];
          }
        }
        else
        {
          string_count = strtoll(part2, &endptr, 0);
          if (*endptr)
          {
           fprintf (stderr, "\nError extracing number from \'%s\'\n", part2);
            error_level_ccc = 1;
            return (0);
          }
        }

        if (string_count > MAX_LINE_LENGTH / 2)
        {
          fprintf (stderr, "\nError: string count \'%s\' too high (max=%d)\n", part2, MAX_LINE_LENGTH / 2);
          error_level_ccc = 1;
          return (0);
        }

        char string_result[MAX_LINE_LENGTH];
        int i;
        for (i = 0; i < string_count+1; i++)
        {
          char leftover[MAX_LINE_LENGTH];
          int scanline = sscanf(temp_var, "%s %[^\n]", string_result, leftover);
          //fprintf (stdout, "string_result=%s scanline=%d\n", string_result, scanline);
          if (scanline < 2 && i+1 == string_count)
          {
            if (verbose_ccc > 1)
            {
              fprintf (stderr, "Error: No string found at count \'%s\'\n", part2);
            }
            if (debug_ccc > 1)
            {
              fprintf (debug_file_ccc, "Error: No string found at count \'%s\'\n", part2);
            }
            error_level_ccc = 2;
            return (0);
          }
          strcpy(temp_var, leftover);
        }

        if (print_hex_ccc)
        {
          value = strtoull(string_result, &endptr, 16);
        }
        else
        {
          value = strtoull(string_result, &endptr, 0);
        }
        if (*endptr)
        {
          if (verbose_ccc > 0)
          {
            fprintf (stderr, "Error processing value \'%s\' from \'%s\'\n",string_result, part1);
          }
          if (debug_ccc > 0)
          {
            fprintf (debug_file_ccc, "Error processing value \'%s\' from \'%s\'\n",string_result, part1);
          }
          error_level_ccc = 2;
          return (0);
        }

        return (value);
      }

    }
    else
    {
      fprintf (stderr, "Error performing math on \'%s\'\n", part1);
      error_level_ccc = 1;
      return (0);
    }
  }

  // if 3 words then perform simple math
  if (scanline == 3)
  {
    if (part1[0] == '$')
    {
      char var_name[MAX_VARIABLE_NAME_LENGTH];
      sscanf(part1+1, "%s", var_name);
      return_value_ccc = check_variable_ccc(var_name);
      if (return_value_ccc < 1)
      {
        fprintf (stderr, "\nError: Variable \'%s\' not found,\n", part1);
        error_level_ccc = 1;
        return (0);
      }
      else if (return_value_ccc != 1)
      {
        fprintf (stderr, "\nError: Variable \'%s\' is wrong type (expecting number variable),\n", part1);
        error_level_ccc = 1;
        return (0);
      }
      else
      {
        int var_num = find_number_variable_ccc(var_name);
        value1 = number_variable_buffer_ccc[var_num];
      }

    }
    else
    {
      value1 = strtoull(part1, &endptr, 0);
      if (*endptr)
      {
        fprintf (stderr, "Error performing math on \'%s\'\n", part1);
        error_level_ccc = 1;
        return (0);
      }
    }

    if (part3[0] == '$')
    {
      char var_name[MAX_VARIABLE_NAME_LENGTH];
      sscanf(part3+1, "%s", var_name);
      return_value_ccc = check_variable_ccc(var_name);
      if (return_value_ccc < 1)
      {
        fprintf (stderr, "\nError: Variable \'%s\' not found,\n", part3);
        error_level_ccc = 1;
        return (0);
      }
      else if (return_value_ccc != 1)
      {
        fprintf (stderr, "\nError: Variable \'%s\' is wrong type (expecting number variable),\n", part3);
        error_level_ccc = 1;
        return (0);
      }
      else
      {
        int var_num = find_number_variable_ccc(var_name);
        value2 = number_variable_buffer_ccc[var_num];
      }

    }
    else
    {
      value2 = strtoull(part3, &endptr, 0);
      if (*endptr)
      {
        fprintf (stderr, "Error performing math on \'%s\'\n", part3);
        error_level_ccc = 1;
        return (0);
      }
    }

    if (strcmp(part2, "+") == 0)
    {
      value = value1 + value2;
      return (value);
    }
    else if (strcmp(part2, "-") == 0)
    {
      value = value1 - value2;
      return (value);
    }
    else if (strcmp(part2, "*") == 0)
    {
      value = value1 * value2;
      return (value);
    }
    else if (strcmp(part2, "/") == 0)
    {
      value = value1 / value2;
      return (value);
    }
    else if (strcmp(part2, "%") == 0)
    {
      value = value1 % value2;
      return (value);
    }
    else if (strcmp(part2, "&") == 0)
    {
      value = value1 & value2;
      return (value);
    }
    else if (strcmp(part2, "|") == 0)
    {
      value = value1 | value2;
      return (value);
    }
    else if (strcmp(part2, "^") == 0)
    {
      value = value1 ^ value2;
      return (value);
    }
    else if (strcmp(part2, "<") == 0)
    {
      value = value1 << value2;
      return (value);
    }
    else if (strcmp(part2, ">") == 0)
    {
      value = value1 >> value2;
      return (value);
    }
    else
    {
      fprintf (stderr, "Error: Unknown operator \'%s\'\n", part2);
      error_level_ccc = 1;
      return (0);
    }
  }


  // if nothing was processed then set error and return
  error_level_ccc = 1;
  return (0);
}





// function to find number variable and return the value
// also puts a value in global return_value_ccc indicating success or failure
long long get_number_variable_value_ccc(char *raw_variable)
{
  int length = strlen(raw_variable);
  char var_name[length];
  int scanline = sscanf(raw_variable+1, "%s [^\n]", var_name);
  if (scanline != 1)
  {
    fprintf (stderr, "\nError processing variable \'%s\',\n", raw_variable);
    return_value_ccc = -1;
    return (-1);
  }
  return_value_ccc = check_variable_ccc(var_name);
  if (return_value_ccc < 1)
  {
    fprintf (stderr, "\nError: Variable \'%s\' not found,\n", var_name);
    return_value_ccc = -1;
    return (-1);
  }
  else if (return_value_ccc == 1)
  {
    int var_num = find_number_variable_ccc(var_name);
    return_value_ccc = 0;
    return (number_variable_buffer_ccc[var_num]);
  }
  else
  {
    fprintf (stderr, "\nError: Variable \'%s\' is wrong type,\n", var_name);
    return_value_ccc = -1;
    return (-1);
  }
  // we should never make it this far down unless stupid programming error
  return_value_ccc = -1;
  return (-1);
}







// function to find string variable and return the value
// also puts a value in global return_value_ccc indicating success or failure
char *get_string_variable_value_ccc(char *raw_variable)
{
  int length = strlen(raw_variable);
  char var_name[length];
  int scanline = sscanf(raw_variable+1, "%s [^\n]", var_name);
  if (scanline != 1)
  {
    fprintf (stderr, "\nError processing variable \'%s\',\n", raw_variable);
    return_value_ccc = -1;
    return ("");
  }
  return_value_ccc = check_variable_ccc(var_name);
  if (return_value_ccc < 1)
  {
    fprintf (stderr, "\nError: Variable \'%s\' not found,\n", var_name);
    return_value_ccc = -1;
    return ("");
  }
  else if (return_value_ccc == 2)
  {
    int var_num = find_string_variable_ccc(var_name);
    return_value_ccc = 0;
    return (string_variable_pointer_ccc[var_num]);
  }
  else
  {
    fprintf (stderr, "\nError: Variable \'%s\' is wrong type,\n", var_name);
    return_value_ccc = -1;
    return ("");
  }
  // we should never make it this far down unless stupid programming error
  return_value_ccc = -1;
  return ("");
}







// function to set a number variable from a long long number
// remember that it assumes the first character of var_name is $
int set_number_variable_value_ccc(char *raw_variable, long long value)
{
  int length = strlen(raw_variable);
  char var_name[length];
  char temp_var[32];
  int scanline = sscanf(raw_variable+1, "%s [^\n]", var_name);
  if (scanline != 1)
  {
    fprintf (stderr, "\nError processing variable \'%s\',\n", raw_variable);
    return_value_ccc = -1;
    return (-1);
  }
  return_value_ccc = check_variable_ccc(var_name);
  if (return_value_ccc < 1)
  {
    fprintf (stderr, "\nError: Variable \'%s\' not found,\n", var_name);
    return_value_ccc = -1;
    return (-1);
  }
  else if (return_value_ccc == 1)
  {
    sprintf(temp_var, "%lld", value);
    int var_num = find_number_variable_ccc(var_name);
    return_value_ccc = set_number_variable_ccc(var_num, temp_var);
    if (return_value_ccc == -1)
    {
      fprintf (stderr, "Error! Variable \'%s\' value \'%s\' was unable to be completely processed,\n", var_name, temp_var);
      return (-1);
    }
    return (0);
  }
  else
  {
    fprintf (stderr, "\nError: Variable \'%s\' is wrong type,\n", var_name);
    return_value_ccc = -1;
    return (-1);
  }
  // we should never make it this far down unless stupid programming error
  return_value_ccc = -1;
  return (-1);
}






// function to set string variable from the raw value
// remember that is assumes the first character of var_name is $
int set_string_variable_value_ccc(char *raw_variable, char *value)
{
  int length = strlen(raw_variable);
  char var_name[length];
  int scanline = sscanf(raw_variable+1, "%s [^\n]", var_name);
  if (scanline != 1)
  {
    fprintf (stderr, "\nError processing variable \'%s\',\n", raw_variable);
    return (-1);
  }
  return_value_ccc = check_variable_ccc(var_name);
  if (return_value_ccc < 1)
  {
    fprintf (stderr, "\nError: Variable \'%s\' not found,\n", var_name);
    return (-1);
  }
  else if (return_value_ccc == 2)
  {
    int var_num = find_string_variable_ccc(var_name);
    set_string_variable_ccc(var_num, value);
    return (0);
  }
  else
  {
    fprintf (stderr, "\nError: Variable \'%s\' is wrong type,\n", var_name);
    return (-1);
  }
  // we should never make it this far down unless stupid programming error
  return (-1);
}





// time function
int get_the_time_ccc(void)
{
  int time_error = 0;
  gettimeofday(&tvCurrent_ccc, NULL);
  long long diff = ((long long)tvCurrent_ccc.tv_usec + (1000000 * (long long)tvCurrent_ccc.tv_sec));
  return_value_ccc = set_number_variable_value_ccc("$time", diff);
  if (diff < 0)
  {
    time_error = -1;
  }

  char tempbuffer[30];
  time_t curtime;
  curtime = tvCurrent_ccc.tv_sec;
  strftime(tempbuffer, 30, "%Y-%m-%d_%H.%M.%S", localtime(&curtime));
  sprintf(current_date_ccc, "%s.%06ld", tempbuffer, tvCurrent_ccc.tv_usec);
  return_value_ccc = set_string_variable_value_ccc("$date", current_date_ccc);
  if (return_value_ccc != 0)
  {
    time_error = -1;
  }

  return (time_error);
}





// return usec since program start
long long get_elapsed_usec_ccc(void)
{
  gettimeofday(&tvCurrent_ccc, NULL);
  long long elapsed_usec = ((long long)tvCurrent_ccc.tv_usec + 1000000 * (long long)tvCurrent_ccc.tv_sec) - ((long long)tvBegin_ccc.tv_usec + 1000000 * (long long)tvBegin_ccc.tv_sec);

  return (elapsed_usec);
}






// rotate funtions
uint8_t rotl8_ccc(uint8_t value, int shift)
{
  return (value << shift) | (value >> (sizeof(value) * 8 - shift));
}
uint8_t rotr8_ccc(uint8_t value, int shift)
{
  return (value >> shift) | (value << (sizeof(value) * 8 - shift));
}


uint16_t rotl16_ccc(uint16_t value, int shift)
{
  return (value << shift) | (value >> (sizeof(value) * 8 - shift));
}
uint16_t rotr16_ccc(uint16_t value, int shift)
{
  return (value >> shift) | (value << (sizeof(value) * 8 - shift));
}


uint32_t rotl32_ccc(uint32_t value, int shift)
{
  return (value << shift) | (value >> (sizeof(value) * 8 - shift));
}
uint32_t rotr32_ccc(uint32_t value, int shift)
{
  return (value >> shift) | (value << (sizeof(value) * 8 - shift));
}


uint64_t rotl64_ccc(uint64_t value, int shift)
{
  return (value << shift) | (value >> (sizeof(value) * 8 - shift));
}
uint64_t rotr64_ccc(uint64_t value, int shift)
{
  return (value >> shift) | (value << (sizeof(value) * 8 - shift));
}







// function to get random value
int get_random_value_ccc(int speed)
{
  struct timeval tvstart;
  gettimeofday(&tvstart, NULL);
  //printf("%ld.%06ld\n", tvstart.tv_sec, tvstart.tv_usec);
  int random_value;
  int i;
  for (i = 0; i < speed; i++)
  {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    //printf("%ld.%06ld\n", tv.tv_sec, tv.tv_usec);
    srand( (tv.tv_usec + 1000000 * tv.tv_sec) );
    random_value = rand();
    //fprintf (stdout, "random=%d\n", random_value);
  }
  struct timeval tvend;
  gettimeofday(&tvend, NULL);
  //printf("%ld.%06ld\n", tvend.tv_sec, tvend.tv_usec);

  return (random_value);
}





int do_nanosleep_ccc(unsigned long long time)
{
  // sleep time in nanoseconds
  struct timespec tim, tim2;
  tim.tv_sec = time / 1000000000;
  tim.tv_nsec = time % 1000000000;
  nanosleep(&tim , &tim2);
  return (0);
}





char *trim_white_space_ccc(char *str)
{
  char *end;

  // Trim leading space
  while(isspace(*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}




int open_source_disk_ccc(void)
{
  // do nothing, this is for clone gui
  return 0;
}



int call_command_on_power_cycle_ccc(void)
{
  // do nothing, this is for clone gui
  return 0;
}



void update_display_status_buttons_ccc(int time_ms)
{
  // do nothing, this is for clone gui
  time_ms = time_ms;
}



int print_gui_error_message_ccc(char *message, char *title, int type)
{
  // do nothing, this is for clone gui
  message = message;
  title = title;
  type = type;
  return 0;
}

int set_lun_dialog_ccc (int max_lun)
{
  printf("We have to set the LUN, the maximum value is %d. Which LUN do you want?\n",max_lun);
  printf("This function is only implemented in the GUI. Now we are choosing 0 by default.\n");
  return 0;
}





