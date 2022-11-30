// Created on May 12, 2010, Hexc, Kanishka

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef MuELossEventAction_h
#define MuELossEventAction_h 1

#include "MuELossDetectorConstruction.hh"

#include "G4UserEventAction.hh"
#include "MuELossOutput.hh"

class MuELossRunAction;
class MuELossEventActionMessenger;
//class MuELossOutput;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MuELossEventAction : public G4UserEventAction
{
public:
  MuELossEventAction(MuELossRunAction*);
  virtual ~MuELossEventAction();
  
  void  BeginOfEventAction(const G4Event*);
  void    EndOfEventAction(const G4Event*);
  
  void SetPrintModulo(G4int    val)  {printModulo = val;};
  
  void AddELossTrackLength(G4int, G4double, G4double);

private:
  MuELossRunAction*  runAct;
  
  std::vector<G4double>  EnergyDepositLayer;
  std::vector<G4double>  TrackLengthLayer;
  G4double TotalEnergyDeposit, TotalTrackLength;

  G4int     scintNearEarthSurfaceID;
  G4int     nhitNearEarthSurface;
  
  G4int     printModulo;
  
  MuELossEventActionMessenger*  eventMessenger;

  MuELossOutput*  fileOut;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
