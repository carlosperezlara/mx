int fitresultsen(int sen=0) {
  TString dir = Form("out/SEN%03d/fit",sen);
  float p[110];
  float in[110];
  float a[110];
  float ea[110];
  float b[110];
  float eb[110];
  float sl[110];
  float esl[110];
  float f2[110];
  float ef2[110];
  float f3[110];
  float ef3[110];
  float f4[110];
  float ef4[110];
  float mip[110];
  float emip[110];
  float sgm[110];
  float esgm[110];
  float tmp;

  float x[110];
  float ex[110];
  for(int i=0; i!=110; ++i) {
    x[i] = ex[i] = 0;
    p[i] = in[i] = 0;
    a[i] = ea[i] = b[i] = eb[i] = sl[i] = esl[i] = 0;
    f2[i] = ef2[i] = f3[i] = ef3[i] = f4[i] = 0;
    ef4[i] = mip[i] = emip[i] = sgm[i] = esgm[i] = 0;
  }


  int run;
  ifstream ifrun("runs.dat.final");
  for(int i=0; i!=110; ++i) {
    ifrun >> run;
    if(!ifrun.good()) break;
    //x[i] = i+1;
    x[i] = run;
    ex[i] = 0;
    ifstream fin;
    fin.open( Form("%s/H%05d.dat",dir.Data(),run) );
    fin >> tmp;
    if(fin.good()) {
      a[i] = tmp;
      fin >> ea[i] >> mip[i] >> emip[i] >> sgm[i] >> esgm[i];
      fin >> f2[i] >> ef2[i] >> f3[i] >> ef3[i] >> f4[i] >> ef4[i];
      fin >> b[i] >> eb[i] >> sl[i] >> esl[i];
      //if(emip[i]<0.001) emip[i] = 0.001;
    }
    fin.close();
    //
    fin.open( Form("%s/P%05d.dat",dir.Data(),run) );
    fin >> tmp;
    if(fin.good()) {
      p[i] = tmp;
    }
    fin.close();
    //
    fin.open( Form("%s/I%03d.dat",dir.Data(),sen) );
    fin >> tmp;
    if(fin.good()) {
      in[i] = tmp;
    }
    fin.close();
  }
  ifrun.close();

  TGraphErrors *grP;
  TGraphErrors *grIN;
  TGraphErrors *grA;
  TGraphErrors *grB;
  TGraphErrors *grF2;
  TGraphErrors *grF3;
  TGraphErrors *grF4;
  TGraphErrors *grSL;
  TGraphErrors *grMIP;
  TGraphErrors *grSGM;
  int color = kOrange-3;
  grP = new TGraphErrors(91,x,p,ex,ex);
  grIN = new TGraphErrors(91,x,in,ex,ex);
  grA = new TGraphErrors(91,x,a,ex,ea);
  grB = new TGraphErrors(91,x,b,ex,eb);
  grF2 = new TGraphErrors(91,x,f2,ex,ef2);
  grF3 = new TGraphErrors(91,x,f3,ex,ef3);
  grF4 = new TGraphErrors(91,x,f4,ex,ef4);
  grSL = new TGraphErrors(91,x,sl,ex,esl);
  grMIP = new TGraphErrors(91,x,mip,ex,emip);
  grSGM = new TGraphErrors(91,x,sgm,ex,esgm);

  grP->SetLineColor( color );
  grP->SetMarkerColor( color );
  grIN->SetLineColor( color );
  grIN->SetMarkerColor( color );
  grA->SetLineColor( color );
  grA->SetMarkerColor( color );
  grB->SetLineColor( color );
  grB->SetMarkerColor( color );
  grF2->SetLineColor( color );
  grF2->SetMarkerColor( color );
  grF3->SetLineColor( color );
  grF3->SetMarkerColor( color );
  grF4->SetLineColor( color );
  grF4->SetMarkerColor( color );
  grSL->SetLineColor( color );
  grSL->SetMarkerColor( color );
  grMIP->SetLineColor( color );
  grMIP->SetMarkerColor( color );
  grSGM->SetLineColor( color );
  grSGM->SetMarkerColor( color );

  TCanvas *main = new TCanvas( Form("SEN%03d",sen),
			       Form("SEN%03d",sen),800,600);
  main->Divide(3,2);

  TLine *line = new TLine;
  line->SetLineColor(kBlack);

  main->cd(1)->SetGridx(1);
  grA->Draw("A*L");
  grA->SetTitle("Sgn");
  grA->GetYaxis()->SetRangeUser(0,1e7);
  grA->GetXaxis()->SetNdivisions(515);

  main->cd(3)->SetGridx(1);
  grF2->Draw("A*L");
  grF2->SetTitle("f2, f3, f4");
  grF2->GetYaxis()->SetRangeUser(0.0,0.40);
  grF2->GetXaxis()->SetNdivisions(515);
  line->DrawLine(0,0.22,50000,0.22);
  grF3->Draw("*LSAME");
  grF3->SetTitle("f3");
  grF3->GetYaxis()->SetRangeUser(0.0,0.15);
  grF3->GetXaxis()->SetNdivisions(515);
  line->DrawLine(0,0.07,50000,0.07);
  grF4->Draw("*LSAME");
  grF4->GetYaxis()->SetRangeUser(0.0,0.10);
  grF4->GetXaxis()->SetNdivisions(515);
  line->DrawLine(0,0.04,50000,0.04);

  main->cd(2)->SetGridx(1);
  grB->Draw("A*L");
  grB->SetTitle("Bgr");
  grB->GetYaxis()->SetRangeUser(0,1e8);
  grB->GetXaxis()->SetNdivisions(515);

  main->cd(4)->SetGridx(1);
  grSL->Draw("A*L");
  grSL->SetTitle("Slope");
  grSL->GetYaxis()->SetRangeUser(-40.0,-20.);
  grSL->GetXaxis()->SetNdivisions(515);
  line->DrawLine(0,-0.4,50000,-0.4);

  main->cd(5)->SetGridx(1);
  main->cd(5)->SetGridy(1);
  grMIP->SetTitle("MPV");
  grMIP->GetYaxis()->SetRangeUser(0.135,0.175);
  grMIP->GetXaxis()->SetNdivisions(515);
  const int fbins=18;
  int rrange[fbins][3];
  rrange[0][0] = 454774; rrange[0][1] = 454789; rrange[0][2] = 19900;
  rrange[1][0] = 454794; rrange[1][1] = 454810; rrange[1][2] = 19901;
  rrange[2][0] = 454933; rrange[2][1] = 454937; rrange[2][2] = 19904;
  rrange[3][0] = 454943; rrange[3][1] = 454948; rrange[3][2] = 19907;
  rrange[4][0] = 455049; rrange[4][1] = 455053; rrange[4][2] = 19914;
  rrange[5][0] = 455056; rrange[5][1] = 455066; rrange[5][2] = 19915;
  rrange[6][0] = 455071; rrange[6][1] = 455083; rrange[6][2] = 19917;
  rrange[7][0] = 455200; rrange[7][1] = 455203; rrange[7][2] = 19920;
  rrange[8][0] = 455206; rrange[8][1] = 455213; rrange[8][2] = 19921;
  rrange[9][0] = 455218; rrange[9][1] = 455224; rrange[9][2] = 19924;
  rrange[10][0] = 455302; rrange[10][1] = 455306; rrange[10][2] = 19925;
  rrange[11][0] = 455342; rrange[11][1] = 455355; rrange[11][2] = 19928;
  rrange[12][0] = 455362; rrange[12][1] = 455367; rrange[12][2] = 19930;
  rrange[13][0] = 455446; rrange[13][1] = 455452; rrange[13][2] = 19932;
  rrange[14][0] = 455476; rrange[14][1] = 455478; rrange[14][2] = 19935;
  rrange[15][0] = 455545; rrange[15][1] = 455552; rrange[15][2] = 19937;
  rrange[16][0] = 455585; rrange[16][1] = 455605; rrange[16][2] = 19940;
  rrange[17][0] = 455637; rrange[17][1] = 455639; rrange[17][2] = 19941;
  
  //ofstream foutout( Form("outtosen%03d",sen) );
  TF1 *fitmpv[fbins];
  for(int i=0; i!=fbins; ++i) {
    fitmpv[i] = new TF1( Form("fitmpv%d",i),
			 Form("[0]+[1]*(x-%d)",rrange[i][0]),
			 rrange[i][0],
			 rrange[i][1]);
    fitmpv[i]->SetLineWidth(1);
    fitmpv[i]->SetParameter(1,0);
    fitmpv[i]->SetParLimits(1,-1e-2,0);
    grMIP->Fit(fitmpv[i],"R","", rrange[i][0], rrange[i][1] );
    //foutout << Form("%.1f",fitmpv[i]->GetParameter(1)*1e4);
    //foutout << " ";
    //foutout << Form("%.1f",fitmpv[i]->GetParameter(0)*1e3) << endl;
  }
  //foutout.close();
  grMIP->Draw("A*L");
  for(int i=0; i!=fbins; ++i) fitmpv[i]->Draw("same");


  main->cd(6)->SetGridx(1);
  grSGM->Draw("A*L");
  grSGM->SetTitle("SGM");
  grSGM->GetYaxis()->SetRangeUser(0.015, 0.040);
  grSGM->GetXaxis()->SetNdivisions(515);
  line->DrawLine(0,3.0,50000,3.0);

  return 0;
}
