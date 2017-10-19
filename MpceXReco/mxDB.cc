#include "mxDB.exped.cc"
#include "mxDB.exlan.cc"
#include "mxDB.exl2h.cc"
#include "mxDB.mpctau.cc"

#include <fstream>

namespace mxDB {
  void read(int run, mxCalibMaster *cal) {
    std::cout << "Reading static table for " << run << std::endl;
    exped(run,cal);
    cal->GetPHSh()->FillWithConst(0);
    cal->GetPLSh()->FillWithConst(0);
    exhlf(run,cal);
    //=====
    cal->GetLMPV()->FillWithConst(1);
    cal->GetLSgm()->FillWithConst(1);
    //exlan(run,cal);
    mpctau(run,cal);
    std::ifstream ifile("THEbadkeys.dat");
    for(;;) {
      int tmp;
      ifile >> tmp;
      if(!ifile.good()) break;
      cal->GetLSgm()->Set(tmp,-1);
    }
  }
};
