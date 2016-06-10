#include<fstream>
void count_deadhot(){
  ifstream high_deadhot("high_deadhot.txt");
  ifstream low_deadhot("low_deadhot.txt");
  int key;
  int new_key;
  std::set<int> bad_high_itg_list;
  std::set<int> bad_low_itg_list;
  int good_high_bad_low=0;
  int bad_high_good_low=0;
  int bad_high_bad_low=0;
  while(high_deadhot>>key>>new_key){
//    cout<<key<<" "<<new_key<<endl;
    bad_high_itg_list.insert(key);
  }

   while(low_deadhot>>key>>new_key){
//    cout<<key<<" "<<new_key<<endl;
    bad_low_itg_list.insert(key);
  }
 
  std::set<int>::iterator it_begin= bad_high_itg_list.begin();
  std::set<int>::iterator it_end= bad_high_itg_list.end();

  for(std::set<int>::iterator it=it_begin; it!=it_end; ++it){
    int high_key = *it;
    if(bad_low_itg_list.find(high_key)==bad_low_itg_list.end()){
      bad_high_good_low++;
    }
    else bad_high_bad_low++;
  }
  it_begin = bad_low_itg_list.begin();
  it_end = bad_low_itg_list.end();
  for(std::set<int>::iterator it=it_begin; it!=it_end; ++it){
    int low_key = *it;
    if(bad_high_itg_list.find(low_key) == bad_high_itg_list.end()){
      good_high_bad_low++;
    }
  }
  
  cout<<"good high bad low: "<<good_high_bad_low<<endl;
  cout<<"bad high good low: "<<bad_high_good_low<<endl;
  cout<<"bad high bad low: "<<bad_high_bad_low<<endl;
}

