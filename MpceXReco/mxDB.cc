#include "mxDB.exped.cc"
#include "mxDB.exlan.cc"
#include "mxDB.exl2h.cc"
#include "mxDB.mpctau.cc"

namespace mxDB {
  void read(int run, mxCalibMaster *cal) {
    exped(run,cal);
    exhlf(run,cal);
    //=====
    for(int i=0; i!=49152; ++i) {
      cal->GetLMPV()->Set(i,1);
      cal->GetLSgm()->Set(i,1);
    }
    //exlan(run,cal);
    mpctau(run,cal);
    ifstream ifile("THEbadkeys.dat");
    for(;;) {
      int tmp;
      ifile >> tmp;
      if(!ifile.good()) break;
      cal->GetLSgm()->Set(tmp,-1);
    }
  }
};
