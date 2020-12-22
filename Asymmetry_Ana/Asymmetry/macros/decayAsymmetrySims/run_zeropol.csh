#! /bin/csh
#root -b -q runpairs_deprecal.C
#mv oscar_$par3$par1.txt jpsi_060.txt
#@ new1 = $1 + 2;
#echo "new par1 = " $new1

@ par3 = $1 ;
echo "par3 = " $par3;
@ par1 = $2;
@ par2 = 0;
echo "par2 = " $par2;
root -b -q jpsidecay.C\($par3,$par1\)
mv oscar_$par3$par1.txt oscar/zeropol/jpsi_$par3$par1$par2.txt

@ par2 = $par2 + 1;
echo "par2 = " $par2;
root -b -q jpsidecay.C\($par3,$par1\)
mv oscar_$par3$par1.txt oscar/zeropol/jpsi_$par3$par1$par2.txt

@ par2 = $par2 + 1;
echo "par2 = " $par2;
root -b -q jpsidecay.C\($par3,$par1\)
mv oscar_$par3$par1.txt oscar/zeropol/jpsi_$par3$par1$par2.txt

@ par2 = $par2 + 1;
echo "par2 = " $par2;
root -b -q jpsidecay.C\($par3,$par1\)
mv oscar_$par3$par1.txt oscar/zeropol/jpsi_$par3$par1$par2.txt

@ par2 = $par2 + 1;
echo "par2 = " $par2;
root -b -q jpsidecay.C\($par3,$par1\)
mv oscar_$par3$par1.txt oscar/zeropol/jpsi_$par3$par1$par2.txt

@ par2 = $par2 + 1;
echo "par2 = " $par2;
root -b -q jpsidecay.C\($par3,$par1\)
mv oscar_$par3$par1.txt oscar/zeropol/jpsi_$par3$par1$par2.txt

@ par2 = $par2 + 1;
echo "par2 = " $par2;
root -b -q jpsidecay.C\($par3,$par1\)
mv oscar_$par3$par1.txt oscar/zeropol/jpsi_$par3$par1$par2.txt

@ par2 = $par2 + 1;
echo "par2 = " $par2;
root -b -q jpsidecay.C\($par3,$par1\)
mv oscar_$par3$par1.txt oscar/zeropol/jpsi_$par3$par1$par2.txt

@ par2 = $par2 + 1;
echo "par2 = " $par2;
root -b -q jpsidecay.C\($par3,$par1\)
mv oscar_$par3$par1.txt oscar/zeropol/jpsi_$par3$par1$par2.txt

@ par2 = $par2 + 1;
echo "par2 = " $par2;
root -b -q jpsidecay.C\($par3,$par1\)
mv oscar_$par3$par1.txt oscar/zeropol/jpsi_$par3$par1$par2.txt


