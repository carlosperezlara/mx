Liankun Zou @2016/06/04

mMpcExDeadHot: module run through DSTs created high and low ADC distributins
               for each channel for both raw and pedestal subtracted.Use the 
	       Minbias trigger, stack=1 and cell id cut
setup.csh:     make install directory in local and add to enviroment

Fun4All_MpcExDataAna.C: macro to run mMpcExDeadHot module
good_good_Run_List.txt: good run list for dA200, exclude runs when has an error

run_deadhot_condor.sh
run_one_job.sh
submit_to_condor.sh : condor script, will run all runs in good_good_Run_List.txt

sum_all_root.sh
sum_root_hadd.sh: merge all segment of one run into one root file
sum_sum_root_hadd.sh: merge between runs for low statistic 

deadhot.C: integrated ADC for all channels(excludes the channels in 
           high_deadhot.txt and low_deadhot.txt) and create Integral.root file
readitg.C: read Integral.root and compare integrated ADCs in rows and create
           bad_low_itg%d.txt bad_low_itg%d.txt files
readitg_r.C: read Integral.root and compare integrated ADCs in r for each layers
             create bad_low_itg%d.txt bad_low_itg%d.txt files

make_bad_ped.sh : run deadhot.C->readitg.C-> 
                  merge bad_low_itg%d.txt bad_low_itg%d.txt to
		  high_deadhot.txt and low_deadhot.txt
		  will run 3 pass
                  
make_bad_rad_ped.sh : run deadhot.C-> readitg_r.C->
                      merge bad_low_itg%d.txt bad_low_itg%d.txt to
		      high_deadhot.txt and low_deadhot.txt
		      will run 3 pass

