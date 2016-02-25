#!/bin/sh

cd /star/data09/reco/production_15GeV_2014/ReversedFullField/dev/2014/
find -type f -name "*.MuDst.root" | wc -l
#du -h --max-depth=1
cd /star/data10/reco/production_15GeV_2014/ReversedFullField/dev/2014/
find -type f -name "*.MuDst.root" | wc -l
#du -h --max-depth=1
cd /star/data12/reco/production_15GeV_2014/ReversedFullField/dev/2014/
find -type f -name "*.MuDst.root" | wc -l
#du -h --max-depth=1

thedir=/star/institutions/rice/llope/pdst/run14/

for i in `ls $thedir`
do
        #echo "$i:\c"
        if [ -r $thedir/$i ];
        then
                echo $i `ls $thedir/$i|wc -l` `du -h $thedir/$i`
        else
                echo "\tNo READ Permissions"
        fi
done


exit
