// Copyright (C) 2015-2022 Scott Dwyer.
// You may use/distribute/modify this freely, under the terms of
// the GNU General Public License version 2 or later version.
// This software is distributed WITHOUT ANY WARRANTY.


#include "clone_gui_language.h"
#include <stdbool.h>

#define MAX_CALL_LENGTH 200
#define MAX_DEVICE_NAME_LENGTH 32
#define MAX_ANALYZE_TEXT_LENGTH 4096
#define MAX_SMART_DATA_TEXT_LENGTH 4096
#define MAXANALYZESECTIONS 32
#define MAXANALYZESLOW 32
#define ANALYZETIME 120000    // 2 min
#define ANALYZETIMEEXTENDED 960000   // 16 min
#define ANALYZESLOWTIME 15000    // 15 sec
#define ANALYZESLOWTIMEEXTENDED 60000    // 1 min
#define MAX_RELAY_NAME_LENGTH 1024
#define PRIMARY_RELAY_ACTIVATION_TIME 3    // 3 sec
#define PRIMARY_RELAY_DELAY_TIME 15    // 15 sec

extern char *title_ccc;
extern char *version_number_ccc;
extern char *copyright_year_ccc;
extern char curlang_ccc[LANGCOUNT][MAXLANGLENGTH];
extern char newlang_ccc[LANGCOUNT][MAXLANGLENGTH];
extern int translate_ccc;
extern char *log_file_ccc;
extern char *ddilog_file_ccc;
extern char *ddelog_file_ccc;
extern char *domain_file_ccc;
extern int check_log_size_ccc;
extern bool do_domain_ccc;
extern bool create_domain_ccc;
extern bool domain_dd_ccc;
extern bool importdd_ccc;
extern char level4_middle_text_ccc[4096];
extern int running_clone_ccc;
extern char command_to_call_ccc[MAX_CALL_LENGTH];
extern char power_command_to_call_ccc[MAX_CALL_LENGTH];
extern bool stop_on_error_ccc;
extern bool do_call_command_ccc;
extern bool activate_primary_relay_on_error_ccc;
extern bool stop_on_power_cycle_ccc;
extern bool call_power_command_ccc;
extern bool power_cycle_primary_relay_ccc;
extern bool check_message_ccc;
extern bool write_buffer_disable_ccc;
extern bool write_buffer_enable_ccc;
extern bool driver_return_error_ccc;
extern bool driver_return_zeros_ccc;
extern bool driver_return_marked_ccc;
extern int memory_used_ccc;
extern bool data_read_from_log_ccc;
extern int driver_mode_ccc;
extern int driver_running_ccc;
extern char virtual_driver_name_ccc[MAX_DEVICE_NAME_LENGTH];
extern long long last_small_read_size_ccc;
extern long long last_big_read_size_ccc;
extern long long last_actual_read_size_ccc;
extern long long driver_read_rate_size_ccc;
extern bool no_io_error_ccc;
extern char new_ports_ccc[256];
extern int running_analyze_ccc;
extern long long analyze_stoptime_ccc;
extern char analyze_text_ccc[MAX_ANALYZE_TEXT_LENGTH];
extern int analyze_read_attempts_ccc[MAXANALYZESECTIONS];
extern int analyze_good_reads_ccc[MAXANALYZESECTIONS];
extern int analyze_bad_reads_ccc[MAXANALYZESECTIONS];
extern int analyze_slow_reads_ccc[MAXANALYZESECTIONS];
extern int analyze_timeouts_ccc[MAXANALYZESECTIONS];
extern long long analyze_read_time_ccc[MAXANALYZESECTIONS];
extern long long analyze_low_time_ccc[MAXANALYZESECTIONS];
extern long long analyze_high_time_ccc[MAXANALYZESECTIONS];
extern bool enable_extended_analyze_ccc;
extern long long analyze_slow_position_ccc[MAXANALYZESLOW];
extern long long analyze_slow_size_ccc[MAXANALYZESLOW];
extern long long analyze_slow_low_ccc[MAXANALYZESLOW];
extern long long analyze_slow_high_ccc[MAXANALYZESLOW];
extern long long analyze_slow_section_ccc[MAXANALYZESLOW];
extern int analyze_slow_total_reads_ccc;
extern long long skip_timeout_copy_ccc;
extern int cluster_size_copy_ccc;
extern char smart_data_text_ccc[MAX_SMART_DATA_TEXT_LENGTH];
extern bool aggressive_driver_ccc;
extern bool color_statusbar_ccc;
extern char primary_relay_name_ccc[MAX_RELAY_NAME_LENGTH];
extern unsigned long long primary_relay_activation_time_ccc;
extern unsigned long long primary_relay_delay_time_ccc;
extern bool rebuild_assist_enabled_ccc;
extern bool use_fpdma_ccc;
extern bool wait_for_ds_bit_ccc;
extern char identify_device_raw_text_ccc[65536];
 
typedef struct
{
  bool no_phase1;
  bool no_phase2;
  bool no_phase3;
  bool no_phase4;
  bool no_trim;
  bool no_scrape;
  bool no_divide1;
  bool do_divide2;
  bool read_bad_head;
  bool mark_bad_head;
  bool rebuild_assist;
  bool reverse;
  int retries;
  int cluster_size;
  int block_size;
  long long input_offset;
  //long long output_offset;
  long long read_size;
  bool skip_fast;
  long long rate_skip;
  long long exit_on_slow;
  long long exit_slow_time;
  long long min_skip_size;
  long long min_skip_size_bak;
  long long max_skip_size;
  long long skip_timeout;
  int sector_size;
  int block_offset;
  long long max_read_rate;
} clone_settings_ccc_type;
extern clone_settings_ccc_type clone_settings_ccc;


typedef struct
{
  bool force_mounted;
  bool force_dangerous;
  bool no_log_backup;
  bool enable_output_offset;
  long long output_offset;
  bool enable_current_position;
  long long current_position;
  char command_to_call[MAX_CALL_LENGTH];
  bool stop_on_error;
  bool call_command;
  bool activate_primary_relay;
  bool write_buffer_auto;
  bool write_buffer_disable;
  bool write_buffer_enable;
  bool driver_return_error;
  bool driver_return_zeros;
  bool driver_return_marked;
  char virtual_disk_device_name[MAX_DEVICE_NAME_LENGTH];
  bool disable_identify;
  bool pio_mode;
  bool enable_rebuild_assist;
  bool enable_process_chunk;
  bool enable_read_twice;
  bool enable_retry_connecting;
  bool enable_scsi_write;
  bool enable_phase_log;
  bool enable_output_sector_size;
  long long output_sector_adjustment;
  bool driver_io_scsi_only;
  bool use_physical_sector_size_for_virtual;
  long long driver_minimum_cluster_size;
  bool color_statusbar;
} advanced_settings_ccc_type;
extern advanced_settings_ccc_type advanced_settings_ccc;



typedef struct
{
  unsigned long long initial_busy_wait_time;
  unsigned long long busy_wait_time;
  unsigned long long soft_reset_time;
  unsigned long long hard_reset_time;
  unsigned long long power_cycle_time;
  unsigned long long general_timeout;
  char power_command_to_call[MAX_CALL_LENGTH];
  bool stop_on_power_cycle;
  bool call_power_command;
  bool power_cycle_primary_relay;
  bool phase_timers;
  unsigned long long p12_soft_reset_time;
  unsigned long long p3_soft_reset_time;
  unsigned long long p4_soft_reset_time;
  unsigned long long td_soft_reset_time;
  unsigned long long d2_soft_reset_time;
  unsigned long long sc_soft_reset_time;
  unsigned long long rt_soft_reset_time;
  bool always_wait_for_reset_timers;
  int usb_bulk_reset_enabled;
  int usb_soft_reset_enabled;
  int usb_hard_reset_enabled;
  int usb_port_reset_enabled;
} timer_settings_ccc_type;
extern timer_settings_ccc_type timer_settings_ccc;


typedef struct
{
  bool activate_primary_relay1;
  bool activate_primary_relay2;
  bool activate_primary_relay3;
  bool activate_primary_relay4;
  bool activate_primary_relay5;
  bool activate_primary_relay6;
  bool activate_primary_relay7;
  bool activate_primary_relay8;
  bool deactivate_primary_relay1;
  bool deactivate_primary_relay2;
  bool deactivate_primary_relay3;
  bool deactivate_primary_relay4;
  bool deactivate_primary_relay5;
  bool deactivate_primary_relay6;
  bool deactivate_primary_relay7;
  bool deactivate_primary_relay8;
  unsigned long long primary_relay_activation_time;
  unsigned long long primary_relay_delay_time;
  char primary_relay_name[MAX_RELAY_NAME_LENGTH];
}
primary_relay_settings_ccc_type;
extern primary_relay_settings_ccc_type primary_relay_settings_ccc;


typedef struct
{
  char logfile[1024];
  char domainfile[1024];
  char source[256];
  char destination[1024];
  char totallba[32];
  char lbatoread[32];
  char domainsize[32];
  char runtime[32];
  char remainingtime[32];
  char currentposition[32];
  char currentstatus[32];
  char currentrate[32];
  char recentrate[32];
  char totalrate[32];
  char skipsize[32];
  char skips[32];
  char slowskips[32];
  char skipruns[32];
  char skipresets[32];
  char runsize[32];
  char finished[64];
  char nontried[64];
  char nontrimmed[64];
  char nondivided[64];
  char nonscraped[64];
  char bad[64];
  char datapreview[1024];
  char totalfilled[64];
  char retried[32];
  char retrypasses[32];
  char longestreadtime[32];
} display_output_ccc_type;
extern display_output_ccc_type display_output_ccc;






int read_log_file_ccc(char *log_file);

int read_ddrescue_log_ccc(char *log_file_ccc);

int write_ddrescue_logfile_ccc(char *log_file);

int read_domain_file_ccc(char *domain_file);

int read_domain_add_file_ccc(char *add_file);

int read_ddrescue_domain_ccc(char *domain_file);

int check_log_ccc(void);

int initialize_memory_ccc(void);

int initialize_logfile_memory_ccc(void);

int initialize_domainfile_memory_ccc(void);

void run_gtk_gui_update_ccc (void);

void quit_and_exit_ccc (void);

void clear_mode_ccc (void);

void set_mode_auto_passthrough_ccc (void);

void set_mode_scsi_passthrough_ccc (void);

void set_mode_ata_passthrough_ccc (void);

void set_mode_direct_ccc (void);

void set_mode_ahci_ccc (void);

void set_mode_fillzero_ccc (void);

void set_mode_fillmark_ccc (void);

void set_mode_fillerase_ccc (void);

void set_mode_driveronly_ccc (void);

void clear_source_ccc (void);

void update_mode_ccc (void);

void load_clone_settings_ccc (void);

void update_clone_settings_ccc (void);

void update_clone_button_settings_ccc (void);

void load_advanced_settings_ccc (void);

void update_advanced_settings_ccc (void);

void update_advanced_button_settings_ccc (void);

void reset_current_status_ccc (void);

void test_command_ccc (void);

void test_power_command_ccc (void);

void load_timer_settings_ccc (void);

void update_timer_settings_ccc (void);

void update_timer_button_settings_ccc (void);

void update_gui_status_buttons_ccc (void);

void invoke_soft_reset_ccc (void);

void invoke_hard_reset_ccc (void);

void invoke_hba_reset_ccc (void);

void clear_domain_ccc(void);

int get_and_install_license_ccc (char *private_file, int force_hardlink);

int open_confirmation_dialog_ccc (char *message);

int delete_license_file_ccc(void);

void disable_ports_ccc (void);

int open_ports_dialog_ccc (char *current_ports);

void restore_ports_ccc (void);

void disable_usb_mass_storage_ccc(void);

void restore_usb_mass_storage_ccc(void);

int analyze_drive_ccc(int sections, int extended);

int extract_smart_data_ccc(void);

void load_primary_relay_settings_ccc (void);

void update_primary_relay_settings_ccc (void);

void update_primary_relay_button_settings_ccc (void);

void update_primary_relay_settings_from_buttons_ccc (void);

int dissconnect_usbr1_ccc(void);

int dissconnect_usbd1_ccc(void);

int activate_primary_relay_ccc(void);

int deactivate_primary_relay_ccc(void);

int cycle_primary_relay_ccc(void);

int get_primary_relay_settings_ccc(void);

void set_primary_relay_settings_ccc(int value);

int process_dma_mode_ccc(void);

int set_lun_dialog_ccc (int max_lun);













