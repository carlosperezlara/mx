int resolution() {
  TFile *file = new TFile("se-455450.root");
  TH2F *ab = (TH2F*) file->Get("Psi2Rex_ARM0_PAIR0_DET0I_DET0O");
  TH2F *cd = (TH2F*) file->Get("Psi2Rex_ARM0_PAIR1_DET1I_DET1O");
  TH2F *ef = (TH2F*) file->Get("Psi2Rex_ARM0_PAIR2_DET2I_DET2O");
  TH2F *gh = (TH2F*) file->Get("Psi2Rex_ARM0_PAIR3_DET3I_DET3O");

  TH2F *ad = (TH2F*) file->Get("Psi2Rex_ARM0_PAIR4_DET0I_DET1O");
  TH2F *cf = (TH2F*) file->Get("Psi2Rex_ARM0_PAIR5_DET1I_DET2O");
  TH2F *eh = (TH2F*) file->Get("Psi2Rex_ARM0_PAIR6_DET2I_DET3O");

  TH2F *bc = (TH2F*) file->Get("Psi2Rex_ARM0_PAIR7_DET0O_DET1I");
  TH2F *de = (TH2F*) file->Get("Psi2Rex_ARM0_PAIR8_DET1O_DET2I");
  TH2F *fg = (TH2F*) file->Get("Psi2Rex_ARM0_PAIR9_DET2O_DET3I");

  TH2F *ac = (TH2F*) file->Get("Psi2Rex_ARM0_PAIR10_DET0I_DET1I");
  TH2F *ce = (TH2F*) file->Get("Psi2Rex_ARM0_PAIR11_DET1I_DET2I");
  TH2F *eg = (TH2F*) file->Get("Psi2Rex_ARM0_PAIR12_DET2I_DET3I");

  TH2F *bd = (TH2F*) file->Get("Psi2Rex_ARM0_PAIR13_DET0O_DET1O");
  TH2F *df = (TH2F*) file->Get("Psi2Rex_ARM0_PAIR14_DET1O_DET2O");
  TH2F *fh = (TH2F*) file->Get("Psi2Rex_ARM0_PAIR15_DET2O_DET3O");

  TH1F *resa = new TH1F("resa","resa",20,-0.5,99.5);
  TH1F *resb = new TH1F("resb","resb",20,-0.5,99.5);
  TH1F *resc = new TH1F("resc","resc",20,-0.5,99.5);
  TH1F *resd = new TH1F("resd","resd",20,-0.5,99.5);
  TH1F *rese = new TH1F("rese","rese",20,-0.5,99.5);
  TH1F *resf = new TH1F("resf","resf",20,-0.5,99.5);
  TH1F *resg = new TH1F("resg","resg",20,-0.5,99.5);
  TH1F *resh = new TH1F("resh","resh",20,-0.5,99.5);


  TH2F *rab = (TH2F*) file->Get("Psi2Reb_ARM0_BBC_DET0I");
  TH2F *rbb = (TH2F*) file->Get("Psi2Reb_ARM0_BBC_DET0O");
  TH2F *rcb = (TH2F*) file->Get("Psi2Reb_ARM0_BBC_DET1I");
  TH2F *rdb = (TH2F*) file->Get("Psi2Reb_ARM0_BBC_DET1O");
  TH2F *reb = (TH2F*) file->Get("Psi2Reb_ARM0_BBC_DET2I");
  TH2F *rfb = (TH2F*) file->Get("Psi2Reb_ARM0_BBC_DET2O");
  TH2F *rgb = (TH2F*) file->Get("Psi2Reb_ARM0_BBC_DET3I");
  TH2F *rhb = (TH2F*) file->Get("Psi2Reb_ARM0_BBC_DET3O");


  for(int b=0; b!=20; ++b) {
    float abx = ab->ProjectionY( Form("AB_CC%d",b), b+1, b+1)->GetMean();;
    float cdx = cd->ProjectionY( Form("CD_CC%d",b), b+1, b+1)->GetMean();;
    float efx = ef->ProjectionY( Form("EF_CC%d",b), b+1, b+1)->GetMean();;
    float ghx = gh->ProjectionY( Form("GH_CC%d",b), b+1, b+1)->GetMean();;
    float adx = ad->ProjectionY( Form("AD_CC%d",b), b+1, b+1)->GetMean();;
    float cfx = cf->ProjectionY( Form("CF_CC%d",b), b+1, b+1)->GetMean();;
    float ehx = eh->ProjectionY( Form("EH_CC%d",b), b+1, b+1)->GetMean();;
    float bcx = bc->ProjectionY( Form("BC_CC%d",b), b+1, b+1)->GetMean();;
    float dex = de->ProjectionY( Form("DE_CC%d",b), b+1, b+1)->GetMean();;
    float fgx = fg->ProjectionY( Form("FG_CC%d",b), b+1, b+1)->GetMean();;
    float acx = ac->ProjectionY( Form("AC_CC%d",b), b+1, b+1)->GetMean();;
    float cex = ce->ProjectionY( Form("CE_CC%d",b), b+1, b+1)->GetMean();;
    float egx = eg->ProjectionY( Form("EG_CC%d",b), b+1, b+1)->GetMean();;
    float bdx = bd->ProjectionY( Form("BD_CC%d",b), b+1, b+1)->GetMean();;
    float dfx = df->ProjectionY( Form("DF_CC%d",b), b+1, b+1)->GetMean();;
    float fhx = fh->ProjectionY( Form("FH_CC%d",b), b+1, b+1)->GetMean();;

    float ax = rab->ProjectionY( Form("A_CC%d",b), b+1, b+1)->GetMean();;
    float bx = rbb->ProjectionY( Form("B_CC%d",b), b+1, b+1)->GetMean();;
    float cx = rcb->ProjectionY( Form("C_CC%d",b), b+1, b+1)->GetMean();;
    float dx = rdb->ProjectionY( Form("D_CC%d",b), b+1, b+1)->GetMean();;
    float ex = reb->ProjectionY( Form("E_CC%d",b), b+1, b+1)->GetMean();;
    float fx = rfb->ProjectionY( Form("F_CC%d",b), b+1, b+1)->GetMean();;
    float gx = rgb->ProjectionY( Form("G_CC%d",b), b+1, b+1)->GetMean();;
    float hx = rhb->ProjectionY( Form("H_CC%d",b), b+1, b+1)->GetMean();;

    float resax = TMath::Sqrt(abx*adx/bdx);
    float resbx = TMath::Sqrt(abx*bcx/acx);
    float rescx = TMath::Sqrt(cdx*cfx/dfx);
    float resdx = TMath::Sqrt(cdx*dex/cex);
    float resex = TMath::Sqrt(efx*ehx/fhx);
    float resfx = TMath::Sqrt(efx*fgx/egx);
    float resgx = TMath::Sqrt(ghx*fgx/fhx);
    float reshx = TMath::Sqrt(ghx*ehx/egx);
    if(1) { // overwrite with BBC based resolution
      cout << abx << " " << ax << " " << bx << endl;
      resax = TMath::Sqrt(abx*ax/bx);
      resbx = TMath::Sqrt(abx*bx/ax);
      rescx = TMath::Sqrt(cdx*cx/dx);
      resdx = TMath::Sqrt(cdx*dx/cx);
      resex = TMath::Sqrt(efx*ex/fx);
      resfx = TMath::Sqrt(efx*fx/ex);
      resgx = TMath::Sqrt(ghx*gx/hx);
      reshx = TMath::Sqrt(ghx*hx/gx);
    }
    if(TMath::IsNaN(resax)) continue;
    if(TMath::IsNaN(resbx)) continue;
    if(TMath::IsNaN(rescx)) continue;
    if(TMath::IsNaN(resdx)) continue;
    if(TMath::IsNaN(resex)) continue;
    if(TMath::IsNaN(resfx)) continue;
    if(TMath::IsNaN(resgx)) continue;
    if(TMath::IsNaN(reshx)) continue;
    //cout << "resa " << b+1 << " ==> " << resax << endl;
    resa->SetBinContent( b+1, resax  );
    resb->SetBinContent( b+1, resbx  );
    resc->SetBinContent( b+1, rescx  );
    resd->SetBinContent( b+1, resdx  );
    rese->SetBinContent( b+1, resex  );
    resf->SetBinContent( b+1, resfx  );
    resg->SetBinContent( b+1, resgx  );
    resh->SetBinContent( b+1, reshx  );
  }
  resa->Draw();
  resb->Draw("same");
  resc->Draw("same");
  resd->Draw("same");
  rese->Draw("same");
  resf->Draw("same");
  resg->Draw("same");
  resh->Draw("same");
  return 0;
}
