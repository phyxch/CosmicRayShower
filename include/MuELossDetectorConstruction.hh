// Created on May 12, 2010, Hexc, Kanishka
// Updated on 6/21/2010, Hexc, Kanishka
// Updated on 11/29/2022, Hexc - fixing an issue with sensitive detector

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef MuELossDetectorConstruction_h
#define MuELossDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include <vector>
#include "MuELossOutput.hh"
#include "MuELossInput.hh"

class G4Box;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4UniformMagField;
class MuELossDetectorMessenger;
class MuELossScintillatorSD;

#define NUMBER_LAYER 99

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MuELossDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  
  MuELossDetectorConstruction();
  ~MuELossDetectorConstruction();
  
public:
  
  void SetMagField(G4double);
  void SetNbOfLayers(G4int);
  void SetCloud(G4int); 
  void SetatmModel(G4int); 
  void SetDenPerc(G4double); 
 
  G4VPhysicalVolume* Construct() override;
  void ConstructSDandField() override;
  
  void UpdateGeometry();
  
public:
  
  void PrintATMParameters(); 
  
  G4double GetWorldSizeZ()           {return WorldSizeZ;}; 
  G4double GetWorldSizeXY()          {return WorldSizeXY;};
  
  G4int GetNbOfLayers()              {return NbOfLayers;}; 
 
  G4double GetAirDensity(G4double, G4int, G4int);
  
  const G4VPhysicalVolume* GetphysiWorld() {return physiWorld;}; 
  //  const G4VPhysicalVolume* GetScintillator() { return physiScintNearEarthSurface;};

private:
  
  G4Material*        defaultMaterial;
  G4Material*        surfaceScintMaterial;
  G4Material*        cloudMaterial;

  G4int              NbOfLayers;
  G4int              CldLayerPos;   // Cloud layer position
  G4int              atmType;   // atm model types, general, summer or winter    
 
  //std::vector<G4Material*> airLayer_Matt;
  //std::vector<G4VPhysicalVolume*> airLayer_phys;
  //std::vector<G4LogicalVolume*> airLayer_logic;
  //std::vector<G4Tubs*> airLayer;

  vector<G4Material*> airLayer_Matt;
  vector<G4VPhysicalVolume*> airLayer_phys;
  vector<G4LogicalVolume*> airLayer_logic;
  vector<G4Tubs*> airLayer;

  //std::vector<G4double> LayerHeight;
  //std::vector<G4double> LayerDensity;
  
  G4Tubs*            solidWorld;    //pointer to the solid World 
  G4LogicalVolume*   logicWorld;    //pointer to the logical World
  G4VPhysicalVolume* physiWorld;    //pointer to the physical World

  G4double           WorldSizeZ;
  G4double           WorldSizeXY;

  G4double upperATMHeight, airLayerThickness;
  G4double              denPer;

  //G4Tubs*  solidScintNearEarthSurface;             //pointer to the earth surface scintillator
  G4LogicalVolume*   logicScintNearEarthSurface;   //pointer to the logical surface scintillator
  //G4VPhysicalVolume* physiScintNearEarthSurface;   //pointer to the physical surface scintillator
  
  G4Tubs*  solidCloudCover;             //pointer to the solid cloud cover
  G4LogicalVolume*   logicCloudCover;   //pointer to the logical cloud cover
  G4VPhysicalVolume* physiCloudCover;   //pointer to the physical cloud cover

  MuELossDetectorMessenger* detectorMessenger;  //pointer to the Messenger
  
  //  MuELossScintillatorSD* scintillatorSD;  // Pointer to the sensitive detector near the Earth surface.

  G4UniformMagField* magField;      //pointer to the magnetic field
  
  MuELossOutput*  fileOut;
  //Xiaohang added the following lines to read in the atmospheric parameters
  MuELossInput*  fileIn;

private:
  
  void DefineMaterials();
  
  // G4VPhysicalVolume* ConstructAtmosphere();     
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
