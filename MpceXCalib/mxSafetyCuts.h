#ifndef __mxSafetyCuts_HH__
#define __mxSafetyCuts_HH__

class PHCompositeNode;

class mxSafetyCuts {
 public:
  mxSafetyCuts();
  virtual ~mxSafetyCuts();
  UChar_t PassSafetyCuts(PHCompositeNode*);
};

#endif /* __mxSafetyCuts_H__ */ 
