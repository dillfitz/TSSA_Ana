Original author of Asymmetry library: Nicole Lewis

The classes that are sued to calculate the asymmetry are:
Asymmetry            - Does the actual count counting and asymmetry calculating
Aceptance Correction - Calculates the <|cos\phi|> correction while the asymmetry
                       counts are being counts
RelativeLuminosity   - Calculates the relative luminosity for when data from 
                       multiple fills are being combined
Polarization         - Calculates the polarization for of a fill gorup when 
                       multiple fills are combined

All of the constants used (i.e. the number of fill groups, pt bins, the option 
ints for what kind of asymmetry you're calculating) are all stored in the 
Constants.h file.

The macros that use these classes are located in the "macro" subdirectory:
asymmetry.C  
processAsymmetry.C

The inputs of this function are meant to make it easier to run over different 
data sets.  This code is currently set up to run over "dp" (direct photon), 
"pi0" and "eta" photon data, as well as single electron data (or single charged tracks) at midrapidity. Setting "dpBackground" to true for pi0 and eta data
will evaluate the asymmetry as a function of photon pt instead of hadron pt.
Set

macros/asymmetry.C is made to run over a file of particles where most data cuts 
have already been aplied and the pt bins (or the bins of what ever value you 
are evaluating the asymmetry as a function of) have already been found and set 
to negative if the entry is found to be out of range.  It also reequires file 
called "fill.root" that has all of the fill by fill polarization and relative 
luminosity in formation in it.  

macros/asymmetry.C outputs a graph recording the relative luminosities and 
polarizations for the fill groups and also graphs of the asymmetry as fucntion 
of fill.  There are six of these graphs (yellow left lumi, yellow right lumi, 
yellow square root, and all the same but for the blue beam) for each pt bin.  

"Blank asymmetries" happen when the one of the counts one used for the asymmetry
calculation are not greater than 10.  When this happens the asymmetry is set to
0 +/- 1

macros/processAsymmetry.C removes these blank 0 +/- 1 asymmetries before fitting
that asymmetry as a function of fill group to a constant.  It makes the 
asymmetry values into TGraphAsymmErrors which can be convenitently plotted. This
macro also takes the weighted average of the left and right relative luminosity 
asymmetries and the weighted average of the blue and yellow beam asymmetries to 
find the total asymmetry for the square root and relative luminosity formulas.  

macros/sinPhi has all the code required to calculate the sin phi modulation TSSA
cross check

macros/bunchShuffling has all the macros for performing the bunch shuffling
systematic study

macros/plotMacros contains plotting macros that produce most of the A_N plots 
seen in AN1326 and AN1373

macros/directPhoton has macros the correct the direct photon TSSA for background
as well as calculate the systematic errors due to the background correction. 

macros/directPhoton/prettyPretty.C produces the final direct photon plot for 
prelinminary

macros/etaMeson has macros to correct the eta meson background, find the 
systematic error due to background subtraction and make the final preliminary
eta meson plots
