#include<map>
void readitg(){
  gSystem->Load("libMyMpcEx.so");
  MpcExMapper* mapper = MpcExMapper::instance();
  TH1D* hhigh_itg[2][5];
  TH1D* hlow_itg[2][5];
  
  TH1D* hhigh_mean_row[5];
  TH1D* hlow_mean_row[5];
  TH1D* hhigh_rms_row[5];
  TH1D* hlow_rms_row[5];

  for(int i = 0;i<5;i++){
    char hname[500];
    sprintf(hname,"hhigh_mean_row_%d",i);
    hhigh_mean_row[i] = new TH1D(hname,hname,1536,-0.5,1536-0.5);
    sprintf(hname,"hlow_mean_row_%d",i);
    hlow_mean_row[i] = new TH1D(hname,hname,1536,-0.5,1536-0.5);
    sprintf(hname,"hhigh_rms_row_%d",i);
    hhigh_rms_row[i] = new TH1D(hname,hname,1536,-0.5,1536-0.5);
    sprintf(hname,"hlow_rms_row_%d",i);
    hlow_rms_row[i] = new TH1D(hname,hname,1536,-0.5,1536-0.5);
  }



  TFile* file = new TFile("Integral.root","READONLY");
  
  //make a map
  map<int,int>key_to_key;
  MpcExMapper* mapper = MpcExMapper::instance();
  for(int i = 0;i < 49152;i++){
    int arm = mapper->get_arm(i);
    int layer = mapper->get_layer(i);
    int quadrant = mapper->get_quadrant(i);
    int sensor = mapper->get_sensor_in_quadrant(i);
    int index = 6*quadrant+sensor;
    int lx = mapper->get_lx(i);
    int ly = mapper->get_ly(i);
    int new_key = 24576*arm+3072*layer+128*index+32*ly+lx;
    key_to_key.insert(pair<int,int>(new_key,i));
  }

  for(int iarm = 0;iarm < 2;iarm++){
    for(int iitg=0;iitg<5;iitg++){
      char hname[500];
      sprintf(hname,"hhigh_adc_arm%d_integral%d",iarm,iitg);
      hhigh_itg[iarm][iitg] = (TH1D*)file->Get(hname);
      sprintf(hname,"hlow_adc_arm%d_integral%d",iarm,iitg);
      hlow_itg[iarm][iitg] = (TH1D*)file->Get(hname);
    }
  }
  for(int iitg=0;iitg<5;iitg++){
    char txt_name[500];
    sprintf(txt_name,"bad_high_itg%d.txt",iitg);
    ofstream bad_high_itg(txt_name);
    sprintf(txt_name,"bad_low_itg%d.txt",iitg);
    ofstream bad_low_itg(txt_name);
    for(int iarm = 0;iarm < 2;iarm++){
      for(int i = 0;i < 192*4;i++){
        double mean0 = 0;
	double mean1 = 0;
	double sq_mean0 = 0;
	double sq_mean1 = 1;
	int count0 = 0;
	int count1 = 0;
	for(int j = 32*i;j<32*(i+1);j++){
          double bin_content0 = hhigh_itg[iarm][iitg]->GetBinContent(j+1);
	  double bin_content1 = hlow_itg[iarm][iitg]->GetBinContent(j+1);
          if(bin_content0 > 1){
            mean0+=bin_content0;
	    sq_mean0+=bin_content0*bin_content0;
	    count0++;
	  }
	  if(bin_content1 > 1){
            mean1+=bin_content1;
	    sq_mean1+=bin_content1*bin_content1;
	    count1++;
	  }
	}
       	if(count0 == 0 || count1 == 0) continue;
	mean0 = mean0/count0;
	mean1 = mean1/count1;
	sq_mean0 = sq_mean0/count0;
	sq_mean1 = sq_mean1/count1;
	double rms0 = sqrt(sq_mean0 - mean0*mean0);
	double rms1 = sqrt(sq_mean1 - mean1*mean1);
//cout<<iarm*192*4+i<<" "<<mean0<<" "<<rms0<<" "<<mean1<<" "<<rms1<<endl;
	hhigh_mean_row[iitg]->Fill(iarm*192*4+i,mean0);
	hhigh_rms_row[iitg]->Fill(iarm*192*4+i,rms0);
	hlow_mean_row[iitg]->Fill(iarm*192*4+i,mean1);
	hlow_rms_row[iitg]->Fill(iarm*192*4+i,rms1);
	 
	for(int j = 32*i;j<32*(i+1);j++){
          double bin_content0 = hhigh_itg[iarm][iitg]->GetBinContent(j+1);
	  double bin_content1 = hlow_itg[iarm][iitg]->GetBinContent(j+1);
          if(bin_content0 > mean0+3*rms0 || bin_content0 < mean0-3*rms0 || bin_content0 < 1){
            int new_key = 24576*iarm+j;
            bad_high_itg<<key_to_key[new_key]<<" "<<new_key<<endl;
	  }
	  if(bin_content1 > mean1+3*rms1 || bin_content1 < mean1-3*rms1 || bin_content1 < 1){
            int new_key = 24576*iarm+j;
	    bad_low_itg<<key_to_key[new_key]<<" "<<new_key<<endl;
	  }
	}
      }
    }
  }


  TFile* ofile = new TFile("row_mean_sigma.root","RECREATE");
  for(int i = 0;i<5;i++){
    hhigh_mean_row[i]->Write();
    hhigh_rms_row[i]->Write();
    hlow_mean_row[i]->Write();
    hlow_rms_row[i]->Write();
  }
  
}
