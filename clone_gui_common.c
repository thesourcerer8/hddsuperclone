// Copyright (C) 2015-2022 Scott Dwyer.
// You may use/distribute/modify this freely, under the terms of
// the GNU General Public License version 2 or later version.
// This software is distributed WITHOUT ANY WARRANTY.


#include <stdbool.h>

// char *title_ccc; -> io.h
extern char *version_number_ccc;
char *copyright_year_ccc;
extern char curlang_ccc[LANGCOUNT][MAXLANGLENGTH];
char newlang_ccc[LANGCOUNT][MAXLANGLENGTH];
int translate_ccc;
char *log_file_ccc;
char *ddilog_file_ccc;
char *ddelog_file_ccc;
char *domain_file_ccc;
int check_log_size_ccc;
bool do_domain_ccc;
bool create_domain_ccc;
bool domain_dd_ccc;
bool importdd_ccc;
char level4_middle_text_ccc[4096];
int running_clone_ccc;
char command_to_call_ccc[MAX_CALL_LENGTH];
char power_command_to_call_ccc[MAX_CALL_LENGTH];
bool stop_on_error_ccc;
bool do_call_command_ccc;
bool activate_primary_relay_on_error_ccc;
bool stop_on_power_cycle_ccc;
bool call_power_command_ccc;
bool power_cycle_primary_relay_ccc;
//bool check_message_ccc; -> io.h
bool write_buffer_disable_ccc;
bool write_buffer_enable_ccc;
bool driver_return_error_ccc;
bool driver_return_zeros_ccc;
bool driver_return_marked_ccc;
int memory_used_ccc;
bool data_read_from_log_ccc;
int driver_mode_ccc;
extern int driver_running_ccc;
//char virtual_driver_name_ccc[MAX_DEVICE_NAME_LENGTH]; -> io.h
long long last_small_read_size_ccc;
long long last_big_read_size_ccc;
long long last_actual_read_size_ccc;
long long driver_read_rate_size_ccc;
bool no_io_error_ccc;
char new_ports_ccc[256];
int running_analyze_ccc;
long long analyze_stoptime_ccc;
char analyze_text_ccc[MAX_ANALYZE_TEXT_LENGTH];
int analyze_read_attempts_ccc[MAXANALYZESECTIONS];
int analyze_good_reads_ccc[MAXANALYZESECTIONS];
int analyze_bad_reads_ccc[MAXANALYZESECTIONS];
int analyze_slow_reads_ccc[MAXANALYZESECTIONS];
int analyze_timeouts_ccc[MAXANALYZESECTIONS];
long long analyze_read_time_ccc[MAXANALYZESECTIONS];
long long analyze_low_time_ccc[MAXANALYZESECTIONS];
long long analyze_high_time_ccc[MAXANALYZESECTIONS];
bool enable_extended_analyze_ccc;
long long analyze_slow_position_ccc[MAXANALYZESLOW];
long long analyze_slow_size_ccc[MAXANALYZESLOW];
long long analyze_slow_low_ccc[MAXANALYZESLOW];
long long analyze_slow_high_ccc[MAXANALYZESLOW];
long long analyze_slow_section_ccc[MAXANALYZESLOW];
int analyze_slow_total_reads_ccc;
long long skip_timeout_copy_ccc;
int cluster_size_copy_ccc;
char smart_data_text_ccc[MAX_SMART_DATA_TEXT_LENGTH];
bool aggressive_driver_ccc;
bool color_statusbar_ccc;
extern char primary_relay_name_ccc[MAX_RELAY_NAME_LENGTH];
extern unsigned long long primary_relay_activation_time_ccc;
extern unsigned long long primary_relay_delay_time_ccc;
extern bool rebuild_assist_enabled_ccc;
extern bool use_fpdma_ccc;
extern bool wait_for_ds_bit_ccc;
char identify_device_raw_text_ccc[65536];

clone_settings_ccc_type clone_settings_ccc;
advanced_settings_ccc_type advanced_settings_ccc;
timer_settings_ccc_type timer_settings_ccc;
extern primary_relay_settings_ccc_type primary_relay_settings_ccc;
display_output_ccc_type display_output_ccc;


