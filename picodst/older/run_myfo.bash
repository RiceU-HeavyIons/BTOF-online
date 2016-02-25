#!/bin/bash

day=122

DIRIN=/star/institutions/rice/llope/myfastoffline
DIROUT=/star/institutions/rice/llope/pdst/run11/my$day/pDst_

root4star -b -q -l 'cp_myfo.C("'$DIRIN'","'$DIROUT'")'

exit
