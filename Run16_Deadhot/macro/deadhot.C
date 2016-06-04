#include <set>
void deadhot(){
  gSystem->Load("libMyMpcEx.so");
  TFile* file = new TFile("sum_sum0.root","READONLY");
  TH2D* hhigh_adc_key = (TH2D*)file->Get("hhigh_adc_key");
  TH2D* hlow_adc_key = (TH2D*)file->Get("hlow_adc_key");

  MpcExMapper* mapper = MpcExMapper::instance();
  ifstream high_bad_rms("high_bad_rms.txt");
  ifstream low_bad_rms("low_bad_rms.txt");
  ifstream high_bad_mean("high_bad_mean.txt");
  ifstream low_bad_mean("low_bad_mean.txt");
  ifstream bad_high_itg_total("high_deadhot.txt");
  ifstream bad_low_itg_total("low_deadhot.txt");

  std::set<int> bad_rms_list_h;
  std::set<int> bad_rms_list_l;
  std::set<int> bad_mean_list_h;
  std::set<int> bad_mean_list_l;
  std::set<int> bad_high_itg_list;
  std::set<int> bad_low_itg_list;

  TH1D* hhigh_itg[2][5];
  TH1D* hlow_itg[2][5];
  TH2D* hhigh_rphi_itg[2][8][5];
  TH2D* hlow_rphi_itg[2][8][5];

  TH2D* hhigh_r_itg[2][8][5];
  TH2D* hlow_r_itg[2][8][5];

  double high_itg_begin_type0[2][5]={{0,15,50,100,150},{0,15,30,60,100}};
  double high_itg_begin_type1[2][5]={{0,10,45,95,145},{0,10,25,55,95}};
  double high_itg_end_type0[2][5]={{15,50,100,150,200},{15,30,60,100,150}};
  double high_itg_end_type1[2][5]={{10,45,95,145,195},{10,25,55,95,145}};

  double low_itg_begin[5] = {0,5,16,30,50};
  double low_itg_end[5] = {5,16,30,50,100};

  double high_range[2][5]={{500000,500000,100000,30000,10000},
                           {30000,20000,20000,10000,5000}};
  double low_range[2][5]={{500000,500000,100000,15000,10000},
                          {40000,30000,20000,3000,1000}};
  
  for(int iarm = 0;iarm < 2;iarm++){
    for(int iitg=0;iitg<5;iitg++){
      char hname[500];
      sprintf(hname,"hhigh_adc_arm%d_integral%d",iarm,iitg);
      hhigh_itg[iarm][iitg] = new TH1D(hname,hname,24576,iarm*24576-0.5,(iarm+1)*24576-0.5);
      sprintf(hname,"hlow_adc_arm%d_integral%d",iarm,iitg);
      hlow_itg[iarm][iitg] = new TH1D(hname,hname,24576,iarm*24576-0.5,(iarm+1)*24576-0.5);
      for(int ilayer = 0;ilayer < 8;ilayer++){
        sprintf(hname,"hhigh_rphi_arm%d_itg%d_layer%d",iarm,iitg,ilayer);
	hhigh_rphi_itg[iarm][ilayer][iitg] = new TH2D(hname,hname,300,0,30,30,-5,5);
	sprintf(hname,"hlow_rphi_arm%d_itg%d_layer%d",iarm,iitg,ilayer);
	hlow_rphi_itg[iarm][ilayer][iitg] = new TH2D(hname,hname,300,0,30,30,-5,5);
      
        sprintf(hname,"hhigh_r_arm%d_itg%d_layer%d",iarm,iitg,ilayer);
	hhigh_r_itg[iarm][ilayer][iitg] = new TH2D(hname,hname,300,0,30,200,0,high_range[iarm][iitg]);
        hhigh_r_itg[iarm][ilayer][iitg]->GetXaxis()->SetTitle("R/cm");
	hhigh_r_itg[iarm][ilayer][iitg]->GetYaxis()->SetTitle("Integrated ADC");
	
	sprintf(hname,"hlow_r_arm%d_itg%d_layer%d",iarm,iitg,ilayer);
	hlow_r_itg[iarm][ilayer][iitg] = new TH2D(hname,hname,300,0,30,200,0,low_range[iarm][iitg]);
        hlow_r_itg[iarm][ilayer][iitg]->GetXaxis()->SetTitle("R/cm");
	hlow_r_itg[iarm][ilayer][iitg]->GetYaxis()->SetTitle("Integrated ADC");
      }
    }
  }

  int key;
  int new_key;
  double rms;
  double mean;
  
  while(high_bad_rms>>key>>rms){
//    cout<<key<<" "<<rms<<endl;
//    bad_rms_list_h.insert(key);
  }
  while(low_bad_rms>>key>>rms){
//    cout<<key<<" "<<rms<<endl;
//    bad_rms_list_l.insert(key);
  }

  while(high_bad_mean>>key>>mean){
//    cout<<key<<" "<<mean<<endl;
//    bad_mean_list_h.insert(key);
  }

  while(low_bad_mean>>key>>mean){
//    cout<<key<<" "<<mean<<endl;
//    bad_mean_list_l.insert(key);
  }

  while(bad_high_itg_total>>key>>new_key){
//    cout<<key<<" "<<new_key<<endl;
    bad_high_itg_list.insert(key);
  }

   while(bad_low_itg_total>>key>>new_key){
//    cout<<key<<" "<<new_key<<endl;
    bad_low_itg_list.insert(key);
  }


  for(int i = 0;i < 24576*2;i++){
    hhigh_adc_key->SetAxisRange(i,i,"X");
    hlow_adc_key->SetAxisRange(i,i,"X");
    int arm = mapper->get_arm(i);
    int layer = mapper->get_layer(i);
    int quadrant = mapper->get_quadrant(i);
    int sensor = mapper->get_sensor_in_quadrant(i);
    int index = 6*quadrant+sensor;
    int lx = mapper->get_lx(i);
    int ly = mapper->get_ly(i);
    int new_key = 24576*arm+3072*layer+128*index+32*ly+lx;
    double pos_x = mapper->get_x(i);
    double pos_y = mapper->get_y(i);
    double r = sqrt(pos_x*pos_x+ pos_y*pos_y);
    double phi = atan2(pos_y,pos_x);
    TH1D* htemp0 = hhigh_adc_key->ProjectionY();
    TH1D* htemp1 = hlow_adc_key->ProjectionY();
    if(htemp0->GetEntries() < 10) continue;

    for(int iitg = 0;iitg<5;iitg++){
      double high_begin = high_itg_begin_type0[arm][iitg];
      double high_end = high_itg_end_type0[arm][iitg];
      if(layer > 1){
        high_begin = high_itg_begin_type1[arm][iitg];
	high_end = high_itg_end_type1[arm][iitg];
      }
      int high_begin_bin = htemp0->FindBin(high_begin);
      int high_end_bin = htemp0->FindBin(high_end);
      double itegral0 = htemp0->Integral(high_begin_bin,high_end_bin);
      
      if(bad_high_itg_list.find(i) == bad_high_itg_list.end()){
        hhigh_itg[arm][iitg]->Fill(new_key,itegral0);
	hhigh_rphi_itg[arm][layer][iitg]->Fill(r,phi,itegral0);
	hhigh_r_itg[arm][layer][iitg]->Fill(r,itegral0);
      }

      double low_begin = low_itg_begin[iitg];
      double low_end = low_itg_end[iitg];
      int low_begin_bin = htemp1->FindBin(low_begin);
      int low_end_bin = htemp1->FindBin(low_end); 
      double itegral1 = htemp1->Integral(low_begin_bin,low_end_bin);
      if(bad_low_itg_list.find(i) == bad_low_itg_list.end()){
	hlow_itg[arm][iitg]->Fill(new_key,itegral1);
	hlow_rphi_itg[arm][layer][iitg]->Fill(r,phi,itegral1);
	hlow_r_itg[arm][layer][iitg]->Fill(r,itegral1);
      }
    }
  }
  TFile* ofile = new TFile("Integral.root","RECREATE");
  for(int iarm = 0;iarm < 2;iarm++){
    for(int iitg = 0;iitg<5;iitg++){
      hhigh_itg[iarm][iitg]->Write();
      hlow_itg[iarm][iitg]->Write();
      for(int ilayer = 0;ilayer < 8;ilayer++){
//       hhigh_rphi_itg[iarm][ilayer][iitg]->Write();
//       hlow_rphi_itg[iarm][ilayer][iitg]->Write();
       hhigh_r_itg[iarm][ilayer][iitg]->Write();
       hlow_r_itg[iarm][ilayer][iitg]->Write();
      }
    }
  }
}
