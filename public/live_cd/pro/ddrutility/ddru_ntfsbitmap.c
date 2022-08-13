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
#include "ddru_ntfsbitmap_help.h"


char *title = "ddru_ntfsbitmap";
char *version_number = "1.5 20150111";
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
  printf("%s.%06d\n", buffer, (int)tv->tv_usec);
}


// Flag set by ‘--verbose’.
static int verbose = false;

// Flag set by ‘--debug’.
static int debug = false;


void help(void);
void version(void);
char *ddrescue_options = "";
char *source_mft_file = "__mftshort";
char *source_bootsec_file = "__bootsec";
char *source_bitmap_file = "__bitmapfile";
char *destination_log_file;
char *mft_domain_log_file;
char *rescue_report_file = "ntfsbitmap_rescue_report.log";
char *source_disk;
char *fillfile = "ff_fill_file";
char *debugfile = "ntfsbitmap_debug.log";
char clusters_per_record;
unsigned long sectors_per_record;
unsigned long long inode;
unsigned long long cluster_size;
unsigned long long input_offset;
unsigned long long output_offset;
unsigned long long data_size;
unsigned long long partition_offset = 0;
unsigned long long mft_part_offset[255];
unsigned long long mft_part_size[255];
unsigned long mft_part_number;
unsigned long long no_position;
unsigned long long no_size;
long long total_size;
FILE *debug_file;
FILE *fill_file;
bool zero_fill = false;
bool ff_fill = false;
bool good_read = true;
bool mft_domain = false;
bool make_mft_domain = false;
bool restart = false;
int return_value = 0;
unsigned int mingap = 0;

int ddrescue_read(char ddrescue_options[], unsigned long long input_offset, unsigned long long output_offset, unsigned long long data_size, char source_disk[], char destination_file[], char log_file[]);

int read_boot_sec_file(char *destination_file);

int read_mft_file (char *input_file, unsigned long record_number);

int processdata(unsigned long offset);

int read_file(char *destination_file);

int process_boot_sector(void);

int process_bitmap_file(char *input_file);

int process_mft_domain(void);




int main (int argc, char **argv)
{
  // Register ctrl-c signal and signal handler
   signal(SIGINT, signal_callback_handler);

  // set start_time
  struct timeval tvBegin, tvEnd, tvDiff;
  gettimeofday(&tvBegin, NULL);
  //printf ("ntfsreadfile start time = ");
  //timeval_print(&tvBegin);

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
      {"mingap",    	required_argument, 0, 'g'},
      {"inputoffset",    	required_argument, 0, 'i'},
      {"options",    	required_argument, 0, 'o'},
      {"mftdomain",    	required_argument, 0, 'm'},
      {"restart",  	no_argument,	0, 'r'},
      {0, 0, 0, 0}
    };
    // getopt_long stores the option index here.
    int option_index = 0;

    command_line_argument = getopt_long (argc, argv, "hvDVg:i:o:m:r",
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
	debug = true;
	break;

      case 'V':
	verbose = true;
	break;

      case 'g':
	mingap = atoi(optarg);
	break;

      case 'i':
	partition_offset = atoll(optarg);
	break;

      case 'o':
	ddrescue_options = optarg;
	break;

      case 'm':
	mft_domain_log_file = optarg;
	mft_domain = true;
	break;

      case 'r':
	restart = true;
	break;

      case '?':
	// getopt_long already printed an error message.
	command_line_error = true;
	break;

      default:
	fprintf (stderr, "aborting\n");
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
  destination_log_file = argv[optind++];



  // exit on command line error
  if (command_line_error)
  {
    fprintf (stderr, "ddru_ntfsbitmap: Both source and logfile must be specified.\n");
    fprintf (stderr, "Try 'ddru_ntfsbitmap --help' for more information.\n");
    exit (3);
  }

  //check if debug is turned on and open debug file if it is
  if (debug == true)
  {
    debug_file = fopen(debugfile, "w");
    if (debug_file == NULL)
    {
      fprintf(stderr, "Cannot open %s for writing (%s).\nAborting...\n", debugfile, strerror(errno));
      exit (4);
    }
  }

  if (debug == true)
  {
    fprintf (debug_file, "Debug file created by test at ");
    time_t mytime;
    mytime = time(NULL);
    fprintf (debug_file, "%s", ctime(&mytime));
    fprintf (debug_file, "source_mft_file = %s\n", source_mft_file);
    fprintf (debug_file, "inode = %lld\n", inode);
    fprintf (debug_file, "ddrescue_options = %s\n", ddrescue_options);
  }

  // end of command line processing

  fprintf (stdout, "%s %s\n", title, version_number);

  // remove the file log so we start fresh every time for accuracy
  remove (rescue_report_file);

  if (restart == true)
  {
    char temp_file[256];
    sprintf (temp_file, "%s.log", source_bootsec_file);
    remove (source_bootsec_file);
    remove (temp_file);
    sprintf (temp_file, "%s.log", source_mft_file);
    remove (source_mft_file);
    remove (temp_file);
    remove (source_bitmap_file);
  }

  // get total size of input
  int input_fd;
  input_fd = open (source_disk, O_RDONLY);
  if (input_fd == -1)
  {
    perror ("open2");
    exit (2);
  }
  total_size = lseek( input_fd, 0, SEEK_END );
  if (total_size == -1)
  {
    perror ("lseek1");
    exit (2);
  }
  close (input_fd);
  if (debug == true)
    fprintf (debug_file, "total_size = %lld\n", total_size);

  // read the boot sector to a file
  char destination_file[255];
  char log_file[255];
  sprintf (destination_file, "%s", source_bootsec_file);
  sprintf (log_file, "%s.log", source_bootsec_file);
  input_offset = partition_offset;
  output_offset = 0;
  data_size = 512;
  fprintf (stdout, "\nReading boot sector...\n");
  ddrescue_read(ddrescue_options, input_offset, output_offset, data_size, source_disk, destination_file, log_file);
  if (good_read == false)
  {
    fprintf (stdout, "\nThere was an error reading the boot sector, aborting.\n");
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

  // read the boot sector file into memory structure for processing
  return_value = read_boot_sec_file(source_bootsec_file);
  if (return_value > 0)
  {
    fprintf (stdout, "ERROR! Unable to correctly process the partition boot sector\n");
    exit (1);
  }

  // process boot sector and read short mft to file
  process_boot_sector();

  // create file filled with "ones" (FF)
  fill_file = fopen(fillfile, "wb");
  if (fill_file == NULL)
  {
    fprintf(stderr, "Cannot open %s for writing (%s).\nAborting...\n", fillfile, strerror(errno));
    exit (4);
  }
  int val = -1;
  fwrite((const void*) & val, sizeof(int) ,1 , fill_file);

  fclose (fill_file);


  // create a domain file for the mft if option selected
  if (mft_domain == true)
    process_mft_domain();

  // read bitmap mft entry into memory and process and then read the data file
  ff_fill = true;
  inode = 6;
  read_mft_file(source_mft_file, inode);
  ff_fill = false;

  // process the bitmap file and create ddrescue log file
  process_bitmap_file(source_bitmap_file);

  // final cleanup at end of main
  if (debug)
    fclose(debug_file);
  remove (fillfile);

  // print time elapsed
    gettimeofday(&tvEnd, NULL);
    //printf ("ntfsreadfile end time = ");
    //timeval_print(&tvEnd);

    // diff
    timeval_subtract(&tvDiff, &tvEnd, &tvBegin);
    fprintf (stdout, "ddru_ntfsbitmap took %d.%06d seconds to complete\n", (int)tvDiff.tv_sec, (int)tvDiff.tv_usec);

    if (good_read)
      exit (0);
    else
      exit (1);
}
// end of main
//******************************************************

// function to read data to file
int ddrescue_read(char *ddrescue_options, unsigned long long input_offset, unsigned long long output_offset, unsigned long long data_size, char *source_disk, \
char *destination_file, char *log_file)
{
  char command[512];
  sprintf (command, "ddrescue %s -i%lld -o%lld -s%lld %s \'%s\' \'%s\'", ddrescue_options, input_offset, output_offset, data_size, source_disk, destination_file, log_file);
  if (debug == true)
    fprintf (debug_file, "command = %s\n", command);
  if (verbose == true)
    fprintf (stdout, "command = %s\n", command);
  return_value = system (command);

  if (WTERMSIG(return_value) > 0)
  {
    if (WTERMSIG(return_value) == 2)
    {
      fprintf (stderr, "ddresuce was terminated by the user\n");
      fprintf (stderr, "full exit value= 0x%04X\n", return_value);
      exit(2);
    }
    else
    {
      fprintf (stderr, "\nddrescue exited abnormally\n");
      fprintf (stderr, "full exit value= 0x%04X\n", return_value);
      exit (3);
    }
  }
  else if (WEXITSTATUS(return_value) > 0)
  {
    fprintf (stderr, "ddresuce exited with code %d\n", WEXITSTATUS(return_value));
    fprintf (stderr, "full exit value= 0x%04X\n", return_value);
    exit (1);
  }

  // check if good finished read or not
  sprintf (command, "ddrescuelog -D -v -i%lld -s%lld \'%s\'", input_offset, data_size, log_file);
  return_value = system (command);

  // if read is bad or not finished then set flag and fill
  if (return_value > 0)
  {
    good_read = false;

    // fill bad and unread spots with zeros if option was selected
    if (zero_fill == true)
    {
      fprintf (stdout, "\n");
      fprintf (stdout, "Filling unread and bad spots with zeros...\n");
      sprintf (command, "ddrescue --fill=-*/? %s -i%lld -o%lld -s%lld /dev/zero \'%s\' \'%s\'", ddrescue_options, input_offset, output_offset, data_size, destination_file, log_file);
      return_value = system (command);

      if (WTERMSIG(return_value) > 0)
      {
	if (WTERMSIG(return_value) == 2)
	{
	  fprintf (stderr, "ddresuce was terminated by the user\n");
	  fprintf (stderr, "full exit value= 0x%04X\n", return_value);
	  exit(2);
	}
	else
	{
	  fprintf (stderr, "\nddrescue exited abnormally\n");
	  fprintf (stderr, "full exit value= 0x%04X\n", return_value);
	  exit (3);
	}
      }
      else if (WEXITSTATUS(return_value) > 0)
      {
	fprintf (stderr, "ddresuce exited with code %d\n", WEXITSTATUS(return_value));
	fprintf (stderr, "full exit value= 0x%04X\n", return_value);
	exit (1);
      }
    }

    // fill bad and unread spots with ones (FF) if option was selected
    if (ff_fill == true)
    {
      fprintf (stdout, "\n");
      fprintf (stdout, "Filling unread and bad spots with ones (FF)...\n");
      sprintf (command, "ddrescue --fill=-*/? %s -i%lld -o%lld -s%lld \'%s\' \'%s\' \'%s\'", ddrescue_options, input_offset, output_offset, data_size, fillfile, destination_file, log_file);
      return_value = system (command);

      if (WTERMSIG(return_value) > 0)
      {
	if (WTERMSIG(return_value) == 2)
	{
	  fprintf (stderr, "ddresuce was terminated by the user\n");
	  fprintf (stderr, "full exit value= 0x%04X\n", return_value);
	  exit(2);
	}
	else
	{
	  fprintf (stderr, "\nddrescue exited abnormally\n");
	  fprintf (stderr, "full exit value= 0x%04X\n", return_value);
	  exit (3);
	}
      }
      else if (WEXITSTATUS(return_value) > 0)
      {
	fprintf (stderr, "ddresuce exited with code %d\n", WEXITSTATUS(return_value));
	fprintf (stderr, "full exit value= 0x%04X\n", return_value);
	exit (1);
      }
    }
  }


  // put ddrescuelog output into file
  sprintf (command, "echo \'.\' >> %s", rescue_report_file);
  system (command);
  sprintf (command, "echo \'.......................................................................................................................\' >> %s", rescue_report_file);
  system (command);
  sprintf (command, "echo \'%s\' >> %s", log_file, rescue_report_file);
  system (command);
  sprintf (command, "echo \'%s\' >> %s", destination_file, rescue_report_file);
  system (command);
  sprintf (command, "ddrescuelog -t -i%lld -s%lld \'%s\' >> %s", input_offset, data_size, log_file, rescue_report_file);
  system (command);

  if (good_read)
    return (0);
  else
    return (1);
}


// function to read boot sector file into memory
int read_boot_sec_file(char *input_file)
{
  FILE *readfile;

  readfile = fopen(input_file, "rb");
  if (readfile == NULL)
  {
    fprintf(stderr, "Cannot open %s for reading (%s).\nAborting...\n", input_file, strerror(errno));
    exit (1);
  }


  if (fread(boot_sector.raw.bootsectfile, 1, 512, readfile) == 0 )
  {
    fprintf(stderr, "Error reading %s (%s).\nAborting...\n", input_file, strerror(errno));
    exit (4);
  }

  if (debug == true)
  {
    fprintf (debug_file, "chJumpInstruction = %02X", (unsigned char)boot_sector.items.chJumpInstruction[1]);
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
    fprintf (stderr, "ERROR! Boot sector ID is not NTFS\n");
    if (debug == false)
    {
      fprintf (stderr, "Removing the boot sector file and log\n");
      char destination_file[255];
      char log_file[255];
      sprintf (destination_file, "%s", source_bootsec_file);
      sprintf (log_file, "%s.log", source_bootsec_file);
      remove (destination_file);
      remove (log_file);
    }
    return (1);
  }

  cluster_size = (boot_sector.items.wBytesPerSec * boot_sector.items.uchSecPerClust);
  clusters_per_record = boot_sector.items.nClustPerMFTRecord;

  if (clusters_per_record > 0)
    mft_record_size = clusters_per_record * cluster_size;
  else
    mft_record_size = 1 << (0 - clusters_per_record);

  fclose(readfile);

  if (boot_sector.items.wBytesPerSec != 0)
    sectors_per_record = (mft_record_size / boot_sector.items.wBytesPerSec);
  else
  {
    fprintf(stderr, "ERROR! Partition boot sector reports 0 bytes per sector which is not possible\n");
    if (debug == false)
    {
      fprintf (stderr, "Removing the boot sector file and log\n");
      char destination_file[255];
      char log_file[255];
      sprintf (destination_file, "%s", source_bootsec_file);
      sprintf (log_file, "%s.log", source_bootsec_file);
      remove (destination_file);
      remove (log_file);
    }
    return (2);
  }

  if (sectors_per_record > 8)
  {
    fprintf(stderr, "ERROR! This program only allows for 8 sectors per MFT record, and this partition reports %ld\n", sectors_per_record);
    if (debug == false)
    {
      fprintf (stderr, "Removing the boot sector file and log\n");
      char destination_file[255];
      char log_file[255];
      sprintf (destination_file, "%s", source_bootsec_file);
      sprintf (log_file, "%s.log", source_bootsec_file);
      remove (destination_file);
      remove (log_file);
    }
    return (2);
  }

  return (0);
}


// function to process boot sector and read bitmap inode to file
int process_boot_sector(void)
{
  char destination_file[255];
  char log_file[255];
  sprintf (destination_file, "%s", source_mft_file);
  sprintf (log_file, "%s.log", source_mft_file);
  input_offset = ( (cluster_size * boot_sector.items.n64MFTLogicalClustNum + (0 * mft_record_size)) + partition_offset);
  output_offset = 0;
  data_size = (16 * mft_record_size);
  fprintf (stdout, "\nReading bitmap inode from mft...\n");
  ddrescue_read(ddrescue_options, input_offset, output_offset, data_size, source_disk, destination_file, log_file);

  return (0);
}


// function to read inode of mft into memory and process and read the data
int read_mft_file (char *input_file, unsigned long record_number)
{
  FILE *readfile;

  readfile = fopen(input_file, "rb");
  if (readfile == NULL)
  {
    fprintf(stderr, "Cannot open %s for reading (%s).\nAborting...\n", input_file, strerror(errno));
    exit (4);
  }

  if (fseek(readfile, (record_number*mft_record_size), SEEK_SET) != 0 )
  {
    fprintf(stderr, "Error seeking %s (%s).\nAborting...\n", input_file, strerror(errno));
    exit (4);
  }

  if (fread(mft_mft.raw.mftfile, 1, mft_record_size, readfile) == 0 )
  {
    fprintf(stderr, "Error reading %s (%s).\nAborting...\n", input_file, strerror(errno));
    exit (4);
  }

  if (debug == true)
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
    fprintf (debug_file, "wFlags = %04X\n", mft_mft.items.wFlags);
    fprintf (debug_file, "dwRecLength = %08X\n", mft_mft.items.dwRecLength);
    fprintf (debug_file, "dwAllLength = %08X\n", mft_mft.items.dwAllLength);
    fprintf (debug_file, "n64BaseMftRec = %016llX\n", (long long)mft_mft.items.n64BaseMftRec);
    fprintf (debug_file, "wNextAttrID = %04X\n", mft_mft.items.wNextAttrID);
    fprintf (debug_file, "wUnknown = %04X\n", mft_mft.items.wUnknown);
    fprintf (debug_file, "dwMFTRecNumber = %08X\n", mft_mft.items.dwMFTRecNumber);
    fprintf (debug_file, "wFixupPattern = %04X\n", mft_mft.items.wFixupPattern);
  }

  // check for inode signature, if not there then exit with error
  if (strncmp(mft_mft.items.chFileSignature, "FILE", 4) !=0)
  {
    fprintf (stderr, "WARNING! Inode does not have the FILE signature\n");
    fprintf (stderr, "Aborting\n");

    if (debug)
    {
      fprintf (debug_file, "WARNING! Inode does not have the FILE signature\n");
      fprintf (debug_file, "Aborting\n");
    }
    exit(1);
  }

  // check that inode matches what we are looking for
  if (mft_mft.items.dwMFTRecNumber != record_number)
  {
    fprintf (stderr, "WARNING! Requested inode %ld does not match the inode %d reported by the mft record\n", record_number, mft_mft.items.dwMFTRecNumber);
    if (debug)
    {
      fprintf (debug_file, "WARNING! Requested inode %ld does not match the inode %d reported by the mft record\n", record_number, mft_mft.items.dwMFTRecNumber);
    }
  }


 // check inode for corruption
 if (mft_mft.items.wFixupOffset != 0x2A && mft_mft.items.wFixupOffset != 0x30)
 {
   if (debug > 0)
   {
     fprintf (debug_file, "ERROR! Fixup Offset of inode %ld is not 0x2A or 0x30. This program assumes that it should be one of those to work\n", record_number);
   }
   exit(1);
 }

 if (mft_mft.items.wFixupOffset == 0x30)
 {
   WORD i;
   unsigned long n = 0;
   for (i = 0; i < mft_mft.items.wFixupSize-1; i++)
   {
     n += 512;
     if (debug)
       fprintf (debug_file, "Inode %ld fixup %d is 0x%x replaced with 0x%x\n",record_number, i, mft_mft.items.wFixupCheck[(n / 2) -26], mft_mft.items.wFixupCheck[i]);
     if (mft_mft.items.wFixupCheck[(n / 2) -26] != mft_mft.items.wFixupPattern)
     {
       if (mft_mft.items.dwRecLength > n - 512)
       {
	 fprintf (stderr, "WARNING! Inode %ld section(sector) %d appears to be corrupt\n", record_number, i+1);
	 fprintf (stderr, "Aborting...\n");
	 if (debug)
	 {
	   fprintf (debug_file, "WARNING! Inode %ld section(sector) %d appears to be corrupt\n", record_number, i+1);
	   fprintf (debug_file, "Aborting...\n");
	 }
	 exit(1);
       }
       else
	 fprintf (stderr, "WARNING! Inode %ld section(sector) %d appears to be corrupt\n", record_number, i+1);
       fprintf (stderr, "  But it is being ignored because it is past the record size of %d\n", mft_mft.items.dwRecLength);
       if (debug)
       {
	 fprintf (debug_file, "WARNING! Inode %ld section(sector) %d appears to be corrupt\n", record_number, i+1);
	 fprintf (debug_file, "  But it is being ignored because it is past the record size of %d\n", mft_mft.items.dwRecLength);
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
   if (debug)
     fprintf (debug_file, "Inode %ld fixup %d is 0x%x replaced with 0x%x\n",record_number, i, mft_mft.items.wFixupCheck[(n / 2) -26], fixup1);
   if (mft_mft.items.wFixupCheck[(n / 2) -26] != mft_mft.items.wUnknown)
   {
     if (mft_mft.items.dwRecLength > n - 512)
     {
       fprintf (stderr, "WARNING! Inode %ld section(sector) %d appears to be corrupt\n", record_number, i+1);
       fprintf (stderr, "Aborting...\n");
       if (debug)
       {
	 fprintf (debug_file, "WARNING! Inode %ld section(sector) %d appears to be corrupt\n", record_number, i+1);
	 fprintf (debug_file, "Aborting...\n");
       }
       exit(1);
     }
     else
       fprintf (stderr, "WARNING! Inode %ld section(sector) %d appears to be corrupt\n", record_number, i+1);
     fprintf (stderr, "  But it is being ignored because it is past the record size of %d\n", mft_mft.items.dwRecLength);
     if (debug)
     {
       fprintf (debug_file, "WARNING! Inode %ld section(sector) %d appears to be corrupt\n", record_number, i+1);
       fprintf (debug_file, "  But it is being ignored because it is past the record size of %d\n", mft_mft.items.dwRecLength);
     }
   }
   mft_mft.items.wFixupCheck[(n / 2) -26] = fixup1;

   i = 1;
   n = 1024;
   if (debug)
     fprintf (debug_file, "Inode %ld fixup %d is 0x%x replaced with 0x%x\n",record_number, i, mft_mft.items.wFixupCheck[(n / 2) -26], fixup2);
   if (mft_mft.items.wFixupCheck[(n / 2) -26] != mft_mft.items.wUnknown)
   {
     if (mft_mft.items.dwRecLength > n - 512)
     {
       fprintf (stderr, "WARNING! Inode %ld section(sector) %d appears to be corrupt\n", record_number, i+1);
       fprintf (stderr, "Aborting...\n");
       if (debug)
       {
	 fprintf (debug_file, "WARNING! Inode %ld section(sector) %d appears to be corrupt\n", record_number, i+1);
	 fprintf (debug_file, "Aborting...\n");
       }
       exit(1);
     }
     else
       fprintf (stderr, "WARNING! Inode %ld section(sector) %d appears to be corrupt\n", record_number, i+1);
     fprintf (stderr, "  But it is being ignored because it is past the record size of %d\n", mft_mft.items.dwRecLength);
     if (debug)
     {
       fprintf (debug_file, "WARNING! Inode %ld section(sector) %d appears to be corrupt\n", record_number, i+1);
       fprintf (debug_file, "  But it is being ignored because it is past the record size of %d\n", mft_mft.items.dwRecLength);
     }
   }
   mft_mft.items.wFixupCheck[(n / 2) -26] = fixup2;
 }


  unsigned long offset = mft_mft.items.wAttribOffset;

  while (offset < (mft_mft.items.dwRecLength))
  {
    unsigned long n = offset;
    unsigned long i;
    for (i = 0; i < 64; i++)
      ntfs_attribute.raw.attribute_location[i] = mft_mft.raw.mftfile[n++];

    if (debug == true)
    {
      fprintf (debug_file, "\n");
      fprintf (debug_file, "offset = %ld\n", offset);
      fprintf (debug_file, "dwType = %08X\n", ntfs_attribute.items.dwType);
      fprintf (debug_file, "dwFullLength = %08X\n", ntfs_attribute.items.dwFullLength);
      fprintf (debug_file, "uchNonResFlag = %02X\n", ntfs_attribute.items.uchNonResFlag);
      fprintf (debug_file, "uchNameLength = %02X\n", ntfs_attribute.items.uchNameLength);
      fprintf (debug_file, "wNameOffset = %04X\n", ntfs_attribute.items.wNameOffset);
      fprintf (debug_file, "wFlags = %04X\n", ntfs_attribute.items.wFlags);
      fprintf (debug_file, "wID = %04X\n", ntfs_attribute.items.wID);
    }

    if ((ntfs_attribute.items.dwType == 0xFFFFFFFF) || (ntfs_attribute.items.dwFullLength <= 24))
    {
      break;
    }

    // resident
    if (ntfs_attribute.items.uchNonResFlag == 0)
    {
      if (debug == true)
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
      if (debug == true)
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

	n = ntfs_attribute.items.Attr.NonResident.wDatarunOffset + offset;
	for (i = 0; i < (ntfs_attribute.items.dwFullLength - ntfs_attribute.items.Attr.NonResident.wDatarunOffset); i++)
	{
	  fprintf (debug_file, "%02X ", (unsigned char)mft_mft.raw.mftfile[n++]);
	}
	fprintf (debug_file, "\n");

	n = ntfs_attribute.items.Attr.NonResident.wDatarunOffset + offset;
	for (i = 0; i < (ntfs_attribute.items.dwFullLength - ntfs_attribute.items.Attr.NonResident.wDatarunOffset); i++)
	{
	  fprintf (debug_file, "%c", isprint(mft_mft.raw.mftfile[n]) ? mft_mft.raw.mftfile[n] : '.');
	  n++;
	}
	fprintf (debug_file, "\n");
      }
    }

    // if type is name then get the name
    if (ntfs_attribute.items.dwType == 0x30)
      getname(offset);

    // if type is data and it is not named (meaning it is actual file data) then process and read the data
      if (ntfs_attribute.items.dwType == 0x80 && ntfs_attribute.items.uchNameLength == 0x00)
	processdata(offset);

      offset += ntfs_attribute.items.dwFullLength;
  }

  fclose(readfile);
  return (0);
}





// function to process and read data
int processdata(unsigned long offset)
{


  // if type is resident process as such
  if (ntfs_attribute.items.uchNonResFlag == 0)
  {
    unsigned long n = ntfs_attribute.items.Attr.Resident.wAttrOffset + offset;

    char destination_file[255];
    FILE *writefile;

    // use file name from mft
    sprintf (destination_file, "%s", source_bitmap_file);

    writefile = fopen(destination_file, "wb");
    if (writefile == NULL)
    {
      fprintf(stderr, "Cannot open %s for reading (%s).\nAborting...\n", destination_file, strerror(errno));
      exit (4);
    }

    fprintf (stdout, "Writing file %s from MFT resident data\n", destination_file);
    if ( fwrite( &mft_mft.raw.mftfile[n], ntfs_attribute.items.Attr.Resident.dwLength, 1, writefile ) < 1 )
      fprintf( stderr, "Error writing \"%s\".\n", destination_file);

    fclose (writefile);
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
	  fprintf (stderr, "Warning! Size actually read of inode %lld was less than the reported allocated size\n", inode);
	break;
      }

      if (debug)
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
	fprintf (stderr, "file read length exceeded allocated size, aborting...");
	fprintf (stderr, "Warning! File read length exceeded allocated size and will not be read\n");
	if (debug)
	  fprintf (debug_file, "file read length exceeded allocated size, aborting...\n");
	exit (1);
      }


      if (debug)
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

      if (debug)
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

	  if (debug)
	    fprintf (debug_file, "fixed read_offset = %016llX\n", (long long)read_offset.offset);
	}

	if (debug)
	  fprintf (debug_file, "lastoffset = %lld\n", lastoffset);

	newoffset = read_offset.offset + lastoffset;
	lastoffset = newoffset;

	if (debug)
	  fprintf (debug_file, "newoffset = %lld\n", newoffset);



	readoffsetbytes = newoffset * (unsigned long long)cluster_size;

	if (debug)
	  fprintf (debug_file, "readoffsetbytes = %016llX\n", readoffsetbytes);


	// read the part to the output file
	  // or create mft_domain_log_file
	  char destination_file[255];
	  char log_file[255];

	  sprintf (destination_file, "%s", source_bitmap_file);

	  sprintf (log_file, "_part%ld%s.log", part, source_bitmap_file);

	  if (restart == true)
	    remove (log_file);

	  input_offset = readoffsetbytes + partition_offset;
	  output_offset = totalread;
	  data_size = readlengthbytes;

	  if (make_mft_domain == false) //read data to file
	  {
	    fprintf (stdout, "\n............Reading part %ld of %s...........\n", part, file_name);
	    ddrescue_read(ddrescue_options, input_offset, output_offset, data_size, source_disk, destination_file, log_file);
	    fprintf (stdout, "\n............ Done reading part %ld of %s...........\n", part, file_name);
	  }

	  else // make mft domain file
	  {
	    mft_part_number = part;
	    mft_part_offset[part] = input_offset;
	    mft_part_size[part] = data_size;
	  }

	  totalread += readlengthbytes;

	  if (debug)
	    fprintf (debug_file, "totalread = %016llX\n", totalread);


	  /* if we have read to the end then stop and trunicate the file to the proper length
	   *   since we read all the clusters to be compatible with ddrescue direct mode and line up with sectors */
	  if ((unsigned long long)ntfs_attribute.items.Attr.NonResident.n64RealSize <= totalread)
	  {
	    keepreading = false;
	    if (make_mft_domain == false) // don't trunicate if we are making mft_domain
	    {
	      char command[255];
	      sprintf (command, "truncate -s %lld \'%s\'", (long long)ntfs_attribute.items.Attr.NonResident.n64RealSize, destination_file);
	      if (debug == true)
		fprintf (debug_file, "command = %s\n", command);
	      system (command);
	    }
	  }

	  part ++;
    }
  }
  return(0);
}

// function to process bitmap file and create ddrescue log file
int process_bitmap_file(char *input_file)
{
  FILE *readfile;
  unsigned char *buffer;
  unsigned long long filelen;

  fprintf (stdout, "Creating ddrescue domain logfile...\n");
  if (mingap > 4096)
  {
    fprintf (stdout, "mingap too high, setting to max of 4096\n");
    mingap = 4096;
  }

  // open bitmap file
  readfile = fopen(input_file, "rb");
  if (readfile == NULL)
  {
    fprintf(stderr, "Cannot open %s for reading (%s).\nAborting...\n", input_file, strerror(errno));
    exit (4);
  }

  // get file length
  fseek(readfile, 0, SEEK_END);
  filelen=ftell(readfile);
  fseek(readfile, 0, SEEK_SET);

  // allocate memory
  buffer=(unsigned char *)malloc(filelen+1);
  if (!buffer)
  {
    fprintf(stderr, "Memory error!");
    fclose(readfile);
    exit (5);
  }

  // read file into buffer
  fread(buffer, filelen, 1, readfile);
  fclose(readfile);


  // process buffer from memory

  // open destination log file
  char destination_file[255];
  FILE *writefile;

  sprintf (destination_file, "%s", destination_log_file);
  writefile = fopen(destination_file, "w");
  if (writefile == NULL)
  {
    fprintf(stderr, "Cannot open %s for reading (%s).\nAborting...\n", destination_file, strerror(errno));
    exit (4);
  }

  fprintf (writefile, "# Rescue log file created by ntfsbitmap\n");
  fprintf (writefile, "# Command line:\n");
  fprintf (writefile, "# current_pos  current_status\n");
  fprintf (writefile, "0x00000000     +\n");
  fprintf (writefile, "#      pos        size  status\n");


  unsigned long i;
  unsigned long long position_bytes;
  unsigned long long size_bytes;
  unsigned long long used_space = 0;
  unsigned long long free_space = 0;
  unsigned long long position_clusters = 0;
  unsigned long long size_clusters = 0;
  unsigned char bit;
  unsigned char lastbit;
  unsigned char byte;
  bool firstbit = true;
  char status;

  // if a partition offset was supplied then put starting info in log
  // that includes the first "track" of the disk in the domain log
  if (partition_offset != 0)
  {
    unsigned int track = 32256;
    if (partition_offset >= track)
      fprintf (writefile, "0x%08X  0x%08X  %c\n", 0, track, '+');
    else
      fprintf (writefile, "0x%08X  0x%08llX  %c\n", 0, partition_offset, '+');

    if (partition_offset > track)
      fprintf (writefile, "0x%08X  0x%08llX  %c\n", track, partition_offset - track, '?');
  }

  for (i=0; i<filelen+1; i++)
  {
    int n;
    byte = (buffer[i]);
    //printf ("%X ", byte);
    for (n=0; n<8; n++)
    {
      bit = byte & 0x01;
      if (firstbit)
      {
	lastbit = bit;
	firstbit = false;
      }
      //printf ("%x", bit);

      if (bit == lastbit)
      {
	size_clusters ++;
      }

      else
      {
	bool smallgap = false;
	// check for small gap first
	if ((lastbit == 1) && (mingap > 0))
	{
	  unsigned long h = i;
	  int m = n;
	  unsigned int c = 0;
	  unsigned char  tempbit = bit;
	  unsigned char tempbyte = byte;
	  while ((c < mingap+1) && (h < filelen+1) && (smallgap == false))
	  {
	    while (m < 8)
	    {
	      tempbit = tempbyte & 0x01;
	      if (tempbit == bit)
		c ++;
	      else
	      {
		if (c <= mingap)
		smallgap = true;
		i = h;
		n = m;
		bit = tempbit;
		byte = tempbyte;
		size_clusters += (c+1);
		break;
	      }
	      tempbyte >>= 1;
	      m++;
	    }
	    m = 0;
	    h++;
	    tempbyte = (buffer[h]);
	  }
	}


	if (smallgap == false)
	{
	  position_bytes = (position_clusters * (unsigned long long)cluster_size + partition_offset);
	  size_bytes = ((size_clusters) * (unsigned long long)cluster_size);

	  if (lastbit)
	  {
	    status = '+';
	    used_space += size_bytes;
	  }
	  else
	  {
	    status = '?';
	    free_space += size_bytes;
	  }

	  fprintf (writefile, "0x%08llX  0x%08llX  %c\n", (long long)position_bytes, (long long)size_bytes, status);
	  position_clusters += size_clusters;
	  size_clusters = 1;
	  lastbit = bit;
	}
      }

      byte >>= 1;
    }

  }

  fprintf (stdout, "end = %lld\n", (position_bytes + size_bytes) );
  fprintf (stdout, "total_size = %lld\n", total_size);

  // if there is more disk space past the end of the current domain log, then add the rest to the log
  if ((unsigned long long)total_size > (position_bytes + size_bytes) )
  {
    fprintf (writefile, "0x%08llX  0x%08llX  %c\n", (long long)(position_bytes + size_bytes), (long long)(total_size - (position_bytes + size_bytes)), '?');
  }

  fprintf(stdout, "Finished creating logfile\n");

  fprintf (stdout, "total= %lld bytes\n", (used_space + free_space));
  fprintf (stdout, "used= %lld (%2.2f%%)\n", used_space, (float)((double)used_space / ((double)used_space + (double)free_space) * 100));
  fprintf (stdout, "free= %lld (%2.2f%%)\n",free_space, (float)((double)free_space / ((double)used_space + (double)free_space) * 100));

  fclose(writefile);
  free(buffer);

  // put ddrescuelog output into file
  char command[255];
  sprintf (command, "echo \'.\' >> %s", rescue_report_file);
  system (command);
  sprintf (command, "echo \'.......................................................................................................................\' >> %s", rescue_report_file);
  system (command);
  sprintf (command, "echo \'%s\' >> %s", destination_log_file, rescue_report_file);
  system (command);
  sprintf (command, "ddrescuelog -t \'%s\' >> %s", destination_log_file, rescue_report_file);
  system (command);

  return (0);
}


// function to create mft domain file
int process_mft_domain(void)
{
  inode = 0;
  no_position = 0;
  make_mft_domain = true;
  read_mft_file(source_mft_file, inode);
  make_mft_domain = false;

  fprintf (stdout, "\nCreating MFT domain logfile...\n");
  fprintf (stdout, "total mft fragments = %ld\n", mft_part_number +1);
  unsigned int i, n, lowest;
  unsigned long long last_mft_offset = 0;
  unsigned long long lowest_offset;

  // open destination log file
  char destination_file[255];
  FILE *writefile;

  sprintf (destination_file, "%s", mft_domain_log_file);
  writefile = fopen(destination_file, "w");
  if (writefile == NULL)
  {
    fprintf(stderr, "Cannot open %s for reading (%s).\nAborting...\n", destination_file, strerror(errno));
    exit (4);
  }

  fprintf (writefile, "# Rescue MFT log file created by ntfsbitmap\n");
  fprintf (writefile, "# Command line:\n");
  fprintf (writefile, "# current_pos  current_status\n");
  fprintf (writefile, "0x00000000     +\n");
  fprintf (writefile, "#      pos        size  status\n");

  // if a partition offset was supplied then put starting info in log
  // that includes the first "track" of the disk in the domain log
  if (partition_offset != 0)
  {
    unsigned int track = 32256;
    if (partition_offset >= track)
      fprintf (writefile, "0x%08X  0x%08X  %c\n", 0, track, '+');
    else
      fprintf (writefile, "0x%08X  0x%08llX  %c\n", 0, partition_offset, '+');

    if (partition_offset > track)
      fprintf (writefile, "0x%08X  0x%08llX  %c\n", track, partition_offset - track, '?');
  }

  // include the partition boot sector (4096 bytes to be safe even though we only need 512)
  fprintf (writefile, "0x%08llX  0x%08llX  %c\n", (long long)partition_offset, (long long)4096, '+');
  no_position = partition_offset + 4096;

  // produce a sorted list of the offsets and size
  if (debug == true)
    fprintf (debug_file, "\n");
  for (i = 0; i <= mft_part_number; i++)
  {
    lowest_offset = 0xFFFFFFFFFFFFFFF;
    for (n = 0; n <= mft_part_number; n++)
    {
      if (mft_part_offset[n] > last_mft_offset && mft_part_offset[n] < lowest_offset)
      {
	lowest_offset = mft_part_offset[n];
	lowest = n;
      }
    }
    last_mft_offset = mft_part_offset[lowest];
    fprintf (stdout, "mft part %d offset=0x%llX size=0x%llX\n", lowest, mft_part_offset[lowest], mft_part_size[lowest]);

    if (debug == true)
      fprintf (debug_file, "mft part %d offset=0x%llX size=0x%llX\n", lowest, mft_part_offset[lowest], mft_part_size[lowest]);

    no_size = (mft_part_offset[lowest] - no_position);
    if (no_size != 0) // don't put an entry in if the size is zero as ddrescue does not like that
      fprintf (writefile, "0x%08llX  0x%08llX  %c\n", (long long)no_position, (long long)no_size, '?');
    fprintf (writefile, "0x%08llX  0x%08llX  %c\n", (long long)mft_part_offset[lowest], (long long)mft_part_size[lowest], '+');
    no_position = (mft_part_offset[lowest] + mft_part_size[lowest]);
  }

  fprintf (writefile, "0x%08llX  0x%08llX  %c\n", (long long)no_position, (long long)(total_size - no_position), '?');

  fclose(writefile);

  // put ddrescuelog output into file
  char command[255];
  sprintf (command, "echo \'.\' >> %s", rescue_report_file);
  system (command);
  sprintf (command, "echo \'.......................................................................................................................\' >> %s", rescue_report_file);
  system (command);
  sprintf (command, "echo \'%s\' >> %s", mft_domain_log_file, rescue_report_file);
  system (command);
  sprintf (command, "ddrescuelog -t \'%s\' >> %s", mft_domain_log_file, rescue_report_file);
  system (command);

  return (0);
}


// function to display help
void help(void)
{
  unsigned int i;
  for (i = 0; i < ddru_ntfsbitmap_help_txt_len; i++)
  {
    printf ("%c", ddru_ntfsbitmap_help_txt[i]);
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