// Created on May 12, 2010, Hexc, Kanishka

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef MuELossPrimaryGeneratorAction_h
#define MuELossPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

#include "MuELossOutput.hh"

class G4ParticleGun;
class G4Event;
class MuELossDetectorConstruction;
class MuELossPrimaryGeneratorMessenger;
class MuELossOutput;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MuELossPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  MuELossPrimaryGeneratorAction(MuELossDetectorConstruction*);    
  virtual ~MuELossPrimaryGeneratorAction();

  void GeneratePrimaries(G4Event*);
  void SetRndmFlag(G4String val) { rndmFlag = val;}
  void SetLaunchHeight(G4double val) { height = val;}
  void SetRigiCut(G4double val) { RigiVal = val;}
  void SetCosmicRayFlag(G4String val) { cosmicRay = val;}
  G4double probProtonKE(G4double energy, G4double NormCnt);

private:
  G4ParticleGun*                particleGun;	  //pointer a to G4  class
  MuELossDetectorConstruction*    MuELossDetector;    //pointer to the geometry
    
  MuELossPrimaryGeneratorMessenger* gunMessenger;   //messenger of this class
  G4String                      rndmFlag;	  //flag for a rndm impact point
  G4String                      cosmicRay;	  //flag for cosmic ray
  G4double                      height;       // particle launching height
  G4double                      RigiVal;  //rigitity value
  G4int                         lnNoTmp; //temparary line numbers
  G4double                      NormConst;

  MuELossOutput*  fileOut;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


