#include "../../pr/mxGeometry.h"
#include "../../pr/mxGeometry.cxx"
#include "../loadbkeys.C"

void plotRfix(int selarm=0, int run=454798) {
  //gStyle->SetOptStat(0);
  loadbkeys();
  mxGeometry *geo = new mxGeometry();

  TH2F *R = new TH2F( "R", "R;Radius  [cm];Ln ( int1 )", 100,5,23, 100,-2,8);
  TH2F *L = new TH2F( "L", "L;Radius  [cm];Ln ( int3 )", 100,5,23, 100,-2,8);

  TF1 *curve1, *curve2, *curve3, *curve4;
  if(selarm<1) {
    curve1 = new TF1("curve1","3.2-1.0/6.0*(x-14)",5,25);
    curve2 = new TF1("curve2","4.5-1.0/6.0*(x-14)",5,25);
    curve3 = new TF1("curve3","0.7-1.3/6.0*(x-14)",5,25);
    curve4 = new TF1("curve4","2.2-1.3/6.0*(x-14)",5,25);
  } else {
    curve1 = new TF1("curve1","+2.0-1.0/8.2*(x-14)",5,25);
    curve2 = new TF1("curve2","+3.4-1.0/8.2*(x-14)",5,25);
    curve3 = new TF1("curve3","-0.8-1.3/8.2*(x-14)",5,25);
    curve4 = new TF1("curve4","+1.0-1.3/8.2*(x-14)",5,25);
  }
  //TF1 *curve1 = new TF1("curve1","17*exp(-0.225*(x-14))",5,25);
  //TF1 *curve2 = new TF1("curve2","100*exp(-0.15*(x-14))",5,25);
  //TF1 *curve3 = new TF1("curve3","12.5*exp(-0.172*(x-14))",5,25);

  ofstream ofile( Form("../badkeys.signaloutoftrend_%d.dat",selarm) );
  ifstream ifile( Form("%d.dat",run) );
  for(int k=0; k!=49152; ++k) {
    float int0, int1, int2, int3;
    ifile >> k >> int0 >> int1 >> int2 >> int3;
    if(fBad[k]<0) continue;
    int key = k;
    float x = geo->X(key);
    float y = geo->Y(key);
    float r = sqrt( x*x + y*y );
    //int lyr = geo->LyrIdx( key );
    int pkt = key/3072;
    int sen = (key%3072)/128;
    int row = key%4;
    int arm = key/24576;
    //if((key/24576)==1) continue;
    if(arm!=selarm) continue;
    //if(sen>4) continue;
    //if(row!=0) continue;
    //cout << lyr << " " << key << endl;
    //if(k%4!=0) continue;
    if( log(int1) < curve1->Eval(r) ) {
      ofile << key << endl;
    }
    if( log(int1) > curve2->Eval(r) ) {
      ofile << key << endl;
    }
    if( log(int3) < curve3->Eval(r) ) {
      ofile << key << endl;
    }
    if( log(int3) > curve4->Eval(r) ) {
      ofile << key << endl;
    }
    R->Fill( r, log(int1) );
    L->Fill( r, log(int3) );
  }
  ifile.close();
  ofile.close();

  TCanvas *main = new TCanvas();
  main->Divide(2,1);
  main->cd(1);
  R->Draw("col");
  curve1->Draw("same");
  curve2->Draw("same");
  main->cd(2);
  L->Draw("col");
  curve3->Draw("same");
  curve4->Draw("same");

  return;
}
