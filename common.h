// Copyright (C) 2015-2022 Scott Dwyer.
// You may use/distribute/modify this freely, under the terms of
// the GNU General Public License version 2 or later version.
// This software is distributed WITHOUT ANY WARRANTY.


#define PRINT_LOCATION()                             \
do                                                 \
{                                                  \
  printf ("At %s:%d\n", __FILE__, __LINE__);       \
} while (0)

#define _GNU_SOURCE
#define _FILE_OFFSET_BITS 64
#include "conditional.h"
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
#include <unistd.h>
#include <fcntl.h>
#include <malloc.h>
#include <sys/ioctl.h>
#include <scsi/sg.h>
#include <net/if.h>
#include <netinet/in.h>
#include <linux/limits.h>
#include <sys/io.h>
#include <sys/mman.h>
#include <byteswap.h>
#include <dirent.h>
#include <usb.h>

#define DEBUG1 0x00000001    // general program startup items
#define DEBUG2 0x00000002    // logfile reading writing
#define DEBUG3 0x00000004    // logfile processing
#define DEBUG4 0x00000008    // longest read time
#define DEBUG5 0x00000010    // device details
#define DEBUG6 0x00000020    // device details extended
#define DEBUG7 0x00000040    // CDB data
#define DEBUG8 0x00000080    // process resource data
#define DEBUG9 0x00000100    // identify device info
#define DEBUG10 0x00000200    // domain processing info
#define DEBUG11 0x00000400    // read chunk info
#define DEBUG12 0x00000800    // write chunk info
#define DEBUG13 0x00001000    // process chunk info
#define DEBUG14 0x00002000    // logfile error processing
#define DEBUG15 0x00004000    // logfile repair
#define DEBUG16 0x00008000    // read errors
#define DEBUG17 0x00010000    // process source
#define DEBUG18 0x00020000    // display output
#define DEBUG19 0x00040000    // USB debug
#define DEBUG20 0x00080000
#define DEBUG21 0x00100000
#define DEBUG22 0x00200000
#define DEBUG23 0x00400000
#define DEBUG24 0x00800000
#define DEBUG25 0x01000000
#define DEBUG26 0x02000000
#define DEBUG27 0x04000000
#define DEBUG28 0x08000000
#define DEBUG29 0x10000000
#define DEBUG30 0x20000000
#define DEBUG31 0x40000000
#define DEBUG32 0x80000000
#define DEBUG33 0x0000000100000000    // dma table
#define DEBUG34 0x0000000200000000    // physical addresses
#define DEBUG35 0x0000000400000000    // soft and hard reset info
#define DEBUG36 0x0000000800000000    // table fis command buffers
#define DEBUG37 0x0000001000000000    // refresh status hba port info
#define DEBUG38 0x0000002000000000    // check changes hba port info
#define DEBUG39 0x0000004000000000    // hba and port info
#define DEBUG40 0x0000008000000000    // rebuild assist head mapping
#define DEBUG41 0x0000010000000000
#define DEBUG42 0x0000020000000000
#define DEBUG43 0x0000040000000000
#define DEBUG44 0x0000080000000000
#define DEBUG45 0x0000100000000000
#define DEBUG46 0x0000200000000000
#define DEBUG47 0x0000400000000000
#define DEBUG48 0x0000800000000000
#define DEBUG49 0x0001000000000000
#define DEBUG50 0x0002000000000000
#define DEBUG51 0x0004000000000000
#define DEBUG52 0x0008000000000000
#define DEBUG53 0x0010000000000000
#define DEBUG54 0x0020000000000000
#define DEBUG55 0x0040000000000000
#define DEBUG56 0x0080000000000000
#define DEBUG57 0x0100000000000000
#define DEBUG58 0x0200000000000000
#define DEBUG59 0x0400000000000000
#define DEBUG60 0x0800000000000000
#define DEBUG61 0x1000000000000000
#define DEBUG62 0x2000000000000000
#define DEBUG63 0x4000000000000000
#define DEBUG64 0x8000000000000000

#define MAX_ARGUMENTS 16
#define MAX_COMMAND_LENGTH 64
#define MAX_BUFFER_SIZE 4194304    // never make this bigger than driver main data buffer size
#define MAX_SCRATCHPAD_SIZE 268435456
#define MAX_SENSEBUFFER_SIZE 256
#define MAX_USB_BUFFER_SIZE 33546240
#define USBCBW_BUFFER_SIZE 31
#define USBCSW_BUFFER_SIZE 13
#define USB_SENSE_BUFFER_SIZE 18
#define USB_SENSE_BUFFER_DESC_SIZE 22
#define MAX_VARIABLE_LENGTH 256
#define MAX_VARIABLE_NAME_LENGTH 64
#define MAX_LINE_LENGTH 256
#define MAX_SCRIPT_DEPTH 32
#define DISPLAY_MESSAGE_SIZE 4096
#define EXIT_MESSAGE_SIZE 2048
#define ERROR_MESSAGE_SIZE 1024
#define TEMP_MESSAGE_SIZE 32768
#define DEBUG_MESSAGE_SIZE 4096
#define REBUILD_ASSIST_FIELD_LENGTH 128

#define MAIN_DRIVER_IOCTL_NAME "hddscbdc"
#define MAIN_DRIVER_MMAP_NAME "hddscmap_m"
#define MAIN_DRIVER_MMAPTB_NAME "hddscmap_tb"
#define MAIN_DRIVER_MMAPMDB_NAME "hddscmap_mdb"

#define FIRST_READ_TIMER 800000  // 0.8 sec
#define SOFT_RESET_TIMER 8000000  // 8 sec
#define HARD_RESET_TIMER 800000  // 0.8 sec
#define BUSY_WAIT_TIMER 20000000  // 20 sec
#define RESET_WAIT_TIMER 8000000  // 8 sec
#define GENERAL_TIMER 60000000  // 60 sec
#define USB_TIMER 8000  // 8 sec
#define CBW_TIMER 8000  // 8 sec
#define CSW_TIMER 8000  // 8 sec
#define INITIAL_BUSY_WAIT_TIME 8000000  // 8 sec
#define P12SOFT_RESET_TIMER 250000  // .25 sec
#define P3SOFT_RESET_TIMER 350000  // .35 sec
#define P4SOFT_RESET_TIMER 500000  // .5 sec
#define TDSOFT_RESET_TIMER 800000  // .8 sec
#define D2SOFT_RESET_TIMER 800000  // .8 sec
#define SCSOFT_RESET_TIMER 800000  // .8 sec
#define RTSOFT_RESET_TIMER 8000000  // 8 sec
#define HBA_RESET_TIME 1000000  // 1 sec

#define DRIVER_READ_TIMEOUT 90000000  // 90 sec, not currently used
#define DRIVER_REQUEST_TIMEOUT 90000000  // 90 sec, not currently used

#define DISPLAY_STATUS_UPDATE_TIME 600    // ms

#define NORMAL_EXIT_CODE 0
#define GENERAL_ERROR_EXIT_CODE 1
#define GENERAL_ERROR_RETURN_CODE -1
#define LOGFILE_ERROR_EXIT_CODE 2
#define LOGFILE_ERROR_RETURN_CODE -2
#define OUTPUT_DEVICE_ERROR_EXIT_CODE 3
#define OUTPUT_DEVICE_ERROR_RETURN_CODE -3
#define INPUT_DEVICE_ERROR_EXIT_CODE 4
#define INPUT_DEVICE_ERROR_RETURN_CODE -4
#define INTERNAL_ERROR_EXIT_CODE 15
#define INTERNAL_ERROR_RETURN_CODE -15
#define EXIT_ON_SLOW_EXIT_CODE 16
#define EXIT_ON_SLOW_RETURN_CODE -16
#define DEVICE_FAULT_EXIT_CODE 32
#define DEVICE_FAULT_RETURN_CODE -32
#define DEVICE_ERROR_EXIT_CODE 64
#define DEVICE_ERROR_RETURN_CODE -64
#define STOP_SIGNAL_RETURN_CODE -128


#define MAX_DEVICES 32
#define MAX_USB_DEVICES 64
#define DEVICE_TYPE_SCSI 0
#define DEVICE_TYPE_ATA 1
#define DEVICE_TYPE_IDE 2
#define DEVICE_TYPE_AHCI 3
#define DEVICE_TYPE_BLOCK 4
#define USB_DIRECTION_IN 0x80
#define USB_DIRECTION_OUT 0
#define USB_RESET_TYPE_SOFT 2
#define USB_RESET_TYPE_HARD 0

#define MAX_RETRIES 8
#define MAX_CLUSTERSIZE 65528
#define MAX_BLOCKSIZE 16
#define MIN_SECTORSIZE 256
#define MAX_SECTORSIZE 8192
#define IDENTIFY_BUFFER_SIZE 512
#define LOG_PAGE_SIZE 512

#define KERNEL_SECTOR_SIZE 512
#define DRIVER_CONTROL_BUFFER_SIZE 131072    // make sure to adjust with page power
#define DRIVER_TRANSFER_BUFFER_SIZE 4194304   // make sure to adjust with page power, and never bigger than 2mb (now 4mb) because of io bitmap
#define DRIVER_MAIN_DATA_BUFFER_SIZE 4194304   // make sure to adjust with page power
#define USER_DATA_BUFFER_SIZE 65536

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

#define SKEY_MEDIUM_ERROR 0x03
#define SKEY_ABORTED_COMMAND 0x0b
#define ASC_UNRECOVERED_READ_ERROR 0x1100
#define ASC_MULTIPLE_READ_ERRORS 0x1103


extern time_t start_time_ccc, end_time_ccc;
extern struct timeval tvBegin_ccc, tvEnd_ccc, tvDiff_ccc, tvCurrent_ccc;
int timeval_subtract_ccc(struct timeval *result, struct timeval *t2, struct timeval *t1);
void timeval_print_ccc(struct timeval *tv);


extern bool check_only_ccc;
extern bool do_indentation_ccc;
extern int ccc_indent_ccc;
extern int spaces_ccc;
extern char *argument_ccc[MAX_ARGUMENTS];
extern unsigned int argument_count_ccc;

extern int process_id_ccc;
extern int driver_return_zeros_on_error_ccc;
extern int driver_io_scsi_only_ccc;
extern bool quiet_ccc;
extern bool stop_signal_ccc;
extern bool superclone_ccc;
extern bool critical_process_ccc;
extern bool pending_exit_ccc;
extern bool logfile_changed_ccc;
extern bool domainfile_changed_ccc;
extern bool force_danger_ccc;
extern bool force_stupid_ccc;
extern bool repair_log_ccc;
extern bool fill_mode_ccc;
extern bool driver_only_ccc;
extern bool cleanup_mode_ccc;
extern unsigned long long verbose_ccc;
extern unsigned long long debug_ccc;
extern bool debugversion_ccc;
extern bool verbosedebug_ccc;
extern int return_value_ccc;
extern bool print_hex_ccc;
extern int if_statement_level_ccc;
extern int while_statement_level_ccc;
extern long long error_level_ccc;
extern unsigned char exitcode_ccc;
extern FILE *debug_file_ccc;
extern char *disk_1_ccc;
extern char *disk_2_ccc;
extern char *script_file_ccc;
extern char current_script_ccc[MAX_LINE_LENGTH];
extern int print_error_message_ccc;
extern char exit_message_ccc[EXIT_MESSAGE_SIZE];
extern char error_message_ccc[ERROR_MESSAGE_SIZE];
extern char display_message_ccc[DISPLAY_MESSAGE_SIZE];
extern char tempmessage_ccc[TEMP_MESSAGE_SIZE];
extern char debug_message_ccc[DEBUG_MESSAGE_SIZE];
extern char log_display_message_ccc[DISPLAY_MESSAGE_SIZE];

extern void* ccc_buffer_ccc;
extern unsigned long long ccc_main_buffer_size_ccc;
extern void* ccc_scratchpad_ccc;
extern unsigned long long ccc_main_scratchpad_size_ccc;
extern void* sensebuffer_ccc;
extern unsigned long long sensebuffer_size_ccc;
extern void* ccc_usbbuffer_ccc;
extern char usbcbwbuffer_ccc[USBCBW_BUFFER_SIZE];
extern char usbcswbuffer_ccc[USBCSW_BUFFER_SIZE];
extern unsigned long long ccc_main_usbbuffer_size_ccc;
extern void* padding_buffer_ccc;
extern unsigned char identify_buffer_ccc[IDENTIFY_BUFFER_SIZE];
extern unsigned char ata_identify_buffer_ccc[IDENTIFY_BUFFER_SIZE];
extern unsigned char read_capacity_buffer_ccc[512];
extern int append_script_ccc;
extern long long *number_variable_buffer_ccc;
extern char **string_variable_pointer_ccc;
extern char current_date_ccc[40];
extern char current_time_ccc[40];
uint8_t rotl8_ccc(uint8_t value, int shift);
uint8_t rotr8_ccc(uint8_t value, int shift);
uint16_t rotl16_ccc(uint16_t value, int shift);
uint16_t rotr16_ccc(uint16_t value, int shift);
uint32_t rotl32_ccc(uint32_t value, int shift);
uint32_t rotr32_ccc(uint32_t value, int shift);
uint64_t rotl64_ccc(uint64_t value, int shift);
uint64_t rotr64_ccc(uint64_t value, int shift);
int get_random_value_ccc(int speed);

extern unsigned char sbyte_ccc[16];
extern unsigned char superbyte_ccc[1024];
extern int activation_days_remaining_ccc;
extern int check_command_ccc;
extern int check_read_ccc;
extern int check_write_ccc;
 
extern int disk1_fd_ccc;
extern int disk2_fd_ccc;
extern int current_disk_ccc;
int disk_reopen_ccc(void);
extern int main_driver_fd_ccc;
extern bool command_line_ccc;
extern int command_length_ccc;
extern unsigned char sense_buf_ccc[255];
extern unsigned char sense_key_ccc;
extern unsigned char asc_ccc;
extern unsigned char ascq_ccc;
extern unsigned char ata_error_ccc;
extern unsigned char ata_status_ccc;
extern unsigned char ata_device_ccc;
extern unsigned char ata_count1_ccc;
extern unsigned char ata_count2_ccc;
extern unsigned char ata_lba1_ccc;
extern unsigned char ata_lba2_ccc;
extern unsigned char ata_lba3_ccc;
extern unsigned char ata_lba4_ccc;
extern unsigned char ata_lba5_ccc;
extern unsigned char ata_lba6_ccc;
extern long long ata_lba_ccc;
extern int ata_count_ccc;
extern int io_status_ccc;
extern int io_masked_status_ccc;
extern int msg_status_ccc;
extern int io_sb_len_wr_ccc;
extern int io_host_status_ccc;
extern int io_driver_status_ccc;
extern int io_resid_ccc;
extern int io_duration_ccc;
extern int io_info_ccc;
extern int ata_return_valid_ccc;
extern int ata_check_ccc;
extern bool pio_mode_ccc;
extern int driver_installed_ccc;
extern long long hpa_lba_ccc;
extern long long dco_lba_ccc;
extern bool extended_support_ccc;
extern bool enable_process_chunk_ccc;
extern bool enable_read_twice_ccc;
extern bool enable_scsi_write_ccc;
extern bool charater_device_driver_mode_ccc;

typedef struct {
  unsigned char command_type;
  unsigned char scsi_cmd[32];
  unsigned char ata_cmd[12];
  bool extended;
  int direction;
  int sg_xfer_direction;
  unsigned char protocol;
  unsigned char transfer_length;
  unsigned char transfer_direction;
  bool byte_block;
  unsigned char multiple_count;
  unsigned char off_line;
  bool check_condition;
} passthrough_ccc_type;

extern passthrough_ccc_type passthrough_ccc;

// TODO always make sure this matches the structure in the driver!
/*
struct
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
} driver_control_data_ccc;
*/
typedef struct __attribute__((packed))
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
  uint64_t  read_timeout;
  uint64_t request_timeout;
  uint64_t ack_timeout;
  uint64_t process_id;
  char name[16];
  char buffer[DRIVER_TRANSFER_BUFFER_SIZE/2];
} driver_control_data_ccc_type;

extern driver_control_data_ccc_type driver_control_data_ccc;


extern unsigned long reg_base_address_ccc;
extern unsigned long control_base_address_ccc;
extern unsigned long bus_base_address_ccc;
extern bool device_select_base_ccc;
extern unsigned long long hba_base_address_ccc;
extern unsigned long long hba_reset_address_ccc;
extern unsigned long long port_base_address_ccc;
extern unsigned long long port_reset_address_ccc;
extern int port_number_base_ccc;
extern char device_driver_base_ccc[64];
extern char device_bus_base_ccc[64];
extern char current_source_model_ccc[64];
extern char current_source_serial_ccc[64];
extern char current_destination_model_ccc[64];
extern char current_destination_serial_ccc[64];
extern int current_device_type_ccc;
extern bool copy_image_mode_ccc;
extern bool generic_mode_ccc;
extern bool direct_mode_ccc;
extern bool ahci_mode_ccc;
extern bool ata_passthrough_ccc;
extern bool scsi_passthrough_ccc;
extern bool nvme_passthrough_ccc;
extern bool auto_passthrough_ccc;
extern bool usb_mode_ccc;
extern bool usb_allow_ata_ccc;
extern bool supertool_mode_chosen_ccc;
extern unsigned int pagesize_ccc;
extern unsigned long long table_virtual_address_ccc;
extern unsigned long long command_list_virtual_address_ccc;
extern unsigned long long fis_virtual_address_ccc;
extern unsigned long long buffer_virtual_address_ccc;
extern unsigned long long table_physical_address_ccc;
extern unsigned long long command_list_physical_address_ccc;
extern unsigned long long fis_physical_address_ccc;
extern unsigned long long buffer_physical_address_ccc[8192];
extern int command_list_size_ccc;
extern int fis_size_ccc;
extern void* table_buffer_ccc;
extern void* command_list_buffer_ccc;
extern void* fis_buffer_ccc;
extern int hba_mem_dev_ccc;
extern uint32_t hba_alloc_mem_size_ccc;
extern void *hba_mem_pointer_ccc;
extern int port_mem_dev_ccc;
extern uint32_t port_alloc_mem_size_ccc;
extern void *port_mem_pointer_ccc;
extern void *port_virt_addr_ccc;
extern unsigned long long max_dma_size_ccc;
extern unsigned long long real_buffer_size_ccc;
extern unsigned long long first_read_time_ccc;
extern unsigned long long soft_reset_time_ccc;
extern unsigned long long hard_reset_time_ccc;
extern unsigned long long busy_wait_time_ccc;
extern unsigned long long reset_wait_time_ccc;
extern unsigned long long initial_busy_wait_time_ccc;
extern unsigned long long general_timeout_ccc;
extern int usb_timeout_ccc;
extern int cbw_timeout_ccc;
extern int csw_timeout_ccc;
extern unsigned long long power_cycle_time_ccc;
extern bool phase_timers_ccc;
extern unsigned long long orig_soft_reset_time_ccc;
extern unsigned long long p12_soft_reset_time_ccc;
extern unsigned long long p3_soft_reset_time_ccc;
extern unsigned long long p4_soft_reset_time_ccc;
extern unsigned long long td_soft_reset_time_ccc;
extern unsigned long long d2_soft_reset_time_ccc;
extern unsigned long long sc_soft_reset_time_ccc;
extern unsigned long long rt_soft_reset_time_ccc;
extern char *drive_serial_ccc;
extern int new_source_ccc;
extern int new_destination_ccc;
extern long long source_total_size_ccc;
extern long long bytes_per_sector_ccc;
extern long long target_total_size_ccc;
extern long long generic_source_total_size_ccc;
extern unsigned char *driver_control_address_ccc;
extern unsigned char *driver_error_bitmap_address_ccc;
extern unsigned char *driver_transfer_buffer_address_ccc;
extern unsigned char *driver_main_data_buffer_address_ccc;
extern unsigned char *driver_table_buffer_ccc;
extern unsigned char *driver_command_list_buffer_ccc;
extern unsigned char *driver_fis_buffer_ccc;
extern int driver_memory_mapped_ccc;
extern bool always_wait_for_reset_timers_ccc;
extern bool dont_wait_for_drdy_ccc;

extern bool table_address_changed_ccc;
extern unsigned char table_address_backup_ccc[4];
extern bool ahci_address_changed_ccc;
extern uint64_t command_list_address_backup_ccc;
extern uint64_t fis_address_backup_ccc;
extern bool ahci_interrupt_changed_ccc;
extern uint32_t interrupt_backup_ccc;
extern bool memory_mapped_ccc;
extern bool memory_failed_ccc;
extern bool io_permissions_ccc;
extern int table_size_ccc;
extern int table_entry_count_ccc;
extern FILE *pagemap_ccc;

extern bool ncq_supported_ccc;    // word 76 bit 8
extern bool rebuild_assist_supported_ccc;    // word 78 bit 11


extern char device_driver_ccc[MAX_DEVICES][64];
extern char device_bus_ccc[MAX_DEVICES][64];
extern unsigned long long reg_address_ccc[MAX_DEVICES];
extern unsigned long long control_address_ccc[MAX_DEVICES];
extern unsigned long long bus_address_ccc[MAX_DEVICES];
extern unsigned long long hba_address_ccc[MAX_DEVICES];
extern int port_number_ccc[MAX_DEVICES];
extern unsigned int port_status_ccc[MAX_DEVICES];
extern unsigned long long port_address_ccc[MAX_DEVICES];
extern unsigned int port_signature_ccc[MAX_DEVICES];
extern bool device_select_ccc[MAX_DEVICES];
extern bool device_present_ccc[MAX_DEVICES];
extern bool device_visable_ccc[MAX_DEVICES];
extern char device_reference_ccc[MAX_DEVICES][32];
extern char device_name_ccc[MAX_DEVICES][32];
extern char model_ccc[MAX_DEVICES][41];
extern char serial_ccc[MAX_DEVICES][21];
extern char drive_list_ccc[MAX_DEVICES][64];
extern long long drive_size_ccc[MAX_DEVICES];
extern int drive_bytes_per_sector_ccc[MAX_DEVICES];
extern int device_type_ccc[MAX_DEVICES];
extern int device_count_ccc;
extern int choose_device_mode_ccc;
extern int did_reset_ccc;
extern bool enable_hba_reset_on_connect_source_ccc;
extern bool enable_hba_test_on_connect_source_ccc;
extern bool enable_dont_identify_on_choose_source_ccc;
extern bool enable_data_dump_ccc;
extern char data_dump_filename_ccc[1024];
extern bool enable_dump_identify_on_check_ccc;

extern int usbr1_vendor_id_ccc;
extern int usbr1_product_id_ccc;
extern char usbr1_vendor_string_ccc[256];
extern char usbr1_product_string_ccc[256];
extern char usbr1_extra_id_string_ccc[256];
extern int usbr1_bus_number_ccc;
extern int usbr1_bus_real_number_ccc;
extern int usbr1_device_number_ccc;

extern int usbd1_vendor_id_ccc;
extern int usbd1_product_id_ccc;
extern char usbd1_vendor_string_ccc[256];
extern char usbd1_product_string_ccc[256];
extern char usbd1_extra_id_string_ccc[256];
extern int usbd1_bus_number_ccc;
extern int usbd1_bus_real_number_ccc;
extern int usbd1_device_number_ccc;
extern int usbd1_bulk_in_endpoint_ccc;
extern int usbd1_bulk_out_endpoint_ccc;

extern int usb_device_count_ccc;
extern int usb_vendor_id_ccc[MAX_USB_DEVICES];
extern int usb_product_id_ccc[MAX_USB_DEVICES];
extern char usb_vendor_string_ccc[MAX_USB_DEVICES][256];
extern char usb_product_string_ccc[MAX_USB_DEVICES][256];
extern char usb_serial_string_ccc[MAX_USB_DEVICES][256];
extern char usb_extra_id_string_ccc[MAX_USB_DEVICES][256];
extern int usb_bus_number_ccc[MAX_USB_DEVICES];
extern int usb_bus_real_number_ccc[MAX_USB_DEVICES];
extern int usb_device_number_ccc[MAX_USB_DEVICES];
extern int usb_known_relay_ccc[MAX_USB_DEVICES];
extern int usb_mass_storage_ccc[MAX_USB_DEVICES];
extern int usb_bulk_in_endpoint_ccc[MAX_USB_DEVICES];
extern int usb_bulk_out_endpoint_ccc[MAX_USB_DEVICES];
extern struct usb_dev_handle *usbr1_dev_handle_ccc;
extern struct usb_dev_handle *usbd1_dev_handle_ccc;
extern int new_usb_ccc;
extern unsigned char usb_transfer_direction_ccc;
extern unsigned char usb_lun_ccc;
extern int usb_lun_set_ccc;
extern uint32_t usb_tag_ccc;
extern uint32_t usb_transfer_length_ccc;
extern unsigned char usb_csw_status_ccc;
extern uint32_t usb_csw_residue_ccc;
extern int usb_read_residue_ccc;
extern int usb_csw_valid_ccc;
extern int usb_bulk_reset_enabled_ccc;
extern int usb_soft_reset_enabled_ccc;
extern int usb_hard_reset_enabled_ccc;
extern int usb_port_reset_enabled_ccc;

typedef struct
{
  unsigned char flags;
  unsigned char status;
  unsigned char error;
  long long lba;
  int count;
  unsigned char device;
  unsigned char sense_key;
  unsigned char sense_asc;
  unsigned char sense_ascq;
  long long final_lba;
} ncq_error_log_data_ccc_type;
extern ncq_error_log_data_ccc_type ncq_error_log_data_ccc;


typedef struct
{
  unsigned char flags;
  unsigned char length;
  unsigned char mask[REBUILD_ASSIST_FIELD_LENGTH];
  unsigned char disabled[REBUILD_ASSIST_FIELD_LENGTH];
} rebuild_assist_log_data_ccc_type;
extern rebuild_assist_log_data_ccc_type rebuild_assist_log_data_ccc;


extern bool use_rebuild_assist_ccc;
extern bool rebuild_assist_data_valid_ccc;
extern unsigned char rebuild_assist_element_length_ccc;
extern char rebuild_assist_element_mask_ccc[REBUILD_ASSIST_FIELD_LENGTH];
extern char rebuild_assist_working_mask_ccc[REBUILD_ASSIST_FIELD_LENGTH];

extern int rebuild_assist_test_ccc;


int supertooltool_ccc (void);

int set_main_buffer_ccc(void);

int set_sense_buffer_ccc(void);

int create_dma_table_ccc(void);

int set_table_buffer_ccc(void);

int get_table_physical_memory_location_ccc(void);

int set_command_list_buffer_ccc(void);

int get_command_list_physical_memory_location_ccc(void);

int set_fis_buffer_ccc(void);

int get_fis_physical_memory_location_ccc(void);

int get_buffer_physical_memory_locations_ccc(void);

int set_main_scratchpad_ccc(void);

int read_script_file_ccc(char *script_file);

int execute_line_ccc(bool perform_check, unsigned int line_number, char *command, char *rest_of_line);

int check_variable_ccc(char *var_name);

int check_arguments_ccc(char *var_name);

int create_number_variable_ccc(char *var_name);

int create_string_variable_ccc(char *var_name);

int find_number_variable_ccc(char *var_name);

int find_string_variable_ccc(char *var_name);

int set_number_variable_ccc(int var_num, char *raw_value);

int set_string_variable_ccc(int var_num, char *raw_value);

int find_command_ccc(char *search_command, unsigned int start_line);

int find_command_plus1_ccc(char *search_command, char *search_command_cap, char *first_item, unsigned int start_line);

char *get_full_line_ccc(unsigned int line_number, char* max_line_buffer);

char *get_rest_of_line_ccc(unsigned int line_number);

int compare_ccc(bool perform_check, char *rest_of_line);

long long get_number_variable_value_ccc(char *raw_variable);

char *get_string_variable_value_ccc(char *raw_variable);

int set_number_variable_value_ccc(char *raw_variable, long long value);

int set_string_variable_value_ccc(char *raw_variable, char *value);

int do_scsi_cmd_ccc(int disk_fd);

int do_ata28_cmd_ccc(int disk_fd);

int do_ata48_cmd_ccc(int disk_fd);

int get_the_time_ccc(void);

char *get_future_date_ccc(long long days);

long long get_elapsed_usec_ccc(void);

int find_all_devices_ccc(void);

int choose_device_ccc(void);

int choose_target_ccc(void);

int identify_device_ccc(void);

int read_capacity_10_ccc(void);

int read_capacity_16_ccc(void);

int do_soft_reset_ccc(int disk_fd);

int do_hard_reset_ccc(int disk_fd);

int do_hba_reset_ccc (void);

int refresh_status_ccc(int disk_fd);

int do_nanosleep_ccc(unsigned long long time);

char *trim_white_space_ccc(char *str);

int set_device_ccc(unsigned long long address, int device);

int message_exit_ccc(char *message);

int message_error_ccc(char *message);

int clear_error_message_ccc(void);

int message_display_ccc(char *message);

int message_now_ccc(char *message);

int message_console_log_ccc(char *message, unsigned long long level);

int message_debug_ccc(char *message, unsigned long long level);

int start_gtk_ccc(int argc, char **argv, char *title, char *version);

int print_gui_error_message_ccc(char *message, char *title, int type);

int update_display_ccc(int time_ms);

int set_initial_data_from_log_ccc(int newproject);

int process_source_ccc(void);

int open_source_disk_ccc(void);

int close_source_disk_ccc(void);

int open_target_destination_ccc(void);

int connect_source_disk_ccc(void);

void release_devices_ccc (void);

void initialize_new_log_ccc (void);

void initialize_domain_log_ccc (void);

void clear_variables_for_new_project_ccc (void);

int check_same_device_ccc(void);

void update_gui_display_ccc (void);

int begin_cloning_ccc (void);

int check_buffer_limit_ccc(void);

int check_and_adjust_variables_after_connection_ccc(void);

int check_and_adjust_variables_after_choosing_source_ccc(int newproject);

int update_logfile_ccc(int time_sec);

int write_logfile_ccc(char *script_file, int time_sec);

int update_domainfile_ccc(int time_sec);

int write_domainfile_ccc(char *log_file, int time_sec);

int reset_log_status_ccc(void);

int complete_log_reset_ccc(void);

int check_and_repair_log_ccc(void);

int call_command_on_power_cycle_ccc(void);

void update_display_status_buttons_ccc(int time_ms);

int start_driver_ccc (void);

int stop_driver_ccc (void);

int begin_driver_ccc (void);

int reset_driver_timers_ccc (void);

int check_driver_ccc (void);

void install_driver_ccc (void);

void uninstall_driver_ccc (void);

int map_driver_memory_ccc (void);

int unmap_driver_memory_ccc (void);

unsigned long long read_ctrl_data_ccc(int item);

void write_ctrl_data_ccc(int item, unsigned long long value);

unsigned char read_ctrl_error_bitmap_ccc(int offset);

void write_ctrl_error_bitmap_ccc(int offset, unsigned char value);

int find_all_usb_devices_ccc(void);

int choose_usb_relay_ccc(void);

int choose_usb_device_ccc(void);

int set_main_usb_buffer_ccc(void);

int do_send_usbr_control_msg_ccc(int requesttype, int request, int value, int index);

int do_send_usbr_interrupt_write_ccc(int endpointout);

int do_send_usbr_interrupt_read_ccc(int endpointin);

int do_send_usbd_control_msg_ccc(int requesttype, int request, int value, int index, int timeout);

int do_usb_read_sense_ccc(int timeout);

int do_usb_raw_read_ccc(int timeout);

int do_usb_raw_write_ccc(int timeout);

int usb_read_data_ccc(int timeout);

int usb_write_data_ccc(int timeout);

int do_usb_send_cbw_ccc(int timeout);

int do_usb_receive_csw_ccc(int timeout);

int usb_get_max_lun_ccc(int timeout);

int check_usb_endpoint_in_ccc(int timeout);

int check_usb_endpoint_out_ccc(int timeout);

int reset_usb_endpoint_in_ccc(void);

int reset_usb_endpoint_out_ccc(void);

int usb_auto_endpoint_reset(int timeout);

int usb_reset_recovery_ccc(int timeout);

int usb_bulk_only_reset_ccc(int timeout);

int do_usb_reset_ccc(void);

int read_ncq_error_log_ccc (void);

int read_rebuild_assist_log_ccc (void);

int write_rebuild_assist_log_ccc (unsigned char data[512]);

int rebuild_assist_map_heads_ccc (void);

int set_rebuild_assist_enabled_ccc (void);

int set_rebuild_assist_disabled_ccc (void);

void dump_data_to_filename_binary_ccc(char *filename, unsigned char *data, int size, char *description);

void dump_data_to_filename_ccc(char *filename, void *data, int size, char *description);

void dump_info_to_filename_ccc(char *filename, char *info);

int connect_usbd1_ccc(void);

int usb_soft_hard_reset_ccc(unsigned char reset_type, int timeout);

int usb_inquiry_ccc(int timeout);

int usb_identify_ccc(int timeout);

int usb_read_smart_ccc(int timeout, unsigned char regs[7]);

int set_dma_mode_ccc(unsigned char mode);

int make_license_files_ccc (int version, int type, int days, int number);

int process_active_licenses_ccc (void);

int disable_rebuild_assist_ccc(void);

int enable_rebuild_assist_ccc(void);

int rebuild_assist_disable_head_ccc (int head);




