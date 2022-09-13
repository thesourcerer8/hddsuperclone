// Copyright (C) 2015-2022 Scott Dwyer.
// You may use/distribute/modify this freely, under the terms of
// the GNU General Public License version 2 or later version.
// This software is distributed WITHOUT ANY WARRANTY.


#include <stdbool.h>
#include <stdint.h>

#define MAX_DRIVERS 24
#define MAX_BUSES 24
#define MAX_RESOURCES 32
#define SATA_SIG_ATA    0x00000101      // SATA drive
#define SATA_SIG_ATAPI  0xEB140101      // SATAPI drive
#define SATA_SIG_SEMB   0xC33C0101      // Enclosure management bridge
#define SATA_SIG_PM     0x96690101      // Port multiplier
#define REG_H2D 0x27  // Register FIS - host to device
#define REG_D2H 0x34  // Register FIS - device to host
#define SDB_D2H 0xa1  // Set device bits FIS - device to host
#define START_BIT_TIME 1  // time in usec
#define FIS_BIT_TIME 1  // time in usec
#define COMMAND_BIT_TIME 100  // time in usec
#define COMMAND_LIST_OVERRIDE_TIME 1000  // time in usec

#define IOCTL_CMD_HDDSC 0xa5a5a5a5
#define START_DRIVE_COMMAND 123
#define STOP_DRIVE_COMMAND 321
#define START_FILE_COMMAND 456
#define STOP_FILE_COMMAND 654
#define ACTIVATE_DRIVE_COMMAND 555
#define READ_MEMORY_DRIVE_COMMAND 777
#define DRIVER_WAIT_TO_ACTIVATE_TIME 2000000000ULL // time in nanoseconds

#define MAX_DEVICE_NAME_LENGTH 32

extern unsigned char io_byte_ccc[16];
extern uint8_t io_singlebyte_ccc;
extern uint16_t io_word_ccc;
extern uint32_t io_doubleword_ccc;
extern uint64_t io_quadword_ccc;
extern uint8_t start_bit_ccc;
extern int command_status_ccc;
extern int identify_flag_ccc;
extern char error_string_ccc[255];
extern int lsblk_ccc;
extern void *hba_virt_addr_ccc;
extern int performing_reset_ccc;
extern int did_hard_reset_ccc;
extern int did_power_cycle_ccc;
extern int words_per_logical_sector_ccc;
extern int bytes_per_logical_sector_ccc;
extern char driver_device_name_ccc[64];
extern bool check_message_ccc;
extern char *title_ccc;
extern char virtual_driver_name_ccc[MAX_DEVICE_NAME_LENGTH];



int prepare_cdb_ccc(void);

int post_ioctl_ccc(void);

int do_ata_pio_read_ccc(int command_type);

int do_ata_dma_read_ccc(int command_type);

int do_ata_pio_write_ccc(int command_type);

int do_ata_dma_write_ccc(int command_type);

int ahci_rw_ccc(int command_type, int write_bit);

int check_for_unwanted_changes_ccc(void);

unsigned int get_ahci_status_ccc(void);

unsigned int get_ahci_error_ccc(void);

int disable_fis_ccc(unsigned long long time);

int disable_start_ccc(unsigned long long time);

int enable_fis_ccc(unsigned long long time);

int enable_start_ccc(unsigned long long time);

int enable_command_issue_ccc(unsigned long long time);

int post_direct_ccc(int command_type);

int set_table_address_ccc(void);

int restore_table_address_ccc(void);

int find_bus_devices_ccc(char *driver);

int get_device_resources_ccc(char *bus, char *driver);

int process_resources_ccc(unsigned long long *start, unsigned long long *end, unsigned long long *flag, int count, char *bus, char *driver);

int identify_device_direct_ccc(unsigned long long address, int device_select, int count);

int identify_device_ahci_ccc(int count);

int identify_device_passthrough_ccc(char *name, int count);

int identify_device_scsi_ccc(char *name, int count);

int soft_reset_ccc(int disk_fd);

int hard_reset_ccc(int disk_fd);

int hba_reset_ccc(void);

int hba_test_ccc(void);

void dump_hba_data_to_file_ccc(FILE *file, unsigned char *data, int size);

void dump_data_to_file_ccc(FILE *file, void *data, int size);

int wait_not_busy_ccc(unsigned long long time);

int wait_not_busy_or_drq_ccc(unsigned long long time, int check);

int wait_drdy_not_busy_or_drq_ccc(unsigned long long time, int check);

int set_and_send_regs_ccc(int command_type);

int get_device_information_ccc(char *driver, char *bus, int bus_count, int device, int current_device_count);

int dump_hba_port_fis_command_data_ccc(unsigned long long hba_address, unsigned long long port_address, int hba, int port, int fis, int cdata, int ctable);

int do_usb_cmd_ccc(void);

int usb_get_sense_ccc(void);

int usb_check_capacity_ccc(int timeout);


extern bool use_fpdma_ccc;
extern bool wait_for_ds_bit_ccc;
extern int driver_running_ccc;
#define MAXLANGLENGTH 2048
#define LANGCOUNT 427
#define LANGERROR 4
#define LANGALLOCATEMEMERR 169
extern char curlang_ccc[LANGCOUNT][MAXLANGLENGTH];
extern char *version_number_ccc;











