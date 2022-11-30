// Created on 7/16/2010  Kanishka, David, Hexc
//
// This code is used for storing hits info in the sensitive detector. 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef MuELossScintHit_h
#define MuELossScintHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "MuELossOutput.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class MuELossScintHit : public G4VHit
{
  public:
  
  MuELossScintHit();
  ~MuELossScintHit();
  MuELossScintHit(const MuELossScintHit&);
  const MuELossScintHit& operator=(const MuELossScintHit&);
  int operator==(const MuELossScintHit&) const;
  
  inline void* operator new(size_t);
  inline void  operator delete(void*);
  
  void Draw();
  void Print();
  
public:
  
  void AddScintHit( G4double e,  G4double gt, G4double x, G4double y, G4double z,
		    G4double px, G4double py, G4double pz, G4int id,  G4String name) 
  {
    EScint = e; GlobalTimeScint = gt; 
    particleName = name;
    xScint = x;
    yScint = y;
    zScint = z;
    pxScint = px;
    pyScint = py;
    pzScint = pz;
    trackID = id;
  }
  
  G4double GetEnergy() 
  { 
    return EScint ; 
  }
  
  G4double GetGlobalTime()  
  {  
    return GlobalTimeScint ; 
  }

  G4String GetParticleName()
  {
    return particleName;
  }

  
private:
  
  G4double EScint, GlobalTimeScint;
  G4double xScint, yScint, zScint;
  G4double pxScint, pyScint, pzScint;
  G4int trackID;
  G4String particleName;
  
  MuELossOutput*  fileOut;
};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

typedef G4THitsCollection<MuELossScintHit> MuELossScintHitsCollection;

extern G4Allocator<MuELossScintHit> MuELossScintHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void* MuELossScintHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*) MuELossScintHitAllocator.MallocSingle();
  return aHit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void MuELossScintHit::operator delete(void* aHit)
{
  MuELossScintHitAllocator.FreeSingle((MuELossScintHit*) aHit);
}

#endif
