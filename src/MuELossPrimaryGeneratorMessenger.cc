// Created on May 12, 2010, Hexc, Kanishka

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "MuELossPrimaryGeneratorMessenger.hh"

#include "MuELossPrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuELossPrimaryGeneratorMessenger::MuELossPrimaryGeneratorMessenger(
                                          MuELossPrimaryGeneratorAction* MuELossGun)
:MuELossAction(MuELossGun)
{
  gunDir = new G4UIdirectory("/MuELoss/gun/");
  gunDir->SetGuidance("PrimaryGenerator control");
   
  RndmCmd = new G4UIcmdWithAString("/MuELoss/gun/rndm",this);
  RndmCmd->SetGuidance("Shoot randomly the incident particle.");
  RndmCmd->SetGuidance("  Choice : on(default), off");
  RndmCmd->SetParameterName("choice",true);
  RndmCmd->SetDefaultValue("on");
  RndmCmd->SetCandidates("on off");
  RndmCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Define "CosmicRay" option
  CosmicRay = new G4UIcmdWithAString("/MuELoss/gun/cosmicRay",this);
  CosmicRay->SetGuidance("Incident primary cosmic protons (based on PDG).");
  CosmicRay->SetGuidance("  Choice : on, off(default)");
  CosmicRay->SetParameterName("choice",true);
  CosmicRay->SetDefaultValue("off");
  CosmicRay->SetCandidates("on off");
  CosmicRay->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Define "LaunchHeight" option
  LaunchHeight = new G4UIcmdWithADoubleAndUnit("/MuELoss/gun/LaunchHeight", this);
  LaunchHeight->SetGuidance("Set the height at which particle is launched.");
  LaunchHeight->SetGuidance("You may include unit.  Default unit is km.");
  LaunchHeight->SetParameterName("LaunchHeight",true);
  LaunchHeight->SetUnitCategory("Length");
  LaunchHeight->SetDefaultUnit("km");

  // Define "Rigitity cut" option
  RigiCutCmd = new G4UIcmdWithADouble("/MuELoss/gun/RigitityCut", this);
  RigiCutCmd->SetGuidance("Set the cut off rigitity value in the location");
  RigiCutCmd->SetGuidance("Unit is GeV. Default value is 1.0");
  RigiCutCmd->SetGuidance("This option can only be used in the cosmicRay on mode");
  RigiCutCmd->SetParameterName("RigiCutCmd",true);
  RigiCutCmd->AvailableForStates(G4State_PreInit,G4State_Idle);



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuELossPrimaryGeneratorMessenger::~MuELossPrimaryGeneratorMessenger()
{
  delete RndmCmd;
  delete CosmicRay;
  delete RigiCutCmd;
  delete LaunchHeight;
  delete gunDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuELossPrimaryGeneratorMessenger::SetNewValue(
                                        G4UIcommand* command, G4String newValue)
{ 
  if( command == RndmCmd )
   { MuELossAction->SetRndmFlag(newValue);}

  if( command == CosmicRay )
   { MuELossAction->SetCosmicRayFlag(newValue);}

  if( command == LaunchHeight )
    { MuELossAction->SetLaunchHeight(LaunchHeight->GetNewDoubleValue(newValue));}

  if( command == RigiCutCmd )
    { MuELossAction->SetRigiCut(RigiCutCmd->GetNewDoubleValue(newValue));}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
