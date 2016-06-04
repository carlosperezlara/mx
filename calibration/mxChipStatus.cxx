#include "mxChipStatus.h"

#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

mxChipStatus::mxChipStatus() {
  fDebug = false;
  for(int k=0; k!=768; ++k) fData[k] = 0;
  for(int k=0; k!=768; ++k) fCID[k] = -1;
  fSLock = false;
  fNLock = false;
}
void mxChipStatus::Print() {
  for(int k=0; k!=768; ++k)
    if(fData[k]==1)
      std::cout << "TAGGED BAD CHIP: " << k << std::endl;
}
int mxChipStatus::CellID(int key) {
  if(key<0||key>=768) return -999999;
  return fCID[key];
}
void mxChipStatus::Set(int key, int val) {
  if(key<0||key>767) return;
  fData[key] = val;;
}

void mxChipStatus::Read(int mySVXMap[2][8][48]) {
  fSLock = false;
  fNLock = false;
  for(int k=0; k!=768; ++k) fCID[k] = -1;

  //===================
  // obtaining cellIDs
  vector<int> pattern;
  int cid[12];
  for(int arm=0; arm!=2; ++arm)
    for(int pkt=0; pkt!=8; ++pkt) {
      int prefix = arm*8*48 + pkt*48;
      for(int chain=0; chain!=4; ++chain) {
	pattern.clear();
	for(int svx = 0; svx!=12; ++svx) {
	  int svi = chain*12+svx;
	  cid[svx] = mySVXMap[arm][pkt][svi];
	  int idx = prefix + svi;
	  if(IsTag(idx)) continue; // removing pre-filtered chips from pattern
	  pattern.push_back(mySVXMap[arm][pkt][svi]);
	  sort(pattern.begin(),pattern.end());
	  pattern.erase(std::unique(pattern.begin(),pattern.end()),pattern.end());
	}
	if(fDebug) {
	  cout << "ALL CIDS: ";
	  for(int chp=0; chp!=12; ++chp)
	    cout << cid[chp] << " ";
	  cout << endl;
	  cout << "PATTERN: ";
	  for(unsigned int i=0; i!=pattern.size(); ++i)
	    cout << pattern.at(i) << " ";
	  cout << endl;
	}
	int maximum = 0;
	int maxvalue = 0; // cellid==zero means bad chip
	for(unsigned int i=0; i!=pattern.size(); ++i) {
	  if( pattern.at(i) < 1 ) continue; // trend must be healthy
	  if( pattern.at(i) > 47 ) continue; // trend must be healthy
	  if(fDebug) cout << " FOR THIS>>" << pattern.at(i);
	  // counting number of chips in this pattern
	  int cnt=0;
	  for(int j=0; j!=12; ++j) {
	    int idx = prefix + chain*12+j;
	    if(IsTag(idx)) continue; // removing pre-filtered chips from trend
	    if(cid[j]!=pattern.at(i)) continue;
	    ++cnt;
	  }
	  if(fDebug) cout << "<< there are " << cnt << endl;
	  // updating
	  if(maximum < cnt) {
	    maximum = cnt;
	    maxvalue = pattern.at(i);
	  }
	}
	if(fDebug) cout << "FINAL CIDS: ";
	for(unsigned int i=0; i!=12; ++i) {
	  int idx = prefix + chain*12+i;
	  fCID[idx] = cid[i]==maxvalue?maxvalue:0;
	  if(fDebug) cout << fCID[idx] << " ";
	}
        if(fDebug) cout << endl << endl;
      }
    }
  //===================

  //===================
  // updating lock status
  for(int chp=0; chp!=384; ++chp)
    if(IsBad(chp)&&(!IsTag(chp)))
      fSLock = true;
  for(int chp=384; chp!=768; ++chp)
    if(IsBad(chp)&&(!IsTag(chp)))
      fNLock = true;
  //===================

  return;
}
