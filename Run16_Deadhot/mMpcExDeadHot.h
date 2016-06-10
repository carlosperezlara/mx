#ifndef __MMPCEXDEADHOT_HH__
#define __MMPCEXDEADHOT_HH__


#ifndef __CINT__
#include <SubsysReco.h>
#endif
 
#include "MpcExConstants.h"
#include <set>


class SubsysReco;
class PHCompositeNode;
class TH2D;
class TMpcExHitContainer;
class TMpcExCalibContainer;
class MpcExRawHit;
class MpcExEventHeader;
class Exogram;

class mMpcExDeadHot:public SubsysReco{
  public:
    mMpcExDeadHot(const char* name = "MMPCEXDEADHOT");
    virtual int Init(PHCompositeNode*);
    virtual int InitRun(PHCompositeNode*);
    virtual int process_event(PHCompositeNode*);
    virtual ~mMpcExDeadHot();
    virtual int End(PHCompositeNode*);
    
    int CellIDCheck(MpcExEventHeader *evt_head);
    int PARSTCheck(MpcExEventHeader *evt_head);
    int StatePhaseCheck(MpcExEventHeader *evt_head);
    void set_high_deadhot(const char* file){_high_deadhot = file;}
    void set_low_deadhot(const char* file){_low_deadhot = file;}

  private:
    void set_interface_ptrs(PHCompositeNode*);
    TMpcExHitContainer* _TMpcExHitContainer;
    TMpcExCalibContainer* _TMpcExCalibContainer;
    MpcExRawHit* _MpcExRawHit;
    MpcExEventHeader* _MpcExEventHeader;
    
    unsigned int _FailCellIDCheck[MpcExConstants::NARMS][MpcExConstants::NPACKETS_PER_ARM][MpcExConstants::NCHAINS_PER_PACKET][MpcExConstants::NCHIPS_PER_CHAIN];

    TH2D* hhigh_rawadc_key;
    TH2D* hlow_rawadc_key;
    TH2D* hhigh_adc_key;
    TH2D* hlow_adc_key;
    Exogram* hgrammyh[2];
    Exogram* hgrammyl[2];
    std::set<int> high_deadhot_list;
    std::set<int> low_deadhot_list;
    const char* _high_deadhot;
    const char* _low_deadhot;
  protected:
};
#endif /*__MMPCEXDEADHOT_HH__*/
