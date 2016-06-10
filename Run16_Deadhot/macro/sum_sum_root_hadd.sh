#!/bin/bash
merge_list=""
run_number=${1}
for i in `ls -d 45*`;
  do
   echo $i
   if grep -q $i good_good_Run_List.txt;
     then
     merge_list="$merge_list $i/sum.root"
     if test $i -eq 455052;then
       break
     fi
   fi
done
echo $merge_list
hadd sum_sum0.root ${merge_list}
