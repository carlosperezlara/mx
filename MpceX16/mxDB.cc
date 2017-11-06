#include "mxDB.exped.cc"
#include "mxDB.exlan.cc"
#include "mxDB.exl2h.cc"
#include "mxDB.mpctau.cc"
#include "mxDB.exlanx.cc"
#include "mxDB.exrun.cc"
#include <fstream>

namespace mxDB {
  void read(int run, mxCalibMaster *cal) {
    std::cout << "Reading static table for " << run << std::endl;
    exped(run,cal);
    exhlf(run,cal);
    exlan(run,cal);
    //=====
    if(mxDB::loadextras) {
      exlsgm(run,cal);
      exla(run,cal);
      exlf2(run,cal);
      exlf3(run,cal);
      exlf4(run,cal);
      exlb(run,cal);
      exlsl(run,cal);
    }
    //=====
    exrun.cc(run,cal);
    //=====
    mpctau(run,cal);
    std::ifstream ifile("THEbadkeys.dat");
    for(;;) {
      int tmp;
      ifile >> tmp;
      if(!ifile.good()) break;
      cal->GetLSgm()->Set(tmp,-1);
    }
  }
  bool loadextras = false;
};
