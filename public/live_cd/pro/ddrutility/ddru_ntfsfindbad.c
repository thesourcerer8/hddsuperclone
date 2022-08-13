/*
Copyright (C) 2016 Scott Dwyer

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "ddru_ntfscommon.h"
#include "ddru_ntfsfindbad_help.h"


char *title = "ddru_ntfsfindbad";
char *version_number = "1.5 20150109";
int copyright_year = 2015;

// Function to handle ctrl-c
void signal_callback_handler(int signum)
{
  fprintf(stderr, "Terminated by user\n");
  exit(signum);
}

// time functions
/* Return 1 if the difference is negative, otherwise 0.  */
int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
  long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
  result->tv_sec = diff / 1000000;
  result->tv_usec = diff % 1000000;

  return (diff<0);
}

void timeval_print(struct timeval *tv)
{
  char buffer[30];
  time_t curtime;

  // printf("%ld.%06ld", tv->tv_sec, tv->tv_usec);
  curtime = tv->tv_sec;
  strftime(buffer, 30, "%m-%d-%Y  %T", localtime(&curtime));
  printf("%s.%06ld\n", buffer, tv->tv_usec);
}


long last_second = 0;
long last_usecond = 0;
int verbose = 0;
int debug = 0;
void help(void);
void version(void);
char *source_log_file;
char *outputfile = "ntfsfindbad.log";
char *source_disk;
char *debugfile = "ntfsfindbad_debug.log";

unsigned long *master_inode_buf;
unsigned long *master_parent_buf;
unsigned long *master_attrib_buf;
char *master_file_name_buf;
char **master_file_name_pt;
char clusters_per_record;
unsigned long sectors_per_record;
unsigned long master_list_count;
unsigned long long cluster_size;
unsigned long long input_offset;
unsigned long long data_size;
unsigned long long partition_offset = 0;
unsigned long long mft_part_offset[255];
unsigned long long mft_part_size[255];
unsigned long mft_part_number;
unsigned long long inode_part_offset[8][255];
unsigned long long inode_part_size[8][255];
unsigned long inode_part_number[8];
unsigned char inode_data_type[8];
unsigned long long mft_real_size;
unsigned long long total_inodes;
unsigned long output_lines;
unsigned long total_lines;
long long total_size;
FILE *debug_file;
FILE *output_file;
FILE *writefile;
bool has_name;
bool process_mft;
int return_value;
unsigned int has_data;
unsigned int mft_hard_errors = 0;
unsigned int mft_soft_errors = 0;
unsigned int deleted_inodes = 0;
unsigned long long *position;
unsigned long long *size;
unsigned char *type;
unsigned long *inode_count_buf;
unsigned long *error_count_buf;
unsigned long long *error_size_buf;
char *inode_type_buf;
char *file_name_buf;
char **file_name_pt;

int read_boot_sec_file(void);

int read_mft_entry (FILE *readfile, unsigned long long input_offset, unsigned long record_number);

int processdata(unsigned long offset);

int read_file(char *log_file);

int process_boot_sector(void);

int process_entry(unsigned long record_number);

int find_errors(void);

int process_list(void);



int main (int argc, char **argv)
{
  // Register ctrl-c signal and signal handler
  signal(SIGINT, signal_callback_handler);

  // set start_time
  gettimeofday(&tvBegin, NULL);
  //printf ("ntfsreadfile start time = ");
  //timeval_print(&tvBegin);

  inode_count = 0;
  no_convert = false;
  outtype = "UTF-8";

  // begin processing command line arguments
  int command_line_argument;
  int arguments_required;	// required number of non-option arguments
  bool command_line_error = false;	// initialize error to false

  while (1)
  {
    static struct option long_options[] =
    {
      {"help",    	no_argument,	0, 'h'},
      {"version", 	no_argument,	0, 'v'},
      {"debug",  	no_argument,	0, 'D'},
      {"verbose",  	no_argument,	0, 'V'},
      {"encoding",    	required_argument, 0, 'e'},
      {"inputoffset",    	required_argument, 0, 'i'},
      {"noconvert",  	no_argument,	0, 'n'},
      {0, 0, 0, 0}
    };
    // getopt_long stores the option index here.
    int option_index = 0;

    command_line_argument = getopt_long (argc, argv, "hvDVe:i:n",
					 long_options, &option_index);

    // Detect the end of the options.
    if (command_line_argument == -1)
      break;

    switch (command_line_argument)
    {
      case 'h':
	help();
	exit(0);

      case 'v':
	version();
	exit(0);

      case 'D':
	debug++;
	break;

      case 'V':
	verbose++;
	break;

      case 'e':
	outtype = optarg;
	break;

      case 'i':
	partition_offset = atoll(optarg);
	break;

      case 'n':
	no_convert = true;
	break;

      case '?':
	// getopt_long already printed an error message.
	command_line_error = true;
	break;

      default:
	fprintf (stderr, "Error processing command line options\n");
	abort ();
    }
  }

  arguments_required = 2;
  if ((argc - optind) != arguments_required)
  {
    command_line_error = true;
    if ((argc - optind) < arguments_required)
      fprintf (stderr, "Error: too few arguments\n");
    if ((argc - optind) > arguments_required)
      fprintf (stderr, "Error: too many arguments\n");
  }

  // get remaining arguments that are not options
  source_disk = argv[optind++];
  source_log_file = argv[optind++];



  // exit on command line error
  if (command_line_error)
  {
    fprintf (stderr, "ddru_ntfsfindbad: Both source and logfile must be specified.\n");
    fprintf (stderr, "Try 'ddru_ntfsfindbad --help' for more information.\n");
    exit (3);
  }

  //check if debug is turned on and open debug file if it is
  if (debug > 0)
  {
    debug_file = fopen(debugfile, "w");
    if (debug_file == NULL)
    {
      fprintf(stderr, "Cannot open %s for writing (%s).\nAborting...\n", debugfile, strerror(errno));
      exit (4);
    }
  }

  if (debug > 0)
  {
    fprintf (debug_file, "ntfs_findbad debug file created by test at ");
    time_t mytime;
    mytime = time(NULL);
    fprintf (debug_file, "%s", ctime(&mytime));
    fprintf (debug_file, "source_disk = %s\n", source_disk);
    fprintf (debug_file, "source_log_file = %s\n", source_log_file);
  }

  // end of command line processing



  fprintf (stdout, "%s %s\n", title, version_number);

  // read the logfile into memory
  return_value = read_file(source_log_file);
  if (return_value > 0)
  {
    fprintf (stdout, "ERROR! Unable to correctly process the logfile\n");
    exit (1);
  }

  // read the boot sector file into memory structure for processing
  return_value = read_boot_sec_file();
  if (return_value > 0)
  {
    fprintf (stdout, "ERROR! Unable to correctly process the partition boot sector\n");
    exit (1);
  }



  // setup conversion stuff
  if (no_convert == false)
  {
    cd = iconv_open (outtype, "UTF-16LE");
    if (cd == (iconv_t) -1)
    {
      /* Something went wrong.  */
      if (errno == EINVAL)
	fprintf (stderr, "conversion from UTF-16LE to '%s' not available\n", outtype);
      else
	perror ("iconv_open");
      exit (1);
    }
    // calloc clears the memory so it will end up null terminated
    inchar = calloc(512, sizeof(char));
    converted = calloc(1024, sizeof(char));
    // need spare pointer to converted as iconv will change original
    converted_start = converted;
    inchar_start = inchar;
  }


  // process boot sector and read mft inode into a list of data runs
  process_mft = true;
  return_value = process_boot_sector();
  if (return_value > 0)
  {
    fprintf (stdout, "ERROR! Unable to correctly process the mft from the partition boot sector\n");
    exit (1);
  }

  total_inodes = total_size / mft_record_size;

  if (verbose > 0)
  {
    fprintf (stdout, "total mft fragments=%ld\n", mft_part_number+1);
    fprintf (stdout, "total mft size=%lld bytes\n", total_size);
    fprintf (stdout, "total inodes=%lld\n", total_inodes);
  }

  if (verbose > 2)
  {
    unsigned int i;
    for (i = 0; i < mft_part_number+1; i++)
    {
      fprintf (stdout, "mftpart=%d, offset=0x%llX, fulloffset=0x%llX, size=0x%llX\n", i, mft_part_offset[i], mft_part_offset[i] + partition_offset, mft_part_size[i]);
    }
  }

  // compare log with mft entries and produce an log of which files have errors
  return_value = find_errors();
  if (return_value > 0)
  {
    fprintf (stderr, "\nError while trying to produce error log\n");
    exit (1);
  }



  if (verbose == 1)
  {
    fprintf (stdout, "MFT hard errors=%d\n", mft_hard_errors);
  }

  if (verbose > 1)
  {
    fprintf (stdout, "MFT hard errors=%d\n", mft_hard_errors);
    fprintf (stdout, "MFT soft errors=%d\n", mft_soft_errors);
    fprintf (stdout, "Skipped deleted inodes=%d\n", deleted_inodes);
  }



  // process the list to get full path names
  process_list();



  // final cleanup at end of main
  if (debug > 0)
    fclose(debug_file);

  free (position);
  free (size);
  free (type);
  free (master_attrib_buf);
  free (master_file_name_buf);
  free (master_file_name_pt);
  free (master_inode_buf);
  free (master_parent_buf);
  free (inode_count_buf);
  free (error_size_buf);
  free (inode_type_buf);
  free (error_count_buf);
  free (file_name_buf);
  free (file_name_pt);

  // print time elapsed
    gettimeofday(&tvEnd, NULL);
    //printf ("ntfsreadfile end time = ");
    //timeval_print(&tvEnd);

    // diff
    timeval_subtract(&tvDiff, &tvEnd, &tvBegin);
    fprintf (stdout, "ddru_ntfsfindbad took %ld.%06ld seconds to complete\n", tvDiff.tv_sec, tvDiff.tv_usec);

    exit (0);

}
// end of main
//******************************************************




// function to read the logfile into memory
int read_file(char *log_file)
{
  fprintf (stdout, "Reading the logfile into memory...\n");
  unsigned int rows = 1000;

  position = malloc(rows * sizeof(*position));
  if (position == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (3);
  }

  size = malloc(rows * sizeof(*size));
  if (size == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (3);
  }

  type = malloc(rows * sizeof(*type));
  if (type == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (3);
  }

  FILE *readfile;
  readfile = fopen(log_file, "r");
  if (readfile == NULL)
  {
    fprintf(stderr, "Cannot open %s for reading (%s).\n", source_log_file, strerror(errno));
    return (1);
  }



  char line[255];
  unsigned long i = 0;
  unsigned long n = 1;
  unsigned long errors = 0;
  bool firstline = true;
  int scanline;
  while (fgets(line, sizeof line, readfile))
  {
    // ignore comment line
    if (*line == '#')
    {
      n++;
      continue;
    }

    scanline = sscanf(line, "%llx %llx %c[^\n]", &position[i], &size[i], &type[i]);
    if (scanline != 3)
    {
      // ignore the first line that has the current position
      if (sscanf(line, "%llx %c[^\n]", &position[i], &type[i]) == 2 && firstline)
      {
        firstline = false;
	n++;
      }
      else
      {
	errors++;
	fprintf (stderr, "error reading logfile line %ld    ", n);
	fprintf (stderr, "%llx %llx %c\n", position[i], size[i], type[i]);
	if (debug > 0)
	{
	  fprintf (debug_file, "error reading logfile line %ld    ", n);
	  fprintf (debug_file, "%llx %llx %c\n", position[i], size[i], type[i]);
	}
      }
    }
    // line processed ok
    else
    {
      if (debug > 2)
      {
	fprintf (debug_file, "line %ld %llx %llx %c\n", i+1, position[i], size[i], type[i]);
      }
      i++;
      n++;
    }
    // if used up allocated memory then increase it
    if (i >= rows)
    {
      rows += 1000;
      position = realloc(position, rows * sizeof(*position));
      if (position == NULL)
      {
	fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
	return (3);
      }

      size = realloc(size, rows * sizeof(*size));
      if (size == NULL)
      {
	fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
	return (3);
      }

      type = realloc(type, rows * sizeof(*type));
      if (type == NULL)
      {
	fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
	return (3);
      }
    }
  }
  total_lines = i;

  if (verbose > 0)
  fprintf (stdout, "processed %ld lines out of %ld with %ld errors\n", total_lines, n-1, errors);

  fclose(readfile);
  return (0);
}



// function to read boot sector file into memory
int read_boot_sec_file(void)
{
  FILE *readfile;
  readfile = fopen(source_disk, "rb");
  if (readfile == NULL)
  {
    fprintf(stderr, "Cannot open %s for reading (%s).\n", source_disk, strerror(errno));
    return (1);
  }

  input_offset = partition_offset;
  data_size = 512;
  fprintf (stdout, "Reading partition boot sector...\n");

  if (fseeko(readfile, input_offset, SEEK_SET) != 0 )
  {
    fprintf(stderr, "Error seeking %s (%s).\n", source_disk, strerror(errno));
    return (4);
  }

  if (fread(boot_sector.raw.bootsectfile, 1, data_size, readfile) < data_size )
  {
    fprintf(stderr, "Error reading %s (%s).\n", source_disk, strerror(errno));
    return (4);
  }

  fclose(readfile);

  if (debug > 2)
  {
    fprintf (debug_file, "\nchJumpInstruction = %02X", (unsigned char)boot_sector.items.chJumpInstruction[1]);
    fprintf (debug_file, "%02X", (unsigned char)boot_sector.items.chJumpInstruction[2]);
    fprintf (debug_file, "%02X\n", (unsigned char)boot_sector.items.chJumpInstruction[3]);
    fprintf (debug_file, "chOemID = %s\n", boot_sector.items.chOemID);
    fprintf (debug_file, "chDummy = %02X", (unsigned char)boot_sector.items.chDummy[1]);
    fprintf (debug_file, "%02X", (unsigned char)boot_sector.items.chDummy[2]);
    fprintf (debug_file, "%02X", (unsigned char)boot_sector.items.chDummy[3]);
    fprintf (debug_file, "%02X\n", (unsigned char)boot_sector.items.chDummy[4]);
    fprintf (debug_file, "wBytesPerSec = 0x%04X\n", boot_sector.items.wBytesPerSec);
    fprintf (debug_file, "uchSecPerClust = 0x%02X\n", boot_sector.items.uchSecPerClust);
    fprintf (debug_file, "wReservedSec = 0x%04X\n", boot_sector.items.wReservedSec);
    fprintf (debug_file, "uchReserved = %02X", (unsigned char)boot_sector.items.uchReserved[1]);
    fprintf (debug_file, "%02X", (unsigned char)boot_sector.items.uchReserved[2]);
    fprintf (debug_file, "%02X\n", (unsigned char)boot_sector.items.uchReserved[3]);
    fprintf (debug_file, "wUnused1 = 0x%04X\n", boot_sector.items.wUnused1);
    fprintf (debug_file, "uchMediaDescriptor = 0x%02X\n", boot_sector.items.uchMediaDescriptor);
    fprintf (debug_file, "wUnused2 = 0x%04X\n", boot_sector.items.wUnused2);
    fprintf (debug_file, "wSecPerTrack = 0x%04X\n", boot_sector.items.wSecPerTrack);
    fprintf (debug_file, "wNumberOfHeads = 0x%04X\n", boot_sector.items.wNumberOfHeads);
    fprintf (debug_file, "dwHiddenSec = 0x%08X\n", boot_sector.items.dwHiddenSec);
    fprintf (debug_file, "dwUnused3 = 0x%08X\n", boot_sector.items.dwUnused3);
    fprintf (debug_file, "dwUnused4 = 0x%08X\n", boot_sector.items.dwUnused4);
    fprintf (debug_file, "n64TotalSec = 0x%016llX\n", (long long)boot_sector.items.n64TotalSec);
    fprintf (debug_file, "n64MFTLogicalClustNum = 0x%016llX\n", (long long)boot_sector.items.n64MFTLogicalClustNum);
    fprintf (debug_file, "n64MFTMirrLogicalClustNum = 0x%016llX\n", (long long)boot_sector.items.n64MFTMirrLogicalClustNum);
    fprintf (debug_file, "nClustPerMFTRecord = 0x%08X\n", boot_sector.items.nClustPerMFTRecord);
    fprintf (debug_file, "nClustPerIndexRecord = 0x%08X\n", boot_sector.items.nClustPerIndexRecord);
    fprintf (debug_file, "n64VolumeSerialNum = 0x%016llX\n", (long long)boot_sector.items.n64VolumeSerialNum);
    fprintf (debug_file, "dwChecksum = 0x%08X\n", boot_sector.items.dwChecksum);
    // 426 char of bootstrap code in between here
    fprintf (debug_file, "wSecMark = 0x%04X\n", boot_sector.items.wSecMark);
  }

  if (strncmp(boot_sector.items.chOemID, "NTFS", 4) !=0)
  {
    fprintf (stderr, "ERROR! Partition boot sector ID is not NTFS\n");
    return (2);
  }

  cluster_size = (boot_sector.items.wBytesPerSec * boot_sector.items.uchSecPerClust);
  clusters_per_record = boot_sector.items.nClustPerMFTRecord;

  if (clusters_per_record > 0)
    mft_record_size = clusters_per_record * cluster_size;
  else
    mft_record_size = 1 << (0 - clusters_per_record);

  if (boot_sector.items.wBytesPerSec != 0)
    sectors_per_record = (mft_record_size / boot_sector.items.wBytesPerSec);
  else
    {
    fprintf(stderr, "ERROR! Partition boot sector reports 0 bytes per sector which is not possible\n");
    return (2);
  }

  if (sectors_per_record > 8)
  {
    fprintf(stderr, "ERROR! This program only allows for 8 sectors per MFT record, and this partition boot sector reports %ld\n", sectors_per_record);
    return (2);
  }

  return (0);
}


// function to process boot sector and read mft inode

int process_boot_sector(void)
{
  input_offset = ( (cluster_size * boot_sector.items.n64MFTLogicalClustNum) + partition_offset);
  //fprintf (stderr, "input_offset= %lld\n", input_offset);
  data_size = mft_record_size;
  fprintf (stdout, "Reading mft inode...\n");

  FILE *readfile;
  readfile = fopen(source_disk, "rb");
  if (readfile == NULL)
  {
    fprintf(stderr, "Cannot open %s for reading (%s).\nAborting...\n", source_disk, strerror(errno));
    return (4);
  }

  unsigned long record_number = 0;
  return_value = read_mft_entry (readfile, input_offset, record_number);
  if (return_value > 0)
  {
    fprintf (stderr, "There was an error in reading or processing the main mft record.\n");
    fprintf (stderr, "Attempting to read the mft mirror...\n");

    input_offset = ( (cluster_size * boot_sector.items.n64MFTMirrLogicalClustNum) + partition_offset);
    //fprintf (stderr, "input_offset= %lld\n", input_offset);
    return_value = read_mft_entry (readfile, input_offset, record_number);
  }

  fclose(readfile);
  return (return_value);
}



// function to process mft entry

int process_entry(unsigned long record_number)
{
  data_size = mft_record_size;

  FILE *readfile;
  readfile = fopen(source_disk, "rb");
  if (readfile == NULL)
  {
    fprintf(stderr, "Cannot open %s for reading (%s).\nAborting...\n", source_disk, strerror(errno));
    exit (4);
  }

  return_value = read_mft_entry (readfile, input_offset, record_number);
  if (return_value > 0)
  {
    if (verbose > 2)
    fprintf (stderr, "\nThere was an error in reading or processing inode record %ld\n", record_number);
  }

  fclose(readfile);
  return (return_value);
}




// function to compare the mft records against logfile to find errors
int find_errors(void)
{
  // assign memory for master inode list
  unsigned int rowsm = 1000;
  master_inode_buf = malloc(rowsm * sizeof(*master_inode_buf));
  if (master_inode_buf == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (3);
  }
  master_parent_buf = malloc(rowsm * sizeof(*master_parent_buf));
  if (master_parent_buf == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (3);
  }
  master_attrib_buf = malloc(rowsm * sizeof(*master_attrib_buf));
  if (master_attrib_buf == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (3);
  }

  int cols = 256;
  unsigned int i;
  master_file_name_buf = malloc(rowsm * cols * sizeof(*master_file_name_buf));
  if (master_file_name_buf == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (3);
  }
  master_file_name_pt = malloc(rowsm * sizeof(*master_file_name_pt));
  if (master_file_name_pt == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (3);
  }
  for (i = 0; i < rowsm; ++i)
  {
    master_file_name_pt[i] = &master_file_name_buf[i * cols];
  }


  // assign memory for error inode list
  unsigned int rows = 1000;
  inode_count_buf = malloc(rows * sizeof(*inode_count_buf));
  if (inode_count_buf == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (3);
  }
  error_size_buf = malloc(rows * sizeof(*error_size_buf));
  if (error_size_buf == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (3);
  }
  inode_type_buf = malloc(rows * sizeof(*inode_type_buf));
  if (inode_type_buf == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (3);
  }
  error_count_buf = malloc(rows * sizeof(*error_count_buf));
  if (error_count_buf == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (3);
  }

  cols = 256;
  file_name_buf = malloc(rows * cols * sizeof(*file_name_buf));
  if (file_name_buf == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (3);
  }
  file_name_pt = malloc(rows * sizeof(*file_name_pt));
  if (file_name_pt == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    return (3);
  }
  for (i = 0; i < rows; ++i)
  {
    file_name_pt[i] = &file_name_buf[i * cols];
  }


  process_mft = false;
  inode_count = 0;
  output_lines = 0;
  for (i = 0; i < mft_part_number+1; i++)
  {
    unsigned long long count = mft_part_size[i] / mft_record_size;
    if (debug > 0)
      fprintf (debug_file, "\nmft part# %d count=%lld\n", i, count);
    unsigned long long n;
    for (n = 0; n < count; n++)
    {
      // check how much time has elapsed
      gettimeofday(&tvEnd, NULL);
      timeval_subtract(&tvDiff, &tvEnd, &tvBegin);
      // if at least .1 second or more has elapsed since last time then do screen output
      if ( ((tvDiff.tv_sec - last_second) >= 1 || (tvDiff.tv_usec - last_usecond) >= 2000) || inode_count+1 == total_inodes)
      {
	last_second = tvDiff.tv_sec;
	last_usecond = tvDiff.tv_usec;
	fprintf (stdout, "\rprocessing inode %ld of %lld", inode_count+1, total_inodes);
      }

      input_offset = mft_part_offset[i] + (n * mft_record_size);
      return_value = process_entry(inode_count);

      if (has_data > 0)
      {
	if (debug > 2)
	  fprintf (debug_file, "inode=%ld, has_data=%d\n", inode_count, has_data);
	unsigned long error_count = 0;
	unsigned long long error_size = 0;
	unsigned long temp_error_count = 0;
	unsigned long long temp_error_size = 0;
	bool new_error = false;
	unsigned long x;
	unsigned int d;
	for (d = 0; d < has_data; d++)
	{
	  unsigned int c;
	  for (c = 0; c < inode_part_number[d]+1; c++)
	  {
	    // check if this data run matches with any error sections of the rescue log
	    for (x = 0; x < total_lines; x++)
	    {
	      if (type[x] == '-' || type[x] == '/' || type[x] == '*')
	      {
		if (position[x] >= inode_part_offset[d][c] && position[x] < (inode_part_offset[d][c] + inode_part_size[d][c]))
		{
		  if ((position[x] + size[x]) < (inode_part_offset[d][c] + inode_part_size[d][c]))
		  {
		    error_size += size[x];
		    temp_error_size += size[x];
		    error_count++;
		    temp_error_count++;
		    new_error = true;
		  }
		  else
		  {
		    error_size += (inode_part_offset[d][c] + inode_part_size[d][c]) - position[x];
		    temp_error_size += (inode_part_offset[d][c] + inode_part_size[d][c]) - position[x];
		    error_count++;
		    temp_error_count++;
		    new_error = true;
		  }
		}
		else
		{
		  if (inode_part_offset[d][c] >= position[x] && inode_part_offset[d][c] < (position[x] + size[x]))
		  {
		    if ((inode_part_offset[d][c] + inode_part_size[d][c]) < (position[x] + size[x]))
		    {
		      error_size += inode_part_size[d][c];
		      temp_error_size += inode_part_size[d][c];
		      error_count++;
		      temp_error_count++;
		      new_error = true;
		    }
		    else
		    {
		      error_size += (position[x] + size[x]) - inode_part_offset[d][c];
		      temp_error_size += (position[x] + size[x]) - inode_part_offset[d][c];
		      error_count++;
		      temp_error_count++;
		      new_error = true;
		    }
		  }
		}

		//fprintf (stdout, "line %ld %llx %llx %c\n", x, position[x], size[x], type[x]);
	      }
	      if (debug > 3 && error_count > 0 && new_error == true)
	      {
		new_error = false;
		fprintf (debug_file, "inode=%ld, part=%d, offset=%lld, size=%lld, type=%02X, errors=%ld, errorsize=%lld\n",inode_count, c, inode_part_offset[d][c], inode_part_size[d][c], inode_data_type[d], error_count, error_size);
	      }
	    }

	    if (debug > 0 && error_count > 0 && new_error == true)
	    {
	      new_error = false;
	      fprintf (debug_file, "inode=%ld  part=%d  offset=0x%llX  fulloffset=0x%llX  size=0x%llX  type=0x%02X  errors=%ld  errorsize=%lld\n",inode_count, c, inode_part_offset[d][c] - partition_offset, inode_part_offset[d][c], inode_part_size[d][c], inode_data_type[d], temp_error_count, temp_error_size);
	      temp_error_count = 0;
	      temp_error_size = 0;
	    }
	  }
	}

	// put it in the list of bad files if it has errors
	if (error_count > 0)
	{
	  // only process used inodes, not non-indexed (deleted)
	  if ((file_attribute.items.Flags & 0x2000) == 0)
	  {
	    // check if file or folder and output accordingly
	    if ((file_attribute.items.Flags & 0x30000000) == 0)
	    {
	      //fprintf (stdout, "inode=%04ld  errors=%04ld  errorsize=%06lld  file   name=%s\n",inode_count, error_count, error_size, file_name);
	      inode_type_buf[output_lines] = 0;
	    }

	    else
	    {
	      //fprintf (stdout, "inode=%04ld  errors=%04ld  errorsize=%06lld  folder name=%s\n",inode_count, error_count, error_size, file_name);
	      inode_type_buf[output_lines] = 1;
	    }

	    inode_count_buf[output_lines] = inode_count;
	    error_count_buf[output_lines] = error_count;
	    error_size_buf[output_lines] = error_size;
	    strcpy (file_name_pt[output_lines], file_name);

	    output_lines++;


	    // if out of assigned memory then assign more
	    if (output_lines >= rows)
	    {
	      rows += 1000;
	      inode_count_buf = realloc(inode_count_buf, rows * sizeof(*inode_count_buf));
	      if (inode_count_buf == NULL)
	      {
		fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
		return (3);
	      }

	      error_size_buf = realloc(error_size_buf, rows * sizeof(*error_size_buf));
	      if (error_size_buf == NULL)
	      {
		fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
		return (3);
	      }

	      inode_type_buf = realloc(inode_type_buf, rows * sizeof(*inode_type_buf));
	      if (inode_type_buf == NULL)
	      {
		fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
		return (3);
	      }

	      error_count_buf = realloc(error_count_buf, rows * sizeof(*error_count_buf));
	      if (error_count_buf == NULL)
	      {
		fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
		return (3);
	      }

	      int cols = 256;
	      unsigned int i;
	      file_name_buf = realloc(file_name_buf, rows * cols * sizeof(*file_name_buf));
	      if (file_name_buf == NULL)
	      {
		fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
		return (3);
	      }

	      file_name_pt = realloc(file_name_pt, rows * sizeof(*file_name_pt));
	      if (file_name_pt == NULL)
	      {
		fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
		return (3);
	      }

	      for (i = 0; i < rows; ++i)
	      {
		file_name_pt[i] = &file_name_buf[i * cols];
	      }
	    }
	  }
	  else
	  {
	    deleted_inodes++;
	    if (debug > 1)
	      fprintf (debug_file, "Inode %ld is not in use (deleted)\n", inode_count);
	  }
	}
      }

      // put it in the master list
      master_inode_buf[master_list_count] = inode_count;
      master_parent_buf[master_list_count] = file_attribute.items.dwParentDirectory;
      master_attrib_buf[master_list_count] = file_attribute.items.Flags;
      strcpy (master_file_name_pt[master_list_count], file_name);

      master_list_count++;
      // if out of assigned memory then assign more
      if (master_list_count >= rowsm)
      {
	rowsm += 1000;
	master_inode_buf = realloc(master_inode_buf, rowsm * sizeof(*master_inode_buf));
	if (master_inode_buf == NULL)
	{
	  fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
	  return (3);
	}

	master_parent_buf = realloc(master_parent_buf, rowsm * sizeof(*master_parent_buf));
	if (master_parent_buf == NULL)
	{
	  fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
	  return (3);
	}

	master_attrib_buf = realloc(master_attrib_buf, rowsm * sizeof(*master_attrib_buf));
	if (master_attrib_buf == NULL)
	{
	  fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
	  return (3);
	}

	int cols = 256;
	unsigned int i;
	master_file_name_buf = realloc(master_file_name_buf, rowsm * cols * sizeof(*master_file_name_buf));
	if (master_file_name_buf == NULL)
	{
	  fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
	  return (3);
	}

	master_file_name_pt = realloc(master_file_name_pt, rowsm * sizeof(*master_file_name_pt));
	if (master_file_name_pt == NULL)
	{
	  fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
	  return (3);
	}

	for (i = 0; i < rowsm; ++i)
	{
	  master_file_name_pt[i] = &master_file_name_buf[i * cols];
	}
      }


      inode_count++;
    }
  }
  fprintf (stdout, "\n");


//  for (i = 0; i < output_lines; i++)
//  {
//    if (inode_type_buf[i] == 1)
//      fprintf (stdout, "inode=%04ld  errors=%04ld  errorsize=%06lld  folder name=%s\n",inode_count_buf[i], error_count_buf[i], error_size_buf[i], file_name_pt[i]);
//    else
//      fprintf (stdout, "inode=%04ld  errors=%04ld  errorsize=%06lld  file   name=%s\n",inode_count_buf[i], error_count_buf[i], error_size_buf[i], file_name_pt[i]);
//  }


  return (0);
}



// function to process the output list to get full path names
int process_list(void)
{

  // process the list into output file
  // open destination file
  writefile = fopen(outputfile, "w");
  if (writefile == NULL)
  {
    fprintf(stderr, "Cannot open %s for writing (%s).\nAborting...\n", outputfile, strerror(errno));
    exit (4);
  }

  unsigned long p = 0;
  unsigned long i = 0;
  unsigned long n = 0;
  char full_file_name[32768];
  char temp_file_name[32768];
  bool found_parent;
  bool process;
  unsigned long rows = master_list_count;

  for (i = 0; i < output_lines; i++)
  {
    // check how much time has elapsed
    gettimeofday(&tvEnd, NULL);
    timeval_subtract(&tvDiff, &tvEnd, &tvBegin);
    // if at least .1 second or more has elapsed since last time then do screen output
    if ( ((tvDiff.tv_sec - last_second) >= 1 || (tvDiff.tv_usec - last_usecond) >= 2000) || i+1 == output_lines)
    {
      last_second = tvDiff.tv_sec;
      last_usecond = tvDiff.tv_usec;
      fprintf (stdout, "\rprocessing error record %ld of %ld", i+1, output_lines);
    }

    strcpy(full_file_name, file_name_pt[i]);

    process = false;
    // don't process if root folder
    if (strcmp(full_file_name, ".") == 0)
      process = false;

    found_parent = true;


    for (n = 0; n < master_list_count; n++)
    {
      if (inode_count_buf[i] == master_inode_buf[n])
      {
	process = true;
	break;
      }
    }


    while (found_parent == true && process == true)
    {

      for (p = 0; p < rows; p++)
      {
	if (master_inode_buf[p] == master_parent_buf[n])
	{
	  // check if parent and inode are the same, indicating top level
	  if (n == p)
	  {
	    found_parent = false;
	    break;
	  }

	  // check if parent is actully a folder
	  if ((master_attrib_buf[p] & 0x30000000) == 0)
	  {
	    found_parent = false;
	    break;
	  }


	  strcpy(temp_file_name, master_file_name_pt[p]);
	  strcat(temp_file_name, "/");
	  strcat(temp_file_name, full_file_name);
	  strcpy(full_file_name, temp_file_name);

	  n = p;
	  break;
	}
	// check if reached end of list without finding parent
	else if (p == rows-1)
	  found_parent = false;

      }
    }
    // only print if good result
    if (process == true)
    {
      // check if file or folder and output accordingly
      if (inode_type_buf[i] == 0)
      {
	fprintf (writefile, "inode=%04ld  errors=%04ld  errorsize=%06lld  FILE   name=%s\n",inode_count_buf[i], error_count_buf[i], error_size_buf[i], full_file_name);
      }

      else
      {
	fprintf (writefile, "inode=%04ld  errors=%04ld  errorsize=%06lld  FOLDER name=%s\n",inode_count_buf[i], error_count_buf[i], error_size_buf[i], full_file_name);
      }

    }
  }
  printf ("\n");
  fclose(writefile);

  return (0);
}



// function to read inode of mft into memory and process and read the data
int read_mft_entry (FILE *readfile, unsigned long long input_offset, unsigned long record_number)
{
  has_data = 0;
  if (fseeko(readfile, input_offset, SEEK_SET) != 0 )
  {
    fprintf(stderr, "Error seeking %s (%s).\n", source_disk, strerror(errno));
    exit (4);
  }

  if (fread(mft_mft.raw.mftfile, 1, mft_record_size, readfile) < mft_record_size )
  {
    fprintf(stderr, "\nError reading inode %ld from %s (%s).\n", inode_count, source_disk, strerror(errno));
    if (debug > 0)
      fprintf(debug_file, "\nError reading inode %ld from %s (%s).\n", inode_count, source_disk, strerror(errno));
    mft_hard_errors++;
    return (4);
  }

  if (debug > 2)
  {
    fprintf (debug_file, "\n");
    fprintf (debug_file, "chFileSignature = %c", (unsigned char)mft_mft.items.chFileSignature[0]);
    fprintf (debug_file, "%c", (unsigned char)mft_mft.items.chFileSignature[1]);
    fprintf (debug_file, "%c", (unsigned char)mft_mft.items.chFileSignature[2]);
    fprintf (debug_file, "%c\n", (unsigned char)mft_mft.items.chFileSignature[3]);
    fprintf (debug_file, "wFixupOffset = %04X\n", mft_mft.items.wFixupOffset);
    fprintf (debug_file, "wFixupSize = %04X\n", mft_mft.items.wFixupSize);
    fprintf (debug_file, "n64LogSeqNumber = %016llX\n", (long long)mft_mft.items.n64LogSeqNumber);
    fprintf (debug_file, "wSequence = %04X\n", mft_mft.items.wSequence);
    fprintf (debug_file, "wHardLinks = %04X\n", mft_mft.items.wHardLinks);
    fprintf (debug_file, "wAttribOffset = %04X\n", mft_mft.items.wAttribOffset);
    fprintf (debug_file, "wFlags = %08X\n", mft_mft.items.wFlags);
    fprintf (debug_file, "dwRecLength = %08X\n", mft_mft.items.dwRecLength);
    fprintf (debug_file, "dwAllLength = %08X\n", mft_mft.items.dwAllLength);
    fprintf (debug_file, "n64BaseMftRec = %016llX\n", (long long)mft_mft.items.n64BaseMftRec);
    fprintf (debug_file, "wNextAttrID = %04X\n", mft_mft.items.wNextAttrID);
    fprintf (debug_file, "wUnknown = %04X\n", mft_mft.items.wUnknown);
    fprintf (debug_file, "dwMFTRecNumber = %08X\n", mft_mft.items.dwMFTRecNumber);
    fprintf (debug_file, "wFixupPattern = %04X\n", mft_mft.items.wFixupPattern);
  }

  // check for inode signature, if not there then exit with error
  // but ignore inodes 16-23 as they may not as they are currently used by ntfs
  if (strncmp(mft_mft.items.chFileSignature, "FILE", 4) !=0 && (record_number < 16 || record_number > 23))
  {
    //fprintf (stderr, "\nWARNING! Inode does not have the FILE signature\n");

    if (debug > 0)
    {
      fprintf (debug_file, "WARNING! (hard error) Inode %ld does not have the FILE signature\n", record_number);
    }
    mft_hard_errors++;
    return(1);
  }

  // check that inode matches what we are looking for
  // but ignore inodes 16-23 as they are currently not used by ntfs
  if (mft_mft.items.dwMFTRecNumber != record_number && (record_number < 16 || record_number > 23) )
  {
    //fprintf (stderr, "\nWARNING! Requested inode %ld does not match the inode %d reported by the mft record\n", record_number, mft_mft.items.dwMFTRecNumber);
    if (debug > 1)
    {
      fprintf (debug_file, "WARNING! (soft error) Requested inode %ld does not match the inode %d reported by the mft record\n", record_number, mft_mft.items.dwMFTRecNumber);
    }
    mft_soft_errors++;
  }


  // check inode for corruption
  // but ignore inodes 16-23 as they are not currently used by ntfs
  if (record_number < 16 || record_number > 23)
  {
    if (mft_mft.items.wFixupOffset != 0x2A && mft_mft.items.wFixupOffset != 0x30)
    {
      fprintf (stdout, "ERROR! Fixup Offset of inode %ld is not 0x2A or 0x30. This program assumes that it should be one of those to work\n", record_number);
      if (debug > 0)
      {
	fprintf (debug_file, "ERROR! Fixup Offset of inode %ld is not 0x2A or 0x30. This program assumes that it should be one of those to work\n", record_number);
      }
      return(3);
    }

    if (mft_mft.items.wFixupOffset == 0x30)
    {
      WORD i;
      unsigned long n = 0;
      for (i = 0; i < mft_mft.items.wFixupSize-1; i++)
      {
	n += 512;
	if (debug > 2)
	  fprintf (debug_file, "Inode %ld fixup %d is 0x%x replaced with 0x%x\n",record_number, i, mft_mft.items.wFixupCheck[(n / 2) -26], mft_mft.items.wFixupCheck[i]);
	if (mft_mft.items.wFixupCheck[(n / 2) -26] != mft_mft.items.wFixupPattern)
	{
	  if (mft_mft.items.dwRecLength > n - 512)
	  {
	    if (debug > 0)
	    {
	      fprintf (debug_file, "WARNING! (hard error) Inode %ld section(sector) %d appears to be corrupt\n", record_number, i+1);
	    }
	    mft_hard_errors++;
	    return(3);
	  }
	  else
	  {
	    if (debug > 1)
	    {
	      fprintf (debug_file, "WARNING! (recoverable error) Inode %ld section(sector) %d appears to be corrupt (but not used, record size is only %d)\n", record_number, i+1, mft_mft.items.dwRecLength);
	    }
	  }
	}
	mft_mft.items.wFixupCheck[(n / 2) -26] = mft_mft.items.wFixupCheck[i];
      }
    }

    // process the old NTFS way
    if (mft_mft.items.wFixupOffset == 0x2A)
    {
      WORD i = 0;
      WORD fixup1 = mft_mft.items.dwMFTRecNumber & 0xffff;
      WORD fixup2 = mft_mft.items.dwMFTRecNumber >> 16;
      unsigned long n = 512;
      if (debug > 2)
	fprintf (debug_file, "Inode %ld fixup %d is 0x%x replaced with 0x%x\n",record_number, i, mft_mft.items.wFixupCheck[(n / 2) -26], fixup1);
      if (mft_mft.items.wFixupCheck[(n / 2) -26] != mft_mft.items.wUnknown)
      {
	if (mft_mft.items.dwRecLength > n - 512)
	{
	  if (debug > 0)
	  {
	    fprintf (debug_file, "WARNING! (hard error) Inode %ld section(sector) %d appears to be corrupt\n", record_number, i+1);
	  }
	  mft_hard_errors++;
	  return(3);
	}
	else
	{
	  if (debug > 1)
	  {
	    fprintf (debug_file, "WARNING! (recoverable error) Inode %ld section(sector) %d appears to be corrupt (but not used, record size is only %d)\n", record_number, i+1, mft_mft.items.dwRecLength);
	  }
	}
      }
      mft_mft.items.wFixupCheck[(n / 2) -26] = fixup1;

      i = 1;
      n = 1024;
      if (debug > 2)
	fprintf (debug_file, "Inode %ld fixup %d is 0x%x replaced with 0x%x\n",record_number, i, mft_mft.items.wFixupCheck[(n / 2) -26], fixup2);
      if (mft_mft.items.wFixupCheck[(n / 2) -26] != mft_mft.items.wUnknown)
      {
	if (mft_mft.items.dwRecLength > n - 512)
	{
	  if (debug > 0)
	  {
	    fprintf (debug_file, "WARNING! (hard error) Inode %ld section(sector) %d appears to be corrupt\n", record_number, i+1);
	  }
	  mft_hard_errors++;
	  return(3);
	}
	else
	{
	  if (debug > 1)
	  {
	    fprintf (debug_file, "WARNING! (recoverable error) Inode %ld section(sector) %d appears to be corrupt (but not used, record size is only %d)\n", record_number, i+1, mft_mft.items.dwRecLength);
	  }
	}
      }
      mft_mft.items.wFixupCheck[(n / 2) -26] = fixup2;
    }

  }


  unsigned long offset = mft_mft.items.wAttribOffset;

  // process attributes
  has_name = false;
  while (offset < (mft_mft.items.dwRecLength))
  {
    unsigned long n = offset;
    unsigned long i;
    for (i = 0; i < 64; i++)
      ntfs_attribute.raw.attribute_location[i] = mft_mft.raw.mftfile[n++];

    if (debug > 2)
    {
      fprintf (debug_file, "\n");
      fprintf (debug_file, "offset = %ld\n", offset);
      fprintf (debug_file, "dwType = %08X\n", ntfs_attribute.items.dwType);
      fprintf (debug_file, "dwFullLength = %08X\n", ntfs_attribute.items.dwFullLength);
      fprintf (debug_file, "uchNonResFlag = %02X\n", ntfs_attribute.items.uchNonResFlag);
      fprintf (debug_file, "uchNameLength = %02X\n", ntfs_attribute.items.uchNameLength);
      fprintf (debug_file, "wNameOffset = %04X\n", ntfs_attribute.items.wNameOffset);
      fprintf (debug_file, "wFlags = %08X\n", ntfs_attribute.items.wFlags);
      fprintf (debug_file, "wID = %04X\n", ntfs_attribute.items.wID);
    }

    if ((ntfs_attribute.items.dwType == 0xFFFFFFFF) || (ntfs_attribute.items.dwFullLength <= 24))
    {
      break;
    }

    // resident
    if (ntfs_attribute.items.uchNonResFlag == 0)
    {
      if (debug > 2)
      {
	fprintf (debug_file, "RESEDENT\n");
	fprintf (debug_file, "dwLength = %08X\n", ntfs_attribute.items.Attr.Resident.dwLength);
	fprintf (debug_file, "wAttrOffset = %04X\n", ntfs_attribute.items.Attr.Resident.wAttrOffset);
	fprintf (debug_file, "uchIndexedTag = %02X\n", ntfs_attribute.items.Attr.Resident.uchIndexedTag);
	fprintf (debug_file, "uchPadding = %02X\n", ntfs_attribute.items.Attr.Resident.uchPadding);
	fprintf (debug_file, "LOCAL DATA = ");
	n = ntfs_attribute.items.Attr.Resident.wAttrOffset + offset;
	for (i = 0; i < ntfs_attribute.items.Attr.Resident.dwLength; i++)
	{
	  fprintf (debug_file, "%02X ", (unsigned char)mft_mft.raw.mftfile[n++]);
	}
	fprintf (debug_file, "\n");

	n = ntfs_attribute.items.Attr.Resident.wAttrOffset + offset;
	for (i = 0; i < ntfs_attribute.items.Attr.Resident.dwLength; i++)
	{
	  fprintf (debug_file, "%c", isprint(mft_mft.raw.mftfile[n]) ? mft_mft.raw.mftfile[n] : '.');
	  n++;
	}
	fprintf (debug_file, "\n");
      }

    }
    //non resident
    else
    {
      if (debug > 1)
      {
	if (debug > 2)
	{
	  fprintf (debug_file, "NONRESIDENT\n");
	  fprintf (debug_file, "n64StartVCN = %016llX\n", (long long)ntfs_attribute.items.Attr.NonResident.n64StartVCN);
	  fprintf (debug_file, "n64EndVCN = %016llX\n", (long long)ntfs_attribute.items.Attr.NonResident.n64EndVCN);
	  fprintf (debug_file, "wDatarunOffset = %04X\n", ntfs_attribute.items.Attr.NonResident.wDatarunOffset);
	  fprintf (debug_file, "wCompressionSize = %04X\n", ntfs_attribute.items.Attr.NonResident.wCompressionSize);
	  fprintf (debug_file, "uchPadding = %02X", ntfs_attribute.items.Attr.NonResident.uchPadding[0]);
	  fprintf (debug_file, "%02X", ntfs_attribute.items.Attr.NonResident.uchPadding[1]);
	  fprintf (debug_file, "%02X", ntfs_attribute.items.Attr.NonResident.uchPadding[2]);
	  fprintf (debug_file, "%02X\n", ntfs_attribute.items.Attr.NonResident.uchPadding[3]);
	  fprintf (debug_file, "n64AllocSize = %016llX\n", (long long)ntfs_attribute.items.Attr.NonResident.n64AllocSize);
	  fprintf (debug_file, "n64RealSize = %016llX\n", (long long)ntfs_attribute.items.Attr.NonResident.n64RealSize);
	  fprintf (debug_file, "n64StreamSize = %016llX\n", (long long)ntfs_attribute.items.Attr.NonResident.n64StreamSize);
	  fprintf (debug_file, "LOCAL DATA = ");
	}

	n = ntfs_attribute.items.Attr.NonResident.wDatarunOffset + offset;
	for (i = 0; i < (ntfs_attribute.items.dwFullLength - ntfs_attribute.items.Attr.NonResident.wDatarunOffset); i++)
	{
	  if (debug > 2)
	    fprintf (debug_file, "%02X ", (unsigned char)mft_mft.raw.mftfile[n]);
	  n++;
	  if (n >= ntfs_attribute.items.dwFullLength +ntfs_attribute.items.Attr.NonResident.wDatarunOffset + offset || n >= mft_mft.items.dwRecLength || n >= mft_record_size)
	    break;
	}
	if (debug > 2)
	  fprintf (debug_file, "\n");

	n = ntfs_attribute.items.Attr.NonResident.wDatarunOffset + offset;
	for (i = 0; i < (ntfs_attribute.items.dwFullLength - ntfs_attribute.items.Attr.NonResident.wDatarunOffset); i++)
	{
	  if (debug > 2)
	    fprintf (debug_file, "%c", isprint(mft_mft.raw.mftfile[n]) ? mft_mft.raw.mftfile[n] : '.');
	  n++;
	  if (n >= ntfs_attribute.items.dwFullLength +ntfs_attribute.items.Attr.NonResident.wDatarunOffset + offset || n >= mft_mft.items.dwRecLength || n >= mft_record_size)
	  {
	    mft_soft_errors++;
	    if (debug > 1)
	      fprintf (debug_file, "Warning! (soft error) A data run in mft entry %ld exceeded the record size limit\n", record_number);
	    return(1);
	  }
	}
	if (debug > 2)
	  fprintf (debug_file, "\n");
      }
    }

    // if type is name then get the name and set name flag
    if (ntfs_attribute.items.dwType == 0x30)
    {
      // if no name yet get it
      if (has_name == false)
	getname(offset);

      // else if has name and it is dos then get new name
	else if (file_name_type == 2)
	  getname(offset);

	has_name = true;
    }

    // process the mft inode special to get the mft data run
    if (process_mft == true)
    {
      // if type is data and it is not named (meaning it is actual file data) then process
      if (ntfs_attribute.items.dwType == 0x80 && ntfs_attribute.items.uchNameLength == 0x00)
	processdata(offset);
    }

    // process everything else into data runs
    else
    {
      // if it is non resident then there is a data run so process it
      if (ntfs_attribute.items.uchNonResFlag != 0)
	processdata(offset);
    }

    offset += ntfs_attribute.items.dwFullLength;
  }

  return (0);
}




// function to process and read data
int processdata(unsigned long offset)
{


  // if type is resident process as such
  if (ntfs_attribute.items.uchNonResFlag == 0)
  {
    fprintf (stderr, "\nProgram error, there should be no resident data processing in this program\n");
  }

  // else process as non resident
  else
  {
    bool keepreading = true;
    bool offset_negative;
    unsigned long long tempoffset = ntfs_attribute.items.Attr.NonResident.wDatarunOffset + offset;
    unsigned long long totalread = 0;
    unsigned long long readlengthbytes, readoffsetbytes;
    unsigned long long newoffset = 0;
    unsigned long long lastoffset = 0;
    unsigned long part = 0;
    unsigned long i;

    union {
      unsigned char byte;
      struct {
	unsigned char len:4;
	unsigned char offs:4;
      } items;
    } len_offs;

    union {
      unsigned char len[8];
      int64_t length;
    } read_length;

    union {
      unsigned char offs[8];
      int64_t offset;
    } read_offset;

    while (keepreading == true)
    {
      len_offs.byte = mft_mft.raw.mftfile[tempoffset];
      if (len_offs.byte == 0)
      {
	if (totalread < (unsigned long long)(ntfs_attribute.items.Attr.NonResident.n64AllocSize * (unsigned long long)cluster_size))
	{

	  mft_soft_errors++;
	  if (debug > 1)
	    fprintf (debug_file, "Warning! (soft error) Size actually read of inode %ld was less than the reported allocated size\n", inode_count);
	}
	break;
      }

      if (debug > 2)
      {
	fprintf (debug_file, "\nPart = %ld\n", part);
	fprintf (debug_file, "byte = %02X, offs = %02X, len = %02X\n", len_offs.byte, len_offs.items.offs, len_offs.items.len);
      }

      tempoffset++;

      read_length.length = 0;
      offset_negative = false;

      for (i = 0; i < len_offs.items.len && i < 8; i++)
      {
	read_length.len[i] = mft_mft.raw.mftfile[tempoffset++];
      }
      readlengthbytes = read_length.length * cluster_size;

      // if size of read exceeds the allocated size then abort
      if (readlengthbytes > ((ntfs_attribute.items.Attr.NonResident.n64AllocSize * cluster_size) - totalread))
      {
	mft_soft_errors++;
	if (debug > 1)
	  fprintf (debug_file, "Warning! (soft error) File read length of inode %ld exceeded allocated size\n", inode_count);
	break;
      }


      if (debug > 2)
      {
	fprintf (debug_file, "read_length = %016llX\n", (long long)read_length.length);
	fprintf (debug_file, "readlengthbytes = %016llX\n", readlengthbytes);
      }

      read_offset.offset = 0;
      for (i = 0; i < len_offs.items.offs && i < 8; i++)
      {
	read_offset.offs[i] = mft_mft.raw.mftfile[tempoffset++];
	// if byte is negative set flag
	// this leaves most significant byte to set the flag
	if ((int8_t) read_offset.offs[i] < 0)
	  offset_negative = true;
	else offset_negative = false;
      }

      if (debug > 2)
	fprintf (debug_file, "read_offset = %016llX\n", (long long)read_offset.offset);

      // fix read offset so it is signed properly
	if (offset_negative)
	{
	  switch (len_offs.items.offs)
	  {
	    case 1:
	      read_offset.offset |= ~(int64_t)0xff;
	      break;

	    case 2:
	      read_offset.offset |= ~(int64_t)0xffff;
	      break;

	    case 3:
	      read_offset.offset |= ~(int64_t)0xffffff;
	      break;

	    case 4:
	      read_offset.offset |= ~(int64_t)0xffffffff;
	      break;

	    case 5:
	      read_offset.offset |= ~(int64_t)0xffffffffff;
	      break;

	    case 6:
	      read_offset.offset |= ~(int64_t)0xffffffffffff;
	      break;

	    case 7:
	      read_offset.offset |= ~(int64_t)0xffffffffffffff;
	      break;

	    case 8:
	      read_offset.offset |= ~(int64_t)0xffffffffffffffff;
	      break;

	    default:
	      fprintf (stderr, "aborting\n");
	      abort ();
	  }

	  if (debug > 2)
	    fprintf (debug_file, "fixed read_offset = %016llX\n", (long long)read_offset.offset);
	}

	if (debug > 2)
	  fprintf (debug_file, "lastoffset = %lld\n", lastoffset);

	newoffset = read_offset.offset + lastoffset;
	lastoffset = newoffset;

	if (debug > 2)
	  fprintf (debug_file, "newoffset = %lld\n", newoffset);



	readoffsetbytes = newoffset * (unsigned long long)cluster_size;

	if (debug > 2)
	  fprintf (debug_file, "readoffsetbytes = %016llX\n", readoffsetbytes);


	// process the part into a data run

	  input_offset = readoffsetbytes + partition_offset;
	  data_size = readlengthbytes;

	  totalread += readlengthbytes;

	  if (debug > 2)
	    fprintf (debug_file, "totalread = %016llX\n", totalread);


	  /* if we have read to the end then stop and trunicate the file to the proper length
	   *   since we read all the clusters to be compatible with ddrescue direct mode and line up with sectors */
	  if ((unsigned long long)ntfs_attribute.items.Attr.NonResident.n64RealSize <= totalread)
	  {
	    keepreading = false;
	    data_size = data_size - (totalread - ntfs_attribute.items.Attr.NonResident.n64RealSize);
	  }

	  if (process_mft == true)
	  {
	    mft_part_number = part;
	    mft_part_offset[part] = input_offset;
	    mft_part_size[part] = data_size;
	    total_size = ntfs_attribute.items.Attr.NonResident.n64RealSize;
	  }
	  else
	  {
	    inode_part_number[has_data] = part;
	    inode_part_offset[has_data][part] = input_offset;
	    inode_part_size[has_data][part] = data_size;
	    inode_data_type[has_data] = ntfs_attribute.items.dwType;
	  }

	  part ++;
    }
    has_data++;
  }
  return(0);
}




// function to display help
void help(void)
{
  unsigned int i;
  for (i = 0; i < ddru_ntfsfindbad_help_txt_len; i++)
  {
    printf ("%c", ddru_ntfsfindbad_help_txt[i]);
  }
}

// function to display version
void version(void)
{
  printf ("%s %s\n", title, version_number);
  printf ("Copyright (C) %d Scott Dwyer.\n", copyright_year);
  printf ("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n");
  printf ("This is free software: you are free to change and redistribute it.\n");
  printf ("There is NO WARRANTY, to the extent permitted by law.\n");
}