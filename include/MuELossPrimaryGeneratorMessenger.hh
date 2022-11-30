// Created on May 12, 2010, Hexc, Kanishka
//
// Updated on 7/9/2010: Added command option for running primary cosmic rays
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef MuELossPrimaryGeneratorMessenger_h
#define MuELossPrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class MuELossPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MuELossPrimaryGeneratorMessenger: public G4UImessenger
{
public:
  MuELossPrimaryGeneratorMessenger(MuELossPrimaryGeneratorAction*);
  virtual ~MuELossPrimaryGeneratorMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  MuELossPrimaryGeneratorAction* MuELossAction;
  G4UIdirectory*               gunDir; 
  G4UIcmdWithAString*          RndmCmd;
  G4UIcmdWithAString*          CosmicRay;
  G4UIcmdWithADoubleAndUnit*   LaunchHeight;
  G4UIcmdWithADouble*          RigiCutCmd;   
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

