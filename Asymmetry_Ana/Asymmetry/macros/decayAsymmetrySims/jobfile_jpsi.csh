#!/bin/csh

#set baseDir=`pwd`
set baseDir = /gpfs/mnt/gpfs02/phenix/spin/spin1/phnxsp01/shlee/polarization/sim/decays
#echo "sourcing setup...";

source $HOME/.login
source /opt/phenix/bin/phenix_setup.csh -n


#printenv OFFLINE_MAIN
#printenv LD_LIBRARY_PATH
echo "input parameters: " $1 $2

echo "running jpsi decays: ";

cd ${baseDir}

./run_zeropol.csh $1 $2 
