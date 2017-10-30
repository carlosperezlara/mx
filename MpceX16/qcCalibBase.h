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
  void FillReImWith( float re[10], float im[10] );
 protected:
  float fRe[10];
  float fIm[10];
};
#endif /* __qcCalibBase_H__ */
