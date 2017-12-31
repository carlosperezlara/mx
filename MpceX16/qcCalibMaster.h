#ifndef __qcCalibMaster_HH__
#define __qcCalibMaster_HH__
class qcCalibMaster {
 public:
  qcCalibMaster();
  qcCalibMaster( const qcCalibMaster &cpy );
  virtual ~qcCalibMaster() {}
  qcCalibBase* GetEXSecond(int det, int cir);
 protected:
  qcCalibBase *fSecond[4][2];
};
#endif /* __qcCalibMaster_H__ */
