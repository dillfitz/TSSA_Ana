#!/bin/csh

#set baseDir=`pwd`
set baseDir = /phenix/spin/phnxsp01/dillfitz/sim/decay
#echo "sourcing setup...";

source $HOME/.login
source /opt/phenix/bin/phenix_setup.csh -n


#printenv OFFLINE_MAIN
#printenv LD_LIBRARY_PATH
echo "input parameters: " $1 $2
set process = $2+300
echo $process

echo "running d0 bar decay asymmetry: ";

cd ${baseDir}

root -b -q decayAsymmetryD0bar_condor.C\($process\)
