// Created on May 12, 2010, Hexc, Kanishka
//
// Modified on July 1, 2010
//
// Updated on 7/9/2010, Add output functionality
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef MuELossRunAction_h
#define MuELossRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

#include "MuELossOutput.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;

class MuELossRunAction : public G4UserRunAction
{
public:
  MuELossRunAction();
  virtual ~MuELossRunAction();

  void BeginOfRunAction(const G4Run*);
  void   EndOfRunAction(const G4Run*);
    
  void fillPerEvent(G4double, G4double); 

private:
  
  const G4Run*  currentRun;
  
  // 
  G4double aveELoss, aveTrackLength;
  G4double aveELoss2, aveTrackLength2;

  MuELossOutput* fileOut;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

