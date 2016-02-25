#!/bin/bash

cd /star/u/llope/run14/online

function doit(){
echo ........................................................
echo $1
./online -r $1
echo ........................................................
}

set echo

doit 15030046
doit 15030048
doit 15030049
doit 15030050
doit 15030051
doit 15030053
doit 15030054
doit 15030055
doit 15030057
doit 15030058
doit 15030059
doit 15030060
doit 15030063
doit 15030064
doit 15030065
doit 15030066
doit 15030067

exit

