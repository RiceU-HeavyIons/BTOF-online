#!/usr/local/bin/tcsh

cd /star/u/llope/run14/daqdoer

set echo

root -b -q -l 'tofmult.C(15048035)'
root -b -q -l 'tofmult.C(15048036)'
root -b -q -l 'tofmult.C(15048037)'
root -b -q -l 'tofmult.C(15048038)'
root -b -q -l 'tofmult.C(15048039)'
root -b -q -l 'tofmult.C(15048040)'
root -b -q -l 'tofmult.C(15048041)'
root -b -q -l 'tofmult.C(15048042)'

root -b -q -l tofmult_summary.C

exit