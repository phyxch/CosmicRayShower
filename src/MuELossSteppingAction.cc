// Created on May 12, 2010, Hexc, Kanishka
// Updated on May 18, 2010
// Updated on June 21, 2010, Hexc, Kanishka
// Updated on Dec 03,2012, Kanishka

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "MuELossSteppingAction.hh"

#include "MuELossDetectorConstruction.hh"
#include "MuELossEventAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include <G4DynamicParticle.hh>
#include "G4UnitsTable.hh"

#include <stdlib.h>

////#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuELossSteppingAction::MuELossSteppingAction(MuELossDetectorConstruction* det,
                                         MuELossEventAction* evt)
  :detector(det), eventaction(evt),lnNo(0)					 
{
 fileOut = MuELossOutput::instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuELossSteppingAction::~MuELossSteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuELossSteppingAction::UserSteppingAction(const G4Step* aStep)
{
  // get volume of the current step
  G4VPhysicalVolume* volume 
  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  
  // collect energy and track length step by step
  G4double edep = aStep->GetTotalEnergyDeposit();

  G4double stepl = 0.;
  if (aStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0.)  // Need check this condition
  stepl = aStep->GetStepLength();
      
  G4int iLayer;
  G4String LayerName;

  LayerName = volume->GetName();
  //G4cout << " ******* LayerName " << LayerName;

  iLayer = atoi((LayerName.substr(5)).c_str());   // The layer name is like:  Layer02
  //G4cout << "   Layer number " << iLayer << G4endl;

  //get the part information 
  G4Track* thisTrack = aStep->GetTrack();
  G4ParticleDefinition* pDefinition = thisTrack->GetDefinition();
  G4String pName = pDefinition->GetParticleName();
  G4int trackID = thisTrack->GetTrackID();
  G4DynamicParticle* dynPart = (G4DynamicParticle*) thisTrack->GetDynamicParticle() ;
  G4double partKE = dynPart->GetKineticEnergy();
  G4int parentID = thisTrack->GetParentID();
  G4double pressureValue;

  if(lnNo == 0) { 
    trackIDprev=trackID; 
    lnNo++; 
    if (pName=="pi+" || pName=="pi-" || pName=="kaon+" || pName=="kaon-" || pName=="mu-" || pName=="mu+" ||  pName=="proton" ||  pName=="neutron"){
      //The bellow line is added to get the air layer pressure when the particle information is printed out. xiaohang 03/12/2014
      pressureValue = detector->GetAirDensity((100-iLayer)*1.e06,1,0); 
      fileOut->fout << "pName  " << pName << "  trackID  " << trackID <<"  Height  " <<(100.-iLayer) <<"  Pressure  "<<pressureValue <<"  KinEnergy  " <<G4BestUnit(partKE,"Energy") << "  parentID  " <<parentID << G4endl; 
      //G4cout<< "pName  " << pName << "  trackID  " << trackID <<"  iLayer  " <<iLayer <<"  Pressure  "<<pressureValue <<"  KinEnergy  " <<G4BestUnit(partKE,"Energy") << "  parentID  " <<parentID << G4endl; 
      }
  } 

  if(trackID!=trackIDprev) {
    if (pName=="pi+" || pName=="pi-" || pName=="kaon+" || pName=="kaon-" || pName=="mu-" || pName=="mu+" ||  pName=="proton" ||  pName=="neutron"){
      pressureValue = detector->GetAirDensity((100-iLayer)*1.e06,1,0); 
      fileOut->fout << "pName  " << pName << "  trackID  " << trackID <<"  Height  " <<(100.-iLayer)<<"  Pressure  "<<pressureValue <<"  KinEnergy  " <<G4BestUnit(partKE,"Energy")<< "  parentID  " <<parentID << G4endl;
      //G4cout<< "pName  " << pName << "  trackID  " << trackID <<"  iLayer  " <<iLayer <<"  Pressure  "<<pressureValue <<"  KinEnergy  " <<G4BestUnit(partKE,"Energy") << "  parentID  " <<parentID << G4endl; 
     
      }
    trackIDprev=trackID;
  }

  if (LayerName != "World") {
    eventaction->AddELossTrackLength(iLayer, edep, stepl);  
  }

}
