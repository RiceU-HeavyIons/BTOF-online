#!/bin/bash

echo $0
echo $1
day=$1
echo $STAR_LEVEL

FILES=/star/data03/daq/tof/llope/fo/pp500_production_2012/ReversedFullField/dev/2012/$day/*/st_physics_*.MuDst.root
DIROUT=/star/institutions/rice/llope/pdst/run12/$day/pDst_
for f in $FILES
do
  fin=`echo $f`
  fout1=`echo $f | cut -c93-`
  fout=$DIROUT$fout1
  echo "f     ="$f
  echo "fin   ="$fin
  echo "fout1 ="$fout1
  echo "fout  ="$fout
  root4star -b -q -l 'cp.C("'$fin'","'$fout'")'
done

exit
