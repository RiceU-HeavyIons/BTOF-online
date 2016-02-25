#!/bin/bash

cd /star/u/llope/run14/daqdoer

function doit(){
echo ........................................................
echo $1
./daqdoer $DATA03/$1/*.daq<<endd
3
${1}_tof
endd
root -b -q -l thub.C
echo ........................................................
}

set echo

#doit 15048035
#doit 15048036
#doit 15048037
#doit 15048038
#doit 15048039
#doit 15048040
#doit 15048041
#doit 15048042

doit 15073033

exit

