#include "Run_MXRECO.C"

void RunMyMacro(int qalevel=1,
		int nEvents=1000,
		int run=454947,
		int seg=0) {
  TString runseg = Form("%010d-%04d",run,seg);
  TString p2p = "/gpfs/mnt/gpfs02/phenix/mpcex/online_production/run16_mpcex_mpcex/run_0000454000_0000455000/";
  TString if0 = Form("%s/DST_EVE/DST_EVE_run16_mpcex_mpcex-%s.root",p2p.Data(),runseg.Data());
  TString if1 = Form("%s/DST_MPCEX/DST_MPCEX_run16_mpcex_mpcex-%s.root",p2p.Data(),runseg.Data());
  TString if2 = Form("%s/DST_MPC/DST_MPC_run16_mpcex_mpcex-%s.root",p2p.Data(),runseg.Data());

  gSystem->Setenv("ODBCINI","/opt/phenix/etc/odbc.ini.mirror");
  gSystem->Load("libfun4all.so");
  gSystem->Load("libfun4allfuncs.so");
  gSystem->Load("libmpcex_base.so");
  gSystem->Load("libmpcex_interface.so");
  gSystem->Load("libmpcex_modules.so");

  Run_MXRECO( qalevel, Form("out_%s",runseg) );

  Fun4AllServer *se = Fun4AllServer::instance(); 
  Fun4AllInputManager *in0 = new Fun4AllDstInputManager( "MYEVE", "DST", "TOP");
  Fun4AllInputManager *in1 = new Fun4AllDstInputManager( "MYMPCEX", "DST", "TOP");
  Fun4AllInputManager *in2 = new Fun4AllDstInputManager( "MYMPC", "DST", "TOP");
  se->registerInputManager(in0);
  se->registerInputManager(in1);
  se->registerInputManager(in2);
  se->fileopen( in0->Name(), if0.Data() );
  se->fileopen( in1->Name(), if1.Data() );
  se->fileopen( in2->Name(), if2.Data() );
  se->run(nEvents);
  se->End();
  se->dumpHistos( Form("output_%d_%s.root",qalevel,runseg.Data()) );
  cout << "[ ALL DONE ]" << endl;
  gSystem->Exit(0);

}
