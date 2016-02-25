#!/bin/bash

echo $0
echo $1
day=$1
echo $STAR_LEVEL
# 
FILES=/star/data09/reco/production_15GeV_2014/ReversedFullField/dev/2014/$day/*/st_physics_1*.MuDst.root
DIROUT=/star/institutions/rice/llope/pdst/run14/$day/pDstvpd_
for f in $FILES
do
  fin=`echo $f`
  fout1=`echo $f | cut -c81-`
  fout=$DIROUT$fout1
  echo "f     ="$f
  echo "fin   ="$fin
  echo "fout1 ="$fout1
  echo "fout  ="$fout
  root4star -b -q -l 'cp_vpd.C("'$fin'","'$fout'")'
done

FILES=/star/data10/reco/production_15GeV_2014/ReversedFullField/dev/2014/$day/*/st_physics_1*.MuDst.root
DIROUT=/star/institutions/rice/llope/pdst/run14/$day/pDstvpd_
for f in $FILES
do
  fin=`echo $f`
  fout1=`echo $f | cut -c81-`
  fout=$DIROUT$fout1
  echo "f     ="$f
  echo "fin   ="$fin
  echo "fout1 ="$fout1
  echo "fout  ="$fout
  root4star -b -q -l 'cp_vpd.C("'$fin'","'$fout'")'
done

FILES=/star/data12/reco/production_15GeV_2014/ReversedFullField/dev/2014/$day/*/st_physics_1*.MuDst.root
DIROUT=/star/institutions/rice/llope/pdst/run14/$day/pDstvpd_
for f in $FILES
do
  fin=`echo $f`
  fout1=`echo $f | cut -c81-`
  fout=$DIROUT$fout1
  echo "f     ="$f
  echo "fin   ="$fin
  echo "fout1 ="$fout1
  echo "fout  ="$fout
  root4star -b -q -l 'cp_vpd.C("'$fin'","'$fout'")'
done

exit
