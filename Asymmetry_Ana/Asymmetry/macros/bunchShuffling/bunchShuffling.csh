#!/bin/tcsh

source /opt/phenix/bin/phenix_setup.csh
setenv LD_LIBRARY_PATH /direct/phenix+u/nialewis/install/lib:${LD_LIBRARY_PATH}

cd /direct/phenix+u/workarea/nialewis/Run15ppPhotons/Asymmetry/macros/bunchShuffling

echo "process number " $1
echo "number of shuffles " $2

root -l asymmetryBunchShuffle.C\($1,$2,\"shuffled$1.root\"\)
