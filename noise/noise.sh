#!/bin/sh
#
# Build and submit Scheduler XML files for single noise run analysis
#
STARLIB=SL21b
WRKDIR=~/scheduler/noise
SUBMIT=/afs/rhic.bnl.gov/star/packages/scripts/star-submit
DAY=yes
BTOFWWW=/star/u/geurts/WWW/noise
HTREE=/star/u/geurts/bin/tree
[ -z "$BTOFONLINE" ] && BTOFONLINE=/star/u/geurts/BTOF-online
[ -z  "$DAQDIR" ]    && DAQDIR=/gpfs01/star/scratch/geurts/daq
[ -z "$LOGDIR" ]     && LOGDIR=/star/u/geurts/logs

RUNNUMBERS=()
while [[ $# -gt 0 ]]
do
key="$1"

case $key  in
  -r)
   DAY=no
   shift
   ;;
  -n)
   WRKDIR=/tmp
   SUBMIT=/usr/bin/cat
   shift
   ;;
  -d)
    DAQDIR=$2
    shift
    shift
    ;;
  -h)
   echo Usage: $0 [-h] [-n] [-r] [-d DAQDIR] runnumber ...
   echo " -n  do not submit"
   echo " -r  use run numbers instead of run days"
   echo " -d  set DAQDIR"
   echo " -h  this message"
   echo Defaults:
   echo - STARLIB = $STARLIB
   echo - WRKDIR = $WRKDIR
   echo - DAY = $DAY
   echo - DAQDIR = $DAQDIR
   echo - LOGDIR = $LOGDIR
   echo - BTOFONLINE = $BTOFONLINE
   exit
   ;;
  *) # unknown option, presume this to be a runnumber
  RUNNUMBERS+=("$1") # save it in an array for later
  shift
  ;;
esac
done

set -- "${RUNNUMBERS[@]}" # restore positional parameters

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
 echo '<job name="tof-noise'$runnumber'_" >' >> $WRKDIR/$infile
 echo '<command>' >> $WRKDIR/$infile
 echo starver $STARLIB >> $WRKDIR/$infile
 echo ./daqdoer '*.daq &lt;&lt; endd' >> $WRKDIR/$infile
 echo 1 >> $WRKDIR/$infile

 if [[ $DAY == 'yes' ]]; then
  echo $run$day >> $WRKDIR/$infile
 else
  echo $runnumber >> $WRKDIR/$infile
 fi

 echo endd >> $WRKDIR/$infile

 if [[ $DAY == 'yes' ]]; then
  echo "./noise -r $run$day" >> $WRKDIR/$infile
 else
  echo "./noise -r $runnumber" >> $WRKDIR/$infile
 fi

# update index file of the local web repository (RCF does not allow directory browsing)
 if [[ -x $HTREE ]]; then
  echo "cd $BTOFWWW/run$(($run-1))" >> $WRKDIR/$infile
  echo "$HTREE --dirsfirst -C -H  'https://www4.rcf.bnl.gov/~geurts/noise/run$(($run-1))' &gt; index.html"  >> $WRKDIR/$infile
 fi

 echo '</command>' >> $WRKDIR/$infile
 echo '<SandBox>' >> $WRKDIR/$infile
 echo ' <Package>' >> $WRKDIR/$infile
 echo "  <File>file:$BTOFONLINE/daqdoer/daqdoer</File>" >> $WRKDIR/$infile
 echo "  <File>file:$BTOFONLINE/noise/noise</File>" >> $WRKDIR/$infile
 echo "  <File>file:$BTOFONLINE/noise/sl.root</File>" >> $WRKDIR/$infile
 echo "  <File>file:$BTOFONLINE/noise/fitzhit_14042.root</File>" >> $WRKDIR/$infile
 echo "  <File>file:$DAQDIR/$year/$day/$runnumber/</File>" >> $WRKDIR/$infile
 echo ' </Package>' >> $WRKDIR/$infile >> $WRKDIR/$infile
 echo '</SandBox>' >> $WRKDIR/$infile >> $WRKDIR/$infile

 if [[ $DAY == 'yes' ]]; then
  echo '<stdout URL="file:'$LOGDIR/$run$day'.log" />' >> $WRKDIR/$infile
 else
  echo '<stdout URL="file:'$LOGDIR/$runnumber'.log" />' >> $WRKDIR/$infile
 fi

 echo '<Generator>' >> $WRKDIR/$infile
 echo " <Location>$WRKDIR</Location>" >> $WRKDIR/$infile
 echo " <ReportLocation>$WRKDIR/reports</ReportLocation>" >> $WRKDIR/$infile
 echo " <ScriptLocation>$WRKDIR/scripts</ScriptLocation>" >> $WRKDIR/$infile
 echo " <ListLocation>$WRKDIR/lists</ListLocation>" >> $WRKDIR/$infile
 echo '</Generator>' >> $WRKDIR/$infile
 echo '</job>' >> $WRKDIR/$infile

 # submit file to SUMS
 cd $WRKDIR
 $SUBMIT $WRKDIR/$infile
done
