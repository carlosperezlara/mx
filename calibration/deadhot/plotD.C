#include "../../pr/mxGeometry.h"
#include "../../pr/mxGeometry.cxx"
#include "../loadbkeys.C"
void plotD(int run=454798) {
  //gStyle->SetOptStat(0);
  mxGeometry *geo = new mxGeometry();
  loadbkeys();
  TH2F *D[16];
  for(int d=0; d!=16; ++d) {
    if(d%2==0)
      D[d] = new TH2F( Form("D%02d",d),Form("D%02d",d), 460,-22,22, 88,-22,22 );
    else
      D[d] = new TH2F( Form("D%02d",d),Form("D%02d",d), 88,-22,22, 460,-22,22 );
  }
  ifstream ifile( Form("%d.dat",run) );
  for(int k=0; k!=49152; ++k) {
    float int0, int1, int2, int3;
    ifile >> k >> int0 >> int1 >> int2 >> int3;
    int lyr = geo->LyrIdx( k );
    if(lyr>8) lyr -=1;
    int key = k;
    int pkt = k/3072;
    float x = geo->X(key);
    float y = geo->Y(key);
    lyr = geo->LyrIdx( key );
    if(lyr>8) lyr -=1;
    D[lyr]->Fill(x,y,pkt);
  }
  ifile.close();

  TCanvas *main = new TCanvas();
  main->Divide(4,4,0,0);
  for(int d=0; d!=16; ++d) {
    main->cd( d+1 );
    //main->SetLogz(1);
    if(d<8) D[d]->GetZaxis()->SetRangeUser(0.1,150);
    else D[d]->GetZaxis()->SetRangeUser(0.1,20);
    D[d]->Draw("colz");
  }

  cout << "[DONE]" << endl;
}
