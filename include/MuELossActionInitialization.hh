/// Created on Nov 30, 2020 by hexc
/// For running MuELoss in multithreaded mode
///
#ifndef MuELossActionInitialization_h
#define MuELossActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "MuELossDetectorConstruction.hh"

/// Action initialization class.
///

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MuELossActionInitialization : public G4VUserActionInitialization
{
public:
  MuELossActionInitialization(MuELossDetectorConstruction*);
  virtual ~MuELossActionInitialization();
  
  virtual void BuildForMaster() const;
  virtual void Build() const;
  
private:
  MuELossDetectorConstruction *detector;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
