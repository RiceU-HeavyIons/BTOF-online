Oflwindows
===

Extract the leading edge and trainling edge times for calculating the offline (online) TOF data selection windows.

## running the code
```bash
./make.sh
./tof_datawindows -o tof\_RN.root -D tof /path/to/daq/file.daq >& tof\_rn.log
```

this will produce the root files with 2D histograms of le / tr times versus tray.