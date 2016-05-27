void Run_MXRECO(const char *out="output.root") {
  gSystem->Load("libMX.so");
  Fun4AllServer *se = Fun4AllServer::instance(); //this lib was loaded upstream

  //recoConsts* rc = recoConsts::instance();
  //Fun4AllServer* se = Fun4AllServer::instance();
  //rc->set_IntFlag("MPC_RECO_MODE",0x16); // 10 (skip raw2) + 2 (tower container) + 4 (cluster container)
  //SubsysReco *mpcreco = new MpcReco("MPCRECO");
  //se->registerSubsystem(mpcreco);

  mxSubsysReco *mxReco = new mxsubsysReco("mpcex");
  mxReco->QA(1);
  se->registerSubsystem(mxReco);
}

void InputData(vector<string> &indata) {
  indata.push_back("dst_eve");
  indata.push_back("dst_mpcex");
  indata.push_back("dst_mpc");
}
