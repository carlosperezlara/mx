#include<string>

void Fun4All_MpcExDataAna(char* input_file_mpcex ="/gpfs/mnt/gpfs02/phenix/mpcex/online_production/run16_mpcex_mpcex/run_0000454000_0000455000/DST_MPCEX/DST_MPCEX_run16_mpcex_mpcex-0000454947-0001.root",char* input_file_mpc="",char* input_file_eve){
  gSystem->Load("libfun4all");
  gSystem->Load("libuspin.so");
  gSystem->Load("libMyMpcEx.so");
  gSystem->Load("libfun4allfuncs.so");
  gSystem->Load("libbbc");
  gSystem->Load("libt0");
  gSystem->Load("libmpc.so");
  gSystem->Setenv("ODBCINI","/opt/phenix/etc/odbc.ini.mirror");
  
  recoConsts* rc = recoConsts::instance();
  Fun4AllServer* se = Fun4AllServer::instance();
  
  char ifile[5000];
  strcpy(ifile, input_file_mpcex);
  strtok(ifile, "-");
  int runnumber = atoi(strtok(0, "-"));
  int segment = atoi(strtok(strtok(0, "-"), "."));


  SubsysReco* createNodeTree = new mMpcExCreateNodeTree();
  se->registerSubsystem(createNodeTree);

  SubsysReco* mpcexLoadCal = new mMpcExLoadCalibrations();
  se->registerSubsystem(mpcexLoadCal);

  mMpcExDeadHot* deadhot = new mMpcExDeadHot();
  deadhot->set_high_deadhot("high_deadhot.txt");
  deadhot->set_low_deadhot("low_deadhot.txt");
  se->registerSubsystem(deadhot);

  Fun4AllInputManager* mpcex_dst_mpcex = new Fun4AllDstInputManager("DST_MPCEX","DST","TOP");
  se->registerInputManager(mpcex_dst_mpcex);

  rc->set_IntFlag("RUNNUMBER",runnumber);
  cout << "run number "<<runnumber<<endl;
 
  se->fileopen(mpcex_dst_mpcex->Name(),input_file_mpcex);
  cout <<"open "<<input_file_mpcex<<endl;

  se->run(0);

  se->End();
  char output[100];
  sprintf(output,"Run16Ana_MinBias_NoCMN_Sub-%d-%d.root",runnumber,segment);
  Fun4AllHistoManager* hm = se->getHistoManager("DeadHot");
  if(hm) hm->dumpHistos(output);

  delete se;

  cout << "Completed reconstruction." <<endl;
}

