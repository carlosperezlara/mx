namespace mxDB {
  void exrun(int run, mxCalibMaster *cal) {
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
    cal->GetSMPV()->FillWithConst(0.147);
    for(int i=0; i!=fbins; ++i) {
      if(run<rrange[i][0]) continue;
      if(run>rrange[i][1]) continue;
      int dx = run - rrange[i][0];
      float mpvrun_m[384], mpvrun_b[384];
      ifstream frun( Form("mxD.exrun.fill%d.dat",rrange[i][2]) );
      for(int s=0;s!=384;++s) {
	frun >> mpvrun_m[s] >> mpvrun_b[s];
	if(!frun.good()) break;
      }
      frun.close();
      for(int s=0; s!=384; ++s) {
	float fitted = mpvrun_m[s]*dx*1e-4 + mpvrun_b[s]*1e-3;
	cal->GetSMPV()->Set(s,fitted);
      }
      break;
    }

  }

}
