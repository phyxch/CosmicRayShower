// Created on May 12, 2010, Hexc, Kanishka
// Updated on May 18, 2010

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef MuELossDetectorMessenger_h
#define MuELossDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class MuELossDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithoutParameter;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MuELossDetectorMessenger: public G4UImessenger
{
  public:
    MuELossDetectorMessenger(MuELossDetectorConstruction* );
   ~MuELossDetectorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    MuELossDetectorConstruction* MuELossDetector;
    
    G4UIdirectory*             MuELossDir;
    G4UIdirectory*             detDir;
    G4UIcmdWithAnInteger*      NbLayersCmd;   
    G4UIcmdWithAnInteger*      MakeCloud;     // Cloud layer position   
    G4UIcmdWithAnInteger*      ATMmodelTypeCmd;     // atm model types, general, summer, winter
    G4UIcmdWithADouble*        ATMDenPerCmd; // atm density percentage change
    G4UIcmdWithADoubleAndUnit* MagFieldCmd;
    G4UIcmdWithoutParameter*   UpdateCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

