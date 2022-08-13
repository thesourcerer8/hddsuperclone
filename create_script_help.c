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
#include <dirent.h>


char name[256][256];

int main(void)
{
  struct dirent **namelist;
  int i,n;
  int count = 0;
  int index = 0;
  char command[512];


  n = scandir("hddscripts", &namelist, 0, alphasort);
  if (n < 0)
  {
    perror("scandir");
    return (1);
  }
  else {
    for (i = 0; i < n; i++)
    {
      if (namelist[i]->d_name[0] != '.')
      {
	//printf("%s\n", namelist[i]->d_name);
	strcpy(name[count],namelist[i]->d_name);
	count++;
      }
      free(namelist[i]);
    }
  }
  free(namelist);

  strcpy (command, "cat hddsupertool-p1.texi > hddsupertool.texi");
  system (command);
  char *newdirectory = "hddscripts";
  chdir (newdirectory);
  char *texifile = "../hddsupertool.texi";
  FILE *texi_file = fopen(texifile, "a");
  if (texi_file == NULL)
  {
    fprintf(stderr, "Cannot open %s for writing (%s).\nAborting...\n", texifile, strerror(errno));
    exit (1);
  }
  while( count > 0 )
  {
    //fprintf(stdout, "\n\n%s\n",name[index]);
    //fprintf(stdout, "@example\n");
    fprintf(texi_file, "\n\n%s\n",name[index]);
    fprintf(texi_file, "@example\n");
    strcpy (command, "../hddsuperclone --tool -Q -t /dev/zero -f ");
    strcat(command, name[index]);
    strcat(command, " help=1 printhelp=1");
    fprintf (stdout, "%s\n", command);

    FILE *fp;
    char path[1035];
    /* Open the command for reading. */
    fp = popen(command, "r");
    if (fp == NULL) {
      printf("Failed to run command\n" );
      exit(1);
    }
    /* Read the output a line at a time - output it. */
    while (fgets(path, sizeof(path)-1, fp) != NULL) {
      //fprintf(stdout, "%s", path);
      fprintf(texi_file, "%s", path);
    }
    /* close */
    pclose(fp);

    //fprintf(stdout, "@end example\n");
    fprintf(texi_file, "@end example\n");

    index++;
    count--;
  }
  fclose (texi_file);
  chdir ("..");
  strcpy (command, "cat hddsupertool-p2.texi >> hddsupertool.texi");
  system (command);

  return(0);
}




/*
#include <dirent.h>
#include <stdio.h>

char name[256][256];

int main(void)
{
  DIR           *d;
  struct dirent *dir;
  int count = 0;
  int index = 0;
  d = opendir(".");
  if (d)
  {
    while ((dir = readdir(d)) != NULL)
    {
      printf("%s\n", dir->d_name);
      strcpy(name[count],dir->d_name);
      count++;
    }

    closedir(d);
  }

  while( count > 0 )
  {
      printf("The directory list is %s\r\n",name[index]);
      index++;
      count--;
  }

  return(0);
}
*/
