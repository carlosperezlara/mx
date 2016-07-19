#ifndef __mxQAReconstruction_HH__
#define __mxQAReconstruction_HH__

#include <vector>

class mxReconstruction;

class mxQAReconstruction {
 public:
  mxQAReconstruction();
  ~mxQAReconstruction();

  void Make(mxReconstruction*);

 private:
  void FillEvents(mxReconstruction*);
  void FillHits(mxReconstruction*);
  void FillParties(mxReconstruction*);
  void FillCoalitions(mxReconstruction*);
  void FillUnions(mxReconstruction*);
};

#endif /* __mxQAReconstruction_H__ */ 
