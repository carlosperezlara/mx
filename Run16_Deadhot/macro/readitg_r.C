#include<map>
#include<vector> 
void readitg_r(){
  gSystem->Load("libMyMpcEx.so");
  MpcExMapper* mapper = MpcExMapper::instance();
  TH1D* hhigh_itg[2][5];
  TH1D* hlow_itg[2][5];
  

  TFile* file = new TFile("Integral.root","READONLY");
  if(!file){
    cout <<"open file failed !!!"<<endl;
    exit(1);
  }

//  double r_min = 6.5;
//  double r_max = 20.6;
  const int ndiv = 9;
//  double dlt = (r_max-r_min)/ndiv;

  double rbins[ndiv+1] ={6.5,9,10.5,12.1,13.5,14.4,15.6,16.8,18.1,21.5}; 
  //make a map
  map<int,int>key_to_key;
  vector<int>layer_new_key[2][8];
  for(int iarm = 0;iarm < 2;iarm++){
    for(int ilayer = 0;ilayer < 8;ilayer++){
      layer_new_key[iarm][ilayer].clear();
    }
  }
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
    layer_new_key[arm][layer].push_back(new_key);
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

//  hlow_itg[1][0]->Draw();
 
  for(int iitg=0;iitg<5;iitg++){
    char txt_name[500];
    sprintf(txt_name,"bad_high_itg%d.txt",iitg);
    ofstream bad_high_itg(txt_name);
    sprintf(txt_name,"bad_low_itg%d.txt",iitg);
    ofstream bad_low_itg(txt_name);
    for(int iarm = 0;iarm < 2;iarm++){
      for(int ilayer = 0;ilayer < 8;ilayer++){
        if(iarm == 0 && (ilayer == 2 || ilayer == 3)) continue;
	double mean0[ndiv] = {0};
	double mean1[ndiv] = {0};
	double sq_mean0[ndiv] = {0};
	double sq_mean1[ndiv] = {0};
	double rms0[ndiv]={0.};
	double rms1[ndiv]={0.};
	int count0[ndiv] = {0};
	int count1[ndiv] = {0};
        int size = layer_new_key[iarm][ilayer].size();
//cout<<size<<endl;
	for(int j = 0;j<size;j++){
	  int new_key = layer_new_key[iarm][ilayer][j];
	  int bin0 = hhigh_itg[iarm][iitg]->FindBin(new_key);
	  int bin1 = hlow_itg[iarm][iitg]->FindBin(new_key);
          double bin_content0 = hhigh_itg[iarm][iitg]->GetBinContent(bin0);
	  double bin_content1 = hlow_itg[iarm][iitg]->GetBinContent(bin1);
          int key = key_to_key[new_key];
	  double pos_x = mapper->get_x(key);
          double pos_y = mapper->get_y(key);
          double r = sqrt(pos_x*pos_x+pos_y*pos_y);
	  int n = -1;
	  for(int ibin=0;ibin<ndiv;ibin++){
            if(r >=rbins[ibin] && r <rbins[ibin+1]){
              n = ibin;
	      break;
	    }
	  }
	  if(n < 0){
            cout<<"bad r "<<r<<endl;
	    return;
	  }
	  if(bin_content0 > 1){
	    mean0[n]+=bin_content0;
	    sq_mean0[n]+=bin_content0*bin_content0;
	    count0[n]++;
	  }
	  if(bin_content1 > 1){
            mean1[n]+=bin_content1;
	    sq_mean1[n]+=bin_content1*bin_content1;
	    count1[n]++;
	  }
	}
      
	for(int k=0;k<ndiv;k++){
	  if(count0[k]== 0 || count1[k]==0){ 
	    cout<<"zero count for k "<<k<<" intgral: "<<iitg<<endl;
	    cout<<"layer "<<ilayer<<" "<<iarm<<endl;
	    continue;
	  }
	  mean0[k] = mean0[k]/count0[k];
	  mean1[k] = mean1[k]/count1[k];
	  sq_mean0[k] = sq_mean0[k]/count0[k];
	  sq_mean1[k] = sq_mean1[k]/count1[k];
	  rms0[k] = sqrt(sq_mean0[k] - mean0[k]*mean0[k]);
	  rms1[k] = sqrt(sq_mean1[k] - mean1[k]*mean1[k]);
	} 
      
	for(int j = 0;j<size;j++){
          int new_key = layer_new_key[iarm][ilayer][j];
          int bin0 = hhigh_itg[iarm][iitg]->FindBin(new_key);
	  int bin1 = hlow_itg[iarm][iitg]->FindBin(new_key);
	  double bin_content0 = hhigh_itg[iarm][iitg]->GetBinContent(bin0);
	  double bin_content1 = hlow_itg[iarm][iitg]->GetBinContent(bin1);
          int key = key_to_key[new_key];
	  double pos_x = mapper->get_x(key);
          double pos_y = mapper->get_y(key);
          double r = sqrt(pos_x*pos_x+pos_y*pos_y);
	  int n = -1;
	  for(int ibin=0;ibin<ndiv;ibin++){
            if(r >=rbins[ibin] && r <rbins[ibin+1]){
              n = ibin;
	      break;
	    }
	  }
	  if(n < 0){
            cout<<"bad r "<<endl;
	    return;
	  }

	  if(bin_content0 > mean0[n]+3.*rms0[n] || bin_content0 < mean0[n]-3.*rms0[n]|| bin_content0 < 1){
            bad_high_itg<<key_to_key[new_key]<<" "<<new_key<<endl;
	  }
	  if(bin_content1 > mean1[n]+3.*rms1[n] || bin_content1 < mean1[n]-3.*rms1[n]|| bin_content1 < 1){
	    bad_low_itg<<key_to_key[new_key]<<" "<<new_key<<endl;
	  }
	}
      }
    }
  }
}
