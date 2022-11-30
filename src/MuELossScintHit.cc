// Created on 7/16/2010  Kanishka, David, Hexc
//
// This code is used for storing hits info in the sensitive detector. 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "MuELossScintHit.hh"
#include "G4UnitsTable.hh"
#include <iomanip>

G4Allocator<MuELossScintHit> MuELossScintHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

MuELossScintHit::MuELossScintHit()
{
   EScint = 0.; GlobalTimeScint = -100.;
   xScint = yScint = zScint = 0;
   pxScint = pyScint = pzScint = 0;
   trackID = -99;

   fileOut = MuELossOutput::instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

MuELossScintHit::~MuELossScintHit()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

MuELossScintHit::MuELossScintHit(const MuELossScintHit& right)
{
  EScint = right.EScint; GlobalTimeScint = right.GlobalTimeScint;
  xScint = right.xScint; yScint = right.yScint; zScint = right.zScint; 
  pxScint = right.pxScint; pyScint = right.pyScint; pzScint = right.pzScint; 
  particleName = right.particleName; trackID = right.trackID;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

const MuELossScintHit& MuELossScintHit::operator=(const MuELossScintHit& right)
{
  EScint = right.EScint; GlobalTimeScint = right.GlobalTimeScint;
  xScint = right.xScint; yScint = right.yScint; zScint = right.zScint; 
  pxScint = right.pxScint; pyScint = right.pyScint; pzScint = right.pzScint; 
  particleName = right.particleName; trackID = right.trackID;
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

int MuELossScintHit::operator==(const MuELossScintHit& right) const
{
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MuELossScintHit::Draw()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MuELossScintHit::Print()
{
//  if(particleName=="pi+" || particleName=="pi-" || particleName=="kaon+" || particleName=="kaon-" || particleName=="mu-" || particleName=="mu+" ||  particleName=="proton" ||  particleName=="neutron")
  //  {
      G4cout << std::setw(6) << particleName
	     << " TrID: " << std::setw(6) << trackID
	     << " E: " << std::setw(8) <<  G4BestUnit(EScint, "Energy") 
	     << " gt: " << std::setw(11) << G4BestUnit(GlobalTimeScint, "Time") 
	     << " x: "  << std::setw(6) << G4BestUnit(xScint, "Length")
	     << " y: "  << std::setw(6) << G4BestUnit(yScint, "Length")
	     << " z: "  << std::setw(6) << G4BestUnit(zScint, "Length")
	     << " px: "  << std::setw(6) << G4BestUnit(pxScint, "Energy")
	     << " py: "  << std::setw(6) << G4BestUnit(pyScint, "Energy")
	     << " pz: "  << std::setw(6) << G4BestUnit(pzScint, "Energy")
	     <<G4endl;;
    //}
  //if(particleName != "anti_nu_mu"&&particleName!= "anti_nu_e" &&particleName != "nu_e" && particleName != "nu_mu" && (pow(pxScint,2)+pow(pyScint,2)+pow(pzScint,2))!=0)
  if(particleName=="pi+" || particleName=="pi-" || particleName=="kaon+" || particleName=="kaon-" || particleName=="mu-" || particleName=="mu+" ||  particleName=="proton" ||  particleName=="neutron")
    {
      
      fileOut->fout << std::setw(6) << particleName
		    << " TrID: " << std::setw(6) << trackID
		    << " E: " << std::setw(8) <<  G4BestUnit(EScint, "Energy") 
		    << " gt: " << std::setw(11) << G4BestUnit(GlobalTimeScint, "Time") 
		    << " x: "  << std::setw(6) << G4BestUnit(xScint, "Length")
		    << " y: "  << std::setw(6) << G4BestUnit(yScint, "Length")
		    << " z: "  << std::setw(6) << G4BestUnit(zScint, "Length")
		    << " px: "  << std::setw(6) << G4BestUnit(pxScint, "Energy")
		    << " py: "  << std::setw(6) << G4BestUnit(pyScint, "Energy")
		    << " pz: "  << std::setw(6) << G4BestUnit(pzScint, "Energy")
		    <<G4endl;;
    }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

