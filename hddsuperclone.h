// Copyright (C) 2015-2022 Scott Dwyer.
// You may use/distribute/modify this freely, under the terms of
// the GNU General Public License version 2 or later version.
// This software is distributed WITHOUT ANY WARRANTY.



#define DEFAULT_SECTOR_SIZE 512
#define ADVANCED_SECTOR_SIZE 4096
#define CRASH 256
#define TRANSLATE 257
#define ENABLE_REBUILD_ASSIST 258
#define ENABLE_HBA_RESET 265
#define RUN_SERVERP 266
#define MANUAL_CREATE 267
#define COMMAND_SERVERP 268
#define REBUILD_ASSIST_TEST 269
#define ENABLE_HBA_TEST 270
#define DISABLE_AHCI_IDENTIFY 271

#define MAKELICENSE 259
#define LICENSETYPE 260
#define LICENSEVERSION 261
#define LICENSEPROCESSACTIVE 262
#define SUPERTOOL 263
#define LICENSETIME 264
#define ATAMODE 512
#define SCSIMODE 513
#define TESTSKIP 514
#define TESTSKIPFAST 515
#define TESTSKIPMB 516
#define USB1VENDORID 517
#define USB1PRODUCTID 518
#define DEBUGOUT 519
#define VERBOSEOUT 520


#define NONTRIED    0
#define NONTRIMMED  0x10
#define NONDIVIDED  0x20
#define NONSCRAPED  0x30
#define BAD         0x40
#define BAD_HEAD    0x80
#define FINISHED    0x7f
#define PHASE1      0x0
#define PHASE2      0x2
#define PHASE3      0x6
#define PHASE4      0x8
#define TRIMMING    0x10
#define DIVIDING1   0x20
#define DIVIDING2   0x22
#define SCRAPING    0x30
#define RETRYING    0x40
#define REBUILDASSIST 0X80
#define FILLING     0x100
#define STATUS_MASK 0xff
#define BAD_HEAD_STATUS  0x80
#define EXPORT_STATUS_MASK    0x70
#define HALF_MASK   0xffffffff
#define INFO_MASK   0xffffff00
#define SKIP_INFO_MASK   0xff00
#define SKIP_RESET_MASK   0xffffffffffff00ff
#define ADD_STATUS_MASK   0xffffffffffffff00
#define FULL_MASK   0xffffffffffffffff
#define BAD_HEAD_MASK   0xffffffffffffff7f
#define FORWARD_SKIP 0x80
#define REVERSE_SKIP 0xc0
#define DIVIDE_VALUE 8
#define DIVIDE1_VALUE 4
#define DIVIDE2_VALUE 16

#define DISPLAY_LINE_COUNT 18
#define BUFFER_DISPLAY_COUNT 3
#define DISPLAY_BUFFER_SIZE 512    // this needs to be a minumum of BUFFER_DISPLAY_COUNT * 16
#define DISPLAY_UPDATE_TIME 1000    // ms
#define LOG_UPDATE_TIME 30         // sec
#define MIN_SKIP_SIZE 0x1000     // 2MiB  not used?
#define MAX_SKIP_SIZE 0x4000000     // 67,108,864 32GiB
#define DEFAULT_SKIP_SIZE 0x1000     // 4096 2MiB
#define DEFAULT_CLUSTER_SIZE 128
#define DEFAULT_BLOCK_SIZE 1
#define SKIP_MULTI1 750
#define SKIP_MULTI2 1250
#define SKIP_RUN_COUNT 5
#define SKIP_RUN_COUNT_FAST 3
#define SKIP_RUN_RETARD_COUNT 0x1f
#define SKIP_RUN_RETARD_COUNT_FAST 0xf
#define FORWARD 0
#define REVERSE 1
#define FAKE_BAD 0x00000
#define RATE_SKIP_COUNT 2
#define EXIT_ON_SLOW_COUNT 2
#define DEFAULT_RATE_SKIP 50000    // B/s
#define DEFAULT_SKIP_TIMEOUT 1000    // ms
#define ALTERNATE_SKIP_TIMEOUT 300000 // ms, used for exit on slow option
#define DEFAULT_EXIT_ON_SLOW -1000
#define HEAD_PATTERN_CHECK_COUNT 3
#define HEAD_AVERAGE_COUNT 6
#define SCSI_WRITE_TIMEOUT 2000    // ms

#define DRIVER_FILE_NAME "hddscbd"



bool forced_exit_ccc = false;
void cleanup_ccc(void);

void help_ccc(void);
void version_ccc(void);

char *debugfile_ccc = "hddsuperclone_debug.log";

char full_command_line_ccc[1024] = "";
int log_rows_ccc = 1000;
int domain_rows_ccc = 1000;
int total_lines_ccc;
int domain_lines_ccc;
long long *lposition_ccc;
long long *lsize_ccc;
long long *lstatus_ccc;
long long *temp_lposition_ccc;
long long *temp_lsize_ccc;
long long *temp_lstatus_ccc;
long long *dposition_ccc;
long long *dsize_ccc;
long long *dstatus_ccc;
long long *temp_dposition_ccc;
long long *temp_dsize_ccc;
long long *temp_dstatus_ccc;
long long current_position_ccc;
long long current_status_ccc;
int domain_status_ccc;
long long total_domain_size_ccc = 0;
char current_status_string_ccc[64] = "";
bool exportdd_ccc = false;
bool regular_file_ccc = true;
bool no_log_backup_ccc = false;
bool disable_log_backup_ccc = false;
char current_model_ccc[41] = "";
char current_serial_ccc[21] = "";
bool ata_identify_success_ccc = false;
extern bool ncq_supported_ccc; // = false;    // word 76 bit 8
extern bool rebuild_assist_supported_ccc; // = false;    // word 78 bit 11
bool lba_supported_ccc = true;
bool drive_locked_ccc = false;
bool smart_supported_ccc = false;
bool smart_enabled_ccc = false;
bool scsi_read16_supported_ccc = true;
bool scsi_read12_supported_ccc = true;
bool scsi_read10_supported_ccc = true;
bool scsi_read6_supported_ccc = true;
bool scsi_write16_supported_ccc = true;
bool scsi_write12_supported_ccc = true;
bool scsi_write10_supported_ccc = true;
bool scsi_write6_supported_ccc = true;
bool scsi_write_checked_ccc = false;
long long input_offset_ccc = 0;
long long output_offset_ccc = -1;
long long read_size_ccc = -1;
int error_count_ccc = 0;
int nontried_count_ccc = 0;
int nontrimmed_count_ccc = 0;
int nondivided_count_ccc = 0;
int nonscraped_count_ccc = 0;
int finished_count_ccc = 0;
int bad_head_count_ccc = 0;
int other_count_ccc = 0;
long long error_size_ccc = 0;
long long nontried_size_ccc = 0;
long long nontrimmed_size_ccc = 0;
long long nondivided_size_ccc = 0;
long long nonscraped_size_ccc = 0;
long long finished_size_ccc = 0;
long long bad_head_size_ccc = 0;
long long other_size_ccc = 0;
long long total_size_ccc = 0;
long long retry_error_size_ccc = 0;
int derror_count_ccc = 0;
int dnontried_count_ccc = 0;
int dnontrimmed_count_ccc = 0;
int dnondivided_count_ccc = 0;
int dnonscraped_count_ccc = 0;
int dfinished_count_ccc = 0;
int dother_count_ccc = 0;
long long derror_size_ccc = 0;
long long dnontried_size_ccc = 0;
long long dnontrimmed_size_ccc = 0;
long long dnondivided_size_ccc = 0;
long long dnonscraped_size_ccc = 0;
long long dfinished_size_ccc = 0;
long long dother_size_ccc = 0;
bool no_phase1_ccc = false;
bool no_phase2_ccc = false;
bool no_phase3_ccc = false;
bool no_phase4_ccc = false;
bool no_trim_ccc = false;
bool no_scrape_ccc = false;
bool no_divide1_ccc = false;
bool do_divide2_ccc = false;
bool read_bad_head_ccc = true;
bool rebuild_assist_ccc = true;
extern int rebuild_assist_test_ccc; // = -1;
bool mark_bad_head_ccc = false;
bool reverse_ccc = false;
int retries_ccc = 0;
int retry_count_ccc = 0;
long long retried_lba_count_ccc = 0;
bool reset_log_ccc = false;
int sector_size_ccc = DEFAULT_SECTOR_SIZE;
int cluster_size_ccc = DEFAULT_CLUSTER_SIZE;
int original_cluster_size_ccc = DEFAULT_CLUSTER_SIZE;
int block_size_ccc = DEFAULT_BLOCK_SIZE;
int block_offset_ccc = 0;
long long start_position_ccc = 0;
long long end_position_ccc = 0;
long long last_display_update_ccc = 0;
long long last_display_status_update_ccc = 0;
long long last_log_write_ccc = 0;
long long last_domain_write_ccc = 0;
bool device_fault_ccc = false;
long long test_skip_mb_ccc = 0;
long long min_skip_size_ccc = -1;
long long original_min_skip_size_ccc = 0;
long long max_skip_size_ccc = -1;
long long original_max_skip_size_ccc = 0;
long long skip_size_ccc = 0;
long long skip_history_ccc[5] = {0, 0, 0, 0, 0};
int skip_count_ccc = 0;
int total_skip_count_ccc = 0;
int total_slow_skips_ccc = 0;
long long last_skip_position_ccc = 0;
long long last_skip_jump_position_ccc = 0;
long long last_skip_size_ccc = 0;
bool skip_ccc = true;
long long copy_start_time_ccc = 0;
long long last_finished_value_ccc = 0;
long long starting_finished_value_ccc = 0;
bool skip_fast_ccc = false;
long long skip_multiplier_ccc = 0;
long long skip_mod_ccc = 0;
bool skipping_ccc = false;
long long highest_skip_ccc = 0;
int skip_run_count_ccc = 0;
int skip_run_limit_ccc = SKIP_RUN_COUNT;
int skip_run_retard_count_ccc = SKIP_RUN_RETARD_COUNT;
long long skip_position_ccc[256];
int total_skip_runs_ccc = 0;
int total_skip_resets_ccc = 0;
long long total_skip_size_ccc = 0;
long long last_total_skip_size_ccc = 0;
long long skip_run_start_ccc = 0;
int direction_ccc = FORWARD;
long long rate_history_ccc[256];
long long modified_rate_history_ccc[256];
long long rate_modifier_ccc = 0;
int rate_count_ccc = 0;
int rate_history_count_ccc = 0;
int rate_skip_count_ccc = 0;
long long rate_skip_ccc = -1000;
int exit_on_slow_count_ccc = 0;
long long exit_on_slow_timer_ccc = 0;
long long exit_on_slow_ccc = -1000;
long long exit_slow_time_ccc = -1000;
long long skip_timeout_ccc = -1000;
bool skip_time_triggered_ccc = false;
long long longest_read_time_ccc = 0;
long long current_read_time_ccc = 0;
long long recent_longest_read_time_ccc = 0;
long long max_longest_read_time_ccc = 0;
long long read_time_history_ccc[256];
int read_time_history_count_ccc = 0;
long long max_read_rate_ccc = 0;
long long last_read_start_time_ccc = 0;
long long last_read_size_ccc = 0;
char display_buffer_ccc[DISPLAY_BUFFER_SIZE];
bool force_mounted_ccc = false;
bool fill_mark_ccc = false;
bool fill_erase_ccc = false;
bool open_source_ccc = true;
long long total_filled_ccc = 0;
long long fill_size_ccc = 0;
long long last_filled_value_ccc = 0;
bool limit_recovery_ccc = false;
int recovery_past_limit_ccc = 0;
long long total_runtime_ccc = 0;
bool enable_output_offset_ccc = false;
bool enable_current_position_ccc = false;
bool enable_retry_connecting_ccc = false;
extern bool enable_scsi_write_ccc;
bool enable_logfile_phase_ccc = false;
long long last_end_position_ccc = 0;
int driver_readahead_multiplier_ccc = 1;
bool enable_output_sector_size_ccc = false;
long long output_sector_size_adjustment_ccc = 0;
bool use_physical_sector_size_for_virtual_ccc = false;
long long driver_minimum_cluster_size_ccc = 1;
struct
{
  long long total_cylinders;
  long long total_heads;
  long long sectors_per_track;
  long long cylinder;
  long long head;
  long long sector;
} chs_ccc;
long long *head_map_position_ccc;
long long *head_map_size_ccc;
long long *head_map_final_ccc;
int *head_map_head_ccc;
int *head_map_status_ccc;
long long *temp_head_map_position_ccc;
long long *temp_head_map_size_ccc;
long long *temp_head_map_final_ccc;
int *temp_head_map_head_ccc;
int *temp_head_map_status_ccc;
int head_map_rows_ccc = 1000;
int head_map_line_ccc = 0;

struct
{
  bool sct_error_recovery_control;    // word 206 bit 3
  bool ncq_supported;    // word 76 bit 8
  bool rebuild_assist_supported;    // word 78 bit 11
  bool extended_support;    // word 83 bit 10
  bool lba_supported;    // word 49 bit 9
  bool drive_locked;    // word 128 bit 2
  bool smart_supported;    // word 82 bit 0
  bool smart_enabled;    // word 85 bit 0
  bool hpa_supported;    // word 82 bit 10
  bool dco_supported;    // word 83 bit 11
  bool multiword_dma_mode0_supported;    // word 63 bit 0
  bool multiword_dma_mode1_supported;    // word 63 bit 1
  bool multiword_dma_mode2_supported;    // word 63 bit 2
  bool ultra_dma_mode0_supported;    // word 88 bit 0
  bool ultra_dma_mode1_supported;    // word 88 bit 1
  bool ultra_dma_mode2_supported;    // word 88 bit 2
  bool ultra_dma_mode3_supported;    // word 88 bit 3
  bool ultra_dma_mode4_supported;    // word 88 bit 4
  bool ultra_dma_mode5_supported;    // word 88 bit 5
  bool ultra_dma_mode6_supported;    // word 88 bit 6
  bool multiword_dma_mode0_selected;    // word 63 bit 8
  bool multiword_dma_mode1_selected;    // word 63 bit 9
  bool multiword_dma_mode2_selected;    // word 63 bit 10
  bool ultra_dma_mode0_selected;    // word 88 bit 8
  bool ultra_dma_mode1_selected;    // word 88 bit 9
  bool ultra_dma_mode2_selected;    // word 88 bit 10
  bool ultra_dma_mode3_selected;    // word 88 bit 11
  bool ultra_dma_mode4_selected;    // word 88 bit 12
  bool ultra_dma_mode5_selected;    // word 88 bit 13
  bool ultra_dma_mode6_selected;    // word 88 bit 14
} identify_flags_ccc;



int do_command_line_ccc(void);

int process_arguments_ccc(void);

int increase_log_memory_ccc(int new_lines);

int increase_domain_memory_ccc(int new_lines);

int check_domain_ccc(void);

int get_log_status_ccc(long long mask);

int get_domain_status_ccc(long long mask);

int upline_ccc(int count);

int delete_line_ccc(int line);

int delete_domain_line_ccc(int line);

int insert_line_ccc(int line, long long position, long long size, long long status);

int insert_domain_line_ccc(int line, long long position, long long size, long long status);

int find_block_ccc(long long position);

int find_domain_block_ccc(long long position);

int find_next_block_ccc(int line, long long status, long long mask);

int find_previous_block_ccc(int line, long long status, long long mask);

int change_chunk_status_ccc(long long position, long long size, long long status, long long mask);

int add_to_domain_ccc(long long position, long long size);

int ata_28_pio_read_ccc(unsigned char *reg);

int clone_forward_ccc(void);

int clone_reverse_ccc(void);

int copy_forward_ccc(int status_type, int status_mask, int new_status_type);

int copy_reverse_ccc(int status_type, int status_mask, int new_status_type);

int trim_forward_ccc(int status_type, int status_mask, int new_status_type);

int trim_reverse_ccc(int status_type, int status_mask, int new_status_type);

int phase1_ccc(void);

int phase2_ccc(void);

int phase3_ccc(void);

int trimming_ccc(void);

int scraping_ccc(void);

int retrying_ccc(void);

int read_chunk_ccc(long long output_position, int size);

int write_chunk_ccc(long long position, int size);

int read_ata_dma_ccc(long long position, int size, int rarc_bit);

int read_ata_pio_ccc(long long position, int size);

int scsi_read_ccc(long long position, int size);

int scsi_write_ccc(long long position, int size);

int scsi_check_read_commands_ccc(void);

long long set_additional_status_ccc(int retstat, int skip_info);

long long get_timing_byte_ccc(void);

int process_chunk_ccc(int new_status_type, int retstat, int rsize, int skip_info);

int process_rebuild_assist_chunk_ccc(int status_type, int new_status_type, int retstat, int rsize, int skip_info);

int process_domain_ccc(long long position, int size, int status, int status_mask);

int domain_size_adjustment_ccc(int original_size, int requested_size);

int process_skip_ccc(void);

int reset_skip_ccc(void);

int reset_rate_ccc(void);

int process_bad_head_ccc(void);

int process_skip_run_ccc(void);

int check_device_ccc(void);

char *host_error_code_ccc(int code);

int do_fill_ccc(int status, long long mask);

long long block_align_ccc (long long value, int offset);

long long block_align_up_ccc (long long value);

int call_command_on_error_ccc(void);

int process_chs_ccc(long long position);

void set_soft_timer_ccc (int status);

int write_logfile_phase_ccc(char *phase);

int driver_clone_forward_ccc(long long start, long long small_end, long long big_end);

int initialize_head_map_memory_ccc(void);

int increase_head_map_memory_ccc(int new_lines);

int rebuild_assist_check_head_ccc (int head);

int rebuild_assist_remove_head_ccc (int head);

int rebuild_assist_disable_head_ccc (int head);

int rebuild_assist_get_next_head_ccc (int head);

int enable_rebuild_assist_ccc(void);

int disable_rebuild_assist_ccc(void);

int rebuild_assist_get_number_of_heads_ccc (void);

int get_asc_from_additional_sense_ccc(int value);

int get_ascq_from_additional_sense_ccc(int value);

int get_flag_data_from_identify_ccc (unsigned int wordnumber, unsigned int lowbit, unsigned int highbit);

int get_highest_multiword_dma_mode_ccc(void);

int get_current_multiword_dma_mode_ccc(void);

int get_highest_ultra_dma_mode_ccc(void);

int get_current_ultra_dma_mode_ccc(void);

void test_skip_ccc(int size);

char *get_smart_attribute_name_ccc(int id);






unsigned char ccc_mac_address_ccc[6];
char *install_directory_ccc = "/usr/local/bin/";
bool path_fail_ccc = false;
char start_working_directory_ccc[PATH_MAX];
char called_name_ccc[PATH_MAX];
char full_exec_path_ccc[PATH_MAX * 2];
bool found_exec_ccc = false;
