void find_MIP_peaks(char * c_infile = "output_from_carlos/output2/454664.root")
{
  TFile * tf_in = new TFile(c_infile);

  TH1F * h_mxSubsysReco_EVENTS    = (TH1F*) tf_in -> Get("mxSubsysReco_EVENTS");
  TH2F * h2_mxSubsysReco_adchi    = (TH2F*) tf_in -> Get("mxSubsysReco_adchi");
  TH2F * h2_mxSubsysReco_adchipty = (TH2F*) tf_in -> Get("mxSubsysReco_adchipty");
  TH2F * h2_mxSubsysReco_adclo    = (TH2F*) tf_in -> Get("mxSubsysReco_adclo");

  //Get the Run Number, assuming the file name is just the runnumber .root
  int run = 0;
  int i_run_number = 0;

  for(int ijk = 6; ijk < 12; ijk ++)
    {
      int start_char = strlen(c_infile);
      int dv = c_infile[start_char-ijk] - '0';
      i_run_number += pow(10,ijk-6)*dv;
    }//for(int ijk = 0; ijk < 11; ijk ++)

  cout<<" MPC EX MIP Calibration RUN: "<<i_run_number<<endl;

  //read in the average mip peak for QA puproses (this requires that the code has been run at least once)
  double d_prev_mip_mpv   = 7.11761;
  double d_prev_mip_sigma = 2.02409;

  ifstream ifs_avg_mip(Form("%d_avg_mip.txt",i_run_number));
  if(ifs_avg_mip.good() )
    {
      while(1)
	{
	  ifs_avg_mip >> d_prev_mip_mpv >> d_prev_mip_sigma;
	  if(ifs_avg_mip.eof() ) break;
	}//while(1)
    }//if(ifs_avg_mip.good() )


  // new TCanvas();
  // h_mxSubsysReco_EVENTS -> Draw();

  // new TCanvas();
  // h2_mxSubsysReco_adchi -> Draw("COLz");

  // new TCanvas();
  // h2_mxSubsysReco_adchipty  -> Draw("COLz");

  // new TCanvas();
  // h2_mxSubsysReco_adclo  -> Draw("COLz");

  //  new TCanvas();

  TH1D * h_mip_mpv   = new TH1D("h_mip_mpv",Form("%d chipty MPV of Landau Fit vs Key",i_run_number),49152,0,49152);
  TH1D * h_mip_sigma = new TH1D("h_mip_sigma",Form("%d chipty SIGMA of Landau Fit vs Key",i_run_number),49152,0,49152);

  int N_num_keys = 1000; //49152;
  for(int ijk = 1; ijk < (N_num_keys+1); ijk++)
    //    for(int ijk = 1; ijk < (4+1); ijk++)
    {
      if((ijk%1000) == 0) cout<<".";
      TH1D * h_mxSubsysReco_adchipty = (TH1D*) h2_mxSubsysReco_adchipty -> ProjectionY(Form("h_mxSubsysReco_adchipty_%d",ijk),ijk,ijk);
      // if(ijk == 1) h_mxSubsysReco_adchipty -> Draw();
      // else h_mxSubsysReco_adchipty -> Draw("same");
      //      new TCanvas(); gPad -> SetLogy(); h_mxSubsysReco_adchipty -> Draw();

      TF1 * tf_landau = new TF1("tf_landau","landau",0.0,250.0);
      h_mxSubsysReco_adchipty -> Fit(tf_landau,"RN","",5.0,15.0);

      double d_mpv       = tf_landau -> GetParameter(1);
      double d_mpv_err   = tf_landau -> GetParError(1);
      cout<<d_mpv<<" "<<d_mpv_err<< endl;

      double d_sigma     = tf_landau -> GetParameter(2);
      double d_sigma_err = tf_landau -> GetParError(2);
      cout<<d_sigma<<" "<<d_sigma_err<< endl;

      double i_Bin = h_mip_mpv -> FindFixBin(ijk+0.5);
      h_mip_mpv    -> SetBinContent(i_Bin,d_mpv);
      h_mip_mpv    -> SetBinError(i_Bin,d_mpv_err);

      h_mip_sigma  -> SetBinContent(i_Bin,d_sigma);
      h_mip_sigma  -> SetBinError(i_Bin,d_sigma_err);

      // check the ADC histogram if the MPV is 

    }//for(int ijk = 1; ijk < (49152+1) ijk++)

    new TCanvas();
    // for qa set the range to the scan range
    TAxis * tax_mip_mpv = (TAxis*) h_mip_mpv -> GetXaxis();
    if(N_num_keys != 49152) tax_mip_mpv -> SetRange(0,N_num_keys);

    
    h_mip_mpv   -> SetMarkerStyle(kStar);
    h_mip_sigma   -> SetMarkerStyle(kStar);
    
    h_mip_sigma -> SetLineColor(kRed);
    h_mip_sigma -> SetMarkerColor(kRed);

    h_mip_mpv -> SetMinimum(-10);
    h_mip_mpv -> SetMaximum(20);

    h_mip_mpv   -> Draw();
    h_mip_sigma -> Draw("same");

    //Get the Average of each
    TF1 * tf1_mpv_vs_key_const = new TF1("tf1_mpv_vs_key_const","[0]",0,49152);
    h_mip_mpv -> Fit(tf1_mpv_vs_key_const,"RL","E",0,N_num_keys);

    h_mip_mpv -> SetTitle(Form("%s, AVG: %f",
			       h_mip_mpv -> GetTitle(),
			       tf1_mpv_vs_key_const -> GetParameter(0) )
			  );

    TF1 * tf1_sigma_vs_key_const = new TF1("tf1_sigma_vs_key_const","[0]",0,49152);
    tf1_sigma_vs_key_const -> SetLineColor(kBlack);
    h_mip_sigma -> Fit(tf1_sigma_vs_key_const,"RL","E",0,N_num_keys);

    h_mip_sigma -> SetTitle(Form("%s, AVG: %f",
				 h_mip_sigma -> GetTitle(),
				 tf1_sigma_vs_key_const -> GetParameter(0) )
			    );

    gPad -> BuildLegend(0.65,0.67,0.999,0.935);

    //Print Averages
    ofstream ofs_avg_mip;
    ofs_avg_mip.open(Form("%d_avg_mip.txt",i_run_number) );
    ofs_avg_mip << tf1_mpv_vs_key_const   -> GetParameter(0)
		<< " "
		<< tf1_sigma_vs_key_const -> GetParameter(0)
		<< endl;
    
    
  
}//void find_MIP_peaks()
