// Copyright (C) 2015-2022 Scott Dwyer.
// You may use/distribute/modify this freely, under the terms of
// the GNU General Public License version 2 or later version.
// This software is distributed WITHOUT ANY WARRANTY.


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "common.h"
#include "clone_gui_common.h"
#include "hddsuperclone.h"
#include "hddsuperclone_help.h"
#include "hddsuperclone_driver.h"
#include "hddsupertool_help.h"

#include "strncpy_wrapper.h"

// Function to handle ctrl-c
void signal_callback_handler_ccc(int signum)
{
  (void) signum;
  if (!critical_process_ccc)
  {
    sprintf(tempmessage_ccc, "Terminated by user\n");
    message_exit_ccc(tempmessage_ccc);
    forced_exit_ccc = true;
    exitcode_ccc = GENERAL_ERROR_EXIT_CODE;
    cleanup_ccc();
    exit(GENERAL_ERROR_EXIT_CODE);
  }
  pending_exit_ccc = true;
}



char *tool_title = "HDDSuperTool";
char *tool_version_number = "1.10_20170129";
char *scripts_title = "Scripts";
char *scripts_version_number = "1.9_20190413";

int main (int argc, char **argv)
{
  title_ccc = "HDDSuperClone";
  version_number_ccc = "2.3.3_20220430";
  copyright_year_ccc = "2022";


  // Register ctrl-c signal and signal handler
  signal(SIGINT, signal_callback_handler_ccc);

  //fprintf (stdout, "argument count= %d\n", argc);

  critical_process_ccc = false;
  pending_exit_ccc = false;
  logfile_changed_ccc = false;
  domainfile_changed_ccc = false;


  // set start_time_ccc
  start_time_ccc = time(NULL);


  // set some variables
  check_command_ccc = 0;
  check_read_ccc = 0;
  check_write_ccc = 0;
  append_script_ccc = 0;
  ccc_indent_ccc = 0;
  spaces_ccc = 0;
  argument_count_ccc = 0;
  check_only_ccc = false;
  do_indentation_ccc = false;

  cleanup_mode_ccc = false;
  quiet_ccc = false;
  did_reset_ccc = 0;
  stop_signal_ccc = false;
  translate_ccc = 0;
  running_clone_ccc = 0;
  superclone_ccc = true;
  force_danger_ccc = false;
  force_stupid_ccc = false;
  pio_mode_ccc = false;
  enable_process_chunk_ccc = false;
  enable_read_twice_ccc = false;
  verbose_ccc = 0;
  debug_ccc = 0;
  debugversion_ccc = false;
  verbosedebug_ccc = false;
  command_line_ccc = false;
  check_message_ccc = false;
  repair_log_ccc = false;
  fill_mode_ccc = false;
  driver_only_ccc = false;
  do_domain_ccc = false;
  create_domain_ccc = false;
  domain_dd_ccc = false;
  importdd_ccc = false;
  stop_on_error_ccc = false;
  do_call_command_ccc = false;
  activate_primary_relay_on_error_ccc = false;
  write_buffer_disable_ccc = false;
  write_buffer_enable_ccc = false;
  driver_return_error_ccc = false;
  driver_return_zeros_ccc = false;
  driver_return_marked_ccc = false;
  strcpy (command_to_call_ccc, "");
  check_log_size_ccc = 1;
  check_command_ccc = 0;
  check_read_ccc = 0;
  check_write_ccc = 0;
  append_script_ccc = 0;
  exitcode_ccc = 0;
  pagesize_ccc = sysconf(_SC_PAGESIZE);
  if (pagesize_ccc > 4096)
  {
    sprintf (tempmessage_ccc, "Internal Error, pagesize too great\n");
    message_exit_ccc(tempmessage_ccc);
    exitcode_ccc = GENERAL_ERROR_EXIT_CODE;
    cleanup_ccc();
  }
  extended_support_ccc = false;
  copy_image_mode_ccc = false;
  generic_mode_ccc = false;
  direct_mode_ccc = false;
  ahci_mode_ccc = false;
  usb_mode_ccc = false;
  usb_allow_ata_ccc = true;
  ata_passthrough_ccc = false;
  scsi_passthrough_ccc = true;
  nvme_passthrough_ccc = false;
  auto_passthrough_ccc = false;
  supertool_mode_chosen_ccc = false;
  table_address_changed_ccc = false;
  ahci_address_changed_ccc = false;
  ahci_interrupt_changed_ccc = false;
  memory_mapped_ccc = false;
  memory_failed_ccc = false;
  io_permissions_ccc = false;
  reg_base_address_ccc = 0;
  control_base_address_ccc = 0;
  bus_base_address_ccc = 0;
  hba_base_address_ccc = 0;
  hba_reset_address_ccc = 0;
  port_base_address_ccc = 0;
  port_reset_address_ccc = 0;
  max_dma_size_ccc = pagesize_ccc;
  real_buffer_size_ccc = MAX_BUFFER_SIZE;
  first_read_time_ccc = 0;
  soft_reset_time_ccc = SOFT_RESET_TIMER;
  hard_reset_time_ccc = HARD_RESET_TIMER;
  busy_wait_time_ccc = BUSY_WAIT_TIMER;
  reset_wait_time_ccc = RESET_WAIT_TIMER;
  general_timeout_ccc = GENERAL_TIMER;
  usb_timeout_ccc = USB_TIMER;
  cbw_timeout_ccc = CBW_TIMER;
  csw_timeout_ccc = CSW_TIMER;
  initial_busy_wait_time_ccc = INITIAL_BUSY_WAIT_TIME;
  power_cycle_time_ccc = RESET_WAIT_TIMER;
  always_wait_for_reset_timers_ccc = false;
  dont_wait_for_drdy_ccc = false;
  strcpy (exit_message_ccc, "\0");
  strcpy (display_message_ccc, "\0");
  strcpy (log_display_message_ccc, "\0");
  domain_status_ccc = FINISHED;
  choose_device_mode_ccc = 0;
  source_total_size_ccc = 0;
  bytes_per_sector_ccc = 0;
  target_total_size_ccc = 0;
  generic_source_total_size_ccc = 0;
  disk1_fd_ccc = -1;
  disk2_fd_ccc = -1;
  enable_scsi_write_ccc = false;
  ata_return_valid_ccc = 1;
  phase_timers_ccc = false;
  orig_soft_reset_time_ccc = SOFT_RESET_TIMER;
  p12_soft_reset_time_ccc = P12SOFT_RESET_TIMER;
  p3_soft_reset_time_ccc = P3SOFT_RESET_TIMER;
  p4_soft_reset_time_ccc = P4SOFT_RESET_TIMER;
  td_soft_reset_time_ccc = TDSOFT_RESET_TIMER;
  d2_soft_reset_time_ccc = D2SOFT_RESET_TIMER;
  sc_soft_reset_time_ccc = SCSOFT_RESET_TIMER;
  rt_soft_reset_time_ccc = RTSOFT_RESET_TIMER;
  memory_used_ccc = 0;
  cluster_size_ccc = DEFAULT_CLUSTER_SIZE;
  block_size_ccc = DEFAULT_BLOCK_SIZE;
  rate_skip_ccc = DEFAULT_RATE_SKIP;
  skip_timeout_ccc = DEFAULT_SKIP_TIMEOUT * 1000;
  exit_on_slow_ccc = DEFAULT_EXIT_ON_SLOW;
  data_read_from_log_ccc = false;
  driver_mode_ccc = 0;
  driver_running_ccc = 0;
  driver_memory_mapped_ccc = 0;
  driver_installed_ccc = 0;
  driver_return_zeros_on_error_ccc = 0;
  driver_io_scsi_only_ccc = false;
  no_io_error_ccc = 0;
  aggressive_driver_ccc = true;
  charater_device_driver_mode_ccc = false; // TODO make this an option normally false
  color_statusbar_ccc = false;
  running_analyze_ccc = 0;
  drive_locked_ccc = false;
  smart_supported_ccc = false;
  smart_enabled_ccc = false;
  enable_extended_analyze_ccc = false;
  strcpy(analyze_text_ccc, "");
  strcpy(smart_data_text_ccc, "");
  usbr1_vendor_id_ccc = -1;
  usbr1_product_id_ccc = -1;
  usbd1_vendor_id_ccc = -1;
  usbd1_product_id_ccc = -1;
  usbd1_bulk_in_endpoint_ccc = -1;
  usbd1_bulk_out_endpoint_ccc = -1;
  usb_mode_ccc = false;
  usbr1_dev_handle_ccc = NULL;
  usbd1_dev_handle_ccc = NULL;
  usb_transfer_direction_ccc = USB_DIRECTION_IN;
  usb_lun_ccc = 0;
  usb_tag_ccc = 0;
  usb_transfer_length_ccc = 0;
  usb_bulk_reset_enabled_ccc = 1;
  usb_soft_reset_enabled_ccc = 0;
  usb_hard_reset_enabled_ccc = 0;
  usb_port_reset_enabled_ccc = 1;
  usb_read_residue_ccc = 0;
  primary_relay_activation_time_ccc = PRIMARY_RELAY_ACTIVATION_TIME;
  primary_relay_delay_time_ccc = PRIMARY_RELAY_DELAY_TIME;
  use_rebuild_assist_ccc = false;
  rebuild_assist_enabled_ccc = false;
  use_fpdma_ccc = false;
  wait_for_ds_bit_ccc = false;
  rebuild_assist_data_valid_ccc = false;
  enable_hba_reset_on_connect_source_ccc = false;    // TODO this is for debug testing, turn off for normal release
  enable_hba_test_on_connect_source_ccc = false;    // TODO this is for debug testing, turn off for normal release
  enable_dont_identify_on_choose_source_ccc = false;    // TODO this is for debug testing, turn off for normal release
  enable_data_dump_ccc = false;    // TODO this is for debug testing, turn off for normal release
  enable_dump_identify_on_check_ccc = false;    // TODO this is for debug testing, turn off for normal release

  strcpy (data_dump_filename_ccc, "debug_data_dump.log");

  strcpy (virtual_driver_name_ccc, "sdv");

  // get process id of this program
  process_id_ccc = getpid();

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


  // get command line
  int argcount;
  for (argcount = 0; argcount < argc; argcount++)
  {
    strcat (full_command_line_ccc, argv[argcount]);
    strcat (full_command_line_ccc, " ");
  }
  //fprintf (stdout, "%s\n", full_command_line_ccc);

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
      {"debug",         required_argument,    0, DEBUGOUT},
      {"verbose",       required_argument,    0, VERBOSEOUT},
      {"tool",          no_argument,    0, SUPERTOOL},
      {"translate",     required_argument, 0, TRANSLATE},
      //{"license-make",     required_argument, 0, MAKELICENSE},
      //{"license-type",     required_argument, 0, LICENSETYPE},
      //{"license-time",     required_argument, 0, LICENSETIME},
      //{"license-version",     required_argument, 0, LICENSEVERSION},
      //{"license-process-active",       no_argument,    0, LICENSEPROCESSACTIVE},
      {"quiet",     no_argument,    0, 'Q'},
      {"crash-computer-destroy-data",         no_argument,    0, CRASH},
      {"enable-rebuild-assist",         no_argument,    0, ENABLE_REBUILD_ASSIST},
      {"rebuild-assist-test",     required_argument, 0, REBUILD_ASSIST_TEST},
      {"enable-hba-reset",         no_argument,    0, ENABLE_HBA_RESET},
      {"enable-hba-test",         no_argument,    0, ENABLE_HBA_TEST},
      {"disable-ahci-identify",         no_argument,    0, DISABLE_AHCI_IDENTIFY},
      {"start-serverp",         no_argument,    0, RUN_SERVERP},
      {"command-serverp",         required_argument,    0, COMMAND_SERVERP},
      {"manual-create",         no_argument,    0, MANUAL_CREATE},
      // supertool command line options
      {"check",         no_argument,    0, 'c'},
      {"command",       no_argument,    0, 'C'},
      {"direct",        no_argument,    0, 'd'},
      {"ahci",          no_argument,    0, 'a'},
      {"ata",           no_argument,    0, ATAMODE},
      {"scsi",          no_argument,    0, SCSIMODE},
      {"file",          required_argument, 0, 'f'},
      {"indent",        required_argument, 0, 'i'},
      {"target",        required_argument, 0, 't'},
      {"testskipmb",      required_argument, 0, TESTSKIPMB},
      {"testskip",      required_argument, 0, TESTSKIP},
      {"testskipfast",      required_argument, 0, TESTSKIPFAST},
      {0, 0, 0, 0}
    };
    // getopt_long stores the option index here.
    int option_index = 0;

    command_line_argument = getopt_long (argc, argv, "hvQcCdaf:i:t:",
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

      case DEBUGOUT:
        debug_ccc = strtoull(optarg, NULL, 16);
        break;

      case VERBOSEOUT:
        verbose_ccc = strtoull(optarg, NULL, 16);
        break;

      case TRANSLATE:
        translate_ccc = strtoull(optarg, NULL, 0);
        break;

      case LICENSETYPE:
        break;

      case LICENSEVERSION:
        break;

      case LICENSETIME:
        break;

      case MAKELICENSE:
        exit(0);

      case LICENSEPROCESSACTIVE:
        exit(0);

      case 'Q':
        quiet_ccc = true;
        break;

      case CRASH:
        force_stupid_ccc = true;
        force_danger_ccc = true;
        break;

      case ENABLE_REBUILD_ASSIST:
        rebuild_assist_enabled_ccc = true;
        break;

      case REBUILD_ASSIST_TEST:
        rebuild_assist_test_ccc = strtoull(optarg, NULL, 0);
        break;

      case ENABLE_HBA_RESET:
        enable_hba_reset_on_connect_source_ccc = true;
        break;

      case ENABLE_HBA_TEST:
        enable_hba_test_on_connect_source_ccc = true;
        enable_dont_identify_on_choose_source_ccc = true;
        break;

      case DISABLE_AHCI_IDENTIFY:
        enable_dont_identify_on_choose_source_ccc = true;
        break;

      case RUN_SERVERP:
        exit(0);

      case COMMAND_SERVERP:
        exit(0);

      case MANUAL_CREATE:
        exit(0);

      case SUPERTOOL:
        superclone_ccc = false;
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

      case 'd':
        direct_mode_ccc = true;
        supertool_mode_chosen_ccc = true;
        break;

      case 'a':
        direct_mode_ccc = true;
        ahci_mode_ccc = true;
        supertool_mode_chosen_ccc = true;
        break;

      case ATAMODE:
        ata_passthrough_ccc = true;
        scsi_passthrough_ccc = false;
        supertool_mode_chosen_ccc = true;
        break;

      case SCSIMODE:
        ata_passthrough_ccc = false;
        scsi_passthrough_ccc = true;
        supertool_mode_chosen_ccc = true;
        break;

      case 't':
        disk_1_ccc = optarg;
        supertool_mode_chosen_ccc = true;
        break;

      case 'f':
        script_file_ccc = optarg;
        break;

      case TESTSKIPMB:
        test_skip_mb_ccc = strtoul(optarg, NULL, 0);
        break;

      case TESTSKIP:
        test_skip_ccc(strtoul(optarg, NULL, 0));
        exit(0);

      case TESTSKIPFAST:
        skip_fast_ccc = true;
        test_skip_ccc(strtoul(optarg, NULL, 0));
        exit(0);

      case '?':
        // getopt_long already printed an error message.
        command_line_error = true;
        break;

      default:
        sprintf (tempmessage_ccc, "Unknown error processing command line options\n");
        message_exit_ccc(tempmessage_ccc);
        command_line_error = true;
    }
  }

  //debug_ccc = debug_ccc | DEBUG39;    //debug TODO this is for hba port testing purposes only
  //verbose_ccc = debug_ccc | DEBUG39;    //debug TODO this is for hba port testing purposes only

  if (debug_ccc || verbose_ccc)
  {
    verbosedebug_ccc = true;
  }

  //check if debug is turned on and open debug file if it is
  if (debug_ccc)
  {
    debug_file_ccc = fopen(debugfile_ccc, "w");
    if (debug_file_ccc == NULL)
    {
      sprintf (tempmessage_ccc, "Cannot open %s for writing (%s).\nAborting...\n", debugfile_ccc, strerror(errno));
      message_exit_ccc(tempmessage_ccc);
      exitcode_ccc = GENERAL_ERROR_EXIT_CODE;
      cleanup_ccc();
    }
    fprintf (debug_file_ccc, "%s debug file created at ", title_ccc);
    time_t mytime;
    mytime = time(NULL);
    fprintf (debug_file_ccc, "%s", ctime(&mytime));
    fprintf (debug_file_ccc, "disk_1 = %s\n", disk_1_ccc);
    fprintf (debug_file_ccc, "script_file = %s\n", log_file_ccc);
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

    if (verbose_ccc & DEBUG1)
    {
      unsigned int c;
      for (c = 0; c < argument_count_ccc; c++)
        fprintf (stdout, "argument%d= %s\n", c+1, argument_ccc[c]);
    }
    if (debug_ccc & DEBUG1)
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

    if (verbose_ccc & DEBUG1)
    {
      unsigned int c;
      for (c = 0; c < argument_count_ccc; c++)
        fprintf (stdout, "argument%d= %s\n", c+1, argument_ccc[c]);
    }
    if (debug_ccc & DEBUG1)
    {
      unsigned int c;
      for (c = 0; c < argument_count_ccc; c++)
        fprintf (debug_file_ccc, "argument%d= %s\n", c+1, argument_ccc[c]);
    }
  }




  // exit on command line error
  if (command_line_error)
  {
    sprintf (tempmessage_ccc, "%s: Command line error.\n", title_ccc);
    message_exit_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, "Try '%s --help' for more information.\n", title_ccc);
    message_exit_ccc(tempmessage_ccc);
    exitcode_ccc = GENERAL_ERROR_EXIT_CODE;
    cleanup_ccc();
  }


  // end of command line processing



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
  superbyte_ccc[20] = 0x29;
  superbyte_ccc[21] = 0xa6;
  superbyte_ccc[22] = 0xd1;
  superbyte_ccc[23] = 0x5b;
  superbyte_ccc[24] = 0xb4;
  superbyte_ccc[25] = 0x71;
  superbyte_ccc[26] = 0xfa;
  superbyte_ccc[27] = 0x1e;
  superbyte_ccc[28] = 0x4f;
  superbyte_ccc[29] = 0x60;
  superbyte_ccc[30] = 0xd9;
  superbyte_ccc[31] = 0x9c;
  superbyte_ccc[32] = 0x83;
  superbyte_ccc[33] = 0xb0;
  superbyte_ccc[34] = 0x51;
  superbyte_ccc[35] = 0x26;
  superbyte_ccc[36] = 0x6f;
  superbyte_ccc[37] = 0xf5;
  superbyte_ccc[38] = 0x50;
  superbyte_ccc[39] = 0x0e;
  superbyte_ccc[40] = 0x05;
  superbyte_ccc[41] = 0xd4;
  superbyte_ccc[42] = 0x20;
  superbyte_ccc[43] = 0x3d;
  superbyte_ccc[44] = 0x88;
  superbyte_ccc[45] = 0x33;
  superbyte_ccc[46] = 0x9b;
  superbyte_ccc[47] = 0x41;
  superbyte_ccc[48] = 0x0a;
  superbyte_ccc[49] = 0xd9;
  superbyte_ccc[50] = 0x22;
  superbyte_ccc[51] = 0x4b;
  superbyte_ccc[52] = 0xf0;
  superbyte_ccc[53] = 0x8f;
  superbyte_ccc[54] = 0xc6;
  superbyte_ccc[55] = 0x46;
  superbyte_ccc[56] = 0x6b;
  superbyte_ccc[57] = 0xcc;
  superbyte_ccc[58] = 0x74;
  superbyte_ccc[59] = 0x0c;
  superbyte_ccc[60] = 0xaa;
  superbyte_ccc[61] = 0x6c;
  superbyte_ccc[62] = 0x15;
  superbyte_ccc[63] = 0x25;
  superbyte_ccc[64] = 0x08;
  superbyte_ccc[65] = 0x08;
  superbyte_ccc[66] = 0x08;
  superbyte_ccc[67] = 0x08;
  superbyte_ccc[68] = 0x04;
  superbyte_ccc[69] = 0x34;
  superbyte_ccc[70] = 0x07;
  superbyte_ccc[71] = 0x0f;
  superbyte_ccc[72] = 0x0e;
  superbyte_ccc[73] = 0x0c;
  superbyte_ccc[74] = 0x04;
  superbyte_ccc[75] = 0x08;
  superbyte_ccc[76] = 0x00;
  superbyte_ccc[77] = 0x7d;
  superbyte_ccc[78] = 0x61;
  superbyte_ccc[79] = 0x79;
  superbyte_ccc[80] = 0x78;
  superbyte_ccc[81] = 0x79;
  superbyte_ccc[82] = 0x75;
  superbyte_ccc[83] = 0x20;
  superbyte_ccc[84] = 0x93;
  superbyte_ccc[85] = 0xd3;
  superbyte_ccc[86] = 0x5b;
  superbyte_ccc[87] = 0x77;
  superbyte_ccc[88] = 0x60;
  superbyte_ccc[89] = 0x8e;
  superbyte_ccc[90] = 0x23;
  superbyte_ccc[91] = 0x73;
  superbyte_ccc[92] = 0x63;
  superbyte_ccc[93] = 0xb5;
  superbyte_ccc[94] = 0x78;
  superbyte_ccc[95] = 0x08;

  // Check if root privilages
  if (geteuid())
  {
    sprintf (tempmessage_ccc, "Root privilages not detected! This program must be run as root.\n");
    message_now_ccc(tempmessage_ccc);
    exitcode_ccc = GENERAL_ERROR_EXIT_CODE;
    cleanup_ccc();
  }


  if (superclone_ccc)
  {
    start_gtk_ccc(argc, argv, title_ccc, version_number_ccc);
  }
  else
  {
    if (!quiet_ccc)
    {
      fprintf (stdout, "%s %s\n", tool_title, tool_version_number);
      fprintf (stdout, "%s %s\n", scripts_title, scripts_version_number);
    }
    supertooltool_ccc();
  }

  cleanup_ccc();
  return 0;
}


// time functions
/* Return 1 if the difference is negative, otherwise 0.  */
int timeval_subtract_ccc(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
  long long diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
  result->tv_sec = diff / 1000000;
  result->tv_usec = diff % 1000000;

  return (diff<0);
}
void timeval_print_ccc(struct timeval *tv)
{
  char buffer[30];
  time_t curtime;

  // printf("%ld.%06ld", tv->tv_sec, tv->tv_usec);
  curtime = tv->tv_sec;
  strftime(buffer, 30, "%Y-%m-%d  %T", localtime(&curtime));
  printf("%s.%06ld\n", buffer, tv->tv_usec);
}








// function to cleanup and exit
void cleanup_ccc(void)
{
  cleanup_mode_ccc = true;
  int other_code = return_value_ccc;
  char gui_exit_message[EXIT_MESSAGE_SIZE * 2] = "";
  char tempstring[256];
  strcat (gui_exit_message, exit_message_ccc);

if (logfile_changed_ccc && log_file_ccc != NULL)
{
  return_value_ccc = write_logfile_ccc(log_file_ccc, 0);
  if (return_value_ccc)
  {
    sprintf (tempstring, "Error writing progress log file\n");
    strcat (gui_exit_message, tempstring);
  }
}

if (domainfile_changed_ccc && domain_file_ccc != NULL)
{
  return_value_ccc = write_domainfile_ccc(domain_file_ccc, 0);
  if (return_value_ccc)
  {
    sprintf (tempstring, "Error writing domain file\n");
    strcat (gui_exit_message, tempstring);
  }
}

  if (debug_ccc)
  {
    fclose(debug_file_ccc);
  }

  if (driver_running_ccc)
  {
    stop_driver_ccc();
  }

  if (memory_mapped_ccc)
  {
    unmap_driver_memory_ccc();
  }

  uninstall_driver_ccc();

  release_devices_ccc();

  dissconnect_usbr1_ccc();
  dissconnect_usbd1_ccc();

  free (ccc_buffer_ccc);
  free (ccc_scratchpad_ccc);
  free (table_buffer_ccc);
  free (fis_buffer_ccc);
  free (command_list_buffer_ccc);
  free (sensebuffer_ccc);
  free (lposition_ccc);
  free (lsize_ccc);
  free (lstatus_ccc);
  free (dposition_ccc);
  free (dsize_ccc);
  free (dstatus_ccc);
  free (padding_buffer_ccc);

  if (enable_data_dump_ccc || enable_dump_identify_on_check_ccc)
  {
    char temp_string[256];
    dump_info_to_filename_ccc (data_dump_filename_ccc, "*****************************************************************\n");
    sprintf (temp_string, "********** %s %s end of dump **********\n", title_ccc, version_number_ccc);
    dump_info_to_filename_ccc (data_dump_filename_ccc, temp_string);
  }

  // print time elapsed
  end_time_ccc = time(NULL);
  if (!quiet_ccc && !superclone_ccc)
  {
    sprintf (tempstring, "Total program run time = %ld seconds\n", end_time_ccc - start_time_ccc);
    strcat (gui_exit_message, tempstring);
  }

  if (!superclone_ccc)
  {
    sprintf (tempstring, "%s = %d\n%s = %d\n", "exit code", exitcode_ccc, "other code", other_code);
    strcat (gui_exit_message, tempstring);
  }

  if (1)
  {
    fprintf(stdout, "%s", gui_exit_message);
    fprintf (stdout, "\n");
  }

  exit (exitcode_ccc);
}





int begin_cloning_ccc (void)
{
  first_read_time_ccc = FIRST_READ_TIMER;
  if (fill_mode_ccc)
  {
    return_value_ccc = do_fill_ccc(FINISHED, STATUS_MASK & BAD_HEAD_MASK);
  }
  else if (reverse_ccc)
  {
    return_value_ccc = clone_reverse_ccc();
  }
  else
  {
    return_value_ccc = clone_forward_ccc();
  }

  if (return_value_ccc < 0 && !exitcode_ccc)
  {
    exitcode_ccc = return_value_ccc * -1;
  }
  else if (!exitcode_ccc)
  {
    exitcode_ccc = return_value_ccc;
  }
  write_logfile_ccc(log_file_ccc, 0);
  return (return_value_ccc);
}





int begin_driver_ccc (void)
{
  // if output offset is not default of -1 then this is a short trip, that is not allowed with the driver
  if (output_offset_ccc != -1)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGERRORDRIVEROFFSET]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return -1;
  }
  // if sector size is not evenly dividable by kernel sector size of 512 that wont work either
  if (sector_size_ccc % KERNEL_SECTOR_SIZE || output_sector_size_adjustment_ccc != 0)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGERRORDRIVERSECTORSIZE]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return -1;
  }
  // if cluster size is too big then cut it down
  if ((sector_size_ccc * original_cluster_size_ccc) > MAX_BUFFER_SIZE)
  {
    int cluster_limit = MAX_BUFFER_SIZE / sector_size_ccc;
    sprintf (tempmessage_ccc, "Warning: The cluster limit for the driver is %d and you chose %d.\n", cluster_limit , original_cluster_size_ccc);
    message_now_ccc(tempmessage_ccc);
    original_cluster_size_ccc = cluster_limit;
    sprintf (tempmessage_ccc, "Using a new cluster size of %d\n", original_cluster_size_ccc);
    message_now_ccc(tempmessage_ccc);
  }
  copy_start_time_ccc = get_elapsed_usec_ccc() / 1000;
  if (driver_return_zeros_ccc)
  {
    driver_return_zeros_on_error_ccc = 1;
  }
  else if (driver_return_marked_ccc)
  {
    driver_return_zeros_on_error_ccc = 2;
  }
  else
  {
    driver_return_zeros_on_error_ccc = 0;
  }
  // backup some settings
  long long current_position_bak = current_position_ccc;
  long long start_position_bak = start_position_ccc;
  long long end_position_bak = end_position_ccc;
  int ret = 0;
  //driver_buffer_ccc = malloc(DRIVER_TRANSFER_BUFFER_SIZE);
  //driver_control_data_ccc.buffer = driver_buffer_ccc;
  driver_control_data_ccc.logical_block_size = sector_size_ccc;
  driver_control_data_ccc.physical_block_size = sector_size_ccc * block_size_ccc;
  driver_control_data_ccc.total_logical_sectors = source_total_size_ccc;
  if (use_physical_sector_size_for_virtual_ccc)
  {
    driver_control_data_ccc.logical_block_size = sector_size_ccc * block_size_ccc;
    driver_control_data_ccc.total_logical_sectors = source_total_size_ccc / block_size_ccc;
  }
  driver_control_data_ccc.chs_heads = 255;
  driver_control_data_ccc.chs_sectors = 63;
  driver_control_data_ccc.chs_cylinders = driver_control_data_ccc.total_logical_sectors / (255*63);
  if (direct_mode_ccc)
  {
    driver_control_data_ccc.read_timeout = general_timeout_ccc / 2;
  }
  else
  {
    driver_control_data_ccc.read_timeout = general_timeout_ccc;
  }
  driver_control_data_ccc.request_timeout = general_timeout_ccc;
  driver_control_data_ccc.ack_timeout = general_timeout_ccc / 5;
  driver_control_data_ccc.process_id = process_id_ccc;
  driver_control_data_ccc.return_zeros_on_error = driver_return_zeros_on_error_ccc;
  driver_control_data_ccc.io_scsi_only = driver_io_scsi_only_ccc;
  write_ctrl_data_ccc(CTRL_RETURN_NO_DATA_FAST, 0);
  write_ctrl_data_ccc(CTRL_DATA_READY, 0);
  ret = start_driver_ccc();
  if (ret)
  {
    //free(driver_buffer_ccc);
    return ret;
  }

  int error = 0;
  int stop = 0;
  int n = 0;
  last_small_read_size_ccc = 0;
  last_big_read_size_ccc = 0;
  last_actual_read_size_ccc = 0;
  driver_read_rate_size_ccc = 0;
  write_ctrl_data_ccc(CTRL_DATA_READY, 0);
  while (1)
  {
    n++;
    strcpy(current_status_string_ccc, curlang_ccc[LANGIDLE]);

    if (aggressive_driver_ccc)
    {
      write_ctrl_data_ccc(CTRL_NO_DRIVER_SLEEP, 1);
    }
    else
    {
      write_ctrl_data_ccc(CTRL_NO_DRIVER_SLEEP, 0);
    }

    if (driver_mode_ccc > 4)
    {
      write_ctrl_data_ccc(CTRL_RETURN_NO_DATA_FAST, 1);
    }
    else
    {
      write_ctrl_data_ccc(CTRL_RETURN_NO_DATA_FAST, 0);
    }

    if (read_ctrl_data_ccc(CTRL_DATA_REQUEST))
    {
      write_ctrl_data_ccc(CTRL_ACK_REQUEST, 1);
      //fprintf (stdout, "request %d sector %lld count %lld total %lld\n", n, read_ctrl_data_ccc(CTRL_KSECTOR_START), read_ctrl_data_ccc(CTRL_KSECTOR_COUNT), read_ctrl_data_ccc(CTRL_KSECTORS_REQUESTED));    //debug
      long long start = read_ctrl_data_ccc(CTRL_KSECTOR_START) * (sector_size_ccc / KERNEL_SECTOR_SIZE);
      long long small_end = start + (read_ctrl_data_ccc(CTRL_KSECTOR_COUNT) * (sector_size_ccc / KERNEL_SECTOR_SIZE));
      long long big_end = start + (read_ctrl_data_ccc(CTRL_KSECTORS_REQUESTED) * (sector_size_ccc / KERNEL_SECTOR_SIZE));
      memset(driver_error_bitmap_address_ccc, 1, DRIVER_TRANSFER_BUFFER_SIZE / KERNEL_SECTOR_SIZE);
      ret = driver_clone_forward_ccc(start, small_end, big_end);
      if (ret)
      {
        break;
      }
      // clear or mark any buffer data not read if not in fast domain only mode
      if (driver_mode_ccc < 5)
      {
        unsigned int i;
        for (i = 0; i < read_ctrl_data_ccc(CTRL_KSECTOR_COUNT); i++)
        {
          // sanity check
          if (i > DRIVER_TRANSFER_BUFFER_SIZE / KERNEL_SECTOR_SIZE)
          {
            ret = -1;
            break;
          }
          if (read_ctrl_error_bitmap_ccc(i))
          {
            if (driver_return_marked_ccc)
            {
              unsigned long long n = 0;
              char *message = "HDDSUPERFILLMARK";
              uint64_t quad_word = 0xffeeeebbddaaeedd;
              while (n < KERNEL_SECTOR_SIZE)
              {
                memcpy(driver_transfer_buffer_address_ccc + (i * KERNEL_SECTOR_SIZE) + n, message, 16);
                n += 16;
                memcpy(driver_transfer_buffer_address_ccc + (i * KERNEL_SECTOR_SIZE) + n, &quad_word, 8);
                n += 8;
                memcpy(driver_transfer_buffer_address_ccc + (i * KERNEL_SECTOR_SIZE) + n, &quad_word, 8);
                n += 8;
              }
            }
            else
            {
              memset(driver_transfer_buffer_address_ccc + (i * KERNEL_SECTOR_SIZE), 0, KERNEL_SECTOR_SIZE);
            }
          }
        }
      }
      if (no_io_error_ccc)
      {
        memset(driver_error_bitmap_address_ccc, 0, DRIVER_TRANSFER_BUFFER_SIZE / KERNEL_SECTOR_SIZE);
      }
      write_ctrl_data_ccc(CTRL_DATA_REQUEST, 0);
      write_ctrl_data_ccc(CTRL_DATA_VALID, 1);
      write_ctrl_data_ccc(CTRL_DATA_READY, 1);
      //fprintf (stdout, "request complete\n");  //debug
    }

    update_display_ccc(DISPLAY_UPDATE_TIME);
    if (read_ctrl_data_ccc(CTRL_STOP_SIGNAL))
    {
      ret = -1;
      break;
    }
    if (stop_signal_ccc)
    {
      stop = 1;
      break;
    }

    do_nanosleep_ccc(1);
  }

  // restore settings
  current_position_ccc = current_position_bak;
  start_position_ccc = start_position_bak;
  end_position_ccc = end_position_bak;

  if (ret && ret != STOP_SIGNAL_RETURN_CODE)
  {
    error = 1;
    strcpy (tempmessage_ccc, curlang_ccc[LANGERRORDRIVERSTOPPEDERROR]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }

  ret = stop_driver_ccc();
  if (ret)
  {
    error = 1;
  }
  //free(driver_buffer_ccc);
  if (error)
  {
    return -1;
  }
  if (stop)
  {
    return STOP_SIGNAL_RETURN_CODE;
  }
  return 0;
}





void release_devices_ccc (void)
{
  if (rebuild_assist_enabled_ccc)
  {
    set_rebuild_assist_disabled_ccc();
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
    memcpy(port_virt_addr_ccc + 0x14, &interrupt_backup_ccc, 4);    //potential superbyte
    ahci_interrupt_changed_ccc = false;
  }

  if (table_address_changed_ccc)
  {
    outb(table_address_backup_ccc[0], bus_base_address_ccc+4);
    outb(table_address_backup_ccc[1], bus_base_address_ccc+5);
    outb(table_address_backup_ccc[2], bus_base_address_ccc+6);
    outb(table_address_backup_ccc[3], bus_base_address_ccc+7);
    table_address_changed_ccc = false;
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

  if (io_permissions_ccc)
  {
    // turn interrupts back on, but make sure correct device first
    set_device_ccc(reg_base_address_ccc, device_select_base_ccc);
    outb(0, control_base_address_ccc);
    // turn off io permissions
    ioperm (reg_base_address_ccc, 8, 0);
    ioperm (control_base_address_ccc, 1, 0);
    ioperm (bus_base_address_ccc, 8, 0);
    io_permissions_ccc = false;
  }

  if (disk1_fd_ccc > 0)
  {
    close (disk1_fd_ccc);
    disk1_fd_ccc = -1;
  }
  if (disk2_fd_ccc > 0)
  {
    close (disk2_fd_ccc);
    disk2_fd_ccc = -1;
  }

  dissconnect_usbd1_ccc();
}





void install_driver_ccc (void)
{
  if (!check_driver_ccc() && driver_memory_mapped_ccc)
  {
    sprintf (tempmessage_ccc, "%s", curlang_ccc[LANGDRIVERALREADYINSTALLED]);
    message_now_ccc(tempmessage_ccc);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
    clear_error_message_ccc();
    driver_installed_ccc = 1;
    return;
  }
  else if (!check_driver_ccc())
  {
    if (!map_driver_memory_ccc())
    {
      sprintf (tempmessage_ccc, "%s", curlang_ccc[LANGDRIVERMEMORYCONNECTED]);
      message_now_ccc(tempmessage_ccc);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
      clear_error_message_ccc();
      driver_installed_ccc = 1;
      return;
    }
    else
    {
      sprintf (tempmessage_ccc, "%s", curlang_ccc[LANGERRORCONNECTINGDRIVER]);
      message_now_ccc(tempmessage_ccc);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
      clear_error_message_ccc();
      driver_installed_ccc = 0;
      return;
    }
  }
  char tempdir[128];
  sprintf (tempdir, "/tmp/hddsctemp%d", process_id_ccc);
  char command[256];
  sprintf (command, "rm -rf %s", tempdir);
  if (system(command))
  {
    sprintf (tempmessage_ccc, "%s", curlang_ccc[LANGFAILEDTOCLEANDIR]);
    message_now_ccc(tempmessage_ccc);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    //return;
  }
  mkdir(tempdir, 0777);
  char name[256];
  sprintf (name, "%s/%s%d.c", tempdir, DRIVER_FILE_NAME, process_id_ccc);
  FILE *writefile;
  writefile = fopen(name, "w");
  if (writefile == NULL)
  {
    sprintf (tempmessage_ccc, "%s %s (%s).\n", curlang_ccc[LANGCANNOTOPENFORWRITING], name, strerror(errno));
    message_now_ccc(tempmessage_ccc);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return;
  }
  unsigned int i;
  for (i = 0; i < driver_hddsuperclone_driver_c_len; i++)
  {
    fprintf (writefile, "%c", driver_hddsuperclone_driver_c[i]);
  }
  int fp = fileno(writefile);
  fsync(fp);
  fclose(writefile);

  sprintf (command, "%s/Makefile", tempdir);
  writefile = fopen(command, "w");
  if (writefile == NULL)
  {
    sprintf (tempmessage_ccc, "%s %s (%s).\n", curlang_ccc[LANGCANNOTOPENFORWRITING], command, strerror(errno));
    message_now_ccc(tempmessage_ccc);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return;
  }
  char obj[256];
  sprintf (obj, "obj-m = %s%d.o", DRIVER_FILE_NAME, process_id_ccc);
  fprintf (writefile, "%s", obj);
  fp = fileno(writefile);
  fsync(fp);
  fclose(writefile);

  sprintf (command, "(cd %s; make -C/lib/modules/`uname -r`/build M=$PWD)", tempdir);
  if (system (command) )
  {
    sprintf (tempmessage_ccc, "%s", curlang_ccc[LANGMAKEFAILED]);
    message_now_ccc(tempmessage_ccc);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return;
  }

  sprintf (command, "insmod %s/%s%d.ko ioctl=%s%d mmap_m=%s%d mmap_tb=%s%d mmap_mdb=%s%d", tempdir, DRIVER_FILE_NAME, process_id_ccc, MAIN_DRIVER_IOCTL_NAME, process_id_ccc, MAIN_DRIVER_MMAP_NAME, process_id_ccc, MAIN_DRIVER_MMAPTB_NAME, process_id_ccc, MAIN_DRIVER_MMAPMDB_NAME, process_id_ccc);
  if (system(command))
  {
    sprintf (tempmessage_ccc, "%s", curlang_ccc[LANGFAILEDTOINSTALLDRIVER]);
    message_now_ccc(tempmessage_ccc);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return;
  }

  sprintf (command, "rm -rf %s", tempdir);
  if (system(command))
  {
    sprintf (tempmessage_ccc, "%s", curlang_ccc[LANGFAILEDTOCLEANDIR]);
    message_now_ccc(tempmessage_ccc);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    //return;
  }

  if (map_driver_memory_ccc())
  {
    sprintf (tempmessage_ccc, "%s", curlang_ccc[LANGERRORCONNECTINGDRIVER]);
    message_now_ccc(tempmessage_ccc);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
    clear_error_message_ccc();
    driver_installed_ccc = 0;
    return;
  }

  sprintf (tempmessage_ccc, "%s", curlang_ccc[LANGDRIVERINSTALLSUCCESS]);
  message_now_ccc(tempmessage_ccc);
  message_error_ccc(tempmessage_ccc);
  print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
  clear_error_message_ccc();
  if (superclone_ccc)
  {
    initialize_memory_ccc();
  }
  driver_installed_ccc = 1;
  return;
}




void uninstall_driver_ccc (void)
{
  if (driver_memory_mapped_ccc)
  {
    unmap_driver_memory_ccc();
  }
  if (check_driver_ccc())
  {
    //sprintf (tempmessage_ccc, "The driver is currently not installed\n");
    //message_now_ccc(tempmessage_ccc);
    //message_error_ccc(tempmessage_ccc);
    //print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
    //clear_error_message_ccc();
    driver_installed_ccc = 0;
    return;
  }
  char name[256];
  sprintf (name, "rmmod %s%d", DRIVER_FILE_NAME, process_id_ccc);
  if (system(name))
  {
    sprintf (tempmessage_ccc, "%s", curlang_ccc[LANGFAILEDTOREMOVEDRIVER]);
    message_now_ccc(tempmessage_ccc);
    //message_error_ccc(tempmessage_ccc);
    //print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    //clear_error_message_ccc();
    return;
  }
  sprintf (tempmessage_ccc, "%s", curlang_ccc[LANGDRIVERREMOVALSUCCESS]);
  message_now_ccc(tempmessage_ccc);
  //message_error_ccc(tempmessage_ccc);
  //print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
  //clear_error_message_ccc();
  if (!cleanup_mode_ccc)
  {
    initialize_memory_ccc();
  }
  driver_installed_ccc = 0;
  return;
}





void fix_driver_memory_driver_ccc (void)
{
  int res0=system("sync");
  int res1=system("echo 3 > /proc/sys/vm/drop_caches");
  int res2=system("echo 1 > /proc/sys/vm/compact_memory");
  if(res0==0 && res1==0 && res2==0)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGOPERATIONSUCCEEDED]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
    clear_error_message_ccc();
  }
}





// function to display help
void help_ccc(void)
{
  if (superclone_ccc)
  {
    unsigned int i;
    for (i = 0; i < hddsuperclone_help_txt_len; i++)
    {
      fprintf (stdout, "%c", hddsuperclone_help_txt[i]);
    }
  }
  else
  {
    unsigned int i;
    for (i = 0; i < hddsupertool_help_txt_len; i++)
    {
      printf ("%c", hddsupertool_help_txt[i]);
    }
  }
}

// function to display version
void version_ccc(void)
{
  if (superclone_ccc)
  {
  fprintf (stdout, "%s %s\n", title_ccc, version_number_ccc);
  }
  else
  {
    printf ("%s %s\n", tool_title, tool_version_number);
    fprintf (stdout, "%s %s\n", scripts_title, scripts_version_number);
  }
#ifdef GODMODE
  fprintf (stdout, "Copyright (C) %s Scott Dwyer.\n", copyright_year_ccc);
  fprintf (stdout, "License type: PROPRIETARY (GOD MODE)\n");
  fprintf (stdout, "There is NO WARRANTY, to the extent permitted by law.\n");
#else
  fprintf (stdout, "Copyright (C) %s Scott Dwyer.\n", copyright_year_ccc);
  fprintf (stdout, "License type: PROPRIETARY\n");
  fprintf (stdout, "There is NO WARRANTY, to the extent permitted by law.\n");
#endif
}





// function to initialize needed memory
int initialize_logfile_memory_ccc(void)
{
  // assign memory for log
  free (lposition_ccc);
  lposition_ccc=malloc(sizeof(*lposition_ccc) * log_rows_ccc);
  if (lposition_ccc == NULL)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGALLOCATEMEMERR]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, " (%s)", strerror(errno));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return GENERAL_ERROR_RETURN_CODE;
  }
  free (lsize_ccc);
  lsize_ccc=malloc(sizeof(*lsize_ccc) * log_rows_ccc);
  if (lsize_ccc == NULL)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGALLOCATEMEMERR]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, " (%s)", strerror(errno));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return GENERAL_ERROR_RETURN_CODE;
  }
  free (lstatus_ccc);
  lstatus_ccc=malloc(sizeof(*lstatus_ccc) * log_rows_ccc);
  if (lstatus_ccc == NULL)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGALLOCATEMEMERR]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, " (%s)", strerror(errno));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return GENERAL_ERROR_RETURN_CODE;
  }

  return 0;
}





// function to initialize needed memory
int initialize_domainfile_memory_ccc(void)
{
  // assign memory for domain
  free (dposition_ccc);
  dposition_ccc=malloc(sizeof(*dposition_ccc) * domain_rows_ccc);
  if (dposition_ccc == NULL)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGALLOCATEMEMERR]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, " (%s)", strerror(errno));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return GENERAL_ERROR_RETURN_CODE;
  }
  free (dsize_ccc);
  dsize_ccc=malloc(sizeof(*dsize_ccc) * domain_rows_ccc);
  if (dsize_ccc == NULL)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGALLOCATEMEMERR]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, " (%s)", strerror(errno));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return GENERAL_ERROR_RETURN_CODE;
  }
  free (dstatus_ccc);
  dstatus_ccc=malloc(sizeof(*dstatus_ccc) * domain_rows_ccc);
  if (dstatus_ccc == NULL)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGALLOCATEMEMERR]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, " (%s)", strerror(errno));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return GENERAL_ERROR_RETURN_CODE;
  }

  return 0;
}






// function to initialize needed memory
int initialize_memory_ccc(void)
{
  real_buffer_size_ccc = MAX_BUFFER_SIZE;
  memory_failed_ccc = true;
  int attempt = 0;
  if (usb_mode_ccc && !driver_memory_mapped_ccc)
  {
    real_buffer_size_ccc = MAX_USB_BUFFER_SIZE;
  }
  while (attempt < 2)
  {
    attempt ++;
    if (!quiet_ccc)
    {
      sprintf (tempmessage_ccc, "Initializing memory\n");
      message_now_ccc(tempmessage_ccc);
    }

    if (attempt == 2 && !driver_installed_ccc)
    {
      install_driver_ccc();
    }

    {
      int multiplier = 1;
      if (driver_installed_ccc && driver_memory_mapped_ccc)
      {
        multiplier = 4;
      }
      max_dma_size_ccc = ((pagesize_ccc * multiplier) / 16) * pagesize_ccc;
      if (ahci_mode_ccc)
      {
        max_dma_size_ccc = ( ((pagesize_ccc * multiplier) - 128) / 16 ) * pagesize_ccc;
      }
    }

    padding_buffer_ccc = malloc(pagesize_ccc);
    // initialize the table buffer
    if (direct_mode_ccc)
    {
      return_value_ccc = set_table_buffer_ccc();
      if (return_value_ccc != 0)
      {
        if (attempt < 2)
        {
          continue;
        }
        strcpy (tempmessage_ccc, curlang_ccc[LANGBUFFERADDRESSRANGE]);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return GENERAL_ERROR_RETURN_CODE;
      }
    }


    // initialize the command list buffer
    if (ahci_mode_ccc)
    {
      return_value_ccc = set_command_list_buffer_ccc();
      if (return_value_ccc != 0)
      {
        if (attempt < 2)
        {
          continue;
        }
        strcpy (tempmessage_ccc, curlang_ccc[LANGBUFFERADDRESSRANGE]);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return GENERAL_ERROR_RETURN_CODE;
      }
    }


    // initialize the FIS buffer
    if (ahci_mode_ccc)
    {
      return_value_ccc = set_fis_buffer_ccc();
      if (return_value_ccc != 0)
      {
        if (attempt < 2)
        {
          continue;
        }
        strcpy (tempmessage_ccc, curlang_ccc[LANGBUFFERADDRESSRANGE]);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return GENERAL_ERROR_RETURN_CODE;
      }
    }


    // initialize main buffer
    // create a buffer that is memory aligned with the pagesize
    if (direct_mode_ccc)
    {
      return_value_ccc = get_buffer_physical_memory_locations_ccc();
      if (return_value_ccc != 0)
      {
        if (attempt < 2)
        {
          continue;
        }
        exitcode_ccc = GENERAL_ERROR_EXIT_CODE;
        return (return_value_ccc);
      }
    }
    else
    {
      if (!driver_memory_mapped_ccc)
      {
        free (ccc_buffer_ccc);
        ccc_buffer_ccc = valloc(real_buffer_size_ccc);
        if (!ccc_buffer_ccc)
        {
          strcpy (tempmessage_ccc, curlang_ccc[LANGPOSIXMEMFAIL]);
          message_error_ccc(tempmessage_ccc);
          sprintf (tempmessage_ccc, " (%s)", strerror(errno));
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
          clear_error_message_ccc();
          return GENERAL_ERROR_RETURN_CODE;
        }
      }
      else
      {
        ccc_buffer_ccc = (char*) driver_main_data_buffer_address_ccc;
      }
      memset (ccc_buffer_ccc, 0, real_buffer_size_ccc);
    }
    if (!quiet_ccc)
    {
      sprintf (tempmessage_ccc, "Memory initialized\n");
      message_now_ccc(tempmessage_ccc);
    }
    attempt ++;
  }

  // set the starting main buffer size
  ccc_main_buffer_size_ccc = 512;
  return_value_ccc = set_main_buffer_ccc();
  if (return_value_ccc != 0)
  {
    exitcode_ccc = GENERAL_ERROR_EXIT_CODE;
    return (-1);
  }

  // set the starting scratchpad size
  ccc_main_scratchpad_size_ccc = 512;
  return_value_ccc = set_main_scratchpad_ccc();
  if (return_value_ccc != 0)
  {
    exitcode_ccc = GENERAL_ERROR_EXIT_CODE;
    return (-1);
  }

  // set the sensebuffer size
  sensebuffer_size_ccc = 256;
  return_value_ccc = set_sense_buffer_ccc();
  if (return_value_ccc != 0)
  {
    exitcode_ccc = GENERAL_ERROR_EXIT_CODE;
    return (-1);
  }

  memory_used_ccc = 0;
  memory_failed_ccc = false;
  return (0);
}







// function to set scratchpad buffer
int set_main_scratchpad_ccc(void)
{
  if (ccc_main_scratchpad_size_ccc > MAX_SCRATCHPAD_SIZE)
  {
    if (superclone_ccc)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGMAXSCRATCHPADSIZE]);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
    }
    else
    {
      fprintf (stderr, "ERROR: Maximum scratchpad size exceeded.\n");
    }
    return GENERAL_ERROR_RETURN_CODE;
  }
  free (ccc_scratchpad_ccc);
  ccc_scratchpad_ccc = calloc(ccc_main_scratchpad_size_ccc, 1);
  if (ccc_scratchpad_ccc == NULL)
  {
    if (superclone_ccc)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGALLOCATEMEMERR]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " (%s)", strerror(errno));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
    }
    else
    {
      fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    }
    return GENERAL_ERROR_RETURN_CODE;
  }
  return (0);
}






// function to set sense buffer
int set_sense_buffer_ccc(void)
{
  if (sensebuffer_size_ccc > MAX_SENSEBUFFER_SIZE)
  {
    if (superclone_ccc)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGMAXSENSESIZE]);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
    }
    else
    {
      fprintf (stderr, "ERROR: Maximum sensebuffer size exceeded.\n");
    }
    return GENERAL_ERROR_RETURN_CODE;
  }
  free (sensebuffer_ccc);
  sensebuffer_ccc = calloc(sensebuffer_size_ccc, 1);
  if (sensebuffer_ccc == NULL)
  {
    if (superclone_ccc)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGALLOCATEMEMERR]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " (%s)", strerror(errno));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
    }
    else
    {
      fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    }
    return GENERAL_ERROR_RETURN_CODE;
  }
  return (0);
}




#if 0
// function to set usb buffer
int set_main_usb_buffer_ccc(void)
{
  if (ccc_main_usbbuffer_size_ccc > MAX_USB_BUFFER_SIZE)
  {
    if (superclone_ccc)
    {
      strcpy (tempmessage_ccc, "ERROR: Maximum USB buffer size exceeded.\n");    //TODO make a language entry for this
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
    }
    else
    {
      fprintf (stderr, "ERROR: Maximum USB buffer size exceeded.\n");
    }
    return GENERAL_ERROR_RETURN_CODE;
  }
  free (ccc_usbbuffer_ccc);
  ccc_usbbuffer_ccc = calloc(ccc_main_usbbuffer_size_ccc, 1);
  if (ccc_usbbuffer_ccc == NULL)
  {
    if (superclone_ccc)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGALLOCATEMEMERR]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " (%s)", strerror(errno));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
    }
    else
    {
      fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    }
    return GENERAL_ERROR_RETURN_CODE;
  }
  return (0);
}
#endif





// function to read the log file into memory
int read_log_file_ccc(char *log_file)
{
  if (log_file == NULL)
  {
    sprintf (tempmessage_ccc, "Error: No log file specified.\n");
    message_error_ccc(tempmessage_ccc);
    return (1);
  }

  if (verbose_ccc & DEBUG2)
  {
    sprintf (tempmessage_ccc, "Reading %s into memory...\n", log_file);
    message_now_ccc(tempmessage_ccc);
  }

  FILE *readfile;
  readfile = fopen(log_file, "r");
  if (readfile == NULL)
  {
    sprintf (tempmessage_ccc, "Cannot open %s for reading (%s).\n", log_file, strerror(errno));
    message_error_ccc(tempmessage_ccc);
    return (1);
  }

  char line[MAX_LINE_LENGTH];
  int i = 0;
  int real_line_number = 0;
  int found_current = 0;
  int found_error = 0;
  int config_data = 0;
  int analyze_data = 0;
  int smart_data = 0;
  int analyze_line = 0;
  int smart_line = 0;
  strcpy (analyze_text_ccc, "");
  strcpy(smart_data_text_ccc, "");
  while (fgets(line, sizeof line, readfile))
  {
    real_line_number++;
    // process the line here
    if (verbose_ccc & DEBUG2)
    {
      sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
      message_now_ccc(tempmessage_ccc);
    }

    int scanline;
    char raw_position[MAX_LINE_LENGTH];
    char raw_size[MAX_LINE_LENGTH];
    char raw_status[MAX_LINE_LENGTH];
    char raw_info[MAX_LINE_LENGTH];
    char raw_errstat[MAX_LINE_LENGTH];
    char rest_of_line[MAX_LINE_LENGTH];
    strcpy (raw_position, "");
    strcpy (raw_size, "");
    strcpy (raw_status, "");
    strcpy (rest_of_line, "");
    scanline = sscanf(line, "%s %s %s %s %s %[^\n]", raw_position, raw_size, raw_status, raw_info, raw_errstat, rest_of_line);
    // ignore blank lines
    if (scanline > 0)
    {
      // if the line doesn't start with a comment then process it
      if (raw_position[0] != '#' && !analyze_data && !smart_data)
      {
        // the first line should be the current position and status
        if (!found_current && scanline == 2)
        {
          char* endptr;
          current_position_ccc = strtoull(raw_position, &endptr, 0);
          if (*endptr)
          {
            sprintf (tempmessage_ccc, "error processing position on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
            message_now_ccc(tempmessage_ccc);
            found_error = 1;
          }
          current_status_ccc = strtoull(raw_size, &endptr, 0);
          if (*endptr)
          {
            sprintf (tempmessage_ccc, "error processing status on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
            message_now_ccc(tempmessage_ccc);
            found_error = 1;
          }
          else
          {
            if (current_status_ccc == PHASE1)
            {
              strcpy(current_status_string_ccc, curlang_ccc[LANGPHASE1]);
            }
            else if (current_status_ccc == PHASE2)
            {
              strcpy(current_status_string_ccc, curlang_ccc[LANGPHASE2]);
            }
            else if (current_status_ccc == PHASE3)
            {
              strcpy(current_status_string_ccc, curlang_ccc[LANGPHASE3]);
            }
            else if (current_status_ccc == PHASE4)
            {
              strcpy(current_status_string_ccc, curlang_ccc[LANGPHASE4]);
            }
            else if (current_status_ccc == TRIMMING)
            {
              strcpy(current_status_string_ccc, curlang_ccc[LANGTRIMMING]);
            }
            else if (current_status_ccc == DIVIDING1)
            {
              strcpy(current_status_string_ccc, curlang_ccc[LANGDIVIDING1]);
            }
            else if (current_status_ccc == DIVIDING2)
            {
              strcpy(current_status_string_ccc, curlang_ccc[LANGDIVIDING2]);
            }
            else if (current_status_ccc == SCRAPING)
            {
              strcpy(current_status_string_ccc, curlang_ccc[LANGSCRAPING]);
            }
            else if (current_status_ccc == FINISHED)
            {
              strcpy(current_status_string_ccc, curlang_ccc[LANGFINISHED]);
            }
            else if (current_status_ccc == RETRYING)
            {
              strcpy(current_status_string_ccc, curlang_ccc[LANGRETRYING]);
            }
            else if (current_status_ccc == FILLING)
            {
              strcpy(current_status_string_ccc, curlang_ccc[LANGFILLING]);
            }
            else
            {
              strcpy(current_status_string_ccc, "");
            }
          }
          found_current = 1;
          sprintf (tempmessage_ccc, "%d=  %d  0x%08llx  0x%08llx\n", real_line_number, i, current_position_ccc, current_status_ccc);    //debug
          message_debug_ccc(tempmessage_ccc, DEBUG14);
        }
        else if (!found_current)
        {
          sprintf (tempmessage_ccc, "error processing progress log file line %d, expecting current status\n", real_line_number);
          message_now_ccc(tempmessage_ccc);
          sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
          message_now_ccc(tempmessage_ccc);
          found_error = 1;
        }
        else if (scanline != 5)
        {
          sprintf (tempmessage_ccc, "error processing progress log file line %d\n", real_line_number);
          message_now_ccc(tempmessage_ccc);
          sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
          message_now_ccc(tempmessage_ccc);
          found_error = 1;
        }
        else
        {
          char* endptr;
          lposition_ccc[i] = strtoull(raw_position, &endptr, 0);
          if (*endptr)
          {
            sprintf (tempmessage_ccc, "error processing position on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
            message_now_ccc(tempmessage_ccc);
            found_error = 1;
          }
          lsize_ccc[i] = strtoull(raw_size, &endptr, 0);
          if (*endptr)
          {
            sprintf (tempmessage_ccc, "error processing size on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
            message_now_ccc(tempmessage_ccc);
            found_error = 1;
          }
          lstatus_ccc[i] = strtoll(raw_status, &endptr, 0);
          if (*endptr)
          {
            sprintf (tempmessage_ccc, "error processing status on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
            message_now_ccc(tempmessage_ccc);
            found_error = 1;
          }
          lstatus_ccc[i] += (strtoll(raw_info, &endptr, 0)) << 8;
          if (*endptr)
          {
            sprintf (tempmessage_ccc, "error processing info on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
            message_now_ccc(tempmessage_ccc);
            found_error = 1;
          }
          lstatus_ccc[i] += (strtoll(raw_errstat, &endptr, 0)) << 32;
          if (*endptr)
          {
            sprintf (tempmessage_ccc, "error processing err/stat on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
            message_now_ccc(tempmessage_ccc);
            found_error = 1;
          }
          sprintf (tempmessage_ccc, "%d=  %d  0x%08llx  0x%08llx  0x%08llx\n", real_line_number, i, lposition_ccc[i], lsize_ccc[i], lstatus_ccc[i]);    //debug
          message_debug_ccc(tempmessage_ccc, DEBUG14);
          i++;
        }
      }
      // if the line starts with # then check for config data
      else if (strcmp(raw_position, "#") == 0)
      {
        char raw_command[MAX_LINE_LENGTH] = "";
        char raw_data[MAX_LINE_LENGTH] = "";
        sscanf(line, "# %s %[^\n]", raw_command, raw_data);
        // if we are not in config data then check if config start
        if (!config_data)
        {
          if ( strcmp(raw_command, "startconfig") == 0 )
          {
            config_data = 1;
          }
        }
        if (!analyze_data)
        {
          if ( strcmp(raw_command, "startanyalyze") == 0 )
          {
            analyze_data = 1;
          }
        }
        if (analyze_data)
        {
          if ( strcmp(raw_command, "endanalyze") == 0 )
          {
            analyze_data = 0;
          }
        }
        if (!smart_data)
        {
          if ( strcmp(raw_command, "startsmart") == 0 )
          {
            smart_data = 1;
          }
        }
        if (smart_data)
        {
          if ( strcmp(raw_command, "endsmart") == 0 )
          {
            smart_data = 0;
          }
        }
        if (config_data)
        {
          // we are in config data so read the data
          if ( strcmp(raw_command, "endconfig") == 0 )
          {
            config_data = 0;
          }
          else if ( strcmp(raw_command, "startconfig") == 0 )
          {
            config_data = 1;
          }

          else
          {
            if ( strcmp(raw_command, "logfile") == 0 )
            {
              // do nothing
            }
            else if ( strcmp(raw_command, "source") == 0 )
            {
              //disk_1_ccc = malloc (sizeof raw_data);
              //memset (disk_1_ccc, 0, sizeof raw_data);
              //strcpy (disk_1_ccc, raw_data);
            }
            else if ( strcmp(raw_command, "destination") == 0 )
            {
              //disk_2_ccc = malloc (sizeof raw_data);
              //memset (disk_2_ccc, 0, sizeof raw_data);
              //strcpy (disk_2_ccc, raw_data);
            }
            else if ( strcmp(raw_command, "domainfile") == 0 )
            {
              domain_file_ccc = malloc (sizeof raw_data);
              memset (domain_file_ccc, 0, sizeof raw_data);
              strcpy (domain_file_ccc, raw_data);
            }
            else if ( strcmp(raw_command, "findserial") == 0 )
            {
              //drive_serial_ccc = malloc (sizeof raw_data);
              //memset (drive_serial_ccc, 0, sizeof raw_data);
              //strcpy (drive_serial_ccc, raw_data);
            }
            else if ( strcmp(raw_command, "model") == 0 )
            {
              strncpy (current_model_ccc, raw_data, sizeof(current_model_ccc)-1);
              strncpy (current_source_model_ccc, raw_data, sizeof(current_source_model_ccc)-1);
            }
            else if ( strcmp(raw_command, "serial") == 0 )
            {
              strncpy (current_serial_ccc, raw_data, sizeof(current_serial_ccc)-1);
              strncpy (current_source_serial_ccc, raw_data, sizeof(current_source_serial_ccc)-1);
            }
            else if ( strcmp(raw_command, "verbose") == 0 )
            {
              // do nothing
            }
            else if ( strcmp(raw_command, "debug") == 0 )
            {
              // do nothing
            }
            else if ( strcmp(raw_command, "forcemounted") == 0 )
            {
              //force_mounted_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "forcedangerous") == 0 )
            {
              //force_mounted_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "importdd") == 0 )
            {
              // do nothing
            }
            else if ( strcmp(raw_command, "exportdd") == 0 )
            {
              // do nothing
            }
            else if ( strcmp(raw_command, "dodomain") == 0 )
            {
              do_domain_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "domaindd") == 0 )
            {
              domain_dd_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "repairlog") == 0 )
            {
              // do nothing
            }
            else if ( strcmp(raw_command, "nologbackup") == 0 )
            {
              no_log_backup_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "phaselogs") == 0 )
            {
              enable_logfile_phase_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "regaddr") == 0 )
            {
              //reg_base_address_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "conaddr") == 0 )
            {
              //control_base_address_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "busaddr") == 0 )
            {
              //bus_base_address_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "device") == 0 )
            {
              //device_select_base_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "hbaaddr") == 0 )
            {
              //hba_base_address_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "portnum") == 0 )
            {
              //port_number_base_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "portaddr") == 0 )
            {
              //port_base_address_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "softtimer") == 0 )
            {
              soft_reset_time_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "hardtimer") == 0 )
            {
              hard_reset_time_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "busytimer") == 0 )
            {
              busy_wait_time_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "initbusytimer") == 0 )
            {
              initial_busy_wait_time_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "resettimer") == 0 ) // this is also power timer
            {
              reset_wait_time_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "generaltimer") == 0 )
            {
              general_timeout_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "phasetimers") == 0 )
            {
              phase_timers_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "p12softtimer") == 0 )
            {
              p12_soft_reset_time_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "p3softtimer") == 0 )
            {
              p3_soft_reset_time_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "p4softtimer") == 0 )
            {
              p4_soft_reset_time_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "tdsofttimer") == 0 )
            {
              td_soft_reset_time_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "d2softtimer") == 0 )
            {
              d2_soft_reset_time_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "scsofttimer") == 0 )
            {
              sc_soft_reset_time_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "rtsofttimer") == 0 )
            {
              rt_soft_reset_time_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "alwayswaitresettime") == 0 )
            {
              always_wait_for_reset_timers_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "usbbulkreset") == 0 )
            {
              usb_bulk_reset_enabled_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "usbsoftreset") == 0 )
            {
              usb_soft_reset_enabled_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "usbhardreset") == 0 )
            {
              usb_hard_reset_enabled_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "usbportreset") == 0 )
            {
              usb_port_reset_enabled_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "imagesource") == 0 )
            {
              //copy_image_mode_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "genericsource") == 0 )
            {
              //generic_mode_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "atapass") == 0 )
            {
              //ata_passthrough_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "scsipass") == 0 )
            {
              //scsi_passthrough_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "skipfast") == 0 )
            {
              skip_fast_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "skiptimer") == 0 )
            {
              skip_timeout_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "direct") == 0 )
            {
              //direct_mode_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "ahci") == 0 )
            {
              //ahci_mode_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "usb") == 0 )
            {
              //usb_mode_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "resetstatus") == 0 )
            {
              // do nothing
            }
            else if ( strcmp(raw_command, "nophase1") == 0 )
            {
              no_phase1_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "nophase2") == 0 )
            {
              no_phase2_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "nophase3") == 0 )
            {
              no_phase3_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "nophase4") == 0 )
            {
              no_phase4_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "nodivide1") == 0 )
            {
              no_divide1_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "dodivide2") == 0 )
            {
              do_divide2_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "notrim") == 0 )
            {
              no_trim_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "noscrape") == 0 )
            {
              no_scrape_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "markbadhead") == 0 )
            {
              mark_bad_head_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "readbadhead") == 0 )
            {
              read_bad_head_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "rebuildassist") == 0 )
            {
              rebuild_assist_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "clustersize") == 0 )
            {
              original_cluster_size_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "blocksize") == 0 )
            {
              block_size_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "sectorsize") == 0 )
            {
              sector_size_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "blockoffset") == 0 )
            {
              block_offset_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "maxreadrate") == 0 )
            {
              max_read_rate_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "retries") == 0 )
            {
              retries_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "retried") == 0 )
            {
              retried_lba_count_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "inputoffset") == 0 )
            {
              input_offset_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "outputoffset") == 0 )
            {
              output_offset_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "size") == 0 )
            {
              read_size_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "rateskip") == 0 )
            {
              rate_skip_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "exitonslow") == 0 )
            {
              exit_on_slow_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "exitonslowtime") == 0 )
            {
              exit_slow_time_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "minskip") == 0 )
            {
              original_min_skip_size_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "maxskip") == 0 )
            {
              original_max_skip_size_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "reverse") == 0 )
            {
              reverse_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "fillmode") == 0 )
            {
              fill_mode_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "fillmark") == 0 )
            {
              fill_mark_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "quiet") == 0 )
            {
              quiet_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "forcestupid") == 0 )
            {
              //force_stupid_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "skipsize") == 0 )
            {
              min_skip_size_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "skips") == 0 )
            {
              total_skip_count_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "slowskips") == 0 )
            {
              total_slow_skips_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "skipruns") == 0 )
            {
              total_skip_runs_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "skipresets") == 0 )
            {
              total_skip_resets_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "stoponerror") == 0 )
            {
              stop_on_error_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "callcommand") == 0 )
            {
              do_call_command_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "commandtocall") == 0 )
            {
              //fprintf (stdout, "command= %s\n", raw_data);    //debug
              strcpy (command_to_call_ccc, raw_data);
            }
            else if ( strcmp(raw_command, "primaryrelayonerror") == 0 )
            {
              activate_primary_relay_on_error_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "callpowercommand") == 0 )
            {
              call_power_command_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "powercommandtocall") == 0 )
            {
              //fprintf (stdout, "powercommand= %s\n", raw_data);    //debug
              strcpy (power_command_to_call_ccc, raw_data);
            }
            else if ( strcmp(raw_command, "primaryrelaypowercycle") == 0 )
            {
              power_cycle_primary_relay_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "writebufferdisable") == 0 )
            {
              write_buffer_disable_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "writebufferenable") == 0 )
            {
              write_buffer_enable_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "driverreturnerror") == 0 )
            {
              driver_return_error_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "driverreturnzeros") == 0 )
            {
              driver_return_zeros_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "driverreturnmarked") == 0 )
            {
              driver_return_marked_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "driverdiskname") == 0 )
            {
              strcpy (virtual_driver_name_ccc, raw_data);
            }
            else if ( strcmp(raw_command, "driverminclustersize") == 0 )
            {
              driver_minimum_cluster_size_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "disableidentify") == 0 )
            {
              enable_dont_identify_on_choose_source_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "piomode") == 0 )
            {
              pio_mode_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "enablerebuildassist") == 0 )
            {
              use_rebuild_assist_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "enableprocesschunk") == 0 )
            {
              enable_process_chunk_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "enablereadtwice") == 0 )
            {
              enable_read_twice_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "enableretryconnecting") == 0 )
            {
              enable_retry_connecting_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "enablescsiwrite") == 0 )
            {
              enable_scsi_write_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "sectorsizeadjust") == 0 )
            {
              output_sector_size_adjustment_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "ioscsionly") == 0 )
            {
              driver_io_scsi_only_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "usephysicalsectorsize") == 0 )
            {
              use_physical_sector_size_for_virtual_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "colorstatusbar") == 0 )
            {
              color_statusbar_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "primerelayactivatetime") == 0 )
            {
              primary_relay_activation_time_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "primerelaydelaytime") == 0 )
            {
              primary_relay_delay_time_ccc = strtoll(raw_data, NULL, 0);
            }
            else if ( strcmp(raw_command, "primerelaysettings") == 0 )
            {
              set_primary_relay_settings_ccc(strtoll(raw_data, NULL, 0));
            }

            else
            {
              fprintf (stdout, "Unrecognized config setting %s\n", raw_command);
            }

          }
        }
        else if (analyze_data)
        {
          if (analyze_line == 0)
          {
            analyze_line = 1;
          }
          else if (analyze_line == 1)
          {
            analyze_line = 2;
            char raw_data[MAX_LINE_LENGTH] = "";
            sscanf(line, "%[^\n]", raw_data);
            strcat (analyze_text_ccc, raw_data);
          }
          else
          {
            strcat (analyze_text_ccc, "\n");
            char raw_data[MAX_LINE_LENGTH] = "";
            sscanf(line, "%[^\n]", raw_data);
            strcat (analyze_text_ccc, raw_data);
          }
        }
        else if (smart_data)
        {
          if (smart_line == 0)
          {
            smart_line = 1;
          }
          else if (smart_line == 1)
          {
            smart_line = 2;
            char raw_data[MAX_LINE_LENGTH] = "";
            sscanf(line, "%[^\n]", raw_data);
            strcat (smart_data_text_ccc, raw_data);
          }
          else
          {
            strcat (smart_data_text_ccc, "\n");
            char raw_data[MAX_LINE_LENGTH] = "";
            sscanf(line, "%[^\n]", raw_data);
            strcat (smart_data_text_ccc, raw_data);
          }
        }
      }
    }

    // if used up allocated memory then increase it
    if (i >= log_rows_ccc)
    {
      increase_log_memory_ccc(1000);
    }

    if (found_error)
    {
      break;
    }
  }
  total_lines_ccc = i;
  if (found_error)
  {
    total_lines_ccc = 0;
  }

  fclose(readfile);
  return (found_error);
}





// function to read the domain file into memory
int read_domain_file_ccc(char *domain_file)
{
  if (domain_file == NULL)
  {
    sprintf (tempmessage_ccc, "Error: No domain file specified.\n");
    message_error_ccc(tempmessage_ccc);
    return (1);
  }

  if (verbose_ccc & DEBUG2)
  {
    sprintf (tempmessage_ccc, "Reading %s into memory...\n", domain_file);
    message_now_ccc(tempmessage_ccc);
  }

  FILE *readfile;
  readfile = fopen(domain_file, "r");
  if (readfile == NULL)
  {
    sprintf (tempmessage_ccc, "Cannot open %s for reading (%s).\n", domain_file, strerror(errno));
    message_error_ccc(tempmessage_ccc);
    return (1);
  }

  char line[MAX_LINE_LENGTH];
  int i = 0;
  int real_line_number = 0;
  int found_current = 0;
  int found_error = 0;
  while (fgets(line, sizeof line, readfile))
  {
    found_error = 0;
    real_line_number++;
    // process the line here
    if (verbose_ccc & DEBUG2)
    {
      sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
      message_now_ccc(tempmessage_ccc);
    }

    int scanline;
    int found_ddrescue = 0;
    int found_hddsuperclone = 0;
    long long temp_position = 0;
    long long temp_size = 0;
    unsigned char temp_status = 0;
    char raw_position[MAX_LINE_LENGTH];
    char raw_size[MAX_LINE_LENGTH];
    char raw_status[MAX_LINE_LENGTH];
    char raw_info[MAX_LINE_LENGTH];
    char raw_errstat[MAX_LINE_LENGTH];
    char rest_of_line[MAX_LINE_LENGTH];
    strcpy (raw_position, "");
    strcpy (raw_size, "");
    strcpy (raw_status, "");
    strcpy (rest_of_line, "");
    scanline = sscanf(line, "%s %s %s %s %s %[^\n]", raw_position, raw_size, raw_status, raw_info, raw_errstat, rest_of_line);
    // ignore blank lines
    if (scanline > 0)
    {
      // if the line doesn't start with a comment then process it
      if (raw_position[0] != '#')
      {
        // the first line should be the current position and status
        // new ddrescue format has copy pass added to first line
        if ( (!found_current && scanline == 2) || (!found_current && scanline == 3) )
        {
          found_current = 1;
        }

        // hddclone progress file format
        else if (scanline == 5)
        {
          found_hddsuperclone = 1;
          domain_dd_ccc = false;
          if (found_ddrescue)
          {
            sprintf (tempmessage_ccc, "domain format error on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
            message_now_ccc(tempmessage_ccc);
            found_error = -1;
          }
          char* endptr;
          dposition_ccc[i] = strtoull(raw_position, &endptr, 0);
          if (*endptr)
          {
            sprintf (tempmessage_ccc, "error processing domain position on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
            message_now_ccc(tempmessage_ccc);
            found_error = -1;
          }
          dsize_ccc[i] = strtoull(raw_size, &endptr, 0);
          if (*endptr)
          {
            sprintf (tempmessage_ccc, "error processing domain size on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
            message_now_ccc(tempmessage_ccc);
            found_error = -1;
          }
          dstatus_ccc[i] = strtoll(raw_status, &endptr, 0);
          if (*endptr)
          {
            sprintf (tempmessage_ccc, "error processing domain status on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
            message_now_ccc(tempmessage_ccc);
            found_error = -1;
          }
          dstatus_ccc[i] += (strtoll(raw_info, &endptr, 0)) << 8;
          if (*endptr)
          {
            sprintf (tempmessage_ccc, "error processing domain info on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
            message_now_ccc(tempmessage_ccc);
            found_error = -1;
          }
          dstatus_ccc[i] += (strtoll(raw_errstat, &endptr, 0)) << 32;
          if (*endptr)
          {
            sprintf (tempmessage_ccc, "error processing domain err/stat on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
            message_now_ccc(tempmessage_ccc);
            found_error = -1;
          }
          sprintf (tempmessage_ccc, "%d=  %d  0x%08llx  0x%08llx  0x%08llx\n", real_line_number, i, lposition_ccc[i], lsize_ccc[i], lstatus_ccc[i]);    //debug
          message_debug_ccc(tempmessage_ccc, DEBUG14);
          i++;
        }

        //ddrescue log format
        else if (scanline == 3)
        {
          found_ddrescue = 1;
          domain_dd_ccc = true;
          if (found_hddsuperclone)
          {
            sprintf (tempmessage_ccc, "domain format error on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
            message_now_ccc(tempmessage_ccc);
            found_error = -1;
          }
          char* endptr;
          temp_position = strtoull(raw_position, &endptr, 0);
          if (*endptr)
          {
            sprintf (tempmessage_ccc, "error processing domain position on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
            message_now_ccc(tempmessage_ccc);
            found_error = -1;
          }
          temp_size = strtoull(raw_size, &endptr, 0);
          if (*endptr)
          {
            sprintf (tempmessage_ccc, "error processing domain size on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
            message_now_ccc(tempmessage_ccc);
            found_error = -1;
          }
          scanline = sscanf(raw_status, "%c %[^\n]", &temp_status, rest_of_line);
          if (scanline != 1)
          {
            sprintf (tempmessage_ccc, "error processing domain status on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            found_error = -1;
          }

          if (!found_error)
          {
            dposition_ccc[i] = temp_position / sector_size_ccc;
            if (temp_position % sector_size_ccc)
            {
              sprintf (tempmessage_ccc, "warning line %d, domain position 0x%llx is not dividable by sector size\n", real_line_number, temp_position);
              message_now_ccc(tempmessage_ccc);
              //fprintf (stdout, "line%d= %s", real_line_number, line);
              found_error += 1;
            }

            dsize_ccc[i] = temp_size / sector_size_ccc;
            if (temp_size % sector_size_ccc)
            {
              sprintf (tempmessage_ccc, "warning line %d, size 0x%llx is not dividable by sector size\n", real_line_number, temp_size);
              message_now_ccc(tempmessage_ccc);
              //fprintf (stdout, "line%d= %s", real_line_number, line);
              found_error += 2;
            }

            if (temp_status == '?')
            {
              dstatus_ccc[i] = NONTRIED;
            }
            else if (temp_status == '+')
            {
              dstatus_ccc[i] = FINISHED;
            }
            else if (temp_status == '*')
            {
              dstatus_ccc[i] = NONTRIMMED;
            }
            else if (temp_status == '/')
            {
              dstatus_ccc[i] = NONSCRAPED;
            }
            else if (temp_status == '-')
            {
              dstatus_ccc[i] = BAD;
            }
            else
            {
              sprintf (tempmessage_ccc, "error processing line %d, domain status not recognized\n", real_line_number);
              message_now_ccc(tempmessage_ccc);
              sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
              message_now_ccc(tempmessage_ccc);
              found_error = -1;
            }

            // if position did not align then increase position and also decrease size if it is not 0
            if ( (found_error & 1) )
            {
              dposition_ccc[i]++;
              if (dsize_ccc[i] > 0)
              {
                dsize_ccc[i]--;
              }
            }
            // if only size did not align then do nothing as it was already cut by the divide
          }

          sprintf (tempmessage_ccc, "%d=  %d  0x%08llx  0x%08llx  0x%08llx\n", real_line_number, i, lposition_ccc[i], lsize_ccc[i], lstatus_ccc[i]);    //debug
          message_debug_ccc(tempmessage_ccc, DEBUG14);
          i++;
        }

        else
        {
          sprintf (tempmessage_ccc, "error processing domain file line %d\n", real_line_number);
          message_now_ccc(tempmessage_ccc);
          sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
          message_now_ccc(tempmessage_ccc);
          found_error = -1;
        }
        found_current = 1;
      }
    }

    // if used up allocated memory then increase it
    if (i >= domain_rows_ccc)
    {
      increase_domain_memory_ccc(1000);
    }

    if (found_error < 0)
    {
      break;
    }
  }
  domain_lines_ccc = i;
  if (found_error < 0)
  {
    domain_lines_ccc = 0;
  }

  fclose(readfile);
  return (found_error);
}





// function to read a bytes file and add it to the current domain
int read_domain_add_file_ccc(char *add_file)
{
  if (add_file == NULL)
  {
    sprintf (tempmessage_ccc, "Error: No file specified.\n");
    message_error_ccc(tempmessage_ccc);
    return (1);
  }

  if (verbose_ccc & DEBUG2)
  {
    sprintf (tempmessage_ccc, "Reading %s into memory...\n", add_file);
    message_now_ccc(tempmessage_ccc);
  }

  FILE *readfile;
  readfile = fopen(add_file, "r");
  if (readfile == NULL)
  {
    sprintf (tempmessage_ccc, "Cannot open %s for reading (%s).\n", add_file, strerror(errno));
    message_error_ccc(tempmessage_ccc);
    return (1);
  }

  char line[MAX_LINE_LENGTH];
  int real_line_number = 0;
  int found_error = 0;
  while (fgets(line, sizeof line, readfile))
  {
    found_error = 0;
    real_line_number++;
    // process the line here
    if (verbose_ccc & DEBUG2)
    {
      sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
      message_now_ccc(tempmessage_ccc);
    }

    int scanline;
    long long temp_position = 0;
    long long temp_size = 0;
    char raw_position[MAX_LINE_LENGTH];
    char raw_size[MAX_LINE_LENGTH];
    char rest_of_line[MAX_LINE_LENGTH];
    strcpy (raw_position, "");
    strcpy (raw_size, "");
    strcpy (rest_of_line, "");
    scanline = sscanf(line, "%s %s %[^\n]", raw_position, raw_size, rest_of_line);
    // ignore blank lines
    if (scanline > 0)
    {
      if (scanline == 2)
      {
        int good = 1;
        char* endptr;
        temp_position = strtoull(raw_position, &endptr, 0);
        if (*endptr)
        {
          sprintf (tempmessage_ccc, "error processing position on line %d\n", real_line_number);
          message_now_ccc(tempmessage_ccc);
          sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
          message_now_ccc(tempmessage_ccc);
          found_error = -1;
          good = 0;
        }
        temp_size = strtoull(raw_size, &endptr, 0);
        if (*endptr)
        {
          sprintf (tempmessage_ccc, "error processing size on line %d\n", real_line_number);
          message_now_ccc(tempmessage_ccc);
          sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
          message_now_ccc(tempmessage_ccc);
          found_error = -1;
          good = 0;
        }
        if (good)
        {
          long long position = temp_position / sector_size_ccc;
          if (temp_position % sector_size_ccc)
          {
            position = position + 1;
          }
          long long size = temp_size / sector_size_ccc;
          if (temp_size % sector_size_ccc)
          {
            size = size + 1;
          }
          if (position + size > source_total_size_ccc)
          {
            sprintf (tempmessage_ccc, "end position greater than source size on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
            message_now_ccc(tempmessage_ccc);
            found_error = -1;
          }
          else if (position > 0 && size > 0)
          {
            int ret = add_to_domain_ccc(position, size);
            if (ret)
            {
              sprintf (tempmessage_ccc, "error adding to domain from line %d\n", real_line_number);
              message_now_ccc(tempmessage_ccc);
              sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
              message_now_ccc(tempmessage_ccc);
              found_error = -1;
            }
          }
        }
      }
      else
      {
        sprintf (tempmessage_ccc, "format error on line %d\n", real_line_number);
        message_now_ccc(tempmessage_ccc);
        sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
        message_now_ccc(tempmessage_ccc);
        found_error = -1;
      }
    }
  }

  fclose(readfile);
  return (found_error);
}





// function to import ddrescue log file
int read_ddrescue_log_ccc(char *log_file)
{
  if (log_file == NULL)
  {
    sprintf (tempmessage_ccc, "Error: No log file specified.\n");
    message_error_ccc(tempmessage_ccc);
    return (1);
  }

  if (verbose_ccc & DEBUG2)
  {
    sprintf (tempmessage_ccc, "Reading %s into memory...\n", log_file);
    message_now_ccc(tempmessage_ccc);
  }

  FILE *readfile;
  readfile = fopen(log_file, "r");
  if (readfile == NULL)
  {
    sprintf (tempmessage_ccc, "Cannot open %s for reading (%s).\n", log_file, strerror(errno));
    message_error_ccc(tempmessage_ccc);
    return (1);
  }

  char line[MAX_LINE_LENGTH];
  int i = 0;
  int real_line_number = 0;
  int found_current = 0;
  int found_error = 0;
  while (fgets(line, sizeof line, readfile))
  {
    found_error = 0;
    real_line_number++;
    // process the line here
    if (verbose_ccc & DEBUG2)
    {
      sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
      message_now_ccc(tempmessage_ccc);
    }

    int scanline;
    long long temp_position = 0;
    long long temp_size = 0;
    unsigned char temp_status = 0;
    char raw_position[MAX_LINE_LENGTH];
    char raw_size[MAX_LINE_LENGTH];
    char raw_status[MAX_LINE_LENGTH];
    char rest_of_line[MAX_LINE_LENGTH];
    strcpy (raw_position, "");
    strcpy (raw_size, "");
    strcpy (raw_status, "");
    strcpy (rest_of_line, "");
    scanline = sscanf(line, "%s %s %s %[^\n]", raw_position, raw_size, raw_status, rest_of_line);
    // ignore blank lines
    if (scanline > 0)
    {
      // if the line doesn't start with a comment then process it
      if (raw_position[0] != '#')
      {
        // the first line should be the current position and status
        if ( (!found_current && scanline == 2) || (!found_current && scanline == 3) )
        {
          char* endptr;
          temp_position = strtoull(raw_position, &endptr, 0);
          if (*endptr)
          {
            sprintf (tempmessage_ccc, "error processing position on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
            message_now_ccc(tempmessage_ccc);
            found_error = -1;
          }
          scanline = sscanf(raw_size, "%c %[^\n]", &temp_status, rest_of_line);
          if (scanline != 1)
          {
            sprintf (tempmessage_ccc, "error processing status on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
            message_now_ccc(tempmessage_ccc);
            found_error = -1;
          }
          found_current = 1;
          sprintf (tempmessage_ccc, "%d=  %d  0x%08llX  %c\n", real_line_number, i, temp_position, temp_status);    //debug
          message_debug_ccc(tempmessage_ccc, DEBUG14);
          if (!found_error)
          {
            current_position_ccc = temp_position / sector_size_ccc;
            if (temp_position % sector_size_ccc)
            {
              sprintf (tempmessage_ccc, "warning line %d, position 0x%llx is not dividable by sector size\n", real_line_number, temp_position);
              message_now_ccc(tempmessage_ccc);
              sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
              message_debug_ccc(tempmessage_ccc, DEBUG14);
              found_error = 1;
            }
            if (temp_status == '?')
            {
              current_status_ccc = NONTRIED;
            }
            else if (temp_status == '+')
            {
              current_status_ccc = FINISHED;
            }
            else if (temp_status == '*')
            {
              current_status_ccc = NONTRIMMED;
            }
            else if (temp_status == '/')
            {
              current_status_ccc = NONSCRAPED;
            }
            else if (temp_status == '-')
            {
              current_status_ccc = BAD;
            }
            else
            {
              sprintf (tempmessage_ccc, "error processing line %d, status not recognized\n", real_line_number);
              message_now_ccc(tempmessage_ccc);
              sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
              message_now_ccc(tempmessage_ccc);
              found_error = -1;
            }
          }
        }
        else if (!found_current)
        {
          sprintf (tempmessage_ccc, "error processing progress log file line %d, expecting current status\n", real_line_number);
          message_now_ccc(tempmessage_ccc);
          sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
          message_now_ccc(tempmessage_ccc);
          found_error = -1;
        }
        else if (scanline != 3)
        {
          sprintf (tempmessage_ccc, "error processing progress log file line %d\n", real_line_number);
          message_now_ccc(tempmessage_ccc);
          sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
          message_now_ccc(tempmessage_ccc);
          found_error = -1;
        }
        else
        {
          char* endptr;
          temp_position = strtoull(raw_position, &endptr, 0);
          if (*endptr)
          {
            sprintf (tempmessage_ccc, "error processing position on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
            message_now_ccc(tempmessage_ccc);
            found_error = -1;
          }
          temp_size = strtoull(raw_size, &endptr, 0);
          if (*endptr)
          {
            sprintf (tempmessage_ccc, "error processing size on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
            message_now_ccc(tempmessage_ccc);
            found_error = -1;
          }
          scanline = sscanf(raw_status, "%c %[^\n]", &temp_status, rest_of_line);
          if (scanline != 1)
          {
            sprintf (tempmessage_ccc, "error processing status on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            found_error = -1;
          }

          if (!found_error)
          {
            lposition_ccc[i] = temp_position / sector_size_ccc;
            if (temp_position % sector_size_ccc)
            {
              sprintf (tempmessage_ccc, "warning line %d, position 0x%llx is not dividable by sector size\n", real_line_number, temp_position);
              message_now_ccc(tempmessage_ccc);
              //fprintf (stdout, "line%d= %s", real_line_number, line);
              found_error += 1;
            }

            lsize_ccc[i] = temp_size / sector_size_ccc;
            if (temp_size % sector_size_ccc)
            {
              sprintf (tempmessage_ccc, "warning line %d, size 0x%llx is not dividable by sector size\n", real_line_number, temp_size);
              message_now_ccc(tempmessage_ccc);
              //fprintf (stdout, "line%d= %s", real_line_number, line);
              found_error += 2;
            }

            if (temp_status == '?')
            {
              lstatus_ccc[i] = NONTRIED;
            }
            else if (temp_status == '+')
            {
              lstatus_ccc[i] = FINISHED;
            }
            else if (temp_status == '*')
            {
              lstatus_ccc[i] = NONTRIMMED;
            }
            else if (temp_status == '/')
            {
              lstatus_ccc[i] = NONSCRAPED;
            }
            else if (temp_status == '-')
            {
              lstatus_ccc[i] = BAD;
            }
            else
            {
              sprintf (tempmessage_ccc, "error processing line %d, status not recognized\n", real_line_number);
              message_now_ccc(tempmessage_ccc);
              sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
              message_now_ccc(tempmessage_ccc);
              found_error = -1;
            }

            // if position did not align then increase position and also decrease size if it is not 0
            if ( (found_error & 1) )
            {
              lposition_ccc[i]++;
              if (lsize_ccc[i] > 0)
              {
                lsize_ccc[i]--;
              }
            }
            // if only size did not align then do nothing as it was already cut by the divide
          }

          sprintf (tempmessage_ccc, "%d=  %d  0x%08llx  0x%08llx  0x%08llx\n", real_line_number, i, lposition_ccc[i], lsize_ccc[i], lstatus_ccc[i]);    //debug
          message_debug_ccc(tempmessage_ccc, DEBUG14);
          i++;
        }
      }
    }

    // if used up allocated memory then increase it
    if (i >= log_rows_ccc)
    {
      return_value_ccc = increase_log_memory_ccc(1000);
      if (return_value_ccc < 0)
      {
        found_error = return_value_ccc;;
      }
    }

    if (found_error < 0)
    {
      break;
    }
  }
  total_lines_ccc = i;
  if (found_error < 0)
  {
    total_lines_ccc = 0;
  }

  fclose(readfile);
  return (found_error);
}





// function to import ddrescue domain file
int read_ddrescue_domain_ccc(char *domain_file)
{
  if (domain_file == NULL)
  {
    sprintf (tempmessage_ccc, "Error: No domain file specified.\n");
    message_error_ccc(tempmessage_ccc);
    return (1);
  }

  if (verbose_ccc & DEBUG2)
  {
    sprintf (tempmessage_ccc, "Reading %s into memory...\n", domain_file);
    message_now_ccc(tempmessage_ccc);
  }

  FILE *readfile;
  readfile = fopen(domain_file, "r");
  if (readfile == NULL)
  {
    sprintf (tempmessage_ccc, "Cannot open %s for reading (%s).\n", domain_file, strerror(errno));
    message_error_ccc(tempmessage_ccc);
    return (1);
  }

  char line[MAX_LINE_LENGTH];
  int i = 0;
  int real_line_number = 0;
  int found_current = 0;
  int found_error = 0;
  while (fgets(line, sizeof line, readfile))
  {
    found_error = 0;
    real_line_number++;
    // process the line here
    if (verbose_ccc & DEBUG2)
    {
      sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
      message_now_ccc(tempmessage_ccc);
    }

    int scanline;
    long long temp_position = 0;
    long long temp_size = 0;
    unsigned char temp_status = 0;
    char raw_position[MAX_LINE_LENGTH];
    char raw_size[MAX_LINE_LENGTH];
    char raw_status[MAX_LINE_LENGTH];
    char rest_of_line[MAX_LINE_LENGTH];
    strcpy (raw_position, "");
    strcpy (raw_size, "");
    strcpy (raw_status, "");
    strcpy (rest_of_line, "");
    scanline = sscanf(line, "%s %s %s %[^\n]", raw_position, raw_size, raw_status, rest_of_line);
    // ignore blank lines
    if (scanline > 0)
    {
      // if the line doesn't start with a comment then process it
      if (raw_position[0] != '#')
      {
        // the first line should be the current position and status
        if (!found_current && scanline == 2)
        {
          found_current = 1;
        }
        else if (scanline != 3)
        {
          sprintf (tempmessage_ccc, "error processing domain file line %d\n", real_line_number);
          message_now_ccc(tempmessage_ccc);
          sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
          message_now_ccc(tempmessage_ccc);
          found_error = -1;
        }
        else
        {
          char* endptr;
          temp_position = strtoull(raw_position, &endptr, 0);
          if (*endptr)
          {
            sprintf (tempmessage_ccc, "error processing domain position on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
            message_now_ccc(tempmessage_ccc);
            found_error = -1;
          }
          temp_size = strtoull(raw_size, &endptr, 0);
          if (*endptr)
          {
            sprintf (tempmessage_ccc, "error processing domain size on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
            message_now_ccc(tempmessage_ccc);
            found_error = -1;
          }
          scanline = sscanf(raw_status, "%c %[^\n]", &temp_status, rest_of_line);
          if (scanline != 1)
          {
            sprintf (tempmessage_ccc, "error processing domain status on line %d\n", real_line_number);
            message_now_ccc(tempmessage_ccc);
            found_error = -1;
          }

          if (!found_error)
          {
            dposition_ccc[i] = temp_position / sector_size_ccc;
            if (temp_position % sector_size_ccc)
            {
              sprintf (tempmessage_ccc, "warning line %d, domain position 0x%llx is not dividable by sector size\n", real_line_number, temp_position);
              message_now_ccc(tempmessage_ccc);
              //fprintf (stdout, "line%d= %s", real_line_number, line);
              found_error += 1;
            }

            dsize_ccc[i] = temp_size / sector_size_ccc;
            if (temp_size % sector_size_ccc)
            {
              sprintf (tempmessage_ccc, "warning line %d, size 0x%llx is not dividable by sector size\n", real_line_number, temp_size);
              message_now_ccc(tempmessage_ccc);
              //fprintf (stdout, "line%d= %s", real_line_number, line);
              found_error += 2;
            }

            if (temp_status == '?')
            {
              dstatus_ccc[i] = NONTRIED;
            }
            else if (temp_status == '+')
            {
              dstatus_ccc[i] = FINISHED;
            }
            else if (temp_status == '*')
            {
              dstatus_ccc[i] = NONTRIMMED;
            }
            else if (temp_status == '/')
            {
              dstatus_ccc[i] = NONSCRAPED;
            }
            else if (temp_status == '-')
            {
              dstatus_ccc[i] = BAD;
            }
            else
            {
              sprintf (tempmessage_ccc, "error processing line %d, domain status not recognized\n", real_line_number);
              message_now_ccc(tempmessage_ccc);
              sprintf (tempmessage_ccc, "line%d= %s", real_line_number, line);
              message_now_ccc(tempmessage_ccc);
              found_error = -1;
            }

            // if position did not align then increase position and also decrease size if it is not 0
            if ( (found_error & 1) )
            {
              dposition_ccc[i]++;
              if (dsize_ccc[i] > 0)
              {
                dsize_ccc[i]--;
              }
            }
            // if only size did not align then do nothing as it was already cut by the divide
          }

          sprintf (tempmessage_ccc, "%d=  %d  0x%08llx  0x%08llx  0x%08llx\n", real_line_number, i, lposition_ccc[i], lsize_ccc[i], lstatus_ccc[i]);    //debug
          message_debug_ccc(tempmessage_ccc, DEBUG14);
          i++;
        }
        found_current = 1;
      }
    }

    // if used up allocated memory then increase it
    if (i >= domain_rows_ccc)
    {
      return_value_ccc = increase_domain_memory_ccc(1000);
      if (return_value_ccc < 0)
      {
        found_error = return_value_ccc;;
      }
    }

    if (found_error < 0)
    {
      break;
    }
  }
  domain_lines_ccc = i;
  if (found_error < 0)
  {
    domain_lines_ccc = 0;
  }

  fclose(readfile);
  return (found_error);
}





int increase_log_memory_ccc(int new_lines)
{
  int found_error = 0;
  log_rows_ccc += new_lines;
  temp_lposition_ccc = realloc(lposition_ccc, log_rows_ccc * sizeof(*lposition_ccc));
  if (temp_lposition_ccc == NULL)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGALLOCATEMEMERR]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, " (%s)", strerror(errno));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return GENERAL_ERROR_RETURN_CODE;
  }
  lposition_ccc = temp_lposition_ccc;

  temp_lsize_ccc = realloc(lsize_ccc, log_rows_ccc * sizeof(*lsize_ccc));
  if (temp_lsize_ccc == NULL)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGALLOCATEMEMERR]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, " (%s)", strerror(errno));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return GENERAL_ERROR_RETURN_CODE;
  }
  lsize_ccc = temp_lsize_ccc;

  temp_lstatus_ccc = realloc(lstatus_ccc, log_rows_ccc * sizeof(*lstatus_ccc));
  if (temp_lstatus_ccc == NULL)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGALLOCATEMEMERR]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, " (%s)", strerror(errno));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return GENERAL_ERROR_RETURN_CODE;
  }
  lstatus_ccc = temp_lstatus_ccc;
  return found_error;
}





int increase_domain_memory_ccc(int new_lines)
{
  int found_error = 0;
  domain_rows_ccc += new_lines;
  temp_dposition_ccc = realloc(dposition_ccc, domain_rows_ccc * sizeof(*dposition_ccc));
  if (temp_dposition_ccc == NULL)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGALLOCATEMEMERR]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, " (%s)", strerror(errno));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return GENERAL_ERROR_RETURN_CODE;
  }
  dposition_ccc = temp_dposition_ccc;

  temp_dsize_ccc = realloc(dsize_ccc, domain_rows_ccc * sizeof(*dsize_ccc));
  if (temp_dsize_ccc == NULL)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGALLOCATEMEMERR]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, " (%s)", strerror(errno));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return GENERAL_ERROR_RETURN_CODE;
  }
  dsize_ccc = temp_dsize_ccc;

  temp_dstatus_ccc = realloc(dstatus_ccc, domain_rows_ccc * sizeof(*dstatus_ccc));
  if (temp_dstatus_ccc == NULL)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGALLOCATEMEMERR]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, " (%s)", strerror(errno));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return GENERAL_ERROR_RETURN_CODE;
  }
  dstatus_ccc = temp_dstatus_ccc;
  return found_error;
}





int update_logfile_ccc(int time_sec)
{
  return_value_ccc = write_logfile_ccc(log_file_ccc, time_sec);
  if (return_value_ccc)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGLOGWRITEERR]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return LOGFILE_ERROR_RETURN_CODE;
  }
  return 0;
}





// function to write the log back to file after update
int write_logfile_ccc(char *log_file, int time_sec)
{
  long long new_time = get_elapsed_usec_ccc() / 1000000;
  if (new_time > last_log_write_ccc + time_sec || time_sec == 0)
  {
    last_log_write_ccc = new_time;
    sprintf (tempmessage_ccc, "Overwriting %s with updated...\n", log_file);    //debug
    message_debug_ccc(tempmessage_ccc, DEBUG2);

    // write a temp logfile so the original still exists if system crashes during log write
    char tmp_logfile[1024];
    strcpy(tmp_logfile, log_file);
    strcat(tmp_logfile, ".tmp");
    FILE *writefile;
    writefile = fopen(tmp_logfile, "w");
    if (writefile == NULL)
    {
      sprintf (tempmessage_ccc, "Cannot open %s for writing (%s).\n", tmp_logfile, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      return (1);
    }

    get_the_time_ccc();
    fprintf (writefile, "# Disk progress log file created by HDDSuperClone version %s\n", version_number_ccc);
    fprintf (writefile, "# %s\n", current_date_ccc);
    fprintf (writefile, "# command= %s\n", full_command_line_ccc);
    fprintf (writefile, "#\n");
    fprintf (writefile, "# ");
    unsigned int i;
    for (i = 0; i < sizeof(log_display_message_ccc); i++)
    {
      if (log_display_message_ccc[i] == '\0')
      {
        break;
      }
      fprintf (writefile, "%c", log_display_message_ccc[i]);
      if (log_display_message_ccc[i] == '\n')
      {
        fprintf (writefile, "# ");
      }
    }
    fprintf (writefile, "\n");
    fprintf (writefile, "#\n");
    fprintf (writefile, "################ START CONFIGURATION DATA ################\n");
    fprintf (writefile, "# startconfig\n");
    fprintf (writefile, "# logfile  \t%s\n", (log_file_ccc != NULL) ? log_file_ccc : "");
    fprintf (writefile, "# source  \t%s\n", (disk_1_ccc != NULL) ? disk_1_ccc : "");
    fprintf (writefile, "# destination  \t%s\n", (disk_2_ccc != NULL) ? disk_2_ccc : "");
    fprintf (writefile, "# verbose  \t0x%llx\n", verbose_ccc);
    fprintf (writefile, "# debug  \t0x%llx\n", debug_ccc);
    fprintf (writefile, "# forcemounted  \t%d\n", force_mounted_ccc);
    fprintf (writefile, "# forcedangerous  \t%d\n", force_danger_ccc);
    fprintf (writefile, "# importdd  \t%d\n", importdd_ccc);
    fprintf (writefile, "# exportdd  \t%d\n", exportdd_ccc);
    fprintf (writefile, "# dodomain  \t%d\n", do_domain_ccc);
    fprintf (writefile, "# domaindd  \t%d\n", domain_dd_ccc);
    fprintf (writefile, "# domainfile  \t%s\n", (domain_file_ccc != NULL) ? domain_file_ccc : "");
    fprintf (writefile, "# repairlog  \t%d\n", repair_log_ccc);
    fprintf (writefile, "# nologbackup  \t%d\n", no_log_backup_ccc);
    fprintf (writefile, "# phaselogs  \t%d\n", enable_logfile_phase_ccc);
    //fprintf (writefile, "# regaddr  \t0x%lx\n", reg_base_address_ccc);
    //fprintf (writefile, "# conaddr  \t0x%lx\n", control_base_address_ccc);
    //fprintf (writefile, "# busaddr  \t0x%lx\n", bus_base_address_ccc);
    //fprintf (writefile, "# device  \t%d\n", device_select_base_ccc);
    //fprintf (writefile, "# hbaaddr  \t0x%llx\n", hba_base_address_ccc);
    //fprintf (writefile, "# portnum  \t%d\n", port_number_base_ccc);
    //fprintf (writefile, "# portaddr  \t0x%llx\n", port_base_address_ccc);
    fprintf (writefile, "# softtimer  \t%lld\n", soft_reset_time_ccc);
    fprintf (writefile, "# hardtimer  \t%lld\n", hard_reset_time_ccc);
    fprintf (writefile, "# busytimer  \t%lld\n", busy_wait_time_ccc);
    fprintf (writefile, "# initbusytimer  \t%lld\n", initial_busy_wait_time_ccc);
    fprintf (writefile, "# resettimer  \t%lld\n", reset_wait_time_ccc); // this is also power timer
    fprintf (writefile, "# generaltimer  \t%lld\n", general_timeout_ccc);
    fprintf (writefile, "# phasetimers  \t%d\n", phase_timers_ccc);
    fprintf (writefile, "# p12softtimer  \t%lld\n", p12_soft_reset_time_ccc);
    fprintf (writefile, "# p3softtimer  \t%lld\n", p3_soft_reset_time_ccc);
    fprintf (writefile, "# p4softtimer  \t%lld\n", p4_soft_reset_time_ccc);
    fprintf (writefile, "# tdsofttimer  \t%lld\n", td_soft_reset_time_ccc);
    fprintf (writefile, "# d2softtimer  \t%lld\n", d2_soft_reset_time_ccc);
    fprintf (writefile, "# scsofttimer  \t%lld\n", sc_soft_reset_time_ccc);
    fprintf (writefile, "# rtsofttimer  \t%lld\n", rt_soft_reset_time_ccc);
    fprintf (writefile, "# alwayswaitresettime  \t%d\n", always_wait_for_reset_timers_ccc);
    fprintf (writefile, "# usbbulkreset  \t%d\n", usb_bulk_reset_enabled_ccc);
    fprintf (writefile, "# usbsoftreset  \t%d\n", usb_soft_reset_enabled_ccc);
    fprintf (writefile, "# usbhardreset  \t%d\n", usb_hard_reset_enabled_ccc);
    fprintf (writefile, "# usbportreset  \t%d\n", usb_port_reset_enabled_ccc);
    fprintf (writefile, "# imagesource  \t%d\n", copy_image_mode_ccc);
    fprintf (writefile, "# genericsource  \t%d\n", generic_mode_ccc);
    fprintf (writefile, "# atapass  \t%d\n", ata_passthrough_ccc);
    fprintf (writefile, "# scsipass  \t%d\n", scsi_passthrough_ccc);
    fprintf (writefile, "# skipfast  \t%d\n", skip_fast_ccc);
    fprintf (writefile, "# skiptimer  \t%lld\n", skip_timeout_ccc);
    fprintf (writefile, "# direct  \t%d\n", direct_mode_ccc);
    fprintf (writefile, "# ahci  \t%d\n", ahci_mode_ccc);
    fprintf (writefile, "# usb  \t%d\n", usb_mode_ccc);
    fprintf (writefile, "# nophase1  \t%d\n", no_phase1_ccc);
    fprintf (writefile, "# nophase2  \t%d\n", no_phase2_ccc);
    fprintf (writefile, "# nophase3 \t %d\n", no_phase3_ccc);
    fprintf (writefile, "# nophase4  \t%d\n", no_phase4_ccc);
    fprintf (writefile, "# notrim  \t%d\n", no_trim_ccc);
    fprintf (writefile, "# nodivide1  \t%d\n", no_divide1_ccc);
    fprintf (writefile, "# dodivide2  \t%d\n", do_divide2_ccc);
    fprintf (writefile, "# notrim  \t%d\n", no_trim_ccc);
    fprintf (writefile, "# noscrape  \t%d\n", no_scrape_ccc);
    fprintf (writefile, "# markbadhead  \t%d\n", mark_bad_head_ccc);
    fprintf (writefile, "# readbadhead  \t%d\n", read_bad_head_ccc);
    fprintf (writefile, "# rebuildassist  \t%d\n", rebuild_assist_ccc);
    fprintf (writefile, "# clustersize  \t%d\n", original_cluster_size_ccc);
    fprintf (writefile, "# blocksize  \t%d\n", block_size_ccc);
    fprintf (writefile, "# sectorsize  \t%d\n", sector_size_ccc);
    fprintf (writefile, "# blockoffset  \t%d\n", block_offset_ccc);
    fprintf (writefile, "# maxreadrate  \t%lld\n", max_read_rate_ccc);
    fprintf (writefile, "# retries  \t%d\n", retries_ccc);
    fprintf (writefile, "# retried  \t%lld\n", retried_lba_count_ccc);
    fprintf (writefile, "# inputoffset  \t%lld\n", input_offset_ccc);
    fprintf (writefile, "# outputoffset  \t%lld\n", output_offset_ccc);
    fprintf (writefile, "# size  \t%lld\n", read_size_ccc);
    fprintf (writefile, "# rateskip  \t%lld\n", rate_skip_ccc);
    fprintf (writefile, "# exitonslow  \t%lld\n", exit_on_slow_ccc);
    fprintf (writefile, "# exitonslowtime  \t%lld\n", exit_slow_time_ccc);
    fprintf (writefile, "# minskip \t %lld\n", original_min_skip_size_ccc);
    fprintf (writefile, "# maxskip  \t%lld\n", original_max_skip_size_ccc);
    fprintf (writefile, "# reverse \t%d\n", reverse_ccc);
    fprintf (writefile, "# fillmode  \t%d\n", fill_mode_ccc);
    fprintf (writefile, "# fillmark  \t%d\n", fill_mark_ccc);
    fprintf (writefile, "# findserial \t %s\n", (drive_serial_ccc != NULL) ? drive_serial_ccc : "");
    fprintf (writefile, "# quiet  \t%d\n", quiet_ccc);
    fprintf (writefile, "# forcestupid  \t%d\n", force_stupid_ccc);
    fprintf (writefile, "# model  \t%s\n", current_model_ccc);
    fprintf (writefile, "# serial  \t%s\n", current_serial_ccc);
    fprintf (writefile, "# skipsize  \t%lld\n", min_skip_size_ccc);
    fprintf (writefile, "# skips  \t%d\n", total_skip_count_ccc);
    fprintf (writefile, "# slowskips  \t%d\n", total_slow_skips_ccc);
    fprintf (writefile, "# skipruns  \t%d\n", total_skip_runs_ccc);
    fprintf (writefile, "# skipresets  \t%d\n", total_skip_resets_ccc);
    fprintf (writefile, "# stoponerror  \t%d\n", stop_on_error_ccc);
    fprintf (writefile, "# callcommand  \t%d\n", do_call_command_ccc);
    fprintf (writefile, "# commandtocall  \t%s\n", command_to_call_ccc);
    fprintf (writefile, "# primaryrelayonerror  \t%d\n", activate_primary_relay_on_error_ccc);
    fprintf (writefile, "# callpowercommand  \t%d\n", call_power_command_ccc);
    fprintf (writefile, "# powercommandtocall  \t%s\n", power_command_to_call_ccc);
    fprintf (writefile, "# primaryrelaypowercycle  \t%d\n", power_cycle_primary_relay_ccc);
    fprintf (writefile, "# writebufferdisable  \t%d\n", write_buffer_disable_ccc);
    fprintf (writefile, "# writebufferenable  \t%d\n", write_buffer_enable_ccc);
    fprintf (writefile, "# driverreturnerror  \t%d\n", driver_return_error_ccc);
    fprintf (writefile, "# driverreturnzeros  \t%d\n", driver_return_zeros_ccc);
    fprintf (writefile, "# driverreturnmarked  \t%d\n", driver_return_marked_ccc);
    fprintf (writefile, "# driverdiskname  \t%s\n", virtual_driver_name_ccc);
    fprintf (writefile, "# driverminclustersize  \t%lld\n", driver_minimum_cluster_size_ccc);
    fprintf (writefile, "# disableidentify  \t%d\n", enable_dont_identify_on_choose_source_ccc);
    fprintf (writefile, "# piomode  \t%d\n", pio_mode_ccc);
    fprintf (writefile, "# enablerebuildassist  \t%d\n", use_rebuild_assist_ccc);
    fprintf (writefile, "# enableprocesschunk  \t%d\n", enable_process_chunk_ccc);
    fprintf (writefile, "# enablereadtwice  \t%d\n", enable_read_twice_ccc);
    fprintf (writefile, "# enableretryconnecting \t%d\n", enable_retry_connecting_ccc);
    fprintf (writefile, "# enablescsiwrite  \t%d\n", enable_scsi_write_ccc);
    fprintf (writefile, "# sectorsizeadjust  \t%lld\n", output_sector_size_adjustment_ccc);
    fprintf (writefile, "# ioscsionly  \t%d\n", driver_io_scsi_only_ccc);
    fprintf (writefile, "# usephysicalsectorsize  \t%d\n", use_physical_sector_size_for_virtual_ccc);
    fprintf (writefile, "# colorstatusbar  \t%d\n", color_statusbar_ccc);
    fprintf (writefile, "# primerelayactivatetime  \t%lld\n", primary_relay_activation_time_ccc);
    fprintf (writefile, "# primerelaydelaytime  \t%lld\n", primary_relay_delay_time_ccc);
    fprintf (writefile, "# primerelaysettings  \t0x%04x\n", get_primary_relay_settings_ccc());
    fprintf (writefile, "# endconfig\n");
    fprintf (writefile, "################ END CONFIGURATION DATA ################\n");
    fprintf (writefile, "#\n");
    if (strcmp(analyze_text_ccc, "") != 0)
    {
      fprintf (writefile, "#\n");
      fprintf (writefile, "################ START ANALYZE DATA ################\n");
      fprintf (writefile, "# startanyalyze\n");
      fprintf (writefile, "%s\n", analyze_text_ccc);
      fprintf (writefile, "# endanalyze\n");
      fprintf (writefile, "################ END ANALYZE DATA ################\n");
      fprintf (writefile, "#\n");
    }
    if (strcmp(smart_data_text_ccc, "") != 0)
    {
      fprintf (writefile, "#\n");
      fprintf (writefile, "################ START SMART DATA ################\n");
      fprintf (writefile, "# startsmart\n");
      fprintf (writefile, "%s\n", smart_data_text_ccc);
      fprintf (writefile, "# endsmart\n");
      fprintf (writefile, "################ END SMART DATA ################\n");
      fprintf (writefile, "#\n");
    }
    if (!generic_mode_ccc)
    {
      fprintf (writefile, "#\n");
      fprintf (writefile, "################ START IDENTIFY DEVICE INFO ################\n");
      fprintf (writefile, "%s", identify_device_raw_text_ccc);
      fprintf (writefile, "################ END IDENTIFY DEVICE INFO ################\n");
      fprintf (writefile, "#\n");
    }
    fprintf (writefile, "#\n");
    fprintf (writefile, "# current position  \t \tstatus\n");
    fprintf (writefile, "0x%06llx        \t \t0x%llx\n", current_position_ccc, current_status_ccc);
    fprintf (writefile, "#\n");
    fprintf (writefile, "# position \tsize     \tstatus \tinfo \terr/status/time\n");
    int line_number;
    for (line_number = 0; line_number < total_lines_ccc; line_number++)
    {
      fprintf (writefile, "0x%06llx \t0x%06llx \t0x%llx \t0x%llx \t0x%llx\n", lposition_ccc[line_number], lsize_ccc[line_number], (lstatus_ccc[line_number] & STATUS_MASK), ((lstatus_ccc[line_number] & INFO_MASK) >> 8), (lstatus_ccc[line_number] >> 32) );
    }

    int error_ret = 0;
    if (fflush(writefile))
    {
      sprintf (tempmessage_ccc, "Flushing %s failed (%s).\n", tmp_logfile, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      error_ret = 1;
    }
    int fp = fileno(writefile);
    if (fsync(fp))
    {
      sprintf (tempmessage_ccc, "Syncing %s failed (%s).\n", tmp_logfile, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      error_ret = 1;
    }
    if (fclose(writefile))
    {
      sprintf (tempmessage_ccc, "Closing %s failed (%s).\n", tmp_logfile, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      error_ret = 1;
    }
    chmod(tmp_logfile, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (error_ret)
    {
      return (1);
    }

    //rename old logfile so it is not lost if system crashes
    if( !no_log_backup_ccc && !disable_log_backup_ccc && (access( log_file, F_OK ) != -1) )
    {
      char backup_logfile[1024];
      strcpy(backup_logfile, log_file);
      strcat(backup_logfile, ".bak");
      if (rename(log_file, backup_logfile) != 0)
      {
        sprintf (tempmessage_ccc, "Renaming %s to %s failed (%s).\n", log_file, backup_logfile, strerror(errno));
        message_error_ccc(tempmessage_ccc);
        return (1);
      }
    }

    //now rename the new logfile
    if (rename(tmp_logfile, log_file) != 0)
    {
      sprintf (tempmessage_ccc, "Renaming %s to %s failed (%s).\n", tmp_logfile, log_file, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      return (1);
    }

    if (exportdd_ccc)
    {
      // write a ddrescue logfile
      return_value_ccc = write_ddrescue_logfile_ccc(ddelog_file_ccc);
      if (return_value_ccc)
      {
        sprintf (tempmessage_ccc, "Error writing ddrescue mapfile\n");
        message_error_ccc(tempmessage_ccc);
        return (1);
      }
    }
    logfile_changed_ccc = false;
  }
  return (0);
}





int update_domainfile_ccc(int time_sec)
{
  // if the domain is null then skip writing it
  if (domain_file_ccc == NULL || (strcmp(domain_file_ccc, "") == 0) )
  {
    return 0;
  }
  else if (domainfile_changed_ccc)
  {
    return_value_ccc = write_domainfile_ccc(domain_file_ccc, time_sec);
    if (return_value_ccc)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGLOGWRITEERR]);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return LOGFILE_ERROR_RETURN_CODE;
    }
  }
  return 0;
}





// function to write the domain back to file after update
int write_domainfile_ccc(char *log_file, int time_sec)
{
  long long new_time = get_elapsed_usec_ccc() / 1000000;
  if (new_time > last_domain_write_ccc + time_sec || time_sec == 0)
  {
    last_domain_write_ccc = new_time;
    sprintf (tempmessage_ccc, "Overwriting %s with updated...\n", log_file);    //debug
    message_debug_ccc(tempmessage_ccc, DEBUG2);

    // write a temp logfile so the original still exists if system crashes during log write
    char tmp_logfile[1024];
    strcpy(tmp_logfile, log_file);
    strcat(tmp_logfile, ".tmp");
    FILE *writefile;
    writefile = fopen(tmp_logfile, "w");
    if (writefile == NULL)
    {
      sprintf (tempmessage_ccc, "Cannot open %s for writing (%s).\n", tmp_logfile, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      return (1);
    }

    get_the_time_ccc();
    fprintf (writefile, "# Domain file created by HDDSuperClone version %s\n", version_number_ccc);
    fprintf (writefile, "# %s\n", current_date_ccc);
    fprintf (writefile, "#\n");

    int line_number;
    for (line_number = 0; line_number < domain_lines_ccc; line_number++)
    {
      fprintf (writefile, "0x%06llx \t0x%06llx \t0x%llx \t0x%llx \t0x%llx\n", dposition_ccc[line_number], dsize_ccc[line_number], (dstatus_ccc[line_number] & STATUS_MASK), ((dstatus_ccc[line_number] & INFO_MASK) >> 8), (dstatus_ccc[line_number] >> 32) );
    }

    int error_ret = 0;
    if (fflush(writefile))
    {
      sprintf (tempmessage_ccc, "Flushing %s failed (%s).\n", tmp_logfile, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      error_ret = 1;
    }
    int fp = fileno(writefile);
    if (fsync(fp))
    {
      sprintf (tempmessage_ccc, "Syncing %s failed (%s).\n", tmp_logfile, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      error_ret = 1;
    }
    if (fclose(writefile))
    {
      sprintf (tempmessage_ccc, "Closing %s failed (%s).\n", tmp_logfile, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      error_ret = 1;
    }
    chmod(tmp_logfile, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (error_ret)
    {
      return (1);
    }

    //rename old logfile so it is not lost if system crashes
    if( !no_log_backup_ccc && (access( log_file, F_OK ) != -1) )
    {
      char backup_logfile[1024];
      strcpy(backup_logfile, log_file);
      strcat(backup_logfile, ".bak");
      if (rename(log_file, backup_logfile) != 0)
      {
        sprintf (tempmessage_ccc, "Renaming %s to %s failed (%s).\n", log_file, backup_logfile, strerror(errno));
        message_error_ccc(tempmessage_ccc);
        return (1);
      }
    }

    //now rename the new logfile
    if (rename(tmp_logfile, log_file) != 0)
    {
      sprintf (tempmessage_ccc, "Renaming %s to %s failed (%s).\n", tmp_logfile, log_file, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      return (1);
    }

    domainfile_changed_ccc = false;
  }
  return (0);
}





// function to export to a ddrescue style log
int write_ddrescue_logfile_ccc(char *log_file)
{
  sprintf (tempmessage_ccc, "Exporting to ddresce style log %s...\n", log_file);    //debug
  message_debug_ccc(tempmessage_ccc, DEBUG2);

  FILE *writefile;
  writefile = fopen(log_file, "w");
  if (writefile == NULL)
  {
    sprintf (tempmessage_ccc, "Cannot open %s for writing (%s).\n", log_file, strerror(errno));
    message_error_ccc(tempmessage_ccc);
    return (1);
  }

  get_the_time_ccc();
  fprintf (writefile, "# Disk progress log file created by HDDSuperClone version %s\n", version_number_ccc);
  fprintf (writefile, "# ddrescue export\n");
  fprintf (writefile, "# %s\n", current_date_ccc);
  fprintf (writefile, "# current_pos  current_status\n");
  char new_status;
  int temp_status = current_status_ccc & STATUS_MASK;
  if (temp_status == NONTRIED)
  {
    new_status = '?';
  }
  else if (temp_status == FINISHED)
  {
    new_status = '+';
  }
  else if (temp_status == TRIMMING || temp_status == DIVIDING1 || temp_status == DIVIDING2)
  {
    new_status = '*';
  }
  else if (temp_status == SCRAPING)
  {
    new_status = '/';
  }
  else if (temp_status == RETRYING)
  {
    new_status = '-';
  }
  // if it doesn't match anything then default to untried
  else
  {
    new_status = '?';
  }
  fprintf (writefile, "0x%08llX     %c\n", current_position_ccc * sector_size_ccc, new_status);
  fprintf (writefile, "#      pos        size  status\n");
  int line_number;
  for (line_number = 0; line_number < total_lines_ccc; line_number++)
  {
    temp_status = lstatus_ccc[line_number] & EXPORT_STATUS_MASK;
    if (temp_status == (NONTRIED & EXPORT_STATUS_MASK))
    {
      new_status = '?';
    }
    else if (temp_status == (FINISHED & EXPORT_STATUS_MASK))
    {
      new_status = '+';
    }
    else if (temp_status == (NONTRIMMED & EXPORT_STATUS_MASK) || temp_status == (NONDIVIDED & EXPORT_STATUS_MASK))
    {
      new_status = '*';
    }
    else if (temp_status == (NONSCRAPED & EXPORT_STATUS_MASK))
    {
      new_status = '/';
    }
    else if (temp_status == (BAD & EXPORT_STATUS_MASK))
    {
      new_status = '-';
    }
    // if it doesn't match anything then default to untried
    else
    {
      new_status = '?';
    }
    fprintf (writefile, "0x%08llX  0x%08llX  %c\n", lposition_ccc[line_number] * sector_size_ccc, lsize_ccc[line_number] * sector_size_ccc, new_status);
  }

  int error_ret = 0;
  if (fflush(writefile))
  {
    sprintf (tempmessage_ccc, "Flushing %s failed (%s).\n", log_file, strerror(errno));
    message_error_ccc(tempmessage_ccc);
    error_ret = 1;
  }
  int fp = fileno(writefile);
  if (fsync(fp))
  {
    sprintf (tempmessage_ccc, "Syncing %s failed (%s).\n", log_file, strerror(errno));
    message_error_ccc(tempmessage_ccc);
    error_ret = 1;
  }
  if (fclose(writefile))
  {
    sprintf (tempmessage_ccc, "Closing %s failed (%s).\n", log_file, strerror(errno));
    message_error_ccc(tempmessage_ccc);
    error_ret = 1;
  }
  chmod(log_file, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  if (error_ret)
  {
    return (1);
  }

  return (0);
}





void clear_variables_for_new_project_ccc (void)
{
  // TODO ?
  source_total_size_ccc = 0;
}





void initialize_new_log_ccc (void)
{
  current_position_ccc = 0;
  current_status_ccc = NONTRIED;
  lposition_ccc[0] = 0;
  lsize_ccc[0] = source_total_size_ccc;
  lstatus_ccc[0] = NONTRIED;
  total_lines_ccc = 1;

  // TODO ?
}





void initialize_domain_log_ccc (void)
{
  dposition_ccc[0] = 0;
  dsize_ccc[0] = source_total_size_ccc;
  dstatus_ccc[0] = NONTRIED;
  domain_lines_ccc = 1;

  int line_number;
      for (line_number = 0; line_number < domain_lines_ccc; line_number++)
      {
        fprintf (stdout, "0x%06llx \t0x%06llx \t0x%llx \t0x%llx \t0x%llx\n", dposition_ccc[line_number], dsize_ccc[line_number], (dstatus_ccc[line_number] & STATUS_MASK), ((dstatus_ccc[line_number] & INFO_MASK) >> 8), (dstatus_ccc[line_number] >> 32) );
      }
}





int check_log_ccc(void)
{
  sprintf (tempmessage_ccc, "Checking progress log file...\n");
  message_now_ccc(tempmessage_ccc);
  int i;
  int fail = 0;
  if (!importdd_ccc || repair_log_ccc)
  {
    // check that first position is 0
    if (lposition_ccc[0] != 0)
    {
      sprintf (tempmessage_ccc, "The first position is not 0\n");
      message_now_ccc(tempmessage_ccc);
      fail = fail | 1;
    }
  }
  for (i = 0; i < total_lines_ccc-1; i++)
  {
    // check if there is an overlap
    if ( (lposition_ccc[i] + lsize_ccc[i]) > lposition_ccc[i+1] )
    {
      sprintf (tempmessage_ccc, "Overlap found on line %d\n", i+1);
      message_now_ccc(tempmessage_ccc);
      fail = fail | 2;
    }
    // check if size is 0
    if (lsize_ccc[i] == 0)
    {
      sprintf (tempmessage_ccc, "Size of 0 found on line %d\n", i+1);
      message_now_ccc(tempmessage_ccc);
      fail = fail | 4;
    }
    // check if there is a gap
    if ( (lposition_ccc[i] + lsize_ccc[i]) < lposition_ccc[i+1])
    {
      sprintf (tempmessage_ccc, "Gap found on line %d\n", i+1);
      message_now_ccc(tempmessage_ccc);
      fail = fail | 8;
    }
    // check if the next status is the same
    if (lstatus_ccc[i] == lstatus_ccc[i+1])
    {
      sprintf (tempmessage_ccc, "Same status found on line %d\n", i+1);
      message_now_ccc(tempmessage_ccc);
      fail = fail | 0x10;
    }
  }
  if ( (!importdd_ccc || repair_log_ccc) && check_log_size_ccc)
  {
    // check that last position ends with total size
    if (lposition_ccc[total_lines_ccc-1] + lsize_ccc[total_lines_ccc-1] != source_total_size_ccc)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGWRONGDRIVEFORLOG1]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, "%lld", lposition_ccc[total_lines_ccc-1] + lsize_ccc[total_lines_ccc-1]);
      message_error_ccc(tempmessage_ccc);
      strcpy (tempmessage_ccc, curlang_ccc[LANGWRONGDRIVEFORLOG2]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, "%lld", source_total_size_ccc);
      message_error_ccc(tempmessage_ccc);
      strcpy (tempmessage_ccc, curlang_ccc[LANGWRONGDRIVEFORLOG3]);
      message_error_ccc(tempmessage_ccc);
      fail = fail | 0x20;
    }
  }
  return fail;
}





int check_domain_ccc(void)
{
  sprintf (tempmessage_ccc, "Checking domain file...\n");
  message_now_ccc(tempmessage_ccc);
  int i;
  int fail = 0;
  for (i = 0; i < domain_lines_ccc-1; i++)
  {
    // check if there is an overlap
    if ( (dposition_ccc[i] + dsize_ccc[i]) > dposition_ccc[i+1] )
    {
      sprintf (tempmessage_ccc, "Overlap found on domain line %d\n", i+1);
      message_now_ccc(tempmessage_ccc);
      fail = 2;
    }
  }
  return fail;
}





int write_logfile_phase_ccc(char *phase)
{
  if (enable_logfile_phase_ccc)
  {
    get_the_time_ccc();
    char tempname[2048];
    strncpy (tempname, log_file_ccc, 1024);
    strcat (tempname, ".");
    strcat (tempname, phase);
    strcat (tempname, ".");
    strncat (tempname, current_date_ccc, 512);
    bool no_log_backup_bak = no_log_backup_ccc;
    no_log_backup_ccc = true;
    return_value_ccc = write_logfile_ccc(tempname, 0);
    no_log_backup_ccc = no_log_backup_bak;
  }
  return 0;
}






int check_and_repair_log_ccc(void)
{
  sprintf (tempmessage_ccc, "Checking and repairing progress log file...\n");
  message_now_ccc(tempmessage_ccc);

  if (repair_log_ccc && !no_log_backup_ccc)
  {
    get_the_time_ccc();
    char tempname[2048];
    strncpy (tempname, log_file_ccc, 1024);
    strcat (tempname, ".bak.");
    strncat (tempname, current_date_ccc, 512);
    disable_log_backup_ccc = true;
    return_value_ccc = write_logfile_ccc(tempname, 0);
    disable_log_backup_ccc = false;
    if (return_value_ccc)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGLOGBAKWRITEERR]);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return LOGFILE_ERROR_RETURN_CODE;
    }
  }

  if (repair_log_ccc && check_log_size_ccc)
  {
    // check that first position is 0, if not insert line
    if (lposition_ccc[0] != 0)
    {
      sprintf (tempmessage_ccc, "The first position is not 0\n");    //debug
      message_debug_ccc(tempmessage_ccc, DEBUG15);
      insert_line_ccc(0, 0, lposition_ccc[1], NONTRIED);
    }

    // check that last position ends with total size, if less then add line, if more then chop
    if (lposition_ccc[total_lines_ccc-1] + lsize_ccc[total_lines_ccc-1] != source_total_size_ccc)
    {
      sprintf (tempmessage_ccc, "The end does not equal the drive size\n");    //debug
      message_debug_ccc(tempmessage_ccc, DEBUG15);
      long long block_end = lposition_ccc[total_lines_ccc-1] + lsize_ccc[total_lines_ccc-1];
      if (block_end < source_total_size_ccc)
      {
        long long difference = source_total_size_ccc - block_end;
        insert_line_ccc(total_lines_ccc, block_end, difference, NONTRIED);
      }
      else
      {
        int ret = find_block_ccc(source_total_size_ccc);
        lsize_ccc[ret] = source_total_size_ccc - lposition_ccc[ret];
        total_lines_ccc = ret+1;
      }
    }
  }

  int i;
  for (i = 0; i < total_lines_ccc-1; i++)
  {
    // check if there is an overlap and if so turn it into a gap
    long long block_end = lposition_ccc[i] + lsize_ccc[i];
    if (block_end > lposition_ccc[i+1])
    {
      sprintf (tempmessage_ccc, "Overlap found on line %d\n", i+1);    //debug
      message_debug_ccc(tempmessage_ccc, DEBUG15);
      long long difference = block_end - lposition_ccc[i+1];
      if (lsize_ccc[i] >= difference)
      {
        lsize_ccc[i] = lsize_ccc[i] - difference;
      }
      else
      {
        lsize_ccc[i] = 0;
      }
      if (difference <= lsize_ccc[i+1])
      {
        lposition_ccc[i+1] = lposition_ccc[i+1] + difference;
      }
    }
  }

  for (i = 0; i < total_lines_ccc; i++)
  {
    // check if size is 0 and if so delete the line
    if (lsize_ccc[i] == 0)
    {
      sprintf (tempmessage_ccc, "Size of 0 found on line %d\n", i+1);    //debug
      message_debug_ccc(tempmessage_ccc, DEBUG15);
      // delete line
      delete_line_ccc(i);
      // go back one line since it will be shifted forward
      if (i > 0)
      {
        i--;
      }
    }
  }

  for (i = 0; i < total_lines_ccc-1; i++)
  {
    int n;
    if (0)    // only enable this for debugging small log files, but not for public release
    {
      for (n = 0; n < total_lines_ccc; n++)
      {
        sprintf (tempmessage_ccc, "%d %d  0x%08llx  0x%08llx  0x%08llx\n", i, n, lposition_ccc[n], lsize_ccc[n], lstatus_ccc[n]);
        message_debug_ccc(tempmessage_ccc, DEBUG15);
      }
    }
    sprintf (tempmessage_ccc, "linec= %d\n", i);
    message_debug_ccc(tempmessage_ccc, DEBUG15);
    // check if there is a gap and if so fill with untried
    long long block_end = lposition_ccc[i] + lsize_ccc[i];
    if (block_end < lposition_ccc[i+1])
    {
      sprintf (tempmessage_ccc, "Gap found on line %d\n", i+1);    //debug
      message_debug_ccc(tempmessage_ccc, DEBUG15);
      long long difference = lposition_ccc[i+1] - block_end;
      insert_line_ccc (i+1, block_end, difference, NONTRIED);
    }
    if (0)    // only enable this for debugging small log files, but not for public release
    {
      for (n = 0; n < total_lines_ccc; n++)
      {
        sprintf (tempmessage_ccc, "%d %d  0x%08llx  0x%08llx  0x%08llx\n", i, n, lposition_ccc[n], lsize_ccc[n], lstatus_ccc[n]);
        message_debug_ccc(tempmessage_ccc, DEBUG15);
      }
    }
    sprintf (tempmessage_ccc, "%d  0x%08llx  0x%08llx  0x%08llx\n", i, lposition_ccc[i], lsize_ccc[i], lstatus_ccc[i]);
    message_debug_ccc(tempmessage_ccc, DEBUG15);
  }

  for (i = 0; i < total_lines_ccc-1; i++)
  {
    // check if the next status is the same and if so merge
    if (lstatus_ccc[i] == lstatus_ccc[i+1])
    {
      sprintf (tempmessage_ccc, "Same status found on line %d\n", i+1);    //debug
      message_debug_ccc(tempmessage_ccc, DEBUG15);
      lsize_ccc[i] = lsize_ccc[i] + lsize_ccc[i+1];
      // delete the next line since we just merged it into the current line
      delete_line_ccc(i+1);
      // go back one line so modified line gets checked
      if (i > 0)
      {
        i--;
      }
    }
  }

  return (0);
}





int reset_log_status_ccc(void)
{
  sprintf (tempmessage_ccc, "Resetting the log\n");
  message_now_ccc(tempmessage_ccc);

  if (!no_log_backup_ccc)
  {
    get_the_time_ccc();
    char tempname[2048];
    strncpy (tempname, log_file_ccc, 1024);
    strcat (tempname, ".bak.");
    strncat (tempname, current_date_ccc, 512);
    disable_log_backup_ccc = true;
    write_logfile_ccc(tempname, 0);
    disable_log_backup_ccc = false;
  }

  current_position_ccc = 0;
  current_status_ccc = NONTRIED;
  int i;
  long long status_reset_mask = STATUS_MASK & BAD_HEAD_MASK;
  for (i = 0; i < total_lines_ccc; i++)
  {
    lstatus_ccc[i] = lstatus_ccc[i] & status_reset_mask;
    if (lstatus_ccc[i] != FINISHED)
    {
      lstatus_ccc[i] = NONTRIED;
    }
  }
  for (i = 0; i < total_lines_ccc-1; i++)
  {
    // check if the next status is the same and if so merge
    if (lstatus_ccc[i] == lstatus_ccc[i+1])
    {
      lsize_ccc[i] = lsize_ccc[i] + lsize_ccc[i+1];
      // delete the next line since we just merged it into the current line
      delete_line_ccc(i+1);
      // go back one line so modified line gets checked
      if (i > 0)
      {
        i--;
      }
    }
  }
  total_skip_count_ccc = 0;
  total_skip_resets_ccc = 0;
  total_skip_runs_ccc = 0;
  total_slow_skips_ccc = 0;
  return 0;
}





int complete_log_reset_ccc(void)
{
  current_position_ccc = 0;
  current_status_ccc = NONTRIED;
  lposition_ccc[0] = 0;
  lsize_ccc[0] = source_total_size_ccc;
  lstatus_ccc[0] = NONTRIED;
  total_lines_ccc = 1;
  return 0;
}





int set_initial_data_from_log_ccc(int newproject)
{
  start_position_ccc = lposition_ccc[0];
  end_position_ccc = lposition_ccc[total_lines_ccc - 1] + lsize_ccc[total_lines_ccc - 1];
  source_total_size_ccc = end_position_ccc - start_position_ccc;

  // set some variables
  if (newproject)
  {
    // TODO ?
    //cluster_size_ccc = DEFAULT_CLUSTER_SIZE;
    //block_size_ccc = DEFAULT_BLOCK_SIZE;
    //rate_skip_ccc = DEFAULT_RATE_SKIP;
    //skip_timeout_ccc = DEFAULT_SKIP_TIMEOUT * 1000;
    //exit_on_slow_ccc = DEFAULT_EXIT_ON_SLOW;
    total_skip_count_ccc = 0;
    total_slow_skips_ccc = 0;
    total_skip_runs_ccc = 0;
    total_skip_resets_ccc = 0;
    last_total_skip_size_ccc = 0;
    retried_lba_count_ccc = 0;
    input_offset_ccc = 0;
    output_offset_ccc = -1;
    read_size_ccc = -1;
    total_runtime_ccc = 0;
    longest_read_time_ccc = 0;
    max_longest_read_time_ccc = 0;
    rate_count_ccc = 0;
    output_sector_size_adjustment_ccc = 0;
    min_skip_size_ccc = original_min_skip_size_ccc;
    enable_process_chunk_ccc = true;
  }

  if (read_size_ccc == -1  || read_size_ccc > (source_total_size_ccc - input_offset_ccc))
  {
    read_size_ccc = source_total_size_ccc - input_offset_ccc;
  }

  cluster_size_ccc = original_cluster_size_ccc;
  if (cluster_size_ccc < block_size_ccc)
  {
    cluster_size_ccc = block_size_ccc;
  }
  cluster_size_ccc = block_align_ccc (cluster_size_ccc, 0);
  original_cluster_size_ccc = cluster_size_ccc;

  max_skip_size_ccc = original_max_skip_size_ccc;

  return 0;
}





int check_and_adjust_variables_after_connection_ccc(void)
{

  return 0;
}



int check_and_adjust_variables_after_choosing_source_ccc(int newproject)
{
  //fprintf (stdout, "check_and_adjust_variables_after_choosing_source_ccc\n");    //debug
  int max_cluster_size = check_buffer_limit_ccc();
  if (max_cluster_size < 0)
  {
    return -1;
  }
  if (original_cluster_size_ccc > max_cluster_size || cluster_size_ccc > max_cluster_size)
  {
    original_cluster_size_ccc = max_cluster_size;
    cluster_size_ccc = max_cluster_size;
  }

  if (newproject)
  {
    min_skip_size_ccc = DEFAULT_SKIP_SIZE;
    max_skip_size_ccc = MAX_SKIP_SIZE;
    if (min_skip_size_ccc > source_total_size_ccc / 1000)
    {
      min_skip_size_ccc = source_total_size_ccc / 1000;
    }
    if (max_skip_size_ccc > source_total_size_ccc / 100)
    {
      max_skip_size_ccc = source_total_size_ccc / 100;
    }
    original_max_skip_size_ccc = max_skip_size_ccc;
    if (min_skip_size_ccc > max_skip_size_ccc)
    {
      min_skip_size_ccc = max_skip_size_ccc;
    }
    original_min_skip_size_ccc = min_skip_size_ccc;
    skip_size_ccc = min_skip_size_ccc;
  }

  return 0;
}





int get_log_status_ccc(long long mask)
{
  long long startpos = start_position_ccc;
  long long endpos = end_position_ccc;
  if (driver_mode_ccc)
  {
    startpos = 0;
    endpos = source_total_size_ccc;
  }
  int start_line = find_block_ccc(startpos);
  int end_line = find_block_ccc(endpos-1);
  //TODO: find_block_ccc() returns -1 on error and is used to index multiple arrays
  //  until I can understand how best to propagate the error, just pretend
  //  everything is OK:
  if (start_line < 0) return 0;
  if (end_line < 0) return 0;
  long long start_size_adjust = startpos - lposition_ccc[start_line];
  long long end_size_adjust = (lposition_ccc[end_line] + lsize_ccc[end_line]) - endpos;
  //fprintf (stdout, "start line=%d start position=%llx end line=%d end position=%llx\n", start_line, startpos, end_line, endpos);    //debug
  //fprintf (stdout, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n");    //debug
  error_count_ccc = 0;
  nontried_count_ccc = 0;
  nontrimmed_count_ccc = 0;
  nondivided_count_ccc = 0;
  nonscraped_count_ccc = 0;
  finished_count_ccc = 0;
  bad_head_count_ccc = 0;
  other_count_ccc = 0;
  error_size_ccc = 0;
  nontried_size_ccc = 0;
  nontrimmed_size_ccc = 0;
  nondivided_size_ccc = 0;
  nonscraped_size_ccc = 0;
  finished_size_ccc = 0;
  bad_head_size_ccc = 0;
  other_size_ccc = 0;
  total_size_ccc = 0;
  if (start_line < 0 || end_line < 0)
  {
    return 0;
  }
  int n = 0;
  int i;
  for (i = start_line; i <= end_line; i++)
  {
    long long size_adjust = 0;
    if (i == start_line)
    {
      size_adjust = start_size_adjust;
    }
    else if (i == end_line)
    {
      size_adjust = end_size_adjust;
    }
    long long size = lsize_ccc[i] - size_adjust;

    // find all matching domain chunks and adjust size from them
    if (do_domain_ccc)
    {
      long long position = lposition_ccc[i];
      long long new_size = 0;
      while (n < domain_lines_ccc)
      {
        long long dposition = dposition_ccc[n];
        long long dsize = dsize_ccc[n];
        // if the domain is past the line then subtract a domain line and break
        if (dposition >= (position + size) )
        {
          if (n > 0)
          {
            n--;
          }
          break;
        }

        // only check if it is positive domain
        if (dstatus_ccc[n] == (domain_status_ccc & mask) )
        {
          // if line fits in domain then keep size and break
          if (position >= dposition && (position + size) <= (dposition + dsize) )
          {
            new_size += size;
            break;
          }
          // if domain fits in line then add domain size
          if (dposition >= position && (dposition + dsize) <= (position + size) )
          {
            new_size += dsize;
          }
          // else if domain is at start of line adjust and add size
          else if (dposition <= position && (dposition + dsize) > position && (dposition + dsize) < (position + size) )
          {
            new_size += (dposition + dsize) - position;
          }
          // else if domain is at end of line adjust and add size
          else if (dposition > position && dposition < (position + size) )
          {
            new_size += (position + size) - dposition;
          }
          // if we made it this far then no match in current domain
        }

        n++;
      }
      size = new_size;
    }

    if ( (lstatus_ccc[i] & mask) == (NONTRIED & mask) )
    {
      nontried_count_ccc++;
      nontried_size_ccc += size;
    }
    else if ( (lstatus_ccc[i] & mask) == (NONTRIMMED & mask) )
    {
      nontrimmed_count_ccc++;
      nontrimmed_size_ccc += size;
    }
    else if ( (lstatus_ccc[i] & mask) == (NONDIVIDED & mask) )
    {
      nondivided_count_ccc++;
      nondivided_size_ccc += size;
    }
    else if ( (lstatus_ccc[i] & mask) == (NONSCRAPED & mask) )
    {
      nonscraped_count_ccc++;
      nonscraped_size_ccc += size;
    }
    else if ( (lstatus_ccc[i] & mask) == (BAD & mask) )
    {
      error_count_ccc++;
      error_size_ccc += size;
    }
    else if ( (lstatus_ccc[i] & mask) == (FINISHED & mask) )
    {
      finished_count_ccc++;
      finished_size_ccc += size;
    }
    else
    {
      other_count_ccc++;
      other_size_ccc += size;
    }
    total_size_ccc += size;

    if (lstatus_ccc[i] & BAD_HEAD_STATUS)
    {
      bad_head_count_ccc++;
      bad_head_size_ccc += size;
    }
  }
  return 0;
}





int get_domain_status_ccc(long long mask)
{
  long long startpos = start_position_ccc;
  long long endpos = end_position_ccc;
  if (driver_mode_ccc)
  {
    startpos = 0;
    endpos = source_total_size_ccc;
  }
  derror_count_ccc = 0;
  dnontried_count_ccc = 0;
  dnontrimmed_count_ccc = 0;
  dnondivided_count_ccc = 0;
  dnonscraped_count_ccc = 0;
  dfinished_count_ccc = 0;
  dother_count_ccc = 0;
  derror_size_ccc = 0;
  dnontried_size_ccc = 0;
  dnontrimmed_size_ccc = 0;
  dnondivided_size_ccc = 0;
  dnonscraped_size_ccc = 0;
  dfinished_size_ccc = 0;
  dother_size_ccc = 0;
  int i;
  for (i = 0; i < domain_lines_ccc; i++)
  {
    //fprintf (stdout, "domain line %d\n", i);    //debug
    // if the domain is in the recovery area then process it
    if ( (dposition_ccc[i] >= startpos && dposition_ccc[i] <= endpos) || ( (dposition_ccc[i] + dsize_ccc[i]) <= endpos && (dposition_ccc[i] + dsize_ccc[i]) >= startpos ) )
    {
      //fprintf (stdout, "dposition %lld dsize %lld start %lld end %lld\n", dposition_ccc[i], dsize_ccc[i], startpos, endpos);    //debug
      //fprintf (stdout, "domain line %d status %llx\n", i, dstatus_ccc[i]);    //debug
      long long size_adjust = 0;
      // if the domain start is before the start position then adjust size
      if (dposition_ccc[i] < startpos)
      {
        //fprintf (stdout, "dposition1 %lld dsize %lld start %lld end %lld\n", dposition_ccc[i], dsize_ccc[i], startpos, endpos);    //debug
        size_adjust = startpos - dposition_ccc[i];
      }
      // if the domain end is after the end postion then adjust the size
      else if (dposition_ccc[i] + dsize_ccc[i] > endpos)
      {
        //fprintf (stdout, "dposition2 %lld dsize %lld start %lld end %lld\n", dposition_ccc[i], dsize_ccc[i], startpos, endpos);    //debug
        size_adjust = (dposition_ccc[i] + dsize_ccc[i]) - endpos;
      }
      long long size = dsize_ccc[i] - size_adjust;
      // sanity check
      if (size < 0)
      {
        //fprintf (stdout, "size %lld dsize %lld adjust %lld\n", size, dsize_ccc[i], size_adjust);    //debug
        size = 0;
      }

      if ( (dstatus_ccc[i] & mask) == (NONTRIED & mask) )
      {
        //fprintf (stdout, "nontried %d status %llx size %lld dsize %lld start %lld end %lld\n", i, dstatus_ccc[i], size, dsize_ccc[i], startpos, endpos);    //debug
        dnontried_count_ccc++;
        dnontried_size_ccc += size;
      }
      else if ( (dstatus_ccc[i] & mask) == (NONTRIMMED & mask) )
      {
        dnontrimmed_count_ccc++;
        dnontrimmed_size_ccc += size;
      }
      else if ( (dstatus_ccc[i] & mask) == (NONDIVIDED & mask) )
      {
        dnondivided_count_ccc++;
        dnondivided_size_ccc += size;
      }
      else if ( (dstatus_ccc[i] & mask) == (NONSCRAPED & mask) )
      {
        dnonscraped_count_ccc++;
        dnonscraped_size_ccc += size;
      }
      else if ( (dstatus_ccc[i] & mask) == (BAD & mask) )
      {
        derror_count_ccc++;
        derror_size_ccc += size;
      }
      else if ( (dstatus_ccc[i] & mask) == (FINISHED & mask) )
      {
        dfinished_count_ccc++;
        dfinished_size_ccc += size;
      }
      else
      {
        dother_count_ccc++;
        dother_size_ccc += size;
      }
    }
  }

  //fprintf (stdout, "derror_size_ccc=%lld\n", derror_size_ccc);    //debug
  //fprintf (stdout, "dnontried_size_ccc=%lld\n", dnontried_size_ccc);    //debug
  //fprintf (stdout, "dnontrimmed_size_ccc=%lld\n", dnontrimmed_size_ccc);    //debug
  //fprintf (stdout, "dnondivided_size_ccc=%lld\n", dnondivided_size_ccc);    //debug
  //fprintf (stdout, "dnonscraped_size_ccc=%lld\n", dnonscraped_size_ccc);    //debug
  //fprintf (stdout, "dfinished_size_ccc=%lld\n", dfinished_size_ccc);    //debug
  //fprintf (stdout, "dother_size_ccc=%lld\n", dother_size_ccc);    //debug
  //fprintf (stdout, "domain_status_ccc=%x\n", domain_status_ccc);    //debug
  switch (domain_status_ccc)
  {
    case FINISHED:
      total_domain_size_ccc = dfinished_size_ccc;
      break;

    case NONTRIED:
      total_domain_size_ccc = dnontried_size_ccc;
      break;

    case NONTRIMMED:
      total_domain_size_ccc = dnontrimmed_size_ccc;
      break;

    case NONDIVIDED:
      total_domain_size_ccc = dnondivided_size_ccc;
      break;

    case NONSCRAPED:
      total_domain_size_ccc = dnonscraped_size_ccc;
      break;

    case BAD:
      total_domain_size_ccc = derror_size_ccc;
      break;
  }

  return 0;
}





int do_fill_ccc(int status, long long mask)
{
  // TODO fix fill mode read size
  copy_start_time_ccc = get_elapsed_usec_ccc() / 1000;

  if (fill_erase_ccc)
  {
    if (open_confirmation_dialog_ccc (curlang_ccc[LANGCONFIRMERASEDESTINATION]))
    {
      long long destination_size = lseek( disk2_fd_ccc, 0, SEEK_END );
      if (destination_size == -1)
      {
        strcpy (tempmessage_ccc, curlang_ccc[LANGCANTGETSIZEDESTINATION]);
        message_error_ccc(tempmessage_ccc);
        sprintf(tempmessage_ccc, " %s (%s)", disk_2_ccc, strerror(errno));
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return OUTPUT_DEVICE_ERROR_RETURN_CODE;
      }
      else if (destination_size == 0)
      {
        destination_size = 0xffffffffffff;
      }
      copy_start_time_ccc = get_elapsed_usec_ccc() / 1000;

      //long long current_position_bak = current_position_ccc;
      //long long start_position_bak = start_position_ccc;
      //long long end_position_bak = end_position_ccc;
      //total_filled_ccc = 0;
      total_filled_ccc = current_position_ccc;
      ccc_main_buffer_size_ccc = original_cluster_size_ccc * sector_size_ccc;
      set_main_buffer_ccc();
      memset (ccc_buffer_ccc, 0, ccc_main_buffer_size_ccc);
      long long destination_sectors = destination_size / sector_size_ccc;
      fill_size_ccc = destination_sectors;
      //current_position_ccc = 0;
      start_position_ccc = 0;
      end_position_ccc = destination_sectors;
      source_total_size_ccc = destination_sectors;
      strcpy(current_status_string_ccc, curlang_ccc[LANGERASEING]);
      while (current_position_ccc < destination_sectors)
      {
        int write_size = original_cluster_size_ccc;
        if (write_size + current_position_ccc > destination_sectors)
        {
          write_size = destination_sectors - current_position_ccc;
        }
        int ret = write_chunk_ccc(current_position_ccc, write_size);
        if (ret)
        {
          // write error
          return ret;
        }

        total_filled_ccc += write_size;
        update_display_ccc(DISPLAY_UPDATE_TIME);
        if (stop_signal_ccc)
        {
          return STOP_SIGNAL_RETURN_CODE;
        }

        current_position_ccc += original_cluster_size_ccc;
      }

      update_display_ccc(0);
      strcpy (tempmessage_ccc, curlang_ccc[LANGRESCUEFINISHED]);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
      clear_error_message_ccc();

      //current_position_ccc = current_position_bak;
      //start_position_ccc = start_position_bak;
      //end_position_ccc = end_position_bak;
    }
    return 0;
  }



  if (current_status_ccc == FILLING)
  {
    start_position_ccc = current_position_ccc;
    if (start_position_ccc < input_offset_ccc)
    {
      start_position_ccc = input_offset_ccc;
    }
  }
  else
  {
    start_position_ccc = input_offset_ccc;
    total_filled_ccc = 0;
  }
  if (read_size_ccc < 0)
  {
    end_position_ccc = lposition_ccc[total_lines_ccc - 1] + lsize_ccc[total_lines_ccc - 1];
  }
  else
  {
    end_position_ccc = input_offset_ccc + read_size_ccc;
  }
  int start_line = find_block_ccc(start_position_ccc);
  long long start_size_adjust = start_position_ccc - lposition_ccc[start_line];
  int end_line = find_block_ccc(end_position_ccc-1);
  long long end_size_adjust = (lposition_ccc[end_line] + lsize_ccc[end_line]) - end_position_ccc;
  current_position_ccc = start_position_ccc;
  start_position_ccc = input_offset_ccc;
  current_status_ccc = FILLING;
  strcpy(current_status_string_ccc, curlang_ccc[LANGFILLING]);
  update_display_ccc(0);
  if (stop_signal_ccc)
  {
    return STOP_SIGNAL_RETURN_CODE;
  }

  int i;
  for (i = start_line; i <= end_line; i++)
  {
    long long position = lposition_ccc[i];
    long long size_adjust = 0;
    if (i == start_line)
    {
      size_adjust = start_size_adjust;
      position += size_adjust;
    }
    else if (i == end_line)
    {
      size_adjust = end_size_adjust;
    }
    long long size = lsize_ccc[i] - size_adjust;

    if ( (lstatus_ccc[i] & mask) != (status & mask) )
    {
     long long fill_size = size;
     long long fill_position = position;
     int write_size = cluster_size_ccc;
     int keep_filling = 1;
     while (keep_filling)
     {
       current_position_ccc = fill_position;
       current_status_ccc = FILLING;
       update_display_ccc(DISPLAY_UPDATE_TIME);
       if (stop_signal_ccc)
       {
         return STOP_SIGNAL_RETURN_CODE;
       }
       // sanity check
       if (fill_position > lposition_ccc[i] + lsize_ccc[i] || fill_position > end_position_ccc)
       {
         break;
       }
       // check for last fill
       if (fill_size <= cluster_size_ccc)
       {
         write_size = fill_size;
         keep_filling = 0;
       }

       ccc_main_buffer_size_ccc = write_size * sector_size_ccc;
       set_main_buffer_ccc();
       if (fill_mark_ccc)
       {
         // fill the sectors with marking data
         unsigned long long n = 0;
         while (n < ccc_main_buffer_size_ccc)
         {
           //uint64_t quad_word = 0xdeadbeefdeadbeef;
           uint64_t quad_word = 0xffeeeebbddaaeedd;
           memcpy(ccc_buffer_ccc + n, &quad_word, 8);
           n += 8;
         }
         char mark_message[64];
         char mark_status[16];

         if ((lstatus_ccc[i] & mask) == (NONTRIED & mask))
         {
           strcpy (mark_status, "NONTRIED");
         }
         else if ((lstatus_ccc[i] & mask) == (NONTRIMMED & mask))
         {
           strcpy (mark_status, "NONTRIMMED");
         }
         else if ((lstatus_ccc[i] & mask) == (NONDIVIDED & mask))
         {
           strcpy (mark_status, "NONDIVIDED");
         }
         else if ((lstatus_ccc[i] & mask) == (NONSCRAPED & mask))
         {
           strcpy (mark_status, "NONSCRAPED");
         }
         else if ((lstatus_ccc[i] & mask) == (BAD & mask))
         {
           strcpy (mark_status, "BAD");
         }
         else
         {
           strcpy (mark_status, "UNKNOWN");
         }

         n = 0;
         while (n < ccc_main_buffer_size_ccc)
         {
           sprintf (mark_message, "HDDSUPERFILLMARK %s 0x%06llx    ", mark_status, fill_position + (n / sector_size_ccc));
           memcpy(ccc_buffer_ccc + n, &mark_message, strlen(mark_message));
           n += sector_size_ccc;
         }

       }
       else
       {
         // zero fill the sectors
         memset (ccc_buffer_ccc, 0, ccc_main_buffer_size_ccc);
       }

       int ret = write_chunk_ccc(fill_position, write_size);
       if (ret)
       {
         // write error
         return ret;
       }
       total_filled_ccc += write_size;
       logfile_changed_ccc = true;

       fill_size -= cluster_size_ccc;
       fill_position += cluster_size_ccc;
     }
    }
  }
  current_status_ccc = FINISHED;
  strcpy(current_status_string_ccc, curlang_ccc[LANGFINISHED]);
  logfile_changed_ccc = true;
  update_display_ccc(0);
  strcpy (tempmessage_ccc, curlang_ccc[LANGRESCUEFINISHED]);
  message_error_ccc(tempmessage_ccc);
  print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
  clear_error_message_ccc();
  return 0;
}






int reset_rate_ccc(void)
{
  rate_count_ccc = 0;
  rate_history_count_ccc = 0;
  read_time_history_count_ccc = 0;
  int i;
  for (i = 0; i < 256; i++)
  {
    rate_history_ccc[i] = 0;
    modified_rate_history_ccc[i] = 0;
    read_time_history_ccc[i] = 0;
  }
  return 0;
}





int update_display_ccc(int time_ms)
{
  if (driver_mode_ccc && reset_driver_timers_ccc())
  {
    //fprintf (stdout, "driver timers reset failed (%s)\n", strerror(errno));    //debug
  }
  long long new_time = get_elapsed_usec_ccc() / 1000;
  if (new_time > last_display_update_ccc + time_ms || time_ms == 0)
  {
    long long difference = new_time - last_display_update_ccc;
    last_display_update_ccc = new_time;
    long long runtime = (new_time - copy_start_time_ccc) / 1000;
    if (copy_start_time_ccc == 0)
    {
      runtime = 0;
    }
    if (running_clone_ccc || running_analyze_ccc)
    {
      total_runtime_ccc = runtime;
    }
    else
    {
      runtime = total_runtime_ccc;
    }
    int days = runtime / 86400;
    int rdays = runtime % 86400;
    int hours = rdays / 3600;
    int rhours = rdays % 3600;
    int minutes = rhours / 60;
    int rminutes = rhours % 60;
    int seconds = rminutes;
    get_log_status_ccc(STATUS_MASK & BAD_HEAD_MASK);
    if (do_domain_ccc || driver_mode_ccc)
    {
      get_domain_status_ccc(STATUS_MASK & BAD_HEAD_MASK);
    }

    long long rate = 0;
    long long total_rate = 0;
    char modifier[8] = "B/s \0";
    char avg_modifier[8] = "B/s \0";
    char total_modifier[8] = "B/s \0";
    char skip_modifier[8] = "B/s \0";


    // only process the new rate if called with a timer
    if (time_ms > 0)
    {
      // prevent divide by zero error
      if (difference > 0)
      {
        if (fill_mode_ccc)
        {
          long long bytes_transfered = (total_filled_ccc - last_filled_value_ccc) * sector_size_ccc;
          rate = (bytes_transfered * 1000) / difference;
        }
        else
        {
          long long bytes_transfered = (finished_size_ccc - last_finished_value_ccc) * sector_size_ccc;
          rate = (bytes_transfered * 1000) / difference;
        }
      }

      if (rate_skip_ccc > 0 && rate < rate_skip_ccc)
      {
        rate_skip_count_ccc++;
      }
      else
      {
        rate_skip_count_ccc = 0;
      }
      if (exit_on_slow_ccc > 0 && rate < exit_on_slow_ccc)
      {
        if (exit_slow_time_ccc < 0)
        {
          exit_on_slow_count_ccc++;
        }
        else
        {
          exit_on_slow_timer_ccc += difference;
          if (exit_on_slow_timer_ccc > exit_slow_time_ccc)
          {
            exit_on_slow_count_ccc = EXIT_ON_SLOW_COUNT + 1;
          }
        }
      }
      else
      {
        exit_on_slow_count_ccc = 0;
        exit_on_slow_timer_ccc = 0;
      }
      int difference_count = difference / 1000;
      if (difference_count < 1)
      {
        difference_count = 1;
      }
      int i;
      for (i = 0; i < difference_count; i++)
      {
        rate_history_ccc[rate_history_count_ccc] = rate;
        modified_rate_history_ccc[rate_history_count_ccc] = rate + ( (rate_modifier_ccc*1000) / ( (difference > 0) ? (difference / difference_count) : 1000 ) );
        rate_history_count_ccc++;
        rate_history_count_ccc = rate_history_count_ccc & 0xff;
      }
      last_finished_value_ccc = finished_size_ccc;
      last_filled_value_ccc = total_filled_ccc;
      if (rate > 10000000)
      {
        rate = rate / 1000000;
        strcpy (modifier, "MB/s");
      }
      else if (rate > 10000)
      {
        rate = rate / 1000;
        strcpy (modifier, "KB/s");
      }

      if (rate_count_ccc < 256)
      {
        rate_count_ccc++;
      }
    }


    long long avergage_rate = 0;
    int i;
    for (i = 0; i < rate_count_ccc; i++)
    {
      avergage_rate += rate_history_ccc[i];
    }
    // prevent divide by zero error
    if (rate_count_ccc > 0)
    {
      avergage_rate = avergage_rate / rate_count_ccc;
    }
    if (avergage_rate > 10000000)
    {
      avergage_rate = avergage_rate / 1000000;
      strcpy (avg_modifier, "MB/s");
    }
    else if (avergage_rate > 10000)
    {
      avergage_rate = avergage_rate / 1000;
      strcpy (avg_modifier, "KB/s");
    }

    if (runtime > 0)
    {
      if (fill_mode_ccc)
      {
        long long total_bytes_transfered = total_filled_ccc * sector_size_ccc;
        total_rate = total_bytes_transfered / runtime;
      }
      else if (driver_mode_ccc)
      {
        long long total_bytes_transfered = driver_read_rate_size_ccc * sector_size_ccc;
        if (difference > 0)
        {
          total_rate = (total_bytes_transfered * 1000) / difference;
        }
        driver_read_rate_size_ccc = 0;
      }
      else
      {
        long long total_bytes_transfered = (finished_size_ccc - starting_finished_value_ccc) * sector_size_ccc;
        total_rate = total_bytes_transfered / runtime;
      }
    }
    if (total_rate > 10000000)
    {
      total_rate = total_rate / 1000000;
      strcpy (total_modifier, "MB/s");
    }
    else if (total_rate > 10000)
    {
      total_rate = total_rate / 1000;
      strcpy (total_modifier, "KB/s");
    }

    long long rate_skip_size = rate_skip_ccc;
    if (rate_skip_size > 10000000)
    {
      rate_skip_size = rate_skip_size / 1000000;
      strcpy (skip_modifier, "MB/s");
    }
    else if (rate_skip_size > 10000)
    {
      rate_skip_size = rate_skip_size / 1000;
      strcpy (skip_modifier, "KB/s");
    }

    float finished_percent = 0;
    float nontried_percent = 0;
    float nontrimmed_percent = 0;
    float nondivided_percent = 0;
    float nonscraped_percent = 0;
    float bad_percent = 0;
    float filled_percent = 0;
    // prevent divide by zero error
    if (total_size_ccc > 0)
    {
      finished_percent = 100.0f * finished_size_ccc / total_size_ccc;
      nontried_percent = 100.0f * nontried_size_ccc / total_size_ccc;
      nontrimmed_percent = 100.0f * nontrimmed_size_ccc / total_size_ccc;
      nondivided_percent = 100.0f * nondivided_size_ccc / total_size_ccc;
      nonscraped_percent = 100.0f * nonscraped_size_ccc / total_size_ccc;
      bad_percent = 100.0f * error_size_ccc / total_size_ccc;
    }
    if (fill_size_ccc > 0)
    {
      filled_percent = 100.0f * total_filled_ccc / fill_size_ccc;
    }

    if (!fill_erase_ccc)
    {
      fill_size_ccc = nontried_size_ccc + nontrimmed_size_ccc + nondivided_size_ccc + nonscraped_size_ccc + error_size_ccc;
    }

    if (limit_recovery_ccc && finished_percent >= 10.0f)
    {
      recovery_past_limit_ccc = 1;
    }



    long long modified_avergage_rate = 0;
    for (i = 0; i < rate_count_ccc; i++)
    {
      modified_avergage_rate += modified_rate_history_ccc[i];
    }
    // prevent divide by zero error
    if (rate_count_ccc > 0)
    {
      modified_avergage_rate = modified_avergage_rate / rate_count_ccc;
    }
    // prevent divide by zero error later
    modified_avergage_rate ++;


    long long adjustment = 0;
    if (0 && do_domain_ccc)
    {
      adjustment = read_size_ccc - total_domain_size_ccc;
      if (nontried_size_ccc - adjustment < 0)
      {
        adjustment = 0;
      }
    }
    long long retries_adjustment = 0;
    if (retries_ccc > 0)
    {
      retries_adjustment = (retries_ccc * retry_error_size_ccc) - retried_lba_count_ccc;
      if (retries_adjustment < 0)
      {
        retries_adjustment = 0;
      }
    }
    long long bytes_left_to_read = 0;
    if (fill_mode_ccc)
    {
      bytes_left_to_read = (fill_size_ccc - total_filled_ccc) * sector_size_ccc;
    }
    else
    {
      bytes_left_to_read = ( (nontried_size_ccc - adjustment) + (nontrimmed_size_ccc*4) + (nondivided_size_ccc*4) + nonscraped_size_ccc + retries_adjustment) * sector_size_ccc;
    }
    if (!read_bad_head_ccc && bytes_left_to_read > (bad_head_size_ccc* sector_size_ccc))
    {
      bytes_left_to_read = bytes_left_to_read - (bad_head_size_ccc* sector_size_ccc);
    }
    long long remaining_time = bytes_left_to_read / modified_avergage_rate;
    if (running_analyze_ccc)
    {
      remaining_time = (analyze_stoptime_ccc/1000) - runtime;
    }
    if (runtime == 0)
    {
      remaining_time = 0;
    }
    int remaining_days = remaining_time / 86400;
    int remaining_rdays = remaining_time % 86400;
    int remaining_hours = remaining_rdays / 3600;
    int remaining_rhours = remaining_rdays % 3600;
    int remaining_minutes = remaining_rhours / 60;
    int remaining_rminutes = remaining_rhours % 60;
    int remaining_seconds = remaining_rminutes;


    if (verbose_ccc & DEBUG4)
    {
      sprintf (tempmessage_ccc, "longest read time: %lld ms\n", longest_read_time_ccc / 1000);
      message_display_ccc(tempmessage_ccc);

    }
    if (longest_read_time_ccc > max_longest_read_time_ccc)
    {
      max_longest_read_time_ccc = longest_read_time_ccc;
    }
    read_time_history_ccc[read_time_history_count_ccc] = longest_read_time_ccc;
    read_time_history_count_ccc++;
    read_time_history_count_ccc = read_time_history_count_ccc & 0xff;
    recent_longest_read_time_ccc = 0;
    for (i = 0; i < 256; i++)
    {
      if (read_time_history_ccc[i] > recent_longest_read_time_ccc)
      {
        recent_longest_read_time_ccc = read_time_history_ccc[i];
      }
    }
    sprintf (tempmessage_ccc, "%lld ms / %lld ms / %lld ms", longest_read_time_ccc / 1000, recent_longest_read_time_ccc / 1000, max_longest_read_time_ccc / 1000);
    strncpy(display_output_ccc.longestreadtime, tempmessage_ccc, sizeof(display_output_ccc.longestreadtime)-1);
    sprintf (tempmessage_ccc, "Current/Recent/Longest %lld ms / %lld ms / %lld ms", longest_read_time_ccc / 1000, recent_longest_read_time_ccc / 1000, max_longest_read_time_ccc / 1000);
    message_display_ccc(tempmessage_ccc);
    longest_read_time_ccc = 0;

    sprintf (tempmessage_ccc, "\n");
    message_display_ccc(tempmessage_ccc);

    sprintf (tempmessage_ccc, "Logfile: %s\n", log_file_ccc);
    message_display_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, "%s ", log_file_ccc);
    strncpy(display_output_ccc.logfile, tempmessage_ccc, sizeof(display_output_ccc.logfile)-1);

    sprintf (tempmessage_ccc, "%s ", domain_file_ccc);
    strncpy(display_output_ccc.domainfile, tempmessage_ccc, sizeof(display_output_ccc.domainfile)-1);

    char data_preview[DISPLAY_BUFFER_SIZE * 8] = "";
    int offset = 0;
    int size = BUFFER_DISPLAY_COUNT * 16;
    for (i = 0; i < size ; i+=16)
    {
      //sprintf (tempmessage_ccc, "#");
      //message_display_ccc(tempmessage_ccc);
      unsigned char *c;
      int n;
      for (n=0; n < 16 && i+n < size; n++)
      {
        c = (unsigned char *)display_buffer_ccc+i+n+offset;
        sprintf (tempmessage_ccc, "%02x ", *c);
        message_display_ccc(tempmessage_ccc);
        strcat(data_preview, tempmessage_ccc);
      }
      sprintf (tempmessage_ccc, "   ");
      message_display_ccc(tempmessage_ccc);
      for (n=0; n < 16 && i+n < size; n++)
      {
        c = (unsigned char *)display_buffer_ccc+i+n+offset;
        sprintf (tempmessage_ccc, "%c", isprint(*c) ? *c : '.');
        message_display_ccc(tempmessage_ccc);
        strcat(data_preview, tempmessage_ccc);
      }
      sprintf (tempmessage_ccc, "\n");
      message_display_ccc(tempmessage_ccc);
      strcat(data_preview, tempmessage_ccc);
    }
    memset (display_buffer_ccc, 0, DISPLAY_BUFFER_SIZE);
    strncpy(display_output_ccc.datapreview, data_preview, sizeof(display_output_ccc.datapreview)-1);

    sprintf (tempmessage_ccc, "     Source: %10s (%s, %s)\n", disk_1_ccc, current_source_model_ccc, current_source_serial_ccc);
    message_display_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, "%s (%s, %s)", disk_1_ccc, current_source_model_ccc, current_source_serial_ccc);
    strncpy(display_output_ccc.source, tempmessage_ccc, sizeof(display_output_ccc.source)-1);
    sprintf (tempmessage_ccc, "#Destination: %10s (%s, %s)\n", disk_2_ccc, current_destination_model_ccc, current_destination_serial_ccc);
    message_display_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, "%s (%s, %s)", disk_2_ccc, current_destination_model_ccc, current_destination_serial_ccc);
    strncpy(display_output_ccc.destination, tempmessage_ccc, sizeof(display_output_ccc.destination)-1);

    sprintf (tempmessage_ccc, "%lld (%.2fGB)", source_total_size_ccc, (double)(source_total_size_ccc*sector_size_ccc) / 1000000000);
    strncpy(display_output_ccc.totallba, tempmessage_ccc, sizeof(display_output_ccc.totallba)-1);
    sprintf (tempmessage_ccc, "%lld (%.2fGB)", read_size_ccc, (double)(read_size_ccc*sector_size_ccc) / 1000000000);
    strncpy(display_output_ccc.lbatoread, tempmessage_ccc, sizeof(display_output_ccc.lbatoread)-1);
    sprintf (tempmessage_ccc, "%lld (%.2fGB)", read_size_ccc, (double)(read_size_ccc*sector_size_ccc) / 1000000000);
    strncpy(display_output_ccc.domainsize, tempmessage_ccc, sizeof(display_output_ccc.domainsize)-1);
    if (do_domain_ccc || driver_mode_ccc)
    {
      sprintf (tempmessage_ccc, "  Total LBA: %10lld        LBA to read: %10lld (%lld)\n", source_total_size_ccc , read_size_ccc, total_domain_size_ccc);
      message_display_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, "%lld (%.2fGB)", total_domain_size_ccc, (double)(total_domain_size_ccc*sector_size_ccc) / 1000000000);
      strncpy(display_output_ccc.domainsize, tempmessage_ccc, sizeof(display_output_ccc.domainsize)-1);
    }
    else if (fill_mode_ccc)
    {
      sprintf (tempmessage_ccc, "  Total LBA: %10lld        LBA to fill: %10lld\n", source_total_size_ccc , fill_size_ccc);
      message_display_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, "%lld (%.2fGB)", fill_size_ccc, (double)(read_size_ccc*sector_size_ccc) / 1000000000);
      strncpy(display_output_ccc.lbatoread, tempmessage_ccc, sizeof(display_output_ccc.lbatoread)-1);
    }
    else
    {
      sprintf (tempmessage_ccc, "  Total LBA: %10lld        LBA to read: %10lld\n", source_total_size_ccc , read_size_ccc);
      message_display_ccc(tempmessage_ccc);
    }

    sprintf (tempmessage_ccc, "   Run time: %1d:%02d:%02d:%02d        Remaining:%4d:%02d:%02d:%02d\n", days, hours, minutes, seconds, remaining_days, remaining_hours, remaining_minutes, remaining_seconds);
    message_display_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, "%1d:%02d:%02d:%02d", days, hours, minutes, seconds);
    strncpy(display_output_ccc.runtime, tempmessage_ccc, sizeof(display_output_ccc.runtime)-1);
    sprintf (tempmessage_ccc, "%1d:%02d:%02d:%02d", remaining_days, remaining_hours, remaining_minutes, remaining_seconds);
    strncpy(display_output_ccc.remainingtime, tempmessage_ccc, sizeof(display_output_ccc.remainingtime)-1);

    sprintf (tempmessage_ccc, "       Rate: %10lld %s   Recent: %lld %s  Total: %lld %s\n", rate, modifier, avergage_rate, avg_modifier, total_rate, total_modifier);
    message_display_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, "%lld %s", rate, modifier);
    strncpy(display_output_ccc.currentrate, tempmessage_ccc, sizeof(display_output_ccc.currentrate)-1);
    sprintf (tempmessage_ccc, "%lld %s", avergage_rate, avg_modifier);
    strncpy(display_output_ccc.recentrate, tempmessage_ccc, sizeof(display_output_ccc.recentrate)-1);
    sprintf (tempmessage_ccc, "%lld %s", total_rate, total_modifier);
    strncpy(display_output_ccc.totalrate, tempmessage_ccc, sizeof(display_output_ccc.totalrate)-1);

    strcpy(display_output_ccc.totalfilled, "");
    strcpy(display_output_ccc.retried, "");
    strcpy(display_output_ccc.retrypasses, "");
    if (driver_mode_ccc)
    {
      sprintf (tempmessage_ccc, "     Last read size: %lld, %lld, %lld\n", last_small_read_size_ccc, last_big_read_size_ccc, last_actual_read_size_ccc);
      message_display_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, "%lld, %lld, %lld", last_small_read_size_ccc, last_big_read_size_ccc, last_actual_read_size_ccc);
      strncpy(display_output_ccc.totalfilled, tempmessage_ccc, sizeof(display_output_ccc.totalfilled)-1);
    }
    if (running_analyze_ccc)
    {
      int total_read_attempts = 0;
      for (i = 0; i < MAXANALYZESECTIONS; i++)
      {
        total_read_attempts = total_read_attempts + analyze_read_attempts_ccc[i];
      }
      int total_good_reads = 0;
      for (i = 0; i < MAXANALYZESECTIONS; i++)
      {
        total_good_reads = total_good_reads + analyze_good_reads_ccc[i];
      }
      int total_bad_reads = 0;
      for (i = 0; i < MAXANALYZESECTIONS; i++)
      {
        total_bad_reads = total_bad_reads + analyze_bad_reads_ccc[i];
      }
      int total_timeouts = 0;
      for (i = 0; i < MAXANALYZESECTIONS; i++)
      {
        total_timeouts = total_timeouts + analyze_timeouts_ccc[i];
      }
      int total_slow_reads = 0;
      for (i = 0; i < MAXANALYZESECTIONS; i++)
      {
        total_slow_reads = total_slow_reads + analyze_slow_reads_ccc[i];
      }
      sprintf (tempmessage_ccc, "%d, %d, %d (%d), %d", total_read_attempts, total_good_reads, total_bad_reads, total_timeouts, total_slow_reads);
      strncpy(display_output_ccc.totalfilled, tempmessage_ccc, sizeof(display_output_ccc.totalfilled)-1);
      sprintf (tempmessage_ccc, "%d", analyze_slow_total_reads_ccc);
      strncpy(display_output_ccc.retried, tempmessage_ccc, sizeof(display_output_ccc.retried)-1);
    }
    if (fill_mode_ccc)
    {
      sprintf (tempmessage_ccc, "     Filled: %10lld (%f%%)\n", total_filled_ccc, filled_percent);
      message_display_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, "%lld (%f%%)", total_filled_ccc, filled_percent);
      strncpy(display_output_ccc.totalfilled, tempmessage_ccc, sizeof(display_output_ccc.totalfilled)-1);
    }
    else if (current_status_ccc == RETRYING)
    {
      sprintf (tempmessage_ccc, "    Retried: %10lld        Remaining retry passes: %d\n", retried_lba_count_ccc, retries_ccc);
      message_display_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, "%lld", retried_lba_count_ccc);
      strncpy(display_output_ccc.retried, tempmessage_ccc, sizeof(display_output_ccc.retried)-1);
      sprintf (tempmessage_ccc, "%d", retries_ccc);
      strncpy(display_output_ccc.retrypasses, tempmessage_ccc, sizeof(display_output_ccc.retrypasses)-1);
    }
    else
    {
      sprintf (tempmessage_ccc, "  Skip size: %10lld  Skips: %d  Slow: %d  Runs: %d  Resets: %d  Run size: %lld\n", min_skip_size_ccc, total_skip_count_ccc, total_slow_skips_ccc, total_skip_runs_ccc, total_skip_resets_ccc, last_total_skip_size_ccc);
      message_display_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, "%lld (%.2fMB)", min_skip_size_ccc, (double)(min_skip_size_ccc*sector_size_ccc) / 1000000);
      strncpy(display_output_ccc.skipsize, tempmessage_ccc, sizeof(display_output_ccc.skipsize)-1);
      sprintf (tempmessage_ccc, "%d", total_skip_count_ccc);
      strncpy(display_output_ccc.skips, tempmessage_ccc, sizeof(display_output_ccc.skips)-1);
      sprintf (tempmessage_ccc, "%d", total_slow_skips_ccc);
      strncpy(display_output_ccc.slowskips, tempmessage_ccc, sizeof(display_output_ccc.skips)-1);
      sprintf (tempmessage_ccc, "%d", total_skip_runs_ccc);
      strncpy(display_output_ccc.skipruns, tempmessage_ccc, sizeof(display_output_ccc.skipruns)-1);
      sprintf (tempmessage_ccc, "%d", total_skip_resets_ccc);
      strncpy(display_output_ccc.skipresets, tempmessage_ccc, sizeof(display_output_ccc.skipresets)-1);
      sprintf (tempmessage_ccc, "%lld (%.2fMB)", last_total_skip_size_ccc, (double)(last_total_skip_size_ccc*sector_size_ccc) / 1000000);
      strncpy(display_output_ccc.runsize, tempmessage_ccc, sizeof(display_output_ccc.runsize)-1);
    }
    //fprintf (stdout, "  Skip hist: %10lld   %lld   %lld   %lld   %lld    \n", skip_history_ccc[0], skip_history_ccc[1], skip_history_ccc[2], skip_history_ccc[3], skip_history_ccc[4]);
    sprintf (tempmessage_ccc, "   Position: %10lld        Status: %10s\n", current_position_ccc, current_status_string_ccc);
    message_display_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, "%lld (%.2fGB)", current_position_ccc, (double)(current_position_ccc*sector_size_ccc) / 1000000000);
    strncpy(display_output_ccc.currentposition, tempmessage_ccc, sizeof(display_output_ccc.currentposition)-1);
    sprintf (tempmessage_ccc, "%s", current_status_string_ccc);
    strncpy(display_output_ccc.currentstatus, tempmessage_ccc, sizeof(display_output_ccc.currentstatus)-1);

    sprintf (tempmessage_ccc, "   Finished: %10lld (%d areas %f%%)\n", finished_size_ccc, finished_count_ccc, finished_percent);
    message_display_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, "%lld (%.2fGB) (%d / %f%%)", finished_size_ccc, (double)(finished_size_ccc*sector_size_ccc) / 1000000000, finished_count_ccc, finished_percent);
    strncpy(display_output_ccc.finished, tempmessage_ccc, sizeof(display_output_ccc.finished)-1);
    sprintf (tempmessage_ccc, "  Non-tried: %10lld (%d areas %f%%)\n", nontried_size_ccc, nontried_count_ccc, nontried_percent);
    message_display_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, "%lld (%.2fGB) (%d / %f%%)", nontried_size_ccc, (double)(nontried_size_ccc*sector_size_ccc) / 1000000000, nontried_count_ccc, nontried_percent);
    strncpy(display_output_ccc.nontried, tempmessage_ccc, sizeof(display_output_ccc.nontried)-1);
    sprintf (tempmessage_ccc, "Non-trimmed: %10lld (%d areas %f%%)\n", nontrimmed_size_ccc, nontrimmed_count_ccc, nontrimmed_percent);
    message_display_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, "%lld (%.2fGB) (%d / %f%%)", nontrimmed_size_ccc, (double)(nontrimmed_size_ccc*sector_size_ccc) / 1000000000, nontrimmed_count_ccc, nontrimmed_percent);
    strncpy(display_output_ccc.nontrimmed, tempmessage_ccc, sizeof(display_output_ccc.nontrimmed)-1);
    sprintf (tempmessage_ccc, "Non-divided: %10lld (%d areas %f%%)\n", nondivided_size_ccc, nondivided_count_ccc, nondivided_percent);
    message_display_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, "%lld (%.2fGB) (%d / %f%%)", nondivided_size_ccc, (double)(nondivided_size_ccc*sector_size_ccc) / 1000000000, nondivided_count_ccc, nondivided_percent);
    strncpy(display_output_ccc.nondivided, tempmessage_ccc, sizeof(display_output_ccc.nondivided)-1);
    sprintf (tempmessage_ccc, "Non-scraped: %10lld (%d areas %f%%)\n", nonscraped_size_ccc, nonscraped_count_ccc, nonscraped_percent);
    message_display_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, "%lld (%.2fGB) (%d / %f%%)", nonscraped_size_ccc, (double)(nonscraped_size_ccc*sector_size_ccc) / 1000000000, nonscraped_count_ccc, nonscraped_percent);
    strncpy(display_output_ccc.nonscraped, tempmessage_ccc, sizeof(display_output_ccc.nonscraped)-1);
    sprintf (tempmessage_ccc, "        Bad: %10lld (%d areas %f%%)\n", error_size_ccc, error_count_ccc, bad_percent);
    message_display_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, "%lld (%.2fGB) (%d / %f%%)", error_size_ccc, (double)(error_size_ccc*sector_size_ccc) / 1000000000, error_count_ccc, bad_percent);
    strncpy(display_output_ccc.bad, tempmessage_ccc, sizeof(display_output_ccc.bad)-1);

    rate_modifier_ccc = 0;
    highest_skip_ccc = 0;

    strcpy (log_display_message_ccc, display_message_ccc);

    update_display_status_buttons_ccc(0);
    update_gui_display_ccc();
    strcpy (display_message_ccc, "\0");
  }
  return 0;
}





void update_display_status_buttons_ccc(int time_ms)
{
  if (superclone_ccc)
  {
    long long new_time = get_elapsed_usec_ccc() / 1000;
    if (new_time > last_display_status_update_ccc + time_ms || time_ms == 0)
    {
      //printf ("status button update\n");    //debug
      last_display_status_update_ccc = new_time;
      update_gui_status_buttons_ccc();
      if (driver_mode_ccc)
      {
        reset_driver_timers_ccc();
      }
    }
  }
}






int upline_ccc(int count)
{
  int i;
  for (i = 0; i < count; i++)
  {
    fprintf (stdout, "\x1b[A");
  }
  return 0;
}





int delete_line_ccc(int line)
{
  int i;
  for (i = line; i < total_lines_ccc; i++)
  {
    lposition_ccc[i] = lposition_ccc[i+1];
    lsize_ccc[i] = lsize_ccc[i+1];
    lstatus_ccc[i] = lstatus_ccc[i+1];
  }
  total_lines_ccc--;
  return (0);
}





int delete_domain_line_ccc(int line)
{
  int i;
  for (i = line; i < domain_lines_ccc; i++)
  {
    dposition_ccc[i] = dposition_ccc[i+1];
    dsize_ccc[i] = dsize_ccc[i+1];
    dstatus_ccc[i] = dstatus_ccc[i+1];
  }
  domain_lines_ccc--;
  return (0);
}





int insert_line_ccc(int line, long long position, long long size, long long status)
{
  if (total_lines_ccc+1 >= log_rows_ccc)
  {
    int ret = increase_log_memory_ccc(1000);
    if (ret)
    {
      return ret;
    }
  }
  int i;
  for (i = total_lines_ccc; i > line; i--)
  {
    lposition_ccc[i] = lposition_ccc[i-1];
    lsize_ccc[i] = lsize_ccc[i-1];
    lstatus_ccc[i] = lstatus_ccc[i-1];
  }
  lposition_ccc[i] = position;
  lsize_ccc[i] = size;
  lstatus_ccc[i] = status;
  total_lines_ccc++;
  return (0);
}





int insert_domain_line_ccc(int line, long long position, long long size, long long status)
{
  if (domain_lines_ccc+1 >= domain_rows_ccc)
  {
    int ret = increase_domain_memory_ccc(1000);
    if (ret)
    {
      return ret;
    }
  }
  int i;
  for (i = domain_lines_ccc; i > line; i--)
  {
    dposition_ccc[i] = dposition_ccc[i-1];
    dsize_ccc[i] = dsize_ccc[i-1];
    dstatus_ccc[i] = dstatus_ccc[i-1];
  }
  dposition_ccc[i] = position;
  dsize_ccc[i] = size;
  dstatus_ccc[i] = status;
  domain_lines_ccc++;
  return (0);
}





int find_block_ccc(long long position)
{
  int first = 0;
  int last = total_lines_ccc - 1;
  int middle = (first + last) / 2;
  while (first <= last)
  {
    if (position >= lposition_ccc[middle] && position < lposition_ccc[middle] + lsize_ccc[middle])
    {
      return (middle);
    }
    else if (position > lposition_ccc[middle])
    {
      first = middle + 1;
    }
    else
    {
      last = middle - 1;
    }
    middle = (first + last) / 2;
  }
  return (-1);
}





int find_domain_block_ccc(long long position)
{
  int first = 0;
  int last = domain_lines_ccc - 1;
  int middle = (first + last) / 2;
  while (first <= last)
  {
    //fprintf (stdout, "pos=0x%llx\n", position);
    if (position >= dposition_ccc[middle] && position < dposition_ccc[middle] + dsize_ccc[middle])
    {
      return (middle);
    }
    else if (position > dposition_ccc[middle])
    {
      first = middle + 1;
    }
    else
    {
      last = middle - 1;
    }
    middle = (first + last) / 2;
  }
  return (-1);
}





int find_next_block_ccc(int line, long long status, long long mask)
{
  int i;
  for (i = line+1; i < total_lines_ccc; i++)
  {
    if ( (lstatus_ccc[i] & mask) == (status & mask) )
    {
      return i;
    }
  }
  return (-1);
}





int find_previous_block_ccc(int line, long long status, long long mask)
{
  int i;
  for (i = line-1; i >= 0; i--)
  {
    if ( (lstatus_ccc[i] & mask) == (status & mask) )
    {
      return i;
    }
  }
  return (-1);
}





int change_chunk_status_ccc(long long position, long long size, long long status, long long mask)
{
  if (debug_ccc & DEBUG13)
  {
    fprintf (debug_file_ccc, "change_chunk_status 0x%06llx  0x%06llx  status=0x%llx  mask=0x%llx\n", position, size, status, mask);
  }
  int block = find_block_ccc(position);
  if (block == -1)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGBLOCKNOTFOUND]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, "\nposition=%06llx  size=0x%06llx  status=0x%llx  mask=0x%llx", position, size, status, mask);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return INTERNAL_ERROR_RETURN_CODE;
  }
  // make sure chunk fits within the block
  if ( position < lposition_ccc[block] || (position + size) > (lposition_ccc[block] + lsize_ccc[block]) )
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGNOTWITHINBLOCK]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, "\nposition=%06llx  size=0x%06llx  status=0x%llx  mask=0x%llx", position, size, status, mask);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return INTERNAL_ERROR_RETURN_CODE;
  }
  // check if already same status
  if ( (lstatus_ccc[block] & mask) == (status & mask) )
  {
    return 1;
  }

  // keep head status
  if ( (lstatus_ccc[block] & BAD_HEAD_STATUS) == BAD_HEAD_STATUS )
  {
    status = status | BAD_HEAD_STATUS;
  }

  // keep skip status
  if ( (lstatus_ccc[block] & SKIP_INFO_MASK) && !(status & SKIP_INFO_MASK) )
  {
    status = status | (lstatus_ccc[block] & SKIP_INFO_MASK);
  }

  critical_process_ccc = true;

  // if postion and size match the block then only need to change status
  if ( position == lposition_ccc[block] && size == lsize_ccc[block] )
  {
    lstatus_ccc[block] = status;
    if (block < total_lines_ccc - 1)
    {
      if ( (lstatus_ccc[block] & mask) == (lstatus_ccc[block+1] & mask) )
      {
        lposition_ccc[block+1] -= size;
        lsize_ccc[block+1] += size;
        delete_line_ccc(block);
      }
    }
    if (block > 0)
    {
      if ( (lstatus_ccc[block] & mask) == (lstatus_ccc[block-1] & mask) )
      {
        lsize_ccc[block-1] += lsize_ccc[block];
        delete_line_ccc(block);
      }
    }
    //return 0;
  }
  // if position matches then check line above and merge or insert new line
  else if (position == lposition_ccc[block])
  {
    if (block > 0 && (status & mask) == (lstatus_ccc[block-1] & mask) )
    {
      lsize_ccc[block-1] += size;
      lposition_ccc[block] += size;
      lsize_ccc[block] -= size;
      //return 0;
    }
    else
    {
      insert_line_ccc(block+1, position + size, lsize_ccc[block] - size, lstatus_ccc[block]);
      lposition_ccc[block] = position;
      lsize_ccc[block] = size;
      lstatus_ccc[block] = status;
      //return 0;
    }
  }
  // if end position matches then check line below and merge or insert new line
  else if (position + size == lposition_ccc[block] + lsize_ccc[block])
  {
    if (block < total_lines_ccc - 1 && (status & mask) == (lstatus_ccc[block+1] & mask) )
    {
      lposition_ccc[block+1] -= size;
      lsize_ccc[block+1] += size;
      lsize_ccc[block] -= size;
      //return 0;
    }
    else
    {
      insert_line_ccc(block, lposition_ccc[block], lsize_ccc[block] - size, lstatus_ccc[block]);
      lposition_ccc[block+1] = lposition_ccc[block] + lsize_ccc[block];
      lsize_ccc[block+1] = size;
      lstatus_ccc[block+1] = status;
      //return 0;
    }
  }
  // if chunk is in the middle of the block then insert 2 lines and adjust
  else
  {
    long long block_end = lposition_ccc[block] + lsize_ccc[block];
    insert_line_ccc(block+1, position + size, block_end - (position + size), lstatus_ccc[block]);
    insert_line_ccc(block, lposition_ccc[block], position - lposition_ccc[block], lstatus_ccc[block]);
    lposition_ccc[block+1] = position;
    lsize_ccc[block+1] = size;
    lstatus_ccc[block+1] = status;
  }
  critical_process_ccc = false;
  logfile_changed_ccc = true;
  return 0;
}





int add_to_domain_ccc(long long position, long long size)
{
  if (debug_ccc & DEBUG10)
  {
    fprintf (debug_file_ccc, "change_domain_chunk_status 0x%06llx  0x%06llx\n", position, size);
  }

  //fprintf (debug_file_ccc, "change_domain_chunk_status 0x%06llx  0x%06llx\n", position, size);
  int line_number;
  //fprintf (debug_file_ccc,"before\n");
  for (line_number = 0; line_number < domain_lines_ccc; line_number++)
  {
    //fprintf (debug_file_ccc, "0x%06llx \t0x%06llx \t0x%llx \t0x%llx \t0x%llx\n", dposition_ccc[line_number], dsize_ccc[line_number], (dstatus_ccc[line_number] & STATUS_MASK), ((dstatus_ccc[line_number] & INFO_MASK) >> 8), (dstatus_ccc[line_number] >> 32) );
  }

  int block;
  while (1)
  {
    //fprintf (debug_file_ccc,"while start position=%06llx  size=0x%06llx\n", position, size);
    block = find_domain_block_ccc(position);
    if (block == -1)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGDOMAINBLOCKNOTFOUND]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, "\nposition=%06llx  size=0x%06llx", position, size);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();

      //fprintf (debug_file_ccc,"error1 position=%06llx  size=0x%06llx\n", position, size);
      for (line_number = 0; line_number < domain_lines_ccc; line_number++)
      {
        //fprintf (debug_file_ccc, "0x%06llx \t0x%06llx \t0x%llx \t0x%llx \t0x%llx\n", dposition_ccc[line_number], dsize_ccc[line_number], (dstatus_ccc[line_number] & STATUS_MASK), ((dstatus_ccc[line_number] & INFO_MASK) >> 8), (dstatus_ccc[line_number] >> 32) );
      }

      return INTERNAL_ERROR_RETURN_CODE;
    }
    critical_process_ccc = true;
    // see if chunk fits within the block
    if ( position >= dposition_ccc[block] && (position + size) <= (dposition_ccc[block] + dsize_ccc[block]) )
    {
      // check if already finished status, if so then nothing to do
      // we are using status mask in case of imported domain file, but extra status will not be kept
      if ( (dstatus_ccc[block] & STATUS_MASK) == (FINISHED & STATUS_MASK) )
      {
        critical_process_ccc = false;
        return 0;
      }
      // it fits so break to allow that processing below
      else
      {
        break;
      }
    }
    else
    {
      // chunk is bigger than block, need to process
      // check if position matches
      if (position == dposition_ccc[block])
      {
        //fprintf (debug_file_ccc,"while1\n");
        // if position matches then we can mark the block, then move position forward to next block reduce size and process
        dstatus_ccc[block] = FINISHED;
        position += dsize_ccc[block];
        size -= dsize_ccc[block];
        for (line_number = 0; line_number < 6; line_number++)
        {
          //fprintf (debug_file_ccc, "0x%06llx \t0x%06llx \t0x%llx \t0x%llx \t0x%llx\n", dposition_ccc[line_number], dsize_ccc[line_number], (dstatus_ccc[line_number] & STATUS_MASK), ((dstatus_ccc[line_number] & INFO_MASK) >> 8), (dstatus_ccc[line_number] >> 32) );
        }

        // check for same status on next block and merge if needed
        if (block < domain_lines_ccc - 1)
        {
          //fprintf (debug_file_ccc,"while2\n");
          if ( (dstatus_ccc[block] & STATUS_MASK) == (dstatus_ccc[block+1] & STATUS_MASK) )
          {
            //fprintf (debug_file_ccc,"while3\n");
            dposition_ccc[block+1] -= dsize_ccc[block];
            dsize_ccc[block+1] += dsize_ccc[block];
            delete_domain_line_ccc(block);
            for (line_number = 0; line_number < 6; line_number++)
            {
              //fprintf (debug_file_ccc, "0x%06llx \t0x%06llx \t0x%llx \t0x%llx \t0x%llx\n", dposition_ccc[line_number], dsize_ccc[line_number], (dstatus_ccc[line_number] & STATUS_MASK), ((dstatus_ccc[line_number] & INFO_MASK) >> 8), (dstatus_ccc[line_number] >> 32) );
            }
          }
        }
        // check for same status on previous block and merge if needed
        if (block > 0 && (FINISHED & STATUS_MASK) == (dstatus_ccc[block-1] & STATUS_MASK) )
        {
          //fprintf (debug_file_ccc,"while4\n");
          dsize_ccc[block-1] += dsize_ccc[block];
          delete_domain_line_ccc(block);
          for (line_number = 0; line_number < 6; line_number++)
          {
            //fprintf (debug_file_ccc, "0x%06llx \t0x%06llx \t0x%llx \t0x%llx \t0x%llx\n", dposition_ccc[line_number], dsize_ccc[line_number], (dstatus_ccc[line_number] & STATUS_MASK), ((dstatus_ccc[line_number] & INFO_MASK) >> 8), (dstatus_ccc[line_number] >> 32) );
          }
        }
      }
      // check if position is in middle of the block
      else if (position > dposition_ccc[block])
      {
        //fprintf (debug_file_ccc,"while5\n");
        // check for same status on next block and merge if needed
        if (block < domain_lines_ccc - 1)
        {
          //fprintf (debug_file_ccc,"while6\n");
          if ( (FINISHED & STATUS_MASK) == (dstatus_ccc[block+1] & STATUS_MASK) )
          {
            //fprintf (debug_file_ccc,"while7\n");
            dsize_ccc[block+1] += dposition_ccc[block+1] - position;
            dposition_ccc[block+1] = position;
          }
        }
        // move position ahead reduce size and shorten size
        long long diff = (dposition_ccc[block] + dsize_ccc[block]) - position;
        //dsize_ccc[block] -= diff;
        position += diff;
        size -= diff;
        for (line_number = 0; line_number < 6; line_number++)
        {
          //fprintf (debug_file_ccc, "0x%06llx \t0x%06llx \t0x%llx \t0x%llx \t0x%llx\n", dposition_ccc[line_number], dsize_ccc[line_number], (dstatus_ccc[line_number] & STATUS_MASK), ((dstatus_ccc[line_number] & INFO_MASK) >> 8), (dstatus_ccc[line_number] >> 32) );
        }
      }
      // the position is before the block which should not be possible
      else
      {
        critical_process_ccc = false;
        strcpy (tempmessage_ccc, curlang_ccc[LANGDOMAINBLOCKNOTFOUND]);
        message_error_ccc(tempmessage_ccc);
        sprintf (tempmessage_ccc, "\nposition=%06llx  size=0x%06llx", position, size);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();

        //fprintf (debug_file_ccc,"error2 position=%06llx  size=0x%06llx\n", position, size);
        for (line_number = 0; line_number < domain_lines_ccc; line_number++)
        {
          //fprintf (debug_file_ccc, "0x%06llx \t0x%06llx \t0x%llx \t0x%llx \t0x%llx\n", dposition_ccc[line_number], dsize_ccc[line_number], (dstatus_ccc[line_number] & STATUS_MASK), ((dstatus_ccc[line_number] & INFO_MASK) >> 8), (dstatus_ccc[line_number] >> 32) );
        }

        return INTERNAL_ERROR_RETURN_CODE;
      }
    }
    //fprintf (debug_file_ccc,"while end position=%06llx  size=0x%06llx\n", position, size);
  }

  // the chunk fits in the block
  // if postion and size match the block then only need to change status
  if ( position == dposition_ccc[block] && size == dsize_ccc[block] )
  {
    dstatus_ccc[block] = FINISHED;
    if (block < domain_lines_ccc - 1)
    {
      if ( (dstatus_ccc[block] & STATUS_MASK) == (dstatus_ccc[block+1] & STATUS_MASK) )
      {
        dposition_ccc[block+1] -= size;
        dsize_ccc[block+1] += size;
        delete_domain_line_ccc(block);
      }
    }
    if (block > 0)
    {
      if ( (dstatus_ccc[block] & STATUS_MASK) == (dstatus_ccc[block-1] & STATUS_MASK) )
      {
        dsize_ccc[block-1] += dsize_ccc[block];
        delete_domain_line_ccc(block);
      }
    }
    //return 0;
  }
  // if position matches then check line above and merge or insert new line
  else if (position == dposition_ccc[block])
  {
    if (block > 0 && (FINISHED & STATUS_MASK) == (dstatus_ccc[block-1] & STATUS_MASK) )
    {
      dsize_ccc[block-1] += size;
      dposition_ccc[block] += size;
      dsize_ccc[block] -= size;
      //return 0;
    }
    else
    {
      insert_domain_line_ccc(block+1, position + size, dsize_ccc[block] - size, dstatus_ccc[block]);
      dposition_ccc[block] = position;
      dsize_ccc[block] = size;
      dstatus_ccc[block] = FINISHED;
      //return 0;
    }
  }
  // if end position matches then check line below and merge or insert new line
  else if (position + size == dposition_ccc[block] + dsize_ccc[block])
  {
    if (block < domain_lines_ccc - 1 && (FINISHED & STATUS_MASK) == (dstatus_ccc[block+1] & STATUS_MASK) )
    {
      dposition_ccc[block+1] -= size;
      dsize_ccc[block+1] += size;
      dsize_ccc[block] -= size;
      //return 0;
    }
    else
    {
      insert_domain_line_ccc(block, dposition_ccc[block], dsize_ccc[block] - size, dstatus_ccc[block]);
      dposition_ccc[block+1] = dposition_ccc[block] + dsize_ccc[block];
      dsize_ccc[block+1] = size;
      dstatus_ccc[block+1] = FINISHED;
      //return 0;
    }
  }
  // if chunk is in the middle of the block then insert 2 lines and adjust
  else
  {
    long long block_end = dposition_ccc[block] + dsize_ccc[block];
    insert_domain_line_ccc(block+1, position + size, block_end - (position + size), dstatus_ccc[block]);
    insert_domain_line_ccc(block, dposition_ccc[block], position - dposition_ccc[block], dstatus_ccc[block]);
    dposition_ccc[block+1] = position;
    dsize_ccc[block+1] = size;
    dstatus_ccc[block+1] = FINISHED;
  }

  //fprintf (debug_file_ccc,"after\n");
  for (line_number = 0; line_number < domain_lines_ccc; line_number++)
  {
    //fprintf (debug_file_ccc, "0x%06llx \t0x%06llx \t0x%llx \t0x%llx \t0x%llx\n", dposition_ccc[line_number], dsize_ccc[line_number], (dstatus_ccc[line_number] & STATUS_MASK), ((dstatus_ccc[line_number] & INFO_MASK) >> 8), (dstatus_ccc[line_number] >> 32) );
  }

  critical_process_ccc = false;
  domainfile_changed_ccc = true;
  return_value_ccc = update_domainfile_ccc(LOG_UPDATE_TIME);
  if (return_value_ccc)
  {
    return return_value_ccc;
  }
  return 0;
}





int clone_forward_ccc(void)
{
  int ret = 0;
  int rollover = 0;
  //fprintf (stdout, "currentpos= %lld currentstat=%lld\n", current_position_ccc, current_status_ccc);  //debug
  if (current_status_ccc == PHASE1 && rebuild_assist_enabled_ccc && rebuild_assist_ccc)
  {
    current_status_ccc = REBUILDASSIST;
  }
  else if (current_status_ccc == FINISHED && rebuild_assist_enabled_ccc)
  {
    current_status_ccc = PHASE1;
    current_position_ccc = start_position_ccc;
  }
  else if (current_status_ccc == FINISHED)
  {
    current_status_ccc = PHASE3;
    current_position_ccc = start_position_ccc;
  }
  //fprintf (stdout, "currentpos= %lld\n", current_position_ccc);  //debug

  // get start time in ms
  copy_start_time_ccc = get_elapsed_usec_ccc() / 1000;

  // set the starting data
  get_log_status_ccc(STATUS_MASK & BAD_HEAD_MASK);
  starting_finished_value_ccc = finished_size_ccc;
  last_finished_value_ccc = finished_size_ccc;

  update_display_ccc(0);
  if (stop_signal_ccc)
  {
    return STOP_SIGNAL_RETURN_CODE;
  }

  if (current_position_ccc < start_position_ccc)
  {
    current_position_ccc = start_position_ccc;
  }
  else if (current_position_ccc >= end_position_ccc)
  {
    current_position_ccc = end_position_ccc - 1;
  }

  int pass_status_mask = STATUS_MASK & BAD_HEAD_MASK;
  if (!read_bad_head_ccc && rebuild_assist_enabled_ccc)
  {
    pass_status_mask = STATUS_MASK;
  }

  if (current_status_ccc == REBUILDASSIST && ret == 0)
  {
    if (rebuild_assist_ccc)
    {
      strcpy(current_status_string_ccc, curlang_ccc[LANGREBUILDASSIST]);
      cluster_size_ccc = original_cluster_size_ccc;
      skip_ccc = true;
      skip_size_ccc = min_skip_size_ccc;
      set_soft_timer_ccc(-1);
      ret = copy_forward_ccc(NONTRIED, STATUS_MASK & BAD_HEAD_MASK, NONTRIMMED);
      write_logfile_phase_ccc(current_status_string_ccc);
    }
    if (ret == 0)
    {
      current_status_ccc = PHASE2;
      rebuild_assist_ccc = false;
      rollover = 1;
    }
  }
  if (current_status_ccc == PHASE1 && ret == 0)
  {
    if (!no_phase1_ccc)
    {
      if (rollover)
      {
        current_position_ccc = start_position_ccc;
      }
      strcpy(current_status_string_ccc, curlang_ccc[LANGPHASE1]);
      cluster_size_ccc = original_cluster_size_ccc;
      skip_ccc = true;
      skip_size_ccc = min_skip_size_ccc;
      set_soft_timer_ccc(PHASE1);
      ret = copy_forward_ccc(NONTRIED, pass_status_mask, NONTRIMMED);
      write_logfile_phase_ccc(current_status_string_ccc);
    }
    if (ret == 0)
    {
      current_status_ccc = PHASE2;
      rollover = 1;
    }
  }
  if (current_status_ccc == PHASE2 && ret == 0)
  {
    if (!no_phase2_ccc)
    {
      if (rollover)
      {
        current_position_ccc = end_position_ccc - 1;
      }
      strcpy(current_status_string_ccc, curlang_ccc[LANGPHASE2]);
      cluster_size_ccc = original_cluster_size_ccc;
      skip_ccc = true;
      skip_size_ccc = min_skip_size_ccc;
      set_soft_timer_ccc(PHASE2);
      ret = copy_reverse_ccc(NONTRIED, pass_status_mask, NONTRIMMED);
      write_logfile_phase_ccc(current_status_string_ccc);
    }
    if (ret == 0)
    {
      current_status_ccc = PHASE3;
      rollover = 1;
    }
  }

  if (0)
  {
    strcpy(current_status_string_ccc, "mapping");
    process_bad_head_ccc();
  }

  if (read_bad_head_ccc)
  {
    pass_status_mask = STATUS_MASK & BAD_HEAD_MASK;
  }
  else
  {
    pass_status_mask = STATUS_MASK;
  }

  int head_pass;
  for (head_pass = 0; head_pass < 1; head_pass++)
  {
    if (rollover)
    {
      min_skip_size_ccc = original_min_skip_size_ccc;
    }
    skip_size_ccc = min_skip_size_ccc;

    if (current_status_ccc == FINISHED)
    {
      current_status_ccc = PHASE3 + head_pass;
    }
    if (current_status_ccc == PHASE3 + head_pass && ret == 0)
    {
      if (!no_phase3_ccc)
      {
        if (rollover)
        {
          current_position_ccc = start_position_ccc;
        }
        if (head_pass == 0)
        {
          strcpy(current_status_string_ccc, curlang_ccc[LANGPHASE3]);
        }
        else
        {
          strcpy(current_status_string_ccc, curlang_ccc[LANGPHASE3]);
          strcat(current_status_string_ccc, curlang_ccc[LANGBADHEAD]);
        }
        cluster_size_ccc = original_cluster_size_ccc;
        skip_ccc = false;
        set_soft_timer_ccc(PHASE3);
        ret = copy_forward_ccc(NONTRIED, pass_status_mask, NONTRIMMED);
        write_logfile_phase_ccc(current_status_string_ccc);
      }
      if (ret == 0)
      {
        current_status_ccc = PHASE4 + head_pass;
        rollover = 1;
      }
    }
    //min_skip_size_ccc = 0;
    //max_skip_size_ccc = 0;
    //skip_size_ccc = 0;
    if (current_status_ccc == PHASE4 + head_pass && ret == 0)
    {
      if (!no_phase4_ccc)
      {
        if (rollover)
        {
          current_position_ccc = start_position_ccc;
        }
        if (head_pass == 0)
        {
          strcpy(current_status_string_ccc, curlang_ccc[LANGPHASE4]);
        }
        else
        {
          strcpy(current_status_string_ccc, curlang_ccc[LANGPHASE4]);
          strcat(current_status_string_ccc, curlang_ccc[LANGBADHEAD]);
        }
        cluster_size_ccc = original_cluster_size_ccc;
        skip_ccc = false;
        set_soft_timer_ccc(PHASE4);
        ret = copy_forward_ccc(NONTRIED, pass_status_mask, NONTRIMMED);
        write_logfile_phase_ccc(current_status_string_ccc);
      }
      if (ret == 0)
      {
        current_status_ccc = TRIMMING + head_pass;
        rollover = 1;
      }
    }
    if (current_status_ccc == TRIMMING + head_pass && ret == 0)
    {
      if (!no_trim_ccc)
      {
        if (rollover)
        {
          current_position_ccc = start_position_ccc;
        }
        if (head_pass == 0)
        {
          strcpy(current_status_string_ccc, curlang_ccc[LANGTRIMMING]);
        }
        else
        {
          strcpy(current_status_string_ccc, curlang_ccc[LANGTRIMMING]);
          strcat(current_status_string_ccc, curlang_ccc[LANGBADHEAD]);
        }
        skip_ccc = false;
        cluster_size_ccc = block_size_ccc;
        set_soft_timer_ccc(TRIMMING);
        ret = trim_forward_ccc(NONTRIMMED, pass_status_mask, NONSCRAPED);
        write_logfile_phase_ccc(current_status_string_ccc);
      }
      if (ret == 0)
      {
        current_status_ccc = DIVIDING1 + head_pass;
        rollover = 1;
      }
    }
    if (current_status_ccc == DIVIDING1 + head_pass && ret == 0)
    {
      if (!no_divide1_ccc)
      {
        if (rollover)
        {
          current_position_ccc = start_position_ccc;
        }
        if (head_pass == 0)
        {
          strcpy(current_status_string_ccc, curlang_ccc[LANGDIVIDING1]);
        }
        else
        {
          strcpy(current_status_string_ccc, curlang_ccc[LANGDIVIDING1]);
          strcat(current_status_string_ccc, curlang_ccc[LANGBADHEAD]);
        }
        skip_ccc = false;
        cluster_size_ccc = original_cluster_size_ccc / (do_divide2_ccc ? DIVIDE1_VALUE : DIVIDE_VALUE);
        if (cluster_size_ccc < block_size_ccc)
        {
          cluster_size_ccc = block_size_ccc;
        }
        set_soft_timer_ccc(DIVIDING1);
        ret = copy_forward_ccc(NONTRIMMED, pass_status_mask, (do_divide2_ccc ? NONDIVIDED : NONSCRAPED));
        write_logfile_phase_ccc(current_status_string_ccc);
      }
      if (ret == 0)
      {
        current_status_ccc = DIVIDING2 + head_pass;
        rollover = 1;
      }
    }
    if (current_status_ccc == DIVIDING2 + head_pass && ret == 0)
    {
      if (!no_divide1_ccc && do_divide2_ccc)
      {
        if (rollover)
        {
          current_position_ccc = start_position_ccc;
        }
        if (head_pass == 0)
        {
          strcpy(current_status_string_ccc, curlang_ccc[LANGDIVIDING2]);
        }
        else
        {
          strcpy(current_status_string_ccc, curlang_ccc[LANGDIVIDING2]);
          strcat(current_status_string_ccc, curlang_ccc[LANGBADHEAD]);
        }
        skip_ccc = false;
        cluster_size_ccc = original_cluster_size_ccc / DIVIDE2_VALUE;
        if (cluster_size_ccc < block_size_ccc)
        {
          cluster_size_ccc = block_size_ccc;
        }
        set_soft_timer_ccc(DIVIDING2);
        ret = copy_forward_ccc(NONDIVIDED, pass_status_mask, NONSCRAPED);
        write_logfile_phase_ccc(current_status_string_ccc);
      }
      if (ret == 0)
      {
        current_status_ccc = SCRAPING + head_pass;
        rollover = 1;
      }
    }
    if (current_status_ccc == SCRAPING + head_pass && ret == 0)
    {
      if (!no_scrape_ccc)
      {
        if (rollover)
        {
          current_position_ccc = start_position_ccc;
        }
        if (head_pass == 0)
        {
          strcpy(current_status_string_ccc, curlang_ccc[LANGSCRAPING]);
        }
        else
        {
          strcpy(current_status_string_ccc, curlang_ccc[LANGSCRAPING]);
          strcat(current_status_string_ccc, curlang_ccc[LANGBADHEAD]);
        }
        skip_ccc = false;
        cluster_size_ccc = block_size_ccc;
        set_soft_timer_ccc(SCRAPING);
        ret = copy_forward_ccc(NONSCRAPED, pass_status_mask, BAD);
        write_logfile_phase_ccc(current_status_string_ccc);
      }
      if (ret == 0)
      {
        current_status_ccc = FINISHED;
        rollover = 1;
      }
    }
  }

  if (current_status_ccc != RETRYING)
  {
    rollover = 1;
  }

  if (retries_ccc > 0 && ret == 0)
  {
    current_status_ccc = RETRYING;
  }
  if (current_status_ccc == RETRYING && ret == 0)
  {
    if (retries_ccc < 1)
    {
      current_status_ccc = FINISHED;
    }
    while (retries_ccc > 0)
    {
      current_status_ccc = RETRYING;
      retry_error_size_ccc = error_size_ccc;
      if (rollover)
      {
        current_position_ccc = start_position_ccc;
        retried_lba_count_ccc = 0;
      }
      strcpy(current_status_string_ccc, curlang_ccc[LANGRETRYING]);
      skip_ccc = false;
      cluster_size_ccc = block_size_ccc;
      set_soft_timer_ccc(RETRYING);
      ret = copy_forward_ccc(BAD, STATUS_MASK & BAD_HEAD_MASK, BAD);
      write_logfile_phase_ccc(current_status_string_ccc);
      if (ret == 0)
      {
        current_status_ccc = FINISHED;
        rollover = 1;
        retries_ccc--;
      }
      else
      {
        break;
      }
    }
  }
  if (current_status_ccc == FINISHED)
  {
    strcpy(current_status_string_ccc, curlang_ccc[LANGFINISHED]);
    update_display_ccc(0);
    strcpy (tempmessage_ccc, curlang_ccc[LANGRESCUEFINISHED]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
    clear_error_message_ccc();
    ret = 0;
  }
  else if (ret)
  {
    update_display_ccc(0);
    strcpy (tempmessage_ccc, curlang_ccc[LANGRESCUEFAILEDERROR]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, "%d", ret);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
    clear_error_message_ccc();
  }
  else
  {
    update_display_ccc(0);
    strcpy (tempmessage_ccc, curlang_ccc[LANGRESCUEFAILED]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
    clear_error_message_ccc();
  }
  set_soft_timer_ccc(-1);
  if (current_status_ccc != FINISHED)
  {
    update_display_ccc(0);
  }

  return ret;
}





int clone_reverse_ccc(void)
{
  int ret = 0;
  int rollover = 0;
  //fprintf (stdout, "currentpos= %lld currentstat=%lld\n", current_position_ccc, current_status_ccc);  //debug
  if (current_status_ccc == REBUILDASSIST)
  {
    current_status_ccc = PHASE1;
  }
  else if (current_status_ccc == FINISHED)
  {
    current_status_ccc = PHASE3;
    current_position_ccc = end_position_ccc;
  }
  //fprintf (stdout, "currentpos= %lld\n", current_position_ccc);  //debug

  // get start time in ms
  copy_start_time_ccc = get_elapsed_usec_ccc() / 1000;

  // set the starting data
  get_log_status_ccc(STATUS_MASK & BAD_HEAD_MASK);
  starting_finished_value_ccc = finished_size_ccc;
  last_finished_value_ccc = finished_size_ccc;

  update_display_ccc(0);
  if (stop_signal_ccc)
  {
    return STOP_SIGNAL_RETURN_CODE;
  }

  if (current_position_ccc < start_position_ccc)
  {
    current_position_ccc = start_position_ccc;
  }
  else if (current_position_ccc >= end_position_ccc)
  {
    current_position_ccc = end_position_ccc;
  }

  int pass_status_mask = STATUS_MASK & BAD_HEAD_MASK;
  if (!read_bad_head_ccc && rebuild_assist_enabled_ccc)
  {
    pass_status_mask = STATUS_MASK;
  }

  if (current_status_ccc == PHASE1 && ret == 0)
  {
    if (!no_phase1_ccc)
    {
      if (current_position_ccc == start_position_ccc)
      {
        current_position_ccc = end_position_ccc;
      }
      strcpy(current_status_string_ccc, curlang_ccc[LANGPHASE1]);
      strcat(current_status_string_ccc, curlang_ccc[LANGREVERSE]);
      cluster_size_ccc = original_cluster_size_ccc;
      skip_ccc = true;
      skip_size_ccc = min_skip_size_ccc;
      set_soft_timer_ccc(PHASE1);
      ret = copy_reverse_ccc(NONTRIED, pass_status_mask, NONTRIMMED);
      write_logfile_phase_ccc(current_status_string_ccc);
    }
    if (ret == 0)
    {
      current_status_ccc = PHASE2;
      rollover = 1;
    }
  }
  if (current_status_ccc == PHASE2 && ret == 0)
  {
    if (!no_phase2_ccc)
    {
      if (rollover)
      {
        current_position_ccc = start_position_ccc;
      }
      strcpy(current_status_string_ccc, curlang_ccc[LANGPHASE2]);
      strcat(current_status_string_ccc, curlang_ccc[LANGREVERSE]);
      cluster_size_ccc = original_cluster_size_ccc;
      skip_ccc = true;
      skip_size_ccc = min_skip_size_ccc;
      set_soft_timer_ccc(PHASE2);
      ret = copy_forward_ccc(NONTRIED, pass_status_mask, NONTRIMMED);
      write_logfile_phase_ccc(current_status_string_ccc);
    }
    if (ret == 0)
    {
      current_status_ccc = PHASE3;
      rollover = 1;
    }
  }

  if (0)
  {
    strcpy(current_status_string_ccc, "mapping");
    process_bad_head_ccc();
  }

  if (read_bad_head_ccc)
  {
    pass_status_mask = STATUS_MASK & BAD_HEAD_MASK;
  }
  else
  {
    pass_status_mask = STATUS_MASK;
  }

  int head_pass;
  for (head_pass = 0; head_pass < 1; head_pass++)
  {
    if (rollover)
    {
      min_skip_size_ccc = original_min_skip_size_ccc;
    }
    skip_size_ccc = min_skip_size_ccc;

    if (current_status_ccc == FINISHED)
    {
      current_status_ccc = PHASE3 + head_pass;
    }
    if (current_status_ccc == PHASE3 + head_pass && ret == 0)
    {
      if (!no_phase3_ccc)
      {
        if (rollover)
        {
          current_position_ccc = end_position_ccc;
        }
        if (head_pass == 0)
        {
          strcpy(current_status_string_ccc, curlang_ccc[LANGPHASE3]);
          strcat(current_status_string_ccc, curlang_ccc[LANGREVERSE]);
        }
        else
        {
          strcpy(current_status_string_ccc, curlang_ccc[LANGPHASE3]);
          strcat(current_status_string_ccc, curlang_ccc[LANGBADHEAD]);
          strcat(current_status_string_ccc, curlang_ccc[LANGREVERSE]);
        }
        cluster_size_ccc = original_cluster_size_ccc;
        skip_ccc = false;
        set_soft_timer_ccc(PHASE3);
        ret = copy_reverse_ccc(NONTRIED, pass_status_mask, NONTRIMMED);
        write_logfile_phase_ccc(current_status_string_ccc);
      }
      if (ret == 0)
      {
        current_status_ccc = PHASE4 + head_pass;
        rollover = 1;
      }
    }
    //min_skip_size_ccc = 0;
    //max_skip_size_ccc = 0;
    //skip_size_ccc = 0;
    if (current_status_ccc == PHASE4 + head_pass && ret == 0)
    {
      if (!no_phase4_ccc)
      {
        if (rollover)
        {
          current_position_ccc = end_position_ccc;
        }
        if (head_pass == 0)
        {
          strcpy(current_status_string_ccc, curlang_ccc[LANGPHASE4]);
          strcat(current_status_string_ccc, curlang_ccc[LANGREVERSE]);
        }
        else
        {
          strcpy(current_status_string_ccc, curlang_ccc[LANGPHASE4]);
          strcat(current_status_string_ccc, curlang_ccc[LANGBADHEAD]);
          strcat(current_status_string_ccc, curlang_ccc[LANGREVERSE]);
        }
        cluster_size_ccc = original_cluster_size_ccc;
        skip_ccc = false;
        set_soft_timer_ccc(PHASE4);
        ret = copy_reverse_ccc(NONTRIED, pass_status_mask, NONTRIMMED);
        write_logfile_phase_ccc(current_status_string_ccc);
      }
      if (ret == 0)
      {
        current_status_ccc = TRIMMING + head_pass;
        rollover = 1;
      }
    }
    if (current_status_ccc == TRIMMING + head_pass && ret == 0)
    {
      if (!no_trim_ccc)
      {
        if (rollover)
        {
          current_position_ccc = end_position_ccc;
        }
        if (head_pass == 0)
        {
          strcpy(current_status_string_ccc, curlang_ccc[LANGTRIMMING]);
          strcat(current_status_string_ccc, curlang_ccc[LANGREVERSE]);
        }
        else
        {
          strcpy(current_status_string_ccc, curlang_ccc[LANGTRIMMING]);
          strcat(current_status_string_ccc, curlang_ccc[LANGBADHEAD]);
          strcat(current_status_string_ccc, curlang_ccc[LANGREVERSE]);
        }
        skip_ccc = false;
        cluster_size_ccc = block_size_ccc;
        set_soft_timer_ccc(TRIMMING);
        ret = trim_reverse_ccc(NONTRIMMED, pass_status_mask, NONSCRAPED);
        write_logfile_phase_ccc(current_status_string_ccc);
      }
      if (ret == 0)
      {
        current_status_ccc = DIVIDING1 + head_pass;
        rollover = 1;
      }
    }
    if (current_status_ccc == DIVIDING1 + head_pass && ret == 0)
    {
      if (!no_divide1_ccc)
      {
        if (rollover)
        {
          current_position_ccc = end_position_ccc;
        }
        if (head_pass == 0)
        {
          strcpy(current_status_string_ccc, curlang_ccc[LANGDIVIDING1]);
          strcat(current_status_string_ccc, curlang_ccc[LANGREVERSE]);
        }
        else
        {
          strcpy(current_status_string_ccc, curlang_ccc[LANGDIVIDING1]);
          strcat(current_status_string_ccc, curlang_ccc[LANGBADHEAD]);
          strcat(current_status_string_ccc, curlang_ccc[LANGREVERSE]);
        }
        skip_ccc = false;
        cluster_size_ccc = original_cluster_size_ccc / (do_divide2_ccc ? DIVIDE1_VALUE : DIVIDE_VALUE);
        if (cluster_size_ccc < block_size_ccc)
        {
          cluster_size_ccc = block_size_ccc;
        }
        set_soft_timer_ccc(DIVIDING1);
        ret = copy_reverse_ccc(NONTRIMMED, pass_status_mask, (do_divide2_ccc ? NONDIVIDED : NONSCRAPED));
        write_logfile_phase_ccc(current_status_string_ccc);
      }
      if (ret == 0)
      {
        current_status_ccc = DIVIDING2 + head_pass;
        rollover = 1;
      }
    }
    if (current_status_ccc == DIVIDING2 + head_pass && ret == 0)
    {
      if (!no_divide1_ccc && do_divide2_ccc)
      {
        if (rollover)
        {
          current_position_ccc = end_position_ccc;
        }
        if (head_pass == 0)
        {
          strcpy(current_status_string_ccc, curlang_ccc[LANGDIVIDING2]);
          strcat(current_status_string_ccc, curlang_ccc[LANGREVERSE]);
        }
        else
        {
          strcpy(current_status_string_ccc, curlang_ccc[LANGDIVIDING2]);
          strcat(current_status_string_ccc, curlang_ccc[LANGBADHEAD]);
          strcat(current_status_string_ccc, curlang_ccc[LANGREVERSE]);
        }
        skip_ccc = false;
        cluster_size_ccc = original_cluster_size_ccc / DIVIDE2_VALUE;
        if (cluster_size_ccc < block_size_ccc)
        {
          cluster_size_ccc = block_size_ccc;
        }
        set_soft_timer_ccc(DIVIDING2);
        ret = copy_reverse_ccc(NONDIVIDED, pass_status_mask, NONSCRAPED);
        write_logfile_phase_ccc(current_status_string_ccc);
      }
      if (ret == 0)
      {
        current_status_ccc = SCRAPING + head_pass;
        rollover = 1;
      }
    }
    if (current_status_ccc == SCRAPING + head_pass && ret == 0)
    {
      if (!no_scrape_ccc)
      {
        if (rollover)
        {
          current_position_ccc = end_position_ccc;
        }
        if (head_pass == 0)
        {
          strcpy(current_status_string_ccc, curlang_ccc[LANGSCRAPING]);
          strcat(current_status_string_ccc, curlang_ccc[LANGREVERSE]);
        }
        else
        {
          strcpy(current_status_string_ccc, curlang_ccc[LANGSCRAPING]);
          strcat(current_status_string_ccc, curlang_ccc[LANGBADHEAD]);
          strcat(current_status_string_ccc, curlang_ccc[LANGREVERSE]);
        }
        skip_ccc = false;
        cluster_size_ccc = block_size_ccc;
        set_soft_timer_ccc(SCRAPING);
        ret = copy_reverse_ccc(NONSCRAPED, pass_status_mask, BAD);
        write_logfile_phase_ccc(current_status_string_ccc);
      }
      if (ret == 0)
      {
        current_status_ccc = FINISHED;
        rollover = 1;
      }
    }
  }

  if (current_status_ccc != RETRYING)
  {
    rollover = 1;
  }

  if (retries_ccc > 0 && ret == 0)
  {
    current_status_ccc = RETRYING;
  }
  if (current_status_ccc == RETRYING && ret == 0)
  {
    if (retries_ccc < 1)
    {
      current_status_ccc = FINISHED;
    }
    while (retries_ccc > 0)
    {
      current_status_ccc = RETRYING;
      if (rollover)
      {
        current_position_ccc = end_position_ccc;
        retried_lba_count_ccc = 0;
      }
      strcpy(current_status_string_ccc, curlang_ccc[LANGRETRYING]);
      strcat(current_status_string_ccc, curlang_ccc[LANGREVERSE]);
      skip_ccc = false;
      cluster_size_ccc = block_size_ccc;
      set_soft_timer_ccc(RETRYING);
      ret = copy_forward_ccc(BAD, STATUS_MASK & BAD_HEAD_MASK, BAD);
      write_logfile_phase_ccc(current_status_string_ccc);
      if (ret == 0)
      {
        current_status_ccc = FINISHED;
        rollover = 1;
        retries_ccc--;
      }
      else
      {
        break;
      }
    }
  }
  if (current_status_ccc == FINISHED)
  {
    strcpy(current_status_string_ccc, curlang_ccc[LANGFINISHED]);
    update_display_ccc(0);
    strcpy (tempmessage_ccc, curlang_ccc[LANGRESCUEFINISHED]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
    clear_error_message_ccc();
    ret = 0;
  }
  else if (ret)
  {
    update_display_ccc(0);
    strcpy (tempmessage_ccc, curlang_ccc[LANGRESCUEFAILEDERROR]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, "%d", ret);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
    clear_error_message_ccc();
  }
  else
  {
    update_display_ccc(0);
    strcpy (tempmessage_ccc, curlang_ccc[LANGRESCUEFAILED]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
    clear_error_message_ccc();
  }
  set_soft_timer_ccc(-1);
  if (current_status_ccc != FINISHED)
  {
    update_display_ccc(0);
  }

  return ret;
}





int driver_clone_forward_ccc(long long start, long long small_end, long long big_end)
{
  // if the destination is null then reset the log to always read from the source
  int no_destination = 0;
  if (strcmp(disk_2_ccc, "/dev/null") == 0)
  {
    complete_log_reset_ccc();
    no_destination = 1;
  }

  last_small_read_size_ccc = small_end - start;
  last_big_read_size_ccc = big_end - start;
  int ret = 0;
  long long the_end;
  int grind = 0;

  start_position_ccc = start;
  the_end = big_end;
  write_ctrl_data_ccc(CTRL_KSECTOR_COUNT, read_ctrl_data_ccc(CTRL_KSECTORS_REQUESTED) );

  if ( (the_end - start) > original_cluster_size_ccc)
  {
    the_end = start + original_cluster_size_ccc;
  }
  if (the_end < small_end)
  {
    the_end = small_end;
  }
  write_ctrl_data_ccc(CTRL_KSECTOR_COUNT, (the_end - start) * (sector_size_ccc / KERNEL_SECTOR_SIZE) );
  end_position_ccc = the_end;
  current_position_ccc = start_position_ccc;
  //fprintf (stdout, "start %lld small_end %lld big_end %lld\n", start, small_end, big_end);    //debug

  // driver read ahead adjustment
  if (driver_mode_ccc == 3 && last_end_position_ccc == current_position_ccc)
  {
    if (driver_readahead_multiplier_ccc < 1024 && (the_end - start) * driver_readahead_multiplier_ccc < DRIVER_TRANSFER_BUFFER_SIZE / sector_size_ccc)
    {
      driver_readahead_multiplier_ccc = driver_readahead_multiplier_ccc * 2;
    }
      long long sectors_requested = (the_end - start) * driver_readahead_multiplier_ccc;
      if (sectors_requested > DRIVER_TRANSFER_BUFFER_SIZE / sector_size_ccc)
      {
        sectors_requested = DRIVER_TRANSFER_BUFFER_SIZE / sector_size_ccc;
      }
      if (sectors_requested < driver_minimum_cluster_size_ccc)
      {
        sectors_requested = driver_minimum_cluster_size_ccc;
      }
      if (sectors_requested > original_cluster_size_ccc)
      {
        sectors_requested = original_cluster_size_ccc;
      }
      if (sectors_requested < small_end - start)
      {
        sectors_requested = small_end - start;
      }
      if (sectors_requested > DRIVER_TRANSFER_BUFFER_SIZE / sector_size_ccc)
      {
        sectors_requested = DRIVER_TRANSFER_BUFFER_SIZE / sector_size_ccc;
      }
      the_end = start + sectors_requested;
      if (the_end > source_total_size_ccc)
      {
        the_end = source_total_size_ccc;
      }
      end_position_ccc = the_end;
      write_ctrl_data_ccc(CTRL_KSECTOR_COUNT, sectors_requested * (sector_size_ccc / KERNEL_SECTOR_SIZE) );
      //fprintf (stdout, "start %lld small %lld big %lld sectors %lld driver read ahead %d\n", start, small_end, big_end, big_end - start, driver_readahead_multiplier_ccc);    //debug
  }
  else if (driver_mode_ccc == 3)
  {
    driver_readahead_multiplier_ccc = 1;
    long long sectors_requested = (the_end - start);
    if (sectors_requested < driver_minimum_cluster_size_ccc)
    {
      sectors_requested = driver_minimum_cluster_size_ccc;
    }
    if (sectors_requested > original_cluster_size_ccc)
    {
      sectors_requested = original_cluster_size_ccc;
    }
    if (sectors_requested > DRIVER_TRANSFER_BUFFER_SIZE / sector_size_ccc)
    {
      sectors_requested = DRIVER_TRANSFER_BUFFER_SIZE / sector_size_ccc;
    }
    the_end = start + sectors_requested;
    if (the_end > source_total_size_ccc)
    {
      the_end = source_total_size_ccc;
    }
    end_position_ccc = the_end;
    write_ctrl_data_ccc(CTRL_KSECTOR_COUNT, sectors_requested * (sector_size_ccc / KERNEL_SECTOR_SIZE) );
    //fprintf (stdout, "start %lld small %lld big %lld sectors %lld driver read ahead %d\n", start, small_end, big_end, big_end - start, driver_readahead_multiplier_ccc);    //debug
  }
  else
  {
    driver_readahead_multiplier_ccc = 1;
  }
  last_end_position_ccc = the_end;
  last_actual_read_size_ccc = the_end - start;
  driver_read_rate_size_ccc += last_actual_read_size_ccc;

  ret = add_to_domain_ccc(start_position_ccc, end_position_ccc - start_position_ccc);
  if (ret)
  {
    return ret;
  }


  if (driver_mode_ccc < 5)
  {
    // first look for data that has been read to destination and retrieve it
    int line = find_block_ccc(current_position_ccc);
    if (line == -1)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGPOSNOTFOUND]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " 0x%llx", current_position_ccc);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return INTERNAL_ERROR_RETURN_CODE;
    }
    while (1)
    {
      if (stop_signal_ccc)
      {
        return STOP_SIGNAL_RETURN_CODE;
      }
      if (no_destination)
      {
        break;
      }
      // check to see if current position is in a block of status type finished
      if ( (lstatus_ccc[line] & STATUS_MASK) != FINISHED)
      {
        line = find_next_block_ccc(line, FINISHED, STATUS_MASK);
        if (line == -1)
        {
          break;
        }
        current_position_ccc = lposition_ccc[line];
        // if past the end position then stop
        if (current_position_ccc >= end_position_ccc)
        {
          break;
        }
      }
      strcpy(current_status_string_ccc, curlang_ccc[LANGREADINGDESTINATION]);
      update_display_ccc(DISPLAY_UPDATE_TIME);

      int rsize = end_position_ccc - current_position_ccc;
      long long block_end = lposition_ccc[line] + lsize_ccc[line];
      // check if the chunk fits in the block and if not then adjust read size
      if (current_position_ccc + rsize > block_end)
      {
        rsize = block_end - current_position_ccc;
      }

      // sanity check
      if (rsize * sector_size_ccc > DRIVER_TRANSFER_BUFFER_SIZE)
      {
        strcpy (tempmessage_ccc, curlang_ccc[LANGDRIVERBUFFEREXCEEDED]);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return INTERNAL_ERROR_RETURN_CODE;
      }

      ret = lseek(disk2_fd_ccc, current_position_ccc * sector_size_ccc, SEEK_SET);
      if (ret == -1)
      {
        strcpy (tempmessage_ccc, curlang_ccc[LANGCANTSEEKDESTINATION]);
        message_error_ccc(tempmessage_ccc);
        sprintf (tempmessage_ccc, " %s (%s)", disk_2_ccc, strerror(errno));
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return OUTPUT_DEVICE_ERROR_RETURN_CODE;
      }
      ret = read(disk2_fd_ccc, ccc_buffer_ccc, rsize * sector_size_ccc);
      if (ret == -1)
      {
        strcpy (tempmessage_ccc, curlang_ccc[LANGCANTREADDESTINATION]);
        message_error_ccc(tempmessage_ccc);
        sprintf (tempmessage_ccc, " %s (%s)", disk_2_ccc, strerror(errno));
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return OUTPUT_DEVICE_ERROR_RETURN_CODE;
      }
      else if (ret != rsize * sector_size_ccc)
      {
        strcpy (tempmessage_ccc, curlang_ccc[LANGCANTREADDESTINATION]);
        message_error_ccc(tempmessage_ccc);
        sprintf (tempmessage_ccc, " %s", disk_2_ccc);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return OUTPUT_DEVICE_ERROR_RETURN_CODE;
      }

      long long kposition = current_position_ccc * (sector_size_ccc / KERNEL_SECTOR_SIZE);
      long long koffset = kposition - read_ctrl_data_ccc(CTRL_KSECTOR_START);
      long long ksize = rsize * (sector_size_ccc / KERNEL_SECTOR_SIZE);
      if (koffset >= 0 && koffset + ksize <= DRIVER_TRANSFER_BUFFER_SIZE / KERNEL_SECTOR_SIZE)
      {
        //fprintf (stdout, "read from destination position%lld offset%lld size%lld\n", kposition, koffset, ksize);    //debug
        memcpy(driver_transfer_buffer_address_ccc + (koffset * KERNEL_SECTOR_SIZE), ccc_buffer_ccc, ksize * KERNEL_SECTOR_SIZE);
        memset(driver_error_bitmap_address_ccc + koffset, 0, ksize);
      }
      memcpy(&display_buffer_ccc, ccc_buffer_ccc, DISPLAY_BUFFER_SIZE);

      line = find_next_block_ccc(line, FINISHED, STATUS_MASK);
      if (line == -1)
      {
        break;
      }
      current_position_ccc = lposition_ccc[line];
      // if past the end position then stop
      if (current_position_ccc >= end_position_ccc)
      {
        break;
      }
    }

    // check if data is complete from destination
    int i;
    int complete = 1;
    for (i = 0; i < (the_end * (sector_size_ccc / KERNEL_SECTOR_SIZE)) - (start * (sector_size_ccc / KERNEL_SECTOR_SIZE)); i++)
    {
      // sanity check
      if (i > DRIVER_TRANSFER_BUFFER_SIZE / KERNEL_SECTOR_SIZE)
      {
        break;
      }
      if (read_ctrl_error_bitmap_ccc(i))
      {
        complete = 0;
        break;
      }
    }



    // now process the source for data that has not been read yet
    int keep_going = 1;
    if (stop_signal_ccc)
    {
      return STOP_SIGNAL_RETURN_CODE;
    }
    if (driver_mode_ccc > 3)
    {
      keep_going = 0;
    }

    ret = 0;
    current_position_ccc = start_position_ccc;
    if (ret == 0 && !complete && keep_going)
    {
      strcpy(current_status_string_ccc, curlang_ccc[LANGREADING]);
      strcat(current_status_string_ccc, curlang_ccc[LANGPHASE4]);
      cluster_size_ccc = original_cluster_size_ccc;
      skip_ccc = false;
      set_soft_timer_ccc(PHASE4);
      ret = copy_forward_ccc(NONTRIED, STATUS_MASK, (grind ? NONSCRAPED : NONTRIMMED));
    }

    if (driver_mode_ccc > 1)
    {
      keep_going = 0;
    }

    if (ret == 0 && !complete && keep_going)
    {
      if (!no_trim_ccc)
      {
        strcpy(current_status_string_ccc, curlang_ccc[LANGREADING]);
        strcat(current_status_string_ccc, curlang_ccc[LANGTRIMMING]);
        current_position_ccc = start_position_ccc;
        skip_ccc = false;
        cluster_size_ccc = block_size_ccc;
        set_soft_timer_ccc(TRIMMING);
        ret = trim_forward_ccc(NONTRIMMED, STATUS_MASK, NONSCRAPED);
      }
    }

    if (ret == 0 && !complete && keep_going)
    {
      if (!no_divide1_ccc)
      {
        strcpy(current_status_string_ccc, curlang_ccc[LANGREADING]);
        strcat(current_status_string_ccc, curlang_ccc[LANGDIVIDING1]);
        current_position_ccc = start_position_ccc;
        skip_ccc = false;
        cluster_size_ccc = original_cluster_size_ccc / (do_divide2_ccc ? DIVIDE1_VALUE : DIVIDE_VALUE);
        if (cluster_size_ccc < block_size_ccc)
        {
          cluster_size_ccc = block_size_ccc;
        }
        set_soft_timer_ccc(DIVIDING1);
        ret = copy_forward_ccc(NONTRIMMED, STATUS_MASK, (do_divide2_ccc ? NONDIVIDED : NONSCRAPED));
      }
    }

    if (ret == 0 && !complete && keep_going)
    {
      if (!no_divide1_ccc && do_divide2_ccc)
      {
        strcpy(current_status_string_ccc, curlang_ccc[LANGREADING]);
        strcat(current_status_string_ccc, curlang_ccc[LANGDIVIDING2]);
        current_position_ccc = start_position_ccc;
        skip_ccc = false;
        cluster_size_ccc = original_cluster_size_ccc / DIVIDE2_VALUE;
        if (cluster_size_ccc < block_size_ccc)
        {
          cluster_size_ccc = block_size_ccc;
        }
        set_soft_timer_ccc(DIVIDING2);
        ret = copy_forward_ccc(NONDIVIDED, STATUS_MASK, NONSCRAPED);
      }
    }

    if (ret == 0 && !complete && keep_going)
    {
      if (!no_scrape_ccc)
      {
        strcpy(current_status_string_ccc, curlang_ccc[LANGREADING]);
        strcat(current_status_string_ccc, curlang_ccc[LANGSCRAPING]);
        current_position_ccc = start_position_ccc;
        skip_ccc = false;
        cluster_size_ccc = block_size_ccc;
        set_soft_timer_ccc(SCRAPING);
        ret = copy_forward_ccc(NONSCRAPED, STATUS_MASK, BAD);
      }
    }

    if (retries_ccc > 0 && ret == 0 && !complete && keep_going)
    {
      while (retries_ccc > 0)
      {
        strcpy(current_status_string_ccc, curlang_ccc[LANGREADING]);
        strcat(current_status_string_ccc, curlang_ccc[LANGRETRYING]);
        retry_error_size_ccc = 0;
        current_position_ccc = start_position_ccc;
        retried_lba_count_ccc = 0;
        skip_ccc = false;
        cluster_size_ccc = block_size_ccc;
        set_soft_timer_ccc(RETRYING);
        ret = copy_forward_ccc(BAD, STATUS_MASK, BAD);
        if (ret == 0)
        {
          retries_ccc--;
        }
        else
        {
          break;
        }
      }
    }
  }

  set_soft_timer_ccc(-1);
  update_display_ccc(DISPLAY_UPDATE_TIME);
  if (stop_signal_ccc)
  {
    return STOP_SIGNAL_RETURN_CODE;
  }

  return ret;
}





int copy_forward_ccc(int status_type, int status_mask, int new_status_type)
{
  if (drive_locked_ccc)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGDRIVELOCKED]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  int dont_mark = 1;
  if (td_soft_reset_time_ccc >= sc_soft_reset_time_ccc)
  {
    dont_mark = 0;
  }
  direction_ccc = FORWARD;
  if (current_position_ccc < start_position_ccc)
  {
    current_position_ccc = start_position_ccc;
  }
  update_display_ccc(driver_mode_ccc ? DISPLAY_UPDATE_TIME : 0);
  if (stop_signal_ccc)
  {
    return STOP_SIGNAL_RETURN_CODE;
  }
  return_value_ccc = update_logfile_ccc(driver_mode_ccc ? LOG_UPDATE_TIME : 0);
  if (return_value_ccc)
  {
    return return_value_ccc;
  }
  last_skip_position_ccc = current_position_ccc;
  rate_skip_count_ccc = 0;
  exit_on_slow_count_ccc = 0;
  reset_skip_ccc();
  reset_rate_ccc();
  while (current_position_ccc < end_position_ccc)
  {
    update_display_ccc(DISPLAY_UPDATE_TIME);
    if (stop_signal_ccc)
    {
      return STOP_SIGNAL_RETURN_CODE;
    }
    return_value_ccc = update_logfile_ccc(LOG_UPDATE_TIME);
    if (return_value_ccc)
    {
      return return_value_ccc;
    }

    if (exit_on_slow_count_ccc >= EXIT_ON_SLOW_COUNT && (current_status_ccc == PHASE1 || current_status_ccc == PHASE2) )
    {
      int ret = call_command_on_error_ccc();
      if (ret)
      {
        strcpy (tempmessage_ccc, curlang_ccc[LANGEXITDUETOSLOW]);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
        clear_error_message_ccc();
        return EXIT_ON_SLOW_RETURN_CODE;
      }
      exit_on_slow_count_ccc = 0;
    }

    if (rate_skip_count_ccc >= RATE_SKIP_COUNT && (current_status_ccc & 0xfe) == (PHASE3 & 0xfe))
    {
      rate_skip_count_ccc = 0;
      total_skip_count_ccc++;
      total_slow_skips_ccc++;
      highest_skip_ccc = skip_size_ccc;
      current_position_ccc += skip_size_ccc;
      current_position_ccc = block_align_ccc(current_position_ccc, block_offset_ccc);
    }

    else
    {
      long long rebuild_assist_skip_position = 0;
      long long rebuild_assist_jump_position = 0;
      int rsize = cluster_size_ccc;
      int line = find_block_ccc(current_position_ccc);
      if (line == -1)
      {
        strcpy (tempmessage_ccc, curlang_ccc[LANGPOSNOTFOUND]);
        message_error_ccc(tempmessage_ccc);
        sprintf (tempmessage_ccc, " 0x%llx", current_position_ccc);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return INTERNAL_ERROR_RETURN_CODE;
      }

      // check to see if current position is in a block of status type, if not then find the next block
      if ( (lstatus_ccc[line] & status_mask) != status_type)
      {
        line = find_next_block_ccc(line, status_type, status_mask);
        if (line == -1)
        {
          break;
        }
        current_position_ccc = lposition_ccc[line];
        // if past the end position then stop
        if (current_position_ccc >= end_position_ccc)
        {
          break;
        }
      }

      // check if the chunk fits in the block and if not then adjust read size
      long long block_end = lposition_ccc[line] + lsize_ccc[line];
      if (current_position_ccc + rsize > block_end)
      {
        rsize = block_end - current_position_ccc;
      }

      // check if chunk goes past end position and adjust read size if needed
      if (current_position_ccc + rsize > end_position_ccc)
      {
        rsize = end_position_ccc - current_position_ccc;
      }

      // align read size to block size to keep aligned
      long long aligned_position = block_align_ccc(current_position_ccc, block_offset_ccc);
      long long size_offset = current_position_ccc - aligned_position;
      if (size_offset != block_size_ccc && size_offset > 0 && size_offset < rsize)
      {
        rsize -= size_offset;
      }

      if (current_position_ccc > last_skip_position_ccc + min_skip_size_ccc)
      {
        reset_skip_ccc();
      }

      // check domain
      if (rsize > 0)
      {
        long long current_start_pos = current_position_ccc;
        long long current_end_pos = current_position_ccc + rsize;
        int current_size = rsize;
        int ret = process_domain_ccc(current_position_ccc, rsize, domain_status_ccc, STATUS_MASK);
        if (verbosedebug_ccc)
        {
          sprintf (tempmessage_ccc, "ret1=%d\n", ret);
          message_debug_ccc(tempmessage_ccc, DEBUG10);
        }
        // if the return is 0 then no data to be read so adjust position before setting read size to 0
        if (ret <= 0)
        {
          if (verbosedebug_ccc)
          {
            sprintf (tempmessage_ccc, "cposition=0x%llx rsize=0x%x\n", current_position_ccc, rsize);
            message_debug_ccc(tempmessage_ccc, DEBUG10);
          }
          current_position_ccc += rsize;
          if (current_position_ccc > end_position_ccc)
          {
            current_position_ccc = end_position_ccc;
          }
          rsize = 0;
        }
        // otherwise set read size to new size, and note that the current position may have been adjusted when the domain was processed
        else
        {
          rsize = ret;
          // sanity check
          if (current_position_ccc < current_start_pos || current_position_ccc + rsize > current_end_pos || rsize > current_size)
          {
            sprintf (tempmessage_ccc, "csposition=0x%llx ceposition=0x%llx nsposition=0x%llx neposition=0x%llx\n", current_start_pos, current_end_pos, current_position_ccc, current_position_ccc + rsize);
            message_debug_ccc(tempmessage_ccc, DEBUG10);
            strcpy (tempmessage_ccc, curlang_ccc[LANGPOSOUTOFRANGE]);
            message_error_ccc(tempmessage_ccc);
            print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
            clear_error_message_ccc();
            return INTERNAL_ERROR_RETURN_CODE;
          }
          rsize = domain_size_adjustment_ccc (cluster_size_ccc, rsize);
        }
      }

      if (rsize > 0)
      {
        if (current_status_ccc == RETRYING)
        {
          retried_lba_count_ccc += rsize;
        }
        int retstat = read_chunk_ccc(current_position_ccc, rsize);
        if (stop_signal_ccc)
        {
          return STOP_SIGNAL_RETURN_CODE;
        }
#ifdef DEBUG
        // if read was successful then write the chunk, otherwise mark the chunk
        long long fake_bad = current_position_ccc;
        if (fake_bad && fake_bad == FAKE_BAD)
        {
          //do_nanosleep_ccc(100000000);
          retstat = 0xfff1;
        }
        else
        {
          fake_bad = 0;
        }
#else
        long long fake_bad = 0;
#endif
        if (!retstat && !(ata_status_ccc & 1) && sense_key_ccc < 2 && !usb_read_residue_ccc && !fake_bad)
        {
          int ret = write_chunk_ccc(current_position_ccc, rsize);
          if (ret)
          {
            // write error
            return ret;
          }
          ret = change_chunk_status_ccc(current_position_ccc, rsize, FINISHED | get_timing_byte_ccc(), FULL_MASK);
          if (ret < 0)
          {
            // error changing chunk
            return ret;
          }

          if (skip_ccc && skip_time_triggered_ccc)
          {
            int skip_info = skip_run_count_ccc | FORWARD_SKIP;
            long long additional_status = set_additional_status_ccc(0, skip_info);
            int ret = change_chunk_status_ccc(current_position_ccc, rsize, FINISHED | additional_status | get_timing_byte_ccc(), FULL_MASK);
            if (ret < 0)
            {
              // error changing chunk
              return ret;
            }

            skip_position_ccc[skip_run_count_ccc] = current_position_ccc;
            current_position_ccc += skip_size_ccc;
            last_skip_jump_position_ccc = current_position_ccc;
            current_position_ccc = block_align_ccc(current_position_ccc, block_offset_ccc);
            if (process_skip_ccc())
            {
              return -1;
            }
          }
        }
        else
        {
          // make a backup of status registers of error for display
          int ata_error_bak = ata_error_ccc;
          int ata_status_bak = ata_status_ccc;

          rate_modifier_ccc += (rsize * sector_size_ccc) / ( (cluster_size_ccc / 16) + 1);
          int skip_info;
          if (skip_ccc)
          {
            skip_info = skip_run_count_ccc | FORWARD_SKIP;
          }
          else
          {
            skip_info = 0;
          }

          int ata_aligned = 0;
          if (ata_lba_ccc == block_align_ccc(ata_lba_ccc, block_offset_ccc) )
          {
            ata_aligned = 1;
          }

          //fprintf (stdout, "ata_aligned=%d ata_return_valid_ccc=%d retstat=%d current_position_ccc=%llx ata_lba_ccc=%llx\n", ata_aligned, ata_return_valid_ccc, retstat, current_position_ccc, ata_lba_ccc);    //debug

          // if no major error and ata and size is greater than block size then check if there is a sector marked as bad
          // and if lba is within the current chunk and aligned and not zero read then process it
          // but if using rebuild assist then process different as data does not need to be read again

          // rebuild assist stuff
          int ncq_return = 0;
          int ncq_error = 0;
          // if using fpdma and there is an error the ncq log must be read
          if (!retstat && use_fpdma_ccc && rebuild_assist_enabled_ccc)
          {
            int ata_return_valid_bak = ata_return_valid_ccc;
            long long ata_lba_bak = ata_lba_ccc;
            ncq_return = read_ncq_error_log_ccc();
            ata_return_valid_ccc = ata_return_valid_bak;
            ata_lba_ccc = ata_lba_bak;
            if (ncq_return || (ata_status_ccc & 1))
            {
              fprintf (stdout, "failed to read ncq log, return=0x%x status=%02x error=%02x\n", ncq_return, ata_status_ccc, ata_error_ccc);
              ncq_error = 1;
            }
            else
            {
              if ( ncq_error_log_data_ccc.sense_key == SKEY_ABORTED_COMMAND && ncq_error_log_data_ccc.sense_asc == get_asc_from_additional_sense_ccc(ASC_MULTIPLE_READ_ERRORS) && ncq_error_log_data_ccc.sense_ascq == get_ascq_from_additional_sense_ccc(ASC_MULTIPLE_READ_ERRORS) && ncq_error_log_data_ccc.final_lba > current_position_ccc )
              {
                rebuild_assist_skip_position = ncq_error_log_data_ccc.final_lba + 1;
                fprintf (stdout, "rebuild assist skip 0x%08llx-0x%08llx\n", ncq_error_log_data_ccc.lba, ncq_error_log_data_ccc.final_lba);
              }
              else if ( ncq_error_log_data_ccc.sense_key == SKEY_MEDIUM_ERROR && ncq_error_log_data_ccc.sense_asc == get_asc_from_additional_sense_ccc(ASC_UNRECOVERED_READ_ERROR) && ncq_error_log_data_ccc.sense_ascq == get_ascq_from_additional_sense_ccc(ASC_UNRECOVERED_READ_ERROR) && ncq_error_log_data_ccc.final_lba > current_position_ccc )
              {
                rebuild_assist_jump_position = ncq_error_log_data_ccc.final_lba + 1;
                //fprintf (stdout, "rebuild assist jump 0x%08llx-0x%08llx\n", ncq_error_log_data_ccc.lba, ncq_error_log_data_ccc.final_lba);
              }
              if (0)
              {
                fprintf (stdout, "current_position=%08llx, lba=%08llx, lbabak%08llx\n", current_position_ccc, ata_lba_ccc, ata_lba_bak);
                fprintf (stdout, "flags=%02x, status=%02x, error=%02x, device=%02x, key=%02x, asc=%02x, ascq=%02x\n", ncq_error_log_data_ccc.flags, ncq_error_log_data_ccc.status, ncq_error_log_data_ccc.error, ncq_error_log_data_ccc.device, ncq_error_log_data_ccc.sense_key, ncq_error_log_data_ccc.sense_asc, ncq_error_log_data_ccc.sense_ascq);    //debug
                fprintf (stdout, "count=%04x, lba=%08llx, final=%08llx\n", ncq_error_log_data_ccc.count, ncq_error_log_data_ccc.lba, ncq_error_log_data_ccc.final_lba);    //debug
              }
              ata_lba_ccc = ncq_error_log_data_ccc.lba;
            }
            ata_error_ccc = ata_error_bak;
            ata_status_ccc = ata_status_bak;
          }

          //fprintf (stdout, "valid=%d retstat=%d current=%llx ncqlba=%llx current+size=%llx\n", ata_return_valid_ccc, retstat, current_position_ccc, ncq_error_log_data_ccc.lba, current_position_ccc + rsize);    //debug
          if (!driver_mode_ccc && use_fpdma_ccc && rebuild_assist_enabled_ccc && !ncq_error && ata_return_valid_ccc && !retstat && ncq_error_log_data_ccc.lba >= current_position_ccc && current_status_ccc == REBUILDASSIST)
          {
            int ret = process_rebuild_assist_chunk_ccc(status_type, new_status_type, retstat, rsize, skip_info);
            if (ret)
            {
              return ret;
            }
          }
          else if (!driver_mode_ccc && enable_process_chunk_ccc && ata_aligned && ata_return_valid_ccc && !retstat && (direct_mode_ccc || ata_identify_success_ccc) && rsize > block_size_ccc && ata_lba_ccc > 0 && ata_lba_ccc >= current_position_ccc && ata_lba_ccc < current_position_ccc + rsize)
          {
            int ret = process_chunk_ccc(new_status_type, retstat, rsize, skip_info);
            if (ret)
            {
              return ret;
            }
          }
          else
          {
            // normal read error
            long long additional_status = set_additional_status_ccc(retstat, skip_info);
            int ret = change_chunk_status_ccc(current_position_ccc, rsize, ((rsize > block_size_ccc || (retstat && dont_mark)) ? new_status_type : BAD) | additional_status | get_timing_byte_ccc(), FULL_MASK);
            if (ret < 0)
            {
              // error changing chunk
              return ret;
            }
            if (driver_mode_ccc)
            {
              long long kposition = current_position_ccc * (sector_size_ccc / KERNEL_SECTOR_SIZE);
              long long koffset = kposition - read_ctrl_data_ccc(CTRL_KSECTOR_START);
              long long ksize = rsize * (sector_size_ccc / KERNEL_SECTOR_SIZE);
              if (koffset >= 0 && koffset + ksize <= DRIVER_TRANSFER_BUFFER_SIZE / KERNEL_SECTOR_SIZE)
              {
                memset(driver_transfer_buffer_address_ccc + (koffset * KERNEL_SECTOR_SIZE), 0, ksize * KERNEL_SECTOR_SIZE);
                memset(driver_error_bitmap_address_ccc + koffset, 0x80, ksize);
              }
            }
          }
          if (skip_ccc && !rebuild_assist_skip_position)
          {
            skip_position_ccc[skip_run_count_ccc] = current_position_ccc;
            current_position_ccc += skip_size_ccc;
            last_skip_jump_position_ccc = current_position_ccc;
            current_position_ccc = block_align_ccc(current_position_ccc, block_offset_ccc);
            skip_time_triggered_ccc = false;
            if (process_skip_ccc())
            {
              return -1;
            }
          }
          else if (skip_ccc && rebuild_assist_skip_position)
          {
            total_skip_count_ccc++;
            total_skip_runs_ccc++;
            last_total_skip_size_ccc = (ncq_error_log_data_ccc.final_lba + 1) - ncq_error_log_data_ccc.lba;
          }
          if (retstat < 0 || retstat > 0xff)
          {
#ifdef DEBUG
            if (fake_bad)
            {
              strcpy (tempmessage_ccc, "fakebad error");
              message_error_ccc(tempmessage_ccc);
              print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
              clear_error_message_ccc();
              return retstat;
            }
#endif
            int ret = call_command_on_error_ccc();
            if (ret)
            {
              return retstat;
            }
          }
          else
          {
            // if everything seems normal check to be sure
            if (check_device_ccc())
            {
              int ret = call_command_on_error_ccc();
              if (ret)
              {
                return DEVICE_ERROR_RETURN_CODE;
              }
            }
          }
          ata_error_ccc = ata_error_bak;
          ata_status_ccc = ata_status_bak;
        }
      }
      current_position_ccc += rsize;
      if (rebuild_assist_skip_position > current_position_ccc)
      {
        current_position_ccc = rebuild_assist_skip_position;
      }
      else if (rebuild_assist_jump_position > current_position_ccc)
      {
        current_position_ccc = rebuild_assist_jump_position;
        fprintf (stdout, "rebuild assist jump to 0x%08llx\n", rebuild_assist_jump_position);
      }
    }
  }
  reset_skip_ccc();
  return 0;
}





int copy_reverse_ccc(int status_type, int status_mask, int new_status_type)
{
  if (drive_locked_ccc)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGDRIVELOCKED]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  int dont_mark = 1;
  if (td_soft_reset_time_ccc >= sc_soft_reset_time_ccc)
  {
    dont_mark = 0;
  }
  direction_ccc = REVERSE;
  int last_sector_adjust = 0;
  if (current_position_ccc >= end_position_ccc)
  {
    current_position_ccc = end_position_ccc - 1;
    last_sector_adjust = 1;
  }
  update_display_ccc(driver_mode_ccc ? DISPLAY_UPDATE_TIME : 0);
  if (stop_signal_ccc)
  {
    return STOP_SIGNAL_RETURN_CODE;
  }
  return_value_ccc = update_logfile_ccc(0);
  if (return_value_ccc)
  {
    return return_value_ccc;
  }
  last_skip_position_ccc = current_position_ccc;
  rate_skip_count_ccc = 0;
  exit_on_slow_count_ccc = 0;
  reset_skip_ccc();
  reset_rate_ccc();
  while (current_position_ccc > start_position_ccc)
  {
    update_display_ccc(DISPLAY_UPDATE_TIME);
    if (stop_signal_ccc)
    {
      return STOP_SIGNAL_RETURN_CODE;
    }
    return_value_ccc = update_logfile_ccc(LOG_UPDATE_TIME);
    if (return_value_ccc)
    {
      return return_value_ccc;
    }

    if (exit_on_slow_count_ccc >= EXIT_ON_SLOW_COUNT && (current_status_ccc == PHASE1 || current_status_ccc == PHASE2) )
    {
      int ret = call_command_on_error_ccc();
      if (ret)
      {
        strcpy (tempmessage_ccc, curlang_ccc[LANGEXITDUETOSLOW]);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
        clear_error_message_ccc();
        return EXIT_ON_SLOW_RETURN_CODE;
      }
      exit_on_slow_count_ccc = 0;
    }

    if (rate_skip_count_ccc >= RATE_SKIP_COUNT && (current_status_ccc & 0xfe) == (PHASE3 & 0xfe))
    {
      rate_skip_count_ccc = 0;
      total_skip_count_ccc++;
      total_slow_skips_ccc++;
      highest_skip_ccc = skip_size_ccc;
      current_position_ccc -= skip_size_ccc;
      current_position_ccc = block_align_up_ccc(current_position_ccc);
    }

    else
    {
      int rsize = cluster_size_ccc;
      int line = find_block_ccc(current_position_ccc);
      if (line == -1)
      {
        strcpy (tempmessage_ccc, curlang_ccc[LANGPOSNOTFOUND]);
        message_error_ccc(tempmessage_ccc);
        sprintf (tempmessage_ccc, " 0x%llx", current_position_ccc);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return INTERNAL_ERROR_RETURN_CODE;
      }

      // check to see if current position is in a block of status type, if not then find the previous block
      if ( (lstatus_ccc[line] & status_mask) != status_type)
      {
        line = find_previous_block_ccc(line, status_type, status_mask);
        if (line == -1)
        {
          break;
        }
        current_position_ccc = lposition_ccc[line] + lsize_ccc[line] - rsize;

        // align position to block size to keep aligned
        long long aligned_position = block_align_up_ccc(current_position_ccc);
        if (aligned_position < lposition_ccc[line] + lsize_ccc[line])
        {
          current_position_ccc = aligned_position;
        }

        // make sure position is still in the current block
        if (current_position_ccc < lposition_ccc[line])
        {
          current_position_ccc = lposition_ccc[line];
        }
        // if before the start position then adjust and if outside of current block then stop
        if (current_position_ccc < start_position_ccc)
        {
          current_position_ccc = start_position_ccc;
          if ( (current_position_ccc < lposition_ccc[line]) || (current_position_ccc > lposition_ccc[line] + lsize_ccc[line]) )
          {
            break;
          }
        }
      }
      else
      {
        // setup for previous chunk
        if (last_sector_adjust)
        {
          current_position_ccc = end_position_ccc;
        }
        current_position_ccc -= rsize;
        if (current_position_ccc < lposition_ccc[line])
        {
          current_position_ccc = lposition_ccc[line];
        }
      }

      // check if the chunk fits in the block and if not then adjust read size
      long long block_end = lposition_ccc[line] + lsize_ccc[line];
      if (current_position_ccc + rsize > block_end)
      {
        rsize = block_end - current_position_ccc;
      }

      // check if chunk goes past end position and adjust read size if needed
      if (current_position_ccc + rsize > end_position_ccc)
      {
        rsize = end_position_ccc - current_position_ccc;
      }

      if (current_position_ccc < last_skip_position_ccc - min_skip_size_ccc)
      {
        reset_skip_ccc();
      }

      // check domain
      if (rsize > 0)
      {
        long long current_start_pos = current_position_ccc;
        long long current_end_pos = current_position_ccc + rsize;
        int current_size = rsize;
        int ret = process_domain_ccc(current_position_ccc, rsize, domain_status_ccc, STATUS_MASK);
        if (verbosedebug_ccc)
        {
          sprintf (tempmessage_ccc, "ret2=%d\n", ret);
          message_debug_ccc(tempmessage_ccc, DEBUG10);
        }
        // if the return is 0 then no data to be read so adjust position before setting read size to 0
        if (ret <= 0)
        {
          current_position_ccc -= rsize;
          if (current_position_ccc < start_position_ccc)
          {
            current_position_ccc = start_position_ccc;
          }
          rsize = 0;
        }
        // otherwise set read size to new size, and note that the current position may have been adjusted when the domain was processed
        else
        {
          rsize = ret;
          // sanity check
          if (current_position_ccc < current_start_pos || current_position_ccc + rsize > current_end_pos || rsize > current_size)
          {
            sprintf (tempmessage_ccc, "csposition=0x%llx ceposition=0x%llx nsposition=0x%llx neposition=0x%llx\n", current_start_pos, current_end_pos, current_position_ccc, current_position_ccc + rsize);
            strcpy (tempmessage_ccc, curlang_ccc[LANGPOSOUTOFRANGE]);
            message_error_ccc(tempmessage_ccc);
            print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
            clear_error_message_ccc();
            return INTERNAL_ERROR_RETURN_CODE;
          }
          rsize = domain_size_adjustment_ccc (cluster_size_ccc, rsize);
        }
      }

      if (rsize > 0)
      {
        if (current_status_ccc == RETRYING)
        {
          retried_lba_count_ccc += rsize;
        }
        int retstat = read_chunk_ccc(current_position_ccc, rsize);
        if (stop_signal_ccc)
        {
          return STOP_SIGNAL_RETURN_CODE;
        }
        // if read was successful then write the chunk, otherwise mark the chunk
#ifdef DEBUG
        long long fake_bad = current_position_ccc;
        if (fake_bad && fake_bad == FAKE_BAD)
        {
          //do_nanosleep_ccc(100000000);
          retstat = 0xfff2;
        }
        else
        {
          fake_bad = 0;
        }
#else
        long long fake_bad = 0;
#endif
        if (!retstat && !(ata_status_ccc & 1) && sense_key_ccc < 2 && !usb_read_residue_ccc && !fake_bad)
        {
          int ret = write_chunk_ccc(current_position_ccc, rsize);
          if (ret)
          {
            // write error
            return ret;
          }
          ret = change_chunk_status_ccc(current_position_ccc, rsize, FINISHED | get_timing_byte_ccc(), FULL_MASK);
          if (ret < 0)
          {
            // error changing chunk
            return ret;
          }

          if (skip_ccc && skip_time_triggered_ccc)
          {
            int skip_info = skip_run_count_ccc | REVERSE_SKIP;
            long long additional_status = set_additional_status_ccc(0, skip_info);
            int ret = change_chunk_status_ccc(current_position_ccc, rsize, FINISHED | additional_status | get_timing_byte_ccc(), FULL_MASK);
            if (ret < 0)
            {
              // error changing chunk
              return ret;
            }

            skip_position_ccc[skip_run_count_ccc] = current_position_ccc;
            current_position_ccc -= skip_size_ccc;
            last_skip_jump_position_ccc = current_position_ccc;
            current_position_ccc = block_align_up_ccc(current_position_ccc);
            if (process_skip_ccc())
            {
              return -1;
            }
          }
        }
        else
        {
          // make a backup of status registers of error for display
          int ata_error_bak = ata_error_ccc;
          int ata_status_bak = ata_status_ccc;
          rate_modifier_ccc += (rsize * sector_size_ccc) / cluster_size_ccc;
          int skip_info;
          if (skip_ccc)
          {
            skip_info = skip_run_count_ccc | REVERSE_SKIP;
          }
          else
          {
            skip_info = 0;
          }

          // if no major error and ata and size is greater than block size then check if there is a sector marked as bad
          // and if lba is within the current chunk and aligned and not zero read then process it
          int ata_aligned = 0;
          if (ata_lba_ccc == block_align_ccc(ata_lba_ccc, block_offset_ccc) )
          {
            ata_aligned = 1;
          }

          // if using fpdma and there is an error the ncq log must be read
          if (!retstat && use_fpdma_ccc)
          {
            int ata_return_valid_bak = ata_return_valid_ccc;
            long long ata_lba_bak = ata_lba_ccc;
            int ncq_return = read_ncq_error_log_ccc();
            ata_return_valid_ccc = ata_return_valid_bak;
            ata_lba_ccc = ata_lba_bak;
            if (ncq_return || (ata_status_ccc & 1))
            {
              fprintf (stdout, "failed to read ncq log, return=0x%x status=%02x error=%02x\n", ncq_return, ata_status_ccc, ata_error_ccc);
            }
            else
            {
              ata_lba_ccc = ncq_error_log_data_ccc.lba;
            }
            ata_error_ccc = ata_error_bak;
            ata_status_ccc = ata_status_bak;
          }

          if (!driver_mode_ccc && enable_process_chunk_ccc && ata_aligned && ata_return_valid_ccc && !retstat && (direct_mode_ccc || ata_identify_success_ccc) && rsize > block_size_ccc && ata_lba_ccc > 0 && ata_lba_ccc >= current_position_ccc && ata_lba_ccc < current_position_ccc + rsize)
          {
            int ret = process_chunk_ccc(new_status_type, retstat, rsize, skip_info);
            if (ret)
            {
              return ret;
            }
          }
          else
          {
            // normal read error
            long long additional_status = set_additional_status_ccc(retstat, skip_info);
            int ret = change_chunk_status_ccc(current_position_ccc, rsize, ((rsize > block_size_ccc || (retstat && dont_mark)) ? new_status_type : BAD) | additional_status | get_timing_byte_ccc(), FULL_MASK);
            if (ret < 0)
            {
              // error changing chunk
              return ret;
            }
            if (driver_mode_ccc)
            {
              long long kposition = current_position_ccc * (sector_size_ccc / KERNEL_SECTOR_SIZE);
              long long koffset = kposition - read_ctrl_data_ccc(CTRL_KSECTOR_START);
              long long ksize = rsize * (sector_size_ccc / KERNEL_SECTOR_SIZE);
              if (koffset >= 0 && koffset + ksize <= DRIVER_TRANSFER_BUFFER_SIZE / KERNEL_SECTOR_SIZE)
              {
                memset(driver_transfer_buffer_address_ccc + (koffset * KERNEL_SECTOR_SIZE), 0, ksize * KERNEL_SECTOR_SIZE);
                memset(driver_error_bitmap_address_ccc + koffset, 0x80, ksize);
              }
            }
          }
          if (skip_ccc)
          {
            skip_position_ccc[skip_run_count_ccc] = current_position_ccc;
            current_position_ccc -= skip_size_ccc;
            last_skip_jump_position_ccc = current_position_ccc;
            current_position_ccc = block_align_up_ccc(current_position_ccc);
            skip_time_triggered_ccc = false;
            if (process_skip_ccc())
            {
              return -1;
            }
          }
          if (retstat < 0 || retstat > 0xff)
          {
#ifdef DEBUG
            if (fake_bad)
            {
              strcpy (tempmessage_ccc, "fakebad error");
              message_error_ccc(tempmessage_ccc);
              print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
              clear_error_message_ccc();
              return retstat;
            }
#endif
            int ret = call_command_on_error_ccc();
            if (ret)
            {
              return retstat;
            }
          }
          else
          {
            // if everything seems normal check to be sure
            if (check_device_ccc())
            {
              int ret = call_command_on_error_ccc();
              if (ret)
              {
                return DEVICE_ERROR_RETURN_CODE;
              }
            }
          }
          ata_error_ccc = ata_error_bak;
          ata_status_ccc = ata_status_bak;
        }
      }
    }
    last_sector_adjust = 0;
  }
  reset_skip_ccc();
  return 0;
}





int trim_forward_ccc(int status_type, int status_mask, int new_status_type)
{
  if (drive_locked_ccc)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGDRIVELOCKED]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  int dont_mark = 1;
  if (td_soft_reset_time_ccc >= sc_soft_reset_time_ccc)
  {
    dont_mark = 0;
  }
  direction_ccc = FORWARD;
  if (current_position_ccc < start_position_ccc)
  {
    current_position_ccc = start_position_ccc;
  }
  update_display_ccc(driver_mode_ccc ? DISPLAY_UPDATE_TIME : 0);
  if (stop_signal_ccc)
  {
    return STOP_SIGNAL_RETURN_CODE;
  }
  return_value_ccc = update_logfile_ccc(driver_mode_ccc ? LOG_UPDATE_TIME : 0);
  if (return_value_ccc)
  {
    return return_value_ccc;
  }
  reset_rate_ccc();
  while (current_position_ccc < end_position_ccc)
  {
    update_display_ccc(DISPLAY_UPDATE_TIME);
    if (stop_signal_ccc)
    {
      return STOP_SIGNAL_RETURN_CODE;
    }
    return_value_ccc = update_logfile_ccc(LOG_UPDATE_TIME);
    if (return_value_ccc)
    {
      return return_value_ccc;
    }
    int rsize = block_size_ccc;
    int line = find_block_ccc(current_position_ccc);
    if (line == -1)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGPOSNOTFOUND]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " 0x%llx", current_position_ccc);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return INTERNAL_ERROR_RETURN_CODE;
    }

    // check to see if current position is in a block of status type, if not then find the next block
    if ( (lstatus_ccc[line] & status_mask) != status_type)
    {
      line = find_next_block_ccc(line, status_type, status_mask);
      if (line == -1)
      {
        break;
      }
      current_position_ccc = lposition_ccc[line];
      // if past the end position then stop
      if (current_position_ccc >= end_position_ccc)
      {
        break;
      }
    }

    long long block_start = lposition_ccc[line];
    long long block_end = lposition_ccc[line] + lsize_ccc[line];
    long long forward_block_error = block_end;
    long long reverse_block_error = block_start;
    long long forward_block_error_size = rsize;
    long long orginal_status = lstatus_ccc[line] & ADD_STATUS_MASK;
    int trim_success = 1;
    int major_error = 0;


    // forward trim
    for (current_position_ccc = block_start; current_position_ccc < block_end; current_position_ccc += cluster_size_ccc)
    {
      update_display_ccc(DISPLAY_UPDATE_TIME);
      if (stop_signal_ccc)
      {
        return STOP_SIGNAL_RETURN_CODE;
      }
      return_value_ccc = update_logfile_ccc(LOG_UPDATE_TIME);
      if (return_value_ccc)
      {
        return return_value_ccc;
      }
      // check if the chunk fits in the block and if not then adjust read size
      if (current_position_ccc + rsize > block_end)
      {
        rsize = block_end - current_position_ccc;
      }

      // check if chunk goes past end position and adjust read size if needed
      if (current_position_ccc + rsize > end_position_ccc)
      {
        rsize = end_position_ccc - current_position_ccc;
      }

      // check domain
      if (rsize > 0)
      {
        long long current_start_pos = current_position_ccc;
        long long current_end_pos = current_position_ccc + rsize;
        int current_size = rsize;
        int ret = process_domain_ccc(current_position_ccc, rsize, domain_status_ccc, STATUS_MASK);
        if (verbosedebug_ccc)
        {
          sprintf (tempmessage_ccc, "ret3=%d\n", ret);
          message_debug_ccc(tempmessage_ccc, DEBUG10);
        }
        // if the return is 0 then no data to be read so adjust read size to 0
        if (ret <= 0)
        {
          rsize = 0;
        }
        // otherwise set read size to new size, and note that the current position may have been adjusted when the domain was processed
        else
        {
          rsize = ret;
          // sanity check
          if (current_position_ccc < current_start_pos || current_position_ccc + rsize > current_end_pos || rsize > current_size)
          {
            sprintf (tempmessage_ccc, "csposition=0x%llx ceposition=0x%llx nsposition=0x%llx neposition=0x%llx\n", current_start_pos, current_end_pos, current_position_ccc, current_position_ccc + rsize);
            strcpy (tempmessage_ccc, curlang_ccc[LANGPOSOUTOFRANGE]);
            message_error_ccc(tempmessage_ccc);
            print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
            clear_error_message_ccc();
            return INTERNAL_ERROR_RETURN_CODE;
          }
          rsize = domain_size_adjustment_ccc (cluster_size_ccc, rsize);
        }
      }

      if (rsize > 0)
      {
        int retstat = read_chunk_ccc(current_position_ccc, rsize);
        if (stop_signal_ccc)
        {
          return STOP_SIGNAL_RETURN_CODE;
        }
        // if read was successful then write the chunk, otherwise mark the chunk
#ifdef DEBUG
        long long fake_bad = current_position_ccc;
        if (fake_bad && fake_bad == FAKE_BAD)
        {
          //do_nanosleep_ccc(100000000);
          retstat = 0xfff3;
        }
        else
        {
          fake_bad = 0;
        }
#else
        long long fake_bad = 0;
#endif
        if (!retstat && !(ata_status_ccc & 1) && sense_key_ccc < 2 && !usb_read_residue_ccc && !fake_bad)
        {
          int ret = write_chunk_ccc(current_position_ccc, rsize);
          if (ret)
          {
            // write error
            return ret;
          }
          ret = change_chunk_status_ccc(current_position_ccc, rsize, FINISHED | get_timing_byte_ccc(), FULL_MASK);
          if (ret < 0)
          {
            // error changing chunk
            return ret;
          }
        }
        else
        {
          // normal read error
          // make a backup of status registers of error for display
          int ata_error_bak = ata_error_ccc;
          int ata_status_bak = ata_status_ccc;

          rate_modifier_ccc += (rsize * sector_size_ccc) / ( (cluster_size_ccc / 16) + 1);
          int skip_info = 0;

          // if using fpdma and there is an error the ncq log must be read
          if (!retstat && use_fpdma_ccc)
          {
            int ata_return_valid_bak = ata_return_valid_ccc;
            long long ata_lba_bak = ata_lba_ccc;
            int ncq_return = read_ncq_error_log_ccc();
            ata_return_valid_ccc = ata_return_valid_bak;
            ata_lba_ccc = ata_lba_bak;
            if (ncq_return || (ata_status_ccc & 1))
            {
              fprintf (stdout, "failed to read ncq log, return=0x%x status=%02x error=%02x\n", ncq_return, ata_status_ccc, ata_error_ccc);
            }
            else
            {
              ata_lba_ccc = ncq_error_log_data_ccc.lba;
            }
            ata_error_ccc = ata_error_bak;
            ata_status_ccc = ata_status_bak;
          }

          long long additional_status = set_additional_status_ccc(retstat, skip_info);
          int ret = change_chunk_status_ccc(current_position_ccc, rsize, ((rsize > block_size_ccc || (retstat && dont_mark)) ? new_status_type : BAD) | additional_status | get_timing_byte_ccc(), FULL_MASK);
          if (ret < 0)
          {
            // error changing chunk
            return ret;
          }
          if (driver_mode_ccc)
          {
            long long kposition = current_position_ccc * (sector_size_ccc / KERNEL_SECTOR_SIZE);
            long long koffset = kposition - read_ctrl_data_ccc(CTRL_KSECTOR_START);
            long long ksize = rsize * (sector_size_ccc / KERNEL_SECTOR_SIZE);
            if (koffset >= 0 && koffset + ksize <= DRIVER_TRANSFER_BUFFER_SIZE / KERNEL_SECTOR_SIZE)
            {
              memset(driver_transfer_buffer_address_ccc + (koffset * KERNEL_SECTOR_SIZE), 0, ksize * KERNEL_SECTOR_SIZE);
              memset(driver_error_bitmap_address_ccc + koffset, 0x80, ksize);
            }
          }

          if (retstat < 0 || retstat > 0xff)
          {
            int ret = call_command_on_error_ccc();
            if (ret)
            {
              major_error = retstat;
            }
          }
          else
          {
            // if everything seems normal check to be sure
            if (check_device_ccc())
            {
              int ret = call_command_on_error_ccc();
              if (ret)
              {
                major_error = DEVICE_ERROR_RETURN_CODE;
              }
            }
          }
          // if there is a major error then mark the rest of the block before returning
          if (major_error)
          {
            if ( (current_position_ccc + rsize) < block_end)
            {
              int ret = change_chunk_status_ccc(current_position_ccc + rsize, block_end - (current_position_ccc + rsize), new_status_type | orginal_status, FULL_MASK);
              if (ret < 0)
              {
                // error changing chunk
                return ret;
              }
            }
#ifdef DEBUG
            if (fake_bad && retstat > 0xff)
            {
              strcpy (tempmessage_ccc, "fakebad error");
              message_error_ccc(tempmessage_ccc);
              print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
              clear_error_message_ccc();
            }
#endif
            return major_error;
          }
          // exit for loop since we hit a read error
          trim_success = 0;
          forward_block_error = current_position_ccc;
          reverse_block_error = current_position_ccc;
          forward_block_error_size = rsize;
          ata_error_ccc = ata_error_bak;
          ata_status_ccc = ata_status_bak;
          break;
        }
      }
    }

    // if forward trim hit read error then do reverse trim;
    if (!trim_success)
    {
      trim_success = 1;
      rsize = block_size_ccc;
      // reverse trim
      for (current_position_ccc = block_end - block_size_ccc; current_position_ccc > block_start; current_position_ccc -= cluster_size_ccc)
      {
        update_display_ccc(DISPLAY_UPDATE_TIME);
        if (stop_signal_ccc)
        {
          return STOP_SIGNAL_RETURN_CODE;
        }
        return_value_ccc = update_logfile_ccc(LOG_UPDATE_TIME);
        if (return_value_ccc)
        {
          return return_value_ccc;
        }
        // check if the chunk is after the error
        if (current_position_ccc <= forward_block_error)
        {
          break;
        }

        // check if the chunk is within the forward_block_error and adjust if needed
        if (current_position_ccc < forward_block_error + forward_block_error_size)
        {
          long long position_adjust = (forward_block_error + forward_block_error_size) - current_position_ccc;
          current_position_ccc = forward_block_error + forward_block_error_size;
          rsize -= position_adjust;
        }

        // check if the chunk fits in the block and if not then adjust read size
        if (current_position_ccc + rsize > block_end)
        {
          rsize = block_end - current_position_ccc;
        }

        // check if chunk goes past end position and adjust read size if needed
        if (current_position_ccc + rsize > end_position_ccc)
        {
          rsize = end_position_ccc - current_position_ccc;
        }

        // check domain
        if (rsize > 0)
        {
          long long current_start_pos = current_position_ccc;
          long long current_end_pos = current_position_ccc + rsize;
          int current_size = rsize;
          int ret = process_domain_ccc(current_position_ccc, rsize, domain_status_ccc, STATUS_MASK);
          if (verbosedebug_ccc)
          {
            sprintf (tempmessage_ccc, "ret4=%d\n", ret);
            message_debug_ccc(tempmessage_ccc, DEBUG10);
          }
          // if the return is 0 then no data to be read so adjust read size to 0
          if (ret <= 0)
          {
            rsize = 0;
          }
          // otherwise set read size to new size, and note that the current position may have been adjusted when the domain was processed
          else
          {
            rsize = ret;
            // sanity check
            if (current_position_ccc < current_start_pos || current_position_ccc + rsize > current_end_pos || rsize > current_size)
            {
              sprintf (tempmessage_ccc, "csposition=0x%llx ceposition=0x%llx nsposition=0x%llx neposition=0x%llx\n", current_start_pos, current_end_pos, current_position_ccc, current_position_ccc + rsize);
              strcpy (tempmessage_ccc, curlang_ccc[LANGPOSOUTOFRANGE]);
              message_error_ccc(tempmessage_ccc);
              print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
              clear_error_message_ccc();
              return INTERNAL_ERROR_RETURN_CODE;
            }
            rsize = domain_size_adjustment_ccc (cluster_size_ccc, rsize);
          }
        }

        if (rsize > 0)
        {
          int retstat = read_chunk_ccc(current_position_ccc, rsize);
          if (stop_signal_ccc)
          {
            return STOP_SIGNAL_RETURN_CODE;
          }
          // if read was successful then write the chunk, otherwise mark the chunk
#ifdef DEBUG
          long long fake_bad = current_position_ccc;
          if (fake_bad && fake_bad == FAKE_BAD)
          {
            //do_nanosleep_ccc(100000000);
            retstat = 0xfff4;
          }
          else
          {
            fake_bad = 0;
          }
#else
          long long fake_bad = 0;
#endif
          if (!retstat && !(ata_status_ccc & 1) && sense_key_ccc < 2 && !usb_read_residue_ccc && !fake_bad)
          {
            int ret = write_chunk_ccc(current_position_ccc, rsize);
            if (ret)
            {
              // write error
              return ret;
            }
            ret = change_chunk_status_ccc(current_position_ccc, rsize, FINISHED | get_timing_byte_ccc(), FULL_MASK);
            if (ret < 0)
            {
              // error changing chunk
              return ret;
            }
          }
          else
          {
            // normal read error
            // make a backup of status registers of error for display
            int ata_error_bak = ata_error_ccc;
            int ata_status_bak = ata_status_ccc;

            rate_modifier_ccc += (rsize * sector_size_ccc) / ( (cluster_size_ccc / 16) + 1);
            int skip_info = 0;

            // if using fpdma and there is an error the ncq log must be read
            if (!retstat && use_fpdma_ccc)
            {
              int ata_return_valid_bak = ata_return_valid_ccc;
              long long ata_lba_bak = ata_lba_ccc;
              int ncq_return = read_ncq_error_log_ccc();
              ata_return_valid_ccc = ata_return_valid_bak;
              ata_lba_ccc = ata_lba_bak;
              if (ncq_return || (ata_status_ccc & 1))
              {
                fprintf (stdout, "failed to read ncq log, return=0x%x status=%02x error=%02x\n", ncq_return, ata_status_ccc, ata_error_ccc);
              }
              else
              {
                ata_lba_ccc = ncq_error_log_data_ccc.lba;
              }
              ata_error_ccc = ata_error_bak;
              ata_status_ccc = ata_status_bak;
            }

            long long additional_status = set_additional_status_ccc(retstat, skip_info);
            int ret = change_chunk_status_ccc(current_position_ccc, rsize, ((rsize > block_size_ccc || (retstat && dont_mark)) ? new_status_type : BAD) | additional_status | get_timing_byte_ccc(), FULL_MASK);
            if (ret < 0)
            {
              // error changing chunk
              return ret;
            }

#ifdef DEBUG
            if (fake_bad && retstat > 0xff)
            {
              strcpy (tempmessage_ccc, "fakebad error");
              message_error_ccc(tempmessage_ccc);
              print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
              clear_error_message_ccc();
            }
#endif
            if (retstat < 0 || retstat > 0xff)
            {
              int ret = call_command_on_error_ccc();
              if (ret)
              {
                major_error = retstat;
              }
            }
            else
            {
              // if everything seems normal check to be sure
              if (check_device_ccc())
              {
                int ret = call_command_on_error_ccc();
                if (ret)
                {
                  major_error = DEVICE_ERROR_RETURN_CODE;
                }
              }
            }
            // exit for loop since we hit a read error
            trim_success = 0;
            reverse_block_error = current_position_ccc;
            ata_error_ccc = ata_error_bak;
            ata_status_ccc = ata_status_bak;
            break;
          }
        }
      }
    }
    // if there is any data in between the errors that was not read then mark it
    if (!trim_success && reverse_block_error > forward_block_error + forward_block_error_size)
    {
      current_position_ccc = forward_block_error + forward_block_error_size;
      rsize = reverse_block_error - (forward_block_error + forward_block_error_size);
      if (rsize > 0)
      {
        int ret = change_chunk_status_ccc(current_position_ccc, rsize, new_status_type | orginal_status, FULL_MASK);
        if (ret < 0)
        {
          // error changing chunk
          return ret;
        }
      }
    }

    current_position_ccc = block_end;

    if (major_error)
    {
      return major_error;
    }
  }

  return 0;
}





int trim_reverse_ccc(int status_type, int status_mask, int new_status_type)
{
  if (drive_locked_ccc)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGDRIVELOCKED]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  int dont_mark = 1;
  if (td_soft_reset_time_ccc >= sc_soft_reset_time_ccc)
  {
    dont_mark = 0;
  }
  direction_ccc = REVERSE;
  int last_sector_adjust = 0;
  if (current_position_ccc >= end_position_ccc)
  {
    current_position_ccc = end_position_ccc - 1;
    last_sector_adjust = 1;
  }
  update_display_ccc(driver_mode_ccc ? DISPLAY_UPDATE_TIME : 0);
  if (stop_signal_ccc)
  {
    return STOP_SIGNAL_RETURN_CODE;
  }
  return_value_ccc = update_logfile_ccc(0);
  if (return_value_ccc)
  {
    return return_value_ccc;
  }
  reset_rate_ccc();
  while (current_position_ccc > start_position_ccc)
  {
    update_display_ccc(DISPLAY_UPDATE_TIME);
    if (stop_signal_ccc)
    {
      return STOP_SIGNAL_RETURN_CODE;
    }
    return_value_ccc = update_logfile_ccc(LOG_UPDATE_TIME);
    if (return_value_ccc)
    {
      return return_value_ccc;
    }
    int rsize = block_size_ccc;
    int line = find_block_ccc(current_position_ccc);
    if (line == -1)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGPOSNOTFOUND]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " 0x%llx", current_position_ccc);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return INTERNAL_ERROR_RETURN_CODE;
    }

    // check to see if current position is in a block of status type, if not then find the previous block
    if ( (lstatus_ccc[line] & status_mask) != status_type)
    {
      line = find_previous_block_ccc(line, status_type, status_mask);
      if (line == -1)
      {
        break;
      }
      current_position_ccc = lposition_ccc[line] + lsize_ccc[line] - rsize;
      // make sure position is still in the current block
      if (current_position_ccc < lposition_ccc[line])
      {
        current_position_ccc = lposition_ccc[line];
      }
      // if before the start position then adjust and if outside of current block then stop
      if (current_position_ccc < start_position_ccc)
      {
        current_position_ccc = start_position_ccc;
        if ( (current_position_ccc < lposition_ccc[line]) || (current_position_ccc > lposition_ccc[line] + lsize_ccc[line]) )
        {
          break;
        }
      }
    }
    else
    {
      // setup for previous chunk
      if (last_sector_adjust)
      {
        current_position_ccc = end_position_ccc;
      }
      current_position_ccc -= rsize;
      if (current_position_ccc < lposition_ccc[line])
      {
        current_position_ccc = lposition_ccc[line];
      }
    }
    last_sector_adjust = 0;

    long long block_start = lposition_ccc[line];
    long long block_end = lposition_ccc[line] + lsize_ccc[line];
    long long forward_block_error = block_end;
    long long reverse_block_error = block_start;
    long long forward_block_error_size = rsize;
    long long orginal_status = lstatus_ccc[line] & ADD_STATUS_MASK;
    int trim_success = 1;
    int major_error = 0;



    // reverse trim;
    trim_success = 1;
    rsize = block_size_ccc;
    // reverse trim
    for (current_position_ccc = block_end - block_size_ccc; current_position_ccc >= block_start; current_position_ccc -= cluster_size_ccc)
    {
      update_display_ccc(DISPLAY_UPDATE_TIME);
      if (stop_signal_ccc)
      {
        return STOP_SIGNAL_RETURN_CODE;
      }
      return_value_ccc = update_logfile_ccc(LOG_UPDATE_TIME);
      if (return_value_ccc)
      {
        return return_value_ccc;
      }

      // check if the chunk fits in the block and if not then adjust read size
      if (current_position_ccc + rsize > block_end)
      {
        rsize = block_end - current_position_ccc;
      }

      // check if chunk goes past end position and adjust read size if needed
      if (current_position_ccc + rsize > end_position_ccc)
      {
        rsize = end_position_ccc - current_position_ccc;
      }

      // check domain
      if (rsize > 0)
      {
        long long current_start_pos = current_position_ccc;
        long long current_end_pos = current_position_ccc + rsize;
        int current_size = rsize;
        int ret = process_domain_ccc(current_position_ccc, rsize, domain_status_ccc, STATUS_MASK);
        if (verbosedebug_ccc)
        {
          sprintf (tempmessage_ccc, "ret5=%d\n", ret);
          message_debug_ccc(tempmessage_ccc, DEBUG10);
        }
        // if the return is 0 then no data to be read so adjust read size to 0
        if (ret <= 0)
        {
          rsize = 0;
        }
        // otherwise set read size to new size, and note that the current position may have been adjusted when the domain was processed
        else
        {
          rsize = ret;
          // sanity check
          if (current_position_ccc < current_start_pos || current_position_ccc + rsize > current_end_pos || rsize > current_size)
          {
            sprintf (tempmessage_ccc, "csposition=0x%llx ceposition=0x%llx nsposition=0x%llx neposition=0x%llx\n", current_start_pos, current_end_pos, current_position_ccc, current_position_ccc + rsize);
            strcpy (tempmessage_ccc, curlang_ccc[LANGPOSOUTOFRANGE]);
            message_error_ccc(tempmessage_ccc);
            print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
            clear_error_message_ccc();
            return INTERNAL_ERROR_RETURN_CODE;
          }
          rsize = domain_size_adjustment_ccc (cluster_size_ccc, rsize);
        }
      }

      if (rsize > 0)
      {
        int retstat = read_chunk_ccc(current_position_ccc, rsize);
        if (stop_signal_ccc)
        {
          return STOP_SIGNAL_RETURN_CODE;
        }
        // if read was successful then write the chunk, otherwise mark the chunk
#ifdef DEBUG
        long long fake_bad = current_position_ccc;
        if (fake_bad && fake_bad == FAKE_BAD)
        {
          //do_nanosleep_ccc(100000000);
          retstat = 0xfff5;
        }
        else
        {
          fake_bad = 0;
        }
#else
        long long fake_bad = 0;
#endif
        if (!retstat && !(ata_status_ccc & 1) && sense_key_ccc < 2 && !usb_read_residue_ccc && !fake_bad)
        {
          int ret = write_chunk_ccc(current_position_ccc, rsize);
          if (ret)
          {
            // write error
            return ret;
          }
          ret = change_chunk_status_ccc(current_position_ccc, rsize, FINISHED | get_timing_byte_ccc(), FULL_MASK);
          if (ret < 0)
          {
            // error changing chunk
            return ret;
          }
        }
        else
        {
          // normal read error
          // make a backup of status registers of error for display
          int ata_error_bak = ata_error_ccc;
          int ata_status_bak = ata_status_ccc;

          rate_modifier_ccc += (rsize * sector_size_ccc) / ( (cluster_size_ccc / 16) + 1);
          int skip_info = 0;

          // if using fpdma and there is an error the ncq log must be read
          if (!retstat && use_fpdma_ccc)
          {
            int ata_return_valid_bak = ata_return_valid_ccc;
            long long ata_lba_bak = ata_lba_ccc;
            int ncq_return = read_ncq_error_log_ccc();
            ata_return_valid_ccc = ata_return_valid_bak;
            ata_lba_ccc = ata_lba_bak;
            if (ncq_return || (ata_status_ccc & 1))
            {
              fprintf (stdout, "failed to read ncq log, return=0x%x status=%02x error=%02x\n", ncq_return, ata_status_ccc, ata_error_ccc);
            }
            else
            {
              ata_lba_ccc = ncq_error_log_data_ccc.lba;
            }
            ata_error_ccc = ata_error_bak;
            ata_status_ccc = ata_status_bak;
          }

          long long additional_status = set_additional_status_ccc(retstat, skip_info);
          int ret = change_chunk_status_ccc(current_position_ccc, rsize, ((rsize > block_size_ccc || (retstat && dont_mark)) ? new_status_type : BAD) | additional_status | get_timing_byte_ccc(), FULL_MASK);
          if (ret < 0)
          {
            // error changing chunk
            return ret;
          }
          if (driver_mode_ccc)
          {
            long long kposition = current_position_ccc * (sector_size_ccc / KERNEL_SECTOR_SIZE);
            long long koffset = kposition - read_ctrl_data_ccc(CTRL_KSECTOR_START);
            long long ksize = rsize * (sector_size_ccc / KERNEL_SECTOR_SIZE);
            if (koffset >= 0 && koffset + ksize <= DRIVER_TRANSFER_BUFFER_SIZE / KERNEL_SECTOR_SIZE)
            {
              memset(driver_transfer_buffer_address_ccc + (koffset * KERNEL_SECTOR_SIZE), 0, ksize * KERNEL_SECTOR_SIZE);
              memset(driver_error_bitmap_address_ccc + koffset, 0x80, ksize);
            }
          }

          if (retstat < 0 || retstat > 0xff)
          {
            int ret = call_command_on_error_ccc();
            if (ret)
            {
              major_error = retstat;
            }
          }
          else
          {
            // if everything seems normal check to be sure
            if (check_device_ccc())
            {
              int ret = call_command_on_error_ccc();
              if (ret)
              {
                major_error = DEVICE_ERROR_RETURN_CODE;
              }
            }
          }
          // if there is a major error then mark the rest of the block before returning
          if (major_error)
          {
            if (current_position_ccc > block_start)
            {
              int ret = change_chunk_status_ccc(block_start, current_position_ccc - block_start, new_status_type | orginal_status, FULL_MASK);
              if (ret < 0)
              {
                // error changing chunk
                return ret;
              }
            }
#ifdef DEBUG
            if (fake_bad && retstat > 0xff)
            {
              strcpy (tempmessage_ccc, "fakebad error");
              message_error_ccc(tempmessage_ccc);
              print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
              clear_error_message_ccc();
            }
#endif
            return major_error;
          }
          // exit for loop since we hit a read error
          trim_success = 0;
          forward_block_error = current_position_ccc;
          reverse_block_error = current_position_ccc;
          forward_block_error_size = rsize;
          ata_error_ccc = ata_error_bak;
          ata_status_ccc = ata_status_bak;
          break;
        }
      }
    }


    // if reverse trim hit read error then do forward trim;
    if (!trim_success)
    {
      for (current_position_ccc = block_start; current_position_ccc < block_end; current_position_ccc += cluster_size_ccc)
      {
        update_display_ccc(DISPLAY_UPDATE_TIME);
        if (stop_signal_ccc)
        {
          return STOP_SIGNAL_RETURN_CODE;
        }
        return_value_ccc = update_logfile_ccc(LOG_UPDATE_TIME);
        if (return_value_ccc)
        {
          return return_value_ccc;
        }
        // check if the chunk is before the error
        if (current_position_ccc >= reverse_block_error)
        {
          break;
        }

        // check if the chunk is within the reverse_block_error and adjust if needed
        if ( (current_position_ccc + rsize) > reverse_block_error)
        {
          long long position_adjust = (current_position_ccc + rsize) - reverse_block_error;
          rsize -= position_adjust;
        }

        // check if the chunk fits in the block and if not then adjust read size
        if (current_position_ccc + rsize > block_end)
        {
          rsize = block_end - current_position_ccc;
        }

        // check if chunk goes past end position and adjust read size if needed
        if (current_position_ccc + rsize > end_position_ccc)
        {
          rsize = end_position_ccc - current_position_ccc;
        }

        // check domain
        if (rsize > 0)
        {
          long long current_start_pos = current_position_ccc;
          long long current_end_pos = current_position_ccc + rsize;
          int current_size = rsize;
          int ret = process_domain_ccc(current_position_ccc, rsize, domain_status_ccc, STATUS_MASK);
          if (verbosedebug_ccc)
          {
            sprintf (tempmessage_ccc, "ret6=%d\n", ret);
            message_debug_ccc(tempmessage_ccc, DEBUG10);
          }
          // if the return is 0 then no data to be read so adjust read size to 0
          if (ret <= 0)
          {
            rsize = 0;
          }
          // otherwise set read size to new size, and note that the current position may have been adjusted when the domain was processed
          else
          {
            rsize = ret;
            // sanity check
            if (current_position_ccc < current_start_pos || current_position_ccc + rsize > current_end_pos || rsize > current_size)
            {
              sprintf (tempmessage_ccc, "csposition=0x%llx ceposition=0x%llx nsposition=0x%llx neposition=0x%llx\n", current_start_pos, current_end_pos, current_position_ccc, current_position_ccc + rsize);
              strcpy (tempmessage_ccc, curlang_ccc[LANGPOSOUTOFRANGE]);
              message_error_ccc(tempmessage_ccc);
              print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
              clear_error_message_ccc();
              return INTERNAL_ERROR_RETURN_CODE;
            }
            rsize = domain_size_adjustment_ccc (cluster_size_ccc, rsize);
          }
        }

        if (rsize > 0)
        {
          int retstat = read_chunk_ccc(current_position_ccc, rsize);
          if (stop_signal_ccc)
          {
            return STOP_SIGNAL_RETURN_CODE;
          }
          // if read was successful then write the chunk, otherwise mark the chunk
#ifdef DEBUG
          long long fake_bad = current_position_ccc;
          if (fake_bad && fake_bad == FAKE_BAD)
          {
            //do_nanosleep_ccc(100000000);
            retstat = 0xfff6;
          }
          else
          {
            fake_bad = 0;
          }
#else
          long long fake_bad = 0;
#endif
          if (!retstat && !(ata_status_ccc & 1) && sense_key_ccc < 2 && !usb_read_residue_ccc && !fake_bad)
          {
            int ret = write_chunk_ccc(current_position_ccc, rsize);
            if (ret)
            {
              // write error
              return ret;
            }
            ret = change_chunk_status_ccc(current_position_ccc, rsize, FINISHED | get_timing_byte_ccc(), FULL_MASK);
            if (ret < 0)
            {
              // error changing chunk
              return ret;
            }
          }
          else
          {
            // normal read error
            // make a backup of status registers of error for display
            int ata_error_bak = ata_error_ccc;
            int ata_status_bak = ata_status_ccc;

            rate_modifier_ccc += (rsize * sector_size_ccc) / ( (cluster_size_ccc / 16) + 1);
            int skip_info = 0;

            // if using fpdma and there is an error the ncq log must be read
            if (!retstat && use_fpdma_ccc)
            {
              int ata_return_valid_bak = ata_return_valid_ccc;
              long long ata_lba_bak = ata_lba_ccc;
              int ncq_return = read_ncq_error_log_ccc();
              ata_return_valid_ccc = ata_return_valid_bak;
              ata_lba_ccc = ata_lba_bak;
              if (ncq_return || (ata_status_ccc & 1))
              {
                fprintf (stdout, "failed to read ncq log, return=0x%x status=%02x error=%02x\n", ncq_return, ata_status_ccc, ata_error_ccc);
              }
              else
              {
                ata_lba_ccc = ncq_error_log_data_ccc.lba;
              }
              ata_error_ccc = ata_error_bak;
              ata_status_ccc = ata_status_bak;
            }

            long long additional_status = set_additional_status_ccc(retstat, skip_info);
            int ret = change_chunk_status_ccc(current_position_ccc, rsize, ((rsize > block_size_ccc || (retstat && dont_mark)) ? new_status_type : BAD) | additional_status | get_timing_byte_ccc(), FULL_MASK);
            if (ret < 0)
            {
              // error changing chunk
              return ret;
            }

#ifdef DEBUG
            if (fake_bad && retstat > 0xff)
            {
              strcpy (tempmessage_ccc, "fakebad error");
              message_error_ccc(tempmessage_ccc);
              print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
              clear_error_message_ccc();
            }
#endif
            if (retstat < 0 || retstat > 0xff)
            {
              int ret = call_command_on_error_ccc();
              if (ret)
              {
                major_error = retstat;
              }
            }
            else
            {
              // if everything seems normal check to be sure
              if (check_device_ccc())
              {
                int ret = call_command_on_error_ccc();
                if (ret)
                {
                  major_error = DEVICE_ERROR_RETURN_CODE;
                }
              }
            }
            // exit for loop since we hit a read error
            trim_success = 0;
            forward_block_error = current_position_ccc;
            ata_error_ccc = ata_error_bak;
            ata_status_ccc = ata_status_bak;
            break;
          }
        }
      }
    }



    // if there is any data in between the errors that was not read then mark it
    if (!trim_success && reverse_block_error > forward_block_error + forward_block_error_size)
    {
      current_position_ccc = forward_block_error + forward_block_error_size;
      rsize = reverse_block_error - (forward_block_error + forward_block_error_size);
      if (rsize > 0)
      {
        int ret = change_chunk_status_ccc(current_position_ccc, rsize, new_status_type | orginal_status, FULL_MASK);
        if (ret < 0)
        {
          // error changing chunk
          return ret;
        }
      }
    }

    current_position_ccc = block_start;

    if (major_error)
    {
      return major_error;
    }
  }

  return 0;
}





int analyze_drive_ccc(int sections, int extended)
{
  if (drive_locked_ccc)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGDRIVELOCKED]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }

  skip_timeout_copy_ccc = skip_timeout_ccc;
  int status_type = NONTRIED;
  int status_mask = STATUS_MASK;
  int new_status_type = NONTRIMMED;
  long long start_position_bak = start_position_ccc;
  long long end_position_bak = end_position_ccc;
  long long current_position_bak = current_position_ccc;
  start_position_ccc = 0;
  end_position_ccc = source_total_size_ccc;
  strcpy(current_status_string_ccc, curlang_ccc[LANGANALYZING]);


  int dont_mark = 1;
  if (td_soft_reset_time_ccc >= sc_soft_reset_time_ccc)
  {
    dont_mark = 0;
  }
  current_position_ccc = start_position_ccc;
  update_display_ccc(0);
  return_value_ccc = update_logfile_ccc(0);
  if (return_value_ccc)
  {
    start_position_ccc = start_position_bak;
    end_position_ccc = end_position_bak;
    current_position_ccc = current_position_bak;
    return return_value_ccc;
  }
  reset_rate_ccc();

  int start = sections;
  analyze_stoptime_ccc = ANALYZETIME;    // maximum time to run in ms
  if (extended)
  {
    analyze_stoptime_ccc = ANALYZETIMEEXTENDED;
  }
  long long maxruntime = analyze_stoptime_ccc;
  int level = 0;
  int stop_level = 4;
  int multiplier = 8;
  int running_multiplier = start;

  // get start time in ms
  copy_start_time_ccc = get_elapsed_usec_ccc() / 1000;

  while (level < stop_level)
  {
    long long cut_size = source_total_size_ccc / running_multiplier;
    int i;
    for (i = 0; i < running_multiplier; i++)
    {
      long long list_pos = i / (running_multiplier / sections);
      current_position_ccc = cut_size * i;
      int n;
      int temp = start;
      int skip = 0;
      for (n = 0; n < level; n++)
      {
        if (i % (running_multiplier / temp) == 0)
        {
          //fprintf (stdout, "skip i=%d l=%lld  pos=%lld\n", i, list_pos, current_position_ccc);    //debug
          skip = 1;
          break;
        }
        temp = temp * multiplier;
      }
      if (skip)
      {
        continue;
      }
      //fprintf (stdout, "i=%d l=%lld  pos=%lld\n", i, list_pos, current_position_ccc);    //debug


      update_display_ccc(DISPLAY_UPDATE_TIME);
      if (stop_signal_ccc)
      {
        start_position_ccc = start_position_bak;
        end_position_ccc = end_position_bak;
        current_position_ccc = current_position_bak;
        return STOP_SIGNAL_RETURN_CODE;
      }
      return_value_ccc = update_logfile_ccc(LOG_UPDATE_TIME);
      if (return_value_ccc)
      {
        start_position_ccc = start_position_bak;
        end_position_ccc = end_position_bak;
        current_position_ccc = current_position_bak;
        return return_value_ccc;
      }

      int rsize = cluster_size_ccc;
      int line = find_block_ccc(current_position_ccc);
      if (line == -1)
      {
        strcpy (tempmessage_ccc, curlang_ccc[LANGPOSNOTFOUND]);
        message_error_ccc(tempmessage_ccc);
        sprintf (tempmessage_ccc, " 0x%llx", current_position_ccc);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        start_position_ccc = start_position_bak;
        end_position_ccc = end_position_bak;
        current_position_ccc = current_position_bak;
        return INTERNAL_ERROR_RETURN_CODE;
      }

      // check to see if current position is in a block of status type, if not then find the next block
      if ( (lstatus_ccc[line] & status_mask) != status_type)
      {
        line = find_next_block_ccc(line, status_type, status_mask);
        if (line == -1)
        {
          break;
        }
        current_position_ccc = lposition_ccc[line];
        // if past the end position then stop
        if (current_position_ccc >= end_position_ccc)
        {
          break;
        }
      }

      if (current_position_ccc >= cut_size * (i+1))
      {
        continue;
      }

      // check if the chunk fits in the block and if not then adjust read size
      long long block_end = lposition_ccc[line] + lsize_ccc[line];
      if (current_position_ccc + rsize > block_end)
      {
        rsize = block_end - current_position_ccc;
      }

      // check if chunk goes past end position and adjust read size if needed
      if (current_position_ccc + rsize > end_position_ccc)
      {
        rsize = end_position_ccc - current_position_ccc;
      }

      // align read size to block size to keep aligned
      long long aligned_position = block_align_ccc(current_position_ccc, block_offset_ccc);
      long long size_offset = current_position_ccc - aligned_position;
      if (size_offset != block_size_ccc && size_offset > 0 && size_offset < rsize)
      {
        rsize -= size_offset;
      }

      if (rsize > 0)
      {
        int retstat = read_chunk_ccc(current_position_ccc, rsize);
        if (stop_signal_ccc)
        {
          start_position_ccc = start_position_bak;
          end_position_ccc = end_position_bak;
          current_position_ccc = current_position_bak;
          return STOP_SIGNAL_RETURN_CODE;
        }
        // if read was successful then write the chunk, otherwise mark the chunk
        if (!retstat && !(ata_status_ccc & 1) && sense_key_ccc < 2 && !usb_read_residue_ccc)
        {
          int ret = write_chunk_ccc(current_position_ccc, rsize);
          if (ret)
          {
            // write error
            start_position_ccc = start_position_bak;
            end_position_ccc = end_position_bak;
            current_position_ccc = current_position_bak;
            return ret;
          }
          ret = change_chunk_status_ccc(current_position_ccc, rsize, FINISHED | get_timing_byte_ccc(), FULL_MASK);
          if (ret < 0)
          {
            // error changing chunk
            start_position_ccc = start_position_bak;
            end_position_ccc = end_position_bak;
            current_position_ccc = current_position_bak;
            return ret;
          }
          analyze_good_reads_ccc[list_pos]++;
          if (skip_timeout_ccc > 0 && current_read_time_ccc > skip_timeout_ccc)
          {
            analyze_slow_reads_ccc[list_pos]++;
          }


          // get data for slow response test
          if (current_read_time_ccc < analyze_slow_low_ccc[list_pos])
          {
            analyze_slow_position_ccc[list_pos] = current_position_ccc;
            analyze_slow_size_ccc[list_pos] = rsize;
            analyze_slow_low_ccc[list_pos] = current_read_time_ccc;
            analyze_slow_high_ccc[list_pos] = current_read_time_ccc;
          }

        }
        else
        {
          // make a backup of status registers of error for display
          int ata_error_bak = ata_error_ccc;
          int ata_status_bak = ata_status_ccc;

          rate_modifier_ccc += (rsize * sector_size_ccc) / ( (cluster_size_ccc / 16) + 1);
          int skip_info = 0;

          // normal read error
          long long additional_status = set_additional_status_ccc(retstat, skip_info);
          int ret = change_chunk_status_ccc(current_position_ccc, rsize, ((rsize > block_size_ccc || (retstat && dont_mark)) ? new_status_type : BAD) | additional_status | get_timing_byte_ccc(), FULL_MASK);
          if (ret < 0)
          {
            // error changing chunk
            start_position_ccc = start_position_bak;
            end_position_ccc = end_position_bak;
            current_position_ccc = current_position_bak;
            return ret;
          }

          if (retstat < 0 || retstat > 0xff)
          {
            int ret = call_command_on_error_ccc();
            if (ret)
            {
              start_position_ccc = start_position_bak;
              end_position_ccc = end_position_bak;
              current_position_ccc = current_position_bak;
              return ret;
            }
          }
          else
          {
            // if everything seems normal check to be sure
            if (check_device_ccc())
            {
              int ret = call_command_on_error_ccc();
              if (ret)
              {
                start_position_ccc = start_position_bak;
                end_position_ccc = end_position_bak;
                current_position_ccc = current_position_bak;
                return ret;
              }
            }
          }
          ata_error_ccc = ata_error_bak;
          ata_status_ccc = ata_status_bak;
          analyze_bad_reads_ccc[list_pos]++;
          if ( (retstat & 0xf0) == 0x30  || (retstat & 0xf0) == 0x40 )
          {
            analyze_timeouts_ccc[list_pos]++;
          }
        }
      }

      analyze_read_attempts_ccc[list_pos]++;

      if (current_read_time_ccc < analyze_low_time_ccc[list_pos])
      {
        analyze_low_time_ccc[list_pos] = current_read_time_ccc;
      }
      if (current_read_time_ccc > analyze_high_time_ccc[list_pos])
      {
        analyze_high_time_ccc[list_pos] = current_read_time_ccc;
      }
      analyze_read_time_ccc[list_pos] = analyze_read_time_ccc[list_pos] + current_read_time_ccc;

      long long runtime = get_elapsed_usec_ccc() / 1000;
      if (runtime > (copy_start_time_ccc + maxruntime))
      {
        break;
      }
    }
    running_multiplier = running_multiplier * multiplier;
    level++;

    long long runtime = get_elapsed_usec_ccc() / 1000;
    if (runtime > (copy_start_time_ccc + maxruntime))
    {
      break;
    }
    if (runtime > (copy_start_time_ccc + (maxruntime / multiplier) ) )
    {
      break;
    }
  }

  if (1)
  {
    // perform slow test
    strcpy(current_status_string_ccc, curlang_ccc[LANGDETECTINGVARIANCE]);
    update_display_ccc(0);
    maxruntime = ANALYZESLOWTIME;
    if (extended)
    {
      maxruntime = ANALYZESLOWTIMEEXTENDED;
    }
    long long slow_start_time = get_elapsed_usec_ccc() / 1000;
    analyze_stoptime_ccc = (slow_start_time - copy_start_time_ccc) + maxruntime;
    long long runtime = get_elapsed_usec_ccc() / 1000;
    long long delay_start_time = (maxruntime / 4) * 3;
    while (runtime < slow_start_time + maxruntime)
    {
      int i;
      for (i = sections - 1; i >= 0; i--)
      {
        update_display_ccc(DISPLAY_UPDATE_TIME);
        if (stop_signal_ccc)
        {
          start_position_ccc = start_position_bak;
          end_position_ccc = end_position_bak;
          current_position_ccc = current_position_bak;
          return STOP_SIGNAL_RETURN_CODE;
        }
        if (analyze_slow_low_ccc[i] > 0 || analyze_slow_position_ccc[i] > 0)
        {
          current_position_ccc = analyze_slow_position_ccc[i];
          int retstat = read_chunk_ccc(analyze_slow_position_ccc[i], analyze_slow_size_ccc[i]);
          if (retstat < 0 || retstat > 0xff)
          {
            int ret = call_command_on_error_ccc();
            if (ret)
            {
              start_position_ccc = start_position_bak;
              end_position_ccc = end_position_bak;
              current_position_ccc = current_position_bak;
              return ret;
            }
          }
          if (current_read_time_ccc < analyze_slow_low_ccc[i])
          {
            analyze_slow_low_ccc[i] = current_read_time_ccc;
          }
          if (current_read_time_ccc > analyze_slow_high_ccc[i])
          {
            analyze_slow_high_ccc[i] = current_read_time_ccc;
          }
          if (runtime > slow_start_time + delay_start_time)
          {
            do_nanosleep_ccc(100000000);    // 100ms  give the drive time to hang itself between reads
          }
          analyze_slow_total_reads_ccc++;
        }
      }
      runtime = get_elapsed_usec_ccc() / 1000;
    }
  }

  start_position_ccc = start_position_bak;
  end_position_ccc = end_position_bak;
  current_position_ccc = current_position_bak;

  update_display_ccc(0);

  return 0;
}





void test_skip_ccc(int size)
{
  // 128 good from 1MB to 300MB
  // 256 good from 2MB to 500MB
  // 512 good from 5MB to 900MB
  // 1024 good from 9MB to 1700MB
  // 2048 good from 17MB to 3300MB
  // 4096 good from 35MB to 6500MB
  // 8192 good from 70MB to 13000MB
  // 16384 good from 150MB to 26000MB
  // 32768 good from 300MB to 50000MB

  long long mb = test_skip_mb_ccc;
  long long limit = (mb * 1000000) / 512;
  direction_ccc = FORWARD;
  original_min_skip_size_ccc = size;
  min_skip_size_ccc = original_min_skip_size_ccc;
  skip_size_ccc = min_skip_size_ccc;
  max_skip_size_ccc = MAX_SKIP_SIZE;
  current_position_ccc = 0;
  total_skip_resets_ccc = 0;
  reset_skip_ccc();
  long long lastsize = skip_size_ccc;
  int i;
  for (i = 0; i < 20; i++)
  {
    current_position_ccc = 0;
    fprintf (stdout, "run=%d\n", i);
    while (current_position_ccc < limit)
    {
      long long diff = skip_size_ccc - lastsize;
      fprintf (stdout, "skipsize=%lld diff=%lld min=%lld runcount=%d pos=%lld\n", skip_size_ccc, diff, min_skip_size_ccc, skip_run_count_ccc, current_position_ccc);
      skip_position_ccc[skip_run_count_ccc] = current_position_ccc;
      current_position_ccc += skip_size_ccc;
      current_position_ccc = block_align_ccc(current_position_ccc, block_offset_ccc);
      lastsize = skip_size_ccc;
      process_skip_ccc();
    }
    reset_skip_ccc();
    fprintf (stdout, "skipsize=%lld runcount=%d pos=%lld\n", skip_size_ccc, skip_run_count_ccc, current_position_ccc);
    if (total_skip_resets_ccc)
    {
      fprintf (stdout, "too big\n");
      break;
    }
  }

}





int process_skip_ccc(void)
{
  int skip_run_count_exceeded = 0;
  if (skip_run_count_ccc == 0)
  {
    if (direction_ccc == FORWARD)
    {
      skip_run_start_ccc = current_position_ccc - skip_size_ccc;
    }
    else
    {
      skip_run_start_ccc = current_position_ccc + skip_size_ccc;
    }
  }
  skip_run_count_ccc++;
  if (skip_run_count_ccc > 0x3f)
  {
    skip_run_count_ccc = 0x3f;
    skip_run_count_exceeded = 1;
  }
  if (skip_time_triggered_ccc)
  {
    total_slow_skips_ccc++;
  }
  total_skip_count_ccc++;
  total_skip_size_ccc += skip_size_ccc;
  skip_count_ccc++;
  if (skip_count_ccc > skip_run_retard_count_ccc)
  {
    skip_count_ccc = 0;
    min_skip_size_ccc = ((min_skip_size_ccc * 950) / 1000);
    if (min_skip_size_ccc < original_min_skip_size_ccc)
    {
      min_skip_size_ccc = original_min_skip_size_ccc;
    }
  }
  last_skip_position_ccc = current_position_ccc;
  last_skip_size_ccc = skip_size_ccc;
  if (last_skip_size_ccc > highest_skip_ccc)
  {
    highest_skip_ccc = last_skip_size_ccc;
  }
  skipping_ccc = true;
  if (skip_fast_ccc)
  {
    long long new_skip = skip_size_ccc;
    if ( (skip_history_ccc[0] + skip_history_ccc[1]) / 2 < skip_size_ccc)
    {
      new_skip = (skip_size_ccc + skip_history_ccc[0] + skip_history_ccc[1]) / 3;
    }
    if (skip_run_count_ccc > 10)
    {
      new_skip = (new_skip * 1200) / 1000;
    }
    else if (skip_run_count_ccc > 7)
    {
      new_skip = (new_skip * 1100) / 1000;
    }
    else if (skip_run_count_ccc < 5)
    {
      new_skip = (new_skip * 990) / 1000;
    }
    if (min_skip_size_ccc < new_skip / 5)
    {
      min_skip_size_ccc = new_skip / 5;
    }
    //skip_size_ccc = ((skip_size_ccc * skip_mod_ccc) / 1000) + 50;
    skip_size_ccc = ((skip_size_ccc * skip_mod_ccc) / 1000) + ((skip_size_ccc / 100) * (skip_run_count_ccc / 4) + skip_run_count_ccc);
    skip_multiplier_ccc = (skip_multiplier_ccc * 1000) / 1925;
  }
  else
  {
    long long new_skip = skip_size_ccc;
    if ( (skip_history_ccc[0] + skip_history_ccc[1]) / 2 < skip_size_ccc)
    {
      new_skip = (skip_size_ccc + skip_history_ccc[0] + skip_history_ccc[1]) / 3;
    }
    if (skip_run_count_ccc > 12)
    {
      new_skip = (new_skip * 1200) / 1000;
    }
    else if (skip_run_count_ccc > 9)
    {
      new_skip = (new_skip * 1100) / 1000;
    }
    else if (skip_run_count_ccc < 8)
    {
      new_skip = (new_skip * 990) / 1000;
    }
    if (min_skip_size_ccc < new_skip / 4)
    {
      min_skip_size_ccc = new_skip / 4;
    }
    //skip_size_ccc = ((skip_size_ccc * skip_mod_ccc) / 1000) + 50;
    skip_size_ccc = ((skip_size_ccc * skip_mod_ccc) / 1000) + ((skip_size_ccc / 100) * (skip_run_count_ccc / 4) + skip_run_count_ccc);
    skip_multiplier_ccc = (skip_multiplier_ccc * 1000) / 1750;
  }
  skip_mod_ccc = skip_multiplier_ccc + 1000;

  //skip_size_ccc = block_align_ccc(skip_size_ccc, 0);

  if (skip_size_ccc >= max_skip_size_ccc)
  {
    skip_size_ccc = max_skip_size_ccc;
    if (skip_run_count_exceeded && max_skip_size_ccc <= (original_min_skip_size_ccc * 16))
    {
      min_skip_size_ccc = original_min_skip_size_ccc;
      total_skip_resets_ccc++;
      reset_skip_ccc();
      return 0;
    }
  }

  if (skip_run_count_exceeded)
  {
    min_skip_size_ccc = original_min_skip_size_ccc;
    total_skip_resets_ccc++;
    reset_skip_ccc();
    strcpy (tempmessage_ccc, curlang_ccc[LANGSKIPRESET]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return -1;
  }
  return 0;
}





int reset_skip_ccc(void)
{
  if (skip_fast_ccc)
  {
    skip_run_limit_ccc = SKIP_RUN_COUNT_FAST;
  }
  else
  {
    skip_run_limit_ccc = SKIP_RUN_COUNT;
  }
  if (skipping_ccc)
  {
    skipping_ccc = false;
    skip_history_ccc[4] = skip_history_ccc[3];
    skip_history_ccc[3] = skip_history_ccc[2];
    skip_history_ccc[2] = skip_history_ccc[1];
    skip_history_ccc[1] = skip_history_ccc[0];
    skip_history_ccc[0] = last_skip_size_ccc;
  }
  if (skip_run_count_ccc >= skip_run_limit_ccc)
  {
    if (direction_ccc == FORWARD)
    {
      last_total_skip_size_ccc = last_skip_jump_position_ccc - skip_run_start_ccc;
    }
    else
    {
      last_total_skip_size_ccc = skip_run_start_ccc - last_skip_jump_position_ccc;
    }
    total_skip_runs_ccc++;
    process_skip_run_ccc();
  }

  // if skip run is short then adjust min skip size
  // if it is too short then ignore and don't change it
  if (skip_fast_ccc)
  {
    if (skip_run_count_ccc < 4 && skip_run_count_ccc > 2)
    {
      min_skip_size_ccc = (min_skip_size_ccc * 990) / 1000;
    }
  }
  else
  {
    if (skip_run_count_ccc < 7 && skip_run_count_ccc > 4)
    {
      min_skip_size_ccc = (min_skip_size_ccc * 990) / 1000;
    }
  }
  if (min_skip_size_ccc < original_min_skip_size_ccc / 2)
  {
    min_skip_size_ccc = original_min_skip_size_ccc;
  }

  skip_size_ccc = min_skip_size_ccc;
  last_skip_size_ccc = 0;
  skip_run_count_ccc = 0;
  total_skip_size_ccc = 0;
  if (skip_fast_ccc)
  {
    skip_multiplier_ccc = SKIP_MULTI2;
  }
  else
  {
    skip_multiplier_ccc = SKIP_MULTI1;
  }
  skip_mod_ccc = skip_multiplier_ccc + 1000;
  return 0;
}




int process_skip_run_ccc(void)
{
  if (mark_bad_head_ccc)
  {
    if (direction_ccc == FORWARD)
    {
      int line = find_block_ccc(skip_position_ccc[0]);
      int skip_number = (lstatus_ccc[line] & 0x3f00) >> 8;
      int direction = (lstatus_ccc[line] & 0xc000) >> 8;
      int a = 0;
      while (direction != FORWARD_SKIP)
      {
        a++;
        if (a > 2)
        {
          break;
        }
        line++;
        skip_number = (lstatus_ccc[line] & 0x3f00) >> 8;
        direction = (lstatus_ccc[line] & 0xc000) >> 8;
      }
      if (direction == FORWARD_SKIP && skip_number == 0)
      {
        int i = 0;
        int n = 0;
        int last_i = 0;
        int previous_i = 0;
        int last_skip_line = 0;
        while (1)
        {
          i++;
          if (line + i > total_lines_ccc)
          {
            break;
          }
          if (lposition_ccc[line+i] > start_position_ccc && lposition_ccc[line+i] < end_position_ccc)
          {
            direction = (lstatus_ccc[line+i] & 0xc000) >> 8;
            if (direction == FORWARD_SKIP)
            {
              skip_number = (lstatus_ccc[line+i] & 0x3f00) >> 8;
              if (skip_number == n+1 || skip_number == 0x3f)
              {
                n++;
                previous_i = last_i;
                last_i = i;
                last_skip_line = i;
              }
              else if (skip_number == n && i == last_skip_line + 1)
              {
                last_skip_line = i;
              }
              else
              {
                break;
              }
            }
          }
          else
          {
            break;
          }
          if (n > skip_run_count_ccc)
          {
            break;
          }
        }
        if (n+1 >= skip_run_limit_ccc)
        {
          int x;
          int y = previous_i;
          if (skip_fast_ccc)
          {
            y = last_i;
          }
          for (x = 0; x <= y; x++)
          {
            lstatus_ccc[line+x] = lstatus_ccc[line+x] | BAD_HEAD;
          }
        }
      }
    }


    // reverse
    else
    {
      int line = find_block_ccc(skip_position_ccc[0]);
      int skip_number = (lstatus_ccc[line] & 0x3f00) >> 8;
      int direction = (lstatus_ccc[line] & 0xc000) >> 8;
      int a = 0;
      while (direction != REVERSE_SKIP)
      {
        a++;
        if (a > 2)
        {
          break;
        }
        line++;
        skip_number = (lstatus_ccc[line] & 0x3f00) >> 8;
        direction = (lstatus_ccc[line] & 0xc000) >> 8;
      }
      if (direction == REVERSE_SKIP && skip_number == 0)
      {
        while (1)
        {
          int next_skip_number = (lstatus_ccc[line+1] & 0x3f00) >> 8;
          int next_direction = (lstatus_ccc[line+1] & 0xc000) >> 8;
          if (next_direction == REVERSE_SKIP && next_skip_number == 0)
          {
            line++;
          }
          else
          {
            break;
          }
        }
      }
      if (direction == REVERSE_SKIP && skip_number == 0)
      {
        int i = 0;
        int n = 0;
        int last_i = 0;
        int previous_i = 0;
        int last_skip_line = 0;
        while (1)
        {
          i++;
          if (line - i < 0)
          {
            break;
          }
          if (lposition_ccc[line-i] >= start_position_ccc && lposition_ccc[line-i] < end_position_ccc)
          {
            direction = (lstatus_ccc[line-i] & 0xc000) >> 8;
            if (direction == REVERSE_SKIP)
            {
              skip_number = (lstatus_ccc[line-i] & 0x3f00) >> 8;
              if (skip_number == n+1 || skip_number == 0x3f)
              {
                n++;
                previous_i = last_i;
                last_i = i;
                last_skip_line = i;
              }
              else if (skip_number == n && i == last_skip_line + 1)
              {
                last_skip_line = i;
              }
              else
              {
                break;
              }
            }
          }
          else
          {
            break;
          }
          if (n > skip_run_count_ccc)
          {
            break;
          }
        }
        if (n+1 >= skip_run_limit_ccc)
        {
          int x;
          int y = previous_i;
          if (skip_fast_ccc)
          {
            y = last_i;
          }
          for (x = 0; x <= y; x++)
          {
            lstatus_ccc[line-x] = lstatus_ccc[line-x] | BAD_HEAD;
          }
        }
      }
    }
  }
  return 0;
}




int process_bad_head_ccc(void)
{
  int d;
  for (d = 0; d < total_lines_ccc; d++)
  {
    if ((lstatus_ccc[d] & STATUS_MASK) != 0xff)
    {
      lstatus_ccc[d] = lstatus_ccc[d] & BAD_HEAD_MASK;
    }
  }
  update_display_ccc(0);
  if (stop_signal_ccc)
  {
    return STOP_SIGNAL_RETURN_CODE;
  }
  current_position_ccc = start_position_ccc;
  int line = find_block_ccc(current_position_ccc);
  while (current_position_ccc < end_position_ccc && 1)
  {
    update_display_ccc(DISPLAY_UPDATE_TIME);
    if (stop_signal_ccc)
    {
      return STOP_SIGNAL_RETURN_CODE;
    }
    while (lposition_ccc[line] >= start_position_ccc && lposition_ccc[line] < end_position_ccc)
    {
      int skip_number = (lstatus_ccc[line] & 0x3f00) >> 8;
      int direction = (lstatus_ccc[line] & 0xc000) >> 8;
      if (direction == FORWARD_SKIP && skip_number == 0)
      {
        int i = 0;
        int n = 0;
        int last_i = 0;
        int previous_i = 0;
        while (1)
        {
          i++;
          if (line + i > total_lines_ccc)
          {
            break;
          }
          //fprintf (stdout, "i=%d n=%d\n", i, n);  //debug
          if (lposition_ccc[line+i] > start_position_ccc && lposition_ccc[line+i] < end_position_ccc)
          {
            direction = (lstatus_ccc[line+i] & 0xc000) >> 8;
            if (direction == FORWARD_SKIP)
            {
              skip_number = (lstatus_ccc[line+i] & 0x3f00) >> 8;
              if (skip_number == n+1 || skip_number == 0x3f)
              {
                n++;
                previous_i = last_i;
                last_i = i;
              }
              else
              {
                break;
              }
            }
          }
          else
          {
            break;
          }
        }
        if (n+1 >= skip_run_limit_ccc)
        {
          int x;
          for (x = 0; x <= previous_i; x++)
          {
            lstatus_ccc[line+x] = lstatus_ccc[line+x] | 0x80;
          }
        }
      }

      line++;
      if (line > total_lines_ccc)
      {
        break;
      }
    }
    if (line + 1 < total_lines_ccc)
    {
      current_position_ccc = lposition_ccc[line+1];
    }
    else
    {
      break;
    }
  }


  current_position_ccc = end_position_ccc - 1;
  line = find_block_ccc(current_position_ccc);
  while (current_position_ccc > start_position_ccc && 1)
  {
    update_display_ccc(DISPLAY_UPDATE_TIME);
    if (stop_signal_ccc)
    {
      return STOP_SIGNAL_RETURN_CODE;
    }
    while (lposition_ccc[line] >= start_position_ccc && lposition_ccc[line] < end_position_ccc)
    {
      //fprintf (stdout, "line=%d\n",line);  //debug
      int skip_number = (lstatus_ccc[line] & 0x3f00) >> 8;
      int direction = (lstatus_ccc[line] & 0xc000) >> 8;
      if (direction == REVERSE_SKIP && skip_number == 0)
      {
        int i = 0;
        int n = 0;
        int last_i = 0;
        int previous_i = 0;
        while (1)
        {
          i++;
          if (line - i < 0)
          {
            break;
          }
          //fprintf (stdout, "i=%d n=%d stat=0x%llx line=%d\n", i, n, lstatus_ccc[line-i], line-i);  //debug
          if (lposition_ccc[line-i] >= start_position_ccc && lposition_ccc[line-i] < end_position_ccc)
          {
            direction = (lstatus_ccc[line-i] & 0xc000) >> 8;
            if (direction == REVERSE_SKIP)
            {
              skip_number = (lstatus_ccc[line-i] & 0x3f00) >> 8;
              if (skip_number == n+1 || skip_number == 0x3f)
              {
                n++;
                previous_i = last_i;
                last_i = i;
              }
              else
              {
                break;
              }
            }
          }
          else
          {
            break;
          }
        }
        if (n+1 >= skip_run_limit_ccc)
        {
          int x;
          for (x = 0; x <= previous_i; x++)
          {
            lstatus_ccc[line-x] = lstatus_ccc[line-x] | 0x80;
          }
        }
      }

      line--;
      if (line < 0)
      {
        break;
      }
    }
    if (line - 1 > 0)
    {
      current_position_ccc = lposition_ccc[line-1];
    }
    else
    {
      break;
    }
  }
  return 0;
}





long long set_additional_status_ccc(int retstat, int skip_info)
{
  long long additional_status = 0;
  if ( direct_mode_ccc || ((scsi_passthrough_ccc || ata_passthrough_ccc || usb_mode_ccc) && ata_return_valid_ccc && ata_identify_success_ccc) )
  {
    additional_status = ((long long)retstat << 16) | ((long long)skip_info << 8) |  ((long long)ata_error_ccc << 48) | ((long long)ata_status_ccc << 40);
  }
  else
  {
    additional_status = ((long long)retstat << 16) | ((long long)skip_info << 8) | ((long long)sense_key_ccc << 56) | ((long long)asc_ccc << 48) | ((long long)ascq_ccc << 40);
  }
  return additional_status;

  if (scsi_passthrough_ccc || usb_mode_ccc || (ata_passthrough_ccc && !ata_return_valid_ccc) )
  {
    additional_status = ((long long)retstat << 16) | ((long long)skip_info << 8) | ((long long)sense_key_ccc << 56) | ((long long)asc_ccc << 48) | ((long long)ascq_ccc << 40);
  }
  else
  {
    additional_status = ((long long)retstat << 16) | ((long long)skip_info << 8) |  ((long long)ata_error_ccc << 48) | ((long long)ata_status_ccc << 40);
  }
  return additional_status;
}





long long get_timing_byte_ccc(void)
{
  unsigned long long readtime = current_read_time_ccc;   // time is in usec
  readtime = readtime / 1000000;
  if (readtime > 0x3f)
  {
    readtime = 0x3f;
  }
  readtime = readtime << 32;
  return readtime;
}





int read_chunk_ccc(long long position, int size)
{
  if (debug_ccc & DEBUG11)
  {
    fprintf (debug_file_ccc, "read_chunk 0x%06llx  0x%x\n", position, size);
  }
  //fprintf (stdout, "read_chunk 0x%06llx  0x%x\n", position, size);    //debug

  if (max_read_rate_ccc > 0)
  {
    long long time_passed = get_elapsed_usec_ccc() - last_read_start_time_ccc;
    long long time_to_pass = (last_read_size_ccc * 1000000) / max_read_rate_ccc;
    long long time_left = time_to_pass - time_passed;
    //fprintf (stdout, "time_passed=%lld  time_to_pass=%lld  time_left = %lld  last_read_size=%lld  max_read_rate=%lld\n", time_passed, time_to_pass, time_left, last_read_size_ccc, max_read_rate_ccc);    //debug
    // 1 second wait limit
    if (time_left > 1000000)
    {
      time_left = 1000000;
    }
    if (time_left > 0)
    {
      do_nanosleep_ccc((time_left * 1000));
    }
    last_read_start_time_ccc = get_elapsed_usec_ccc();
    last_read_size_ccc = size * sector_size_ccc;
  }

  ccc_main_buffer_size_ccc = size * sector_size_ccc;
  set_main_buffer_ccc();
  memset (ccc_buffer_ccc, 0, ccc_main_buffer_size_ccc);
  skip_time_triggered_ccc = false;
  usb_read_residue_ccc = 0;
  long long start_time = get_elapsed_usec_ccc();
  int ret = 0;

  if (generic_mode_ccc)
  {
    // make sure some unused things are clear
    sense_key_ccc = 0;
    asc_ccc = 0;
    ascq_ccc = 0;ata_error_ccc = 0;
    ata_status_ccc = 0;
    ata_lba_ccc = 0;
    ata_count_ccc = 0;
    ata_device_ccc = 0;
    ata_return_valid_ccc = 0;

    int read_ret = lseek(disk1_fd_ccc, position * sector_size_ccc, SEEK_SET);
    if (read_ret == -1)
    {
      check_message_ccc = true;
      strcpy (tempmessage_ccc, curlang_ccc[LANGCANTSEEKSOURCE]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " %s (%s)", disk_1_ccc, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return INPUT_DEVICE_ERROR_RETURN_CODE;
    }
    read_ret = read(disk1_fd_ccc, ccc_buffer_ccc, size * sector_size_ccc);
    if (0 && read_ret == -1)    // this is disabled as bad sectors cause simple I/O errors
    {
      check_message_ccc = true;
      strcpy (tempmessage_ccc, curlang_ccc[LANGCANTREADSOURCE]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " %s (%s)", disk_1_ccc, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return INPUT_DEVICE_ERROR_RETURN_CODE;
    }
    else if (read_ret != size * sector_size_ccc)
    {
      //fprintf (stdout, "return read size was %d\n", ret);    //debug
      sprintf (tempmessage_ccc, "error %d reading sector %lld size %d (%s)\n", ret, position * sector_size_ccc, size * sector_size_ccc, strerror(errno));    //debug
      message_debug_ccc(tempmessage_ccc, DEBUG16);
      ret = 1;
    }
  }
  else if (scsi_passthrough_ccc || usb_mode_ccc)
  {
    ret = scsi_read_ccc(position, size);
  }
  else if (pio_mode_ccc)
  {
    ret = read_ata_pio_ccc(position, size);
  }
  else
  {
    int rarc_bit = 1;
    if (use_fpdma_ccc)
    {
      wait_for_ds_bit_ccc = true;
      if (rebuild_assist_enabled_ccc && rebuild_assist_ccc && current_status_ccc == REBUILDASSIST)
      {
        rarc_bit = 0;
      }
    }
    //fprintf (stdout, "position=%lld\n", position);    //debug
    ret = read_ata_dma_ccc(position, size, rarc_bit);
    wait_for_ds_bit_ccc = false;
  }
  if (stop_signal_ccc)
  {
    return STOP_SIGNAL_RETURN_CODE;
  }

  // read twice on timeout
  if (enable_read_twice_ccc && (direct_mode_ccc || usb_mode_ccc) && (ret == 0x31 || ret == 0x30))
  {
    unsigned long long soft_reset_time_bak = soft_reset_time_ccc;
    soft_reset_time_ccc = rt_soft_reset_time_ccc;
    if (usb_mode_ccc)
    {
      ret = scsi_read_ccc(position, size);
    }
    else if (pio_mode_ccc)
    {
      ret = read_ata_pio_ccc(position, size);
    }
    else
    {
      int rarc_bit = 1;
      if (use_fpdma_ccc)
      {
        wait_for_ds_bit_ccc = true;
        if (rebuild_assist_enabled_ccc && rebuild_assist_ccc && current_status_ccc == REBUILDASSIST)
        {
          rarc_bit = 0;
        }
      }
      //fprintf (stdout, "position=%lld\n", position);    //debug
      ret = read_ata_dma_ccc(position, size, rarc_bit);
      wait_for_ds_bit_ccc = false;
    }
    soft_reset_time_ccc = soft_reset_time_bak;
  }

  long long end_time = get_elapsed_usec_ccc();
  long long elapsed_time = end_time - start_time;
  current_read_time_ccc = elapsed_time;
  if (elapsed_time > longest_read_time_ccc)
  {
    longest_read_time_ccc = elapsed_time;
  }
  if (skip_timeout_ccc > 0 && elapsed_time > skip_timeout_ccc)
  {
    skip_time_triggered_ccc = true;
  }

  int io_host_status_bak = io_host_status_ccc;

  // check for device fault
  if (scsi_passthrough_ccc || usb_mode_ccc || (ata_passthrough_ccc && !ata_return_valid_ccc))
  {
    if (sense_key_ccc == 0x4 && asc_ccc == 0x44 && ascq_ccc == 0 && check_device_ccc())
    {
      check_message_ccc = true;
      strcpy (tempmessage_ccc, curlang_ccc[LANGDEVICEFAULT]);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return DEVICE_FAULT_RETURN_CODE;
    }
  }
  else if (!generic_mode_ccc)
  {
    if ( (ata_status_ccc & 1) && (ata_error_ccc & 0x20) && check_device_ccc() )
    {
      check_message_ccc = true;
      strcpy (tempmessage_ccc, curlang_ccc[LANGDEVICEFAULT]);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return DEVICE_FAULT_RETURN_CODE;
    }
  }

  // check for device error
  if (scsi_passthrough_ccc || ata_passthrough_ccc)
  {
    if (io_host_status_bak != 0)
    {
      if (check_device_ccc())
      {
        check_message_ccc = true;
        strcpy (tempmessage_ccc, curlang_ccc[LANGHOSTIOFAULT]);
        message_error_ccc(tempmessage_ccc);
        sprintf(tempmessage_ccc, " %d (%s)", io_host_status_bak, host_error_code_ccc(io_host_status_bak));
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return DEVICE_ERROR_RETURN_CODE - io_host_status_bak;
      }
      if (1)
      {
        // some usb devices can give a host io error during normal good reading, so this will cause them to fail
        sprintf (tempmessage_ccc, "Host IO error %d during read (%s)\n", io_host_status_bak, host_error_code_ccc(io_host_status_bak));
        message_console_log_ccc(tempmessage_ccc, 0);
        return io_host_status_bak;
      }
    }
  }

  // check if past allowed read limit
  if (recovery_past_limit_ccc)
  {
    check_message_ccc = true;
    strcpy (tempmessage_ccc, curlang_ccc[LANGFREEREADLIMIT]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return GENERAL_ERROR_RETURN_CODE;
  }
  //fprintf (stdout, "return was %d\n", ret);    //debug
  if (debug_ccc & DEBUG11)
  {
    fprintf (debug_file_ccc, "return=%d status=0x%02x error=0x%02x sense=%02x %02x %02x\n", ret, ata_status_ccc, ata_error_ccc, sense_key_ccc, asc_ccc, ascq_ccc);
  }
  return ret;
}





int write_chunk_ccc(long long position, int size)
{
  if (debug_ccc & DEBUG12)
  {
    fprintf (debug_file_ccc, "write_chunk 0x%06llx  0x%x\n", position, size);
  }
  if (output_offset_ccc > -1)
  {
    position = (output_offset_ccc - input_offset_ccc) + position;
  }

  int sector_size_bak = sector_size_ccc;
  long long main_buffer_size_bak = ccc_main_buffer_size_ccc;
  void* temp_buffer = NULL;
  if (output_sector_size_adjustment_ccc != 0)
  {
    // sanity check
    if (size * sector_size_ccc != (int)ccc_main_buffer_size_ccc)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGCANTWRITEDESTINATION]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, "\ninternal program sector size error");
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return OUTPUT_DEVICE_ERROR_RETURN_CODE;
    }
    temp_buffer = malloc(main_buffer_size_bak);
    memcpy (temp_buffer, ccc_buffer_ccc, main_buffer_size_bak);
    sector_size_ccc = sector_size_ccc + output_sector_size_adjustment_ccc;
    ccc_main_buffer_size_ccc = size * sector_size_ccc;
    memset (ccc_buffer_ccc, 0, ccc_main_buffer_size_ccc);
    int copy_size = sector_size_bak;
    if (sector_size_ccc < sector_size_bak)
    {
      copy_size = sector_size_ccc;
    }
    int i;
    for (i = 0; i < size; i++)
    {
      memcpy (ccc_buffer_ccc + (sector_size_ccc * i), temp_buffer + (sector_size_bak * i), copy_size);
    }

  }

  if (enable_scsi_write_ccc)
  {
    // special scsi write mode
    long long start_time = get_elapsed_usec_ccc();
    int ret = scsi_write_ccc(position, size);
    if (ret)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGCANTWRITEDESTINATION]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " %s (%s)", disk_2_ccc, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return OUTPUT_DEVICE_ERROR_RETURN_CODE;
    }
    else if (sense_key_ccc > 1)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGERRORWRITINGDESTINATION]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, "\n %02x %02x %02x", sense_key_ccc, asc_ccc, ascq_ccc);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return OUTPUT_DEVICE_ERROR_RETURN_CODE;
    }
    else if (io_host_status_ccc)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGERRORWRITINGDESTINATION]);
      message_error_ccc(tempmessage_ccc);
      sprintf(tempmessage_ccc, "\n %d (%s)", io_host_status_ccc, host_error_code_ccc(io_host_status_ccc));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return OUTPUT_DEVICE_ERROR_RETURN_CODE;
    }
    long long end_time = get_elapsed_usec_ccc();
    long long elapsed_time = end_time - start_time;
    if (elapsed_time > (SCSI_WRITE_TIMEOUT * 1000))
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGERRORWRITINGDESTINATION]);
      message_error_ccc(tempmessage_ccc);
      sprintf(tempmessage_ccc, "\n %lldms", elapsed_time / 1000);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return OUTPUT_DEVICE_ERROR_RETURN_CODE;
    }
  }
  else
  {
    int ret = lseek(disk2_fd_ccc, position * sector_size_ccc, SEEK_SET);
    if (ret == -1)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGCANTSEEKDESTINATION]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " %s (%s)", disk_2_ccc, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return OUTPUT_DEVICE_ERROR_RETURN_CODE;
    }
    ret = write(disk2_fd_ccc, ccc_buffer_ccc, size * sector_size_ccc);
    if (ret == -1)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGCANTWRITEDESTINATION ]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " %s (%s)", disk_2_ccc, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return OUTPUT_DEVICE_ERROR_RETURN_CODE;
    }
    else if (ret != size * sector_size_ccc)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGCANTWRITEDESTINATION ]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " %s", disk_2_ccc);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return OUTPUT_DEVICE_ERROR_RETURN_CODE;
    }
  }
  memcpy(&display_buffer_ccc, ccc_buffer_ccc, DISPLAY_BUFFER_SIZE);

  if (output_sector_size_adjustment_ccc != 0)
  {
    sector_size_ccc = sector_size_bak;
    ccc_main_buffer_size_ccc = main_buffer_size_bak;
    memcpy (ccc_buffer_ccc, temp_buffer, main_buffer_size_bak);    // this is needed if there are possible operations after the write
    free(temp_buffer);
  }

  if (driver_mode_ccc)
  {
    long long kposition = position * (sector_size_ccc / KERNEL_SECTOR_SIZE);
    long long koffset = kposition - read_ctrl_data_ccc(CTRL_KSECTOR_START);
    long long ksize = size * (sector_size_ccc / KERNEL_SECTOR_SIZE);
    //fprintf (stdout, "source accessed position%lld offset%lld size%lld ", kposition, koffset, ksize);    //debug
    if (koffset >= 0 && koffset + ksize <= DRIVER_TRANSFER_BUFFER_SIZE / KERNEL_SECTOR_SIZE)
    {
      //fprintf (stdout, "read from source\n");    //debug
      memcpy(driver_transfer_buffer_address_ccc + (koffset * KERNEL_SECTOR_SIZE), ccc_buffer_ccc, ksize * KERNEL_SECTOR_SIZE);
      memset(driver_error_bitmap_address_ccc + koffset, 0, ksize);
    }
    else
    {
      //fprintf (stdout, "not read\n");    //debug
    }
  }

  return 0;
}





int process_rebuild_assist_chunk_ccc(int status_type, int new_status_type, int retstat, int rsize, int skip_info)
{
  // set this for use later
  long long additional_status = set_additional_status_ccc(retstat, skip_info);
  long long original_timing_byte = get_timing_byte_ccc();
  //fprintf (stdout, "process_rebuild_assist_chunk current_position_ccc=%llx ncq_error_log_data_ccc.lba=%llx\n", current_position_ccc, ncq_error_log_data_ccc.lba);    //debug
  if (debug_ccc & DEBUG13)
  {
    fprintf (debug_file_ccc, "process_rebuild_assist_chunk newstatus=0x%x  retstat=0x%x  rsize=0x%x  skipinfo=0x%x\n", new_status_type, retstat, rsize, skip_info);
  }

  int predicted_error = 0;
  if ( ncq_error_log_data_ccc.sense_key == SKEY_ABORTED_COMMAND && ncq_error_log_data_ccc.sense_asc == get_asc_from_additional_sense_ccc(ASC_MULTIPLE_READ_ERRORS) && ncq_error_log_data_ccc.sense_ascq == get_ascq_from_additional_sense_ccc(ASC_MULTIPLE_READ_ERRORS) && ncq_error_log_data_ccc.final_lba > current_position_ccc )
  {
    predicted_error = 1;
  }

  int trust_returned_data = 0;
  if (predicted_error)
  {
    // if it is the first sector of the read then mark the whole chunk as same status type with bad head
    if (ncq_error_log_data_ccc.lba == current_position_ccc)
    {
      //fprintf (stdout, "rsize=%x\n", rsize);    //debug
      int ret = change_chunk_status_ccc(current_position_ccc, rsize, status_type | BAD_HEAD | additional_status | get_timing_byte_ccc(), FULL_MASK);
      if (ret < 0)
      {
        return ret;
      }
      if (driver_mode_ccc)
      {
        long long kposition = current_position_ccc * (sector_size_ccc / KERNEL_SECTOR_SIZE);
        long long koffset = kposition - read_ctrl_data_ccc(CTRL_KSECTOR_START);
        long long ksize = rsize * (sector_size_ccc / KERNEL_SECTOR_SIZE);
        if (koffset >= 0 && koffset + ksize <= DRIVER_TRANSFER_BUFFER_SIZE / KERNEL_SECTOR_SIZE)
        {
          memset(driver_transfer_buffer_address_ccc + (koffset * KERNEL_SECTOR_SIZE), 0, ksize * KERNEL_SECTOR_SIZE);
          memset(driver_error_bitmap_address_ccc + koffset, 0x80, ksize);
        }
      }
    }
    else
    {
      // if not the first sector of the read then write the data up to the reported error and mark as good
      int rsize2 = ncq_error_log_data_ccc.lba - current_position_ccc;
      int ret = 0;
      // if the error lba is greater than it should be then correct size
      if (rsize2 > rsize)
      {
        rsize2 = rsize;
      }
      if (trust_returned_data)
      {
        ret = write_chunk_ccc(current_position_ccc, rsize2);
        if (ret)
        {
          // write error
          return ret;
        }
        ret = change_chunk_status_ccc(current_position_ccc, rsize2, FINISHED | get_timing_byte_ccc(), FULL_MASK);
        if (ret < 0)
        {
          // error changing chunk
          return ret;
        }
      }
      else
      {
        // don't write data if we don't trust it, just mark same status
        ret = change_chunk_status_ccc(current_position_ccc, rsize2, status_type | get_timing_byte_ccc(), FULL_MASK);
        if (ret < 0)
        {
          // error changing chunk
          return ret;
        }
      }
      // if there is leftover then mark it as same status type with bad head
      int rsize3 = rsize - rsize2;
      //fprintf (stdout, "rsize=%x rsize2=%x rsize3=%x\n", rsize, rsize2, rsize3);    //debug
      if (rsize3 > 0)
      {
        ret = change_chunk_status_ccc(current_position_ccc + rsize2, rsize3, status_type | BAD_HEAD | additional_status | original_timing_byte, FULL_MASK);
        if (ret < 0)
        {
          // error changing chunk
          return ret;
        }
      }
      if (driver_mode_ccc)
      {
        long long kposition = (current_position_ccc + rsize2) * (sector_size_ccc / KERNEL_SECTOR_SIZE);
        long long koffset = kposition - read_ctrl_data_ccc(CTRL_KSECTOR_START);
        long long ksize = (rsize3) * (sector_size_ccc / KERNEL_SECTOR_SIZE);
        if (koffset >= 0 && koffset + ksize <= DRIVER_TRANSFER_BUFFER_SIZE / KERNEL_SECTOR_SIZE)
        {
          memset(driver_transfer_buffer_address_ccc + (koffset * KERNEL_SECTOR_SIZE), 0, ksize * KERNEL_SECTOR_SIZE);
          memset(driver_error_bitmap_address_ccc + koffset, 0x80, ksize);
        }
      }
    }
    // now mark the rest of the head as a bad head
    long long start = current_position_ccc + rsize;
    if (ncq_error_log_data_ccc.lba >= start && ncq_error_log_data_ccc.lba < ncq_error_log_data_ccc.final_lba)
    {
      start = ncq_error_log_data_ccc.lba;
    }
    long long end = ncq_error_log_data_ccc.final_lba;
    long long position = start;
    while (1)
    {
      int block = find_block_ccc(position);
      if (block == -1)
      {
        break;
      }
      if (lposition_ccc[block] > end)
      {
        break;
      }
      long long block_end = lposition_ccc[block] + lsize_ccc[block] - 1;
      if (block_end >= end)
      {
        // bad head fits within block, change chunk status and break
        int ret = change_chunk_status_ccc(position, (end + 1) - position, lstatus_ccc[block] | BAD_HEAD, FULL_MASK);
        if (ret < 0)
        {
          return ret;
        }
        break;
      }
      else
      {
        // bad head doesn't fit in block so only change block status, advance position and continue
        int ret = change_chunk_status_ccc(position, (block_end + 1) - position, lstatus_ccc[block] | BAD_HEAD, FULL_MASK);
        if (ret < 0)
        {
          return ret;
        }
        position = block_end + 1;
        if (position > end)
        {
          break;
        }
      }
    }
  }
  else
  {
    // unpredicted error
    // if it is the first sector of the read then mark it as bad and mark the rest as needed
    if (ncq_error_log_data_ccc.lba == current_position_ccc)
    {
      int ret = change_chunk_status_ccc(current_position_ccc, block_size_ccc, BAD | additional_status | get_timing_byte_ccc(), FULL_MASK);
      if (ret < 0)
      {
        return ret;
      }
      ret = change_chunk_status_ccc(current_position_ccc + block_size_ccc, rsize - block_size_ccc, new_status_type | additional_status | get_timing_byte_ccc(), FULL_MASK);
      if (ret < 0)
      {
        return ret;
      }
      if (driver_mode_ccc)
      {
        long long kposition = current_position_ccc * (sector_size_ccc / KERNEL_SECTOR_SIZE);
        long long koffset = kposition - read_ctrl_data_ccc(CTRL_KSECTOR_START);
        long long ksize = rsize * (sector_size_ccc / KERNEL_SECTOR_SIZE);
        if (koffset >= 0 && koffset + ksize <= DRIVER_TRANSFER_BUFFER_SIZE / KERNEL_SECTOR_SIZE)
        {
          memset(driver_transfer_buffer_address_ccc + (koffset * KERNEL_SECTOR_SIZE), 0, ksize * KERNEL_SECTOR_SIZE);
          memset(driver_error_bitmap_address_ccc + koffset, 0x80, ksize);
        }
      }
    }
    else
    {
      // if not the first sector of the read then write the data up to the reported error and mark as good
      int rsize2 = ncq_error_log_data_ccc.lba - current_position_ccc;
      int ret = 0;
      // if the error lba is greater than it should be then correct size
      if (rsize2 > rsize)
      {
        rsize2 = rsize;
      }
      if (trust_returned_data)
      {
        ret = write_chunk_ccc(current_position_ccc, rsize2);
        if (ret)
        {
          // write error
          return ret;
        }
        ret = change_chunk_status_ccc(current_position_ccc, rsize2, FINISHED | get_timing_byte_ccc(), FULL_MASK);
        if (ret < 0)
        {
          // error changing chunk
          return ret;
        }
      }
      else
      {
        // don't write data if we don't trust it, just mark same status
        ret = change_chunk_status_ccc(current_position_ccc, rsize2, status_type | get_timing_byte_ccc(), FULL_MASK);
        if (ret < 0)
        {
          // error changing chunk
          return ret;
        }
      }
      // mark the reported bad sector as bad if there is one
      if (rsize > rsize2)
      {
        ret = change_chunk_status_ccc(current_position_ccc + rsize2, block_size_ccc, BAD | additional_status | original_timing_byte, FULL_MASK);
        if (ret < 0)
        {
          return ret;
        }
      }
      // if there is leftover then mark it as new status type
      int rsize3 = rsize - rsize2 - block_size_ccc;
      if (rsize3 > 0)
      {
        ret = change_chunk_status_ccc(current_position_ccc + rsize2 + block_size_ccc, rsize3, new_status_type | additional_status | original_timing_byte, FULL_MASK);
        if (ret < 0)
        {
          // error changing chunk
          return ret;
        }
      }
      if (driver_mode_ccc)
      {
        long long kposition = (current_position_ccc + rsize2) * (sector_size_ccc / KERNEL_SECTOR_SIZE);
        long long koffset = kposition - read_ctrl_data_ccc(CTRL_KSECTOR_START);
        long long ksize = (rsize3 + block_size_ccc) * (sector_size_ccc / KERNEL_SECTOR_SIZE);
        if (koffset >= 0 && koffset + ksize <= DRIVER_TRANSFER_BUFFER_SIZE / KERNEL_SECTOR_SIZE)
        {
          memset(driver_transfer_buffer_address_ccc + (koffset * KERNEL_SECTOR_SIZE), 0, ksize * KERNEL_SECTOR_SIZE);
          memset(driver_error_bitmap_address_ccc + koffset, 0x80, ksize);
        }
      }
    }
  }
  return 0;
}





int process_chunk_ccc(int new_status_type, int retstat, int rsize, int skip_info)
{
  int dont_mark = 1;
  if (td_soft_reset_time_ccc >= sc_soft_reset_time_ccc)
  {
    dont_mark = 0;
  }
  // set this for use later
  long long additional_status = set_additional_status_ccc(retstat, skip_info);
  long long original_timing_byte = get_timing_byte_ccc();
  //fprintf (stdout, "process_chunk current_position_ccc=%llx ata_lba_ccc=%llx\n", current_position_ccc, ata_lba_ccc);    //debug
  if (!lba_supported_ccc || pio_mode_ccc)
  {
    // process the whole chunk as normal read error if chs or pio
    int ret = change_chunk_status_ccc(current_position_ccc, rsize, ((rsize > block_size_ccc || (retstat && dont_mark)) ? new_status_type : BAD) | additional_status | get_timing_byte_ccc(), FULL_MASK);
    if (ret < 0)
    {
      // error changing chunk
      return ret;
    }
    if (driver_mode_ccc)
    {
      long long kposition = current_position_ccc * (sector_size_ccc / KERNEL_SECTOR_SIZE);
      long long koffset = kposition - read_ctrl_data_ccc(CTRL_KSECTOR_START);
      long long ksize = rsize * (sector_size_ccc / KERNEL_SECTOR_SIZE);
      if (koffset >= 0 && koffset + ksize <= DRIVER_TRANSFER_BUFFER_SIZE / KERNEL_SECTOR_SIZE)
      {
        memset(driver_transfer_buffer_address_ccc + (koffset * KERNEL_SECTOR_SIZE), 0, ksize * KERNEL_SECTOR_SIZE);
        memset(driver_error_bitmap_address_ccc + koffset, 0x80, ksize);
      }
    }
    return 0;
  }
  if (debug_ccc & DEBUG13)
  {
    fprintf (debug_file_ccc, "process_chunk newstatus=0x%x  retstat=0x%x  rsize=0x%x  skipinfo=0x%x\n", new_status_type, retstat, rsize, skip_info);
  }

  // if it is the first sector of the read then mark it as bad and mark the rest as needed
  if (ata_lba_ccc == current_position_ccc)
  {
    int ret = change_chunk_status_ccc(current_position_ccc, block_size_ccc, BAD | additional_status | get_timing_byte_ccc(), FULL_MASK);
    if (ret < 0)
    {
      return ret;
    }
    ret = change_chunk_status_ccc(current_position_ccc + block_size_ccc, rsize - block_size_ccc, new_status_type | additional_status | get_timing_byte_ccc(), FULL_MASK);
    if (ret < 0)
    {
      return ret;
    }
    if (driver_mode_ccc)
    {
      long long kposition = current_position_ccc * (sector_size_ccc / KERNEL_SECTOR_SIZE);
      long long koffset = kposition - read_ctrl_data_ccc(CTRL_KSECTOR_START);
      long long ksize = rsize * (sector_size_ccc / KERNEL_SECTOR_SIZE);
      if (koffset >= 0 && koffset + ksize <= DRIVER_TRANSFER_BUFFER_SIZE / KERNEL_SECTOR_SIZE)
      {
        memset(driver_transfer_buffer_address_ccc + (koffset * KERNEL_SECTOR_SIZE), 0, ksize * KERNEL_SECTOR_SIZE);
        memset(driver_error_bitmap_address_ccc + koffset, 0x80, ksize);
      }
    }
  }
  else
  {
    // if not the first sector of the read then do a new read up to the reported error and recheck
    int rsize2 = ata_lba_ccc - current_position_ccc;
    int retstat2 = read_chunk_ccc(current_position_ccc, rsize2);
    if (stop_signal_ccc)
    {
      return STOP_SIGNAL_RETURN_CODE;
    }
    // if read was successful then write the chunk, otherwise mark the whole chunk
    if (!retstat2 && !(ata_status_ccc & 1) && sense_key_ccc < 2 && !usb_read_residue_ccc)
    {
      int ret = write_chunk_ccc(current_position_ccc, rsize2);
      if (ret)
      {
        // write error
        return ret;
      }
      ret = change_chunk_status_ccc(current_position_ccc, rsize2, FINISHED | get_timing_byte_ccc(), FULL_MASK);
      if (ret < 0)
      {
        // error changing chunk
        return ret;
      }
      // mark the reported bad sector as bad
      ret = change_chunk_status_ccc(current_position_ccc + rsize2, block_size_ccc, BAD | additional_status | original_timing_byte, FULL_MASK);
      if (ret < 0)
      {
        return ret;
      }
      // if there is leftover then mark it as new status type
      int rsize3 = rsize - rsize2 - block_size_ccc;
      if (rsize3 > 0)
      {
        ret = change_chunk_status_ccc(current_position_ccc + rsize2 + block_size_ccc, rsize3, new_status_type | additional_status | original_timing_byte, FULL_MASK);
        if (ret < 0)
        {
          // error changing chunk
          return ret;
        }
      }
      if (driver_mode_ccc)
      {
        long long kposition = (current_position_ccc + rsize2) * (sector_size_ccc / KERNEL_SECTOR_SIZE);
        long long koffset = kposition - read_ctrl_data_ccc(CTRL_KSECTOR_START);
        long long ksize = (rsize3 + block_size_ccc) * (sector_size_ccc / KERNEL_SECTOR_SIZE);
        if (koffset >= 0 && koffset + ksize <= DRIVER_TRANSFER_BUFFER_SIZE / KERNEL_SECTOR_SIZE)
        {
          memset(driver_transfer_buffer_address_ccc + (koffset * KERNEL_SECTOR_SIZE), 0, ksize * KERNEL_SECTOR_SIZE);
          memset(driver_error_bitmap_address_ccc + koffset, 0x80, ksize);
        }
      }
    }
    else
    {
      // if it failed the second read attempt then process the whole chunk as normal read error
      int ret = change_chunk_status_ccc(current_position_ccc, rsize, ((rsize > block_size_ccc || (retstat && dont_mark)) ? new_status_type : BAD) | additional_status | original_timing_byte, FULL_MASK);
      if (ret < 0)
      {
        // error changing chunk
        return ret;
      }
      if (driver_mode_ccc)
      {
        long long kposition = current_position_ccc * (sector_size_ccc / KERNEL_SECTOR_SIZE);
        long long koffset = kposition - read_ctrl_data_ccc(CTRL_KSECTOR_START);
        long long ksize = rsize * (sector_size_ccc / KERNEL_SECTOR_SIZE);
        if (koffset >= 0 && koffset + ksize <= DRIVER_TRANSFER_BUFFER_SIZE / KERNEL_SECTOR_SIZE)
        {
          memset(driver_transfer_buffer_address_ccc + (koffset * KERNEL_SECTOR_SIZE), 0, ksize * KERNEL_SECTOR_SIZE);
          memset(driver_error_bitmap_address_ccc + koffset, 0x80, ksize);
        }
      }
    }
  }
  return 0;
}





int process_domain_ccc(long long position, int size, int status, int status_mask)
{
  if (do_domain_ccc)
  {
    int not_in_domain = 0;
    int found_block = 0;
    int first = 0;
    int last = domain_lines_ccc - 1;
    int middle = (first + last) / 2;
    while (first <= last)
    {
      if (0 && verbosedebug_ccc)
      {
        sprintf (tempmessage_ccc, "position=0x%llx size=0x%x dposition=0x%llx dsize=0x%llx\n", position, size, dposition_ccc[middle], dsize_ccc[middle]);
        message_debug_ccc(tempmessage_ccc, DEBUG10);
      }
      // check if any part of the chunk is in the domain
      if ( (position >= dposition_ccc[middle] && position < dposition_ccc[middle] + dsize_ccc[middle]) || (dposition_ccc[middle] >= position && dposition_ccc[middle] < position + size) )
      {
        // found a block
        found_block = 1;
        break;
      }
      else if (position > dposition_ccc[middle])
      {
        first = middle + 1;
      }
      else
      {
        last = middle - 1;
      }
      middle = (first + last) / 2;
    }
    if (found_block)
    {
      if (verbosedebug_ccc)
      {
        sprintf (tempmessage_ccc, "position=0x%llx size=0x%x dposition=0x%llx dsize=0x%llx\n", position, size, dposition_ccc[middle], dsize_ccc[middle]);
        message_debug_ccc(tempmessage_ccc, DEBUG10);
      }
      // check if chunk fits in domain
      if (position >= dposition_ccc[middle] && position + size < dposition_ccc[middle] + dsize_ccc[middle])
      {
        // chunk fits completely inside domain block
        if ( (dstatus_ccc[middle] & status_mask) == status)
        {
          // status matches so read the whole chunk
          if (verbosedebug_ccc)
          {
            sprintf (tempmessage_ccc, "chunk fits in domain\n");
            message_debug_ccc(tempmessage_ccc, DEBUG10);
          }
          return size;
        }
        else
        {
          // status does not match so don't read anything
          return not_in_domain;
        }
      }

      // chunk does not fit in domain so check for other domain blocks of proper type
      int best_domain = middle;
      // if going forward then check first if there is a previous domain
      if ( (!reverse_ccc && current_status_ccc != PHASE2) || (reverse_ccc && current_status_ccc == PHASE2) )
      {
        int i = 1;
        int keep_checking = 1;
        int found_domain = 0;
        while (keep_checking)
        {
          // range check
          if (middle - i >= 0)
          {
            // check if any part of the chunk is in the domain
            if ( (position >= dposition_ccc[middle-i] && position < dposition_ccc[middle-i] + dsize_ccc[middle-i]) || (dposition_ccc[middle-i] >= position && dposition_ccc[middle-i] < position + size) )
            {
              // check if domain matches status type
              if ( (dstatus_ccc[middle-i] & status_mask) == status)
              {
                // domain matches so mark as best
                best_domain = middle - i;
                found_domain = 1;
                if (verbosedebug_ccc)
                {
                  sprintf (tempmessage_ccc, "best1=%d dposition=0x%llx dsize=0x%llx\n",best_domain, dposition_ccc[best_domain], dsize_ccc[best_domain]);
                  message_debug_ccc(tempmessage_ccc, DEBUG10);
                }
              }
            }
            else
            {
              // no part of the chunk is in the domain so break
              break;
            }
          }
          else
          {
            // out of range so break
            break;
          }
          i++;
        }
        // if no matching domain yet then look for next possible domain
        if (!found_domain)
        {
          i = 1;
          keep_checking = 1;
          while (keep_checking)
          {
            // range check
            if (middle + i <= domain_lines_ccc - 1)
            {
              // check if any part of the chunk is in the domain
              if ( (position >= dposition_ccc[middle+i] && position < dposition_ccc[middle+i] + dsize_ccc[middle+i]) || (dposition_ccc[middle+i] >= position && dposition_ccc[middle+i] < position + size) )
              {
                // check if domain matches status type
                if ( (dstatus_ccc[middle+i] & status_mask) == status)
                {
                  // domain matches so mark as best
                  best_domain = middle + i;
                  found_domain = 1;
                  if (verbosedebug_ccc)
                  {
                    sprintf (tempmessage_ccc, "best2=%d dposition=0x%llx dsize=0x%llx\n",best_domain, dposition_ccc[best_domain], dsize_ccc[best_domain]);
                    message_debug_ccc(tempmessage_ccc, DEBUG10);
                  }
                  // we found the next best so break;
                  break;
                }
              }
              else
              {
                // no part of the chunk is in the domain so break
                break;
              }
            }
            else
            {
              // out of range so break
              break;
            }
            i++;
          }
        }
      }
      // for reverse check for the next domain first
      else
      {
        int i = 1;
        int keep_checking = 1;
        int found_domain = 0;
        while (keep_checking)
        {
          // range check
          if (middle + i <= domain_lines_ccc - 1)
          {
            // check if any part of the chunk is in the domain
            if ( (position >= dposition_ccc[middle+i] && position < dposition_ccc[middle+i] + dsize_ccc[middle+i]) || (dposition_ccc[middle+i] >= position && dposition_ccc[middle+i] < position + size) )
            {
              // check if domain matches status type
              if ( (dstatus_ccc[middle+i] & status_mask) == status)
              {
                // domain matches so mark as best
                best_domain = middle + i;
                found_domain = 1;
                if (verbosedebug_ccc)
                {
                  sprintf (tempmessage_ccc, "best3=%d dposition=0x%llx dsize=0x%llx\n",best_domain, dposition_ccc[best_domain], dsize_ccc[best_domain]);
                  message_debug_ccc(tempmessage_ccc, DEBUG10);
                }
              }
            }
            else
            {
              // no part of the chunk is in the domain so break
              break;
            }
          }
          else
          {
            // out of range so break
            break;
          }
          i++;
        }
        // if no matching domain yet then look for previous domain
        if (!found_domain)
        {
          i = 1;
          keep_checking = 1;
          while (keep_checking)
          {
            // range check
            if (middle - i >= 0)
            {
              // check if any part of the chunk is in the domain
              if ( (position >= dposition_ccc[middle-i] && position < dposition_ccc[middle-i] + dsize_ccc[middle-i]) || (dposition_ccc[middle-i] >= position && dposition_ccc[middle-i] < position + size) )
              {
                // check if domain matches status type
                if ( (dstatus_ccc[middle-i] & status_mask) == status)
                {
                  // domain matches so mark as best
                  best_domain = middle - i;
                  found_domain = 1;
                  if (verbosedebug_ccc)
                  {
                    sprintf (tempmessage_ccc, "best4=%d dposition=0x%llx dsize=0x%llx\n",best_domain, dposition_ccc[best_domain], dsize_ccc[best_domain]);
                    message_debug_ccc(tempmessage_ccc, DEBUG10);
                  }
                  // we found a previous domain so break
                  break;
                }
              }
              else
              {
                // no part of the chunk is in the domain so break
                break;
              }
            }
            else
            {
              // out of range so break
              break;
            }
            i++;
          }
        }
      }
      middle = best_domain;
      if (verbosedebug_ccc)
      {
        sprintf (tempmessage_ccc, "best domain dposition=0x%llx dsize=0x%llx\n", dposition_ccc[middle], dsize_ccc[middle]);
        message_debug_ccc(tempmessage_ccc, DEBUG10);
      }

      // only do further processing if the domain is of proper status
      if ( (dstatus_ccc[middle] & status_mask) == status)
      {
        // check if position is inside domain
        if (position >= dposition_ccc[middle] && position < dposition_ccc[middle] + dsize_ccc[middle])
        {
          // position is inside domain so only the size needs to be adjusted if the status matches
          if ( (dstatus_ccc[middle] & status_mask) == status)
          {
            // status matches to adjust read size
            long long domain_end_position = dposition_ccc[middle] + dsize_ccc[middle];
            long long current_end_position = position + size;
            long long new_size = size;
            if (domain_end_position < current_end_position)
            {
              new_size = domain_end_position - position;
            }
            if (verbosedebug_ccc)
            {
              sprintf (tempmessage_ccc, "newsize1=0x%llx\n", new_size);
              message_debug_ccc(tempmessage_ccc, DEBUG10);
            }
            return new_size;
          }
        }

        // check if the domain fits inside the chunk
        if (dposition_ccc[middle] >= position && dposition_ccc[middle] + dsize_ccc[middle] <= position + size)
        {
          // both the start and end position are outside of the domain so both the position and size need to be set
          long long new_position = dposition_ccc[middle];
          long long new_size = dsize_ccc[middle];
          current_position_ccc = new_position;
          if (verbosedebug_ccc)
          {
            sprintf (tempmessage_ccc, "newposition2=0x%llx newsize2=0x%llx\n", current_position_ccc, new_size);
            message_debug_ccc(tempmessage_ccc, DEBUG10);
          }
          return new_size;
        }

        // check if end is inside domain
        if (position + size > dposition_ccc[middle] && position + size <= dposition_ccc[middle] + dsize_ccc[middle])
        {
          // end position is inside domain so the current position needs to be adjusted along with the size
          long long current_end_position = position + size;
          long long new_position = dposition_ccc[middle];
          long long new_size = current_end_position - new_position;
          current_position_ccc = new_position;
          if (verbosedebug_ccc)
          {
            sprintf (tempmessage_ccc, "newposition3=0x%llx newsize3=0x%llx\n", current_position_ccc, new_size);
            message_debug_ccc(tempmessage_ccc, DEBUG10);
          }
          return new_size;
        }
        strcpy (tempmessage_ccc, curlang_ccc[LANGDOMAINERROR]);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return INTERNAL_ERROR_RETURN_CODE;
      }
      else
      {
        // domain is not of proper status type
        if (verbosedebug_ccc)
        {
          sprintf (tempmessage_ccc, "domain wrong status dposition=0x%llx dsize=0x%llx\n", dposition_ccc[middle], dsize_ccc[middle]);
          message_debug_ccc(tempmessage_ccc, DEBUG10);
        }
        return not_in_domain;
      }
    }
    else
    {
      // no matching domain block found
      if (verbosedebug_ccc)
      {
        sprintf (tempmessage_ccc, "no matching domain position=0x%llx size=0x%x \n", position, size);
        message_debug_ccc(tempmessage_ccc, DEBUG10);
      }
      return not_in_domain;
    }
  }
  // domain not processed so return the original size
  return size;
}





int domain_size_adjustment_ccc(int original_size, int requested_size)
{
  if (do_domain_ccc)
  {
    long long newsize = requested_size;
    if (requested_size > 0 && requested_size < original_size && requested_size < driver_minimum_cluster_size_ccc)
    {
      newsize = driver_minimum_cluster_size_ccc;
      if (newsize > original_size)
      {
        newsize = original_size;
      }
      if (current_position_ccc + newsize > end_position_ccc)
      {
        newsize = end_position_ccc - current_position_ccc;
      }
      int line = find_block_ccc(current_position_ccc);
      if (line == -1)
      {
        return requested_size;
      }
      long long block_end = lposition_ccc[line] + lsize_ccc[line];
      long long chunck_end = current_position_ccc + newsize;
      if (block_end < chunck_end)
      {
        newsize = block_end - current_position_ccc;
      }
      if (newsize <= 0 || newsize > original_size)
      {
        newsize = requested_size;
      }
    }
    return newsize;
  }
  return requested_size;
}





int scsi_read_ccc(long long position, int size)
{
  if (scsi_read16_supported_ccc)
  {
    command_length_ccc = 16;
  }
  else if (scsi_read12_supported_ccc)
  {
    command_length_ccc = 12;
  }
  else if (scsi_read10_supported_ccc)
  {
    command_length_ccc = 10;
  }
  else
  {
    command_length_ccc = 6;
  }

  switch (command_length_ccc)
  {
    case 6:
      passthrough_ccc.scsi_cmd[0] = 0x08; // read 6 command
      passthrough_ccc.scsi_cmd[1] = (unsigned char)((position >> 16) & 0x1f);
      passthrough_ccc.scsi_cmd[2] = (unsigned char)((position>> 8) & 0xff);
      passthrough_ccc.scsi_cmd[3] = (unsigned char)(position & 0xff);
      passthrough_ccc.scsi_cmd[4] = (unsigned char)(size & 0xff);
      passthrough_ccc.scsi_cmd[5] = 0;
      break;

    case 10:
      passthrough_ccc.scsi_cmd[0] = 0x28; // read 10 command
      passthrough_ccc.scsi_cmd[1] = 0;
      passthrough_ccc.scsi_cmd[2] = (unsigned char)((position >> 24) & 0xff);
      passthrough_ccc.scsi_cmd[3] = (unsigned char)((position >> 16) & 0xff);
      passthrough_ccc.scsi_cmd[4] = (unsigned char)((position>> 8) & 0xff);
      passthrough_ccc.scsi_cmd[5] = (unsigned char)(position & 0xff);
      passthrough_ccc.scsi_cmd[6] = 0;
      passthrough_ccc.scsi_cmd[7] = (unsigned char)((size >> 8) & 0xff);
      passthrough_ccc.scsi_cmd[8] = (unsigned char)(size & 0xff);
      passthrough_ccc.scsi_cmd[9] = 0;
      break;

    case 12:
      passthrough_ccc.scsi_cmd[0] = 0xa8; // read 12 command
      passthrough_ccc.scsi_cmd[1] = 0;
      passthrough_ccc.scsi_cmd[2] = (unsigned char)((position >> 24) & 0xff);
      passthrough_ccc.scsi_cmd[3] = (unsigned char)((position >> 16) & 0xff);
      passthrough_ccc.scsi_cmd[4] = (unsigned char)((position>> 8) & 0xff);
      passthrough_ccc.scsi_cmd[5] = (unsigned char)(position & 0xff);
      passthrough_ccc.scsi_cmd[6] = (unsigned char)((size >> 24) & 0xff);
      passthrough_ccc.scsi_cmd[7] = (unsigned char)((size >> 16) & 0xff);
      passthrough_ccc.scsi_cmd[8] = (unsigned char)((size >> 8) & 0xff);
      passthrough_ccc.scsi_cmd[9] = (unsigned char)(size & 0xff);
      passthrough_ccc.scsi_cmd[10] = 0;
      passthrough_ccc.scsi_cmd[11] = 0;
      break;

    case 16:
      passthrough_ccc.scsi_cmd[0] = 0x88; // read 16 command
      passthrough_ccc.scsi_cmd[1] = 0;
      passthrough_ccc.scsi_cmd[2] = (unsigned char)((position >> 56) & 0xff);
      passthrough_ccc.scsi_cmd[3] = (unsigned char)((position >> 48) & 0xff);
      passthrough_ccc.scsi_cmd[4] = (unsigned char)((position >> 40) & 0xff);
      passthrough_ccc.scsi_cmd[5] = (unsigned char)((position >> 32) & 0xff);
      passthrough_ccc.scsi_cmd[6] = (unsigned char)((position >> 24) & 0xff);
      passthrough_ccc.scsi_cmd[7] = (unsigned char)((position >> 16) & 0xff);
      passthrough_ccc.scsi_cmd[8] = (unsigned char)((position >> 8) & 0xff);
      passthrough_ccc.scsi_cmd[9] = (unsigned char)(position & 0xff);
      passthrough_ccc.scsi_cmd[10] = (unsigned char)((size >> 24) & 0xff);
      passthrough_ccc.scsi_cmd[11] = (unsigned char)((size >> 16) & 0xff);
      passthrough_ccc.scsi_cmd[12] = (unsigned char)((size >> 8) & 0xff);
      passthrough_ccc.scsi_cmd[13] = (unsigned char)(size & 0xff);
      passthrough_ccc.scsi_cmd[14] = 0;
      passthrough_ccc.scsi_cmd[15] = 0;
      break;
  }
  passthrough_ccc.sg_xfer_direction = SG_DXFER_FROM_DEV;
  usb_transfer_direction_ccc = USB_DIRECTION_IN;
  return_value_ccc = do_scsi_cmd_ccc(current_disk_ccc);
  return return_value_ccc;
}





int scsi_check_read_commands_ccc(void)
{
  int check6 = 1;
  //int check10 = 1;    we always check read 10
  int check12 = 1;
  int check16 = 1;
  if (ata_identify_success_ccc)    // only check 10 and 16 if ata
  {
    check6 = 0;
    check12 = 0;
    scsi_read6_supported_ccc = false;
    scsi_read12_supported_ccc = false;
  }
  if (!extended_support_ccc)    // only check 16 if extended
  {
    check16 = 0;
    scsi_read16_supported_ccc = false;
  }
  if (usb_mode_ccc && !usb_allow_ata_ccc)    // don't check 6 and 12 if normal usb mode
  {
    check6 = 0;
    check12 = 0;
    scsi_read6_supported_ccc = false;
    scsi_read12_supported_ccc = false;
  }

  fprintf (stdout, "checking scsi read command support\n");
  unsigned char buffbak[ccc_main_buffer_size_ccc];
  memcpy (buffbak, ccc_buffer_ccc, ccc_main_buffer_size_ccc);
  unsigned long long ccc_main_buffer_size_bak = ccc_main_buffer_size_ccc;
  ccc_main_buffer_size_ccc = sector_size_ccc;
  usb_transfer_direction_ccc = USB_DIRECTION_IN;
  set_main_buffer_ccc();
  passthrough_ccc.sg_xfer_direction = SG_DXFER_FROM_DEV;
  unsigned long long soft_reset_time_bak;

  if (check6)
  {
    passthrough_ccc.scsi_cmd[0] = 0x08; // read 6 command
    passthrough_ccc.scsi_cmd[1] = 0;
    passthrough_ccc.scsi_cmd[2] = 0;
    passthrough_ccc.scsi_cmd[3] = 0;
    passthrough_ccc.scsi_cmd[4] = 1;
    passthrough_ccc.scsi_cmd[5] = 0;
    command_length_ccc = 6;
    soft_reset_time_bak = soft_reset_time_ccc;
    soft_reset_time_ccc = initial_busy_wait_time_ccc;
    return_value_ccc = do_scsi_cmd_ccc(current_disk_ccc);
    soft_reset_time_ccc = soft_reset_time_bak;
    if (return_value_ccc)
    {
      scsi_read6_supported_ccc = false;
      //return -1;
    }
    else if (sense_key_ccc == 5)
    {
      scsi_read6_supported_ccc = false;
    }
    else if (sense_key_ccc != 0)
    {
      scsi_read6_supported_ccc = false;
      fprintf (stdout, "read6 sense = %02x\n", sense_key_ccc);
    }
    else if (io_host_status_ccc)
    {
      scsi_read6_supported_ccc = false;
      fprintf (stdout, "read6 host io error = %d\n", io_host_status_ccc);
    }
    else
    {
      scsi_read6_supported_ccc = true;
    }
  }
  else
  {
    scsi_read6_supported_ccc = false;
  }

  passthrough_ccc.scsi_cmd[0] = 0x28; // read 10 command
  passthrough_ccc.scsi_cmd[1] = 0;
  passthrough_ccc.scsi_cmd[2] = 0;
  passthrough_ccc.scsi_cmd[3] = 0;
  passthrough_ccc.scsi_cmd[4] = 0;
  passthrough_ccc.scsi_cmd[5] = 0;
  passthrough_ccc.scsi_cmd[6] = 0;
  passthrough_ccc.scsi_cmd[7] = 0;
  passthrough_ccc.scsi_cmd[8] = 1;
  passthrough_ccc.scsi_cmd[9] = 0;
  command_length_ccc = 10;
  soft_reset_time_bak = soft_reset_time_ccc;
  soft_reset_time_ccc = initial_busy_wait_time_ccc;
  return_value_ccc = do_scsi_cmd_ccc(current_disk_ccc);
  soft_reset_time_ccc = soft_reset_time_bak;
  if (return_value_ccc)
  {
    scsi_read10_supported_ccc = false;
    //return -1;
  }
  else if (sense_key_ccc == 5)
  {
    scsi_read10_supported_ccc = false;
  }
  else if (sense_key_ccc != 0)
  {
    scsi_read10_supported_ccc = false;
    fprintf (stdout, "read10 sense = %02x\n", sense_key_ccc);
  }
  else if (io_host_status_ccc)
  {
    scsi_read10_supported_ccc = false;
    fprintf (stdout, "read10 host io error = %d\n", io_host_status_ccc);
  }
  else
  {
    scsi_read10_supported_ccc = true;
  }

  if (check12)
  {
    passthrough_ccc.scsi_cmd[0] = 0xa8; // read 12 command
    passthrough_ccc.scsi_cmd[1] = 0;
    passthrough_ccc.scsi_cmd[2] = 0;
    passthrough_ccc.scsi_cmd[3] = 0;
    passthrough_ccc.scsi_cmd[4] = 0;
    passthrough_ccc.scsi_cmd[5] = 0;
    passthrough_ccc.scsi_cmd[6] = 0;
    passthrough_ccc.scsi_cmd[7] = 0;
    passthrough_ccc.scsi_cmd[8] = 0;
    passthrough_ccc.scsi_cmd[9] = 1;
    passthrough_ccc.scsi_cmd[10] = 0;
    passthrough_ccc.scsi_cmd[11] = 0;
    command_length_ccc = 12;
    soft_reset_time_bak = soft_reset_time_ccc;
    soft_reset_time_ccc = initial_busy_wait_time_ccc;
    return_value_ccc = do_scsi_cmd_ccc(current_disk_ccc);
    soft_reset_time_ccc = soft_reset_time_bak;
    if (return_value_ccc)
    {
      scsi_read12_supported_ccc = false;
      //return -1;
    }
    else if (sense_key_ccc == 5)
    {
      scsi_read12_supported_ccc = false;
    }
    else if (sense_key_ccc != 0)
    {
      scsi_read12_supported_ccc = false;
      fprintf (stdout, "read12 sense = %02x\n", sense_key_ccc);
    }
    else if (io_host_status_ccc)
    {
      scsi_read12_supported_ccc = false;
      fprintf (stdout, "read12 host io error = %d\n", io_host_status_ccc);
    }
    else
    {
      scsi_read12_supported_ccc = true;
    }
  }
  else
  {
    scsi_read12_supported_ccc = false;
  }

  if (check16)
  {
    passthrough_ccc.scsi_cmd[0] = 0x88; // read 16 command
    passthrough_ccc.scsi_cmd[1] = 0;
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
    passthrough_ccc.scsi_cmd[13] = 1;
    passthrough_ccc.scsi_cmd[14] = 0;
    passthrough_ccc.scsi_cmd[15] = 0;
    command_length_ccc = 16;
    soft_reset_time_bak = soft_reset_time_ccc;
    soft_reset_time_ccc = initial_busy_wait_time_ccc;
    return_value_ccc = do_scsi_cmd_ccc(current_disk_ccc);
    soft_reset_time_ccc = soft_reset_time_bak;
    if (return_value_ccc)
    {
      scsi_read16_supported_ccc = false;
      //return -1;
    }
    else if (sense_key_ccc == 5)
    {
      scsi_read16_supported_ccc = false;
    }
    else if (sense_key_ccc != 0)
    {
      scsi_read16_supported_ccc = false;
      fprintf (stdout, "read16 sense = %02x\n", sense_key_ccc);
    }
    else if (io_host_status_ccc)
    {
      scsi_read16_supported_ccc = false;
      fprintf (stdout, "read16 host io error = %d\n", io_host_status_ccc);
    }
    else
    {
      scsi_read16_supported_ccc = true;
    }
  }

  ccc_main_buffer_size_ccc = ccc_main_buffer_size_bak;
  set_main_buffer_ccc();
  memcpy (ccc_buffer_ccc, buffbak, ccc_main_buffer_size_ccc);
  if (check6)
  {
    fprintf (stdout, "scsi read6 support = %s\n", scsi_read6_supported_ccc ? "true" : "false");
  }
  fprintf (stdout, "scsi read10 support = %s\n", scsi_read10_supported_ccc ? "true" : "false");
  if (check12)
  {
    fprintf (stdout, "scsi read12 support = %s\n", scsi_read12_supported_ccc ? "true" : "false");
  }
  if (check16)
  {
    fprintf (stdout, "scsi read16 support = %s\n", scsi_read16_supported_ccc ? "true" : "false");
  }
  if (!scsi_read16_supported_ccc && !scsi_read12_supported_ccc && !scsi_read10_supported_ccc && !scsi_read6_supported_ccc)
  {
    fprintf (stdout, "Warning: No supported read modes found!\n");
    if (extended_support_ccc)
    {
      fprintf (stdout, "Assuming read16 support from readcapacity16 support\n");
      scsi_read16_supported_ccc = true;
    }
    else
    {
      fprintf (stdout, "Assuming read10 support from readcapacity10 support\n");
      scsi_read10_supported_ccc = true;
    }
  }
  return 0;
}





int scsi_write_ccc(long long position, int size)
{
  if (!scsi_write_checked_ccc)
  {
    int error = 0;
    fprintf (stdout, "checking scsi write command support by checking read support\n");
    unsigned char buffbak[ccc_main_buffer_size_ccc];
    memcpy (buffbak, ccc_buffer_ccc, ccc_main_buffer_size_ccc);
    unsigned long long ccc_main_buffer_size_bak = ccc_main_buffer_size_ccc;
    ccc_main_buffer_size_ccc = sector_size_ccc + output_sector_size_adjustment_ccc;
    set_main_buffer_ccc();
    passthrough_ccc.sg_xfer_direction = SG_DXFER_FROM_DEV;
    passthrough_ccc.scsi_cmd[0] = 0x08; // read 6 command
    passthrough_ccc.scsi_cmd[1] = 0;
    passthrough_ccc.scsi_cmd[2] = 0;
    passthrough_ccc.scsi_cmd[3] = 0;
    passthrough_ccc.scsi_cmd[4] = 1;
    passthrough_ccc.scsi_cmd[5] = 0;
    command_length_ccc = 6;
    return_value_ccc = do_scsi_cmd_ccc(disk2_fd_ccc);
    if (return_value_ccc)
    {
      error = 1;
    }
    else if (sense_key_ccc == 5)
    {
      scsi_write6_supported_ccc = false;
    }
    else
    {
      scsi_write6_supported_ccc = true;
    }

    passthrough_ccc.scsi_cmd[0] = 0x28; // read 10 command
    passthrough_ccc.scsi_cmd[1] = 0;
    passthrough_ccc.scsi_cmd[2] = 0;
    passthrough_ccc.scsi_cmd[3] = 0;
    passthrough_ccc.scsi_cmd[4] = 0;
    passthrough_ccc.scsi_cmd[5] = 0;
    passthrough_ccc.scsi_cmd[6] = 0;
    passthrough_ccc.scsi_cmd[7] = 0;
    passthrough_ccc.scsi_cmd[8] = 1;
    passthrough_ccc.scsi_cmd[9] = 0;
    command_length_ccc = 10;
    return_value_ccc = do_scsi_cmd_ccc(disk2_fd_ccc);
    if (return_value_ccc)
    {
      error = 1;
    }
    else if (sense_key_ccc == 5)
    {
      scsi_write10_supported_ccc = false;
    }
    else
    {
      scsi_write10_supported_ccc = true;
    }

    passthrough_ccc.scsi_cmd[0] = 0xa8; // read 12 command
    passthrough_ccc.scsi_cmd[1] = 0;
    passthrough_ccc.scsi_cmd[2] = 0;
    passthrough_ccc.scsi_cmd[3] = 0;
    passthrough_ccc.scsi_cmd[4] = 0;
    passthrough_ccc.scsi_cmd[5] = 0;
    passthrough_ccc.scsi_cmd[6] = 0;
    passthrough_ccc.scsi_cmd[7] = 0;
    passthrough_ccc.scsi_cmd[8] = 0;
    passthrough_ccc.scsi_cmd[9] = 1;
    passthrough_ccc.scsi_cmd[10] = 0;
    passthrough_ccc.scsi_cmd[11] = 0;
    command_length_ccc = 12;
    return_value_ccc = do_scsi_cmd_ccc(disk2_fd_ccc);
    if (return_value_ccc)
    {
      error = 1;
    }
    else if (sense_key_ccc == 5)
    {
      scsi_write12_supported_ccc = false;
    }
    else
    {
      scsi_write12_supported_ccc = true;
    }

    passthrough_ccc.scsi_cmd[0] = 0x88; // read 16 command
    passthrough_ccc.scsi_cmd[1] = 0;
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
    passthrough_ccc.scsi_cmd[13] = 1;
    passthrough_ccc.scsi_cmd[14] = 0;
    passthrough_ccc.scsi_cmd[15] = 0;
    command_length_ccc = 16;
    return_value_ccc = do_scsi_cmd_ccc(disk2_fd_ccc);
    if (return_value_ccc)
    {
      error = 1;
    }
    else if (sense_key_ccc == 5)
    {
      scsi_write16_supported_ccc = false;
    }
    else
    {
      scsi_write16_supported_ccc = true;
    }

    ccc_main_buffer_size_ccc = ccc_main_buffer_size_bak;
    set_main_buffer_ccc();
    memcpy (ccc_buffer_ccc, buffbak, ccc_main_buffer_size_ccc);
    fprintf (stdout, "scsi write6 support = %s\n", scsi_write6_supported_ccc ? "true" : "false");
    fprintf (stdout, "scsi write10 support = %s\n", scsi_write10_supported_ccc ? "true" : "false");
    fprintf (stdout, "scsi write12 support = %s\n", scsi_write12_supported_ccc ? "true" : "false");
    fprintf (stdout, "scsi write16 support = %s\n", scsi_write16_supported_ccc ? "true" : "false");
    if (!scsi_write16_supported_ccc && !scsi_write12_supported_ccc && !scsi_write10_supported_ccc && !scsi_write6_supported_ccc)
    {
      fprintf (stdout, "ERROR: No supported write modes found!\n");
      strcpy (tempmessage_ccc, curlang_ccc[LANGCANTWRITEDESTINATION]);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return -1;
    }

    if (!error)
    {
      scsi_write_checked_ccc = true;
    }
  }

  if (scsi_write16_supported_ccc)
  {
    command_length_ccc = 16;
  }
  else if (scsi_write12_supported_ccc)
  {
    command_length_ccc = 12;
  }
  else if (scsi_write10_supported_ccc)
  {
    command_length_ccc = 10;
  }
  else
  {
    command_length_ccc = 6;
  }

  switch (command_length_ccc)
  {
    case 6:
      passthrough_ccc.scsi_cmd[0] = 0x0a; // write 6 command
      passthrough_ccc.scsi_cmd[1] = (unsigned char)((position >> 16) & 0x1f);
      passthrough_ccc.scsi_cmd[2] = (unsigned char)((position>> 8) & 0xff);
      passthrough_ccc.scsi_cmd[3] = (unsigned char)(position & 0xff);
      passthrough_ccc.scsi_cmd[4] = (unsigned char)(size & 0xff);
      passthrough_ccc.scsi_cmd[5] = 0;
      break;

    case 10:
      passthrough_ccc.scsi_cmd[0] = 0x2a; // write 10 command
      passthrough_ccc.scsi_cmd[1] = 0;
      passthrough_ccc.scsi_cmd[2] = (unsigned char)((position >> 24) & 0xff);
      passthrough_ccc.scsi_cmd[3] = (unsigned char)((position >> 16) & 0xff);
      passthrough_ccc.scsi_cmd[4] = (unsigned char)((position>> 8) & 0xff);
      passthrough_ccc.scsi_cmd[5] = (unsigned char)(position & 0xff);
      passthrough_ccc.scsi_cmd[6] = 0;
      passthrough_ccc.scsi_cmd[7] = (unsigned char)((size >> 8) & 0xff);
      passthrough_ccc.scsi_cmd[8] = (unsigned char)(size & 0xff);
      passthrough_ccc.scsi_cmd[9] = 0;
      break;

    case 12:
      passthrough_ccc.scsi_cmd[0] = 0xaa; // write 12 command
      passthrough_ccc.scsi_cmd[1] = 0;
      passthrough_ccc.scsi_cmd[2] = (unsigned char)((position >> 24) & 0xff);
      passthrough_ccc.scsi_cmd[3] = (unsigned char)((position >> 16) & 0xff);
      passthrough_ccc.scsi_cmd[4] = (unsigned char)((position>> 8) & 0xff);
      passthrough_ccc.scsi_cmd[5] = (unsigned char)(position & 0xff);
      passthrough_ccc.scsi_cmd[6] = (unsigned char)((size >> 24) & 0xff);
      passthrough_ccc.scsi_cmd[7] = (unsigned char)((size >> 16) & 0xff);
      passthrough_ccc.scsi_cmd[8] = (unsigned char)((size >> 8) & 0xff);
      passthrough_ccc.scsi_cmd[9] = (unsigned char)(size & 0xff);
      passthrough_ccc.scsi_cmd[10] = 0;
      passthrough_ccc.scsi_cmd[11] = 0;
      break;

    case 16:
      passthrough_ccc.scsi_cmd[0] = 0x8a; // write 16 command
      passthrough_ccc.scsi_cmd[1] = 0;
      passthrough_ccc.scsi_cmd[2] = (unsigned char)((position >> 56) & 0xff);
      passthrough_ccc.scsi_cmd[3] = (unsigned char)((position >> 48) & 0xff);
      passthrough_ccc.scsi_cmd[4] = (unsigned char)((position >> 40) & 0xff);
      passthrough_ccc.scsi_cmd[5] = (unsigned char)((position >> 32) & 0xff);
      passthrough_ccc.scsi_cmd[6] = (unsigned char)((position >> 24) & 0xff);
      passthrough_ccc.scsi_cmd[7] = (unsigned char)((position >> 16) & 0xff);
      passthrough_ccc.scsi_cmd[8] = (unsigned char)((position >> 8) & 0xff);
      passthrough_ccc.scsi_cmd[9] = (unsigned char)(position & 0xff);
      passthrough_ccc.scsi_cmd[10] = (unsigned char)((size >> 24) & 0xff);
      passthrough_ccc.scsi_cmd[11] = (unsigned char)((size >> 16) & 0xff);
      passthrough_ccc.scsi_cmd[12] = (unsigned char)((size >> 8) & 0xff);
      passthrough_ccc.scsi_cmd[13] = (unsigned char)(size & 0xff);
      passthrough_ccc.scsi_cmd[14] = 0;
      passthrough_ccc.scsi_cmd[15] = 0;
      break;
  }
  passthrough_ccc.sg_xfer_direction = SG_DXFER_TO_DEV;
  return_value_ccc = do_scsi_cmd_ccc(disk2_fd_ccc);
  return return_value_ccc;
}






int ata_28_pio_read_ccc(unsigned char *reg)
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

  int cmd_length = 7;
  int i;
  for (i = 0; i < cmd_length; i++)
  {
    passthrough_ccc.ata_cmd[i] = reg[i];
  }

  command_length_ccc = 12;
  return_value_ccc = do_ata28_cmd_ccc(current_disk_ccc);
  return (return_value_ccc);
}





int read_ata_dma_ccc(long long position, int size, int rarc_bit)
{
  int ret;
  passthrough_ccc.transfer_length = 2;    // sector field
  passthrough_ccc.byte_block = 1;    // block
  passthrough_ccc.protocol = 6;    // dma
  passthrough_ccc.check_condition = 1;    // check condition true
  passthrough_ccc.direction = 2;    // from
  passthrough_ccc.transfer_direction = 1;    // from
  passthrough_ccc.sg_xfer_direction = SG_DXFER_FROM_DEV;

  if (use_fpdma_ccc)
  {
    passthrough_ccc.protocol = 12;    // fpdma/ncq

    passthrough_ccc.ata_cmd[0] = size >> 8;        // feature high used for sector count
    passthrough_ccc.ata_cmd[1] = size & 0xff;      // feature low used for sector count
    passthrough_ccc.ata_cmd[2] = 0 >> 8;        // sector count high used for flags
    passthrough_ccc.ata_cmd[3] = rarc_bit & 0xff;      // sector count low used for flags
    passthrough_ccc.ata_cmd[4] = position >> 24;      // LBA4 31:24
    passthrough_ccc.ata_cmd[5] = position & 0xff;     // LBA1 7:0
    passthrough_ccc.ata_cmd[6] = position >> 32;      // LBA5 39:32
    passthrough_ccc.ata_cmd[7] = position >> 8;       // LBA2 15:8
    passthrough_ccc.ata_cmd[8] = position >> 40;      // LBA6 47:40
    passthrough_ccc.ata_cmd[9] = position >> 16;      // LBA3 23:16
    passthrough_ccc.ata_cmd[10] = 0x40;    // Device bit 6 set
    passthrough_ccc.ata_cmd[11] = 0x60;    // Command read fpdma queued
    command_length_ccc = 16;
    ret = do_ata48_cmd_ccc(current_disk_ccc);
  }
  else if (extended_support_ccc)
  {
    passthrough_ccc.ata_cmd[0] = 0 >> 8;        // feature high
    passthrough_ccc.ata_cmd[1] = 0 & 0xff;      // feature low
    passthrough_ccc.ata_cmd[2] = size >> 8;        // sector count high
    passthrough_ccc.ata_cmd[3] = size & 0xff;      // sector count low
    passthrough_ccc.ata_cmd[4] = position >> 24;      // LBA4 31:24
    passthrough_ccc.ata_cmd[5] = position & 0xff;     // LBA1 7:0
    passthrough_ccc.ata_cmd[6] = position >> 32;      // LBA5 39:32
    passthrough_ccc.ata_cmd[7] = position >> 8;       // LBA2 15:8
    passthrough_ccc.ata_cmd[8] = position >> 40;      // LBA6 47:40
    passthrough_ccc.ata_cmd[9] = position >> 16;      // LBA3 23:16
    passthrough_ccc.ata_cmd[10] = 0xe0;    // Device bits 5 6 7 all set for compatibility
    passthrough_ccc.ata_cmd[11] = 0x25;    // Command read dma ext
    command_length_ccc = 16;
    ret = do_ata48_cmd_ccc(current_disk_ccc);
  }
  else if (lba_supported_ccc)
  {
    passthrough_ccc.ata_cmd[0] = 0;        // feature
    passthrough_ccc.ata_cmd[1] = size;        // sector count
    passthrough_ccc.ata_cmd[2] = position & 0xff;     // LBA1 7:0
    passthrough_ccc.ata_cmd[3] = position >> 8;       // LBA2 15:8
    passthrough_ccc.ata_cmd[4] = position >> 16;      // LBA3 23:16
    passthrough_ccc.ata_cmd[5] = 0xe0 | ( (position >> 24) & 0xf );    // Device
    passthrough_ccc.ata_cmd[6] = 0xc8;    // Command read dma
    command_length_ccc = 12;
    ret = do_ata28_cmd_ccc(current_disk_ccc);
  }
  else
  {
    ret = process_chs_ccc(position);
    if (ret < 0)
    {
      return ret;
    }
    passthrough_ccc.ata_cmd[0] = 0;        // feature
    passthrough_ccc.ata_cmd[1] = size;        // sector count
    passthrough_ccc.ata_cmd[2] = chs_ccc.sector;     // sector
    passthrough_ccc.ata_cmd[3] = chs_ccc.cylinder;       // cylinder low
    passthrough_ccc.ata_cmd[4] = chs_ccc.cylinder >> 8;      // cylinder high
    passthrough_ccc.ata_cmd[5] = 0xe0 | ( chs_ccc.head & 0xf );    // Device
    passthrough_ccc.ata_cmd[6] = 0xc8;    // Command read dma
    command_length_ccc = 12;
    ret = do_ata28_cmd_ccc(current_disk_ccc);
  }

  return ret;
}





int read_ata_pio_ccc(long long position, int size)
{
  int ret;
  passthrough_ccc.transfer_length = 2;    // sector field
  passthrough_ccc.byte_block = 1;    // block
  passthrough_ccc.protocol = 4;    // pio
  passthrough_ccc.check_condition = 1;    // check condition true
  passthrough_ccc.direction = 2;    // from
  passthrough_ccc.transfer_direction = 1;    // from
  passthrough_ccc.sg_xfer_direction = SG_DXFER_FROM_DEV;

  if (extended_support_ccc)
  {
    passthrough_ccc.ata_cmd[0] = 0 >> 8;        // feature high
    passthrough_ccc.ata_cmd[1] = 0 & 0xff;      // feature low
    passthrough_ccc.ata_cmd[2] = size >> 8;        // sector count high
    passthrough_ccc.ata_cmd[3] = size & 0xff;      // sector count low
    passthrough_ccc.ata_cmd[4] = position >> 24;      // LBA4 31:24
    passthrough_ccc.ata_cmd[5] = position & 0xff;     // LBA1 7:0
    passthrough_ccc.ata_cmd[6] = position >> 32;      // LBA5 39:32
    passthrough_ccc.ata_cmd[7] = position >> 8;       // LBA2 15:8
    passthrough_ccc.ata_cmd[8] = position >> 40;      // LBA6 47:40
    passthrough_ccc.ata_cmd[9] = position >> 16;      // LBA3 23:16
    passthrough_ccc.ata_cmd[10] = 0xe0;    // Device
    passthrough_ccc.ata_cmd[11] = 0x24;    // Command read pio ext
    command_length_ccc = 16;
    ret = do_ata48_cmd_ccc(current_disk_ccc);
  }
  else if (lba_supported_ccc)
  {
    passthrough_ccc.ata_cmd[0] = 0;        // feature
    passthrough_ccc.ata_cmd[1] = size;        // sector count
    passthrough_ccc.ata_cmd[2] = position & 0xff;     // LBA1 7:0
    passthrough_ccc.ata_cmd[3] = position >> 8;       // LBA2 15:8
    passthrough_ccc.ata_cmd[4] = position >> 16;      // LBA3 23:16
    passthrough_ccc.ata_cmd[5] = 0xe0 | ( (position >> 24) & 0xf );    // Device
    passthrough_ccc.ata_cmd[6] = 0x20;    // Command read pio
    command_length_ccc = 12;
    ret = do_ata28_cmd_ccc(current_disk_ccc);
  }
  else
  {
    ret = process_chs_ccc(position);
    if (ret < 0)
    {
      return ret;
    }
    passthrough_ccc.ata_cmd[0] = 0;        // feature
    passthrough_ccc.ata_cmd[1] = size;        // sector count
    passthrough_ccc.ata_cmd[2] = chs_ccc.sector;     // sector
    passthrough_ccc.ata_cmd[3] = chs_ccc.cylinder;       // cylinder low
    passthrough_ccc.ata_cmd[4] = chs_ccc.cylinder >> 8;      // cylinder high
    passthrough_ccc.ata_cmd[5] = 0xe0 | ( chs_ccc.head & 0xf );    // Device
    passthrough_ccc.ata_cmd[6] = 0x20;    // Command read pio
    command_length_ccc = 12;
    ret = do_ata28_cmd_ccc(current_disk_ccc);
  }

  return ret;
}





int process_chs_ccc(long long position)
{
  chs_ccc.cylinder = position / (chs_ccc.total_heads * chs_ccc.sectors_per_track);
  long long adjusted_position = position - (chs_ccc.cylinder * chs_ccc.total_heads * chs_ccc.sectors_per_track);
  chs_ccc.head = adjusted_position / chs_ccc.sectors_per_track;
  chs_ccc.sector = (adjusted_position - (chs_ccc.head * chs_ccc.sectors_per_track)) + 1;

  if (chs_ccc.cylinder >= chs_ccc.total_cylinders || chs_ccc.head >= chs_ccc.total_heads || chs_ccc.sector > chs_ccc.sectors_per_track || chs_ccc.cylinder > 65535 || chs_ccc.head > 15 || chs_ccc.sector > 256)
  {
    check_message_ccc = true;
    strcpy (tempmessage_ccc, curlang_ccc[LANGCHSERROR]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, " C=%lld H=%lld S=%lld\n", chs_ccc.cylinder, chs_ccc.head, chs_ccc.sector);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return INTERNAL_ERROR_RETURN_CODE;
  }

  int size = (chs_ccc.sectors_per_track - chs_ccc.sector) + 1;
  return size;
}






int check_buffer_limit_ccc(void)
{
  int multiplier = 1;
  if (driver_installed_ccc && driver_memory_mapped_ccc)
  {
    multiplier = 4;
  }
  //max_dma_size_ccc = (pagesize_ccc / 8) * pagesize_ccc;
  max_dma_size_ccc = ((pagesize_ccc * multiplier) / 16) * pagesize_ccc;    // limited to this to more match the ahci limit
  if (ahci_mode_ccc)
  {
    max_dma_size_ccc = ( ((pagesize_ccc * multiplier) - 128) / 16 ) * pagesize_ccc;
  }
  int max_size = 0;
  if (direct_mode_ccc)
  {
    max_size = max_dma_size_ccc/sector_size_ccc;
    if (cluster_size_ccc * sector_size_ccc > (long long)max_dma_size_ccc)
    {
      sprintf (tempmessage_ccc, "Warning: The cluster limit is %lld and you chose %d.\n", max_dma_size_ccc/sector_size_ccc, cluster_size_ccc);
      message_now_ccc(tempmessage_ccc);
      cluster_size_ccc = max_dma_size_ccc/sector_size_ccc;
      sprintf (tempmessage_ccc, "Using a new cluster size of %d\n", cluster_size_ccc);
      message_now_ccc(tempmessage_ccc);
    }
  }

  else if (scsi_passthrough_ccc || ata_passthrough_ccc)
  {
    // if passthrough we need to get the buffer limit that the kernel uses
    // and we can also use this to find out if whole disk or partition
    char file_name[80] = "/sys/block/\0";
    strcat (file_name, disk_1_ccc+5);
    strcat (file_name, "/queue/max_sectors_kb");

    FILE *file_pointer;
    file_pointer = fopen(file_name, "r");
    if (file_pointer == NULL)
    {
      max_size = DEFAULT_CLUSTER_SIZE;
      sprintf (tempmessage_ccc, "Warning: Cannot open %s for reading (%s)\n", file_name, strerror(errno));
      message_now_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, "Source must be whole disk and not partition\n" );
      message_now_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, "Unable to determine maximum cluster size, using default of %d\n", max_size);
      message_now_ccc(tempmessage_ccc);
    }
    else
    {
      char line[20];
      while (fgets(line, sizeof line, file_pointer))
      {
        break;
      }
      fclose(file_pointer);
      long long max_sectors_kb = strtoull(line, NULL, 0);
      max_size = (max_sectors_kb*1024)/sector_size_ccc;
      if (cluster_size_ccc * sector_size_ccc > max_sectors_kb * 1024)
      {
        sprintf (tempmessage_ccc, "Warning: The cluster limit is %lld and you chose %d.\n", (max_sectors_kb*1024)/sector_size_ccc, cluster_size_ccc);
        message_now_ccc(tempmessage_ccc);
        cluster_size_ccc = (max_sectors_kb*1024)/sector_size_ccc;
        sprintf (tempmessage_ccc, "Using a new cluster size of %d\n", cluster_size_ccc);
        message_now_ccc(tempmessage_ccc);
      }
    }
  }
  else if (usb_mode_ccc && !driver_memory_mapped_ccc)
  {
    max_size = MAX_USB_BUFFER_SIZE / sector_size_ccc;
  }
  else
  {
    // generic
    max_size = MAX_BUFFER_SIZE / sector_size_ccc;
  }
  return max_size;
}






// function to close and reopen the disk
int disk_reopen_ccc(void)
{
  if (current_disk_ccc == disk1_fd_ccc && disk_1_ccc != NULL)
  {
    if (superclone_ccc)
    {
      sprintf (tempmessage_ccc, "disk reopen\n");
      message_console_log_ccc(tempmessage_ccc, 0);
    }
    close (disk1_fd_ccc);
    if (generic_mode_ccc && superclone_ccc)
    {
      disk1_fd_ccc = open (disk_1_ccc, O_RDONLY);
    }
    else if (generic_mode_ccc && !superclone_ccc)
    {
      disk1_fd_ccc = open (disk_1_ccc, O_RDWR);
    }
    else
    {
      disk1_fd_ccc = open (disk_1_ccc, O_RDWR | O_NONBLOCK | O_DIRECT);
      // if there is an error then try without direct
      if (disk1_fd_ccc == -1)
      {
        disk1_fd_ccc = open (disk_1_ccc, O_RDWR | O_NONBLOCK);
      }
      // if there is still an error try with read only
      if (disk1_fd_ccc == -1)
      {
        disk1_fd_ccc = open (disk_1_ccc, O_RDONLY | O_NONBLOCK);
      }
    }
    if (disk1_fd_ccc == -1)
    {
      check_message_ccc = true;
      strcpy (tempmessage_ccc, curlang_ccc[LANGUNABLETOREOPEN]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " %s (%s)\n", disk_1_ccc, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      return -1;
    }
    current_disk_ccc = disk1_fd_ccc;
  }
  return (0);
}





int call_command_on_error_ccc(void)
{
  if (do_call_command_ccc)
  {
    // disconnect devices before calling command
    release_devices_ccc();

    sprintf (tempmessage_ccc, "%s %s\n", curlang_ccc[LANGCALLCOMMAND], command_to_call_ccc);
    message_console_log_ccc(tempmessage_ccc, 0);
    int ret = system (command_to_call_ccc);
    if (ret)
    {
      int status = WEXITSTATUS(ret);
      int signal = WTERMSIG(ret);
      strcpy (tempmessage_ccc, curlang_ccc[LANGCOMMANDFAILED]);
      message_error_ccc(tempmessage_ccc);
      sprintf(tempmessage_ccc, " (%d / %d)", signal, status);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return -1;
    }
    fprintf (stdout, "%s\n", curlang_ccc[LANGCOMMANDCOMPLETED]);

    // reconnect devices after command completes
    if (connect_source_disk_ccc() || open_target_destination_ccc())
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGCONNECTIONERR]);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return -1;
    }

    // check device to see if it is good now, if not try reconnecting
    if (check_device_ccc())
    {
      fprintf (stdout, "disconnecting and reconnecting with wait of %lldms\n", reset_wait_time_ccc / 1000);
      release_devices_ccc();
      do_nanosleep_ccc((reset_wait_time_ccc * 1000));
      if (connect_source_disk_ccc() || open_target_destination_ccc())
      {
        strcpy (tempmessage_ccc, curlang_ccc[LANGCONNECTIONERR]);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return -1;
      }
    }
    // check device to see if it is good now
    if (check_device_ccc())
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGDRIVEUNRESPONSIVE]);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return -1;
    }
    process_dma_mode_ccc();
    return 0;
  }
  else if (activate_primary_relay_on_error_ccc)
  {
    // disconnect devices before calling command
    release_devices_ccc();

    sprintf (tempmessage_ccc, "cycle primary relay\n");
    message_console_log_ccc(tempmessage_ccc, 0);
    if (cycle_primary_relay_ccc())
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGUSBRELAYERROR]);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return -1;
    }

    // reconnect devices after command completes
    if (connect_source_disk_ccc() || open_target_destination_ccc())
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGCONNECTIONERR]);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return -1;
    }

    // check device to see if it is good now, if not try reconnecting
    if (check_device_ccc() && enable_retry_connecting_ccc)
    {
      fprintf (stdout, "disconnecting and reconnecting with wait of %lldms\n", reset_wait_time_ccc / 1000);
      release_devices_ccc();
      do_nanosleep_ccc((reset_wait_time_ccc * 1000));
      if (connect_source_disk_ccc() || open_target_destination_ccc())
      {
        strcpy (tempmessage_ccc, curlang_ccc[LANGCONNECTIONERR]);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return -1;
      }
    }
    // check device to see if it is good now
    if (check_device_ccc())
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGDRIVEUNRESPONSIVE]);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return -1;
    }
    process_dma_mode_ccc();
    return 0;
  }
  return -1;
}





int call_command_on_power_cycle_ccc(void)
{
  if (call_power_command_ccc)
  {
    // disconnect devices before calling command
    release_devices_ccc();

    sprintf (tempmessage_ccc, "%s %s\n", curlang_ccc[LANGCALLCOMMAND], power_command_to_call_ccc);
    message_console_log_ccc(tempmessage_ccc, 0);
    int ret = system (power_command_to_call_ccc);
    if (ret)
    {
      int status = WEXITSTATUS(ret);
      int signal = WTERMSIG(ret);
      strcpy (tempmessage_ccc, curlang_ccc[LANGCOMMANDFAILED]);
      message_error_ccc(tempmessage_ccc);
      sprintf(tempmessage_ccc, " (%d / %d)", signal, status);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return 2;
    }
    sprintf (tempmessage_ccc, "%s\n", curlang_ccc[LANGCOMMANDCOMPLETED]);
    message_console_log_ccc(tempmessage_ccc, 0);

    // reconnect devices after command completes
    if (connect_source_disk_ccc() || open_target_destination_ccc())
    {
      return 0x8000;
    }

    // check device to see if it is good now
    //if (check_device_ccc())
    //{
      //strcpy (tempmessage_ccc, curlang_ccc[LANGDRIVEUNRESPONSIVE]);
      //message_error_ccc(tempmessage_ccc);
      //print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      //clear_error_message_ccc();
    //  return 1;
    //}
    process_dma_mode_ccc();
    return 0;
  }
  else if (power_cycle_primary_relay_ccc)
  {
    // disconnect devices before calling command
    release_devices_ccc();

    sprintf (tempmessage_ccc, "cycle primary relay\n");
    message_console_log_ccc(tempmessage_ccc, 0);
    if (cycle_primary_relay_ccc())
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGUSBRELAYERROR]);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return 2;
    }

    // reconnect devices after command completes
    if (connect_source_disk_ccc() || open_target_destination_ccc())
    {
      return 0x8000;
    }

    // check device to see if it is good now
    //if (check_device_ccc())
    //{
      //strcpy (tempmessage_ccc, curlang_ccc[LANGDRIVEUNRESPONSIVE]);
      //message_error_ccc(tempmessage_ccc);
      //print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      //clear_error_message_ccc();
    //  return 1;
    //}
    process_dma_mode_ccc();
    return 0;
  }
  return 1;
}






// time function
int get_the_time_ccc(void)
{
  int time_error = 0;
  if (superclone_ccc)
  {
    gettimeofday(&tvCurrent_ccc, NULL);
    char tempbuffer[30];
    time_t curtime;
    curtime = tvCurrent_ccc.tv_sec;
    strftime(tempbuffer, 30, "%Y-%m-%d_%H.%M.%S", localtime(&curtime));
    sprintf(current_date_ccc, "%s.%06ld", tempbuffer, tvCurrent_ccc.tv_usec);
    strftime(tempbuffer, 30, "%H.%M.%S", localtime(&curtime));
    sprintf(current_time_ccc, "%s.%06ld", tempbuffer, tvCurrent_ccc.tv_usec);
  }

  else
  {
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





// return future date
char *get_future_date_ccc(long long days)
{
    gettimeofday(&tvCurrent_ccc, NULL);
    char tempbuffer[30];
    time_t curtime;
    curtime = tvCurrent_ccc.tv_sec + (days * 86400);
    strftime(tempbuffer, 30, "%Y-%m-%d", localtime(&curtime));
    char *futuredate = malloc(sizeof(char) * 30);
    strcpy (futuredate, tempbuffer);
    return futuredate;
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






// check if size has changed
int check_device_ccc(void)
{
  unsigned char sense_key_bak = sense_key_ccc;
  unsigned char asc_bak = asc_ccc;
  unsigned char ascq_bak = ascq_ccc;
  unsigned char ata_error_bak = ata_error_ccc;
  unsigned char ata_status_bak = ata_status_ccc;
  unsigned char ata_device_bak = ata_device_ccc;
  unsigned char ata_count1_bak = ata_count1_ccc;
  unsigned char ata_count2_bak = ata_count2_ccc;
  unsigned char ata_lba1_bak = ata_lba1_ccc;
  unsigned char ata_lba2_bak = ata_lba2_ccc;
  unsigned char ata_lba3_bak = ata_lba3_ccc;
  unsigned char ata_lba4_bak = ata_lba4_ccc;
  unsigned char ata_lba5_bak = ata_lba5_ccc;
  unsigned char ata_lba6_bak = ata_lba6_ccc;
  long long ata_lba_bak = ata_lba_ccc;
  int ata_count_bak = ata_count_ccc;
  int io_status_bak = io_status_ccc;
  int io_masked_status_bak = io_masked_status_ccc;
  int msg_status_bak = msg_status_ccc;
  int io_sb_len_wr_bak = io_sb_len_wr_ccc;
  int io_host_status_bak = io_host_status_ccc;
  int io_driver_status_bak = io_driver_status_ccc;
  int io_resid_bak = io_resid_ccc;
  int io_duration_bak = io_duration_ccc;
  int io_info_bak = io_info_ccc;
  int ata_return_valid_bak = ata_return_valid_ccc;

  long long drive_size = 0;
  if (ata_passthrough_ccc || direct_mode_ccc)
  {
    if (enable_dump_identify_on_check_ccc)
    {
      char temp_string[256];
      sprintf (temp_string, "dump before check device sourcesize=%lld reportedsize=%lld\n", source_total_size_ccc, drive_size);
      dump_info_to_filename_ccc (data_dump_filename_ccc, temp_string);
      dump_data_to_filename_ccc(data_dump_filename_ccc, identify_buffer_ccc, IDENTIFY_BUFFER_SIZE, "identify device data");
    }
    did_reset_ccc = 0;
    int ret = identify_device_ccc();
    if ( (ret || (ata_status_ccc & 1)) && !did_reset_ccc )
    {
      ret = do_soft_reset_ccc(0);
    }
    if (ret)
    {
      update_display_status_buttons_ccc(0);
      check_message_ccc = true;
      strcpy (tempmessage_ccc, curlang_ccc[LANGSOURCEIMPROPERIDENTIFY]);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return DEVICE_ERROR_RETURN_CODE;
    }
    else if (ata_status_ccc & 1)
    {
      check_message_ccc = true;
      strcpy (tempmessage_ccc, curlang_ccc[LANGSOURCEERRORIDENTIFY]);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return DEVICE_ERROR_RETURN_CODE;
    }
    else if (ata_passthrough_ccc && io_host_status_ccc)
    {
      check_message_ccc = true;
      strcpy (tempmessage_ccc, curlang_ccc[LANGHOSTIOERRORIDENTIFY]);
      message_error_ccc(tempmessage_ccc);
      sprintf(tempmessage_ccc, " %d (%s)", io_host_status_ccc, host_error_code_ccc(io_host_status_ccc));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return DEVICE_ERROR_RETURN_CODE;
    }
    uint32_t double_word;
    uint64_t quad_word;
    memcpy(&double_word, identify_buffer_ccc+120, 4);
    memcpy(&quad_word, identify_buffer_ccc+200, 8);
    unsigned char c;
    memcpy(&c, identify_buffer_ccc+167, 1);
    if (c & 4)
    {
      drive_size = quad_word;
    }
    else
    {
      drive_size = double_word;
    }
    if (enable_dump_identify_on_check_ccc)
    {
      char temp_string[256];
      sprintf (temp_string, "dump after check device sourcesize=%lld reportedsize=%lld\n", source_total_size_ccc, drive_size);
      dump_info_to_filename_ccc (data_dump_filename_ccc, temp_string);
      dump_data_to_filename_ccc(data_dump_filename_ccc, identify_buffer_ccc, IDENTIFY_BUFFER_SIZE, "identify device data");
    }
  }
  else if (scsi_passthrough_ccc || usb_mode_ccc)
  {
    // if it can do ata passthrough then attempt an identify
    if (ata_identify_success_ccc)
    {
      if (usb_mode_ccc)
      {
        if (usb_identify_ccc(initial_busy_wait_time_ccc / 1000))
        {
          check_message_ccc = true;
          strcpy (tempmessage_ccc, curlang_ccc[LANGIDENTIFYFAILED]);
          message_error_ccc(tempmessage_ccc);
          sprintf(tempmessage_ccc, " %x %x %x.\n", sense_key_ccc, asc_ccc, ascq_ccc);
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
          clear_error_message_ccc();
          return INPUT_DEVICE_ERROR_RETURN_CODE;
        }
      }
      else
      {
        if (identify_device_ccc())
        {
          check_message_ccc = true;
          strcpy (tempmessage_ccc, curlang_ccc[LANGIDENTIFYFAILED]);
          message_error_ccc(tempmessage_ccc);
          sprintf(tempmessage_ccc, " %x %x %x.\n", sense_key_ccc, asc_ccc, ascq_ccc);
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
          clear_error_message_ccc();
          return INPUT_DEVICE_ERROR_RETURN_CODE;
        }
      }
    }
    // scsi
    // need to perform read capacity command to get size
    uint32_t double_word;
    uint64_t quad_word;
    unsigned long long capacity = 0;
    if (extended_support_ccc)
    {
      if (read_capacity_16_ccc() || sense_key_ccc > 1 || io_host_status_ccc)
      {
        // try reopening the device
        int ret = disk_reopen_ccc();
        if (ret)
        {
          check_message_ccc = true;
          strcpy (tempmessage_ccc, curlang_ccc[LANGFAILEDSIZECHECK]);
          message_error_ccc(tempmessage_ccc);
          sprintf(tempmessage_ccc, "\n");
          message_error_ccc(tempmessage_ccc);
          strcpy (tempmessage_ccc, curlang_ccc[LANGUNABLETOREOPEN]);
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
          clear_error_message_ccc();
          return DEVICE_ERROR_RETURN_CODE;
        }
      }
    }
    else
    {
      if (read_capacity_10_ccc() || sense_key_ccc > 1 || io_host_status_ccc)
      {
        // try reopening the device
        int ret = disk_reopen_ccc();
        if (ret)
        {
          check_message_ccc = true;
          strcpy (tempmessage_ccc, curlang_ccc[LANGFAILEDSIZECHECK]);
          message_error_ccc(tempmessage_ccc);
          sprintf(tempmessage_ccc, "\n");
          message_error_ccc(tempmessage_ccc);
          strcpy (tempmessage_ccc, curlang_ccc[LANGUNABLETOREOPEN]);
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
          clear_error_message_ccc();
          return DEVICE_ERROR_RETURN_CODE;
        }
      }
    }

    if (extended_support_ccc)
    {
      if (read_capacity_16_ccc() || sense_key_ccc > 1)
      {
        check_message_ccc = true;
        strcpy (tempmessage_ccc, curlang_ccc[LANGREADCAPFAILED]);
        message_error_ccc(tempmessage_ccc);
        sprintf(tempmessage_ccc, " %x %x %x.\n", sense_key_ccc, asc_ccc, ascq_ccc);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return INPUT_DEVICE_ERROR_RETURN_CODE;
      }
      else if (io_host_status_ccc)
      {
        check_message_ccc = true;
        strcpy (tempmessage_ccc, curlang_ccc[LANGHOSTIOERRCAPACITY]);
        message_error_ccc(tempmessage_ccc);
        sprintf(tempmessage_ccc, " %d (%s)", io_host_status_ccc, host_error_code_ccc(io_host_status_ccc));
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return INPUT_DEVICE_ERROR_RETURN_CODE;
      }
      else
      {
        memcpy(&quad_word, read_capacity_buffer_ccc + 0, 8);
        capacity = __bswap_64 (quad_word);
      }
    }
    else
    {
      if (read_capacity_10_ccc() || sense_key_ccc > 1)
      {
        check_message_ccc = true;
        strcpy (tempmessage_ccc, curlang_ccc[LANGREADCAPFAILED]);
        message_error_ccc(tempmessage_ccc);
        sprintf(tempmessage_ccc, " %x %x %x.\n", sense_key_ccc, asc_ccc, ascq_ccc);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return INPUT_DEVICE_ERROR_RETURN_CODE;
      }
      else if (io_host_status_ccc)
      {
        check_message_ccc = true;
        strcpy (tempmessage_ccc, curlang_ccc[LANGHOSTIOERRCAPACITY]);
        message_error_ccc(tempmessage_ccc);
        sprintf(tempmessage_ccc, " %d (%s)", io_host_status_ccc, host_error_code_ccc(io_host_status_ccc));
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return INPUT_DEVICE_ERROR_RETURN_CODE;
      }
      else
      {
        memcpy(&double_word, read_capacity_buffer_ccc + 0, 4);
        capacity = __bswap_32 (double_word);
      }
    }

    drive_size = capacity + 1;
    if (capacity == 0)
    {
      drive_size = capacity;
    }
  }
  else
  {
    // generic
    drive_size = (lseek( disk1_fd_ccc, 0, SEEK_END )) / sector_size_ccc;
  }

  if (drive_size != source_total_size_ccc)
  {
    fprintf (stdout, "original size = %lld reported size = %lld\n", source_total_size_ccc, drive_size);
    check_message_ccc = true;
    strcpy (tempmessage_ccc, curlang_ccc[LANGSOURCESIZECHANGED]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return DEVICE_ERROR_RETURN_CODE;
  }

  sense_key_ccc = sense_key_bak;
  asc_ccc = asc_bak;
  ascq_ccc = ascq_bak;
  ata_error_ccc = ata_error_bak;
  ata_status_ccc = ata_status_bak;
  ata_device_ccc = ata_device_bak;
  ata_count1_ccc = ata_count1_bak;
  ata_count2_ccc = ata_count2_bak;
  ata_lba1_ccc = ata_lba1_bak;
  ata_lba2_ccc = ata_lba2_bak;
  ata_lba3_ccc = ata_lba3_bak;
  ata_lba4_ccc = ata_lba4_bak;
  ata_lba5_ccc = ata_lba5_bak;
  ata_lba6_ccc = ata_lba6_bak;
  ata_lba_ccc = ata_lba_bak;
  ata_count_ccc = ata_count_bak;
  io_status_ccc = io_status_bak;
  io_masked_status_ccc = io_masked_status_bak;
  msg_status_ccc = msg_status_bak;
  io_sb_len_wr_ccc = io_sb_len_wr_bak;
  io_host_status_ccc = io_host_status_bak;
  io_driver_status_ccc = io_driver_status_bak;
  io_resid_ccc = io_resid_bak;
  io_duration_ccc = io_duration_bak;
  io_info_ccc = io_info_bak;
  ata_return_valid_ccc = ata_return_valid_bak;

  return 0;
}






char *host_error_code_ccc(int code)
{
  char *message;
  switch (code)
  {
    case 0:
      message = "NO error";
      break;

    case 1:
      message = "Couldn't connect before timeout period";
      break;

    case 2:
      message = "BUS stayed busy through time out period";
      break;

    case 3:
      message = "TIMED OUT for other reason";
      break;

    case 4:
      message = "BAD target, device not responding?";
      break;

    case 5:
      message = "Told to abort for some other reason.";
      break;

    case 6:
      message = "Parity error.";
      break;

    case 7:
      message = "Internal error detected in the host adapter.";
      break;

    case 8:
      message = "The SCSI bus (or this device) has been reset.";
      break;

    case 9:
      message = "Got an interrupt we weren't expecting";
      break;

    case 10:
      message = "Force command past mid-layer";
      break;

    case 11:
      message = "The low level driver wants a retry";
      break;

    default:
      message = "Unknown code                  ";
      sprintf (message, "Unknown code %d", code);
      break;
  }


  return message;
}





int open_target_destination_ccc(void)
{
  // open the target for IO
  if (disk_2_ccc != NULL)
  {
    struct stat statbuf;
    if ( stat(disk_2_ccc, &statbuf) == 0 && !S_ISREG(statbuf.st_mode) )
    {
      regular_file_ccc = false;
      // check to make sure that the user did not choose the OS drive as the target
      char command[255];
      strcpy (command, "mount | grep ' / '|cut -d' ' -f 1");
      FILE *fp = popen(command, "r");
      char line[255];
      while (fgets(line, sizeof line, fp))
      {
        break;
      }
      pclose(fp);
      if (strncmp(disk_2_ccc, line, 8) == 0)
      {
        strcpy (tempmessage_ccc, curlang_ccc[LANGTARGETISHOSTOS]);
        message_error_ccc(tempmessage_ccc);
        sprintf(tempmessage_ccc, " (%s)", disk_2_ccc);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return OUTPUT_DEVICE_ERROR_RETURN_CODE;
      }

      if (disk2_fd_ccc > 0)
      {
        close (disk2_fd_ccc);
        disk2_fd_ccc = -1;
      }
      if (write_buffer_disable_ccc)
      {
        disk2_fd_ccc = open (disk_2_ccc, O_DIRECT  | O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
      }
      else
      {
        disk2_fd_ccc = open (disk_2_ccc, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
      }
      // if there is an error then try without direct
      if (disk2_fd_ccc == -1)
      {
        disk2_fd_ccc = open (disk_2_ccc, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
      }
    }
    else
    {
      regular_file_ccc = true;
      if (disk2_fd_ccc > 0)
      {
        close (disk2_fd_ccc);
        disk2_fd_ccc = -1;
      }
      if (write_buffer_disable_ccc)
      {
        disk2_fd_ccc = open (disk_2_ccc, O_DIRECT  | O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
      }
      else
      {
        disk2_fd_ccc = open (disk_2_ccc, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
      }
      // if there is an error then try witout direct
      if (disk2_fd_ccc == -1)
      {
        disk2_fd_ccc = open (disk_2_ccc, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
      }
    }
    if (disk2_fd_ccc == -1)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGCANTOPENDESTINATION]);
      message_error_ccc(tempmessage_ccc);
      sprintf(tempmessage_ccc, " %s (%s)", disk_2_ccc, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return OUTPUT_DEVICE_ERROR_RETURN_CODE;
    }
    if (!enable_scsi_write_ccc)
    {
      if( lseek( disk2_fd_ccc, 0, SEEK_SET ) )
      {
        strcpy (tempmessage_ccc, curlang_ccc[LANGCANTSEEKDESTINATION]);
        message_error_ccc(tempmessage_ccc);
        sprintf(tempmessage_ccc, " %s (%s)", disk_2_ccc, strerror(errno));
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return OUTPUT_DEVICE_ERROR_RETURN_CODE;
      }
      // get total size of target
      target_total_size_ccc = lseek( disk2_fd_ccc, 0, SEEK_END );
      if (target_total_size_ccc == -1)
      {
        strcpy (tempmessage_ccc, curlang_ccc[LANGCANTGETSIZEDESTINATION]);
        message_error_ccc(tempmessage_ccc);
        sprintf(tempmessage_ccc, " %s (%s)", disk_2_ccc, strerror(errno));
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return OUTPUT_DEVICE_ERROR_RETURN_CODE;
      }
      sprintf (tempmessage_ccc, "Target size in bytes = %lld\n", target_total_size_ccc);    //debug
      message_debug_ccc(tempmessage_ccc, DEBUG1);
      target_total_size_ccc = target_total_size_ccc / sector_size_ccc;
    }
  }
  else
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGMISSINGDESTINATION]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return OUTPUT_DEVICE_ERROR_RETURN_CODE;
  }
  return 0;
}


int check_same_device_ccc(void)
{
// check to make sure source and target are not the same
  if (disk_1_ccc != NULL && disk_2_ccc != NULL)
  {
    if (strcmp (disk_1_ccc, disk_2_ccc) == 0)
   {
      return 1;
   }
  }
  return 0;
}




int open_source_disk_ccc(void)
{
  // open the source disk for IO
  if (disk_1_ccc != NULL && !direct_mode_ccc)
  {
    if (!force_mounted_ccc)
    {
      // check to see if the drive is mounted
      char command[255];
      sprintf(command, "mount | grep %s", disk_1_ccc);
      FILE *fp = popen(command, "r");
      char line[255];
      while (fgets(line, sizeof line, fp))
      {
        break;
      }
      pclose(fp);
      if (strncmp(disk_1_ccc, line, 8) == 0)
      {
        strcpy (tempmessage_ccc, curlang_ccc[LANGSOURCEMOUNTEDA]);
        message_error_ccc(tempmessage_ccc);
        sprintf(tempmessage_ccc, " (%s).\n", disk_1_ccc);
        message_error_ccc(tempmessage_ccc);
        strcpy (tempmessage_ccc, curlang_ccc[LANGSOURCEMOUNTEDB]);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return INPUT_DEVICE_ERROR_RETURN_CODE;
      }
    }

    if (disk1_fd_ccc > 0)
    {
      close (disk1_fd_ccc);
      disk1_fd_ccc = -1;
    }
    if (generic_mode_ccc && superclone_ccc)
    {
      disk1_fd_ccc = open (disk_1_ccc, O_RDONLY);
    }
    else if (generic_mode_ccc && !superclone_ccc)
    {
      disk1_fd_ccc = open (disk_1_ccc, O_RDWR);
    }
    else
    {
      disk1_fd_ccc = open (disk_1_ccc, O_RDWR | O_NONBLOCK | O_DIRECT);
      // if there is an error then try without direct
      if (disk1_fd_ccc == -1)
      {
        disk1_fd_ccc = open (disk_1_ccc, O_RDWR | O_NONBLOCK);
      }
      // if there is still an error try with read only
      if (disk1_fd_ccc == -1)
      {
        disk1_fd_ccc = open (disk_1_ccc, O_RDONLY | O_NONBLOCK);
      }
    }
    if (disk1_fd_ccc == -1)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGCANTOPENSOURCE]);
      message_error_ccc(tempmessage_ccc);
      sprintf(tempmessage_ccc, " %s (%s)", disk_1_ccc, strerror(errno));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return INPUT_DEVICE_ERROR_RETURN_CODE;
    }
  }
  current_disk_ccc = disk1_fd_ccc;
  return 0;
}





int close_source_disk_ccc(void)
{
  if (disk1_fd_ccc > 0)
  {
    close (disk1_fd_ccc);
    disk1_fd_ccc = -1;
  }
  return 0;
}





int process_source_ccc(void)
{
  //fprintf (stdout, "process_source_ccc\n");    //debug
  // first if auto passthrough see if an ata identify works and save the data if it does
  ata_identify_success_ccc = false;
  memset (ata_identify_buffer_ccc, 0, IDENTIFY_BUFFER_SIZE);
  if (auto_passthrough_ccc)
  {
    bool auto_passthrough_bak = auto_passthrough_ccc;
    bool ata_passthrough_bak = ata_passthrough_ccc;
    bool scsi_passthrough_bak = scsi_passthrough_ccc;
    auto_passthrough_ccc = false;
    ata_passthrough_ccc = true;
    scsi_passthrough_ccc = false;
    memset(&passthrough_ccc, 0, sizeof(passthrough_ccc));
    if ( !identify_device_ccc() && !(ata_status_ccc & 1) && sense_key_ccc < 2 )
    {
      ata_identify_success_ccc = true;
      memcpy (ata_identify_buffer_ccc, identify_buffer_ccc, IDENTIFY_BUFFER_SIZE);
    }
    auto_passthrough_ccc = auto_passthrough_bak;
    ata_passthrough_ccc = ata_passthrough_bak;
    scsi_passthrough_ccc = scsi_passthrough_bak;
  }

  if (auto_passthrough_ccc)
  {
    set_mode_scsi_passthrough_ccc();
    auto_passthrough_ccc = true;
  }
  // clear/initialize the passthrough structure
  memset(&passthrough_ccc, 0, sizeof(passthrough_ccc));
  strcpy (identify_device_raw_text_ccc, "");

  drive_locked_ccc = false;
  smart_supported_ccc = false;
  smart_enabled_ccc = false;
  ncq_supported_ccc = false;    // word 76 bit 8
  rebuild_assist_supported_ccc = false;    // word 78 bit 11
  rebuild_assist_data_valid_ccc = false;
  hpa_lba_ccc = 0;
  dco_lba_ccc = 0;
  // clear the identify flags data
  memset (&identify_flags_ccc, 0, sizeof(identify_flags_ccc));

  // if direct mode then perform a reset first in case it is locked up
  if (direct_mode_ccc)
  {
    if (ahci_mode_ccc)
    {
      do_hard_reset_ccc(0);
    }
    else
    {
      do_soft_reset_ccc(0);
    }
  }
  if (usb_mode_ccc)
  {
    if (usb_bulk_reset_enabled_ccc)
    {
      usb_reset_recovery_ccc(reset_wait_time_ccc / 1000);
    }
    if (usb_hard_reset_enabled_ccc && usb_allow_ata_ccc)
    {
      usb_soft_hard_reset_ccc(USB_RESET_TYPE_HARD, reset_wait_time_ccc / 1000);
    }
    else if (usb_soft_reset_enabled_ccc && usb_allow_ata_ccc)
    {
      usb_soft_hard_reset_ccc(USB_RESET_TYPE_SOFT, reset_wait_time_ccc / 1000);
    }
    if (usb_port_reset_enabled_ccc)
    {
      do_usb_reset_ccc();
    }
    usb_reset_recovery_ccc(reset_wait_time_ccc / 1000);
  }

  // perform initial identify device command to get some info
  if ( identify_device_ccc() || (ata_status_ccc & 1) )
  {
    do_soft_reset_ccc(current_disk_ccc);
    if ( identify_device_ccc() )
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGIDENTIFYFAILED]);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return INPUT_DEVICE_ERROR_RETURN_CODE;
    }
    else if (ata_status_ccc & 1)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGIDENTIFYFAILED]);
      message_error_ccc(tempmessage_ccc);
      strcpy (tempmessage_ccc, curlang_ccc[LANGSTATUSSLASHERROR]);
      message_error_ccc(tempmessage_ccc);
      sprintf (tempmessage_ccc, " %02x / %02x", ata_status_ccc, ata_error_ccc);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return INPUT_DEVICE_ERROR_RETURN_CODE;
    }
    else if ((scsi_passthrough_ccc || ata_passthrough_ccc) && io_host_status_ccc)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGHOSTIOERRIDENTIFY]);
      message_error_ccc(tempmessage_ccc);
      sprintf(tempmessage_ccc, " %d (%s)", io_host_status_ccc, host_error_code_ccc(io_host_status_ccc));
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return INPUT_DEVICE_ERROR_RETURN_CODE;
    }
  }
  if (0)    //debug
  {
    int i;
    for (i = 0; i < IDENTIFY_BUFFER_SIZE; i+=16)
    {
      fprintf (stdout, "%x: ", i);
      unsigned char *c;
      int n;
      for (n=0; n < 16 && i+n < IDENTIFY_BUFFER_SIZE; n++)
      {
        c = (unsigned char *)identify_buffer_ccc+i+n;
        fprintf (stdout, "%02x ", *c);
      }
      fprintf (stdout, "   ");
      for (n=0; n < 16 && i+n < IDENTIFY_BUFFER_SIZE; n++)
      {
        c = (unsigned char *)identify_buffer_ccc+i+n;
        fprintf (stdout, "%c", isprint(*c) ? *c : '.');
      }
      fprintf (stdout, "\n");
    }
  }

  // extract some information from the identify device
  // get the model and serial
  if (ata_check_ccc && auto_passthrough_ccc)
  {
    set_mode_ata_passthrough_ccc();
    auto_passthrough_ccc = true;
  }
  if (ata_passthrough_ccc || direct_mode_ccc || ata_check_ccc)
  {
   // if (!ata_passthrough_ccc && !direct_mode_ccc && !ata_check_ccc && ata_identify_success_ccc)
   // {
   //   memcpy (identify_buffer_ccc, ata_identify_buffer_ccc, IDENTIFY_BUFFER_SIZE);
   // }
    memcpy (ata_identify_buffer_ccc, identify_buffer_ccc, IDENTIFY_BUFFER_SIZE);
    int i;
    char temp_model[41] = "";
    char temp_serial[21] = "";
    for (i = 0; i < 40; i += 2)
    {
      unsigned char *c = (unsigned char *)identify_buffer_ccc+54+i+1;
      unsigned char *d = (unsigned char *)identify_buffer_ccc+54+i;
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
      unsigned char *c = (unsigned char *)identify_buffer_ccc+20+i+1;
      unsigned char *d = (unsigned char *)identify_buffer_ccc+20+i;
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

    strcpy (current_model_ccc, found_model);
    strcpy (current_serial_ccc, found_serial);
    strcpy (current_source_model_ccc, found_model);
    strcpy (current_source_serial_ccc, found_serial);
    //fprintf(stdout, "Model= %s\n", current_model_ccc);    //debug
    //fprintf(stdout, "Serial= %s\n", current_serial_ccc);    //debug

    uint16_t word;
    uint32_t double_word;
    uint64_t quad_word;
    int logical_sectors_per_physical = 1;
    int sector_offset = 0;
    bytes_per_sector_ccc = 512;
    memcpy(&word, identify_buffer_ccc+212, 2);  // word 106
    //check if word data is valid
    if ((word & 0xc000) == 0x4000)
    {
      // check if sector size greater than 256 words
      if (word & 0x1000)
      {
        memcpy(&double_word, identify_buffer_ccc+234, 4);  // word 117
        bytes_per_sector_ccc = double_word * 2;
      }
      // check if multiple logical sectors per physical
      if (word & 0x2000)
      {
        int x = word & 0xf;
        int n;
        for (n = 0; n < x; n++)
        {
          logical_sectors_per_physical = logical_sectors_per_physical * 2;
        }
        // check for offset
        memcpy(&word, identify_buffer_ccc+418, 2);  // word 209
        //check if word data is valid
        if ((word & 0xc000) == 0x4000)
        {
          sector_offset = word & 0x3fff;
        }
      }
    }
    memcpy(&double_word, identify_buffer_ccc+120, 4);  // word 60
    memcpy(&quad_word, identify_buffer_ccc+200, 8);  // word 100
    unsigned char c;
    memcpy(&c, identify_buffer_ccc+167, 1);  // word 83
    if (c & 4)
    {
      extended_support_ccc = true;
      source_total_size_ccc = quad_word;
    }
    else
    {
      extended_support_ccc = false;
      source_total_size_ccc = double_word;
    }
    //fprintf(stdout, "Extended= %d\n", extended_support_ccc);    //debug
    //fprintf (stdout, "Source size= %lld\n", source_total_size_ccc);    //debug
    chs_ccc.total_cylinders = 0;
    chs_ccc.total_heads = 0;
    chs_ccc.sectors_per_track = 0;
    memcpy(&c, identify_buffer_ccc+99, 1);  // word 49
    if (c & 2)
    {
      lba_supported_ccc = true;
    }
    else
    {
      lba_supported_ccc = false;
      bytes_per_sector_ccc = 512;
      logical_sectors_per_physical = 1;
      sector_offset = 0;
      uint16_t word;
      memcpy(&word, identify_buffer_ccc+106, 2);  // word 53
      if (word & 1)
      {
        memcpy(&word, identify_buffer_ccc+108, 2);  // word 54
        chs_ccc.total_cylinders = word;
        memcpy(&word, identify_buffer_ccc+110, 2);  // word 55
        chs_ccc.total_heads = word;
        memcpy(&word, identify_buffer_ccc+112, 2);  // word 56
        chs_ccc.sectors_per_track = word;
        memcpy(&word, identify_buffer_ccc+114, 2);  // word 57
        source_total_size_ccc = word << 16;
        memcpy(&word, identify_buffer_ccc+116, 2);  // word 58
        source_total_size_ccc = source_total_size_ccc + word;
        //fprintf(stdout, "initialized sectors= %lld c=%lld h=%lld s=%lld\n", source_total_size_ccc, chs_ccc.total_cylinders, chs_ccc.total_heads, chs_ccc.sectors_per_track);    //debug
      }
      else
      {
        memcpy(&word, identify_buffer_ccc+2, 2);  // word 1
        chs_ccc.total_cylinders = word;
        memcpy(&word, identify_buffer_ccc+6, 2);  // word 3
        chs_ccc.total_heads = word;
        memcpy(&word, identify_buffer_ccc+12, 2);  // word 6
        chs_ccc.sectors_per_track = word;
        source_total_size_ccc = chs_ccc.total_cylinders * chs_ccc.total_heads * chs_ccc.sectors_per_track;
        //fprintf(stdout, "default sectors= %lld c=%lld h=%lld s=%lld\n", source_total_size_ccc, chs_ccc.total_cylinders, chs_ccc.total_heads, chs_ccc.sectors_per_track);    //debug
      }
    }
    if (!data_read_from_log_ccc)
    {
      sector_size_ccc = bytes_per_sector_ccc;
      block_size_ccc = logical_sectors_per_physical;
      block_offset_ccc = sector_offset;
    }
    //fprintf(stdout, "LBA supported= %d\n", lba_supported_ccc);    //debug
    memcpy(&c, identify_buffer_ccc+256, 1);  // word 128
    if ((c & 7) == 7)
    {
      drive_locked_ccc = true;
    }
    memcpy(&word, identify_buffer_ccc+164, 2);  // word 82
    if ((word & 1) == 1)
    {
      smart_supported_ccc = true;
    }
    memcpy(&word, identify_buffer_ccc+170, 2);  // word 85
    if ((word & 1)  == 1)
    {
      smart_enabled_ccc = true;
    }
    memcpy(&word, identify_buffer_ccc+156, 2);  // word 78
    if ((word & 0x800) == 0x800)
    {
      rebuild_assist_supported_ccc = true;
    }
    memcpy(&word, identify_buffer_ccc+152, 2);  // word 76
    if ((word & 0x100) == 0x100)
    {
      ncq_supported_ccc = true;
    }

    if (ncq_supported_ccc && rebuild_assist_supported_ccc)
    {
      if (!read_rebuild_assist_log_ccc())
      {
        rebuild_assist_element_length_ccc = rebuild_assist_log_data_ccc.length;
        memcpy (rebuild_assist_element_mask_ccc, rebuild_assist_log_data_ccc.mask, sizeof(rebuild_assist_log_data_ccc.mask));
        rebuild_assist_data_valid_ccc = true;
      }
    }


  }
  else if (scsi_passthrough_ccc || usb_mode_ccc)
  {
    if (usb_mode_ccc && usb_allow_ata_ccc)
    {
      if (!usb_identify_ccc(initial_busy_wait_time_ccc / 1000))
      {
        memcpy(ata_identify_buffer_ccc, ccc_usbbuffer_ccc, IDENTIFY_BUFFER_SIZE);
        ata_identify_success_ccc = true;
      }
      else
      {
        usb_reset_recovery_ccc(usb_timeout_ccc);
      }
    }

    // if there was good ata idendify data then get model and serial from that
    // also if locked and if supports smart
    if (ata_identify_success_ccc)
    {
      int i;
      char temp_model[41] = "";
      char temp_serial[21] = "";
      for (i = 0; i < 40; i += 2)
      {
        unsigned char *c = (unsigned char *)ata_identify_buffer_ccc+54+i+1;  // word
        unsigned char *d = (unsigned char *)ata_identify_buffer_ccc+54+i;  // word
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
        unsigned char *c = (unsigned char *)ata_identify_buffer_ccc+20+i+1;  // word
        unsigned char *d = (unsigned char *)ata_identify_buffer_ccc+20+i;  // word
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

      strcpy (current_model_ccc, found_model);
      strcpy (current_serial_ccc, found_serial);
      strcpy (current_source_model_ccc, found_model);
      strcpy (current_source_serial_ccc, found_serial);
      //fprintf(stdout, "Model= %s\n", current_model_ccc);    //debug
      //fprintf(stdout, "Serial= %s\n", current_serial_ccc);    //debug

      uint16_t word;
      unsigned char c;
      memcpy(&c, ata_identify_buffer_ccc+256, 1);  // word
      if ((c & 7) == 7)
      {
        drive_locked_ccc = true;
      }
      memcpy(&word, ata_identify_buffer_ccc+164, 2);  // word
      if ((word & 1) == 1)
      {
        smart_supported_ccc = true;
      }
      memcpy(&word, ata_identify_buffer_ccc+170, 2);  // word
      if ((word & 1)  == 1)
      {
        smart_enabled_ccc = true;
      }
    }

    else
    {
      // get the model and serial from scsi
      int i;
      char temp_model[41] = "";
      char temp_serial[21] = "";
      for (i = 0; i < 8; i ++)
      {
        unsigned char *c = (unsigned char *)identify_buffer_ccc+8+i;
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
        unsigned char *d = (unsigned char *)identify_buffer_ccc+16+n;
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
        unsigned char *c = (unsigned char *)identify_buffer_ccc+36+i;
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

      strcpy (current_model_ccc, found_model);
      strcpy (current_serial_ccc, found_serial);
      strcpy (current_source_model_ccc, found_model);
      strcpy (current_source_serial_ccc, found_serial);
      //fprintf(stdout, "Model= %s\n", current_model_ccc);    //debug
      //fprintf(stdout, "Serial= %s\n", current_serial_ccc);    //debug
    }

    // need to perform read capacity command to get size
    uint16_t word;
    uint32_t double_word;
    uint64_t quad_word;
    unsigned long long capacity;
    unsigned int blocksize;
    unsigned int logical_sectors_per_physical = 1;
    int sector_offset = 0;
    int allow16 = 1;
    if (usb_mode_ccc)
    {
      read_capacity_10_ccc();    // do initial read capacity commands to clear possible unit attention
      read_capacity_10_ccc();
    }
    if (usb_mode_ccc && !usb_allow_ata_ccc)
    {
      allow16 = 0;
      if (read_capacity_10_ccc() || sense_key_ccc > 1)
      {
        strcpy (tempmessage_ccc, curlang_ccc[LANGREADCAPFAILED]);
        message_error_ccc(tempmessage_ccc);
        sprintf(tempmessage_ccc, " %x %x %x.\n", sense_key_ccc, asc_ccc, ascq_ccc);
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return INPUT_DEVICE_ERROR_RETURN_CODE;
      }
      else if (io_host_status_ccc)
      {
        strcpy (tempmessage_ccc, curlang_ccc[LANGHOSTIOERRCAPACITY]);
        message_error_ccc(tempmessage_ccc);
        sprintf(tempmessage_ccc, " %d (%s)", io_host_status_ccc, host_error_code_ccc(io_host_status_ccc));
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return INPUT_DEVICE_ERROR_RETURN_CODE;
      }
      else
      {
        extended_support_ccc = false;
        logical_sectors_per_physical = 1;
        sector_offset = 0;
        memcpy(&double_word, read_capacity_buffer_ccc + 0, 4);
        capacity = __bswap_32 (double_word);
        memcpy(&double_word, read_capacity_buffer_ccc + 4, 4);
        blocksize = __bswap_32 (double_word);
      }
      if (capacity == 0xffffffff)
      {
        allow16 = 1;
      }
    }
    if (allow16)
    {
      if (read_capacity_16_ccc() || sense_key_ccc > 1 || io_host_status_ccc)
      {
        if (sense_key_ccc > 1)
        {
          sprintf (tempmessage_ccc, "read capacity 16 failed: bad sense %d\n", sense_key_ccc);
        }
        else if (io_host_status_ccc)
        {
          sprintf (tempmessage_ccc, "read capacity 16 failed: bad host status %d\n", io_host_status_ccc);
        }
        else
        {
          sprintf (tempmessage_ccc, "read capacity 16 failed: bad return\n");
        }
        message_console_log_ccc(tempmessage_ccc, 0);
        // if read capacity 16 failed then try read capacity 10
        if (read_capacity_10_ccc() || sense_key_ccc > 1)
        {
          strcpy (tempmessage_ccc, curlang_ccc[LANGREADCAPFAILED]);
          message_error_ccc(tempmessage_ccc);
          sprintf(tempmessage_ccc, " %x %x %x.\n", sense_key_ccc, asc_ccc, ascq_ccc);
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
          clear_error_message_ccc();
          return INPUT_DEVICE_ERROR_RETURN_CODE;
        }
        else if (io_host_status_ccc)
        {
          strcpy (tempmessage_ccc, curlang_ccc[LANGHOSTIOERRCAPACITY]);
          message_error_ccc(tempmessage_ccc);
          sprintf(tempmessage_ccc, " %d (%s)", io_host_status_ccc, host_error_code_ccc(io_host_status_ccc));
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
          clear_error_message_ccc();
          return INPUT_DEVICE_ERROR_RETURN_CODE;
        }
        else
        {
          extended_support_ccc = false;
          logical_sectors_per_physical = 1;
          sector_offset = 0;
          memcpy(&double_word, read_capacity_buffer_ccc + 0, 4);
          capacity = __bswap_32 (double_word);
          memcpy(&double_word, read_capacity_buffer_ccc + 4, 4);
          blocksize = __bswap_32 (double_word);
        }
      }
      else
      {
        extended_support_ccc = true;
        memcpy(&quad_word, read_capacity_buffer_ccc + 0, 8);
        capacity = __bswap_64 (quad_word);
        memcpy(&double_word, read_capacity_buffer_ccc + 8, 4);
        blocksize =__bswap_32 (double_word);
        unsigned char c;
        memcpy(&c, read_capacity_buffer_ccc + 13, 1);
        int n;
        for (n = 0; n < c; n++)
        {
          logical_sectors_per_physical = logical_sectors_per_physical * 2;
        }
        int temp;
        memcpy(&word, read_capacity_buffer_ccc + 14, 2);
        temp = __bswap_16 (word);
        temp = temp & 0x3fff;
        sector_offset = logical_sectors_per_physical - temp;
        if (sector_offset < 0)
        {
          sector_offset = 0;
        }
      }
    }

    source_total_size_ccc = capacity + 1;
    if (capacity == 0)
    {
      source_total_size_ccc = capacity;
    }
    //fprintf(stdout, "Capacity= 0x%llx\n", capacity);    //debug
    //fprintf(stdout, "Blocksize= 0x%x\n", blocksize);    //debug
    if (!data_read_from_log_ccc)
    {
      sector_size_ccc = blocksize;
      block_size_ccc = logical_sectors_per_physical;
      block_offset_ccc = sector_offset;
    }
    if (scsi_check_read_commands_ccc())
    {
      return INPUT_DEVICE_ERROR_RETURN_CODE;
    }
  }
  else if (!copy_image_mode_ccc)
  {
    // generic block device
    char command[255];
    sprintf (command, "lsblk -d -n -b -o NAME,SIZE,LOG-SEC,PHY-SEC,ALIGNMENT,MODEL %s", disk_1_ccc);
    FILE *fp = popen(command, "r");
    int cols = 255;
    char line[cols];
    while (fgets(line, sizeof line, fp))
    {
      break;
    }
    pclose(fp);
    char name[cols];
    char model[cols];
    char raw_size[cols];
    char raw_bytes_per_log_sec[cols];
    char raw_bytes_per_phy_sec[cols];
    char raw_alignment[cols];
    strcpy (name, "");
    strcpy (model, "");
    strcpy (raw_size, "");
    strcpy (raw_bytes_per_log_sec, "");
    strcpy (raw_bytes_per_phy_sec, "");
    strcpy (raw_alignment, "");
    sscanf(line, "%s %s %s %s %s %[^\n]", name, raw_size, raw_bytes_per_log_sec, raw_bytes_per_phy_sec, raw_alignment, model);
    fprintf (stdout, "model=%s\n", model);
    strncpy (current_model_ccc, model, sizeof(current_model_ccc));
    strncpy (current_source_model_ccc, model, sizeof(current_source_model_ccc));
    // get device info
    long long size = strtoull(raw_size, NULL, 0);
    long long bytes_per_log_sec = strtoull(raw_bytes_per_log_sec, NULL, 0);
    long long bytes_per_phy_sec = strtoull(raw_bytes_per_phy_sec, NULL, 0);
    long long alignment = strtoull(raw_alignment, NULL, 0);
    long long blocksize = 1;
    long long sector_offset = 0;
    long long sector_size = bytes_per_log_sec;
    long long remainder = 0;
    if (sector_size > 0)
    {
      source_total_size_ccc = size / sector_size;
      remainder = size % sector_size;
    }
    else
    {
      source_total_size_ccc = 0;
    }
    if (remainder)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGSOURCENOTDIVISABLE]);
      message_error_ccc(tempmessage_ccc);
      sprintf(tempmessage_ccc, " %lld", size);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return INPUT_DEVICE_ERROR_RETURN_CODE;
    }
    if (bytes_per_log_sec > 0)
    {
      blocksize = bytes_per_phy_sec / bytes_per_log_sec;
      long long temp = alignment / bytes_per_log_sec;
      if (temp > 0)
      {
        sector_offset = blocksize - temp;
      }
    }

    if (!data_read_from_log_ccc)
    {
      sector_size_ccc = bytes_per_log_sec;
      block_size_ccc = blocksize;
      block_offset_ccc = sector_offset;
    }
  }
  else
  {
    // copy image file mode
    generic_source_total_size_ccc = lseek( disk1_fd_ccc, 0, SEEK_END );
    source_total_size_ccc = generic_source_total_size_ccc / sector_size_ccc;
    long long remainder = generic_source_total_size_ccc % sector_size_ccc;
    if (remainder)
    {
      strcpy (tempmessage_ccc, curlang_ccc[LANGSOURCENOTDIVISABLE]);
      message_error_ccc(tempmessage_ccc);
      sprintf(tempmessage_ccc, " %lld", generic_source_total_size_ccc);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return INPUT_DEVICE_ERROR_RETURN_CODE;
    }
  }

  if (source_total_size_ccc <= 0)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGSOURCESIZEINVALID]);
    message_error_ccc(tempmessage_ccc);
    sprintf(tempmessage_ccc, " %lld", source_total_size_ccc);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }

  if (read_size_ccc == -1 || read_size_ccc > (source_total_size_ccc - input_offset_ccc))
  {
    read_size_ccc = source_total_size_ccc - input_offset_ccc;
  }
  start_position_ccc = input_offset_ccc;
  end_position_ccc = input_offset_ccc + read_size_ccc;
  if (end_position_ccc > source_total_size_ccc)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGREADSIZEPASTEND]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return INPUT_DEVICE_ERROR_RETURN_CODE;
  }
  if (!generic_mode_ccc)
  {
    int size = IDENTIFY_BUFFER_SIZE;
    if ( (scsi_passthrough_ccc || usb_mode_ccc) && !ata_identify_success_ccc)
    {
      size = IDENTIFY_BUFFER_SIZE / 4;
      strcat (identify_device_raw_text_ccc, "# scsi inquiry\n");
    }
    unsigned char idbuffer[IDENTIFY_BUFFER_SIZE];
    if (ata_identify_success_ccc)
    {
      memcpy (idbuffer, ata_identify_buffer_ccc, IDENTIFY_BUFFER_SIZE);
    }
    else
    {
      memcpy (idbuffer, identify_buffer_ccc, IDENTIFY_BUFFER_SIZE);
    }
    char tempstring[256];
    int i;
    for (i = 0; i < size; i+=16)
    {
      sprintf (tempstring, "# %x: ", i);
      strcat (identify_device_raw_text_ccc, tempstring);
      unsigned char *c;
      int n;
      for (n=0; n < 16 && i+n < size; n++)
      {
        c = (unsigned char *)idbuffer+i+n;
        sprintf (tempstring, "%02x ", *c);
        strcat (identify_device_raw_text_ccc, tempstring);
      }
      sprintf (tempstring, "   ");
      for (n=0; n < 16 && i+n < size; n++)
      {
        c = (unsigned char *)idbuffer+i+n;
        sprintf (tempstring, "%c", isprint(*c) ? *c : '.');
        strcat (identify_device_raw_text_ccc, tempstring);
      }
      strcat (identify_device_raw_text_ccc, "\n");
    }
    if ( (scsi_passthrough_ccc || usb_mode_ccc) && !ata_identify_success_ccc)
    {
      strcat (identify_device_raw_text_ccc, "# scsi read capacity\n");
      size = 32;
      memcpy (&idbuffer, &read_capacity_buffer_ccc, size);
      int i;
      for (i = 0; i < size; i+=16)
      {
        sprintf (tempstring, "# %x: ", i);
        strcat (identify_device_raw_text_ccc, tempstring);
        unsigned char *c;
        int n;
        for (n=0; n < 16 && i+n < size; n++)
        {
          c = (unsigned char *)idbuffer+i+n;
          sprintf (tempstring, "%02x ", *c);
          strcat (identify_device_raw_text_ccc, tempstring);
        }
        sprintf (tempstring, "   ");
        for (n=0; n < 16 && i+n < size; n++)
        {
          c = (unsigned char *)idbuffer+i+n;
          sprintf (tempstring, "%c", isprint(*c) ? *c : '.');
          strcat (identify_device_raw_text_ccc, tempstring);
        }
        strcat (identify_device_raw_text_ccc, "\n");
      }
    }
    else
    {
      // list identify flags
      identify_flags_ccc.lba_supported = get_flag_data_from_identify_ccc(49, 9, 9);
      identify_flags_ccc.extended_support = get_flag_data_from_identify_ccc(83, 10, 10);
      identify_flags_ccc.drive_locked = get_flag_data_from_identify_ccc(128, 2, 2);
      identify_flags_ccc.smart_supported = get_flag_data_from_identify_ccc(82, 0, 0);
      identify_flags_ccc.smart_enabled = get_flag_data_from_identify_ccc(85, 0, 0);
      identify_flags_ccc.rebuild_assist_supported = get_flag_data_from_identify_ccc(78, 11, 11);
      identify_flags_ccc.ncq_supported = get_flag_data_from_identify_ccc(76, 8, 8);
      identify_flags_ccc.sct_error_recovery_control = get_flag_data_from_identify_ccc(206, 3, 3);
      identify_flags_ccc.multiword_dma_mode0_supported = get_flag_data_from_identify_ccc(63, 0, 0);
      identify_flags_ccc.multiword_dma_mode1_supported = get_flag_data_from_identify_ccc(63, 1, 1);
      identify_flags_ccc.multiword_dma_mode2_supported = get_flag_data_from_identify_ccc(63, 2, 2);
      identify_flags_ccc.ultra_dma_mode0_supported = get_flag_data_from_identify_ccc(88, 0, 0);
      identify_flags_ccc.ultra_dma_mode1_supported = get_flag_data_from_identify_ccc(88, 1, 1);
      identify_flags_ccc.ultra_dma_mode2_supported = get_flag_data_from_identify_ccc(88, 2, 2);
      identify_flags_ccc.ultra_dma_mode3_supported = get_flag_data_from_identify_ccc(88, 3, 3);
      identify_flags_ccc.ultra_dma_mode4_supported = get_flag_data_from_identify_ccc(88, 4, 4);
      identify_flags_ccc.ultra_dma_mode5_supported = get_flag_data_from_identify_ccc(88, 5, 5);
      identify_flags_ccc.ultra_dma_mode6_supported = get_flag_data_from_identify_ccc(88, 6, 6);
      identify_flags_ccc.multiword_dma_mode0_selected = get_flag_data_from_identify_ccc(63, 8, 8);
      identify_flags_ccc.multiword_dma_mode1_selected = get_flag_data_from_identify_ccc(63, 9, 9);
      identify_flags_ccc.multiword_dma_mode2_selected = get_flag_data_from_identify_ccc(63, 10, 10);
      identify_flags_ccc.ultra_dma_mode0_selected = get_flag_data_from_identify_ccc(88, 8, 8);
      identify_flags_ccc.ultra_dma_mode1_selected = get_flag_data_from_identify_ccc(88, 9, 9);
      identify_flags_ccc.ultra_dma_mode2_selected = get_flag_data_from_identify_ccc(88, 10, 10);
      identify_flags_ccc.ultra_dma_mode3_selected = get_flag_data_from_identify_ccc(88, 11, 11);
      identify_flags_ccc.ultra_dma_mode4_selected = get_flag_data_from_identify_ccc(88, 12, 12);
      identify_flags_ccc.ultra_dma_mode5_selected = get_flag_data_from_identify_ccc(88, 13, 13);
      identify_flags_ccc.ultra_dma_mode6_selected = get_flag_data_from_identify_ccc(88, 14, 14);
      strcat (identify_device_raw_text_ccc, "#\n");
      sprintf (tempstring, "# lba supported = %s\n", identify_flags_ccc.lba_supported ? "yes" : "no");
      strcat (identify_device_raw_text_ccc, tempstring);
      sprintf (tempstring, "# 48 bit supported = %s\n", identify_flags_ccc.extended_support ? "yes" : "no");
      strcat (identify_device_raw_text_ccc, tempstring);
      sprintf (tempstring, "# smart supported = %s\n", identify_flags_ccc.smart_supported ? "yes" : "no");
      strcat (identify_device_raw_text_ccc, tempstring);
      sprintf (tempstring, "# smart enabled = %s\n", identify_flags_ccc.smart_enabled ? "yes" : "no");
      strcat (identify_device_raw_text_ccc, tempstring);
      sprintf (tempstring, "# security locked = %s\n", identify_flags_ccc.drive_locked ? "yes" : "no");
      strcat (identify_device_raw_text_ccc, tempstring);
      //sprintf (tempstring, "# sct error recovery control supported = %s\n", identify_flags_ccc.sct_error_recovery_control ? "yes" : "no");
      //strcat (identify_device_raw_text_ccc, tempstring);
      //sprintf (tempstring, "# ncq supported = %s\n", identify_flags_ccc.ncq_supported ? "yes" : "no");
      //strcat (identify_device_raw_text_ccc, tempstring);
      sprintf (tempstring, "# rebuild assist supported = %s\n", identify_flags_ccc.rebuild_assist_supported ? "yes" : "no");
      strcat (identify_device_raw_text_ccc, tempstring);
      //sprintf (tempstring, "# hpa supported = %s\n", identify_flags_ccc.hpa_supported ? "yes" : "no");
      //strcat (identify_device_raw_text_ccc, tempstring);
      strcat (identify_device_raw_text_ccc, "#\n");
    }
  }
  return 0;
}





int get_flag_data_from_identify_ccc (unsigned int wordnumber, unsigned int lowbit, unsigned int highbit)
{
  if (wordnumber > 255 || lowbit > 15 || highbit > 15 || lowbit > highbit)
  {
    fprintf (stdout, "internal error, invalid parameter for getting identify flag data\n");
    return -1;
  }
  uint16_t word;
  memcpy(&word, ata_identify_buffer_ccc + (wordnumber*2), 2);
  //fprintf (stdout, "wordnumber = %d\n", wordnumber);    //debug
  //fprintf (stdout, "raw word = %04x\n", word);    //debug
  word = word >> lowbit;
  //fprintf (stdout, "shifted word = %04x\n", word);    //debug
  uint16_t mask = 1;
  unsigned int i;
  for (i = 0; i < highbit - lowbit; i++)
  {
    mask = mask << 1;
    mask = mask | 1;
  }
  //fprintf (stdout, "mask = %04x\n", mask);    //debug
  word = word & mask;
  //fprintf (stdout, "result = %04x\n", word);    //debug
  if (0)    //debug
  {
    int i;
    for (i = 0; i < IDENTIFY_BUFFER_SIZE; i+=16)
    {
      fprintf (stdout, "%x: ", i);
      unsigned char *c;
      int n;
      for (n=0; n < 16 && i+n < IDENTIFY_BUFFER_SIZE; n++)
      {
        c = (unsigned char *)ata_identify_buffer_ccc+i+n;
        fprintf (stdout, "%02x ", *c);
      }
      fprintf (stdout, "   ");
      for (n=0; n < 16 && i+n < IDENTIFY_BUFFER_SIZE; n++)
      {
        c = (unsigned char *)ata_identify_buffer_ccc+i+n;
        fprintf (stdout, "%c", isprint(*c) ? *c : '.');
      }
      fprintf (stdout, "\n");
    }
  }
  return word;
}





int get_highest_multiword_dma_mode_ccc(void)
{
  int ret = -1;
  if (identify_flags_ccc.multiword_dma_mode0_supported)
  {
    ret = 0;
  }
  if (identify_flags_ccc.multiword_dma_mode1_supported)
  {
    ret = 1;
  }
  if (identify_flags_ccc.multiword_dma_mode2_supported)
  {
    ret = 2;
  }
  return ret;
}





int get_current_multiword_dma_mode_ccc(void)
{
  int ret = -1;
  if (identify_flags_ccc.multiword_dma_mode0_selected)
  {
    ret = 0;
  }
  if (identify_flags_ccc.multiword_dma_mode1_selected)
  {
    ret = 1;
  }
  if (identify_flags_ccc.multiword_dma_mode2_selected)
  {
    ret = 2;
  }
  return ret;
}





int get_highest_ultra_dma_mode_ccc(void)
{
  int ret = -1;
  if (identify_flags_ccc.ultra_dma_mode0_supported)
  {
    ret = 0;
  }
  if (identify_flags_ccc.ultra_dma_mode1_supported)
  {
    ret = 1;
  }
  if (identify_flags_ccc.ultra_dma_mode2_supported)
  {
    ret = 2;
  }
  if (identify_flags_ccc.ultra_dma_mode3_supported)
  {
    ret = 3;
  }
  if (identify_flags_ccc.ultra_dma_mode4_supported)
  {
    ret = 4;
  }
  if (identify_flags_ccc.ultra_dma_mode5_supported)
  {
    ret = 5;
  }
  if (identify_flags_ccc.ultra_dma_mode6_supported)
  {
    ret = 6;
  }
  return ret;
}





int get_current_ultra_dma_mode_ccc(void)
{
  int ret = -1;
  if (identify_flags_ccc.ultra_dma_mode0_selected)
  {
    ret = 0;
  }
  if (identify_flags_ccc.ultra_dma_mode1_selected)
  {
    ret = 1;
  }
  if (identify_flags_ccc.ultra_dma_mode2_selected)
  {
    ret = 2;
  }
  if (identify_flags_ccc.ultra_dma_mode3_selected)
  {
    ret = 3;
  }
  if (identify_flags_ccc.ultra_dma_mode4_selected)
  {
    ret = 4;
  }
  if (identify_flags_ccc.ultra_dma_mode5_selected)
  {
    ret = 5;
  }
  if (identify_flags_ccc.ultra_dma_mode6_selected)
  {
    ret = 6;
  }
  return ret;
}





int process_dma_mode_ccc(void)
{
  if (ahci_mode_ccc)
  {
    int mode_changed = 0;
    identify_flags_ccc.multiword_dma_mode0_supported = get_flag_data_from_identify_ccc(63, 0, 0);
    identify_flags_ccc.multiword_dma_mode1_supported = get_flag_data_from_identify_ccc(63, 1, 1);
    identify_flags_ccc.multiword_dma_mode2_supported = get_flag_data_from_identify_ccc(63, 2, 2);
    identify_flags_ccc.ultra_dma_mode0_supported = get_flag_data_from_identify_ccc(88, 0, 0);
    identify_flags_ccc.ultra_dma_mode1_supported = get_flag_data_from_identify_ccc(88, 1, 1);
    identify_flags_ccc.ultra_dma_mode2_supported = get_flag_data_from_identify_ccc(88, 2, 2);
    identify_flags_ccc.ultra_dma_mode3_supported = get_flag_data_from_identify_ccc(88, 3, 3);
    identify_flags_ccc.ultra_dma_mode4_supported = get_flag_data_from_identify_ccc(88, 4, 4);
    identify_flags_ccc.ultra_dma_mode5_supported = get_flag_data_from_identify_ccc(88, 5, 5);
    identify_flags_ccc.ultra_dma_mode6_supported = get_flag_data_from_identify_ccc(88, 6, 6);
    identify_flags_ccc.multiword_dma_mode0_selected = get_flag_data_from_identify_ccc(63, 8, 8);
    identify_flags_ccc.multiword_dma_mode1_selected = get_flag_data_from_identify_ccc(63, 9, 9);
    identify_flags_ccc.multiword_dma_mode2_selected = get_flag_data_from_identify_ccc(63, 10, 10);
    identify_flags_ccc.ultra_dma_mode0_selected = get_flag_data_from_identify_ccc(88, 8, 8);
    identify_flags_ccc.ultra_dma_mode1_selected = get_flag_data_from_identify_ccc(88, 9, 9);
    identify_flags_ccc.ultra_dma_mode2_selected = get_flag_data_from_identify_ccc(88, 10, 10);
    identify_flags_ccc.ultra_dma_mode3_selected = get_flag_data_from_identify_ccc(88, 11, 11);
    identify_flags_ccc.ultra_dma_mode4_selected = get_flag_data_from_identify_ccc(88, 12, 12);
    identify_flags_ccc.ultra_dma_mode5_selected = get_flag_data_from_identify_ccc(88, 13, 13);
    identify_flags_ccc.ultra_dma_mode6_selected = get_flag_data_from_identify_ccc(88, 14, 14);
    int ultra_mode_supported = get_highest_ultra_dma_mode_ccc();
    int multiword_mode_supported = get_highest_multiword_dma_mode_ccc();
    if (ultra_mode_supported >= 0)
    {
      int ultra_mode_selected = get_current_ultra_dma_mode_ccc();
      fprintf (stdout, "setting ultra dma mode %d \n", ultra_mode_supported);
      if (1 || ultra_mode_selected != ultra_mode_supported)
      {
        unsigned char new_mode = ultra_mode_supported | 0x40;
        int ret = set_dma_mode_ccc(new_mode);
        if (ret)
        {
          fprintf (stdout, "setting ultra dma mode %d failed %d\n", ultra_mode_supported, ret);
          return -1;
        }
        else if (ata_status_ccc & 1)
        {
          fprintf (stdout, "setting ultra dma mode %d failed status/error %02x/%02x\n", ultra_mode_supported, ata_status_ccc, ata_error_ccc);
          return -1;
        }
        mode_changed = 1;
      }
    }
    else if (multiword_mode_supported >= 0)
    {
      int multiword_mode_selected = get_current_multiword_dma_mode_ccc();
      fprintf (stdout, "setting multiword dma mode %d \n", multiword_mode_supported);
      if (1 || multiword_mode_selected != multiword_mode_supported)
      {
        unsigned char new_mode = multiword_mode_supported | 0x20;
        int ret = set_dma_mode_ccc(new_mode);
        if (ret)
        {
          fprintf (stdout, "setting multiword dma mode %d failed %d\n", multiword_mode_supported, ret);
          return -1;
        }
        else if (ata_status_ccc & 1)
        {
          fprintf (stdout, "setting multiword dma mode %d failed status/error %02x/%02x\n", multiword_mode_supported, ata_status_ccc, ata_error_ccc);
          return -1;
        }
        mode_changed = 1;
      }
    }
    return mode_changed;
  }
  return 0;
}





int extract_smart_data_ccc(void)
{
  if (!smart_supported_ccc)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGSMARTNOTSUPPORTED]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return 1;
  }
  if (!smart_enabled_ccc)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGSMARTNOTENABLED]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return 1;
  }
  unsigned char smart_data[512];
  memset (smart_data, 0, sizeof(smart_data));
  unsigned char threshold_data[512];
  memset (threshold_data, 0, sizeof(threshold_data));
  ccc_main_buffer_size_ccc = 512;
  set_main_buffer_ccc();
  memset(ccc_buffer_ccc, 0, 512);

  // read smart data
  unsigned char regs[7] = {0xd0, 0, 0, 0x4f, 0xc2, 0xa0, 0xb0};
  int retsmart;
  if (usb_mode_ccc)
  {
    retsmart = usb_read_smart_ccc(initial_busy_wait_time_ccc / 1000, regs);
  }
  else
  {
    retsmart = ata_28_pio_read_ccc(regs);
  }
  if (!retsmart && !(ata_status_ccc & 1) && sense_key_ccc < 2)
  {
    if (usb_mode_ccc)
    {
      memcpy(smart_data, ccc_usbbuffer_ccc, 512);
    }
    else
    {
      memcpy(smart_data, ccc_buffer_ccc, 512);
    }
  }
  else
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGERRORREADINGSMART]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, "return=%d status=0x%x sense=0x%x", retsmart, ata_status_ccc, sense_key_ccc);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return 1;
  }

  if (0)
  {
    int i;
    for (i = 0; i < 512; i+=16)
    {
      fprintf (stdout, "%x: ", i);
      int n;
      for (n=0; n < 16 && i+n < 512; n++)
      {
        fprintf (stdout, "%02x ", smart_data[i+n]);
      }
      fprintf (stdout, "   ");
      for (n=0; n < 16 && i+n < 512; n++)
      {
        fprintf (stdout, "%c", isprint(smart_data[i+n]) ? smart_data[i+n]: '.');
      }
      fprintf (stdout, "\n");
    }
  }

  // read threshold data
  int threshold_data_valid = 0;
  int retthresh;
  unsigned char regt[7] = {0xd1, 0, 0, 0x4f, 0xc2, 0xa0, 0xb0};
  if (usb_mode_ccc)
  {
    retthresh = usb_read_smart_ccc(initial_busy_wait_time_ccc / 1000, regt);
  }
  else
  {
    retthresh = ata_28_pio_read_ccc(regt);
  }
  if (!retthresh && !(ata_status_ccc & 1) && sense_key_ccc < 2)
  {
    if (usb_mode_ccc)
    {
      memcpy(threshold_data, ccc_usbbuffer_ccc, 512);
    }
    else
    {
      memcpy(threshold_data, ccc_buffer_ccc, 512);
    }
    threshold_data_valid = 1;
  }

  char temp[20000];
  uint16_t version;
  memcpy (&version, smart_data, 2);
  sprintf (temp, "# %s %d\n", curlang_ccc[LANGSMARTVERSION], version);
  strcpy (smart_data_text_ccc, temp);
  sprintf (temp, "# %s    \t%s  \t%s  \t%s  \t%s  \t%s      \t%s", curlang_ccc[LANGID], curlang_ccc[LANGFLAGS], curlang_ccc[LANGCURRENT], curlang_ccc[LANGWORST], curlang_ccc[LANGTHRESHOLD], curlang_ccc[LANGRAWDATA], curlang_ccc[LANGATTRIBUTENAME]);
  strcat (smart_data_text_ccc, temp);
  uint8_t threshold = 0;
  int offset = 2;
  int count = 0;
  while (count < 30)
  {
    threshold = 0;
    uint8_t id;
    uint16_t flags;
    uint8_t current;
    uint8_t worst;
    uint64_t raw;
    memcpy (&id, smart_data + offset, 1);
    memcpy (&flags, smart_data + offset + 1, 2);
    memcpy (&current, smart_data + offset + 3, 1);
    memcpy (&worst, smart_data + offset + 4, 1);
    memcpy (&raw, smart_data + offset + 5, 8);
    raw = raw & 0xffffffffffffff;
    char *name = get_smart_attribute_name_ccc(id);

    if (threshold_data_valid && id > 0)
    {
      int tcount = 0;
      int toffset = 2;
      while (tcount < 30)
      {
        uint8_t tid;
        memcpy (&tid, threshold_data + offset, 1);
        if (tid == id)
        {
          memcpy (&threshold, threshold_data + offset + 1, 1);
        }
        toffset += 12;
        tcount++;
      }
    }

    if (id > 0)
    {
      sprintf (temp, "\n# %03d  \t0x%04x   \t%03d  \t%03d  \t%03d  \t0x%014llx    \t%s", id, flags, current, worst, threshold, (unsigned long long)raw, name);
      strcat (smart_data_text_ccc, temp);
      if (id == 5 && raw > 0)
      {
        sprintf (temp, "\n# *** WARNING *** Reallocated Sectors Count = %lld", (unsigned long long)raw);
        strcat (smart_data_text_ccc, temp);
      }
      else if (id == 197 && raw > 0)
      {
        sprintf (temp, "\n# *** WARNING *** Current Pending Sectors = %lld", (unsigned long long)raw);
        strcat (smart_data_text_ccc, temp);
      }
    }

    offset += 12;
    count++;
  }

  return 0;
}




char *get_smart_attribute_name_ccc(int id)
{
  char *message;
  switch (id)
  {
    case 0:
      message = "Invalid ID";
      break;

    case 1:
      message = "Read Error Rate";
      break;

    case 2:
      message = "Throughput Performance";
      break;

    case 3:
      message = "Spin-Up Time";
      break;

    case 4:
      message = "Start/Stop Count";
      break;

    case 5:
      message = "Reallocated Sectors Count";
      break;

    case 6:
      message = "Read Channel Margin";
      break;

    case 7:
      message = "Seek Error Rate";
      break;

    case 8:
      message = "Seek Time Performance";
      break;

    case 9:
      message = "Power-On Hours Count";
      break;

    case 10:
      message = "Spin Retry Count";
      break;

    case 11:
      message = "Calibration Retries";
      break;

    case 12:
      message = "Power Cycle Count";
      break;

    case 13:
      message = "Soft Read Error Rate";
      break;

    case 100:
      message = "Erase/Program Cycles";
      break;

    case 103:
      message = "Translation Table Rebuild";
      break;

    case 166:
      message = "Min W/E Cycle";
      break;

    case 167:
      message = "Min Bad Block/Die";
      break;

    case 168:
      message = "Maximum Erase Cycle";
      break;

    case 169:
      message = "Total Bad Block";
      break;

    case 170:
      message = "Reserved Block Count";
      break;

    case 171:
      message = "Program Fail Count";
      break;

    case 172:
      message = "Erase Fail Count";
      break;

    case 173:
      message = "Wear Leveller Worst Case Erase Count";
      break;

    case 174:
      message = "Unexpected Power Loss";
      break;

    case 175:
      message = "Program Fail Count";
      break;

    case 176:
      message = "Erase Fail Count";
      break;

    case 177:
      message = "Wear Leveling Count";
      break;

    case 178:
      message = "Used Reserved Block Count";
      break;

    case 179:
      message = "Used Reserved Block Count";
      break;

    case 180:
      message = "Unused Reserved Block Count";
      break;

    case 181:
      message = "Program Fail Count";
      break;

    case 182:
      message = "Erase Fail Count";
      break;

    case 183:
      message = "SATA Downshifts";
      break;

    case 184:
      message = "End-to-End error";
      break;

    case 185:
      message = "Head Stability";
      break;

    case 186:
      message = "Induced Op-Vibration Detection";
      break;

    case 187:
      message = "Reported Uncorrectable Errors";
      break;

    case 188:
      message = "Command Timeout";
      break;

    case 189:
      message = "High Fly Writes";
      break;

    case 190:
      message = "Temperature";
      break;

    case 191:
      message = "G-Sense Errors";
      break;

    case 192:
      message = "Power-Off Retract Cycles";
      break;

    case 193:
      message = "Load/Unload Cycles";
      break;

    case 194:
      message = "Temperature";
      break;

    case 195:
      message = "Hardware ECC Recovered";
      break;

    case 196:
      message = "Reallocation Events";
      break;

    case 197:
      message = "Current Pending Sectors";
      break;

    case 198:
      message = "Off-line Uncorrectable";
      break;

    case 199:
      message = "UDMA CRC Error Rate";
      break;

    case 200:
      message = "Write Error Rate";
      break;

    case 201:
      message = "Soft Read Errors";
      break;

    case 202:
      message = "Data Address Mark Errors";
      break;

    case 203:
      message = "Run Out Cancel";
      break;

    case 204:
      message = "Soft ECC Corrections";
      break;

    case 205:
      message = "Thermal Asperity Rate";
      break;

    case 206:
      message = "Flying Height";
      break;

    case 207:
      message = "Spin High Current";
      break;

    case 208:
      message = "Spin Buzz";
      break;

    case 209:
      message = "Offline Seek Performance";
      break;

    case 210:
      message = "Vibration During Write";
      break;

    case 211:
      message = "Vibration During Write";
      break;

    case 212:
      message = "Shock During Write";
      break;

    case 220:
      message = "Disk Shift";
      break;

    case 221:
      message = "G-Sense Error Rate";
      break;

    case 222:
      message = "Loaded Hours";
      break;

    case 223:
      message = "Load/Unload Retries";
      break;

    case 224:
      message = "Load Friction";
      break;

    case 225:
      message = "Load/Unload Cycles";
      break;

    case 226:
      message = "Load-in Time";
      break;

    case 227:
      message = "Torque Amplification Count";
      break;

    case 228:
      message = "Power-Off Retracts";
      break;

    case 230:
      message = "GMR Head Amplitude";
      break;

    case 231:
      message = "Temperature";
      break;

    case 232:
      message = "Available Reserved Space";
      break;

    case 233:
      message = "Media Wearout Indicator";
      break;

    case 234:
      message = "Average/Maximum Erase Count";
      break;

    case 235:
      message = "Good/System(Free) Block Count";
      break;

    case 240:
      message = "Head Flying Hours";
      break;

    case 241:
      message = "Total LBAs Written";
      break;

    case 242:
      message = "Total LBAs Read";
      break;

    case 249:
      message = "NAND_Writes_1GiB";
      break;

    case 250:
      message = "Read Error Retry Rate";
      break;

    case 254:
      message = "Free Fall Protection";
      break;

    default:
      message = "Unknown Attribute ID";
      break;
  }
  return message;
}





long long block_align_ccc (long long value, int offset)
{
  long long new_value = value / block_size_ccc;
  new_value = (new_value * block_size_ccc) - offset;
  return new_value;
}



long long block_align_up_ccc (long long value)
{
  long long new_value = value / block_size_ccc;
  long long remainder = value % block_size_ccc;
  if (remainder)
  {
    new_value = (new_value + 1) * block_size_ccc;
  }
  else
  {
    new_value = new_value * block_size_ccc;
  }
  return new_value;
}





void quit_and_exit_ccc (void)
{
  sprintf(tempmessage_ccc, "Terminated by user\n");
  message_exit_ccc(tempmessage_ccc);
  cleanup_ccc();
  exit (exitcode_ccc);
}





void clear_mode_ccc (void)
{
  copy_image_mode_ccc = false;
  generic_mode_ccc = false;
  scsi_passthrough_ccc = false;
  nvme_passthrough_ccc = false;
  auto_passthrough_ccc = false;
  ata_passthrough_ccc = false;
  direct_mode_ccc = false;
  ahci_mode_ccc = false;
  usb_mode_ccc = false;
  usb_allow_ata_ccc = false;
  fill_mode_ccc = false;
  fill_mark_ccc = false;
  fill_erase_ccc = false;
  pio_mode_ccc = false;
  limit_recovery_ccc = false;
  driver_only_ccc = false;
}




void set_mode_copy_image_ccc (void)
{
  if (!copy_image_mode_ccc)
  {
    clear_source_ccc();
    clear_mode_ccc();
    generic_mode_ccc = true;
    copy_image_mode_ccc = true;
    update_mode_ccc();
    // re-initialize memory after mode change
    initialize_memory_ccc();
  }
}



void set_mode_generic_ccc (void)
{
  if (!generic_mode_ccc || copy_image_mode_ccc)
  {
    clear_source_ccc();
    clear_mode_ccc();
    generic_mode_ccc = true;
    update_mode_ccc();
    // re-initialize memory after mode change
    initialize_memory_ccc();
  }
}



void set_mode_auto_passthrough_ccc (void)
{
  if (!auto_passthrough_ccc)
  {
    if (direct_mode_ccc || ahci_mode_ccc)
    {
      clear_source_ccc();
    }
    clear_mode_ccc();
    scsi_passthrough_ccc = true;
    auto_passthrough_ccc = true;
    update_mode_ccc();
    // re-initialize memory after mode change
    initialize_memory_ccc();
  }
}



void set_mode_scsi_passthrough_ccc (void)
{
  if (!scsi_passthrough_ccc || auto_passthrough_ccc)
  {
    if (direct_mode_ccc || ahci_mode_ccc)
    {
      clear_source_ccc();
    }
    clear_mode_ccc();
    scsi_passthrough_ccc = true;
    update_mode_ccc();
    // re-initialize memory after mode change
    initialize_memory_ccc();
  }
}



void set_mode_ata_passthrough_ccc (void)
{
  if (!ata_passthrough_ccc || auto_passthrough_ccc)
  {
    if (direct_mode_ccc || ahci_mode_ccc)
    {
      clear_source_ccc();
    }
    clear_mode_ccc();
    ata_passthrough_ccc = true;
    update_mode_ccc();
    // re-initialize memory after mode change
    initialize_memory_ccc();
  }
}



void set_mode_nvme_passthrough_ccc (void)
{
  if (!nvme_passthrough_ccc || auto_passthrough_ccc)
  {
    if (direct_mode_ccc || ahci_mode_ccc)
    {
      clear_source_ccc();
    }
    clear_mode_ccc();
    nvme_passthrough_ccc = true;
    update_mode_ccc();
    // re-initialize memory after mode change
    initialize_memory_ccc();
  }
}



void set_mode_direct_ccc (void)
{
  if (!direct_mode_ccc || ahci_mode_ccc)
  {
    clear_mode_ccc();
    direct_mode_ccc = true;
    clear_source_ccc();
    update_mode_ccc();
    // re-initialize memory after mode change
    if (initialize_memory_ccc())
    {
      //set_mode_auto_passthrough_ccc();
    }
  }
}



void set_mode_ahci_ccc (void)
{
  if (!ahci_mode_ccc)
  {
    clear_mode_ccc();
    direct_mode_ccc = true;
    ahci_mode_ccc = true;
    clear_source_ccc();
    update_mode_ccc();
    // re-initialize memory after mode change
    if (initialize_memory_ccc())
    {
      //set_mode_auto_passthrough_ccc();
    }
  }
}





void set_mode_usb_ccc (void)
{
  if (!usb_mode_ccc || usb_allow_ata_ccc)
  {
    clear_mode_ccc();
    usb_mode_ccc = true;
    usb_allow_ata_ccc = false;
    clear_source_ccc();
    update_mode_ccc();
    // re-initialize memory after mode change
    initialize_memory_ccc();
  }
}





void set_mode_usb_ata_ccc (void)
{
  if (!usb_mode_ccc || !usb_allow_ata_ccc)
  {
    clear_mode_ccc();
    usb_mode_ccc = true;
    usb_allow_ata_ccc = true;
    clear_source_ccc();
    update_mode_ccc();
    // re-initialize memory after mode change
    initialize_memory_ccc();
  }
}





void set_mode_fillzero_ccc (void)
{
  if (!fill_mode_ccc)
  {
    clear_mode_ccc();
    fill_mode_ccc = true;
    clear_source_ccc();
    source_total_size_ccc = (total_lines_ccc)? lposition_ccc[total_lines_ccc - 1] + lsize_ccc[total_lines_ccc - 1] : 0;
    update_mode_ccc();
    // re-initialize memory after mode change
    initialize_memory_ccc();
  }
}





void set_mode_fillmark_ccc (void)
{
  if (!fill_mark_ccc)
  {
    clear_mode_ccc();
    fill_mode_ccc = true;
    fill_mark_ccc = true;
    clear_source_ccc();
    source_total_size_ccc = (total_lines_ccc)? lposition_ccc[total_lines_ccc - 1] + lsize_ccc[total_lines_ccc - 1] : 0;
    update_mode_ccc();
    // re-initialize memory after mode change
    initialize_memory_ccc();
  }
}





void set_mode_fillerase_ccc (void)
{
  if (!fill_erase_ccc)
  {
    clear_mode_ccc();
    fill_mode_ccc = true;
    fill_erase_ccc = true;
    clear_source_ccc();
    source_total_size_ccc = 0;
    update_mode_ccc();
    // re-initialize memory after mode change
    initialize_memory_ccc();
  }
}





void set_mode_driveronly_ccc (void)
{
  if (!driver_only_ccc)
  {
    clear_mode_ccc();
    driver_only_ccc = true;
    clear_source_ccc();
    source_total_size_ccc = (total_lines_ccc)? lposition_ccc[total_lines_ccc - 1] + lsize_ccc[total_lines_ccc - 1] : 0;
    update_mode_ccc();
    // re-initialize memory after mode change
    initialize_memory_ccc();
  }
}





void load_clone_settings_ccc (void)
{
  fprintf (stdout, "loading clone settings\n");

  clone_settings_ccc.no_phase1 = no_phase1_ccc;
  clone_settings_ccc.no_phase2 = no_phase2_ccc;
  clone_settings_ccc.no_phase3 = no_phase3_ccc;
  clone_settings_ccc.no_phase4 = no_phase4_ccc;
  clone_settings_ccc.no_trim = no_trim_ccc;
  clone_settings_ccc.no_scrape = no_scrape_ccc;
  clone_settings_ccc.no_divide1 = no_divide1_ccc;
  clone_settings_ccc.do_divide2 = do_divide2_ccc;
  clone_settings_ccc.read_bad_head = read_bad_head_ccc;
  clone_settings_ccc.rebuild_assist = rebuild_assist_ccc;
  clone_settings_ccc.mark_bad_head = mark_bad_head_ccc;
  clone_settings_ccc.reverse = reverse_ccc;
  clone_settings_ccc.retries = retries_ccc;
  clone_settings_ccc.cluster_size = original_cluster_size_ccc;
  clone_settings_ccc.block_size = block_size_ccc;
  clone_settings_ccc.input_offset = input_offset_ccc;
  //clone_settings_ccc.output_offset = output_offset_ccc;
  clone_settings_ccc.read_size = read_size_ccc;
  clone_settings_ccc.skip_fast = skip_fast_ccc;
  clone_settings_ccc.rate_skip = rate_skip_ccc;
  clone_settings_ccc.exit_on_slow = exit_on_slow_ccc;
  clone_settings_ccc.exit_slow_time = exit_slow_time_ccc;
  clone_settings_ccc.min_skip_size = min_skip_size_ccc;
  clone_settings_ccc.min_skip_size_bak = min_skip_size_ccc;
  clone_settings_ccc.max_skip_size = max_skip_size_ccc;
  clone_settings_ccc.skip_timeout = skip_timeout_ccc;
  clone_settings_ccc.sector_size = sector_size_ccc;
  clone_settings_ccc.block_offset = block_offset_ccc;
  clone_settings_ccc.max_read_rate = max_read_rate_ccc;

}





void update_clone_settings_ccc (void)
{
  fprintf (stdout, "updating clone settings\n");

  no_phase1_ccc = clone_settings_ccc.no_phase1;
  no_phase2_ccc = clone_settings_ccc.no_phase2;
  no_phase3_ccc = clone_settings_ccc.no_phase3;
  no_phase4_ccc = clone_settings_ccc.no_phase4;
  no_trim_ccc = clone_settings_ccc.no_trim;
  no_scrape_ccc = clone_settings_ccc.no_scrape;
  no_divide1_ccc = clone_settings_ccc.no_divide1;
  do_divide2_ccc = clone_settings_ccc.do_divide2;
  read_bad_head_ccc = clone_settings_ccc.read_bad_head;
  rebuild_assist_ccc = clone_settings_ccc.rebuild_assist;
  mark_bad_head_ccc = clone_settings_ccc.mark_bad_head;
  reverse_ccc = clone_settings_ccc.reverse;
  retries_ccc = clone_settings_ccc.retries;
  original_cluster_size_ccc = clone_settings_ccc.cluster_size;
  block_size_ccc = clone_settings_ccc.block_size;
  input_offset_ccc = clone_settings_ccc.input_offset;
  //output_offset_ccc = clone_settings_ccc.output_offset;
  read_size_ccc = clone_settings_ccc.read_size;
  skip_fast_ccc = clone_settings_ccc.skip_fast;
  rate_skip_ccc = clone_settings_ccc.rate_skip;
  exit_on_slow_ccc = clone_settings_ccc.exit_on_slow;
  exit_slow_time_ccc = clone_settings_ccc.exit_slow_time;
  min_skip_size_ccc = clone_settings_ccc.min_skip_size;
  max_skip_size_ccc = clone_settings_ccc.max_skip_size;
  skip_timeout_ccc = clone_settings_ccc.skip_timeout;
  sector_size_ccc = clone_settings_ccc.sector_size;
  block_offset_ccc = clone_settings_ccc.block_offset;
  max_read_rate_ccc = clone_settings_ccc.max_read_rate;

  if (clone_settings_ccc.min_skip_size != clone_settings_ccc.min_skip_size_bak)
  {
    original_min_skip_size_ccc = min_skip_size_ccc;
  }
  original_max_skip_size_ccc = max_skip_size_ccc;
  cluster_size_ccc = original_cluster_size_ccc;

  if (read_size_ccc > (source_total_size_ccc - input_offset_ccc))
  {
    read_size_ccc = source_total_size_ccc - input_offset_ccc;
  }
  start_position_ccc = input_offset_ccc;
  end_position_ccc = input_offset_ccc + read_size_ccc;

  if (exit_on_slow_ccc > 0)
  {
    skip_timeout_ccc = ALTERNATE_SKIP_TIMEOUT * 1000;
  }

}





void load_advanced_settings_ccc (void)
{
  fprintf (stdout, "loading advanced settings\n");

  advanced_settings_ccc.force_mounted = force_mounted_ccc;
  advanced_settings_ccc.force_dangerous = force_danger_ccc;
  advanced_settings_ccc.no_log_backup = no_log_backup_ccc;
  advanced_settings_ccc.enable_output_offset = enable_output_offset_ccc;
  advanced_settings_ccc.output_offset = output_offset_ccc;
  advanced_settings_ccc.enable_current_position = enable_current_position_ccc;
  advanced_settings_ccc.current_position = current_position_ccc;
  advanced_settings_ccc.stop_on_error = stop_on_error_ccc;
  advanced_settings_ccc.call_command = do_call_command_ccc;
  advanced_settings_ccc.activate_primary_relay = activate_primary_relay_on_error_ccc;
  advanced_settings_ccc.write_buffer_disable = write_buffer_disable_ccc;
  advanced_settings_ccc.write_buffer_enable = write_buffer_enable_ccc;
  advanced_settings_ccc.driver_return_error = driver_return_error_ccc;
  advanced_settings_ccc.driver_return_zeros = driver_return_zeros_ccc;
  advanced_settings_ccc.driver_return_marked = driver_return_marked_ccc;
  advanced_settings_ccc.disable_identify = enable_dont_identify_on_choose_source_ccc;
  advanced_settings_ccc.pio_mode = pio_mode_ccc;
  advanced_settings_ccc.enable_rebuild_assist = use_rebuild_assist_ccc;
  advanced_settings_ccc.enable_process_chunk = enable_process_chunk_ccc;
  advanced_settings_ccc.enable_read_twice = enable_read_twice_ccc;
  advanced_settings_ccc.enable_retry_connecting = enable_retry_connecting_ccc;
  advanced_settings_ccc.enable_scsi_write = enable_scsi_write_ccc;
  advanced_settings_ccc.enable_phase_log = enable_logfile_phase_ccc;
  advanced_settings_ccc.enable_output_sector_size = enable_output_sector_size_ccc;
  advanced_settings_ccc.output_sector_adjustment = output_sector_size_adjustment_ccc;
  advanced_settings_ccc.driver_io_scsi_only = driver_io_scsi_only_ccc;
  advanced_settings_ccc.use_physical_sector_size_for_virtual = use_physical_sector_size_for_virtual_ccc;
  advanced_settings_ccc.driver_minimum_cluster_size = driver_minimum_cluster_size_ccc;
  advanced_settings_ccc.color_statusbar = color_statusbar_ccc;
  strcpy(advanced_settings_ccc.command_to_call, command_to_call_ccc);
  strcpy(advanced_settings_ccc.virtual_disk_device_name, virtual_driver_name_ccc);

}





void update_advanced_settings_ccc (void)
{
  fprintf (stdout, "updating advanced settings\n");

  force_mounted_ccc = advanced_settings_ccc.force_mounted;
  force_danger_ccc = advanced_settings_ccc.force_dangerous;
  no_log_backup_ccc = advanced_settings_ccc.no_log_backup;
  enable_output_offset_ccc = advanced_settings_ccc.enable_output_offset;
  output_offset_ccc = advanced_settings_ccc.output_offset;
  enable_current_position_ccc = advanced_settings_ccc.enable_current_position;
  current_position_ccc = advanced_settings_ccc.current_position;
  enable_current_position_ccc = advanced_settings_ccc.enable_current_position;
  stop_on_error_ccc = advanced_settings_ccc.stop_on_error;
  do_call_command_ccc = advanced_settings_ccc.call_command;
  activate_primary_relay_on_error_ccc = advanced_settings_ccc.activate_primary_relay;
  write_buffer_disable_ccc = advanced_settings_ccc.write_buffer_disable;
  write_buffer_enable_ccc = advanced_settings_ccc.write_buffer_enable;
  driver_return_error_ccc = advanced_settings_ccc.driver_return_error;
  driver_return_zeros_ccc = advanced_settings_ccc.driver_return_zeros;
  driver_return_marked_ccc = advanced_settings_ccc.driver_return_marked;
  enable_dont_identify_on_choose_source_ccc = advanced_settings_ccc.disable_identify;
  pio_mode_ccc = advanced_settings_ccc.pio_mode;
  use_rebuild_assist_ccc = advanced_settings_ccc.enable_rebuild_assist;
  enable_process_chunk_ccc = advanced_settings_ccc.enable_process_chunk;
  enable_read_twice_ccc = advanced_settings_ccc.enable_read_twice;
  enable_retry_connecting_ccc = advanced_settings_ccc.enable_retry_connecting;
  enable_scsi_write_ccc = advanced_settings_ccc.enable_scsi_write;
  enable_logfile_phase_ccc = advanced_settings_ccc.enable_phase_log;
  enable_output_sector_size_ccc = advanced_settings_ccc.enable_output_sector_size;
  output_sector_size_adjustment_ccc = advanced_settings_ccc.output_sector_adjustment;
  driver_io_scsi_only_ccc= advanced_settings_ccc.driver_io_scsi_only;
  use_physical_sector_size_for_virtual_ccc= advanced_settings_ccc.use_physical_sector_size_for_virtual;
  driver_minimum_cluster_size_ccc = advanced_settings_ccc.driver_minimum_cluster_size;
  color_statusbar_ccc = advanced_settings_ccc.color_statusbar;
  strcpy(command_to_call_ccc, advanced_settings_ccc.command_to_call);
  strcpy(virtual_driver_name_ccc, advanced_settings_ccc.virtual_disk_device_name);

}





void load_timer_settings_ccc (void)
{
  fprintf (stdout, "loading timer settings\n");

  power_cycle_time_ccc = reset_wait_time_ccc;

  timer_settings_ccc.initial_busy_wait_time = initial_busy_wait_time_ccc / 1000;
  timer_settings_ccc.busy_wait_time = busy_wait_time_ccc / 1000;
  timer_settings_ccc.soft_reset_time = soft_reset_time_ccc / 1000;
  timer_settings_ccc.hard_reset_time = hard_reset_time_ccc / 1000;
  timer_settings_ccc.power_cycle_time = power_cycle_time_ccc / 1000;
  timer_settings_ccc.general_timeout = general_timeout_ccc / 1000;
  timer_settings_ccc.stop_on_power_cycle = stop_on_power_cycle_ccc;
  timer_settings_ccc.call_power_command = call_power_command_ccc;
  timer_settings_ccc.power_cycle_primary_relay = power_cycle_primary_relay_ccc;
  strcpy(timer_settings_ccc.power_command_to_call, power_command_to_call_ccc);

  timer_settings_ccc.phase_timers = phase_timers_ccc;
  timer_settings_ccc.p12_soft_reset_time = p12_soft_reset_time_ccc / 1000;
  timer_settings_ccc.p3_soft_reset_time = p3_soft_reset_time_ccc / 1000;
  timer_settings_ccc.p4_soft_reset_time = p4_soft_reset_time_ccc / 1000;
  timer_settings_ccc.td_soft_reset_time = td_soft_reset_time_ccc / 1000;
  timer_settings_ccc.d2_soft_reset_time = d2_soft_reset_time_ccc / 1000;
  timer_settings_ccc.sc_soft_reset_time = sc_soft_reset_time_ccc / 1000;
  timer_settings_ccc.rt_soft_reset_time = rt_soft_reset_time_ccc / 1000;

  timer_settings_ccc.always_wait_for_reset_timers = always_wait_for_reset_timers_ccc;

  timer_settings_ccc.usb_bulk_reset_enabled = usb_bulk_reset_enabled_ccc;
  timer_settings_ccc.usb_soft_reset_enabled = usb_soft_reset_enabled_ccc;
  timer_settings_ccc.usb_hard_reset_enabled = usb_hard_reset_enabled_ccc;
  timer_settings_ccc.usb_port_reset_enabled = usb_port_reset_enabled_ccc;

}





void update_timer_settings_ccc (void)
{
  fprintf (stdout, "updating timer settings\n");

  initial_busy_wait_time_ccc = timer_settings_ccc.initial_busy_wait_time * 1000;
  busy_wait_time_ccc = timer_settings_ccc.busy_wait_time * 1000;
  soft_reset_time_ccc = timer_settings_ccc.soft_reset_time * 1000;
  orig_soft_reset_time_ccc = timer_settings_ccc.soft_reset_time * 1000;
  hard_reset_time_ccc = timer_settings_ccc.hard_reset_time * 1000;
  power_cycle_time_ccc = timer_settings_ccc.power_cycle_time * 1000;
  general_timeout_ccc = timer_settings_ccc.general_timeout * 1000;
  stop_on_power_cycle_ccc = timer_settings_ccc.stop_on_power_cycle;
  call_power_command_ccc = timer_settings_ccc.call_power_command;
  power_cycle_primary_relay_ccc = timer_settings_ccc.power_cycle_primary_relay;
  strcpy(power_command_to_call_ccc, timer_settings_ccc.power_command_to_call);

  phase_timers_ccc = timer_settings_ccc.phase_timers;
  p12_soft_reset_time_ccc = timer_settings_ccc.p12_soft_reset_time * 1000;
  p3_soft_reset_time_ccc = timer_settings_ccc.p3_soft_reset_time * 1000;
  p4_soft_reset_time_ccc = timer_settings_ccc.p4_soft_reset_time * 1000;
  td_soft_reset_time_ccc = timer_settings_ccc.td_soft_reset_time * 1000;
  d2_soft_reset_time_ccc = timer_settings_ccc.d2_soft_reset_time * 1000;
  sc_soft_reset_time_ccc = timer_settings_ccc.sc_soft_reset_time * 1000;
  rt_soft_reset_time_ccc = timer_settings_ccc.rt_soft_reset_time * 1000;

  always_wait_for_reset_timers_ccc = timer_settings_ccc.always_wait_for_reset_timers;

  usb_bulk_reset_enabled_ccc = timer_settings_ccc.usb_bulk_reset_enabled;
  usb_soft_reset_enabled_ccc = timer_settings_ccc.usb_soft_reset_enabled;
  usb_hard_reset_enabled_ccc = timer_settings_ccc.usb_hard_reset_enabled;
  usb_port_reset_enabled_ccc = timer_settings_ccc.usb_port_reset_enabled;

  reset_wait_time_ccc = power_cycle_time_ccc;

}





void set_soft_timer_ccc (int status)
{
  soft_reset_time_ccc = orig_soft_reset_time_ccc;
  if (phase_timers_ccc)
  {
    if (status == PHASE1 || status == PHASE2)
    {
      soft_reset_time_ccc = p12_soft_reset_time_ccc;
    }
    else if (status == PHASE3)
    {
      soft_reset_time_ccc = p3_soft_reset_time_ccc;
    }
    else if (status == PHASE4)
    {
      soft_reset_time_ccc = p4_soft_reset_time_ccc;
    }
    else if (status == TRIMMING || status == DIVIDING1)
    {
      soft_reset_time_ccc = td_soft_reset_time_ccc;
    }
    else if (status == DIVIDING2)
    {
      soft_reset_time_ccc = d2_soft_reset_time_ccc;
    }
    else if (status == SCRAPING)
    {
      soft_reset_time_ccc = sc_soft_reset_time_ccc;
    }
    else if (status == RETRYING)
    {
      soft_reset_time_ccc = rt_soft_reset_time_ccc;
    }
  }
  //fprintf (stdout, "soft timer= %lld\n", soft_reset_time_ccc);    //debug
}





void reset_current_status_ccc (void)
{
  if (!no_log_backup_ccc)
  {
    get_the_time_ccc();
    char tempname[2048];
    strncpy (tempname, log_file_ccc, 1024);
    strcat (tempname, ".bak.");
    strncat (tempname, current_date_ccc, 512);
    disable_log_backup_ccc = true;
    write_logfile_ccc(tempname, 0);
    disable_log_backup_ccc = false;
  }

  current_status_ccc = PHASE1;
  current_position_ccc = 0;
  sprintf (tempmessage_ccc, "Resetting the skip data\n");
  message_now_ccc(tempmessage_ccc);
  int i;
  for (i = 0; i < total_lines_ccc-1; i++)
  {
    lstatus_ccc[i] = lstatus_ccc[i] & SKIP_RESET_MASK;
  }
  for (i = 0; i < total_lines_ccc-1; i++)
  {
    // check if the next status is the same and if so merge
    if (lstatus_ccc[i] == lstatus_ccc[i+1])
    {
      lsize_ccc[i] = lsize_ccc[i] + lsize_ccc[i+1];
      // delete the next line since we just merged it into the current line
      delete_line_ccc(i+1);
      // go back one line so modified line gets checked
      if (i > 0)
      {
        i--;
      }
    }
  }
  total_skip_count_ccc = 0;
  total_skip_resets_ccc = 0;
  total_skip_runs_ccc = 0;
  total_slow_skips_ccc = 0;
}





void invoke_soft_reset_ccc (void)
{
  unsigned long long soft_reset_time_bak = soft_reset_time_ccc;
  unsigned long long hard_reset_time_bak = hard_reset_time_ccc;
  unsigned long long busy_wait_time_bak = busy_wait_time_ccc;
  unsigned long long reset_wait_time_bak = reset_wait_time_ccc;
  unsigned long long general_timeout_bak = general_timeout_ccc;
  unsigned long long initial_busy_wait_time_bak = initial_busy_wait_time_ccc;
  unsigned long long power_cycle_time_bak = power_cycle_time_ccc;

  //soft_reset_time_ccc = SOFT_RESET_TIMER;
  //hard_reset_time_ccc = HARD_RESET_TIMER;
  //busy_wait_time_ccc = BUSY_WAIT_TIMER;
  //reset_wait_time_ccc = RESET_WAIT_TIMER;
  //general_timeout_ccc = GENERAL_TIMER;
  //initial_busy_wait_time_ccc = INITIAL_BUSY_WAIT_TIME;
  //power_cycle_time_ccc = RESET_WAIT_TIMER;

  if ( do_soft_reset_ccc(disk1_fd_ccc) )
  {
    sprintf (tempmessage_ccc, "reset failed\n");
    message_console_log_ccc(tempmessage_ccc, 0);
  }
  else
  {
    sprintf (tempmessage_ccc, "reset success\n");
    message_console_log_ccc(tempmessage_ccc, 0);
  }

  soft_reset_time_ccc = soft_reset_time_bak;
  hard_reset_time_ccc = hard_reset_time_bak;
  busy_wait_time_ccc = busy_wait_time_bak;
  reset_wait_time_ccc = reset_wait_time_bak;
  general_timeout_ccc = general_timeout_bak;
  initial_busy_wait_time_ccc = initial_busy_wait_time_bak;
  power_cycle_time_ccc = power_cycle_time_bak;
}






void invoke_hard_reset_ccc (void)
{
  unsigned long long soft_reset_time_bak = soft_reset_time_ccc;
  unsigned long long hard_reset_time_bak = hard_reset_time_ccc;
  unsigned long long busy_wait_time_bak = busy_wait_time_ccc;
  unsigned long long reset_wait_time_bak = reset_wait_time_ccc;
  unsigned long long general_timeout_bak = general_timeout_ccc;
  unsigned long long initial_busy_wait_time_bak = initial_busy_wait_time_ccc;
  unsigned long long power_cycle_time_bak = power_cycle_time_ccc;

  //soft_reset_time_ccc = SOFT_RESET_TIMER;
  //hard_reset_time_ccc = HARD_RESET_TIMER;
  //busy_wait_time_ccc = BUSY_WAIT_TIMER;
  //reset_wait_time_ccc = RESET_WAIT_TIMER;
  //general_timeout_ccc = GENERAL_TIMER;
  //initial_busy_wait_time_ccc = INITIAL_BUSY_WAIT_TIME;
  //power_cycle_time_ccc = RESET_WAIT_TIMER;

  if ( do_hard_reset_ccc(disk1_fd_ccc) )
  {
    sprintf (tempmessage_ccc, "reset failed\n");
    message_console_log_ccc(tempmessage_ccc, 0);
  }
  else
  {
    sprintf (tempmessage_ccc, "reset success\n");
    message_console_log_ccc(tempmessage_ccc, 0);
  }

  soft_reset_time_ccc = soft_reset_time_bak;
  hard_reset_time_ccc = hard_reset_time_bak;
  busy_wait_time_ccc = busy_wait_time_bak;
  reset_wait_time_ccc = reset_wait_time_bak;
  general_timeout_ccc = general_timeout_bak;
  initial_busy_wait_time_ccc = initial_busy_wait_time_bak;
  power_cycle_time_ccc = power_cycle_time_bak;
}





void invoke_hba_reset_ccc (void)
{
  if (do_hba_reset_ccc())
  {
    strcpy (tempmessage_ccc, "hba reset error");
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
  }
  else
  {
    strcpy (tempmessage_ccc, "hba reset complete");
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
    clear_error_message_ccc();
  }
}





void disable_ports_ccc (void)
{
  int res0=system("cp -n /boot/grub/grub.cfg /boot/grub/grub_hddsc_original_bakup.cfg");
  int res1=system("cp -n /etc/default/grub /etc/default/grub_hddsc_original_bakup");
  int res2=system("cp -f /boot/grub/grub.cfg /boot/grub/grub_hddsc_last_bakup.cfg");
  int res3=system("cp -f /etc/default/grub /etc/default/grub_hddsc_last_bakup");
  if(res0 || res1 || res2 || res3)
  {
  }
  if (access("/boot/grub/grub_hddsc_original_bakup.cfg", F_OK ) == -1)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGNOBACKUPFILE]);
    strcat (tempmessage_ccc, "\n/boot/grub/grub_hddsc_original_bakup.cfg");
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    fprintf (stdout, "Error: no backup of /boot/grub/grub.cfg\n");
    return;
  }
  if (access("/etc/default/grub_hddsc_original_bakup", F_OK ) == -1)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGNOBACKUPFILE]);
    strcat (tempmessage_ccc, "\n/etc/default/grub_hddsc_original_bakup");
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    fprintf (stdout, "Error: no backup of /etc/default/grub\n");
    return;
  }

  FILE *readfile = fopen("/etc/default/grub", "r");
  if (readfile == NULL)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGUNABLETOOPEN]);
    strcat (tempmessage_ccc, "\n/etc/default/grub");
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    fprintf (stdout, "Error opening /etc/default/grub (%s)\n", strerror(errno));
    return;
  }
  FILE *writefile = fopen("/etc/default/grub_tmp", "w");
  if (writefile == NULL)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGUNABLETOOPEN]);
    strcat (tempmessage_ccc, "\n/etc/default/grub_tmp");
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    fprintf (stdout, "Error opening /etc/default/grub_tmp (%s)\n", strerror(errno));
    return;
  }

  int maxlinelen = 65536;
  char line[maxlinelen];
  char original_line[maxlinelen];
  char modified_line[maxlinelen];
  memset (original_line, 0, maxlinelen);
  memset (modified_line, 0, maxlinelen);
  int confirm = 0;
  int found_default_line = 0;
  while (fgets(line, sizeof line, readfile))
  {
    int n = 0;
    while (line[n] != '\n' && line[n] != '\0' && n < maxlinelen)
    {
      if (strncmp(line+n, "GRUB_CMDLINE_LINUX_DEFAULT=\"", 28) == 0)
      {
        found_default_line = 1;
        strcpy (original_line, line);
        // start of line processing
        fprintf (stdout, "Found line = %s", line);    //debug
        n += 28;
        // find end of line
        int end_of_data = n;
        while (line[end_of_data] != '"' && line[end_of_data] != '\n' && line[end_of_data] != '\0' && end_of_data < maxlinelen)
        {
          end_of_data++;
        }
        //fprintf (stdout, "end_of_data = %d\n", end_of_data);    //debug
        int found_current = 0;
        char current_ports[maxlinelen];
        strcpy (current_ports, "");
        char newline[maxlinelen];
        memset (newline, 0, maxlinelen);
        strncpy (newline, line, n);
        //fprintf (stdout, "newline = %s\n", newline);    //debug
        while (line[n] != '\0' && n < maxlinelen)
        {
          if (strncmp(line+n, "libata.force=", 13) == 0)
          {
            found_current = 1;
            int i = n;
            while (line[i] != ' ' && line[i] != '"' && line[i] != '\0' && i < maxlinelen)
            {
              i++;
            }
            strncpy (current_ports, line+n, i-n);
            confirm = open_ports_dialog_ccc (current_ports);
            strncat (newline, new_ports_ccc, strlen(new_ports_ccc));
            n = i-1;
          }
          else
          {
            strncat (newline, line + n, 1);
          }
          n++;
        }
        if (!found_current)
        {
          strcpy (newline, "");
          confirm = open_ports_dialog_ccc (current_ports);
          strncat (newline, line, end_of_data);
          strcat (newline, " ");
          strcat (newline, new_ports_ccc);
          strcat (newline, line + end_of_data);
        }
        //fprintf (stdout, "Current ports = %s\n", current_ports);    //debug
        //fprintf (stdout, "New ports = %s\n", new_ports_ccc);    //debug
        fprintf (stdout, "New line = %s", newline);    //debug
        if (confirm)
        {
          strcpy (line, newline);
        }
        //fprintf (stdout, "line = %s\n", line);    //debug
        strcpy (modified_line, line);
        // end of line processing
        break;
      }
      n++;
    }
    fprintf (writefile, "%s", line);
  }

  fclose (readfile);
  fclose (writefile);

  if (!found_default_line)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGNOGRUBDEFAULTLINE]);
    strcat (tempmessage_ccc, "\nGRUB_CMDLINE_LINUX_DEFAULT=");
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return;
  }

  if (confirm)
  {
    if (open_confirmation_dialog_ccc(curlang_ccc[LANGCONFIRMGRUBUPDATE]))
    {
      if (system("cp -f /etc/default/grub_tmp /etc/default/grub"))
      {
        // error copying
        strcpy (tempmessage_ccc, curlang_ccc[LANGCOPYFAILED]);
        strcat (tempmessage_ccc, "\ncp -f /etc/default/grub_tmp /etc/default/grub");
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return;
      }
      else
      {
        if (system("update-grub"))
        {
          // error updating
          strcpy (tempmessage_ccc, curlang_ccc[LANGUPDATEGRUBFAILED]);
          strcat (tempmessage_ccc, "\nupdate-grub");
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
          clear_error_message_ccc();
          return;
        }
      }
      strcpy (tempmessage_ccc, curlang_ccc[LANGOPERATIONSUCCEEDED]);
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
      clear_error_message_ccc();
    }
  }

  return;
}





void restore_ports_ccc (void)
{
  if (access("/boot/grub/grub_hddsc_original_bakup.cfg", F_OK ) == -1)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGNOBACKUPFILE]);
    strcat (tempmessage_ccc, "\n/boot/grub/grub.cfg (/boot/grub/grub_hddsc_original_bakup.cfg)");
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    fprintf (stdout, "Error: no backup of /boot/grub/grub.cfg\n");
  }
  if (access("/etc/default/grub_hddsc_original_bakup", F_OK ) == -1)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGNOBACKUPFILE]);
    strcat (tempmessage_ccc, "\n/etc/default/grub (/etc/default/grub_hddsc_original_bakup)");
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    fprintf (stdout, "Error: no backup of /etc/default/grub\n");
    return;
  }

  if (open_confirmation_dialog_ccc(curlang_ccc[LANGCONFIRMGRUBUPDATE]))
  {
    if (system("cp -f /etc/default/grub_hddsc_original_bakup /etc/default/grub"))
    {
      // error copying
      strcpy (tempmessage_ccc, curlang_ccc[LANGCOPYFAILED]);
      strcat (tempmessage_ccc, "\ncp -f /etc/default/grub_hddsc_original_bakup /etc/default/grub");
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return;
    }
    else
    {
      if (system("update-grub"))
      {
        // error updating
        strcpy (tempmessage_ccc, curlang_ccc[LANGUPDATEGRUBFAILED]);
        strcat (tempmessage_ccc, "\nupdate-grub");
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return;
      }
    }
    strcpy (tempmessage_ccc, curlang_ccc[LANGOPERATIONSUCCEEDED]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
    clear_error_message_ccc();
  }


  return;
}





void disable_usb_mass_storage_ccc(void)
{
  int res0=system("cp -n /lib/modules/$(uname -r)/kernel/drivers/usb/storage/usb-storage.ko /root/usb-storage.ko.original");
  int res1=system("cp -f /lib/modules/$(uname -r)/kernel/drivers/usb/storage/usb-storage.ko /root/usb-storage.ko.backup");
  if(res0 || res1)
  {
    // Perhaps it went wrong?
  }
  if (access("/root/usb-storage.ko.original", F_OK ) == -1)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGNOBACKUPFILE]);
    strcat (tempmessage_ccc, "\n/root/usb-storage.ko.original");
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    fprintf (stdout, "Error: no backup of /root/usb-storage.ko.original\n");
    return;
  }
  if (access("/root/usb-storage.ko.backup", F_OK ) == -1)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGNOBACKUPFILE]);
    strcat (tempmessage_ccc, "\n/root/usb-storage.ko.backup");
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    fprintf (stdout, "Error: no backup of /root/usb-storage.ko.backup\n");
    return;
  }

  if (open_confirmation_dialog_ccc(curlang_ccc[LANGUSBMASSDISABLE]))
  {
    int res2=system("modprobe -r usb-storage");
    if(res2)
    {
      // perhaps it went wrong?
    }
    if (system("mv -fv /lib/modules/$(uname -r)/kernel/drivers/usb/storage/usb-storage.ko /root/usb-storage.ko"))
    {
      // error copying
      strcpy (tempmessage_ccc, curlang_ccc[LANGCOPYFAILED]);
      strcat (tempmessage_ccc, "\nmv -fv /lib/modules/$(uname -r)/kernel/drivers/usb/storage/usb-storage.ko /root/usb-storage.ko");
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return;
    }
    strcpy (tempmessage_ccc, curlang_ccc[LANGOPERATIONSUCCEEDED]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
    clear_error_message_ccc();
  }
}





void restore_usb_mass_storage_ccc(void)
{
  if (access("/root/usb-storage.ko.original", F_OK ) == -1)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGNOBACKUPFILE]);
    strcat (tempmessage_ccc, "\n/root/usb-storage.ko.original");
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    fprintf (stdout, "Warning: no backup of /root/usb-storage.ko.original\n");
  }
  if (access("/root/usb-storage.ko.backup", F_OK ) == -1)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGNOBACKUPFILE]);
    strcat (tempmessage_ccc, "\n/root/usb-storage.ko.backup");
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    fprintf (stdout, "Warning: no backup of /root/usb-storage.ko.backup\n");
  }

  if (open_confirmation_dialog_ccc(curlang_ccc[LANGUSBMASSRESTORE]))
  {
    if (system("mv -fv /root/usb-storage.ko /lib/modules/$(uname -r)/kernel/drivers/usb/storage/usb-storage.ko"))
    {
      // error copying
      strcpy (tempmessage_ccc, curlang_ccc[LANGCOPYFAILED]);
      strcat (tempmessage_ccc, "\nmv -fv /root/usb-storage.ko /lib/modules/$(uname -r)/kernel/drivers/usb/storage/usb-storage.ko");
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return;
    }
    strcpy (tempmessage_ccc, curlang_ccc[LANGOPERATIONSUCCEEDED]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGINFO], 0);
    clear_error_message_ccc();
  }
}





int get_asc_from_additional_sense_ccc(int value)
{
  value = (value >> 8) & 0xff;
  return value;
}





int get_ascq_from_additional_sense_ccc(int value)
{
  value = value & 0xff;
  return value;
}




#if 0
int rebuild_assist_disable_head_ccc (int head)
{
  // remember to disable the rebuild assist to clear it or this likely won't work
  unsigned char data[LOG_PAGE_SIZE];
  memset (data, 0, sizeof(data));

  // process the head number
  int bytenumber = head / 8;
  int byteremainder = head % 8;
  //int checkmask = rebuild_assist_working_mask_ccc[bytenumber] >> byteremainder;
  int checkmask = rebuild_assist_element_mask_ccc[bytenumber] >> byteremainder;
  checkmask = checkmask & 1;
  if (!checkmask)
  {
    fprintf (stdout, "invalid head for rebuild assist\n");
    return -1;
  }
  int byteplacement = 8 + rebuild_assist_element_length_ccc + bytenumber;
  if (byteplacement >= LOG_PAGE_SIZE)
  {
    fprintf (stdout, "internal error, byte placement out of bounds\n");
    return -1;
  }
  int bytedata = 1 << byteremainder;
  data[byteplacement] = bytedata;
  data[0] = 1;
  int ret = write_rebuild_assist_log_ccc (data);
  if (ret)
  {
    fprintf (stdout, "failed to disable head, return=0x%x\n", ret);
    return ret;
  }
  if (ata_status_ccc & 1)
  {
    fprintf (stdout, "error disabling head, status=%02x error=%02x\n", ata_status_ccc, ata_error_ccc);
    return -1;
  }

  if (0)
  {
    fprintf (stdout, "rebuild assist write log data\n");
    int i;
    for (i = 0; i < 32; i+=16)
    {
      fprintf (stdout, "%x: ", i);
      int n;
      for (n=0; n < 16 && i+n < 32; n++)
      {
        fprintf (stdout, "%02x ", data[i+n]);
      }
      fprintf (stdout, "   ");
      for (n=0; n < 16 && i+n < 32; n++)
      {
        fprintf (stdout, "%c", isprint(data[i+n]) ? data[i+n] : '.');
      }
      fprintf (stdout, "\n");
    }
  }

  return 0;
}
#endif




int rebuild_assist_get_next_head_ccc (int head)
{
  if (rebuild_assist_element_length_ccc == 0)
  {
    fprintf (stdout, "internal error, element length can't be zero\n");
    return - 1;
  }
  int start = head;
  head++;
  if ( (head / 8) >= rebuild_assist_element_length_ccc)
  {
    head = 0;
  }
  int count = 0;
  while (head != start)
  {
    int bytenumber = head / 8;
    int byteremainder = head % 8;
    int checkmask = rebuild_assist_working_mask_ccc[bytenumber] >> byteremainder;
    checkmask = checkmask & 1;
    if (checkmask)
    {
      if (head > 7)
      {
        //head = 0;    //TODO this is to limit for testing
      }
      return head;
    }

    head++;
    if ( (head / 8) >= rebuild_assist_element_length_ccc)
    {
      head = 0;
    }

    // prevent endless loop
    count++;
    if (count > rebuild_assist_element_length_ccc * 8)
    {
      break;
    }
  }
  fprintf (stdout, "error, unable to get next head\n");
  return -1;
}





int rebuild_assist_get_number_of_heads_ccc (void)
{
  if (rebuild_assist_element_length_ccc == 0)
  {
    fprintf (stdout, "internal error, element length can't be zero\n");
    return - 1;
  }
  int head = 0;
  int count = 0;
  while (1)
  {
    int bytenumber = head / 8;
    int byteremainder = head % 8;
    int checkmask = rebuild_assist_working_mask_ccc[bytenumber] >> byteremainder;
    checkmask = checkmask & 1;
    if (checkmask)
    {
      count++;
    }

    head++;
    if ( (head / 8) >= rebuild_assist_element_length_ccc)
    {
      break;
    }
  }
  return count;
}





int rebuild_assist_get_highest_head_ccc (void)
{
  if (rebuild_assist_element_length_ccc == 0)
  {
    fprintf (stdout, "internal error, element length can't be zero\n");
    return - 1;
  }
  int head = 0;
  int highest = 0;
  while (1)
  {
    int bytenumber = head / 8;
    int byteremainder = head % 8;
    int checkmask = rebuild_assist_working_mask_ccc[bytenumber] >> byteremainder;
    checkmask = checkmask & 1;
    if (checkmask)
    {
      highest = head;
    }

    head++;
    if ( (head / 8) >= rebuild_assist_element_length_ccc)
    {
      break;
    }
  }
  return highest;
}





int rebuild_assist_check_head_ccc (int head)
{
  int bytenumber = head / 8;
  int byteremainder = head % 8;
  int checkmask = rebuild_assist_working_mask_ccc[bytenumber] >> byteremainder;
  checkmask = checkmask & 1;
  return checkmask;
}





int rebuild_assist_remove_head_ccc (int head)
{
  int bytenumber = head / 8;
  int byteremainder = head % 8;
  int fixmask = 1;
  fixmask = fixmask << byteremainder;
  int checkmask = rebuild_assist_working_mask_ccc[bytenumber] >> byteremainder;
  checkmask = checkmask & 1;
  if (checkmask)
  {
    rebuild_assist_working_mask_ccc[bytenumber] = rebuild_assist_working_mask_ccc[bytenumber] ^ fixmask;
  }
  else
  {
    fprintf (stdout, "head %d already removed\n", head);
    return -1;
  }
  return 0;
}




#if 0
int enable_rebuild_assist_ccc(void)
{
  unsigned char data[LOG_PAGE_SIZE];
  memset (data, 0, sizeof(data));
  data[0] = 1;
  int ret = write_rebuild_assist_log_ccc (data);
  if (ret)
  {
    fprintf (stdout, "failed to enable rebuild assist, return=0x%x\n", ret);
    return ret;
  }
  if (ata_status_ccc & 1)
  {
    fprintf (stdout, "error enabling rebuild assist, status=%02x error=%02x\n", ata_status_ccc, ata_error_ccc);
    return -1;
  }
  ret = read_rebuild_assist_log_ccc();
  if (ret || ata_status_ccc & 1)
  {
    fprintf (stdout, "failed to read rebuild assist log, return=%d status=0x%02x error=0x%02x\n", ret, ata_status_ccc, ata_error_ccc);
  }
  use_fpdma_ccc = true;
  fprintf (stdout, "rebuild assist enabled\n");
  return 0;
}
#endif



#if 0
int disable_rebuild_assist_ccc(void)
{
  unsigned char data[LOG_PAGE_SIZE];
  memset (data, 0, sizeof(data));
  int ret = write_rebuild_assist_log_ccc (data);
  if (ret || (ata_status_ccc & 1))
  {
    do_soft_reset_ccc(0);
  }
  ret = write_rebuild_assist_log_ccc (data);
  if (ret)
  {
    fprintf (stdout, "failed to disable rebuild assist, return=%d\n", ret);
    return ret;
  }
  if (ata_status_ccc & 1)
  {
    fprintf (stdout, "error disabling rebuild assist, status=%02x error=%02x\n", ata_status_ccc, ata_error_ccc);
    return -1;
  }
  read_rebuild_assist_log_ccc();
  use_fpdma_ccc = false;
  fprintf (stdout, "rebuild assist disabled\n");
  return 0;
}
#endif




int initialize_head_map_memory_ccc(void)
{
  free (head_map_position_ccc);
  head_map_position_ccc=malloc(sizeof(*head_map_position_ccc) * head_map_rows_ccc);
  if (head_map_position_ccc == NULL)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGALLOCATEMEMERR]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, " (%s)", strerror(errno));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return GENERAL_ERROR_RETURN_CODE;
  }

  free (head_map_size_ccc);
  head_map_size_ccc=malloc(sizeof(*head_map_size_ccc) * head_map_rows_ccc);
  if (head_map_size_ccc == NULL)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGALLOCATEMEMERR]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, " (%s)", strerror(errno));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return GENERAL_ERROR_RETURN_CODE;
  }

  free (head_map_final_ccc);
  head_map_final_ccc=malloc(sizeof(*head_map_final_ccc) * head_map_rows_ccc);
  if (head_map_final_ccc == NULL)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGALLOCATEMEMERR]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, " (%s)", strerror(errno));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return GENERAL_ERROR_RETURN_CODE;
  }

  free (head_map_head_ccc);
  head_map_head_ccc=malloc(sizeof(*head_map_head_ccc) * head_map_rows_ccc);
  if (head_map_head_ccc == NULL)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGALLOCATEMEMERR]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, " (%s)", strerror(errno));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return GENERAL_ERROR_RETURN_CODE;
  }

  free (head_map_status_ccc);
  head_map_status_ccc=malloc(sizeof(*head_map_status_ccc) * head_map_rows_ccc);
  if (head_map_status_ccc == NULL)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGALLOCATEMEMERR]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, " (%s)", strerror(errno));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return GENERAL_ERROR_RETURN_CODE;
  }

  return 0;
}





int increase_head_map_memory_ccc(int new_lines)
{
  head_map_rows_ccc += new_lines;

  temp_head_map_position_ccc = realloc(head_map_position_ccc, head_map_rows_ccc * sizeof(*head_map_position_ccc));
  if (temp_head_map_position_ccc == NULL)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGALLOCATEMEMERR]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, " (%s)", strerror(errno));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return GENERAL_ERROR_RETURN_CODE;
  }
  head_map_position_ccc = temp_head_map_position_ccc;

  temp_head_map_size_ccc = realloc(head_map_size_ccc, head_map_rows_ccc * sizeof(*head_map_size_ccc));
  if (temp_head_map_size_ccc == NULL)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGALLOCATEMEMERR]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, " (%s)", strerror(errno));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return GENERAL_ERROR_RETURN_CODE;
  }
  head_map_size_ccc = temp_head_map_size_ccc;

  temp_head_map_final_ccc = realloc(head_map_final_ccc, head_map_rows_ccc * sizeof(*head_map_final_ccc));
  if (temp_head_map_final_ccc == NULL)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGALLOCATEMEMERR]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, " (%s)", strerror(errno));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return GENERAL_ERROR_RETURN_CODE;
  }
  head_map_final_ccc = temp_head_map_final_ccc;

  temp_head_map_head_ccc = realloc(head_map_head_ccc, head_map_rows_ccc * sizeof(*head_map_head_ccc));
  if (temp_head_map_head_ccc== NULL)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGALLOCATEMEMERR]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, " (%s)", strerror(errno));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return GENERAL_ERROR_RETURN_CODE;
  }
  head_map_head_ccc = temp_head_map_head_ccc;

  temp_head_map_status_ccc = realloc(head_map_status_ccc, head_map_rows_ccc * sizeof(*head_map_status_ccc));
  if (temp_head_map_status_ccc == NULL)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGALLOCATEMEMERR]);
    message_error_ccc(tempmessage_ccc);
    sprintf (tempmessage_ccc, " (%s)", strerror(errno));
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return GENERAL_ERROR_RETURN_CODE;
  }
  head_map_status_ccc = temp_head_map_status_ccc;

  return 0;
}




#if 0
int set_rebuild_assist_enabled_ccc (void)
{
  if (!ahci_mode_ccc)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGREBUILDASSISTAHCIONLY]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return -1;
  }
  if (!ncq_supported_ccc || !rebuild_assist_supported_ccc)
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGREBUILDASSISTNOTSUPPORTED]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return -1;
  }

  // first disable rebuild assist to make sure it is cleared
  if (disable_rebuild_assist_ccc())
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGREBUILDASSISTERRORDISABLING]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return -1;
  }
  rebuild_assist_enabled_ccc = false;

  // now enable rebuild assist
  if (enable_rebuild_assist_ccc())
  {
    strcpy (tempmessage_ccc, curlang_ccc[LANGREBUILDASSISTERRORENABLING]);
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    disable_rebuild_assist_ccc();
    return -1;
  }
  rebuild_assist_enabled_ccc = true;

  // only for testing
  if (rebuild_assist_test_ccc >= 0)
  {
  //disable_rebuild_assist_ccc();
  rebuild_assist_disable_head_ccc(rebuild_assist_test_ccc);
  }

  read_rebuild_assist_log_ccc();
  if (1)
  {
    int i;
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
    int head = 0;
    while ((head / 8) < rebuild_assist_element_length_ccc)
    {
      int bytenumber = head / 8;
      int byteremainder = head % 8;
      int checkmask = rebuild_assist_log_data_ccc.mask[bytenumber] >> byteremainder;
      checkmask = checkmask & 1;
      int disabled = rebuild_assist_log_data_ccc.disabled[bytenumber] >> byteremainder;
      disabled  = disabled  & 1;
      if (checkmask && disabled)
      {
        fprintf (stdout, "head %d is disabled\n", head);
      }
      else if (checkmask)
      {
        fprintf (stdout, "head %d is enabled\n", head);
      }
      head++;
    }
  }

  return 0;
}
#endif




int set_rebuild_assist_disabled_ccc (void)
{
  if (ncq_supported_ccc && rebuild_assist_supported_ccc)
  {
    stop_signal_ccc = false;
    disable_rebuild_assist_ccc();
  }
  rebuild_assist_enabled_ccc = false;

  read_rebuild_assist_log_ccc();
  if (1)
  {
    int i;
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

  return 0;
}





int rebuild_assist_map_heads_ccc (void)
{
  memcpy (rebuild_assist_working_mask_ccc, rebuild_assist_element_mask_ccc, REBUILD_ASSIST_FIELD_LENGTH);
  if (!ncq_supported_ccc || !rebuild_assist_supported_ccc)
  {
    strcpy (tempmessage_ccc, "rebuild assist is not supported on this device");
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return -1;
  }
  if (!ahci_mode_ccc)
  {
    strcpy (tempmessage_ccc, "rebuild assist only works in direct ahci mode");
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return -1;
  }
  if (initialize_head_map_memory_ccc())
  {
    return -1;
  }
  head_map_line_ccc = 0;

  // first disable rebuild assist to make sure it is cleared
  if (disable_rebuild_assist_ccc())
  {
    strcpy (tempmessage_ccc, "error disabling rebuild assist\n");
    message_error_ccc(tempmessage_ccc);
    strcpy (tempmessage_ccc, "error mapping heads");
    message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return -1;
  }

  FILE *headmap_debug_file = fopen("headmap_debug.log", "w");
  fprintf (headmap_debug_file, "%s %s headmap debug file created at ", title_ccc, version_number_ccc);
  time_t mytime;
  mytime = time(NULL);
  fprintf (headmap_debug_file, "%s", ctime(&mytime));

  int brute_force = 0;

  // first find the pattern
  int current_head = 0;
  if (!rebuild_assist_check_head_ccc(current_head))
  {
    current_head = rebuild_assist_get_next_head_ccc(current_head);
    if (current_head < 0)
    {
      strcpy (tempmessage_ccc, "error getting next head\n");
      message_error_ccc(tempmessage_ccc);
      strcpy (tempmessage_ccc, "error mapping heads");
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return -1;
    }
  }
  int number_of_heads = rebuild_assist_get_number_of_heads_ccc();
  if (number_of_heads < 1)
  {
    strcpy (tempmessage_ccc, "error getting number of heads\n");
    message_error_ccc(tempmessage_ccc);
    strcpy (tempmessage_ccc, "error mapping heads");
      message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return -1;
  }
  int highest_head = rebuild_assist_get_highest_head_ccc();
  if (highest_head < 1)
  {
    strcpy (tempmessage_ccc, "error getting highest head\n");
    message_error_ccc(tempmessage_ccc);
    strcpy (tempmessage_ccc, "error mapping heads");
      message_error_ccc(tempmessage_ccc);
    print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
    clear_error_message_ccc();
    return -1;
  }
  unsigned char pattern_head[REBUILD_ASSIST_FIELD_LENGTH * HEAD_PATTERN_CHECK_COUNT];
  long long pattern_position[REBUILD_ASSIST_FIELD_LENGTH * HEAD_PATTERN_CHECK_COUNT];
  long long pattern_final[REBUILD_ASSIST_FIELD_LENGTH * HEAD_PATTERN_CHECK_COUNT];
  long long pattern_size[REBUILD_ASSIST_FIELD_LENGTH * HEAD_PATTERN_CHECK_COUNT];
  long long head_last_final[REBUILD_ASSIST_FIELD_LENGTH];
  int head_finished[REBUILD_ASSIST_FIELD_LENGTH];
  memset (head_last_final, 0, sizeof(head_last_final));
  memset (head_finished, 0, sizeof(head_finished));
  int pattern_count = 0;
  int heads_checked = 0;
  long long current_position = 0;
  long long end_position = source_total_size_ccc - 1;
  //current_position = 3907018584ULL;    // testing
  int failcount = 0;
  int retries = 0;
  int failed = 0;
  //while (pattern_count < number_of_heads * HEAD_PATTERN_CHECK_COUNT)
  //while (current_position < end_position)
  while (1)
  {
    if (brute_force)
    {
      if (current_position >= end_position)
      {
        break;
      }
    }
    else
    {
      if (pattern_count >= number_of_heads * HEAD_PATTERN_CHECK_COUNT)
      {
        break;
      }
    }
    failed = 0;
    if (disable_rebuild_assist_ccc())
    {
      strcpy (tempmessage_ccc, "error disabling rebuild assist\n");
      message_error_ccc(tempmessage_ccc);
      strcpy (tempmessage_ccc, "error mapping heads");
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      return -1;
    }
    if (rebuild_assist_disable_head_ccc(current_head))
    {
      strcpy (tempmessage_ccc, "error disabling head\n");
      message_error_ccc(tempmessage_ccc);
      strcpy (tempmessage_ccc, "error mapping heads");
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      disable_rebuild_assist_ccc();
      return -1;
    }
    read_rebuild_assist_log_ccc();
    use_fpdma_ccc = true;
    int retstat = read_chunk_ccc(current_position, 1);
    use_fpdma_ccc = false;
    if (retstat)
    {
      fprintf (stdout, "read chunk failure, return=0x%x status=%02x error=%02x\n", retstat, ata_status_ccc, ata_error_ccc);
      strcpy (tempmessage_ccc, "error mapping heads");
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      disable_rebuild_assist_ccc();
      return -1;
    }
    if ( !(ata_status_ccc & 1) )
    {
      fprintf (stdout, "read not aborted, status=%02x error=%02x\n", ata_status_ccc, ata_error_ccc);
      fprintf (headmap_debug_file, "read not aborted, status=%02x error=%02x\n", ata_status_ccc, ata_error_ccc);
      failed = 1;
    }
    else
    {
      int ret = read_ncq_error_log_ccc();
      if (ret || (ata_status_ccc & 1))
      {
        fprintf (stdout, "failed to read ncq log, return=0x%x status=%02x error=%02x\n", ret, ata_status_ccc, ata_error_ccc);
        strcpy (tempmessage_ccc, "error mapping heads");
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        disable_rebuild_assist_ccc();
        return -1;
      }
      if ( ncq_error_log_data_ccc.sense_key == SKEY_ABORTED_COMMAND && ncq_error_log_data_ccc.sense_asc == get_asc_from_additional_sense_ccc(ASC_MULTIPLE_READ_ERRORS) && ncq_error_log_data_ccc.sense_ascq == get_ascq_from_additional_sense_ccc(ASC_MULTIPLE_READ_ERRORS) )
      {
        long long size = (ncq_error_log_data_ccc.final_lba - current_position) + 1;
        fprintf (stdout, "head=%02x position=%08llx size=%06llx final=%08llx\n", current_head, current_position, size, ncq_error_log_data_ccc.final_lba);
        fprintf (headmap_debug_file, "head=%02x position=%08llx size=%06llx final=%08llx\n", current_head, current_position, size, ncq_error_log_data_ccc.final_lba);
        if (!heads_checked)
        {
          pattern_head[pattern_count] = current_head;
          pattern_position[pattern_count] = current_position;
          pattern_final[pattern_count] = ncq_error_log_data_ccc.final_lba;
          pattern_size[pattern_count] = (ncq_error_log_data_ccc.final_lba - current_position) + 1;
          pattern_count++;
        }
        if (brute_force)
        {
          head_map_head_ccc[head_map_line_ccc] = current_head;
          head_map_position_ccc[head_map_line_ccc] = current_position;
          head_map_final_ccc[head_map_line_ccc] = ncq_error_log_data_ccc.final_lba;
          head_map_size_ccc[head_map_line_ccc] = (ncq_error_log_data_ccc.final_lba - current_position) + 1;
          head_map_status_ccc[head_map_line_ccc] = 0xff;
          head_map_line_ccc++;
          if (head_map_line_ccc+1 >= head_map_rows_ccc)
          {
            increase_head_map_memory_ccc(1000);
          }
        }
        failcount = 0;
        retries = 0;
      }
      else
      {
        fprintf (stdout, "read not aborted key=%02x asc=%02x ascq=%02x\n", ncq_error_log_data_ccc.sense_key, ncq_error_log_data_ccc.sense_asc, ncq_error_log_data_ccc.sense_ascq);
        fprintf (headmap_debug_file, "read not aborted key=%02x asc=%02x ascq=%02x\n", ncq_error_log_data_ccc.sense_key, ncq_error_log_data_ccc.sense_asc, ncq_error_log_data_ccc.sense_ascq);
        failed = 1;
      }
    }
    if (failed)
    {
      failcount++;
      if (retries == 0)
      {
        retries = 1;
        fprintf (stdout, "head %d failed position=%08llx, trying soft reset\n", current_head, current_position);
        fprintf (headmap_debug_file, "head %d failed position=%08llx, trying soft reset\n", current_head, current_position);
        failcount--;
        do_soft_reset_ccc(0);
        continue;
      }
      else if (failcount > number_of_heads)
      {
        fprintf (stdout, "unable to find pattern\n");
        strcpy (tempmessage_ccc, "error mapping heads");
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        disable_rebuild_assist_ccc();
        return -1;
      }
      else
      {
        retries = 0;
        fprintf (stdout, "head %d failed position=%08llx, trying head ", current_head, current_position);
        fprintf (headmap_debug_file, "head %d failed position=%08llx, trying head ", current_head, current_position);
        current_head = rebuild_assist_get_next_head_ccc(current_head);
        fprintf (stdout, "%d\n", current_head);
        fprintf (headmap_debug_file, "%d\n", current_head);
        continue;
      }
    }

    current_position = ncq_error_log_data_ccc.final_lba + 1;
    current_head = rebuild_assist_get_next_head_ccc(current_head);
    if (current_head < 0)
    {
      strcpy (tempmessage_ccc, "error mapping heads");
      message_error_ccc(tempmessage_ccc);
      print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
      clear_error_message_ccc();
      disable_rebuild_assist_ccc();
      return -1;
    }

    if (brute_force && !heads_checked && pattern_count > number_of_heads * HEAD_PATTERN_CHECK_COUNT)
    {
      heads_checked = 1;
      unsigned char working_heads[REBUILD_ASSIST_FIELD_LENGTH];
      memset (working_heads, 0, sizeof(working_heads));
      int heads_in_pattern = 0;
      int i;
      for (i = 0; i < pattern_count; i++)
      {
        fprintf (stdout, "pattern head=%02x position=%08llx size=%06llx final=%08llx\n", pattern_head[i], pattern_position[i], pattern_size[i], pattern_final[i]);
        fprintf (headmap_debug_file, "pattern head=%02x position=%08llx size=%06llx final=%08llx\n", pattern_head[i], pattern_position[i], pattern_size[i], pattern_final[i]);
        working_heads[pattern_head[i]] = 1;
        // get number of heads in pattern
        if (i > 0)
        {
          if (!heads_in_pattern && pattern_head[i] == pattern_head[0])
          {
            heads_in_pattern = i;
          }
        }
      }
      fprintf (stdout, "working heads = %d\n", heads_in_pattern);
      fprintf (headmap_debug_file, "working heads = %d\n", heads_in_pattern);

      // check that the pattern repeats
      int pattern_fail = 0;
      int n;
      for (n = 1; n < HEAD_PATTERN_CHECK_COUNT; n++)
      {
        for (i = 0; i < heads_in_pattern; i++)
        {
          if (pattern_head[i] != pattern_head[(heads_in_pattern * n) + i])
          {
            pattern_fail = 1;
          }
        }
      }
      if (pattern_fail)
      {
        fprintf (stdout, "pattern failed to repeat\n");
        fprintf (headmap_debug_file, "pattern failed to repeat\n");
      }
      else
      {
        fprintf (stdout, "pattern is good\n");
        fprintf (headmap_debug_file, "pattern is good\n");
      }

      // check if any heads need to be removed
      if (heads_in_pattern < number_of_heads)
      {
        fprintf (stdout, "head counts don't match\n");
        fprintf (headmap_debug_file, "head counts don't match\n");
        for (i = 0; i <= highest_head; i++)
        {
          if (working_heads[i] == 0)
          {
            fprintf (stdout, "removing head %d\n", i);
            fprintf (headmap_debug_file, "removing head %d\n", i);
            rebuild_assist_remove_head_ccc(i);
          }
        }
      }
    }

    if (stop_signal_ccc)
    {
      disable_rebuild_assist_ccc();
      return STOP_SIGNAL_RETURN_CODE;
    }
  }


  if (!brute_force)
  {
    unsigned char working_heads[REBUILD_ASSIST_FIELD_LENGTH];
    memset (working_heads, 0, sizeof(working_heads));
    int heads_in_pattern = 0;
    int i;
    for (i = 0; i < pattern_count; i++)
    {
      fprintf (stdout, "pattern head=%02x position=%08llx size=%06llx final=%08llx\n", pattern_head[i], pattern_position[i], pattern_size[i], pattern_final[i]);
      fprintf (headmap_debug_file, "pattern head=%02x position=%08llx size=%06llx final=%08llx\n", pattern_head[i], pattern_position[i], pattern_size[i], pattern_final[i]);
      working_heads[pattern_head[i]] = 1;
      // get number of heads in pattern
      if (i > 0)
      {
        if (!heads_in_pattern && pattern_head[i] == pattern_head[0])
        {
          heads_in_pattern = i;
        }
      }
    }
    fprintf (stdout, "working heads = %d\n", heads_in_pattern);
    fprintf (headmap_debug_file, "working heads = %d\n", heads_in_pattern);

    // check that the pattern repeats
    int pattern_fail = 0;
    int n;
    for (n = 1; n < HEAD_PATTERN_CHECK_COUNT; n++)
    {
      for (i = 0; i < heads_in_pattern; i++)
      {
        if (pattern_head[i] != pattern_head[(heads_in_pattern * n) + i])
        {
          pattern_fail = 1;
        }
      }
    }
    if (pattern_fail)
    {
      fprintf (stdout, "pattern failed to repeat\n");
      fprintf (headmap_debug_file, "pattern failed to repeat\n");
    }
    else
    {
      fprintf (stdout, "pattern is good\n");
      fprintf (headmap_debug_file, "pattern is good\n");
    }

    // check if any heads need to be removed
    if (heads_in_pattern < number_of_heads)
    {
      fprintf (stdout, "head counts don't match\n");
      fprintf (headmap_debug_file, "head counts don't match\n");
      for (i = 0; i <= highest_head; i++)
      {
        if (working_heads[i] == 0)
        {
          fprintf (stdout, "removing head %d\n", i);
          fprintf (headmap_debug_file, "removing head %d\n", i);
          rebuild_assist_remove_head_ccc(i);
        }
      }
    }

    // get average size
    long long starting_average_size = 0;
    for (n = 0; n < HEAD_PATTERN_CHECK_COUNT; n++)
    {
      for (i = 0; i < heads_in_pattern; i++)
      {
        starting_average_size += pattern_size[(heads_in_pattern * n) + i];
      }
    }
    starting_average_size = starting_average_size / HEAD_PATTERN_CHECK_COUNT;
    fprintf (stdout, "pattern size=%06llx\n", starting_average_size);
    fprintf (headmap_debug_file, "pattern size=%06llx\n", starting_average_size);
    fprintf (stdout, "average per head = %lld\n", (starting_average_size * 512) / heads_in_pattern);
    fprintf (headmap_debug_file, "average per head = %lld\n", (starting_average_size * 512) / heads_in_pattern);




    // populate head map with known good data
    for (n = 0; n < HEAD_PATTERN_CHECK_COUNT; n++)
    {
      for (i = 0; i < heads_in_pattern; i++)
      {
        head_map_head_ccc[head_map_line_ccc] = pattern_head[(heads_in_pattern * n) + i];
        head_map_position_ccc[head_map_line_ccc] = pattern_position[(heads_in_pattern * n) + i];
        head_map_final_ccc[head_map_line_ccc] = pattern_final[(heads_in_pattern * n) + i];
        head_map_size_ccc[head_map_line_ccc] = pattern_size[(heads_in_pattern * n) + i];
        head_map_status_ccc[head_map_line_ccc] = 0xff;
        head_map_line_ccc++;
      }
    }

    // now start getting the final lba data
    for (i = 0; i < heads_in_pattern; i++)
    {
      long long running_average_size = starting_average_size;
      long long adjusted_average_size;
      long long size_adjustment;
      long long average_size[HEAD_AVERAGE_COUNT];
      int n;
      for (n = 0; n < HEAD_AVERAGE_COUNT; n++)
      {
        average_size[n] = starting_average_size;
      }
      int average_size_counter = 0;
      size_adjustment = ((running_average_size / heads_in_pattern) / 100) * 66;
      adjusted_average_size = running_average_size - size_adjustment;
      current_head = pattern_head[(heads_in_pattern * (HEAD_PATTERN_CHECK_COUNT - 1)) + i];
      current_position = pattern_final[(heads_in_pattern * (HEAD_PATTERN_CHECK_COUNT - 1)) + i] + adjusted_average_size;
      long long last_final = pattern_final[(heads_in_pattern * (HEAD_PATTERN_CHECK_COUNT - 1)) + i];
      retries = -1;
      int too_far = 0;
      if (disable_rebuild_assist_ccc())
      {
        strcpy (tempmessage_ccc, "error disabling rebuild assist\n");
        message_error_ccc(tempmessage_ccc);
        strcpy (tempmessage_ccc, "error mapping heads");
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        return -1;
      }
      do_soft_reset_ccc(0);
      if (rebuild_assist_disable_head_ccc(current_head))
      {
        strcpy (tempmessage_ccc, "error disabling head\n");
        message_error_ccc(tempmessage_ccc);
        strcpy (tempmessage_ccc, "error mapping heads");
        message_error_ccc(tempmessage_ccc);
        print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
        clear_error_message_ccc();
        disable_rebuild_assist_ccc();
        return -1;
      }
      while (current_position < end_position)
      {
        failed = 0;
        read_rebuild_assist_log_ccc();
        use_fpdma_ccc = true;
        int retstat = read_chunk_ccc(current_position, 1);
        use_fpdma_ccc = false;
        if (retstat)
        {
          fprintf (stdout, "read chunk failure, return=0x%x status=%02x error=%02x\n", retstat, ata_status_ccc, ata_error_ccc);
          strcpy (tempmessage_ccc, "error mapping heads");
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
          clear_error_message_ccc();
          disable_rebuild_assist_ccc();
          return -1;
        }
        if ( !(ata_status_ccc & 1) )
        {
          fprintf (stdout, "read not aborted, status=%02x error=%02x\n", ata_status_ccc, ata_error_ccc);
          fprintf (headmap_debug_file, "read not aborted, status=%02x error=%02x\n", ata_status_ccc, ata_error_ccc);
          failed = 1;
        }
        else
        {
          int ret = read_ncq_error_log_ccc();
          if (ret || (ata_status_ccc & 1))
          {
            fprintf (stdout, "failed to read ncq log, return=0x%x status=%02x error=%02x\n", ret, ata_status_ccc, ata_error_ccc);
            strcpy (tempmessage_ccc, "error mapping heads");
            message_error_ccc(tempmessage_ccc);
            print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
            clear_error_message_ccc();
            disable_rebuild_assist_ccc();
            return -1;
          }
          if ( ncq_error_log_data_ccc.sense_key == SKEY_ABORTED_COMMAND && ncq_error_log_data_ccc.sense_asc == get_asc_from_additional_sense_ccc(ASC_MULTIPLE_READ_ERRORS) && ncq_error_log_data_ccc.sense_ascq == get_ascq_from_additional_sense_ccc(ASC_MULTIPLE_READ_ERRORS) )
          {
            long long size = (ncq_error_log_data_ccc.final_lba - current_position) + 1;
            fprintf (stdout, "head=%02x position=%08llx final=%08llx size=%06llx run=%06llx adj=%06llx\n", current_head, current_position, ncq_error_log_data_ccc.final_lba, size, running_average_size, adjusted_average_size);
            fprintf (headmap_debug_file, "head=%02x position=%08llx final=%08llx size=%06llx run=%06llx adj=%06llx\n", current_head, current_position, ncq_error_log_data_ccc.final_lba, size, running_average_size, adjusted_average_size);
            pattern_size[pattern_count] = (ncq_error_log_data_ccc.final_lba - current_position) + 1;
            head_map_head_ccc[head_map_line_ccc] = current_head;
            //head_map_position_ccc[head_map_line_ccc] = current_position;
            head_map_final_ccc[head_map_line_ccc] = ncq_error_log_data_ccc.final_lba;
            //head_map_size_ccc[head_map_line_ccc] = (ncq_error_log_data_ccc.final_lba - current_position) + 1;
            if (retries == -1)
            {
              head_map_status_ccc[head_map_line_ccc] = 1;
            }
            else
            {
              head_map_status_ccc[head_map_line_ccc] = -1;
            }
            head_map_line_ccc++;
            if (head_map_line_ccc+1 >= head_map_rows_ccc)
            {
              increase_head_map_memory_ccc(1000);
            }
            failcount = 0;
            retries = -1;
          }
          else
          {
            fprintf (stdout, "read not aborted key=%02x asc=%02x ascq=%02x\n", ncq_error_log_data_ccc.sense_key, ncq_error_log_data_ccc.sense_asc, ncq_error_log_data_ccc.sense_ascq);
            fprintf (headmap_debug_file, "read not aborted key=%02x asc=%02x ascq=%02x\n", ncq_error_log_data_ccc.sense_key, ncq_error_log_data_ccc.sense_asc, ncq_error_log_data_ccc.sense_ascq);
            failed = 1;
          }
        }
        if (failed && !too_far)
        {
          int multiplier;
          if (retries < 0)
          {
            multiplier = retries * -1;
          }
          else
          {
            multiplier = retries;
          }
          if ( (retries < 0) && (last_final + adjusted_average_size - ((size_adjustment / 3) * multiplier)) <= last_final)
          {
            retries = 1;
            multiplier = retries;
          }
          if (retries < 0)
          {
            fprintf (stdout, "head %d failed position=%08llx, trying retarded position\n", current_head, current_position);
            fprintf (headmap_debug_file, "head %d failed position=%08llx, trying retarded position\n", current_head, current_position);
            current_position = last_final + adjusted_average_size - ((size_adjustment / 3) * multiplier);
            retries--;
          }
          else
          {
            fprintf (stdout, "head %d failed position=%08llx, trying advanced position\n", current_head, current_position);
            fprintf (headmap_debug_file, "head %d failed position=%08llx, trying advanced position\n", current_head, current_position);
            current_position = last_final + adjusted_average_size + ((size_adjustment / 3) * multiplier);
            retries++;
          }
          if ( (retries > 0) && ((size_adjustment / 3) * multiplier) > (running_average_size * 3))
          {
            too_far = 1;
          }
          continue;
        }
        else if (failed)
        {
          fprintf (stdout, "head %d failed position=%08llx\n", current_head, current_position);
          strcpy (tempmessage_ccc, "error mapping heads");
          message_error_ccc(tempmessage_ccc);
          print_gui_error_message_ccc(error_message_ccc, curlang_ccc[LANGERROR], 1);
          clear_error_message_ccc();
          disable_rebuild_assist_ccc();
          return -1;
        }

        // only update the average if it is not too far out of sync
        if (retries >= -6 && retries <= 6)
        {
          average_size[average_size_counter] = ncq_error_log_data_ccc.final_lba - last_final;
          average_size_counter++;
          if (average_size_counter >= HEAD_AVERAGE_COUNT)
          {
            average_size_counter = 0;
          }
        }
        long long new_average = 0;
        int n;
        for (n = 0; n < HEAD_AVERAGE_COUNT; n++)
        {
          new_average += average_size[n];
        }
        running_average_size = new_average / HEAD_AVERAGE_COUNT;
        size_adjustment = ((running_average_size / heads_in_pattern) / 100) * 66;
        adjusted_average_size = running_average_size - size_adjustment;
        //fprintf (stdout, "adjusted average = %06llx\n", adjusted_average_size;
        current_position = ncq_error_log_data_ccc.final_lba + adjusted_average_size;
        last_final = ncq_error_log_data_ccc.final_lba;


        if (stop_signal_ccc)
        {
          disable_rebuild_assist_ccc();
          return STOP_SIGNAL_RETURN_CODE;
        }
      }
    }
  }


  fprintf (stdout, "sorting the data\n");
  fprintf (headmap_debug_file, "sorting the data\n");
  // sort the headmap
  int x;
  for (x = 0; x < head_map_line_ccc; x++)
  {
    int lowest = -1;
    long long lowest_value = head_map_final_ccc[x];
    int z;
    for (z = x + 1; z < head_map_line_ccc; z++)
    {
      if (head_map_final_ccc[z] < lowest_value)
      {
        lowest = z;
        lowest_value = head_map_final_ccc[z];
      }
    }
    if (lowest > 0)
    {
      long long temp_head_map_position = head_map_position_ccc[x];
      long long temp_head_map_size = head_map_size_ccc[x];
      long long temp_head_map_final = head_map_final_ccc[x];
      int temp_head_map_head = head_map_head_ccc[x];
      int temp_head_map_status = head_map_status_ccc[x];
      head_map_position_ccc[x] = head_map_position_ccc[lowest];
      head_map_size_ccc[x] = head_map_size_ccc[lowest];
      head_map_final_ccc[x] = head_map_final_ccc[lowest];
      head_map_head_ccc[x] = head_map_head_ccc[lowest];
      head_map_status_ccc[x] = head_map_status_ccc[lowest];
      head_map_position_ccc[lowest] = temp_head_map_position;
      head_map_size_ccc[lowest] = temp_head_map_size;
      head_map_final_ccc[lowest] = temp_head_map_final;
      head_map_head_ccc[lowest] = temp_head_map_head;
      head_map_status_ccc[lowest] = temp_head_map_status;
    }
  }


  // populate the position and size values
  for (x = 0; x < head_map_line_ccc; x++)
  {
    if (head_map_status_ccc[x] == 0xff)
    {
      continue;
    }
    if (x > 1)
    {
      head_map_position_ccc[x] = head_map_final_ccc[x-1] + 1;
      head_map_size_ccc[x] = (head_map_final_ccc[x] - head_map_position_ccc[x]) + 1;
    }
  }


  fprintf (stdout, "writing the data\n");
  fprintf (headmap_debug_file, "writing the data\n");
  // write the headmap file
  char *file_name = malloc (1024);
  memset (file_name, 0, 1024);
  strncpy (file_name, log_file_ccc, 1012);
  strcat (file_name, ".headmap");
  FILE *writefile = fopen(file_name, "w");
  if (writefile == NULL)
  {
    fprintf (stdout, "Cannot open %s for writing (%s).\n", file_name, strerror(errno));
    return -1;
  }
  fprintf (writefile, "# Headmap file create by %s %s\n", title_ccc, version_number_ccc);
  //time_t mytime;
  mytime = time(NULL);
  fprintf (writefile, "# %s", ctime(&mytime));
  fprintf (writefile, "# head \tstart     \tend     \tsize     \tstatus\n");
  int line_number;
  for (line_number = 0; line_number < head_map_line_ccc; line_number++)
  {
    fprintf (writefile, "0x%02x \t0x%06llx \t0x%06llx \t0x%06llx \t0x%02x\n", head_map_head_ccc[line_number], head_map_position_ccc[line_number], head_map_final_ccc[line_number], head_map_size_ccc[line_number], head_map_status_ccc[line_number]);
  }
  fclose (writefile);

  fprintf (stdout, "head mapping done\n");
  fprintf (headmap_debug_file, "head mapping done\n");

  disable_rebuild_assist_ccc();
  fprintf (headmap_debug_file, "%s %s headmap debug file ended at ", title_ccc, version_number_ccc);
  mytime = time(NULL);
  fprintf (headmap_debug_file, "%s", ctime(&mytime));
  fclose (headmap_debug_file);
  return 0;
}
