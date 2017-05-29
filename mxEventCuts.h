#ifndef __mxEventCuts_HH__
#define __mxEventCuts_HH__

class PHCompositeNode;

class mxEventCuts {
 public:
  mxEventCuts();
  virtual ~mxEventCuts();
  UChar_t PassEventCuts(PHCompositeNode*);
};

#endif /* __mxEventCuts_H__ */ 
