#!/bin/bash

#echo $0
#echo $1
day=$1
#echo star_level=$STAR_LEVEL
echo day=$day
if [ -z $day ]; 
then 
	echo "";
	echo "Input the three character day number!"; 
	echo "";
	exit
fi
# 
count1=0
FILES=/star/institutions/rice/llope/pdst/run14/mudst/$day/st_physics_1*.MuDst.root
DIROUT=/star/institutions/rice/llope/pdst/run14/$day/pDst_
for f in $FILES
do
  fin=`echo $f`
  fout1=`echo $f | cut -c52-`
  fout=$DIROUT$fout1
  echo "fin   ="$fin
  echo "fout1 ="$fout1
  echo "fout  ="$fout
  root4star -b -q -l 'cp_novpd.C("'$fin'","'$fout'")'
  count1=$((count1+1))
done
# 
count2=0
FILES=/star/institutions/rice/llope/pdst/run14/mudst/$day/st_hltgood_1*.MuDst.root
DIROUT=/star/institutions/rice/llope/pdst/run14/$day/pDst_
for f in $FILES
do
  fin=`echo $f`
  fout1=`echo $f | cut -c52-`
  fout=$DIROUT$fout1
  echo "fin   ="$fin
  echo "fout1 ="$fout1
  echo "fout  ="$fout
  root4star -b -q -l 'cp_novpd.C("'$fin'","'$fout'")'
  count2=$((count2+1))
done

echo ""
echo Number of physics files seen = $count1
echo Number of hltgood files seen = $count2
echo ""

exit
