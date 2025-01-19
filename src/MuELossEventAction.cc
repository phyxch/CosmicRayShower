// Created on May 12, 2010, Hexc, Kanishka
// Jan 19, 2025, hexc. A code cleanng up was done.
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "MuELossEventAction.hh"

#include "MuELossRunAction.hh"
#include "MuELossEventActionMessenger.hh"

#include "MuELossScintHit.hh"

#include "G4Event.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VTrajectory.hh"
#include "G4VVisManager.hh"
#include "G4UnitsTable.hh"
#include "G4SDManager.hh"

#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuELossEventAction::MuELossEventAction(MuELossRunAction* run)
  :runAct(run), printModulo(1), eventMessenger(0), scintNearEarthSurfaceID(-1)
{
  eventMessenger = new MuELossEventActionMessenger(this);
  for (G4int iLayer = 0; iLayer < NUMBER_LAYER; iLayer++) {
    EnergyDepositLayer.push_back(0.0);
    TrackLengthLayer.push_back(0.0);
  }
  
  // Get the reference for writing output to a file
  fileOut = MuELossOutput::instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuELossEventAction::~MuELossEventAction()
{
  delete eventMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuELossEventAction::BeginOfEventAction(const G4Event* evt)
{  
  G4int evtNb = evt->GetEventID();
  
  if (evtNb%printModulo == 0) { 
    G4cout << "\n---> Begin of event: " << evtNb << G4endl;
  }

  G4String colName;
  if (scintNearEarthSurfaceID == -1) {
    auto *SDman = G4SDManager::GetSDMpointer();
    scintNearEarthSurfaceID = SDman->GetCollectionID(colName="scintillatorSD/ScintHitCollection");
    G4cout << " I am here %%%%%%%%%%%%%%%%%%%%%%%   scintNearEarthSurfaceID = " << scintNearEarthSurfaceID << G4endl;
  }
  
  // initialisation per event
  EnergyDepositLayer.clear();
  TrackLengthLayer.clear();
  for (G4int iLayer = 0; iLayer < NUMBER_LAYER; iLayer++) {
    EnergyDepositLayer.push_back(0.0);
    TrackLengthLayer.push_back(0.0);
  }
  TotalEnergyDeposit = 0.0;
  TotalTrackLength = 0.0;
  
  nhitNearEarthSurface = 0;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuELossEventAction::EndOfEventAction(const G4Event* evt)
{
  //print per event (modulo n)
  //
  G4int evtNb = evt->GetEventID();

  // Output the event number
  fileOut->fout << "Event number: " << evtNb << G4endl;
  
  if (evtNb%printModulo == 0) {
    G4cout << "---> End of event: " << evtNb << G4endl;	
    
    for (G4int iLayer = 0; iLayer < EnergyDepositLayer.size(); iLayer++) {
      G4cout
	<< "   Energy loss in layer: " << std::setw(4) << iLayer << std::setw(12) 
	<< G4BestUnit(EnergyDepositLayer[iLayer],"Energy")
	<< "       total track length: " << std::setw(7)
	<< G4BestUnit(TrackLengthLayer[iLayer],"Length")
	<< G4endl;
      /*
      // Write to the output file
      fileOut->fout
      << "   Energy loss in layer: " << std::setw(4) << iLayer << std::setw(12) 
      << G4BestUnit(EnergyDepositLayer[iLayer],"Energy")
      << "       total track length: " << std::setw(7)
      << G4BestUnit(TrackLengthLayer[iLayer],"Length")
      << G4endl;
      */
      
      TotalEnergyDeposit += EnergyDepositLayer[iLayer];
      TotalTrackLength += TrackLengthLayer[iLayer];     
    }
    /*
      G4cout << " Total energy loss: " << std::setw(7)
      << G4BestUnit(TotalEnergyDeposit,"Energy")
      << "       total track length: " << std::setw(7)
      << G4BestUnit(TotalTrackLength,"Length")
      << G4endl;
      // Write to output file
      fileOut->fout << " Total energy loss: " << std::setw(7)
      << G4BestUnit(TotalEnergyDeposit,"Energy")
      << "       total track length: " << std::setw(7)
		  << G4BestUnit(TotalTrackLength,"Length")
		  << G4endl;
    */
  }
  
  //
  // accumulates statistic, i.e., to calculate the mean energy loss and track length
  //
  runAct->fillPerEvent(TotalEnergyDeposit, TotalTrackLength);  
  
  // Collect information of particle hits in the near earth surface scintillator
  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();

  MuELossScintHitsCollection* SHC = NULL;

  if (HCE) SHC = (MuELossScintHitsCollection*)(HCE->GetHC(scintNearEarthSurfaceID));

  if (SHC) 
    {
      G4cout << " (MuELossEventAction) scintNearEarthSurfaceID = " << scintNearEarthSurfaceID << G4endl;
      nhitNearEarthSurface = SHC->entries();

      SHC->PrintAllHits();
    }
  
  G4cout << "Number of hits seen in the near surface scintillator: " <<
    nhitNearEarthSurface << G4endl;

  fileOut->fout << "Number of hits seen in the near surface scintillator: " <<
    nhitNearEarthSurface << G4endl;

  G4cout << " End Of Event in MuELossEventAction. " << G4endl;

}  


void MuELossEventAction::AddELossTrackLength(G4int iLayer, G4double edep, G4double stepl)
{
  //G4cout << " <<<<<<<< In AddELossTrackLength >>>>>>>>> " << iLayer << " edep = "
  // << edep << "  stepLength = " << stepl << G4endl;
  EnergyDepositLayer.at(iLayer) += edep;
  TrackLengthLayer.at(iLayer) += stepl;
  //G4cout << " <<<<<<<< In AddELossTrackLength >>>>>>>>> Done!!! " << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
