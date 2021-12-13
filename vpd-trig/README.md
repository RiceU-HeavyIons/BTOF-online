# vpd-trig
vpd trigger commissioning code (makes trgEvent trees, computes TAC offsets)
---

### original code from Shuai:   
/star/u/syang/run18/mtdTrig    
```bash
ls -1 /home/starsink/raw/daq/2018/071/19071076/
hpss_user.pl /home/starsink/raw/daq/2018/071/19071076/run19071076.tgz /star/data03/daq/jdb/tof/run19071076.tgz
hpss_user.pl /home/starsink/raw/daq/2018/071/19071076/st_physics_19071076_raw_0000001.daq /star/data03/daq/jdb/tof/st_physics_19071076_raw_0000001.daq
```

1. Create trgEvent trees
```bash
rm /star/data01/subsysg/TOF/vpd-trig/readDat/trgdata/*
cp /star/data03/daq/jdb/tof/run19071076.tgz /star/data01/subsysg/TOF/vpd-trig/readDat/trgdata/
cd /star/data01/subsysg/TOF/vpd-trig/readDat/trgdata/
tar -xzvf run19071076.tgz

cd /star/data01/subsysg/TOF/vpd-trig/readDat/
./processTrgData.sh
```
 produced trees in :   
/star/data01/subsysg/TOF/vpd-trig/readDat/output/19071076/   

2. Compute TAC offsets
```bash
make
cd /star/data01/subsysg/TOF/vpd-trig/vpdOffset
ls /star/data01/subsysg/TOF/vpd-trig/output/19071076/run19071076*.root > list.lis
./analysis list.lis run19071076
```
produced:  
bbq_run19071076.pdf   
mxq_run19071076.pdf  
mvpd_tac_run19071076.dat  
vpd_tac_run19071076.dat  

logged into startrg:  
```bash
scp jdb@rftpexp.rhic.bnl.gov:/star/data01/subsysg/TOF/vpd-trig/vpdOffset/mvpd_tac_run19071076.dat mvpd_tac_20180312_run19071076.dat
scp jdb@rftpexp.rhic.bnl.gov:/star/data01/subsysg/TOF/vpd-trig/vpdOffset/vpd_tac_run19071076.dat vpd_tac_20180312_run19071076.dat

unlink vpd_tac.dat
ln -s vpd_tac_20180312_run19071076.dat vpd_tac.dat

unlink mvpd_tac.dat
ln -s mvpd_tac_20180312_run19071076.dat mvpd_tac.dat

create_bbq_offset
create_mxq_offset
```

3. Reboot TRG  
4. pedestal_rhicclock_clean TRG+DAQ   
TAC offsets are applied.   

