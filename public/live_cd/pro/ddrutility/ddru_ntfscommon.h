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

// version 1.1 20140525
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <iconv.h>

time_t start_time, end_time;

struct timeval tvBegin, tvEnd, tvDiff;

typedef unsigned char BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef int32_t LONG;
typedef int64_t LONGLONG;

union {
  struct {
    char attribute_location[64];
  } raw, *p_raw;
  struct __attribute__ ((__packed__)) {
    DWORD dwType;
    DWORD dwFullLength;
    BYTE uchNonResFlag;
    BYTE uchNameLength;
    WORD wNameOffset;
    WORD wFlags;
    WORD wID;
    union {
      struct {
	DWORD dwLength;
	WORD wAttrOffset;
	BYTE uchIndexedTag;
	BYTE uchPadding;
      } Resident;
      struct {
	LONGLONG n64StartVCN;
	LONGLONG n64EndVCN;
	WORD wDatarunOffset;
	WORD wCompressionSize;
	BYTE uchPadding[4];
	LONGLONG n64AllocSize;
	LONGLONG n64RealSize;
	LONGLONG n64StreamSize;
      } NonResident;
    } Attr;
  } items, *p_items;
} ntfs_attribute, *p_ntfs_attribute;

union {
  struct {
    char mftfile[65336];
  } raw, *p_raw;
  struct __attribute__ ((__packed__)) {
    char chFileSignature[4];
    WORD wFixupOffset;
    WORD wFixupSize;
    LONGLONG n64LogSeqNumber;
    WORD wSequence;
    WORD wHardLinks;
    WORD wAttribOffset;
    WORD wFlags;
    DWORD dwRecLength;
    DWORD dwAllLength;
    LONGLONG n64BaseMftRec;
    WORD wNextAttrID;
    WORD wUnknown;
    DWORD dwMFTRecNumber;
    WORD wFixupPattern; // next byte is at offset 460
    WORD wFixupCheck[32738];
  } items, *p_items;
} mft_mft, *p_mft_mft;

union {
  struct {
    char bootsectfile[512];
  } raw, *p_raw;
  struct __attribute__ ((__packed__)) {
    char chJumpInstruction[3];
    char chOemID[4];
    char chDummy[4];
    WORD wBytesPerSec;
    BYTE uchSecPerClust;
    WORD wReservedSec;
    BYTE uchReserved[3];
    WORD wUnused1;
    BYTE uchMediaDescriptor;
    WORD wUnused2;
    WORD wSecPerTrack;
    WORD wNumberOfHeads;
    DWORD dwHiddenSec;
    DWORD dwUnused3;
    DWORD dwUnused4;
    LONGLONG n64TotalSec;
    LONGLONG n64MFTLogicalClustNum; // ( cluster for MFT
    LONGLONG n64MFTMirrLogicalClustNum;
    LONG nClustPerMFTRecord;
    LONG nClustPerIndexRecord;
    LONGLONG n64VolumeSerialNum;
    DWORD dwChecksum;
    char chBootstrapCode[426];
    WORD wSecMark;
  } items, *p_items;
}boot_sector, *p_boot_sector;

union {
  struct {
    char filenamedata[576];
  } raw, *p_raw;
  struct __attribute__ ((__packed__)) {
    uint32_t dwParentDirectory;
    int32_t UnusedPartOfParentDirectory;
    int64_t n64DAteCreated;
    int64_t n64DateModified;
    int64_t n64DateMFTModified;
    int64_t n64DateAccessed;
    int64_t n64LogicalFileSize;
    int64_t n64SizeOnDisk;
    int32_t Flags;
    int32_t ReparseValue;
    unsigned char NameLength;
    unsigned char NameType;
    uint8_t Name[510];
  } items, *p_items;
}file_attribute, *p_fileattribute;



unsigned long inode_count;
char *outtype;
char *inchar;
char *inchar_start;
char *converted;
char *converted_start;
iconv_t cd;
bool no_convert;
char file_name[1024];
unsigned long mft_record_size;
unsigned char file_name_type;
int getname(unsigned long offset);

