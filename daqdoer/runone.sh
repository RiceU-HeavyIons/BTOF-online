#!/usr/local/bin/tcsh

setenv thisrunnum 15086055

cd /star/u/llope/run14/daqdoer
set echo

#./daqdoer $DATA03/$thisrunnum/*${thisrunnum}*.daq << endd

./daqdoer $DATA03/$thisrunnum/st_physics_15086055_raw_1500003.daq \
$DATA03/$thisrunnum/st_physics_15086055_raw_1500013.daq \
$DATA03/$thisrunnum/st_zerobias_adc_15086055_raw_1500005.daq \
$DATA03/$thisrunnum/st_zerobias_adc_15086055_raw_2000006.daq \
$DATA03/$thisrunnum/st_zerobias_adc_15086055_raw_2500011.daq << endd
3
${thisrunnum}_tof
endd

exit