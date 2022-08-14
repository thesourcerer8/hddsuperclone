// Copyright (C) 2015-2022 Scott Dwyer.
// You may use/distribute/modify this freely, under the terms of
// the GNU General Public License version 2 or later version.
// This software is distributed WITHOUT ANY WARRANTY.


extern unsigned int script_rows_ccc;
extern unsigned int number_variable_rows_ccc;
extern unsigned int string_variable_rows_ccc;
extern char *menufile1_ccc;
extern char *menufile2_ccc;
extern char *menufile3_ccc;
extern char script_directory1_ccc[PATH_MAX];
extern char script_directory2_ccc[PATH_MAX];
extern char script_directory3_ccc[PATH_MAX];
extern char script_directory4_ccc[PATH_MAX];
extern char *current_script_directory_ccc;
extern char full_script_path_ccc[PATH_MAX];
extern char *script_line_buffer_ccc;
extern char **script_line_pointer_ccc;
extern unsigned int total_script_lines_ccc;
extern char *number_variable_name_buffer_ccc;
extern char **number_variable_name_pointer_ccc;
extern unsigned int total_number_variables_ccc;
extern char *string_variable_name_buffer_ccc;
extern char **string_variable_name_pointer_ccc;
extern char *string_variable_buffer_ccc;
extern unsigned int total_string_variables_ccc;
 


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












