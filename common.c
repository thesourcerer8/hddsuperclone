// Copyright (C) 2015-2022 Scott Dwyer.
// You may use/distribute/modify this freely, under the terms of
// the GNU General Public License version 2 or later version.
// This software is distributed WITHOUT ANY WARRANTY.


#include "common.h"
#include "clone_gui_language.h"
#include "io.h"

time_t start_time_ccc, end_time_ccc;
struct timeval tvBegin_ccc, tvEnd_ccc, tvDiff_ccc, tvCurrent_ccc;
bool check_only_ccc;
bool do_indentation_ccc;
int ccc_indent_ccc;
int spaces_ccc;
char *argument_ccc[MAX_ARGUMENTS];
unsigned int argument_count_ccc;

bool ncq_supported_ccc = false;    // word 76 bit 8
bool rebuild_assist_supported_ccc = false;    // word 78 bit 11
bool rebuild_assist_enabled_ccc;
int rebuild_assist_test_ccc = -1;

int process_id_ccc;
int driver_return_zeros_on_error_ccc;
int driver_io_scsi_only_ccc;
bool quiet_ccc;
bool stop_signal_ccc;
bool superclone_ccc;
bool critical_process_ccc;
bool pending_exit_ccc;
bool logfile_changed_ccc;
bool domainfile_changed_ccc;
bool force_danger_ccc;
bool force_stupid_ccc;
bool repair_log_ccc;
bool fill_mode_ccc;
bool driver_only_ccc;
bool cleanup_mode_ccc;
unsigned long long verbose_ccc;
unsigned long long debug_ccc;
bool debugversion_ccc;
bool verbosedebug_ccc;
int return_value_ccc;
bool print_hex_ccc;
int if_statement_level_ccc;
int while_statement_level_ccc;
long long error_level_ccc;
unsigned char exitcode_ccc;
FILE *debug_file_ccc;
char *disk_1_ccc;
char *disk_2_ccc;
char *script_file_ccc;
char current_script_ccc[MAX_LINE_LENGTH];
int print_error_message_ccc;
char exit_message_ccc[EXIT_MESSAGE_SIZE];
char error_message_ccc[ERROR_MESSAGE_SIZE];
char display_message_ccc[DISPLAY_MESSAGE_SIZE];
char tempmessage_ccc[TEMP_MESSAGE_SIZE];
char debug_message_ccc[DEBUG_MESSAGE_SIZE];
char log_display_message_ccc[DISPLAY_MESSAGE_SIZE];

void* ccc_buffer_ccc;
unsigned long long ccc_main_buffer_size_ccc;
void* ccc_scratchpad_ccc;
unsigned long long ccc_main_scratchpad_size_ccc;
void* sensebuffer_ccc;
unsigned long long sensebuffer_size_ccc;
void* ccc_usbbuffer_ccc;
char usbcbwbuffer_ccc[USBCBW_BUFFER_SIZE];
char usbcswbuffer_ccc[USBCSW_BUFFER_SIZE];
unsigned long long ccc_main_usbbuffer_size_ccc;
void* padding_buffer_ccc;
unsigned char identify_buffer_ccc[IDENTIFY_BUFFER_SIZE];
unsigned char ata_identify_buffer_ccc[IDENTIFY_BUFFER_SIZE];
unsigned char read_capacity_buffer_ccc[512];
int append_script_ccc;
long long *number_variable_buffer_ccc;
char **string_variable_pointer_ccc;
char current_date_ccc[40];
char current_time_ccc[40];
unsigned char random_data_ccc[2048];

unsigned char sbyte_ccc[16];

unsigned char superbyte_ccc[1024];
int license_type_ccc;
int license_version_ccc;
int license_time_ccc;
int activation_type_ccc;
int activation_days_remaining_ccc;
int check_command_ccc;
int check_read_ccc;
int check_write_ccc;

int disk1_fd_ccc;
int disk2_fd_ccc;
int current_disk_ccc;

int main_driver_fd_ccc;
bool command_line_ccc;
int command_length_ccc;
unsigned char sense_buf_ccc[255];
unsigned char sense_key_ccc;
unsigned char asc_ccc;
unsigned char ascq_ccc;
unsigned char ata_error_ccc;
unsigned char ata_status_ccc;
unsigned char ata_device_ccc;
unsigned char ata_count1_ccc;
unsigned char ata_count2_ccc;
unsigned char ata_lba1_ccc;
unsigned char ata_lba2_ccc;
unsigned char ata_lba3_ccc;
unsigned char ata_lba4_ccc;
unsigned char ata_lba5_ccc;
unsigned char ata_lba6_ccc;
long long ata_lba_ccc;
int ata_count_ccc;
int io_status_ccc;
int io_masked_status_ccc;
int msg_status_ccc;
int io_sb_len_wr_ccc;
int io_host_status_ccc;
int io_driver_status_ccc;
int io_resid_ccc;
int io_duration_ccc;
int io_info_ccc;
int ata_return_valid_ccc;
int ata_check_ccc;
bool pio_mode_ccc;
int driver_installed_ccc;
long long hpa_lba_ccc;
long long dco_lba_ccc;
bool extended_support_ccc;
bool enable_process_chunk_ccc;
bool enable_read_twice_ccc;
bool enable_scsi_write_ccc=false;
bool charater_device_driver_mode_ccc;

unsigned long reg_base_address_ccc;
unsigned long control_base_address_ccc;
unsigned long bus_base_address_ccc;
bool device_select_base_ccc;
unsigned long long hba_base_address_ccc;
unsigned long long hba_reset_address_ccc;
unsigned long long port_base_address_ccc;
unsigned long long port_reset_address_ccc;
int port_number_base_ccc;
char device_driver_base_ccc[64];
char device_bus_base_ccc[64];
char current_source_model_ccc[64];
char current_source_serial_ccc[64];
char current_destination_model_ccc[64];
char current_destination_serial_ccc[64];
int current_device_type_ccc;
bool copy_image_mode_ccc;
bool generic_mode_ccc;
bool direct_mode_ccc;
bool ahci_mode_ccc;
bool ata_passthrough_ccc;
bool scsi_passthrough_ccc;
bool nvme_passthrough_ccc;
bool auto_passthrough_ccc;
bool usb_mode_ccc;
bool usb_allow_ata_ccc;
bool supertool_mode_chosen_ccc;
unsigned int pagesize_ccc;
unsigned long long table_virtual_address_ccc;
unsigned long long command_list_virtual_address_ccc;
unsigned long long fis_virtual_address_ccc;
unsigned long long buffer_virtual_address_ccc;
unsigned long long table_physical_address_ccc;
unsigned long long command_list_physical_address_ccc;
unsigned long long fis_physical_address_ccc;
unsigned long long buffer_physical_address_ccc[8192];
int command_list_size_ccc;
int fis_size_ccc;
void* table_buffer_ccc;
void* command_list_buffer_ccc;
void* fis_buffer_ccc;
int hba_mem_dev_ccc;
uint32_t hba_alloc_mem_size_ccc;
void *hba_mem_pointer_ccc;
int port_mem_dev_ccc;
uint32_t port_alloc_mem_size_ccc;
void *port_mem_pointer_ccc;
void *port_virt_addr_ccc;
unsigned long long max_dma_size_ccc;
unsigned long long real_buffer_size_ccc;
unsigned long long first_read_time_ccc;
unsigned long long soft_reset_time_ccc;
unsigned long long hard_reset_time_ccc;
unsigned long long busy_wait_time_ccc;
unsigned long long reset_wait_time_ccc;
unsigned long long initial_busy_wait_time_ccc;
unsigned long long general_timeout_ccc;
int usb_timeout_ccc;
int cbw_timeout_ccc;
int csw_timeout_ccc;
unsigned long long power_cycle_time_ccc;
bool phase_timers_ccc;
unsigned long long orig_soft_reset_time_ccc;
unsigned long long p12_soft_reset_time_ccc;
unsigned long long p3_soft_reset_time_ccc;
unsigned long long p4_soft_reset_time_ccc;
unsigned long long td_soft_reset_time_ccc;
unsigned long long d2_soft_reset_time_ccc;
unsigned long long sc_soft_reset_time_ccc;
unsigned long long rt_soft_reset_time_ccc;
char *drive_serial_ccc;
int new_source_ccc;
int new_destination_ccc;
long long source_total_size_ccc;
long long bytes_per_sector_ccc;
long long target_total_size_ccc;
long long generic_source_total_size_ccc;
unsigned char *driver_control_address_ccc;
unsigned char *driver_error_bitmap_address_ccc;
unsigned char *driver_transfer_buffer_address_ccc;
unsigned char *driver_main_data_buffer_address_ccc;
unsigned char *driver_table_buffer_ccc;
unsigned char *driver_command_list_buffer_ccc;
unsigned char *driver_fis_buffer_ccc;
int driver_memory_mapped_ccc;
bool always_wait_for_reset_timers_ccc;
bool dont_wait_for_drdy_ccc;

bool table_address_changed_ccc;
unsigned char table_address_backup_ccc[4];
bool ahci_address_changed_ccc;
uint64_t command_list_address_backup_ccc;
uint64_t fis_address_backup_ccc;
bool ahci_interrupt_changed_ccc;
uint32_t interrupt_backup_ccc;
bool memory_mapped_ccc;
bool memory_failed_ccc;
bool io_permissions_ccc;
int table_size_ccc;
int table_entry_count_ccc;
FILE *pagemap_ccc;


char device_driver_ccc[MAX_DEVICES][64];
char device_bus_ccc[MAX_DEVICES][64];
unsigned long long reg_address_ccc[MAX_DEVICES];
unsigned long long control_address_ccc[MAX_DEVICES];
unsigned long long bus_address_ccc[MAX_DEVICES];
unsigned long long hba_address_ccc[MAX_DEVICES];
int port_number_ccc[MAX_DEVICES];
unsigned int port_status_ccc[MAX_DEVICES];
unsigned long long port_address_ccc[MAX_DEVICES];
unsigned int port_signature_ccc[MAX_DEVICES];
bool device_select_ccc[MAX_DEVICES];
bool device_present_ccc[MAX_DEVICES];
bool device_visable_ccc[MAX_DEVICES];
char device_reference_ccc[MAX_DEVICES][32];
char device_name_ccc[MAX_DEVICES][32];
char model_ccc[MAX_DEVICES][41];
char serial_ccc[MAX_DEVICES][21];
char drive_list_ccc[MAX_DEVICES][64];
long long drive_size_ccc[MAX_DEVICES];
int drive_bytes_per_sector_ccc[MAX_DEVICES];
int device_type_ccc[MAX_DEVICES];
int device_count_ccc;
int choose_device_mode_ccc;
int did_reset_ccc;
bool enable_hba_reset_on_connect_source_ccc;
bool enable_hba_test_on_connect_source_ccc;
bool enable_dont_identify_on_choose_source_ccc;
bool enable_data_dump_ccc;
char data_dump_filename_ccc[1024];
bool enable_dump_identify_on_check_ccc;

int usbr1_vendor_id_ccc;
int usbr1_product_id_ccc;
char usbr1_vendor_string_ccc[256];
char usbr1_product_string_ccc[256];
char usbr1_extra_id_string_ccc[256];
int usbr1_bus_number_ccc;
int usbr1_bus_real_number_ccc;
int usbr1_device_number_ccc;

int usbd1_vendor_id_ccc;
int usbd1_product_id_ccc;
char usbd1_vendor_string_ccc[256];
char usbd1_product_string_ccc[256];
char usbd1_extra_id_string_ccc[256];
int usbd1_bus_number_ccc;
int usbd1_bus_real_number_ccc;
int usbd1_device_number_ccc;
int usbd1_bulk_in_endpoint_ccc;
int usbd1_bulk_out_endpoint_ccc;

int usb_device_count_ccc;
int usb_vendor_id_ccc[MAX_USB_DEVICES];
int usb_product_id_ccc[MAX_USB_DEVICES];
char usb_vendor_string_ccc[MAX_USB_DEVICES][256];
char usb_product_string_ccc[MAX_USB_DEVICES][256];
char usb_serial_string_ccc[MAX_USB_DEVICES][256];
char usb_extra_id_string_ccc[MAX_USB_DEVICES][256];
int usb_bus_number_ccc[MAX_USB_DEVICES];
int usb_bus_real_number_ccc[MAX_USB_DEVICES];
int usb_device_number_ccc[MAX_USB_DEVICES];
int usb_known_relay_ccc[MAX_USB_DEVICES];
int usb_mass_storage_ccc[MAX_USB_DEVICES];
int usb_bulk_in_endpoint_ccc[MAX_USB_DEVICES];
int usb_bulk_out_endpoint_ccc[MAX_USB_DEVICES];
struct usb_dev_handle *usbr1_dev_handle_ccc;
struct usb_dev_handle *usbd1_dev_handle_ccc;
int new_usb_ccc;
unsigned char usb_transfer_direction_ccc;
unsigned char usb_lun_ccc;
int usb_lun_set_ccc;
uint32_t usb_tag_ccc;
uint32_t usb_transfer_length_ccc;
unsigned char usb_csw_status_ccc;
uint32_t usb_csw_residue_ccc;
int usb_read_residue_ccc;
int usb_csw_valid_ccc;
int usb_bulk_reset_enabled_ccc;
int usb_soft_reset_enabled_ccc;
int usb_hard_reset_enabled_ccc;
int usb_port_reset_enabled_ccc;

bool use_rebuild_assist_ccc;
bool rebuild_assist_data_valid_ccc;
unsigned char rebuild_assist_element_length_ccc;
char rebuild_assist_element_mask_ccc[REBUILD_ASSIST_FIELD_LENGTH];
char rebuild_assist_working_mask_ccc[REBUILD_ASSIST_FIELD_LENGTH];

passthrough_ccc_type passthrough_ccc;
driver_control_data_ccc_type driver_control_data_ccc;
ncq_error_log_data_ccc_type ncq_error_log_data_ccc;
rebuild_assist_log_data_ccc_type rebuild_assist_log_data_ccc;


unsigned int total_script_lines_ccc;
char **script_line_pointer_ccc;


int message_exit_ccc(char *message)
{
  if (debug_ccc > 0)
  {
    fprintf (debug_file_ccc, "%s", message);
  }
  strncat(exit_message_ccc, message, EXIT_MESSAGE_SIZE - strlen(exit_message_ccc) - 1);
  return 0;
}




int message_error_ccc(char *message)
{
  if (debug_ccc > 0)
  {
    fprintf (debug_file_ccc, "%s", message);
  }
  strncat(error_message_ccc, message, ERROR_MESSAGE_SIZE - strlen(error_message_ccc) - 1);
  return 0;
}




int clear_error_message_ccc(void)
{
  strcpy(error_message_ccc, "\0");
  return 0;
}




int message_display_ccc(char *message)
{
  if (debug_ccc & DEBUG18)
  {
    fprintf (debug_file_ccc, "%s", message);
  }
  strncat(display_message_ccc, message, DISPLAY_MESSAGE_SIZE - strlen(display_message_ccc) - 1);
  if (strlen(message) + strlen(display_message_ccc) < DISPLAY_MESSAGE_SIZE -1)
  {
    //strcat(display_message_ccc, message);
  }
  return 0;
}




int message_now_ccc(char *message)
{
  fprintf (stdout, "%s", message);
  if (debug_ccc)
  {
    get_the_time_ccc();
    fprintf (debug_file_ccc, "%s %s", current_time_ccc, message);
  }
  return 0;
}



int message_console_log_ccc(char *message, unsigned long long level)
{
  get_the_time_ccc();
  if (level == 0 || (level & verbose_ccc))
  {
    fprintf (stdout, "%s %s", current_time_ccc, message);
    if (debug_ccc)
    {
      fprintf (debug_file_ccc, "%s %s", current_time_ccc, message);
    }
    if (enable_data_dump_ccc || enable_dump_identify_on_check_ccc)
    {
      char temp_string[1024];
      sprintf (temp_string, "%s %s", current_time_ccc, message);
      dump_info_to_filename_ccc (data_dump_filename_ccc, temp_string);
    }
  }
  return 0;
}




int message_debug_ccc(char *message, unsigned long long level)
{
  get_the_time_ccc();
  if (debug_ccc & level)
  {
    fprintf (debug_file_ccc, "%s %s", current_time_ccc, message);
  }
  if (verbose_ccc & level)
  {
    fprintf (stdout, "%s %s", current_time_ccc, message);
  }
  return 0;
}




// function to set main buffer
int set_main_buffer_ccc(void)
{
  if (usb_mode_ccc)
  {
    if (ccc_main_buffer_size_ccc > MAX_USB_BUFFER_SIZE || ccc_main_buffer_size_ccc > real_buffer_size_ccc)
    {
      sprintf (tempmessage_ccc, "ERROR: Maximum buffer size exceeded.\n");
      message_now_ccc(tempmessage_ccc);
      return (-1);
    }
  }
  else if (ccc_main_buffer_size_ccc > MAX_BUFFER_SIZE || ccc_main_buffer_size_ccc > real_buffer_size_ccc)
  {
    sprintf (tempmessage_ccc, "ERROR: Maximum buffer size exceeded.\n");
    message_now_ccc(tempmessage_ccc);
    return (-1);
  }
  if (superbyte_ccc[29] == 0x60)
  {
    if (ahci_mode_ccc && ccc_main_buffer_size_ccc > max_dma_size_ccc)
    {
      sprintf (tempmessage_ccc, "ERROR: Maximum AHCI buffer size (%lld) exceeded.\n", max_dma_size_ccc);
      message_now_ccc(tempmessage_ccc);
      return (-1);
    }
    if (direct_mode_ccc)
    {
      create_dma_table_ccc();
    }
  }
  return (0);
}





int create_dma_table_ccc(void)
{
  // ahci mode
  if (ahci_mode_ccc)
  {
    int max_entries = max_dma_size_ccc / pagesize_ccc;
    memset (table_buffer_ccc, 0, table_size_ccc);
    unsigned int write_size;
    int i;
    int page_count = (ccc_main_buffer_size_ccc / pagesize_ccc);
    if (ccc_main_buffer_size_ccc % pagesize_ccc > 0)
    {
      page_count++;
    }
    //fprintf (stdout, "page_count2=%d\n", page_count);
    table_entry_count_ccc = page_count;
    for (i = 0; i < page_count; i++)
    {
      if (i >= max_entries)
      {
        table_entry_count_ccc = max_entries;
        break;
      }
      int n = (i*16) + superbyte_ccc[15];    //potential superbyte
      uint32_t dword = buffer_physical_address_ccc[i];
      memcpy(table_buffer_ccc+n, &dword, 4);
      memset(table_buffer_ccc+n+4, 0, 8);

      // if last page get remainder of buffer size
      if (i == page_count - 1)
      {
        if (ccc_main_buffer_size_ccc % pagesize_ccc == 0)
        {
          write_size = pagesize_ccc;
        }
        else
        {
          write_size = ccc_main_buffer_size_ccc % pagesize_ccc;
        }
      }
      else
      {
        write_size = pagesize_ccc;
      }
      //fprintf (stdout, "write_size=%d\n", write_size);
      dword = (write_size - 1);
      memcpy(table_buffer_ccc+12+n, &dword, 4);
      #ifdef DEBUG
      if (debug_ccc & DEBUG33)
      {
        int i;
        for (i = 0; i < 16; i++)
        {
          unsigned char *c;
          c = (unsigned char *)table_buffer_ccc+i+n;
          fprintf (stdout, "tb%02x ", *c);
        }
        fprintf (stdout, "\n");
      }
      #endif
    }
  }


  //direct mode
  else
  {
    int max_entries = max_dma_size_ccc / pagesize_ccc;
    memset (table_buffer_ccc, 0, table_size_ccc);
    unsigned int write_size;
    unsigned char c;
    int i;
    int page_count = (ccc_main_buffer_size_ccc / pagesize_ccc);
    if (ccc_main_buffer_size_ccc % pagesize_ccc > 0)
    {
      page_count++;
    }
    //fprintf (stdout, "page_count2=%d\n", page_count);
    for (i = 0; i < page_count; i++)
    {
      if (i >= max_entries)
      {
        break;
      }
      c = buffer_physical_address_ccc[i];
      memcpy(table_buffer_ccc+(i*8), &c, 1);
      c = buffer_physical_address_ccc[i] >> 8;
      memcpy(table_buffer_ccc+1+(i*8), &c, 1);
      c = buffer_physical_address_ccc[i] >> 16;
      memcpy(table_buffer_ccc+2+(i*8), &c, 1);
      c = buffer_physical_address_ccc[i] >> 24;
      memcpy(table_buffer_ccc+3+(i*8), &c, 1);

      // if last page get remainder of buffer size
      if (i == page_count - 1)
      {
        if (ccc_main_buffer_size_ccc % pagesize_ccc == 0)
        {
          write_size = pagesize_ccc;
        }
        else
        {
          write_size = ccc_main_buffer_size_ccc % pagesize_ccc;
        }
      }
      else
      {
        write_size = pagesize_ccc;
      }
      //fprintf (stdout, "write_size=%d\n", write_size);
      c = write_size;
      memcpy(table_buffer_ccc+4+(i*8), &c, 1);
      c = write_size >> 8;
      memcpy(table_buffer_ccc+5+(i*8), &c, 1);

      c = 0;
      memcpy(table_buffer_ccc+6+(i*8), &c, 1);
      // if last page then mark it
      if (i == page_count - 1)
      {
        c = superbyte_ccc[15];    //potential superbyte
      }
      else
      {
        c = superbyte_ccc[11];    //potential superbyte
      }
      memcpy(table_buffer_ccc+7+(i*8), &c, 1);
      #ifdef DEBUG
      if (debug_ccc & DEBUG33)
      {
        int i;
        for (i = 0; i < 8; i++)
        {
          unsigned char *c;
          c = (unsigned char *)table_buffer_ccc+i;
          fprintf (stdout, "tb%02x ", *c);
        }
        fprintf (stdout, "\n");
      }
      #endif
    }
  }
  return (0);
}





// function to initialize table buffer
int set_table_buffer_ccc(void)
{
  int multiplier = 1;
  if (driver_installed_ccc && driver_memory_mapped_ccc)
  {
    multiplier = 4;
  }
  table_size_ccc = pagesize_ccc * multiplier;
  long long tries = 0;
  long long maxtries = 0x10000000 / table_size_ccc;
  //fprintf (stdout, "maxtries=%lld\n", maxtries);   //debug
  while (tries < maxtries)
  {
    if (!driver_memory_mapped_ccc)
    {
      free (table_buffer_ccc);
      table_buffer_ccc = NULL;
    }
    if (tries != 0)
    {
      padding_buffer_ccc = realloc(padding_buffer_ccc, table_size_ccc*tries);
    }
    if (driver_memory_mapped_ccc)
    {
      table_buffer_ccc = driver_table_buffer_ccc;
    }
    else
    {
      table_buffer_ccc = valloc(table_size_ccc);
      if(!table_buffer_ccc) {
        sprintf (tempmessage_ccc, "posix_memalign failed (%s)", strerror(errno));
        message_now_ccc(tempmessage_ccc);
        return (-1);
      }
    }
    memset (table_buffer_ccc, 0, table_size_ccc);
    #if UINTPTR_MAX == 0xffffffff
    table_virtual_address_ccc = (unsigned int)table_buffer_ccc;
    #elif UINTPTR_MAX == 0xffffffffffffffff
    table_virtual_address_ccc = (unsigned long long)table_buffer_ccc;
    #else
    table_virtual_address_ccc = (unsigned int)table_buffer_ccc;
    #endif
    //fprintf (stdout, "table address=0x%llx\n", table_virtual_address_ccc);   //debug

    return_value_ccc = get_table_physical_memory_location_ccc();
    if (return_value_ccc == 0)
    {
      break;
    }
    if (driver_memory_mapped_ccc)
    {
      return -1;
    }

    tries++;
  }
  //fprintf (stdout, "tries=%lld\n", tries);   //debug
  return (return_value_ccc);
}





int get_table_physical_memory_location_ccc(void)
{
  int out_of_range = 0;
  // get table buffer location
  int multiplier = 1;
  if (driver_installed_ccc && driver_memory_mapped_ccc)
  {
    multiplier = 4;
  }
  table_size_ccc = pagesize_ccc * multiplier;
  unsigned long long file_offset = table_virtual_address_ccc / pagesize_ccc * 8;

  int proc_id = getpid();
  //char command[255];
  //sprintf (command, "cat /proc/%d/maps", proc_id);
  //fprintf (stdout, "%s\n", command);
  //system (command);

  char path[255];
  sprintf (path, "/proc/%d/pagemap", proc_id);
  pagemap_ccc = fopen(path, "rb");
  if(!pagemap_ccc)
  {
    sprintf(tempmessage_ccc, "Error! Cannot open %s\n", path);
    message_now_ccc(tempmessage_ccc);
    return -1;
  }

  //printf("Vaddr: 0x%llx, pagesize_ccc: %d\n", table_virtual_address_ccc, pagesize_ccc);
  //printf("Reading %s at 0x%llx\n", path, file_offset);
  int status = fseek(pagemap_ccc, file_offset, SEEK_SET);
  if(status){
    sprintf (tempmessage_ccc, "Failed to do fseek! (%s)", strerror(errno));
    message_now_ccc(tempmessage_ccc);
    return -1;
  }

  int count = (table_size_ccc / pagesize_ccc);
  if (table_size_ccc % pagesize_ccc > 0)
  {
    count++;
  }
  unsigned int read_size = 8 * count;
  //fprintf (stdout, "count=%d\n", count);
  //exit (0);
  unsigned char file_data[read_size];
  if (fread(file_data, 1, read_size, pagemap_ccc) == read_size )
  {
    while (read_size > 0)
    {
      int i;
      for (i = read_size-1; i >= (int)read_size - 8; i--)
      {
        //fprintf (stdout, "%02x", file_data[i]);
        table_physical_address_ccc = (table_physical_address_ccc << 8) + file_data[i];
      }
      //fprintf (stdout, "\n");
      table_physical_address_ccc <<= 12;
#ifdef DEBUG
      if (debug_ccc & DEBUG34)
      {
        fprintf (stdout, "table_physical_address=0x%llx\n", table_physical_address_ccc);
      }
#endif
      if (table_physical_address_ccc > 0xffffffff)
      {
#ifdef DEBUG
        if (debug_ccc & DEBUG34)
        {
          fprintf (stdout, "table_physical_address=0x%llx is out of range\n", table_physical_address_ccc);
        }
#endif
        out_of_range++;
      }
      read_size -= 8;
    }
  }
  fclose(pagemap_ccc);
  return (out_of_range);
}






// function to initialize command list buffer
int set_command_list_buffer_ccc(void)
{
  command_list_size_ccc = pagesize_ccc;
  long long tries = 0;
  long long maxtries = 0x10000000 / command_list_size_ccc;
  //fprintf (stdout, "maxtries=%lld\n", maxtries);
  while (tries < maxtries)
  {
    if (!driver_memory_mapped_ccc)
    {
      free (command_list_buffer_ccc);
    }
    unsigned int align = pagesize_ccc;
    if (tries != 0)
    {
      padding_buffer_ccc = realloc(padding_buffer_ccc, command_list_size_ccc*tries);
      //memset (padding_buffer_ccc, 0, command_list_size_ccc*tries);
    }
    if (driver_memory_mapped_ccc)
    {
      command_list_buffer_ccc = driver_command_list_buffer_ccc;
    }
    else if (posix_memalign(&command_list_buffer_ccc, align, command_list_size_ccc))
    {
      sprintf (tempmessage_ccc, "posix_memalign failed (%s)", strerror(errno));
      message_now_ccc(tempmessage_ccc);
      return (-1);
    }
    memset (command_list_buffer_ccc, 0, command_list_size_ccc);
    #if UINTPTR_MAX == 0xffffffff
    command_list_virtual_address_ccc = (unsigned int)command_list_buffer_ccc;
    #elif UINTPTR_MAX == 0xffffffffffffffff
    command_list_virtual_address_ccc = (unsigned long long)command_list_buffer_ccc;
    #else
    command_list_virtual_address_ccc = (unsigned int)command_list_buffer_ccc;
    #endif
    //fprintf (stdout, "command list address=0x%llx\n", command_list_virtual_address_ccc);

    return_value_ccc = get_command_list_physical_memory_location_ccc();
    if (return_value_ccc == 0)
    {
      break;
    }
    if (driver_memory_mapped_ccc)
    {
      return (-1);
    }

    tries++;
  }
  //fprintf (stdout, "tries=%lld\n", tries);
  return (return_value_ccc);
}






int get_command_list_physical_memory_location_ccc(void)
{
  int out_of_range = 0;
  // get command list buffer location
  command_list_size_ccc = pagesize_ccc;
  unsigned long long file_offset = command_list_virtual_address_ccc / pagesize_ccc * 8;

  int proc_id = getpid();
  //char command[255];
  //sprintf (command, "cat /proc/%d/maps", proc_id);
  //fprintf (stdout, "%s\n", command);
  //system (command);

  char path[255];
  sprintf (path, "/proc/%d/pagemap", proc_id);
  pagemap_ccc = fopen(path, "rb");
  if(!pagemap_ccc)
  {
    sprintf(tempmessage_ccc, "Error! Cannot open %s\n", path);
    message_now_ccc(tempmessage_ccc);
    return -1;
  }

  //printf("Vaddr: 0x%llx, pagesize_ccc: %d\n", command_list_virtual_address_ccc, pagesize_ccc);
  //printf("Reading %s at 0x%llx\n", path, file_offset);
  int status = fseek(pagemap_ccc, file_offset, SEEK_SET);
  if(status){
    sprintf (tempmessage_ccc, "Failed to do fseek! (%s)", strerror(errno));
    message_now_ccc(tempmessage_ccc);
    return -1;
  }

  int count = (command_list_size_ccc / pagesize_ccc);
  if (command_list_size_ccc % pagesize_ccc > 0)
  {
    count++;
  }
  unsigned int read_size = 8 * count;
  //fprintf (stdout, "count=%d\n", count);
  //exit (0);
  unsigned char file_data[read_size];
  if (fread(file_data, 1, read_size, pagemap_ccc) == read_size )
  {
    while (read_size > 0)
    {
      int i;
      for (i = read_size-1; i >= (int)read_size - 8; i--)
      {
        //fprintf (stdout, "%02x", file_data[i]);
        command_list_physical_address_ccc = (command_list_physical_address_ccc << 8) + file_data[i];
      }
      //fprintf (stdout, "\n");
      command_list_physical_address_ccc <<= 12;
#ifdef DEBUG
      if (debug_ccc & DEBUG34)
      {
        fprintf (stdout, "command_list_physical_address=0x%llx\n", command_list_physical_address_ccc);
      }
#endif
      if (command_list_physical_address_ccc > 0xffffffff)
      {
#ifdef DEBUG
        if (debug_ccc & DEBUG34)
        {
          fprintf (stdout, "command_list_physical_address=0x%llx is out of range\n", command_list_physical_address_ccc);
        }
#endif
        out_of_range++;
      }
      read_size -= 8;
    }
  }
  fclose(pagemap_ccc);
  return (out_of_range);
}





// function to initialize fis buffer
int set_fis_buffer_ccc(void)
{
  int fis_size_ccc = pagesize_ccc;
  long long tries = 0;
  long long maxtries = 0x10000000 / fis_size_ccc;
  //fprintf (stdout, "maxtries=%lld\n", maxtries);
  while (tries < maxtries)
  {
    if (!driver_memory_mapped_ccc)
    {
      free (fis_buffer_ccc);
    }
    unsigned int align = pagesize_ccc;
    if (tries != 0)
    {
      padding_buffer_ccc = realloc(padding_buffer_ccc, fis_size_ccc*tries);
      //memset (padding_buffer_ccc, 0, command_list_size_ccc*tries);
    }
    if (driver_memory_mapped_ccc)
    {
      fis_buffer_ccc = driver_fis_buffer_ccc;
    }
    else if (posix_memalign(&fis_buffer_ccc, align, fis_size_ccc))
    {
      sprintf (tempmessage_ccc, "posix_memalign failed (%s)", strerror(errno));
      message_now_ccc(tempmessage_ccc);
      return (-1);
    }
    memset (fis_buffer_ccc, 0, fis_size_ccc);
    #if UINTPTR_MAX == 0xffffffff
    fis_virtual_address_ccc = (unsigned int)fis_buffer_ccc;
    #elif UINTPTR_MAX == 0xffffffffffffffff
    fis_virtual_address_ccc = (unsigned long long)fis_buffer_ccc;
    #else
    fis_virtual_address_ccc = (unsigned int)fis_buffer_ccc;
    #endif
    //fprintf (stdout, "fis address=0x%llx\n", fis_virtual_address_ccc);

    return_value_ccc = get_fis_physical_memory_location_ccc();
    if (return_value_ccc == 0)
    {
      break;
    }
    if (driver_memory_mapped_ccc)
    {
      return -1;
    }

    tries++;
  }
  //fprintf (stdout, "tries=%lld\n", tries);
  return (return_value_ccc);
}





int get_fis_physical_memory_location_ccc(void)
{
  int out_of_range = 0;
  // get FIS buffer location
  fis_size_ccc = pagesize_ccc;
  unsigned long long file_offset = fis_virtual_address_ccc / pagesize_ccc * 8;

  int proc_id = getpid();
  //char command[255];
  //sprintf (command, "cat /proc/%d/maps", proc_id);
  //fprintf (stdout, "%s\n", command);
  //system (command);

  char path[255];
  sprintf (path, "/proc/%d/pagemap", proc_id);
  pagemap_ccc = fopen(path, "rb");
  if(!pagemap_ccc)
  {
    sprintf(tempmessage_ccc, "Error! Cannot open %s\n", path);
    message_now_ccc(tempmessage_ccc);
    return -1;
  }

  //printf("Vaddr: 0x%llx, pagesize_ccc: %d\n", fis_virtual_address_ccc, pagesize_ccc);
  //printf("Reading %s at 0x%llx\n", path, file_offset);
  int status = fseek(pagemap_ccc, file_offset, SEEK_SET);
  if(status){
    sprintf (tempmessage_ccc, "Failed to do fseek! (%s)", strerror(errno));
    message_now_ccc(tempmessage_ccc);
    return -1;
  }

  int count = (fis_size_ccc / pagesize_ccc);
  if (fis_size_ccc % pagesize_ccc > 0)
  {
    count++;
  }
  unsigned int read_size = 8 * count;
  //fprintf (stdout, "count=%d\n", count);
  //exit (0);
  unsigned char file_data[read_size];
  if (fread(file_data, 1, read_size, pagemap_ccc) == read_size )
  {
    while (read_size > 0)
    {
      int i;
      for (i = read_size-1; i >= (int)read_size - 8; i--)
      {
        //fprintf (stdout, "%02x", file_data[i]);
        fis_physical_address_ccc = (fis_physical_address_ccc << 8) + file_data[i];
      }
      //fprintf (stdout, "\n");
      fis_physical_address_ccc <<= 12;
#ifdef DEBUG
      if (debug_ccc & DEBUG34)
      {
        fprintf (stdout, "fis_physical_address=0x%llx\n", fis_physical_address_ccc);
      }
#endif
      if (fis_physical_address_ccc > 0xffffffff)
      {
#ifdef DEBUG
        if (debug_ccc & DEBUG34)
        {
          fprintf (stdout, "fis_physical_address=0x%llx is out of range\n", fis_physical_address_ccc);
        }
#endif
        out_of_range++;
      }
      read_size -= 8;
    }
  }
  fclose(pagemap_ccc);
  return (out_of_range);
}





int get_buffer_physical_memory_locations_ccc(void)
{
  int out_of_range = 0;
  long long tries = 0;
  long long maxtries = 0x100000000 / max_dma_size_ccc;
  //fprintf (stdout, "maxtries=%lld\n", maxtries);    //debug
  while (tries < maxtries)
  {
    unsigned int align = pagesize_ccc;
    if (!driver_memory_mapped_ccc)
    {
      free (ccc_buffer_ccc);
    }
    if (tries != 0)
    {
    padding_buffer_ccc = realloc(padding_buffer_ccc, real_buffer_size_ccc * tries);
    }

    if (driver_memory_mapped_ccc)
    {
      ccc_buffer_ccc = driver_main_data_buffer_address_ccc;
    }
    else if (posix_memalign(&ccc_buffer_ccc, align, real_buffer_size_ccc))
    {
      sprintf (tempmessage_ccc, "posix_memalign failed (%s)", strerror(errno));
      message_now_ccc(tempmessage_ccc);
      return (-1);
    }
    memset (ccc_buffer_ccc, 0, real_buffer_size_ccc);

    out_of_range = 0;
    #if UINTPTR_MAX == 0xffffffff
    buffer_virtual_address_ccc = (unsigned int)ccc_buffer_ccc;
    #elif UINTPTR_MAX == 0xffffffffffffffff
    buffer_virtual_address_ccc = (unsigned long long)ccc_buffer_ccc;
    #else
    buffer_virtual_address_ccc = (unsigned int)ccc_buffer_ccc;
    #endif
    //fprintf (stdout, "buffer address=0x%llx\n", buffer_virtual_address_ccc);    //debug

    int proc_id = getpid();
    //char command[255];
    //sprintf (command, "cat /proc/%d/maps", proc_id);
    //fprintf (stdout, "%s\n", command);
    //system (command);

    char path[255];
    sprintf (path, "/proc/%d/pagemap", proc_id);
    pagemap_ccc = fopen(path, "rb");
    if(!pagemap_ccc)
    {
      sprintf(tempmessage_ccc, "Error! Cannot open %s\n", path);
      message_now_ccc(tempmessage_ccc);
      return -1;
    }

    // get main buffer location
    unsigned long long file_offset = buffer_virtual_address_ccc / pagesize_ccc * 8;
    //printf("Vaddr: 0x%llx, pagesize_ccc: %d\n", buffer_virtual_address_ccc, pagesize_ccc);    //debug
    //printf("Reading %s at 0x%llx\n", path, file_offset);    //debug
    int status = fseek(pagemap_ccc, file_offset, SEEK_SET);
    if(status){
      sprintf (tempmessage_ccc, "Failed to do fseek! (%s)", strerror(errno));
      message_now_ccc(tempmessage_ccc);
      return -1;
    }

    int count = (real_buffer_size_ccc / pagesize_ccc);
    //int count = (max_dma_size_ccc / pagesize_ccc);
    if (real_buffer_size_ccc % pagesize_ccc > 0)
    {
      count++;
    }
    unsigned int read_size = 8 * count;
    //fprintf (stdout, "total_buffer_page_count=%d\n", count);    //debug
    //exit (0);
    count--;
    unsigned char file_data[read_size];
    if (fread(file_data, 1, read_size, pagemap_ccc) == read_size )
    {
      while (read_size > 0)
      {
        buffer_physical_address_ccc[count] = 0;
        int i;
        for (i = read_size-1; i >= (int)read_size - 8; i--)
        {
          //fprintf (stdout, "%02x", file_data[i]);    //debug
          buffer_physical_address_ccc[count] = (buffer_physical_address_ccc[count] << 8) + file_data[i];
        }
        //fprintf (stdout, "\n");
        buffer_physical_address_ccc[count] <<= 12;
#ifdef DEBUG
        if ((debug_ccc & DEBUG34) && count < (int)(max_dma_size_ccc / pagesize_ccc) )
        {
          fprintf (stdout, "buffer_physical_address[%d]=0x%llx\n", count, buffer_physical_address_ccc[count]);
        }
#endif
        if (buffer_physical_address_ccc[count] > 0xffffffff && count < (int)(max_dma_size_ccc / pagesize_ccc) )
        {
#ifdef DEBUG
          if (debug_ccc & DEBUG34)
          {
            fprintf (stdout, "buffer_physical_address[%d]=0x%llx is out of range\n", count, buffer_physical_address_ccc[count]);
          }
#endif
          out_of_range++;
        }
        read_size -= 8;
        count--;
      }
    }
    fclose(pagemap_ccc);
    if (out_of_range == 0)
    {
      break;
    }
    if (driver_memory_mapped_ccc)
    {
      return -1;
    }
    tries++;
  }
  //fprintf (stdout, "tries=%lld\n", tries);    //debug
  free (padding_buffer_ccc);
  return (out_of_range);
}





void dump_data_to_filename_binary_ccc(char *filename, unsigned char *data, int size, char *description)
{
  FILE *file = fopen(filename, "a");
  time_t mytime;
  mytime = time(NULL);
  fprintf (file, "******dump data ****** %d %s %s", size, description, ctime(&mytime));
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
  int fp = fileno(file);
  fsync(fp);
  fclose(file);
}





void dump_data_to_filename_ccc(char *filename, void* buffer, int size, char *description)
{
  unsigned char *data = buffer;
  FILE *file = fopen(filename, "a");
  time_t mytime;
  mytime = time(NULL);
  fprintf (file, "****** dump data ****** %d %s %s", size, description, ctime(&mytime));
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
  int fp = fileno(file);
  fsync(fp);
  fclose(file);
}





void dump_info_to_filename_ccc(char *filename, char *info)
{
  FILE *file = fopen(filename, "a");
  time_t mytime;
  mytime = time(NULL);
  fprintf (file, "****** dump info ****** %s", ctime(&mytime));
  fprintf (file, "%s", info);
  int fp = fileno(file);
  fsync(fp);
  fclose(file);
}




// function to find the next matching command and return the line number
// returns -1 if not found
int find_command_ccc(char *search_command, unsigned int start_line)
{
  int scanline;
  unsigned int line_number;
  char command[MAX_LINE_LENGTH];
  for (line_number = start_line; line_number < total_script_lines_ccc; line_number++)
  {
    scanline = sscanf(script_line_pointer_ccc[line_number], "%s[^\n]", command);

    // if there is a command found check to see if it matches
    if (scanline == 1)
    {
      //fprintf (stdout, "command= %s\n", command);
      if (strcasecmp(search_command, command) == 0)
      {
        return (line_number);
      }
    }
  }
  return (-1);
}






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



#if 1
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

