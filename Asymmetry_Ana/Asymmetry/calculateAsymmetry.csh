#!/bin/tcsh
setenv LD_LIBRARY_PATH /direct/phenix+u/nialewis/install/lib:${LD_LIBRARY_PATH}

#This is a .csh file to do all of the pesky compiling asymmetry calculating and #asymmetry processing in one command!

echo "particle" $1 #can be either: \"dp\" \"pi0\" or \"eta\"
echo "Direct Photon Background - " $2 #True or false, Only applies to pi0 or eta
echo "Isolation Cut            - " $3 #True or false, Only applies to pi0 or eta

cd Run15ppPhotons/Asymmetry

make install

echo
echo
echo
echo
echo

cd macros

root -l -q asymmetry.C\($1,$2,$3\)

echo
echo
echo
echo
echo

root -l -b -q processAsymmetry.C\($1,$2,$3\)

echo
echo
echo "************************************"
echo "  particle" $1 #can be either: \"dp\" \"pi0\" or \"eta\"
echo "  Direct Photon Background - " $2 #True or false, Only for to pi0 or eta
echo "  Isolation Cut            - " $3 #True or false, Only for to pi0 or eta
echo "************************************"
