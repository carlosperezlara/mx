#!/bin/bash
#high pedestal

pass=3

#type 1 pass (remove deadhot by row in one sensor)
for j in $(seq 1 $pass);
  do
   echo pass $j
   root -l -q -b deadhot.C
   root -l -q -b readitg_r.C
   high_ped="1 2 3 4"
   high_merge=""
   for i in $high_ped;
     do 
     high_merge=" $high_merge bad_high_itg$i.txt"
   done
   rm -f temp_deadhot.txt 
   cat $high_merge > temp_deadhot.txt
   sort -u -n temp_deadhot.txt > high_deadhot.txt
   rm -f temp_deadhot.txt
   low_ped="1 2 3 4"
   low_merge=""
   for i in $low_ped;
     do
      low_merge="$low_merge bad_low_itg$i.txt"
   done
   rm -f temp_deadhot.txt
   cat $low_merge > temp_deadhot.txt
   sort -u -n temp_deadhot.txt > low_deadhot.txt
   rm -f temp_deadhot.txt
done
 

