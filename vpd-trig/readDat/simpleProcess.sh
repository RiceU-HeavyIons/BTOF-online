#!/bin/bash

nRuns=0
oldrun=0

nFiles=`ls trgdata/*.dat | wc -l`
if [ $nFiles = 0 ]; then
     echo "There is no trigger data files in trgdata! Please add *.dat files to trgdata file!"
     exit 0
fi

for inFileName in `ls trgdata/*.dat`
do 
	datname=`basename $inFileName`
	outFileName="output/"${datname//dat/root}
	#echo $outFileName

    root4star -l -q 'TDFReader.C("'$inFileName'","'$outFileName'",1.e8)'
done

# echo "Total Runs: "$nRuns
# echo "Total Files: "$nFiles

# for runNum in `cat runnumber.list`
# do
# 	 ls output/$runNum/*.*.root > file.list
# 	 nFilesPerRun=`cat file.list | wc -l`
# 	 hadd.x file.list
#      rm -rf file.list
# 	 mv $nFilesPerRun.root output/$runNum/run$runNum.root
# 	 rm -rf output/$runNum/*.*.root
# done
