#!/bin/tcsh

source /opt/phenix/bin/phenix_setup.csh
setenv LD_LIBRARY_PATH /direct/phenix+u/dillfitz/install/lib:${LD_LIBRARY_PATH}

cd /phenix/spin/phnxsp01/dillfitz/pA_pi0_eta/TSSA_Ana/Asymmetry_Ana/Asymmetry/macros/bunchShuffling
echo "process number " $1
echo "number of shuffles " $2
echo "particle " $3
echo "collision system " $4
echo "background (true or false)" $5

root -l asymmetryBunchShuffle_pA.C\($1,$2,$3,$4,$5 \)

