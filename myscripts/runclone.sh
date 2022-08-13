#! /bin/sh

LOG="zzz.log"
SOURCE="/dev/sda"
TARGET="/dev/null"
EXITSLOWSPEED="100000"
EXITSLOWTIME="2"

CLONECOMMAND="hddsuperclone -f $LOG -t $TARGET -s $SOURCE --exit-on-slow $EXITSLOWSPEED --exit-slow-time $EXITSLOWTIME"

while :; do

$CLONECOMMAND
CODE=$?
echo $CODE
# quit if the exit code is not 16 (exit on slow)
if [ $CODE -ne 16 ];then
   exit
fi

hddsupertool -f idle -t $SOURCE number=1


done