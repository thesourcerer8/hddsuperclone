// Copyright (C) 2015-2022 Scott Dwyer.
// You may use/distribute/modify this freely, under the terms of
// the GNU General Public License version 2 or later version.
// This software is distributed WITHOUT ANY WARRANTY.



#define MAX_SUB_LEVEL 256
#define MAX_ENDIF_LEVEL 256
#define MAX_WHILE_LEVEL 256

unsigned int ccc_subroutine_stack_ccc[MAX_SUB_LEVEL];
unsigned int subroutine_while_stack_ccc[MAX_SUB_LEVEL];
unsigned int subroutine_endif_stack_ccc[MAX_SUB_LEVEL];
int subroutine_stack_counter_ccc = 0;
int subroutine_count_ccc = 0;
int return_count_ccc = 0;
unsigned int ccc_endif_stack_ccc[MAX_ENDIF_LEVEL];
int endif_stack_counter_ccc = 0;
bool statement_condition_ccc[MAX_ENDIF_LEVEL];
unsigned int ccc_while_stack_ccc[MAX_WHILE_LEVEL];
unsigned int while_endif_stack_ccc[MAX_WHILE_LEVEL];
unsigned int done_stack_ccc[MAX_WHILE_LEVEL];
bool while_condition_ccc[MAX_WHILE_LEVEL];
int while_stack_counter_ccc = 0;
bool setting_buffer_ccc = false;
bool setting_scratchpad_ccc = false;
bool setting_usbbuffer_ccc = false;
char script_list_ccc[MAX_SCRIPT_DEPTH][MAX_LINE_LENGTH];
int script_count_ccc = 0;



int echo_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int seti_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int sets_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int printbuffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int setmainbuffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int endmainbuffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int change_main_buffer_size_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int print_sense_buffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int write_buffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int read_buffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int set_direction_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int scsi_6_cmd_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int scsi_10_cmd_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int scsi_12_cmd_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int scsi_16_cmd_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int set_protocol_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int set_tlength_field_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int set_byte_block_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int set_check_condition_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int ata_28_cmd_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int ata_48_cmd_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int write_log_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int get_time_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int string_to_buffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int setup_pio_read_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int setup_dma_read_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int setup_pio_write_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int setup_dma_write_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int get_status_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int perform_soft_reset_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int perform_hard_reset_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int u_sleep_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int busytimeout_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int resettimeout_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int softtimeout_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int hardtimeout_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int generaltimeout_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int load_script_file_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int previous_script_file_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int include_script_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int string_to_scratchpad_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int wordflip_buffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int wordflip_scratchpad_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int get_file_size_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int write_scratchpad_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int read_scratchpad_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int delete_file_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int call_command_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int user_input_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int copy_buffer_to_scratch_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int copy_scratch_to_buffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int variable_check_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int printscratchpad_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int setmainscratchpad_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int endmainscratchpad_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int change_main_scratchpad_size_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int gosub_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int ccc_subroutine_ccc(bool perform_check, unsigned int line_number);

int endsubroutine_ccc(bool perform_check, unsigned int line_number);

int returnsub_ccc(bool perform_check, unsigned int line_number);

int ifstate_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int ccc_endif_ccc(bool perform_check, unsigned int line_number);

int elsestate_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int ccc_elseif_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int dowhile_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int ccc_done_ccc(bool perform_check, unsigned int line_number);

int dobreak_ccc(bool perform_check, unsigned int line_number);

int exit_with_code_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);



int string_to_usbbuffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int wordflip_usbbuffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int write_usbbuffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int read_usbbuffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int print_usbbuffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int set_usbbuffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int end_usbbuffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int change_usbbuffer_size_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int copy_usbbuffer_to_buffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int copy_buffer_to_usbbuffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int copy_usbbuffer_to_scratch_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int copy_scratch_to_usbbuffer_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int usbtimeout_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int send_usb_control_msg_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int usb_raw_read_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int usb_raw_write_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);

int usb_reset_ccc(bool perform_check, unsigned int line_number, char *rest_of_line);


















