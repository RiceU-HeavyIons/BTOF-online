#!/bin/sh
STARLIB=SL18f
WRKDIR=~/scheduler
DAQDOER=./daqdoer
SUBMIT=/afs/rhic.bnl.gov/star/packages/scripts/star-submit
[ -z "$BTOFONLINE" ] && BTOFONLINE=/star/u/geurts/BTOF-online
[ -z  "$DAQDIR" ]    && DAQDIR=/gpfs01/star/scratch/geurts/daq
[ -z "$LOGDIR" ]     && LOGDIR=/star/u/geurts/logs

for runnumber in $@
do
 echo '=== Scheduling Run Number: '$runnumber' ==='
 #runnumber=$1

 run=${runnumber:0:2}
 year=$((1999+run))
 day=${runnumber:2:3}
 infile=noise$runnumber.xml

 # build input file
 echo '<?xml version="1.0" encoding="utf-8" ?>' > $WRKDIR/$infile
 echo '<job name="tof noise ' $runnumber'" >' >> $WRKDIR/$infile
 echo '<command>' >> $WRKDIR/$infile
 echo starver $STARLIB >> $WRKDIR/$infile
 echo cd $BTOFONLINE/daqdoer >> $WRKDIR/$infile
 echo $DAQDOER $DAQDIR/$year/$day/$runnumber/'*.daq &lt;&lt; endd' >> $WRKDIR/$infile
 echo 1 >> $WRKDIR/$infile
 echo $run$day >> $WRKDIR/$infile
 echo endd >> $WRKDIR/$infile
 echo cd $BTOFONLINE/noise >> $WRKDIR/$infile
 echo "./noise -r $run$day" >> $WRKDIR/$infile
 echo '</command>' >> $WRKDIR/$infile
 echo '<stdout URL="file:'$LOGDIR/$run$day'.log" />' >> $WRKDIR/$infile
 echo '</job>' >> $WRKDIR/$infile

 # execute
 cd $WRKDIR
 $SUBMIT $infile
done
