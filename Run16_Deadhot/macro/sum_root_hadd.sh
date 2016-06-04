#!/bin/bash
merge_list=""
run_number=${1}
for i in `ls ${run_number}/*.root`;
  do
#   echo $i
   merge_list="$merge_list $i"
done
echo $merge_list
hadd ${run_number}/sum.root ${merge_list}
