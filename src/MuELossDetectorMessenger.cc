// Created on May 12, 2010, Hexc, Kanishka

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "MuELossDetectorMessenger.hh"

#include "MuELossDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuELossDetectorMessenger::MuELossDetectorMessenger(
                                           MuELossDetectorConstruction* MuELossDet)
:MuELossDetector(MuELossDet)
{ 
  MuELossDir = new G4UIdirectory("/MuELoss/");
  MuELossDir->SetGuidance("UI commands of this example");
  
  detDir = new G4UIdirectory("/MuELoss/det/");
  detDir->SetGuidance("detector control");
       
  NbLayersCmd = new G4UIcmdWithAnInteger("/MuELoss/det/setNbOfLayers",this);
  NbLayersCmd->SetGuidance("Set number of layers.");
  NbLayersCmd->SetParameterName("NbLayers",false);
  NbLayersCmd->SetRange("NbLayers>0 && NbLayers<500");
  NbLayersCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  UpdateCmd = new G4UIcmdWithoutParameter("/MuELoss/det/update",this);
  UpdateCmd->SetGuidance("Update calorimeter geometry.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);
      
  MagFieldCmd = new G4UIcmdWithADoubleAndUnit("/MuELoss/det/setField",this);  
  MagFieldCmd->SetGuidance("Define magnetic field.");
  MagFieldCmd->SetGuidance("Magnetic field will be in Z direction.");
  MagFieldCmd->SetParameterName("Bz",false);
  MagFieldCmd->SetUnitCategory("Magnetic flux density");
  MagFieldCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  


  MakeCloud = new G4UIcmdWithAnInteger("/MuELoss/det/MakeCloud",this);
  MakeCloud->SetGuidance("Set position of the Cloud layer from the ground");
  MakeCloud->SetParameterName("LayerPos",false);
  MakeCloud->SetRange("LayerPos>=0 && LayerPos<99");
  MakeCloud->AvailableForStates(G4State_PreInit,G4State_Idle);

  ATMmodelTypeCmd = new G4UIcmdWithAnInteger("/MuELoss/det/atmModelType",this);
  ATMmodelTypeCmd->SetGuidance("Set the types of the atm model: General, Summer time, Winter time");
  ATMmodelTypeCmd->SetGuidance("General model = 0, Summer time model = 1, Winter time = 2");
  ATMmodelTypeCmd->SetParameterName("atmModel",false);
  ATMmodelTypeCmd->SetRange("atmModel>=0 && atmModel<=2");
  ATMmodelTypeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  ATMDenPerCmd = new G4UIcmdWithADouble("/MuELoss/det/atmDenPerc",this);
  ATMDenPerCmd->SetGuidance("Set the atm density percentage ");
  ATMDenPerCmd->SetGuidance("This value can only be chaged in the Genaral atm model");
  ATMDenPerCmd->SetGuidance("The default value is 1.0");
  ATMDenPerCmd->SetGuidance("Ex:1% den. decrease=0.99 and 1% den. increase=1.01");
  ATMDenPerCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuELossDetectorMessenger::~MuELossDetectorMessenger()
{
  delete NbLayersCmd;
  delete UpdateCmd;
  delete MagFieldCmd;
  delete detDir;
  delete MuELossDir; 
  delete MakeCloud; 
  delete ATMmodelTypeCmd; 
  delete ATMDenPerCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuELossDetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
   
  if( command == NbLayersCmd )
   { MuELossDetector->SetNbOfLayers(NbLayersCmd->GetNewIntValue(newValue));}

  if( command == MakeCloud)
   { MuELossDetector->SetCloud(MakeCloud->GetNewIntValue(newValue));}

  if( command == ATMmodelTypeCmd)
    { MuELossDetector->SetatmModel(ATMmodelTypeCmd->GetNewIntValue(newValue));}

  if( command == ATMDenPerCmd)
    { MuELossDetector->SetDenPerc(ATMDenPerCmd->GetNewDoubleValue(newValue));}
  
  if( command == UpdateCmd )
   { MuELossDetector->UpdateGeometry(); }

  if( command == MagFieldCmd )
   { MuELossDetector->SetMagField(MagFieldCmd->GetNewDoubleValue(newValue));}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
