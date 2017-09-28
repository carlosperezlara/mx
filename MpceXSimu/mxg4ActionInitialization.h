// origin: S.Karthas (Aug 2016)

#ifndef mxg4ActionInitialization_h
#define mxg4ActionInitialization_h 1
#include "G4VUserActionInitialization.hh"
class mxg4DetectorConstruction;

class mxg4ActionInitialization : public G4VUserActionInitialization
{
  public:
    mxg4ActionInitialization();
    virtual ~mxg4ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

#endif
