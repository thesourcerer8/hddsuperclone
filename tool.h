// Copyright (C) 2015-2022 Scott Dwyer.
// You may use/distribute/modify this freely, under the terms of
// the GNU General Public License version 2 or later version.
// This software is distributed WITHOUT ANY WARRANTY.


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
char **script_line_pointer_ccc;
unsigned int total_script_lines_ccc;
char *number_variable_name_buffer_ccc;
char **number_variable_name_pointer_ccc;
unsigned int total_number_variables_ccc;
char *string_variable_name_buffer_ccc;
char **string_variable_name_pointer_ccc;
char *string_variable_buffer_ccc;
unsigned int total_string_variables_ccc;



void supertool_cleanup_ccc(void);

int process_arguments_ccc(void);

int initialize_tool_memory_ccc(void);

int increase_number_variable_memory_ccc(void);

int increase_string_variable_memory_ccc(void);

int write_file_ccc(char *script_file);

int process_lines_ccc(void);

int process_command_line_ccc(void);

int set_sense_buffer_ccc(void);

int create_default_variables_ccc(void);

long long math_ccc(char *raw_value);












