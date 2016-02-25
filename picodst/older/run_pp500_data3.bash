#!/bin/bash

echo $0
echo $1
day=$1
echo $STAR_LEVEL
# 
FILES=/star/data03/daq/tof/llope/fomudst/st_physics_14${day}*.MuDst.root
DIROUT=/star/institutions/rice/llope/pdst/run13/$day/pDst_
for f in $FILES
do
  fin=`echo $f`
  fout1=`echo $f | cut -c36-`
  fout=$DIROUT$fout1
  echo "....................................................................................."
  echo "f     ="$f
  echo "fin   ="$fin
  echo "fout1 ="$fout1
  echo "fout  ="$fout
  root4star -b -q -l 'cp.C("'$fin'","'$fout'")'
done


exit
