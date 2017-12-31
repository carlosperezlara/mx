#ifndef __qcCalibBase_HH__
#define __qcCalibBase_HH__
class qcCalibBase {
 public:
  qcCalibBase();
  qcCalibBase( const qcCalibBase &cpy );
  virtual ~qcCalibBase() {}
  float GetRe(int order);
  float GetIm(int order);
  void SetRe(int order, float val);
  void SetIm(int order, float val);
  void FillReImWith( float re[16], float im[16] );
 protected:
  float fRe[16];
  float fIm[16];
};
#endif /* __qcCalibBase_H__ */
