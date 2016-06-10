#include "mMpcExDeadHot.h"
#include "PHCompositeNode.h"
#include "TH2D.h"
#include "TMpcExHitContainer.h"
#include "TMpcExCalibContainer.h"
#include "MpcExRawHit.h"
#include "MpcExEventHeader.h"
#include "TMpcExHitSet.h"
#include "TMpcExHit.h"
#include "TMpcExCalib.h"
#include "getClass.h"
#include "Fun4AllReturnCodes.h"
#include "Fun4AllServer.h"
#include "Fun4AllHistoManager.h"
#include "stdlib.h"
#include "TriggerHelper.h"
#include "Exogram.h"
#include <fstream>

using namespace std;
using namespace findNode;
 
mMpcExDeadHot::mMpcExDeadHot(const char* name) :
  SubsysReco(name)
{
  _TMpcExHitContainer = NULL;
  _TMpcExCalibContainer = NULL;
  _MpcExRawHit = NULL;
  _MpcExEventHeader = NULL;
  hhigh_rawadc_key = NULL;
  hlow_rawadc_key = NULL;
  hhigh_adc_key = NULL;
  hlow_adc_key = NULL;
  hgrammyh[0] = NULL;
  hgrammyl[0] = NULL;
  hgrammyh[1] = NULL;
  hgrammyl[1] = NULL;
  _high_deadhot = NULL;
  _low_deadhot = NULL;
}
int mMpcExDeadHot::End(PHCompositeNode* topNode){
  return EVENT_OK;
}
 
mMpcExDeadHot::~mMpcExDeadHot(){
}
 
int mMpcExDeadHot::Init(PHCompositeNode* topNode){
  Fun4AllServer* se = Fun4AllServer::instance();
  Fun4AllHistoManager* hm = se->getHistoManager("DeadHot");
  if(!hm){
    hm = new Fun4AllHistoManager("DeadHot");
    se->registerHistoManager(hm);
  }
 
  hhigh_rawadc_key = new TH2D("hhigh_rawadc_key","High Raw ADC vs Key",49152,-0.5,49152-0.5,300,-40-0.5,260-0.5);
  hhigh_rawadc_key->GetXaxis()->SetTitle("Key");
  hhigh_rawadc_key->GetYaxis()->SetTitle("Raw ADC");
  hm->registerHisto(hhigh_rawadc_key);

  hlow_rawadc_key = new TH2D("hlow_rawadc_key","Low Raw ADC vs Key",49152,-0.5,49152-0.5,300,-40-0.5,260-0.5);
  hhigh_rawadc_key->GetXaxis()->SetTitle("Key");
  hhigh_rawadc_key->GetYaxis()->SetTitle("Raw ADC");
  hm->registerHisto(hlow_rawadc_key);

  hhigh_adc_key = new TH2D("hhigh_adc_key","High ADC(Pedestal Subtracted) vs Key",49152,-0.5,49152-0.5,300,-40-0.5,260-0.5);
  hhigh_adc_key->GetXaxis()->SetTitle("Key");
  hhigh_adc_key->GetYaxis()->SetTitle("ADC");
  hm->registerHisto(hhigh_adc_key);

  hlow_adc_key = new TH2D("hlow_adc_key","Low ADC(Pedestal Subtracted) vs Key",49152,-0.5,49152-0.5,300,-40-0.5,260-0.5);
  hlow_adc_key->GetXaxis()->SetTitle("Key");
  hlow_adc_key->GetYaxis()->SetTitle("ADC");
  hm->registerHisto(hlow_adc_key);

  hgrammyh[0] = new Exogram("hgrammyh0","Exogram high arm 0",900,-24,24,900,-24,24,8,-0.5,7.5);
  hm->registerHisto(hgrammyh[0]);

  hgrammyh[1] = new Exogram("hgrammyh1","Exogram high arm 1",900,-24,24,900,-24,24,8,-0.5,7.5);
  hm->registerHisto(hgrammyh[1]);

  hgrammyl[0] = new Exogram("hgrammyl0","Exogram low arm 0",900,-24,24,900,-24,24,8,-0.5,7.5);
  hm->registerHisto(hgrammyl[0]);

  hgrammyl[1] = new Exogram("hgrammyl1","Exogram low arm 1",900,-24,24,900,-24,24,8,-0.5,7.5);
  hm->registerHisto(hgrammyl[1]);

  ifstream bad_high_itg_total(_high_deadhot);
  ifstream bad_low_itg_total(_low_deadhot);

  if(bad_high_itg_total.is_open()){
    int key;
    int new_key;
    while(bad_high_itg_total>>key>>new_key){
       high_deadhot_list.insert(key);  
    }
  }

  if(bad_low_itg_total.is_open()){
    int key;
    int new_key;
    while(bad_low_itg_total>>key>>new_key){
       low_deadhot_list.insert(key);  
    }

  }

  return EVENT_OK;
}
 
int mMpcExDeadHot::InitRun(PHCompositeNode* topNode){
  set_interface_ptrs(topNode);
  return EVENT_OK;
}
void mMpcExDeadHot::set_interface_ptrs(PHCompositeNode* topNode){
  _TMpcExHitContainer = getClass<TMpcExHitContainer>(topNode,"TMpcExHitContainer");
  if(!_TMpcExHitContainer){
    cout <<"No TMpcExHitContainer !!!"<<endl;
    exit(1);
  }
  _TMpcExCalibContainer = getClass<TMpcExCalibContainer>(topNode,"TMpcExCalibContainer");
  if(!_TMpcExCalibContainer){
    cout <<"No TMpcExCalibContainer !!!"<<endl;
    exit(1);
  }
  _MpcExRawHit = getClass<MpcExRawHit>(topNode,"MpcExRawHit");
  if(!_MpcExRawHit){
    cout <<"No MpcExRawHit !!!"<<endl;
    exit(1);
  }

  _MpcExEventHeader = findNode::getClass<MpcExEventHeader>(topNode,"MpcExEventHeader");
  if(!_MpcExEventHeader){
    cout <<"No MpcExEventHeader !!!"<<endl;
    exit(1);
  }
}

int mMpcExDeadHot::process_event(PHCompositeNode* topNode){
  //trigger setting
  TriggerHelper* myTH = new TriggerHelper(topNode);
//cout<<"0"<<endl;
  int fire_minbias0 = myTH->trigScaled("BBCLL1(>0 tubes)");
//cout<<"0.1"<<endl;  
  int fire_minbias1 = myTH->trigScaled("BBCLL1(>0 tubes) novertex");
//cout<<"0.2"<<endl;  
  int fire_minbias2 = myTH->trigScaled("BBCLL1(>0 tubes) narrowvtx");
//cout<<"0.3"<<endl;  
  if(!(fire_minbias0||fire_minbias1||fire_minbias2)) return EVENT_OK;
//cout<<"0.35"<<endl;
  //data check stack = 1
  if(_MpcExEventHeader->getStack() != 1) return EVENT_OK;
//cout<<"0.4"<<endl;
//  if(_MpcExEventHeader->getStack()>) {
//     if(!StatePhaseCheck(_MpcExEventHeader)) return ABORTEVENT; 
//  }
  int PARSTCode = PARSTCheck(_MpcExEventHeader);
//cout<<"0.5"<<endl;  
  if(!PARSTCode) return EVENT_OK;
//cout<<"0.6"<<endl;
  CellIDCheck(_MpcExEventHeader);

//cout<<"1"<<endl;
  TMpcExHitSet<> rawHits(_MpcExRawHit);
  TMpcExHitSet<>::const_iterator itr = rawHits.get_iterator();
  TMpcExHitSet<>::const_iterator last = rawHits.end();

  for(; itr!=last; ++itr){
    TMpcExHit* hit = *itr;
    unsigned int key = hit->key();
    TMpcExCalib *calib = _TMpcExCalibContainer->get(key);
    unsigned short arm = hit->arm();
    unsigned short packet = hit->packet();
    unsigned short chain = hit->chain();
    unsigned short chip = hit->chip();
//    unsigned short chipmap = hit->chipmap();
    // PARST check
    if( ((PARSTCode==2) && (arm==0)) || ((PARSTCode==3) && (arm==1)) )continue; 
    if (_FailCellIDCheck[arm][packet][chain][chip]) continue;
    
    if(hit->high()>0.0) hhigh_rawadc_key->Fill(key,hit->high());
    if(hit->low()>0.0) hlow_rawadc_key->Fill(key,hit->low());
    if(hit->high()>0.0 && calib->high_pedestal_chi2()>0) hhigh_adc_key->Fill(key,hit->high()-calib->high_pedestal());
    if(hit->low()>0.0 && calib->low_pedestal_chi2()>0) hlow_adc_key->Fill(key,hit->low()-calib->low_pedestal());
    double high_adc_sub = hit->high() - calib->high_pedestal();
    double low_adc_sub = hit->low() - calib->low_pedestal();
    if(high_adc_sub > 10 && calib->high_pedestal_chi2()>0){
      if(high_deadhot_list.find(key) == high_deadhot_list.end()){
        hgrammyh[arm]->FillEx(key,high_adc_sub);
      }
    }
    if(low_adc_sub > 5 && calib->low_pedestal_chi2()>0){
      if(low_deadhot_list.find(key) == low_deadhot_list.end()){
        hgrammyl[arm]->FillEx(key,low_adc_sub);
      }
    }
  }

//cout<<"2"<<endl;
  return EVENT_OK;
}

int mMpcExDeadHot::CellIDCheck(MpcExEventHeader *evt_head){
  //this method stores a list of svx4s that fail the cellid check in this event

  //fill map and array of cellID values for each svx4
  unsigned int Svx4CellIDs[2][8][48] = {{{0}}};
  for (unsigned int i = 0; i < evt_head->getCellIDsSize(); i++){
    //read event header to get cell ID for each chip
    unsigned int a = evt_head->getCellIDsArm(i);
    unsigned int p = evt_head->getCellIDsPkt(i);
    unsigned int s = evt_head->getCellIDsSVXID(i); //this is the chip + 12*chain
    unsigned int cellIDvalue =  evt_head->getCellIDsValue(i);
    Svx4CellIDs[a][p][s] = cellIDvalue;
  }

  //loop over quadrants
  for (unsigned int a=0; a < MpcExConstants::NARMS; a++) { //arm loop
    for (unsigned int p=0; p < MpcExConstants::NPACKETS_PER_ARM; p++) { //packet
      for (unsigned int c=0; c < MpcExConstants::NCHAINS_PER_PACKET; c++) { //chain 

        //make and clear cellIDMap
        std::map<unsigned int, int> CellIDMap;
        CellIDMap.clear();

        //count frequency of each cellID value
        for (unsigned int s=0; s < MpcExConstants::NCHIPS_PER_CHAIN; s++) { //chip
          //initialize the flag to default fail
          _FailCellIDCheck[a][p][c][s] = 1; //true
          
          //find cellID value for a given arm, packet, chain, chip
          int svnum = c*12+s;
          unsigned int cellID = Svx4CellIDs[a][p][svnum];
          
          //if it is new to the map, fill it in
          if (CellIDMap.find(cellID)==CellIDMap.end()) 
            { 
              CellIDMap[cellID] = 1;   
            }
          else      //otherwise increment cellID count
            CellIDMap[cellID]++;
        }
        
        //find most common non0 cellID
        int maxFreq = -1;
        unsigned int MostCommonCellID = 9999;
        std::map<unsigned int, int>::iterator it;
        for (it = CellIDMap.begin(); it != CellIDMap.end(); it++){
          unsigned int cellID = it->first;
          int Freq = it->second;
          if (Freq > maxFreq && cellID!=0)
            {
              maxFreq = Freq;
              MostCommonCellID = cellID;
            }
        }             
        //if all cellIDs=0, then MostCommonCellID=9999 and all in this quadrant fail the check

        for (unsigned int s=0; s < MpcExConstants::NCHIPS_PER_CHAIN; s++) { //chip
          int svnum = c*12+s;
          unsigned int cellID = Svx4CellIDs[a][p][svnum];
          if (cellID == MostCommonCellID) //if most common it doesn't fail
            {
              _FailCellIDCheck[a][p][c][s] = 0; //false
            }
        }
      }
    }
  }

  return 0;
}


// Check the PARST times - better be the same for all the packets in an 
// arm, and not within the bad range for each arm. 
int mMpcExDeadHot::PARSTCheck(MpcExEventHeader *evt_head){

  // Run-16 J. Lajoie 5/28/2016
  int nbadp[MpcExConstants::NARMS] = {6,6};
  int bad_parsttime[MpcExConstants::NARMS][6] = { {32,33,43,44,45,46},{32,33,43,44,45,46} }; 
  int retval = 1; // 1=both arms OK, 2=skip arm=0, 3=skip arm=1
  
  for (unsigned int a=0; a < MpcExConstants::NARMS; a++) { //arm loop

    int prev_parsttime = -1; 
    for (unsigned int p=0; p < MpcExConstants::NPACKETS_PER_ARM; p++) { //packet

      int parsttime = (int) evt_head->getPARSTTime(a*MpcExConstants::NPACKETS_PER_ARM+p); 
      if((parsttime!=prev_parsttime) && (prev_parsttime>0)){
        if(a==0) {
          retval = 2;
        }
        else if (a==1){
          if(retval==2) 
            return 0; // skip event, both arms bad
          else
            retval = 3; 
        }       
      }
      else{
        prev_parsttime = parsttime;
        for(int i=0; i<nbadp[a]; i++){
          if(parsttime==bad_parsttime[a][i]){
            if(a==0) {
              retval = 2;
            }
            else if (a==1){
              if(retval==2) 
                return 0; // skip event, both arms bad
              else
                retval = 3; 
            }
          }
        }
      }
    }
  }

  return retval; 
}


int mMpcExDeadHot::StatePhaseCheck(MpcExEventHeader *evt_head){
  // Run-16 J. Lajoie 3/30/2016
  int nbadsp[MpcExConstants::NARMS] = {50,50};
  unsigned bad_statephase[MpcExConstants::NARMS][50] = { {39,40,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,84,85,86,118,119,150,151,182,183,184,201,213,214,215,216,248,249,250,281,282,283,312,313,346,347,378,388,411,443,475,476,499}, {39,40,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,84,85,86,118,119,150,151,182,183,184,201,213,214,215,216,248,249,250,281,282,283,312,313,346,347,378,388,411,443,475,476,499} }; 

  for (unsigned int a=0; a < MpcExConstants::NARMS; a++) { //arm loop
    for (unsigned int p=0; p < MpcExConstants::NPACKETS_PER_ARM; p++) { //packet
      for(int i=0; i<nbadsp[a]; i++){        
      unsigned int statephase = evt_head->getStatephase(a*MpcExConstants::NPACKETS_PER_ARM + p) & 0xfff; 
        if(statephase==bad_statephase[a][i]) return 0;
      }
    }
  }
  return 1; 
}
