#include <cstdlib>
#include <iostream>
#include <fstream>

#include "TCanvas.h"
#include "TH2F.h"

#include "mxGeometry.h"

int main(int argn, char **argc) {
  mxGeometry *geo = new mxGeometry();
  TH2F *aero[18];
  for(int i=0; i!=8; ++i) {
    if(i%2==0) {
      aero[i]   = new TH2F( Form("S%d",i), Form("S%d",i), 32*8, -18, +18, 32, -18, +18 );
      aero[9+i] = new TH2F( Form("N%d",i), Form("N%d",i), 32*8, -18, +18, 32, -18, +18 );
    } else {
      aero[i]   = new TH2F( Form("S%d",i), Form("S%d",i), 32, -18, +18, 32*8, -18, +18 );
      aero[9+i] = new TH2F( Form("N%d",i), Form("N%d",i), 32, -18, +18, 32*8, -18, +18 );
    }
  }
  int evini = 0;
  int evfin = 0;
  std::ifstream input("input.dat");
  int hits, idx;
  float sgn, xx, yy;
  for(int ev;; ++ev) {
    input >> hits;
    if(!input.good()) break;
    for(int i=0; i!=hits; ++i) {
      input >> idx >> sgn;// >> xx >> yy;
      if(ev<evini || ev>evfin) continue;
      //std::cout << geo->LyrIdx(idx) << std::endl;
      float x = geo->X(idx);
      float y = geo->Y(idx);
      //std::cout << " DIFF " << ev << " || " << idx << " || GEO " << x << " " << y << " || G4 " << xx << " " << yy << std::endl;
      aero[geo->LyrIdx(idx)]->Fill(x,y,sgn);
    }
  }
  input.close();
  std::cout << "AAA" << std::endl;

  TCanvas *main = new TCanvas();
  main->Divide(4,4);
  main->cd(1);  aero[0]->Draw("colz");
  main->cd(2);  aero[1]->Draw("colz");
  main->cd(3);  aero[2]->Draw("colz");
  main->cd(4);  aero[3]->Draw("colz");
  main->cd(5);  aero[4]->Draw("colz");
  main->cd(6);  aero[5]->Draw("colz");
  main->cd(7);  aero[6]->Draw("colz");
  main->cd(8);  aero[7]->Draw("colz");
  main->cd(9);  aero[9]->Draw("colz");
  main->cd(10); aero[10]->Draw("colz");
  main->cd(11); aero[11]->Draw("colz");
  main->cd(12); aero[12]->Draw("colz");
  main->cd(13); aero[13]->Draw("colz");
  main->cd(14); aero[14]->Draw("colz");
  main->cd(15); aero[15]->Draw("colz");
  main->cd(16); aero[16]->Draw("colz");
  main->SaveAs("aero.pdf","pdf");
  return 0;
}
