#!/bin/bash

echo $0
echo $1
day=$1
echo $STAR_LEVEL
# 
#FILES=/star/data09/reco/pp500_production_2011/ReversedFullField/dev/2011/$day/*/st_physics_1*.MuDst.root
#FILES=/star/data09/reco/AuAu19_production/ReversedFullField/dev/2011/$day/*/st_physics_1*.MuDst.root
#FILES=/star/data09/reco/pp500_production_2012/ReversedFullField/dev/2012/$day/*/st_physics_1*.MuDst.root
FILES=/star/data09/reco/UU_production_2012/ReversedFullField/dev/2012/$day/*/st_physics_1*.MuDst.root
DIROUT=/star/institutions/rice/llope/pdst/run12/$day/pDst_
for f in $FILES
do
  fin=`echo $f`
  fout1=`echo $f | cut -c78-`
  fout=$DIROUT$fout1
  echo "f     ="$f
  echo "fin   ="$fin
  echo "fout1 ="$fout1
  echo "fout  ="$fout
  root4star -b -q -l 'cp.C("'$fin'","'$fout'")'
done

#FILES=/star/data10/reco/pp500_production_2011/ReversedFullField/dev/2011/$day/*/st_physics_1*.MuDst.root
#FILES=/star/data10/reco/AuAu19_production/ReversedFullField/dev/2011/$day/*/st_physics_1*.MuDst.root
#FILES=/star/data10/reco/pp500_production_2012/ReversedFullField/dev/2012/$day/*/st_physics_1*.MuDst.root
FILES=/star/data10/reco/UU_production_2012/ReversedFullField/dev/2012/$day/*/st_physics_1*.MuDst.root
DIROUT=/star/institutions/rice/llope/pdst/run12/$day/pDst_
for f in $FILES
do
  fin=`echo $f`
  fout1=`echo $f | cut -c78-`
  fout=$DIROUT$fout1
  echo "Processing $fin $fout"
  root4star -b -q -l 'cp.C("'$fin'","'$fout'")'
done

#FILES=/star/data12/reco/pp500_production_2011/ReversedFullField/dev/2011/$day/*/st_physics_1*.MuDst.root
#FILES=/star/data12/reco/AuAu19_production/ReversedFullField/dev/2011/$day/*/st_physics_1*.MuDst.root
#FILES=/star/data12/reco/pp500_production_2012/ReversedFullField/dev/2012/$day/*/st_physics_1*.MuDst.root
FILES=/star/data12/reco/UU_production_2012/ReversedFullField/dev/2012/$day/*/st_physics_1*.MuDst.root
DIROUT=/star/institutions/rice/llope/pdst/run12/$day/pDst_
for f in $FILES
do
  fin=`echo $f`
  fout1=`echo $f | cut -c78-`
  fout=$DIROUT$fout1
  echo "Processing $fin $fout"
  root4star -b -q -l 'cp.C("'$fin'","'$fout'")'
done

exit
