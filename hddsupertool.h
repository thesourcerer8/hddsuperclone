// Copyright (C) 2015-2022 Scott Dwyer.
// You may use/distribute/modify this freely, under the terms of
// the GNU General Public License version 2 or later version.
// This software is distributed WITHOUT ANY WARRANTY.


// version 0.1 20141018

#define MAX_ARGUMENTS 16
#define MAX_COMMAND_LENGTH 64
#define CRASH 256
#define REGADDR 257
#define CONADDR 258
#define BUSADDR 259
#define DEVICE 260
#define HBAADDR 261
#define PORTNUM 262
#define PORTADDR 263
#define ATAMODE 264
#define SCSIMODE 265

#define SUPERTOOL 1

#ifdef FULLVERSION
int fullversion_ccc = 1;
#else
int fullversion_ccc = 0;
#endif


extern time_t start_time_ccc, end_time_ccc;

extern struct timeval tvBegin_ccc, tvEnd_ccc, tvDiff_ccc, tvCurrent_ccc;

extern bool forced_exit_ccc;

void cleanup_ccc(void);


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


void help_ccc(void);
void version_ccc(void);

extern char *debugfile_ccc;

extern char *argument_ccc[MAX_ARGUMENTS];
extern unsigned int argument_count_ccc;
extern bool check_only_ccc;
extern bool do_indentation_ccc;
extern bool quiet_ccc;
extern int ccc_indent_ccc;
extern int spaces_ccc;
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
extern unsigned int total_lines_ccc;
extern char *number_variable_name_buffer_ccc;
extern char **number_variable_name_pointer_ccc;
extern unsigned int total_number_variables_ccc;
extern char *string_variable_name_buffer_ccc;
extern char **string_variable_name_pointer_ccc;
extern char *string_variable_buffer_ccc;
extern unsigned int total_string_variables_ccc;
 


int process_arguments_ccc(void);

int initialize_memory_ccc(void);

int increase_number_variable_memory_ccc(void);

int increase_string_variable_memory_ccc(void);

int write_file_ccc(char *script_file);

int process_lines_ccc(void);

int process_command_line_ccc(void);

int set_sense_buffer_ccc(void);

int create_default_variables_ccc(void);

long long math_ccc(char *raw_value);


#ifdef FULLVERSION
unsigned char sletter_ccc[25];
unsigned char ccc_mac_address_ccc[6];
char *install_directory_ccc = "/usr/local/bin/";
char *license_file_name_ccc = "/usr/local/etc/hddsupertool.lic";
unsigned int expire_day_ccc;
int days_remaining_ccc = 0;
char default_sbytes_ccc[16] = {126, 28, 237, 191, 14, 118, 223, 135, 59, 111, 195, 157, 183, 225, 206, 27};
bool path_fail_ccc = false;
char start_working_directory_ccc[PATH_MAX];
char called_name_ccc[PATH_MAX];
char full_exec_path_ccc[PATH_MAX * 2];
bool found_exec_ccc = false;
unsigned char license_information_ccc = 0;
int compare_mac_ccc = 0;
int license_file_checksum_ccc = 0;
int check_checksum_ccc = 0;
bool full_license_ccc = false;

int process_serial_ccc(char *serial);

int decode_serial_ccc(char *serial);

int check_serial_ccc(void);

int make_serial_file_ccc(void);

int check_license_file_ccc(void);

int get_mac_address_ccc(void);

int print_license_ccc(int show);

#endif


