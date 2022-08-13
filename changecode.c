// Copyright (C) 2015-2022 Scott Dwyer.
// You may use/distribute/modify this freely, under the terms of
// the GNU General Public License version 2 or later version.
// This software is distributed WITHOUT ANY WARRANTY.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 2048
#define MAX_WORDS 5000
#define MAX_WORD_LENGTH 128

char *script_line_buffer;
char **script_line_pointer;
unsigned int total_lines;
unsigned int total_words;
unsigned int script_rows;
char changeword[MAX_WORDS][MAX_WORD_LENGTH];

int read_input_file(char *input_file);
int write_file(char *input_file);
int process_lines(void);
int read_change_file(char *input_file);



int main(void)
{
  char *checkdir = "/home/tech/hddsupertool";
  char *wordfile = "code_change_words";

  int count = 23;
  char file[count][255];
  strcpy(file[0], "tool.c");
  strcpy(file[1], "tool.h");
  strcpy(file[2], "common.h");
  strcpy(file[3], "commands.c");
  strcpy(file[4], "commands.h");
  strcpy(file[5], "io.c");
  strcpy(file[6], "io.h");
  strcpy(file[7], "common.c");
  strcpy(file[8], "hddsuperclone.c");
  strcpy(file[9], "hddsuperclone.h");
  strcpy(file[10], "clone_gui2.c");
  strcpy(file[11], "clone_gui2.h");
  strcpy(file[12], "clone_gui3.c");
  strcpy(file[13], "clone_gui3.h");
  strcpy(file[14], "clone_gui_common.h");
  strcpy(file[15], "clone_gui_language.h");
  strcpy(file[16], "clone_gui_language.c");
  strcpy(file[17], "hddsuperclone2.glade");
  strcpy(file[18], "hddsuperclone3.glade");
  strcpy(file[19], "usbrelay.c");
  strcpy(file[20], "usbrelay.h");
  strcpy(file[21], "smtp.c");
  strcpy(file[22], "smtp.h");





  char directory[1024];
  if (getcwd(directory, sizeof(directory)) == NULL)
  {
    fprintf (stderr, "Error finding current working directory (%s)\n", strerror(errno));
    exit (1);
  }
  fprintf (stdout, "%s\n", directory);
  if (strcmp(directory, checkdir) == 0)
  {
    fprintf (stderr, "Not allowed in this directory\n");
    exit (1);
  }

  read_change_file(wordfile);

  int i;
  for (i = 0; i < count; i++)
  {
    read_input_file(file[i]);
    process_lines();
    write_file(file[i]);
  }
  exit (0);
}



// function to read the script file into memory
int read_input_file(char *input_file)
{
  // assign memory for the script
  free (script_line_buffer);
  free (script_line_pointer);
  script_rows = 1000;
  unsigned int cols = MAX_LINE_LENGTH;
  unsigned int i;
  script_line_buffer = malloc(script_rows * cols * sizeof(*script_line_buffer));
  if (script_line_buffer == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    exit (1);
  }
  script_line_pointer = malloc(script_rows * sizeof(*script_line_pointer));
  if (script_line_pointer == NULL)
  {
    fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
    exit (1);
  }
  for (i = 0; i < script_rows; ++i)
  {
    script_line_pointer[i] = &script_line_buffer[i * cols];
  }


  if (input_file == NULL)
  {
    fprintf (stdout, "Error: No script file specified.\n");
    exit (1);
  }

  fprintf (stdout, "Reading %s into memory...\n", input_file);
  FILE *readfile;
  readfile = fopen(input_file, "r");
  if (readfile == NULL)
  {
    fprintf(stderr, "Cannot open %s for reading (%s).\n", input_file, strerror(errno));
    exit (1);
  }

  char line[cols];
  i = 0;
  while (fgets(line, sizeof line, readfile))
  {
    bool line_too_long = true;
    unsigned int n;
    for (n = 0; n < cols; n++)
    {
      // if we find end of line before the end then the line isn't too long
      if (line[n] == '\n')
      {
        line_too_long = false;
        break;
      }
    }
    for (n = 0; n < cols - 1; n++)
    {
      // if we find end of file before the end then the line isn't too long
      if (line[n] == '\0')
      {
        line_too_long = false;
        break;
      }
    }
    if (line_too_long)
    {
      fprintf (stderr, "ERROR! Line %d of %s is too long.\n", i+1, input_file);
      exit (1);
    }

    strcpy (script_line_pointer[i], line);

    i++;

    // if used up allocated memory then increase it
    if (i >= script_rows)
    {
      script_rows += 1000;
      unsigned int i;
      script_line_buffer = realloc(script_line_buffer, script_rows * cols * sizeof(*script_line_buffer));
      if (script_line_buffer == NULL)
      {
        fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
        exit (1);
      }

      script_line_pointer = realloc(script_line_pointer, script_rows * sizeof(*script_line_pointer));
      if (script_line_pointer == NULL)
      {
        fprintf (stderr, "Error allocating memory (%s)\n", strerror(errno));
        exit (1);
      }

      for (i = 0; i < script_rows; ++i)
      {
        script_line_pointer[i] = &script_line_buffer[i * cols];
      }
    }
  }
  total_lines = i;


  fclose(readfile);
  return (0);
}






// function to write the script back to file after indentation cleanup
int write_file(char *input_file)
{
  fprintf (stdout, "Overwriting %s with changes...\n", input_file);

  FILE *writefile;
  writefile = fopen(input_file, "w");
  if (writefile == NULL)
  {
    fprintf(stderr, "Cannot open %s for writing (%s).\n", input_file, strerror(errno));
    exit (1);
  }

  unsigned int line_number;
  for (line_number = 0; line_number < total_lines; line_number++)
  {
    fprintf (writefile, "%s", script_line_pointer[line_number]);
    //fprintf (stdout, "%s", script_line_pointer[line_number]);
  }

  fclose(writefile);
  return (0);
}






// function to read the change file into memory
int read_change_file(char *input_file)
{
  fprintf (stdout, "Reading %s into memory...\n", input_file);
  FILE *readfile;
  readfile = fopen(input_file, "r");
  if (readfile == NULL)
  {
    fprintf(stderr, "Cannot open %s for reading (%s).\n", input_file, strerror(errno));
    exit (1);
  }

  unsigned int cols = MAX_WORD_LENGTH;
  char line[cols];
  int i = 0;
  while (fgets(line, sizeof line, readfile))
  {
    bool line_too_long = true;
    unsigned int n;
    for (n = 0; n < cols; n++)
    {
      // if we find end of line before the end then the line isn't too long
      if (line[n] == '\n')
      {
        line_too_long = false;
        break;
      }
    }
    for (n = 0; n < cols - 1; n++)
    {
      // if we find end of file before the end then the line isn't too long
      if (line[n] == '\0')
      {
        line_too_long = false;
        break;
      }
    }
    if (line_too_long)
    {
      fprintf (stderr, "ERROR! Line %d of %s is too long.\n", i+1, input_file);
      exit (1);
    }

    char leftover[cols];
    sscanf(line, "%s %[^\n]", changeword[i], leftover);
    //fprintf (stdout, "%d %s\n", i+1, changeword[i]);
    i++;
  }
  total_words = i;


  fclose(readfile);
  return (0);
}










// function to process the script lines
int process_lines(void)
{
  unsigned int line_number;
  char line[MAX_LINE_LENGTH];

  for (line_number = 0; line_number < total_lines; line_number++)
  {
    strcpy (line, "");
    sscanf(script_line_pointer[line_number], "%[^\n]", line);

    int length = strlen(line);
    if (length > 0)
    {
      //fprintf (stdout, "%s\n", line);
      unsigned int wordcount;
      for (wordcount = 0; wordcount < total_words; wordcount++)
      {

          int wordlength = strlen(changeword[wordcount]);
          int lettercount;
          for (lettercount = 0; lettercount < length; lettercount++)
          {
            if (strncmp(changeword[wordcount], line+lettercount, wordlength) == 0)
            {
              //fprintf (stdout, "lettercount=%d length=%d %s\n", lettercount, length, line);
              char new_line[MAX_LINE_LENGTH] = "";
              strncpy(new_line, line, lettercount);
              char newword[MAX_WORD_LENGTH] = "";
              sprintf(newword, "c%d", wordcount+1);
              strcat(new_line, newword);
              strcat(new_line, line+lettercount+wordlength-1);
              strcpy(line, new_line);
              length = strlen(line);
              lettercount = 0;
            }
          }

      }
    }
    strcpy(script_line_pointer[line_number], line);
    strcat(script_line_pointer[line_number], "\n");
  }


  return (0);
}




