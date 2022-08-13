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

// version 1.1 20141011

// funtion to get file name
int getname(unsigned long offset)
{
  // if type is resident process as such
  if (ntfs_attribute.items.uchNonResFlag == 0)
  {
    // if no-convert then do it the old way so iconv is not used
    if (no_convert == true)
    {
      unsigned int n;
      unsigned int i;
      unsigned int c = 0;

      n = ntfs_attribute.items.Attr.Resident.wAttrOffset + offset;
      for (i = 0; i < 576 && n < mft_record_size; i++)
      {
	file_attribute.raw.filenamedata[i] = mft_mft.raw.mftfile[n];
	n++;
      }

      for (i = 0; i < (file_attribute.items.NameLength * 2); i += 2)
      {
	file_name[c] = isprint(file_attribute.items.Name[i]) ? file_attribute.items.Name[i] : '.';
	file_name[c] = (file_name[c] != '/') ? file_name[c] : '.';
	file_name[c] = (file_name[c] != '\\') ? file_name[c] : '.';
	file_name[c] = (file_name[c] != '\'') ? file_name[c] : '.';
	file_name[c] = (file_name[c] != '\"') ? file_name[c] : '.';
	c++;
      }
      file_name[c] = '\0';
    }

    // else do proper conversion of the file name
    else
    {
      unsigned int n;
      unsigned int i;

      n = ntfs_attribute.items.Attr.Resident.wAttrOffset + offset;
      for (i = 0; i < 576 && n < mft_record_size; i++)
      {
	file_attribute.raw.filenamedata[i] = mft_mft.raw.mftfile[n];
	n++;
      }

      memset (converted_start, 0, 1024);
      memset (inchar_start, 0, 512);
      converted = converted_start;
      inchar = inchar_start;
      size_t inlen = (file_attribute.items.NameLength * 2);
      size_t outlen = 1024;

      for (i = 0; i < (file_attribute.items.NameLength * 2) && i < 512; i++)
      {
	inchar[i] = file_attribute.items.Name[i];
      }

      int skipped = 0;
      bool keepconverting = true;
      size_t ret;
      while (keepconverting)
      {
	ret = iconv(cd, &inchar, &inlen, &converted, &outlen);

	if (ret == (size_t) -1)
	{
	  switch (errno)
	  {
	    /* See "man 3 iconv" for an explanation. */
	    case EILSEQ:
	      if (inlen > 2)
	      {
		inlen -= 2;
		inchar += 2;
		skipped ++;
	      }
	      else
		keepconverting = false;
	      break;
	    case EINVAL:
	      fprintf (stderr, "iconv failed: inode %ld in string '%s', length %d, out string '%s', length %d -- ", inode_count, inchar, (int)inlen, converted_start, (int)outlen);
	      fprintf (stderr, "Incomplete multibyte sequence.\n");
	      keepconverting = false;
	      break;
	    case E2BIG:
	      fprintf (stderr, "iconv failed: inode %ld in string '%s', length %d, out string '%s', length %d -- ", inode_count, inchar, (int)inlen, converted_start, (int)outlen);
	      fprintf (stderr, "No more room.\n");
	      keepconverting = false;
	      break;
	    default:
	      fprintf (stderr, "iconv failed: inode %ld in string '%s', length %d, out string '%s', length %d -- ", inode_count, inchar, (int)inlen, converted_start, (int)outlen);
	      fprintf (stderr, "Error: %s.\n", strerror (errno));
	      keepconverting = false;
	  }
	}
	else
	  keepconverting = false;
      }

      if (skipped > 0)
      {
	fprintf (stdout, "\rInode %ld skipped %d invalid name characters out of %d: ", inode_count, skipped, file_attribute.items.NameLength);
	fprintf (stdout, "'%s'\n", converted_start);
      }

      if ((int)ret > 0)
      {
	fprintf (stdout, "\rInode %ld converted %d invalid name characters out of %d: ", inode_count, (int)ret, file_attribute.items.NameLength);
	fprintf (stdout, "'%s'\n", converted_start);
      }

      if (ret > 0 && (int)ret < 0)
      {
	fprintf (stdout, "\rInode %ld ignored an unknown number of name characters out of %d: ", inode_count, file_attribute.items.NameLength);
	fprintf (stdout, "'%s'\n", converted_start);
      }

      for (i = 0; i < 1024; i++)
      {
	file_name[i] = converted_start[i];
      }
    }

    file_name_type = file_attribute.items.NameType;
  }

  // else process as non resident
  else
  {
    fprintf (stderr, "\r\nERROR! non resident name processing should go here, which should probably never happen\n");
    fprintf (stderr, "If you see this message, please contact the author and tell him it did happen\n\n");
  }

  return(0);
}

