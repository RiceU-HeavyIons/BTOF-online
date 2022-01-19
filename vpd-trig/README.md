## Zero VPD TACs
from stargw log into 
```
ssh -A -x staruser@startrg.starp.bnl.gov
```

from in `/home/startrg/staruser` we should see

![[attachments/Screen Shot 2021-12-13 at 9.07.21 .png]]

We want it to look like:
```
lrwxrwxrwx. 1 staruser staruser   18 Dec 16 11:48 vpd_tac.dat -> vpd_tac_zeroed.dat
```

Now RUN  `create_bbq_offset` to make it stick


### For MTD electronics (MXQ)

```
lrwxrwxrwx. 1 staruser staruser   24 Feb 26  2019 mvpd_tac.dat -> mvpd_tac_run20054122.dat
```

then ZERO it
```
unlink mvpd_tac.dat

ln -s mvpd_tac_zeroed.dat mvpd_tac.dat
```

then check with `ls -lah`

```
lrwxrwxrwx. 1 staruser staruser   19 Dec 13 11:10 mvpd_tac.dat -> mvpd_tac_zeroed.dat
```

Next run:
```
create_mxq_offset
```

Then to apply these take:
pedestal_rhicclock_clean TRG+DAQ 1k events 
NOTE: unless something is really broken, it is 100% safe to take this during collisions

After that take the data you need for the TAC calibration:
tune or production strawman trigger with NON-VPD coincidence trigger (usually BBC or EPD, sometimes ZDC but it is less efficient to take more events)
TRG+DAQ+TOF 500k events (more if using ZDC)



## TAC Offset analysis
Github repo with commissioning code:
https://github.com/RiceU-HeavyIons/BTOF-online

working in 

```
pwd
> /star/data01/subsysg/TOF/run22/BTOF-online/online
git clone git@github.com:RiceU-HeavyIons/BTOF-online.git
```

previously this did not include `vpd-trig` but I have added it.

Get the data: (note we forgot to zero the MXQ, so this is practice)
```
hsi
ls -1 /home/starsink/raw/daq/2021/347/22347035

# get trigger data, you can use DAQ files but this is much faster
hpss_user.pl /home/starsink/raw/daq/2021/347/22347034/run22347034.tgz /star/data01/subsysg/TOF/run22/DAQ/tac/run22347034.tgz 
```


NOTE: there is a README in the directory with the below info (but I've updated below some)
```
cd /star/data01/subsysg/TOF/run22/BTOF-online/vpd-trig
cd readDat
starver dev
cons
```

1. Create trgEvent trees

make sure the trigger data pulled from HPSS is in a usable area and untar it, this will output a ton of `.dat` files:
```
pwd
>/star/data01/subsysg/TOF/run22/DAQ/tac
tar -xzvf run22347034.tgz 

ls -lah *.dat

[rcas6006] /<3>TOF/run22/DAQ/tac/> ls -lah *.dat
-rw-r--r-- 1 jdb rhstar 32M Dec 13 09:44 run22347034.10.dat
-rw-r--r-- 1 jdb rhstar 32M Dec 13 09:44 run22347034.11.dat
-rw-r--r-- 1 jdb rhstar 32M Dec 13 09:44 run22347034.12.dat
-rw-r--r-- 1 jdb rhstar 32M Dec 13 09:44 run22347034.13.dat
-rw-r--r-- 1 jdb rhstar 32M Dec 13 09:44 run22347034.14.dat
-rw-r--r-- 1 jdb rhstar 32M Dec 13 09:44 run22347034.15.dat
...
...

```


```bash

cd /star/data01/subsysg/TOF/run22/BTOF-online/vpd-trig/readDat/
ln -sf /star/data01/subsysg/TOF/run22/DAQ/tac trgdata
mkdir output
#build the code
starver dev
cons
# now run it
./simpleProcess.sh
```
 produced trees in `output`:

2. Compute TAC offsets
```bash
cd /star/data01/subsysg/TOF/run22/BTOF-online/vpd-trig/vpdOffset/
make
ls -1 /star/data01/subsysg/TOF/run22/BTOF-online/vpd-trig/readDat/output/*.root > list.lis
[rcas6006] /<4>run22/BTOF-online/vpd-trig/vpdOffset/> ./analysis list.lis run22347034
 read in file /star/data01/subsysg/TOF/run22/BTOF-online/vpd-trig/readDat/output/run22347034.10.root
 read in file /star/data01/subsysg/TOF/run22/BTOF-online/vpd-trig/readDat/output/run22347034.11.root
 read in file /star/data01/subsysg/TOF/run22/BTOF-online/vpd-trig/readDat/output/run22347034.12.root
3 files read in
== total entries : 30000
Proceeding 0-th event
Proceeding 3000-th event
Proceeding 6000-th event
Proceeding 9000-th event
Proceeding 12000-th event
Proceeding 15000-th event
Proceeding 18000-th event
Proceeding 21000-th event
Proceeding 24000-th event
Proceeding 27000-th event
VPD and MVPD offset calibration done !
```
NOTE: with only 3 files it was not really enough statistics, just giving an example - use as much data as possible.

produced:
bbq_run22347034.pdf
mvpd_tac_run22347034.dat
mxq_run22347034.pdf
vpd_tac_run22347034.dat

I like to always copy these to my webspace somewhere, eg:
https://www.star.bnl.gov/protected/lfsupc/jdb/ops/22/

### Apply the new TAC offsets

logged into startrg:
```bash
scp jdb@rftpexp.rhic.bnl.gov:/star/data01/subsysg/TOF/run22/BTOF-online/vpd-trig/vpdOffset/mvpd_tac_run22347034.dat mvpd_tac_20211213_run22347034.dat
scp jdb@rftpexp.rhic.bnl.gov:/star/data01/subsysg/TOF/run22/BTOF-online/vpd-trig/vpdOffset/vpd_tac_run22347034.dat vpd_tac_20211213_run22347034.dat

unlink vpd_tac.dat
ln -s vpd_tac_20211213_run22347034.dat vpd_tac.dat

unlink mvpd_tac.dat
ln -s mvpd_tac_20211213_run22347034.dat mvpd_tac.dat

create_bbq_offset
create_mxq_offset
```
AGAIN NOTE: this particular run did not have the MXQ offsets zeroed, so it is not used, just an example of the procedure


3. Reboot TRG
4. pedestal_rhicclock_clean TRG+DAQ (1k)
TAC offsets are applied.

