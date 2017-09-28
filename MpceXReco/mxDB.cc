#include "mxDB.exped.cc"
#include "mxDB.exlan.cc"
#include "mxDB.exl2h.cc"
#include "mxDB.mpctau.cc"

namespace mxDB {
  void read(int run, mxCalibMaster *cal) {
    exped(run,cal);
    exhlf(run,cal);
    exlan(run,cal);
    mpctau(run,cal);
  }
};
