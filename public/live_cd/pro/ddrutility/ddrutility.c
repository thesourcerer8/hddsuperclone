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

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>

#include "ddrutility_help.h"

char *title = "ddrutility";
char *version_number = "2.8 20161123";
int copyright_year = 2016;

void help(void);
void version(void);

int main (int argc, char **argv)
{

  // begin processing command line arguments
  int command_line_argument;
  bool command_line_error = false;	// initialize error to false

  while (1)
  {
    static struct option long_options[] =
    {
      {"help",    	no_argument,	0, 'h'},
      {"version", 	no_argument,	0, 'v'},
      {0, 0, 0, 0}
    };
    // getopt_long stores the option index here.
    int option_index = 0;

    command_line_argument = getopt_long (argc, argv, "hv",
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

      case '?':
	// getopt_long already printed an error message.
	command_line_error = true;
	break;

      default:
	puts ("aborting");
	abort ();
    }
  }

  // exit on command line error
  if (command_line_error)
  {
    puts ("ERROR: command line input error");
    exit (3);
  }
  // end of command line processing
  printf ("%s - data recovery utility tool set\n", title);
  printf ("try '%s --help' for more information\n", title);
  return(0);
}


// function to display help
void help(void)
{
  unsigned int i;
  for (i = 0; i < ddrutility_help_txt_len; i++)
  {
    printf ("%c", ddrutility_help_txt[i]);
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