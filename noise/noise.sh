#!/bin/sh
#
# Build and submit Scheduler XML files for single noise run analysis
#
STARLIB=SL18f
WRKDIR=~/scheduler/noise
SUBMIT=/afs/rhic.bnl.gov/star/packages/scripts/star-submit
[ -z "$BTOFONLINE" ] && BTOFONLINE=/star/u/geurts/BTOF-online
[ -z  "$DAQDIR" ]    && DAQDIR=/gpfs01/star/scratch/geurts/daq
[ -z "$LOGDIR" ]     && LOGDIR=/star/u/geurts/logs

for runnumber in $@
do
 echo '=== Scheduling Run Number: '$runnumber' ==='

 # deconstruct run number
 run=${runnumber:0:2}
 year=$((1999+run))
 day=${runnumber:2:3}
 infile=noise$runnumber.xml

 # build input file
 echo '<?xml version="1.0" encoding="utf-8" ?>' > $WRKDIR/$infile
 echo '<job name="tof noise' $runnumber'" >' >> $WRKDIR/$infile
 echo '<command>' >> $WRKDIR/$infile
 echo starver $STARLIB >> $WRKDIR/$infile
 echo ./daqdoer '*.daq &lt;&lt; endd' >> $WRKDIR/$infile
 echo 1 >> $WRKDIR/$infile
 echo $run$day >> $WRKDIR/$infile
 echo endd >> $WRKDIR/$infile
 echo "./noise -r $run$day" >> $WRKDIR/$infile
 echo '</command>' >> $WRKDIR/$infile
 echo '<SandBox>' >> $WRKDIR/$infile
 echo ' <Package>' >> $WRKDIR/$infile
 echo "  <File>file:$BTOFONLINE/daqdoer/daqdoer</File>" >> $WRKDIR/$infile
 echo "  <File>file:$BTOFONLINE/noise/noise</File>" >> $WRKDIR/$infile
 echo "  <File>file:$DAQDIR/$year/$day/$runnumber/</File>" >> $WRKDIR/$infile
 echo ' </Package>' >> $WRKDIR/$infile >> $WRKDIR/$infile
 echo '</SandBox>' >> $WRKDIR/$infile >> $WRKDIR/$infile
 echo '<stdout URL="file:'$LOGDIR/$run$day'.log" />' >> $WRKDIR/$infile
 echo '<Generator>' >> $WRKDIR/$infile
 echo " <Location>$WRKDIR</Location>" >> $WRKDIR/$infile
 echo " <ReportLocation>$WRKDIR/reports</ReportLocation>" >> $WRKDIR/$infile
 echo " <ScriptLocation>$WRKDIR/scripts</ScriptLocation>" >> $WRKDIR/$infile
 echo " <ListLocation>$WRKDIR/lists</ListLocation>" >> $WRKDIR/$infile
 echo '</Generator>' >> $WRKDIR/$infile
 echo '</job>' >> $WRKDIR/$infile

 # submit file to SUMS
 $SUBMIT $WRKDIR/$infile
done
