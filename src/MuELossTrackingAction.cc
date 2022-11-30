// 10/6/2014: Hexc, Olesya - Adding code to write root output for data
//    analysis.  This is copied from ECRS tracking action code
#include "MuELossTrackingAction.hh"

#include "G4UserTrackingAction.hh"
#include "G4ParticleDefinition.hh"
#include "G4UnitsTable.hh"
#include "G4AnalysisManager.hh"

//#include "g4root.hh"

using namespace CLHEP;

MuELossTrackingAction::MuELossTrackingAction()
{}

MuELossTrackingAction::~MuELossTrackingAction()
{}

void MuELossTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  // Here we print the Particle Data Group particle id number as well as the
  // energy when a new particle is created for tracking
  G4ParticleDefinition* particle = aTrack->GetDefinition();
  G4int PID = particle->GetPDGEncoding();
  G4double KE = aTrack->GetKineticEnergy();
  G4double Global_time = aTrack->GetGlobalTime();
  G4double Local_time = aTrack->GetLocalTime();
  G4double Proper_time = aTrack->GetProperTime();
  G4ThreeVector P = aTrack->GetMomentum();
  
  G4String particleName = particle->GetParticleName();
  G4ThreeVector position = aTrack->GetPosition();

  G4int flagParticle = 99;   // Initialize 
  
  if (particleName == "proton") { flagParticle = 1; }
  else if (particleName == "neutron")  { flagParticle = 2; }
  else if (particleName == "mu+") { flagParticle = 3; }
  else if (particleName == "mu-") { flagParticle = 4; }
  else if (particleName == "e+") { flagParticle = 5; }
  else if (particleName == "e-") { flagParticle = 6; }
  else if (particleName == "gamma") {flagParticle = 7; }
  else if (particleName == "pi+") { flagParticle = 8; }
  else if (particleName == "pi-") { flagParticle = 9; }
  else if (particleName == "C12")  { flagParticle = 10; }
  else if (particleName == "C13")  { flagParticle = 11; }
  else if (particleName == "He3")  { flagParticle = 12; }
  else if (particleName == "deutron") { flagParticle = 13; }
  else if (particleName == "N14") { flagParticle = 14; }
  else if (particleName == "anti_proton") { flagParticle = 15; }
  else if (particleName == "anti_neutron") { flagParticle = 16; }
  else if (particleName == "triton") { flagParticle = 17; }
  else 
    {
      flagParticle = 99;
    }
  
  // get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  
  G4double altitude = sqrt( position.getX()/m * position.getX()/m + 
			    position.getY()/m * position.getY()/m + 
			    position.getZ()/m * position.getZ()/m) - 6371200;

  // fill ntuple
  analysisManager->FillNtupleDColumn(0, flagParticle);
  analysisManager->FillNtupleDColumn(1, KE/MeV);
  analysisManager->FillNtupleDColumn(2, position.getX()/m);
  analysisManager->FillNtupleDColumn(3, position.getY()/m);
  analysisManager->FillNtupleDColumn(4, position.getZ()/m);
  analysisManager->FillNtupleDColumn(5, altitude);
  analysisManager->FillNtupleDColumn(6, P.getX()/MeV);
  analysisManager->FillNtupleDColumn(7, P.getY()/MeV);
  analysisManager->FillNtupleDColumn(8, P.getZ()/MeV);
  analysisManager->FillNtupleDColumn(9, Global_time/ms);
  analysisManager->FillNtupleDColumn(10, Local_time/ms);
  analysisManager->FillNtupleDColumn(11, Proper_time/ms);
  analysisManager->AddNtupleRow();

}

void MuELossTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
}
