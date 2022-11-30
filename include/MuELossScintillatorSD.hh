// Created on 7/16/2010  Kanishka, David, Hexc
//
// This code is for defining a sensitive detector volume near the surface of the
// Earth to recording particle flux.
// 
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef MuELossScintillatorSD_h
#define MuELossScintillatorSD_h 1

#include "G4VSensitiveDetector.hh"
#include "globals.hh"

class MuELossDetectorConstruction;
class G4HCofThisEvent;
class G4Step;

#include "MuELossScintHit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class MuELossScintillatorSD : public G4VSensitiveDetector
{
public:
  
  MuELossScintillatorSD(G4String, MuELossDetectorConstruction* );
  ~MuELossScintillatorSD();
  
  void Initialize(G4HCofThisEvent*);
  G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  void EndOfEvent(G4HCofThisEvent*);
  void clear();
  void DrawAll();
  void PrintAll();
  
private:
  
  MuELossScintHitsCollection*  ScintCollection;      
  MuELossDetectorConstruction* Detector;
  G4int                   nhits;
  std::vector<G4int> trackIDList;

};

#endif
