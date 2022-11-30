// Created on May 12, 2010, Hexc, Kanishka


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef MuELossSteppingAction_h
#define MuELossSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "MuELossDetectorConstruction.hh"
#include "globals.hh"
//#include "MuELossOutput.hh"


class MuELossDetectorConstruction;
class MuELossEventAction;
class MuELossOutput;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MuELossSteppingAction : public G4UserSteppingAction
{
public:
  MuELossSteppingAction(MuELossDetectorConstruction*, MuELossEventAction*);
  virtual ~MuELossSteppingAction();

  void UserSteppingAction(const G4Step*);

  //This line is added by xiaohang on 03/11/2014 to call GetAirDensity  
  //MuELossDetectorConstruction* MDC;
  MuELossDetectorConstruction* detector;
    
private:

  MuELossEventAction*          eventaction;  
  MuELossOutput*  fileOut;

  G4int lnNo;
  G4int trackIDprev;  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
