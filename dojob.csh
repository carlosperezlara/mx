#!/bin/tcsh
@ num = $1 + 1
setenv file `head -$num files.dat | tail -1`
echo $file
./recoMPC $file
exit
