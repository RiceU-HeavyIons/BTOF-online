Oflwindows
===

Extract the leading edge and trainling edge times for calculating the offline (online) TOF data selection windows.

## Running the code
```bash
./make.sh
./tof_datawindows -o tof_{RN}.root -D tof /path/to/daq/file.daq >& tof_{RN}.log
```
Where {RN} is the run number of interest.
This will produce the root files with 2D histograms of le / tr times versus tray.

## Plot & Compute Windows
Make sure that the input file is named "tof_{RN}.root".
next:

```bash
root -b -q -l 'extract_windows.C( "{RN}" )'
```

This will produce:
- "tof\_datawindows\_{RN}.pdf" : a pdf report of the fit and windows
- "tof\_datawindows\_{RN}.dat" : the low and high edge values to be uploaded to the DB