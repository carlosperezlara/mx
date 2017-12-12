#ifndef __PHQCDATA_H__
#define __PHQCDATA_H__

#include <vector>
#include "PHObject.h"
#include "qcData.h"

class PHQCData:public PHObject {
 public:
  PHQCData() {}
  virtual ~PHQCData() {fData.Reset();}
  qcData* GetData() {return &fData;}
 private:
  qcData fData;
};
#endif /* __PHQCDATA_H__ */
