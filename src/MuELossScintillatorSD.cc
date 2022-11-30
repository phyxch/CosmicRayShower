// Created on 7/16/2010  Kanishka, David, Hexc
//
// This code is for defining a sensitive detector volume near the surface of the
// Earth to recording particle flux.
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "MuELossScintillatorSD.hh"

#include "MuELossScintHit.hh"
#include "MuELossDetectorConstruction.hh"

#include "G4VPhysicalVolume.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include "G4UnitsTable.hh"

#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

MuELossScintillatorSD::MuELossScintillatorSD(G4String name,
					     MuELossDetectorConstruction* det)
  :G4VSensitiveDetector(name), Detector(det), nhits(0)
{
  collectionName.insert("ScintHitCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

MuELossScintillatorSD::~MuELossScintillatorSD()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MuELossScintillatorSD::Initialize(G4HCofThisEvent* HCE)
{
  G4cout << "Initializing MuELossScintillatorD" << G4endl;
  ScintCollection = new MuELossScintHitsCollection (SensitiveDetectorName, collectionName[0]);
  //ScintCollection = new MuELossScintHitsCollection (GetName(), collectionName[0]);   
  G4int HCID = -1;
  if (HCID < 0) HCID = G4SDManager::GetSDMpointer()->GetCollectionID(ScintCollection);
  HCE->AddHitsCollection(HCID, ScintCollection);
  G4cout << " HCID = " << HCID << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool MuELossScintillatorSD::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  if (edep == 0.) return     true;

  auto etot = aStep->GetTrack()->GetTotalEnergy();
  G4double globt = aStep->GetTrack()->GetGlobalTime();
  G4ThreeVector pos = aStep->GetTrack()->GetPosition();
  G4ThreeVector pvec = aStep->GetTrack()->GetMomentum();
  G4double xhit, yhit, zhit, px, py, pz;
  G4int tid = aStep->GetTrack()->GetTrackID();

  xhit = pos.x(); yhit = pos.y(); zhit = pos.z();
  px = pvec.x(); py = pvec.y(); pz = pvec.z();

  /*
  // Need to check to see if the track ID has been stored in the list or not
  for (G4int i = 0; i < trackIDList.size(); i++)
    {
      if (trackIDList[i] == tid) return true;
    }
  */  

  trackIDList.push_back(tid);
  
  // The following lines are good for debugging the code!
  
  G4cout << "Particles hit scintillator: " << aStep->GetTrack()->GetDefinition()->GetParticleName() 
	 << " ID:  " << aStep->GetTrack()->GetTrackID()
	 << "   etot: " << G4BestUnit(etot, "Energy") <<  "   globt: " << G4BestUnit(globt, "Time") 
	 << " pos: " << pos << "  pvec: " << pvec << G4endl;
  
  auto  theTouchable = (aStep->GetPreStepPoint()->GetTouchable());
    
  auto  physVol = theTouchable->GetVolume();

  // Create the hit object and fill the info
  auto scintHit = new MuELossScintHit();
  scintHit->AddScintHit(etot, globt, xhit, yhit, zhit,
			px, py, pz, tid,
			aStep->GetTrack()->GetDefinition()->GetParticleName());
  
  //  if (physVol == Detector->GetScintillator()) {
  ScintCollection->insert(scintHit);
  //    nhits++;
  //  }
  
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MuELossScintillatorSD::EndOfEvent(G4HCofThisEvent* HCE)
{
  static G4int HCID = -1;
  if(HCID<0)
    { 
      HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); 
    }

  HCE->AddHitsCollection(HCID, ScintCollection);
  // G4cout << " (MuELossScintillatorSD) nhits: *** " << nhits << "  HCID: " << HCID << G4endl;

  // The line below is for debugging purposes
  // ScintCollection->PrintAllHits();
  trackIDList.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MuELossScintillatorSD::clear()
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MuELossScintillatorSD::DrawAll()
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MuELossScintillatorSD::PrintAll()
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

