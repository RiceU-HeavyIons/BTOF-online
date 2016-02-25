#!/bin/bash

source ${GROUP_DIR}/.starold

echo $0
echo $1
day=$1
echo $STAR_LEVEL
# 
FILES=/star/data03/daq/llope/$day/st_physics_11${day}*.MuDst.root
DIROUT=/star/institutions/rice/llope/pdst/run10/$day/pDst_
#
COUNT=0
#
for f in $FILES
do
  fin=`echo $f`
  fout1=`echo $f | cut -c28-`
  fout=$DIROUT$fout1
  echo "....................................................................................."
  echo "f     ="$f
  #echo "fin   ="$fin
  echo "fout1 ="$fout1
  echo "fout  ="$fout
  COUNT=$((COUNT + 1))
  root4star -b -q -l 'cp.C("'$fin'","'$fout'")'
  #
  #echo ""
  #mv -v $f /star/data03/daq/llope/done
  rm -v $f
  #echo ""
  #
done

echo ""
echo "Count = "$COUNT
echo ""

exit
