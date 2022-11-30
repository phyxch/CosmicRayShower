// Created on May 12, 2010, Hexc, Kanishka
// Updated on May 18, 2010
// Updated on 9/16/2014, Hexc, Olesya:  Added CLHEP namespace

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "MuELossPrimaryGeneratorAction.hh"

#include "MuELossDetectorConstruction.hh"
#include "MuELossPrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "G4UnitsTable.hh"
#include "math.h"

using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuELossPrimaryGeneratorAction::MuELossPrimaryGeneratorAction(MuELossDetectorConstruction* MuELossDC)
  :MuELossDetector(MuELossDC),rndmFlag("off"),
   cosmicRay("false"),  RigiVal(1.0), lnNoTmp(0), NormConst(1.0584),
   height(49.0*km)
{

  // Get the reference for writing output to a file
  fileOut = MuELossOutput::instance();

  G4int n_particle = 1;
  particleGun  = new G4ParticleGun(n_particle);
  
  //create a messenger for this class
  gunMessenger = new MuELossPrimaryGeneratorMessenger(this);

  // default particle kinematic

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
                    = particleTable->FindParticle(particleName="mu-");
  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,-1.));
  //  particleGun->SetParticleEnergy(5.*GeV);
  particleGun->SetParticleMomentum(5.*GeV);
  G4double position = -0.5*(MuELossDetector->GetWorldSizeZ());
  particleGun->SetParticlePosition(G4ThreeVector(0.*cm,0.*cm, position));

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuELossPrimaryGeneratorAction::~MuELossPrimaryGeneratorAction()
{
  delete particleGun;
  delete gunMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuELossPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of event
  // 
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;

  // I commented this out since we are setting the particle launching height from the command prompt
  //  G4double z0 = 0.98*0.5*(MuELossDetector->GetWorldSizeZ());
  G4double z0 = height;
  G4double y0 = 0.*cm, x0 = 0.*cm;

 
  if (rndmFlag == "on")
    {
      z0 = (MuELossDetector->GetWorldSizeZ())*(G4UniformRand()-0.5);
    }
  
  if (cosmicRay == "on") 
    {
      G4ParticleDefinition* particle
	= particleTable->FindParticle(particleName="proton");
      particleGun->SetParticleDefinition(particle);
      // We set the proton KE below 100 GeV.
      G4double protonKE;
      NormConst = ((18./17.)*pow(RigiVal,-1.7)) - ((18./17.)*pow(100.0,-1.7));
      //fileOut->fout <<"***************************Rigi Val & Norm Cosnt " <<RigiVal << "  "<< NormConst << G4endl;
 
      while(1) {
	protonKE = RigiVal + ((100.0-RigiVal) * G4UniformRand());
	if (probProtonKE(protonKE,NormConst) > G4UniformRand()) break;
      }
      particleGun->SetParticleEnergy(protonKE*GeV);

      //display the Rigitity cut one time in the output file
      if (lnNoTmp==0) { fileOut->fout <<"Rigitity cut  " << RigiVal << G4endl; lnNoTmp++; }

    } 
  
  if (cosmicRay == "off") 
    {
      G4ParticleDefinition* particle
	= particleTable->FindParticle(particleName="mu-");
      particleGun->SetParticleDefinition(particle);
      particleGun->SetParticleMomentum(5.*GeV);
      
    }
  
  particleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  particleGun->GeneratePrimaryVertex(anEvent);

  // Output primary particle information

  fileOut->fout << particleGun->GetParticleDefinition()->GetParticleName() << "   "
		<< G4BestUnit(particleGun->GetParticleEnergy(), "Energy") << "  "
		<< particleGun->GetParticleMomentum() << "  "
		<< particleGun->GetParticleTime() << "   "
		<< (particleGun->GetParticleMomentumDirection()).getX() << "  "
		<< (particleGun->GetParticleMomentumDirection()).getY() << "  "
		<< (particleGun->GetParticleMomentumDirection()).getZ() << G4endl;
  
}

G4double MuELossPrimaryGeneratorAction::probProtonKE(G4double energy, G4double NormCnt)
{
  // We used the primary proton KE distribution from PDG for selecting proton KE
  // The constant, 1.0584, was determined from a normalization.  This is only valid for choosing KE in a
  // range from 1 to 100 GeV.  For higher KE, one has to determine this constant again.  See the code
  // "primaryProtonDist.C".

  //I modified the code such that now it calculates the normalization constant for the given rigitity cut off value. 
  //The default rigitity value is 1.0 GeV. 
  //But still this is only valid for choosing KE in the range from given rigitity value to 100 GeV.
  //Kanishka 10/19/12.
  
  return 1.8*pow(energy, -2.7) / NormCnt;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

