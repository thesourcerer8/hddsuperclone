#! /bin/sh

# Copyright (C) 2016 Scott Dwyer

# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

## get start time
TT="$(date +%s)"

VERSION="1.11 20141015"
version ()
{
echo "ddru_findbad $VERSION
Copyright (C) 2014 Scott Dwyer.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law."
}

help ()
{
echo "ddru_findbad - Part of the ddrutility tool set

Ddru_findbad is meant to be a compliment to gnuddrescue. It tries to find which files are related to the bad sectors.

Usage: ddru_findbad <target> <logfile> [options]

OPTIONS:
 -v, --version		   Show version
 -h, --help		   Show help
 -o, --output <file>	   Results files base name, default is results
 -w, --loopwait <seconds>  Number of seconds to wait before doing loop commands
 -s, --sectorsize <bytes>  Sector size, default is 512
 -a, --autooff             Allows manual control of partition types
 -m, --moreinfo            Puts lots of extra data in the long results (slow)
 -e, --extraoutput         Produces more types of output files (really slow)
 -q, --quick               Only check first and last sector of each group
 -Q, --quickntfs           Quick with special NTFS processing

Report bugs to maximus57@hotmail.com
Or better yet, report them at the project home page:
http://sourceforge.net/projects/ddrutility/
"
}


###################################################################
#The following are sources that this code was created from
###################################################################
#Antonio Diaz Diaz
#GNU ddrescue Manual
#http://www.gnu.org/software/ddrescue/manual/ddrescue_manual.html#Ddrescuelog
###################################################################
#Raphael Hoegger
#Useful code-snippets in datarecovery
#http://pfuender.net/?p=80
###################################################################
#Bruce Allen and Douglas Gilbert
#Bad block HOWTO for smartmontools
#http://smartmontools.sourceforge.net/badblockhowto.html
###################################################################
#Technically Tim
#Disk recovery – Which files are damaged?
#http://tim.purewhite.id.au/2011/04/disk-recovery-which-files-are-damaged/
###################################################################
###################################################################
#NOTE: IF COPYING AND PASTING THIS SCRIPT FROM THE WEB,
#CERTAIN CHARACTERS MAY NOT BE THE SAME AND WOULD NEED
#TO BE RETYPED MANUALLY, SUCH AS THE QUOTES AND MINUS SIGN.
###################################################################
###################################################################


#######################################################################
###################### Get Commands ###################################
#######################################################################
commands ()
{

## preset variables
TARGETDRIVE=""
LOGFILE=""
OUTPUT=results
LOOPWAIT=2
SECTORSIZE=512
MOREINFO="false"
EXTRAOUT="false"
QUICK="false"
QUICKNTFS="false"

## process command line input
while test "$1" != "" ; do
        case "$1" in
                --output|-o)
                        OUTPUT="$2"
			shift 2
                ;;
                --loopwait|-w)
                        LOOPWAIT="$2"
			shift 2
                ;;
                --sectorsize|-s)
                        SECTORSIZE="$2"
			shift 2
		;;
                --autooff|-a)
                        MANUAL="true"
			shift 1
                ;;
                --version|-v)
                        version
                        exit 0
                ;;
                --help|-h)
                        help
                        exit 0
                ;;
                --moreinfo|-m)
                        MOREINFO="true"
			shift 1
                ;;
                --extraoutput|-e)
                        EXTRAOUT="true"
			shift 1
                ;;
                --quick|-q)
                        QUICK="true"
			shift 1
                ;;
                --quickntfs|-Q)
                        QUICK="true"
			QUICKNTFS="true"
			shift 1
                ;;
                -*)
                        echo "### Error: no such option "$1" ###"
                        echo "Try 'ddru_findbad --help' for more information"
                        exit 1
                ;;
                *)
			if [ "$2" = "" ]; then
				echo Target="$1"
				echo Logfile="$2"
				echo "### Error: you must provide both target and logfile ###"
				echo "Try 'ddru_findbad --help' for more information"
				exit 1
			fi
                        TARGETDRIVE="$1"
			LOGFILE="$2"
                        shift 2
                ;;

        esac
done
echo "Command line input was processed succesfully"

if [ "$TARGETDRIVE" = "" ] || [ "$LOGFILE" = "" ]; then
	echo Target="$TARGETDRIVE"
	echo Logfile="$LOGFILE"
	echo "### Error: you must provide both target and logfile ###"
	echo "Try 'ddru_findbad --help' for more information"
	exit 1
fi

}

#######################################################################
###################### Pre-Startup ####################################
#######################################################################
prestart ()
{

## check if run as root
if [ "$(id -u)" -ne 0 ]; then
   echo "Error! This script must be run as root"
   exit 1
fi

## check for requirements
NTFSCHECK=$(which ntfscluster)
if [ "$NTFSCHECK" = "" ]; then
	echo "Warning! ntfscluster is not detected!"
	echo "Therefore NTFS partitions will not be able to be processed,"
	echo "and this script may produce errors if trying to do so."
	echo "See help file for more info."
fi

GFDISK="gfdisk"
GFDISKCHECK=$(which gfdisk)
if [ "$GFDISKCHECK" = "" ]; then
	echo "Waring! GNU fdisk is not detected!"
	echo "Therefore GPT partitioned disks will not be able to be processed"
	echo "and this script may produce errors if trying to do so."
	echo "See help file for more info."
	GFDISK="fdisk"
fi

SLEUTHCHECK=$(which ifind)
if [ "$SLEUTHCHECK" = "" ]; then
	echo "Error! sleuthkit is not detected!"
	echo "This program requires sleuthkit for proper operation"
	echo "See help file for more info."
	exit 1
fi

## setup output filenames
OUTPUTDEBUG="$OUTPUT""_debug.txt"
OUTPUTLIST="$OUTPUT""_list.txt"
OUTPUTINFO="$OUTPUT""_info.txt"
OUTPUTSUMMARY="$OUTPUT""_summary.txt"
OUTPUTBYSECTOR="$OUTPUT""_bysector.txt"

## cleanup any files that could be left over from a forced exit
rm -f tempddrfdisk
rm -f tempddrdev
rm -f tempddrstart
rm -f tempddrend
rm -f tempddrid
rm -f tempddrtype
rm -f tempddrout
rm -f tempddrbad
rm -f tempddrcmdi
rm -f tempddrcmdn
rm -f tempddricheck
rm -f tempddrinode
rm -f tempddrncheck
rm -f tempddrffind
rm -f tempddrifind
rm -f tempddrcluster
rm -f tempddrlog
rm -f tempddrlogb
rm -f tempddrloop
rm -f tempddrprocess
rm -f tempddrsector
rm -f tempddrntfsquick

## remove the output files for a fresh start
rm -f "$OUTPUTDEBUG"
rm -f "$OUTPUTLIST"
rm -f "$OUTPUTINFO"
rm -f "$OUTPUTSUMMARY"
rm -f "$OUTPUTBYSECTOR"

## Put title info in beginning of output files
echo "########## ddru_findbad $VERSION info output file ##########" >> "$OUTPUTINFO"
echo "########## ddru_findbad $VERSION debug output file ##########" >> "$OUTPUTDEBUG"

## Create some files to prevent no file errors
touch "$OUTPUTLIST"

# Show current variables
echo Target = "$TARGETDRIVE" | tee -a "$OUTPUTDEBUG"
echo Logfile = "$LOGFILE" | tee -a "$OUTPUTDEBUG"
echo Output base name = "$OUTPUT" | tee -a "$OUTPUTDEBUG"
echo Sector size = "$SECTORSIZE" | tee -a "$OUTPUTDEBUG"
echo Loop wait time = "$LOOPWAIT" | tee -a "$OUTPUTDEBUG"
echo More info = "$MOREINFO" | tee -a "$OUTPUTDEBUG"
echo Extra output = "$EXTRAOUT" | tee -a "$OUTPUTDEBUG"
echo Quick = "$QUICK" | tee -a "$OUTPUTDEBUG"
echo Quick ntfs = "$QUICKNTFS" | tee -a "$OUTPUTDEBUG"

}

#######################################################################
###################### Process Target #################################
#######################################################################
processtarget ()
{

## get process start time
T="$(date +%s)"

## check to see if image or device
IMAGE=none
if [ -b "$TARGETDRIVE" ]; then
	IMAGE=false
	echo "Target $TARGETDRIVE is detected to be a block device" | tee -a "$OUTPUTDEBUG"
	## check if device exists
fi

if [ -f "$TARGETDRIVE" ]; then
	IMAGE=true
	echo "Target $TARGETDRIVE is detected to be a file" | tee -a "$OUTPUTDEBUG"
fi

if [ "$IMAGE" = "none" ]; then
	echo "Error! Target $TARGETDRIVE not found!" | tee -a "$OUTPUTDEBUG"
	exit 1
fi

## delete previous loops if they exist in case of forced exit
## this is based on any loops found linked to the target image
## if you have created any loops to the same target image it will delete them also
if [ "$IMAGE" = "true" ]; then
	losetup -a | grep -a "$TARGETDRIVE" | awk ' { print $1 } ' > tempddrloop
	for LOOP in $(cat tempddrloop) ; do
		echo Existing loop linked to target found, deleting ${LOOP%?}
		sleep $LOOPWAIT ## wait to give loop time to not be busy
		losetup -d ${LOOP%?}
	done
	rm -f tempddrloop
fi

## setup either device or image
if [ "$IMAGE" = "true" ]; then
	LOOPA=$(losetup -f)
	sleep $LOOPWAIT ## wait to give loop time to not be busy
	losetup -r $LOOPA "$TARGETDRIVE"
	TARGET=$LOOPA
else
	TARGET="$TARGETDRIVE"
fi

## check to see if whole drive or partition
DRIVE=$(mmstat "$TARGET" 2>/dev/null)
PARTTYPE=$(fsstat "$TARGET" 2>/dev/null | head -40 | grep -a "File System Type:" | awk ' { print $4 } ')
if [ "$DRIVE" = "" ] && [ "$PARTTYPE" = "" ]; then
	echo "Neither mmstat or fsstat gave results"
	echo "The partition table or file system could be corrupt"
	echo "Cannot determine if target is whole drive or single partition"
	echo "See file $OUTPUTDEBUG for more info"
	echo "########## mmstat $TARGET ##########" >> "$OUTPUTDEBUG"
	mmstat "$TARGET" 2>> "$OUTPUTDEBUG" >> "$OUTPUTDEBUG"
	echo "########## fsstat $TARGET ##########" >> "$OUTPUTDEBUG"
	fsstat "$TARGET" 2>> "$OUTPUTDEBUG" >> "$OUTPUTDEBUG"
	exit 1
fi
if [ "$DRIVE" != "" ] && [ "$PARTTYPE" != "" ]; then
	echo "Both mmstat and fsstat gave results, this is conflicting"
	echo Cannot determine if target is whole drive or single partition
	echo See file "$OUTPUTDEBUG"
	echo "########## mmstat $TARGET ##########" >> "$OUTPUTDEBUG"
	mmstat "$TARGET" 2>> "$OUTPUTDEBUG" >> "$OUTPUTDEBUG"
	echo "########## fsstat $TARGET ##########" >> "$OUTPUTDEBUG"
	fsstat "$TARGET" 2>> "$OUTPUTDEBUG" >> "$OUTPUTDEBUG"
	exit 1
fi
if [ "$DRIVE" != "" ]; then
	PARTITIONONLY=false
	echo "Target $TARGETDRIVE is found to be a whole $DRIVE partitioned disk" | tee -a "$OUTPUTDEBUG"
else
	PARTITIONONLY=true

	echo "Target $TARGETDRIVE is found to be a single $PARTTYPE partition" | tee -a "$OUTPUTDEBUG"
fi

## calculate process run time
T="$(($(date +%s)-T))"
printf "Processing time Process Target: %02d:%02d:%02d:%02d\n" "$((T/86400))" "$((T/3600%24))" "$((T/60%60))" "$((T%60))"  >> "$OUTPUTDEBUG"

}

#######################################################################
######################### Find Partition Type #########################
#######################################################################
findpartitiontype ()
{

## get process start time
T="$(date +%s)"

PARTTYPE=$(fsstat "$PARTITION" 2>>"$OUTPUTDEBUG" | head -40 | grep -a "File System Type:" | awk ' { print $4 } ')

case "$PARTTYPE" in
	NTFS)
		PARTITIONTYPE=ntfs
	;;
	Ext2|Ext3|Ext4)
		PARTITIONTYPE=linux
	;;
	FAT16|FAT32)
		PARTITIONTYPE=fat
	;;
	HFS|HFS+)
		PARTITIONTYPE=hfs
	;;
	*)
		PARTITIONTYPE=none
esac

echo "..................................................................." | tee -a "$OUTPUTINFO" >> "$OUTPUTDEBUG"
echo "..................................................................." | tee -a "$OUTPUTINFO" >> "$OUTPUTDEBUG"

echo "Partition $PART type $PARTTYPE is found to be $PARTITIONTYPE" | tee -a "$OUTPUTDEBUG" "$OUTPUTINFO"

## add either extra or short info to output
if [ "$MOREINFO" = "true" ]; then
	echo "Performing full fsstat command, this could take awhile if the filesystem is big"
	echo "##########################################################" >> "$OUTPUTINFO"
	echo "########## Partition $PART fsstat output ##########" >> "$OUTPUTINFO"
	fsstat "$PARTITION" 2>>"$OUTPUTINFO" >> "$OUTPUTINFO"
	echo "Performing full fls command, this could take awhile if the filesystem is big"
	echo "##########################################################" >> "$OUTPUTINFO"
	echo "########## Partition $PART fls output ##########" >> "$OUTPUTINFO"
	fls -rp "$PARTITION" 2>>"$OUTPUTINFO" >> "$OUTPUTINFO"
else
	echo "##########################################################" >> "$OUTPUTINFO"
	echo "########## Partition $PART fsstat short output ##########" >> "$OUTPUTINFO"
	fsstat "$PARTITION" 2>>"$OUTPUTINFO" | head -40 >> "$OUTPUTINFO"
fi

## calculate process run time
T="$(($(date +%s)-T))"
printf "Processing time Find Partition Type: %02d:%02d:%02d:%02d\n" "$((T/86400))" "$((T/3600%24))" "$((T/60%60))" "$((T%60))"  >> "$OUTPUTDEBUG"

}

#######################################################################
###################### Process Logfile ################################
#######################################################################
processlogfile ()
{

## get process start time
T="$(date +%s)"

## check if image file exists
if [ ! -f "$LOGFILE" ]; then
	echo "Error! Logfile $LOGFILE not found!" | tee -a "$OUTPUTDEBUG"
	exit 1
fi

## check to see if it is ddrescue logfile or not
## if not then process it as a sector file
## if yes then process as ddrescue log file

DDRESCUE=$(cat "$LOGFILE" | grep -a ddrescue | head -1)

if [ "$DDRESCUE" = "" ]; then
	## copy sector file to temp file

	echo Processing sector file | tee -a "$OUTPUTDEBUG"
	cat "$LOGFILE" > tempddrsector

else
	## get all bad and unread sectors from ddrescue logfile and process

	echo Processing ddrescue log file
	echo Using log processor >> "$OUTPUTDEBUG"
	grep -a 0x "$LOGFILE" | tail -n +2 > tempddrlog
	grep -a "-" tempddrlog >> tempddrlogb
	grep -a "/" tempddrlog >> tempddrlogb
	grep -a "*" tempddrlog >> tempddrlogb


	NONSPLIT=$(grep -a "/" tempddrlogb)
	if [ "$NONSPLIT" != "" ]; then
		echo ................................................................... >> "$OUTPUTDEBUG"
		echo "WARNING! The ddrescue logfile indicates unread non-split sectors" | tee -a "$OUTPUTDEBUG"
		echo "Ddrescue possibly was only ran once with the -n option" | tee -a "$OUTPUTDEBUG"
		echo "All unread sectors will be processed as bad sectors" | tee -a "$OUTPUTDEBUG"
	fi

	NONTRIMMED=$(grep -a "*" tempddrlogb)
	if [ "$NONTRIMMED" != "" ]; then
		echo ................................................................... >> "$OUTPUTDEBUG"
		echo "WARNING! The ddrescue logfile indicates unread non-trimmed sectors" | tee -a "$OUTPUTDEBUG"
		echo "Ddrescue possibly was interupted and did not finish" | tee -a "$OUTPUTDEBUG"
		echo "All unread sectors will be processed as bad sectors" | tee -a "$OUTPUTDEBUG"
	fi

	while read line
	do
		LOG=$line
		echo $LOG
		POSITION=$(echo $LOG | awk ' { print $1 } ')
		BYTES=$(echo $LOG | awk ' { print $2 } ')
		COUNT=$(( ($BYTES/$SECTORSIZE) ))
		SECTOR=$(( ($POSITION/$SECTORSIZE) ))
		END=$(( ($SECTOR+$COUNT-1) ))
		echo $SECTOR $END >> tempddrsector
	done < tempddrlogb

	rm -f tempddrlog
	rm -f tempddrlogb

fi

## process the sector range list into full list of sectors
## or just first and last sector if quick option

if [ ! -f tempddrsector ]; then
  echo "No bad sectors were found in the logfile"
  exit 0

else
  if [ "$QUICK" = "true" ];then

	  ## quick process
	  while read line
	  do

		  LOG=$line
		  echo $LOG
		  POSITION=$(echo $LOG | awk ' { print $1 } ')
		  END=$(echo $LOG | awk ' { print $2 } ')

		  if [ "$END" = "" ]; then END=$POSITION; fi

		  echo $POSITION >> tempddrbad
		  if [ "$END" != "$POS" ]; then echo $END >> tempddrbad; fi

	  done < tempddrsector

  else
	  ## full process
	  while read line
	  do

		  LOG=$line
		  echo $LOG
		  SECTOR=$(echo $LOG | awk ' { print $1 } ')
		  END=$(echo $LOG | awk ' { print $2 } ')

		  if [ "$END" = "" ]; then END=$POSITION; fi

		  while [ $SECTOR -le $END  ]; do
			  echo $SECTOR >> tempddrbad
			  SECTOR=$((SECTOR+1))
		  done
	  done < tempddrsector
  fi
fi

## calculate process run time
T="$(($(date +%s)-T))"
printf "Processing time Process Logfile: %02d:%02d:%02d:%02d\n" "$((T/86400))" "$((T/3600%24))" "$((T/60%60))" "$((T%60))"  >> "$OUTPUTDEBUG"

}

#######################################################################
######################## NTFS #########################################
#######################################################################
ntfs ()
{

## get process start time
T="$(date +%s)"

###### check if quickntfs ######
if [ "$QUICKNTFS" = "true" ]; then
	## quickntfs
	for BADSector in $(cat tempddrsector | awk ' { print $1 } ') ; do
		NTFSsector=$(( ($BADSector-$START) ))
		if [ "$NTFSsector" -ge 0 ] && [ "$NTFSsector" -le "$BLOCKS" ]; then
			TEMPEND=$(grep -a -w $BADSector tempddrsector | awk ' { print $2 } ')
			if [ "$TEMPEND" = "" ]; then TEMPEND=$BADSector; fi
#			if [ "$TEMPEND" -ge "$BLOCKS" ]; then TEMPEND=$BLOCKS; fi
			NTFSEnd=$(( ($TEMPEND-$START) ))
			echo "##########################################################" >> tempddrntfsquick
			echo "########## Files in group StartSector=$BADSector EndSector=$TEMPEND ##########" >> tempddrntfsquick
			ntfscluster -f -s "$NTFSsector"-"$NTFSEnd" $PARTITION 2>>"$OUTPUTDEBUG" | tee -a "$OUTPUTDEBUG" tempddrntfsquick
		fi
	done

else
	## normal
	CLUSTERSIZE=$(ntfscluster $PARTITION 2>/dev/null | grep -a "bytes per cluster" | awk ' { print $5 } ')
	if [ "$CLUSTERSIZE" = "" ]; then
	  echo "Error! ntfscluster did not report a cluster size."
	  echo "This could be caused by the disk being mounted."
	  echo "Try unmounting the disk."
	  exit 1
	fi

	CHECKEDCLUSTER=""

	for BADSector in $(cat tempddrbad | awk ' { print $1 } ') ; do
		NTFSsector=$(( ($BADSector-$START) ))
		if [ "$NTFSsector" -ge 0 ] && [ "$NTFSsector" -le "$BLOCKS" ]; then

			NTFScluster=$(( ($NTFSsector*$SECTORSIZE)/$CLUSTERSIZE ))

			if [ "$CHECKEDCLUSTER" = "$NTFScluster" ]; then
				echo -n "Partition $PART Type NTFS DeviceSector $BADSector PartitionSector $NTFSsector Cluster $NTFScluster " | tee -a "$OUTPUTLIST" >/dev/null
				echo -n "Allocated $ALLOCATED " >> "$OUTPUTLIST"
				echo Inode $INODE File $FILE >> "$OUTPUTLIST"

			else
				echo -n "Checking cluster $NTFScluster "
				echo -n "Partition $PART Type NTFS DeviceSector $BADSector PartitionSector $NTFSsector Cluster $NTFScluster " | tee -a "$OUTPUTLIST" "$OUTPUTDEBUG" >/dev/null
				ntfscluster -f -s $NTFSsector $PARTITION 2>>"$OUTPUTDEBUG" | tee -a "$OUTPUTDEBUG" | grep -a Inode > tempddrcluster
				INODE=$(cat tempddrcluster | awk ' { print $2 } ')
				if [ "$INODE" = "" ]; then INODE="none"; ALLOCATED="no"; else ALLOCATED="yes"; fi
				FILE=$(cat tempddrcluster | cut -d " " -f3-)
				if [ "$FILE" = "" ]; then FILE="none"; fi
				echo -n "Allocated $ALLOCATED " >> "$OUTPUTLIST"
				echo "Inode $INODE File $FILE" | tee -a "$OUTPUTLIST"
				CHECKEDCLUSTER=$NTFScluster
			fi
		fi
	done

	rm -f tempddrcluster

fi

## calculate process run time
T="$(($(date +%s)-T))"
printf "Processing time NTFS: %02d:%02d:%02d:%02d\n" "$((T/86400))" "$((T/3600%24))" "$((T/60%60))" "$((T%60))"  >> "$OUTPUTDEBUG"
}

#######################################################################
####################### LINUX #########################################
#######################################################################
linux ()
{

## get process start time
T="$(date +%s)"

BLOCKSIZE=$(tune2fs -l $PARTITION | grep -a "Block size" | awk ' { print $3 } ')
CHECKEDBLOCK=""

for BADSector in $(cat tempddrbad | awk ' { print $1 } ') ; do
	LINUXsector=$(( ($BADSector-$START) ))
	if [ "$LINUXsector" -ge 0 ] && [ "$LINUXsector" -le "$BLOCKS" ]; then

		LINUXblock=$(( ($LINUXsector*$SECTORSIZE)/$BLOCKSIZE ))

		if [ "$CHECKEDBLOCK" = "$LINUXblock" ]; then
		echo -n "Partition $PART Type Linux DeviceSector $BADSector PartitionSector $LINUXsector Block $LINUXblock " | tee -a "$OUTPUTLIST" >/dev/null
			echo -n "Allocated $ALLOCATED " >> "$OUTPUTLIST"
			echo Inode $INODE File $FILE >> "$OUTPUTLIST"

		else
			echo -n "Checking block $LINUXblock "
			echo -n "Partition $PART Type Linux DeviceSector $BADSector PartitionSector $LINUXsector Block $LINUXblock " | tee -a "$OUTPUTLIST" "$OUTPUTDEBUG" >/dev/null
			echo open $PARTITION >> tempddrcmdi
			echo testb "$LINUXblock" >> tempddrcmdi
			echo icheck "$LINUXblock" >> tempddrcmdi

			debugfs -f tempddrcmdi 2>>"$OUTPUTDEBUG" | tee -a "$OUTPUTDEBUG" > tempddricheck
			ALLOCATED=$(grep -a "in use" tempddricheck | awk ' { print $3 } ')
			INODE=$(grep -a -v debugfs tempddricheck | grep -a -v -i block | awk ' { print $2 } ')

			if [ "$INODE" != "" ]; then
				ALLOCATED="yes"
				echo "open $PARTITION" >> tempddrcmdn
				echo "ncheck $INODE" >> tempddrcmdn

				debugfs -f tempddrcmdn 2>>"$OUTPUTDEBUG" | tee -a "$OUTPUTDEBUG" > tempddrncheck
				grep -a -v debugfs tempddrncheck | grep -a -v -i inode > tempddrout
				FILE=$(cat tempddrout | cut -f2-)
				if [ "$FILE" = "" ]; then FILE="none"; fi
				echo -n "Allocated $ALLOCATED " >> "$OUTPUTLIST"
				echo "Inode $INODE File $FILE" | tee -a "$OUTPUTLIST"

			else
				if [ "$ALLOCATED" = "marked" ]; then INODE="metadata"; ALLOCATED="yes"; else INODE="none"; ALLOCATED="no"; fi
				FILE="none"
				echo -n "Allocated $ALLOCATED " >> "$OUTPUTLIST"
				echo "Inode $INODE File $FILE" | tee -a "$OUTPUTLIST"
			fi
			CHECKEDBLOCK="$LINUXblock"
		fi

		rm -f tempddrcmdi
		rm -f tempddrcmdn

	fi
done

rm -f tempddrout
rm -f tempddricheck
rm -f tempddrinode
rm -f tempddrncheck

## calculate process run time
T="$(($(date +%s)-T))"
printf "Processing time Linux: %02d:%02d:%02d:%02d\n" "$((T/86400))" "$((T/3600%24))" "$((T/60%60))" "$((T%60))"  >> "$OUTPUTDEBUG"

}

#######################################################################
########################## FAT ########################################
#######################################################################
fat ()
{

## get process start time
T="$(date +%s)"

CLUSTERSIZE=$(fsstat $PARTITION | head -40 | grep -a "Cluster Size:" | awk ' { print $3 } ')
CHECKEDCLUSTER=""

for BADSector in $(cat tempddrbad | awk ' { print $1 } ') ; do
	FATsector=$(( ($BADSector-$START) ))
	if [ "$FATsector" -ge 0 ] && [ "$FATsector" -le "$BLOCKS" ]; then
		FATcluster=$(( ($FATsector*$SECTORSIZE)/$CLUSTERSIZE ))

		if [ "$CHECKEDCLUSTER" = "$FATcluster" ]; then
			echo -n "Partition $PART Type FAT DeviceSector $BADSector PartitionSector $FATsector Cluster $FATcluster " | tee -a "$OUTPUTLIST" >/dev/null
			echo -n "Allocated $ALLOCATED " >> "$OUTPUTLIST"
			if [ "$INODE" = "Inode not found" ]; then INODE=none; fi
			echo Inode $INODE File $FILE >> "$OUTPUTLIST"


		else
			echo -n "Checking cluster $FATcluster "
			echo -n "Partition $PART Type FAT DeviceSector $BADSector PartitionSector $FATsector Cluster $FATcluster " | tee -a "$OUTPUTLIST" "$OUTPUTDEBUG" >/dev/null
			blkstat $PARTITION $FATsector 2>> "$OUTPUTDEBUG" >> "$OUTPUTDEBUG"
			ALLOCATED=$(blkstat $PARTITION $FATsector | grep -a Allocated)
			if [ "$ALLOCATED" = "Allocated" ]; then
				ifind $PARTITION -d $FATsector 2>> "$OUTPUTDEBUG" | tee -a "$OUTPUTDEBUG" > tempddrifind
				IFIND=$(cat tempddrifind)
				INODE="$IFIND"
				ffind $PARTITION $IFIND 2>> "$OUTPUTDEBUG" | tee -a "$OUTPUTDEBUG" > tempddrffind
				FILE=$(cat tempddrffind)
			fi

			if [ "$ALLOCATED" = "Allocated" ]; then ALLOCATED=yes; fi
			if [ "$ALLOCATED" = "Not Allocated" ]; then ALLOCATED=no; INODE=none; FILE=none; fi
			if [ "$ALLOCATED" = "Allocated (Meta)" ]; then ALLOCATED=yes; INODE=metadata; FILE=none; fi
			if [ "$INODE" = "Inode not found" ]; then INODE=none; FILE=none; fi
			echo -n "Allocated $ALLOCATED " >> "$OUTPUTLIST"
			echo "Inode $INODE File $FILE" | tee -a "$OUTPUTLIST"
			CHECKEDCLUSTER=$FATcluster
		fi
	fi
done

rm -f tempddrffind
rm -f tempddrifind

## calculate process run time
T="$(($(date +%s)-T))"
printf "Processing time FAT: %02d:%02d:%02d:%02d\n" "$((T/86400))" "$((T/3600%24))" "$((T/60%60))" "$((T%60))"  >> "$OUTPUTDEBUG"

}

#######################################################################
########################## HFS ########################################
#######################################################################
hfs ()
{

## get process start time
T="$(date +%s)"

BLOCKSIZE=$(fsstat $PARTITION | head -40 | grep -a "Allocation Block Size" | awk ' { print $4 } ')
CHECKEDBLOCK=""

for BADSector in $(cat tempddrbad | awk ' { print $1 } ') ; do
	HFSsector=$(( ($BADSector-$START) ))
	if [ "$HFSsector" -ge 0 ] && [ "$HFSsector" -le "$BLOCKS" ]; then
		HFSblock=$(( ($HFSsector*$SECTORSIZE)/$BLOCKSIZE ))

		if [ "$CHECKEDBLOCK" = "$HFSblock" ]; then
			echo -n "Partition $PART Type HFS DeviceSector $BADSector PartitionSector $HFSsector Block $HFSblock " | tee -a "$OUTPUTLIST" >/dev/null
			echo -n "Allocated $ALLOCATED " >> "$OUTPUTLIST"
			if [ "$INODE" = "Inode not found" ]; then INODE=none; fi
			echo Inode $INODE File $FILE >> "$OUTPUTLIST"

		else
			echo -n "Checking block $HFSblock "
			echo -n "Partition $PART Type HFS DeviceSector $BADSector PartitionSector $HFSsector Block $HFSblock " | tee -a "$OUTPUTLIST" "$OUTPUTDEBUG" >/dev/null
			blkstat $PARTITION $HFSblock 2>> "$OUTPUTDEBUG" >> "$OUTPUTDEBUG"
			ALLOCATED=$(blkstat $PARTITION $HFSblock | grep -a Allocated)
			if [ "$ALLOCATED" = "Allocated" ]; then
				ifind $PARTITION -d $HFSblock 2>> "$OUTPUTDEBUG" | tee -a "$OUTPUTDEBUG" > tempddrifind
				IFIND=$(cat tempddrifind)
				INODE="$IFIND"
				ffind $PARTITION $IFIND 2>> "$OUTPUTDEBUG" | tee -a "$OUTPUTDEBUG" > tempddrffind
				FILE=$(cat tempddrffind)
			fi
			if [ "$ALLOCATED" = "Allocated" ]; then ALLOCATED=yes; fi
			if [ "$ALLOCATED" = "Not Allocated" ]; then ALLOCATED=no; INODE=none; FILE=none; fi
			if [ "$ALLOCATED" = "Allocated (Meta)" ]; then ALLOCATED=yes; INODE=metadata; FILE=none; fi
			if [ "$INODE" = "Inode not found" ]; then INODE=none; FILE=none; fi
			echo -n "Allocated $ALLOCATED " >> "$OUTPUTLIST"
			echo "Inode $INODE File $FILE" | tee -a "$OUTPUTLIST"
			CHECKEDBLOCK=$HFSblock
		fi
	fi
done

rm -f tempddrffind
rm -f tempddrifind

## calculate process run time
T="$(($(date +%s)-T))"
printf "Processing time HFS: %02d:%02d:%02d:%02d\n" "$((T/86400))" "$((T/3600%24))" "$((T/60%60))" "$((T%60))"  >> "$OUTPUTDEBUG"

}

#######################################################################
###################### Process Output Files ###########################
#######################################################################
processoutput ()
{

## get process start time
T="$(date +%s)"

echo "Processing output files"

## copy sector range list to results_info
echo "##########################################################" >> "$OUTPUTINFO"
echo "########## List of bad sectors as a sector range list ##########" >> "$OUTPUTINFO"
cat tempddrsector >> "$OUTPUTINFO"


###### summary results ######
echo "########## ddru_findbad $VERSION summary output file ##########" >> "$OUTPUTSUMMARY"

if [ "$QUICK" = "true" ];then
	echo "########## Note that the quick option was used ##########" >> "$OUTPUTSUMMARY"
	echo "########## Therefore this is not a complete result ##########" >> "$OUTPUTSUMMARY"
	echo "########## And the stats will not be accurate ##########" >> "$OUTPUTSUMMARY"
fi

if [ "$QUICKNTFS" = "true" ];then
	echo "########## Note that the quick ntfs option was used ##########" >> "$OUTPUTSUMMARY"
	echo "########## NTFS results will be complete but ##########" >> "$OUTPUTSUMMARY"
	echo "########## will not have sector counts ##########" >> "$OUTPUTSUMMARY"
fi

echo "Producing stats"
## give stats
Number=$(grep -c "" tempddrbad)
echo "There are $Number bad sectors total from the log file" >> "$OUTPUTSUMMARY"
Number=$(grep -c "" "$OUTPUTLIST")
echo "$Number sectors were in partitions that were able to be processed" >> "$OUTPUTSUMMARY"
grep -a -w "Allocated yes" "$OUTPUTLIST" > tempddrprocess
Number=$(grep -c "Allocated" tempddrprocess)
echo "$Number sectors are listed as allocated" >> "$OUTPUTSUMMARY"
Number=$(grep -v -w -c "File none" tempddrprocess)
echo "$Number of those have a file or data listing related to them" >> "$OUTPUTSUMMARY"
Number=$(grep -w -c "File none" tempddrprocess)
echo "leaving $Number that do not have a file or data listing related to them" >> "$OUTPUTSUMMARY"
grep -a -w "Allocated no" "$OUTPUTLIST" > tempddrprocess
Number=$(grep -c "Allocated" tempddrprocess)
echo "$Number sectors are listed as not allocated" >> "$OUTPUTSUMMARY"

## list of files with count
echo "Creating list of files with counts"
echo ................................................................... >> "$OUTPUTSUMMARY"
echo "Below is the list of files related to the bad sectors" >> "$OUTPUTSUMMARY"
echo "with the number of bad sectors in each file" >> "$OUTPUTSUMMARY"
echo ................................................................... >> "$OUTPUTSUMMARY"
cut -d " " -f13- "$OUTPUTLIST" | sort -u > tempddrprocess

## change $IFS variable to allow for loop to process spaces
SAVEIFS=$IFS
IFS=$(printf '\n\b')

for File in $(cat tempddrprocess) ; do
	Number=$(grep -w -c "$File" "$OUTPUTLIST")
	echo "BadSectors=$Number $File" >> "$OUTPUTSUMMARY"
done

## put $IFS back to normal
IFS=$SAVEIFS

if [ "$QUICKNTFS" = "true" ];then
	grep -a "Inode" tempddrntfsquick | sort -u >> "$OUTPUTSUMMARY"
fi

######## do extra processing if extraout is true ########
if [ "$EXTRAOUT" = "true" ]; then
	echo "Processing extra output files"
	echo "This could take awhile, but you asked for it!"
	echo "So please be patient"

	###### results by sectors ######
	echo "########## ddru_findbad $VERSION results by sectors file ##########" >> "$OUTPUTBYSECTOR"
	echo "########## Note that sectors are based on the whole drive ##########" >> "$OUTPUTBYSECTOR"
	echo "########## not on individual partitions ##########" >> "$OUTPUTBYSECTOR"

	if [ "$QUICK" = "true" ];then
		echo "########## Note that the quick option was used ##########" >> "$OUTPUTBYSECTOR"
		echo "########## Therefore this is not a complete result ##########" >> "$OUTPUTBYSECTOR"
	fi

	if [ "$QUICKNTFS" = "true" ];then
		echo "########## Note that the quick ntfs option was used ##########" >> "$OUTPUTBYSECTOR"
		echo "########## Therefore any NTFS results will be complete ##########" >> "$OUTPUTBYSECTOR"
	fi

	echo "Processing results by sector file"
	for START in $(cat tempddrsector | awk ' { print $1 } ') ; do
		END=$(grep -a -w $START tempddrsector | awk ' { print $2 } ')
		if [ "$END" = "" ]; then END=$START; fi
		SECTOR=$START

		rm -f tempddrprocess
		if [ "$QUICK" = "true" ];then
			grep -a -w "DeviceSector $SECTOR" "$OUTPUTLIST" | cut -d " " -f13- >> tempddrprocess
			grep -a -w "DeviceSector $END" "$OUTPUTLIST" | cut -d " " -f13- >> tempddrprocess
		else
			while [ $SECTOR -le $END  ]; do
				grep -a -w "DeviceSector $SECTOR" "$OUTPUTLIST" | cut -d " " -f13- >> tempddrprocess
				SECTOR=$((SECTOR+1))
			done
		fi
		echo "##########################################################" >> "$OUTPUTBYSECTOR"
		echo "########## Files in group StartSector=$START EndSector=$END ##########" >> "$OUTPUTBYSECTOR"
		cat tempddrprocess | sort -u >> "$OUTPUTBYSECTOR"

	done

if [ "$QUICKNTFS" = "true" ];then
	cat tempddrntfsquick  >> "$OUTPUTBYSECTOR"
fi

fi

rm -f tempddrprocess

## calculate process run time
T="$(($(date +%s)-T))"
printf "Processing time Process Ouput: %02d:%02d:%02d:%02d\n" "$((T/86400))" "$((T/3600%24))" "$((T/60%60))" "$((T%60))"  >> "$OUTPUTDEBUG"

}

#######################################################################
###################### Get Partition Type #############################
#######################################################################
getparttype ()
{
echo 'Choose the partition type to try from the list-'
echo '1) NTFS                 3) FAT                  5) Skip this partition'
echo '2) LINUX                4) HFS                  6) Quit'
while :; do
    printf 'Choose wisely: '
    read opt
    case $opt in
        1)
            PARTITIONTYPE="ntfs"
            break
            ;;
        2)
            PARTITIONTYPE="linux"
            break
            ;;
        3)
            PARTITIONTYPE="fat"
            break
            ;;
        4)
            PARTITIONTYPE="hfs"
            break
            ;;
        5)
            PARTITIONTYPE="none"
            break
            ;;
        6)
            exit 0
            ;;
        *) echo invalid option;;
    esac
done
}

#######################################################################
###################### Finish Cleanup #################################
#######################################################################
cleanup ()
{

echo Cleaning up
## delete main loop if used
if [ "$IMAGE" = "true" ]; then
	sleep $LOOPWAIT ## wait to give loop time to not be busy
	losetup -d $LOOPA
fi
## remove leftover temp files
rm -f tempddrfdisk
rm -f tempddrbad
rm -f tempddrsector
rm -f tempddrntfsquick
}

#######################################################################
############################# Main ####################################
#######################################################################

commands "$@"

echo "ddru_findbad $VERSION"

prestart

processtarget

processlogfile

if [ $PARTITIONONLY != "true" ]; then

	## add info to debug output and info output
	echo "##########################################################" | tee -a "$OUTPUTINFO"  >>"$OUTPUTDEBUG"
	echo "########## $GFDISK output ##########" | tee -a "$OUTPUTINFO" >> "$OUTPUTDEBUG"
	$GFDISK -lu "$TARGET" | tee -a "$OUTPUTINFO" >> "$OUTPUTDEBUG"
	echo "##########################################################" | tee -a "$OUTPUTINFO"  >>"$OUTPUTDEBUG"
	echo "########## mmls output ##########" | tee -a "$OUTPUTINFO" >> "$OUTPUTDEBUG"
	mmls "$TARGET" | tee -a "$OUTPUTINFO" >> "$OUTPUTDEBUG"

	$GFDISK -lu "$TARGET" | grep -a "$TARGET" | grep -a -v Disk > tempddrfdisk

	for PART in $(cat tempddrfdisk | awk ' { print $1 } ') ; do
		echo Checking $PART
		grep -a -w $PART tempddrfdisk >>tempddrdev
		grep -a -v "*" tempddrdev | awk ' { print $2 } ' >>tempddrstart
		grep -a "*" tempddrdev | awk ' { print $3 } ' >>tempddrstart
		grep -a -v "*" tempddrdev | awk ' { print $3 } ' >>tempddrend
		grep -a "*" tempddrdev | awk ' { print $4 } ' >>tempddrend
		grep -a -v "*" tempddrdev | awk ' { print $5 } ' >>tempddrid
		grep -a "*" tempddrdev | awk ' { print $6 } ' >>tempddrid
		grep -a -v "*" tempddrdev | tr -s " " | cut -d " " -f6- >>tempddrtype
		grep -a "*" tempddrdev | tr -s " " | cut -d " " -f7- >>tempddrtype
		START=$(cat tempddrstart)
		END=$(cat tempddrend)
		BLOCKS=$(( ($END-$START) ))
		ID=$(cat tempddrid)
		TYPE=$(cat tempddrtype)

		if [ "$IMAGE" = "true" ]; then
			LOOPB=$(losetup -f)
			sleep $LOOPWAIT ## wait to give loop time to not be busy
			losetup -r $LOOPB "$TARGETDRIVE" -o $(( $START*$SECTORSIZE ))
			PARTITION=$LOOPB
		else
		PARTITION=$PART
		fi

		findpartitiontype

		if [ "$MANUAL" = "true" ]; then
			getparttype
		fi

		case $PARTITIONTYPE in
			ntfs)
				ntfs
			;;
			linux)
				linux
			;;
			fat)
				fat
			;;
			hfs)
				hfs
			;;
			none)
				echo "Either you selected not to process this partition," | tee -a "$OUTPUTDEBUG"
				echo "or fsstat could not determine the partition type." | tee -a "$OUTPUTDEBUG"
				echo "Therefore the partition cannot be processed" | tee -a "$OUTPUTDEBUG"
				echo "The partition type may not be supported" | tee -a "$OUTPUTDEBUG"
				echo "Or the filesystem may be damaged" | tee -a "$OUTPUTDEBUG"
				echo "$GFDISK reports the partition ID is" $ID "and the type is" $TYPE | tee -a "$OUTPUTDEBUG"
		esac


		if [ "$IMAGE" = "true" ]; then
		sleep $LOOPWAIT ## wait to give loop time to not be busy
		losetup -d $LOOPB
		fi

		rm -f tempddrdev
		rm -f tempddrstart
		rm -f tempddrend
		rm -f tempddrid
		rm -f tempddrtype
	done
fi

if [ $PARTITIONONLY = "true" ]; then
	PARTITION="$TARGET"
	PART="$TARGET"

	START="0"
	END="999999999999"
	BLOCKS=$(( ($END-$START) ))

	findpartitiontype

	if [ "$MANUAL" = "true" ]; then
		getparttype
	fi

	case $PARTITIONTYPE in
		ntfs)
			ntfs
		;;
		linux)
			linux
		;;
		fat)
			fat
		;;
		hfs)
			hfs
		;;
		none)
			echo "Either you selected not to process this partition," | tee -a "$OUTPUTDEBUG"
			echo "or fsstat could not determine the partition type." | tee -a "$OUTPUTDEBUG"
			echo "Therefore the partition cannot be processed" | tee -a "$OUTPUTDEBUG"
			echo "The partition type may not be supported" | tee -a "$OUTPUTDEBUG"
			echo "Or the filesystem may be damaged" | tee -a "$OUTPUTDEBUG"
	esac

fi

processoutput

cleanup

## calculate and show total run time
TT="$(($(date +%s)-TT))"
printf "Total elapsed time: %02d:%02d:%02d:%02d\n" "$((TT/86400))" "$((TT/3600%24))" "$((TT/60%60))" "$((TT%60))" | tee -a "$OUTPUTDEBUG"

exit 0


