#include "qcDB.second.cc"
#include <fstream>

namespace qcDB {
  void read(int run, qcCalibMaster *cal) {
    std::cout << "Reading static table for " << run << std::endl;
    qcsecond(run,cal);
  }
};
