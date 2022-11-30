/// Created on Nov 30, 2020 by hexc
/// for running MuELoss in multithreaded mode
///

#include "MuELossActionInitialization.hh"
#include "MuELossRunAction.hh"
#include "MuELossEventAction.hh"
#include "MuELossPrimaryGeneratorAction.hh"
#include "MuELossSteppingAction.hh"
#include "MuELossTrackingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuELossActionInitialization::MuELossActionInitialization(MuELossDetectorConstruction *MuELossDetector)
 : G4VUserActionInitialization()
{
  detector = MuELossDetector;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuELossActionInitialization::~MuELossActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuELossActionInitialization::BuildForMaster() const
{
  SetUserAction(new MuELossRunAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuELossActionInitialization::Build() const
{
  MuELossRunAction* runAction = new MuELossRunAction();
  SetUserAction(runAction);

  G4VUserPrimaryGeneratorAction* genAction = 
    new MuELossPrimaryGeneratorAction(detector);
  SetUserAction(genAction);
  
  MuELossEventAction* eventAction = new MuELossEventAction(runAction);
  SetUserAction(eventAction);

  SetUserAction(new MuELossSteppingAction(detector, eventAction));
  SetUserAction(new MuELossTrackingAction);

}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
