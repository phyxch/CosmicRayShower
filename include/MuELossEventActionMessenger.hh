// Created on May 12, 2010, Hexc, Kanishka

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef MuELossEventActionMessenger_h
#define MuELossEventActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class MuELossEventAction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MuELossEventActionMessenger: public G4UImessenger
{
public:
  MuELossEventActionMessenger(MuELossEventAction*);
  virtual ~MuELossEventActionMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  MuELossEventAction*     eventAction;
  G4UIdirectory*        eventDir;   
  G4UIcmdWithAnInteger* PrintCmd;    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
