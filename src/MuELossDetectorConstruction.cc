// Created on May 12, 2010, Hexc, Kanishka
// Updated on June 21, 2010, Hexc, Kanishka
// Updated on 9/16/2014, Hexc, Olesya:  Added CLHEP namespace
// Updated on 10/31/2014, Hexc - fixing and cleaning up the code for compilation under Mac 
// Updated on 11/8/2014, Hexc - fixed a bug of character array : layer
// Updated on 11/29/2022, Hexc - fixing a sensitive detector isse
// Updated on 1/14/2025, hexc & Mia Boyd - Disable the visualization of the atm layers. 
//

#include "MuELossDetectorConstruction.hh"
#include "MuELossDetectorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UniformMagField.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "MuELossScintillatorSD.hh"

#include "G4RunManager.hh"

#include <math.h>

using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuELossDetectorConstruction::MuELossDetectorConstruction()
  :defaultMaterial(0), surfaceScintMaterial(0), cloudMaterial(0), NbOfLayers(0),
   solidWorld(0),logicWorld(0),physiWorld(0),
   solidCloudCover(0), logicCloudCover(0), physiCloudCover(0),
   WorldSizeZ(0.), WorldSizeXY(0.),
   upperATMHeight(0.), airLayerThickness(0.),CldLayerPos(-1),atmType(0), denPer(1.0),
   magField(0)
{
  // default parameter values of the atmospheric volume
  atmType = 0;
  denPer = 1.0;
  NbOfLayers  = NUMBER_LAYER;
  upperATMHeight = 1.e08*mm;
  airLayerThickness = 1.e06*mm;

  WorldSizeZ = 1.10*upperATMHeight;
  WorldSizeXY = 6.e07*mm;

  fileOut = MuELossOutput::instance();

  // create commands for interactive definition of the atmospheric layers
  detectorMessenger = new MuELossDetectorMessenger(this);

  G4cout << "Done with Detector Messenger instatiation !" << G4endl; 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuELossDetectorConstruction::~MuELossDetectorConstruction()
{ delete detectorMessenger;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuELossDetectorConstruction::DefineMaterials()
{ 
  
  G4NistManager* man = G4NistManager::Instance();
  man->SetVerbose(1);

  //This function illustrates the possible ways to define materials
  
  G4String symbol;             //a=mass of a mole;
  G4double a, z, density;      //z=mean number of protons;  
  
  G4int ncomponents, natoms;
  G4double fractionmass;
  
  //
  // define Elements
  //
  
  G4Element* H  = new G4Element("Hydrogen",symbol="H" , z= 1., a= 1.01*g/mole);
  G4Element* C  = new G4Element("Carbon"  ,symbol="C" , z= 6., a= 12.01*g/mole);
  G4Element* N  = new G4Element("Nitrogen",symbol="N" , z= 7., a= 14.01*g/mole);
  G4Element* O  = new G4Element("Oxygen"  ,symbol="O" , z= 8., a= 16.00*g/mole);
  //G4Element* Si = new G4Element("Silicon",symbol="Si" , z= 14., a= 28.09*g/mole);
  
  //
  // define a material from elements.   case 1: chemical molecule
  //

  // This is a fake cloud density.  Need a better definition.
  G4Material* H2O = new G4Material("Water", density= 1.000*mg/cm3, ncomponents=2);
  H2O->AddElement(H, natoms=2);
  H2O->AddElement(O, natoms=1);
  // overwrite computed meanExcitationEnergy with ICRU recommended value 
  H2O->GetIonisation()->SetMeanExcitationEnergy(75.0*eV);
  cloudMaterial= H2O;

  // Define the material:  BC408 plastic scintillator
  G4Material* Sci = new G4Material("Scintillator", density= 1.032*g/cm3, ncomponents=2);
  Sci->AddElement(C, natoms=10);
  Sci->AddElement(H, natoms=11);
  surfaceScintMaterial = Sci;
  
  //
  // define a material from elements.   case 2: mixture by fractional mass
  //
  
  G4Material* Air = new G4Material("Air"  , density= 1.290*mg/cm3, ncomponents=2);
  Air->AddElement(N, fractionmass=0.7);
  Air->AddElement(O, fractionmass=0.3);
  
  //
  // examples of gas in non STP conditions
  //
  
  G4Material* CO2 = 
    new G4Material("CarbonicGas", density= 1.842*mg/cm3, ncomponents=2,
		   kStateGas, 325.*kelvin, 50.*atmosphere);
  CO2->AddElement(C, natoms=1);
  CO2->AddElement(O, natoms=2);
  
  //G4Material* steam = 
  //  new G4Material("WaterSteam", density= 0.3*mg/cm3, ncomponents=1,
  //		   kStateGas, 500.*kelvin, 2.*atmosphere);
  //steam->AddMaterial(H2O, fractionmass=1.);
  
  //
  // examples of vacuum
  //
  
  G4Material* vac  = man->FindOrBuildMaterial("G4_Galactic");
  
  airLayer_Matt.clear();

  // Define air layer material
  
  // Need to define the air-density for each airlayer, i.e., fill the LayerDensity vector
  // Maximum number of layers should be less than 99.  Otherwise, one need to increase the array from 10 to 11 
  // for the Layer array.

  char Layer[11];  // Changed the index from 10 to 11 since it causes crashing on my Mac.
  G4Material* AirLayer[NUMBER_LAYER];
  G4double height, densityValue, temperatureValue, pressureValue;
 
  for (int iLayer = 0; iLayer<NbOfLayers; iLayer++)
    {
      // Make volume
      sprintf(Layer, "AirLayer%02d", iLayer);
      height = upperATMHeight - iLayer * airLayerThickness;

      densityValue = GetAirDensity(height,0,1);      
      //Xiaohang added following two lines to get temperature and pressure
      temperatureValue = GetAirDensity(height,2,0);
      pressureValue = GetAirDensity(height,1,0);
      
      //fileOut->fout  << "Height: " << height << "   Density: " << densityValue << G4endl;
      //AirLayer[iLayer] = new G4Material(Layer, density= densityValue*mg/cm3, ncomponents=2);
      //G4Material *AirLayer = new G4Material(Layer, density= densityValue*mg/cm3, ncomponents=2, kStateGas, temperatureValue*kelvin, pressureValue*pascal);
      AirLayer[iLayer] = new G4Material(Layer, density= densityValue*mg/cm3, ncomponents=2, kStateGas, temperatureValue*kelvin, pressureValue*pascal);
      AirLayer[iLayer]->AddElement(N, fractionmass=0.7);
      AirLayer[iLayer]->AddElement(O, fractionmass=0.3);
      airLayer_Matt.push_back(AirLayer[iLayer]);
      G4cout << "Height(km): " << height/(1.E6) << " iLayer= " << iLayer << "   Density(mg/cm^3): " << densityValue << "   Pressure(hPa): " << pressureValue<< "   Temperature(K): " << temperatureValue<< G4endl;
      
    }

  // The following line will print out the Material table defined.
  //  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
  
  defaultMaterial  = vac;

  //return EXIT_SUCCESS;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//G4VPhysicalVolume* MuELossDetectorConstruction::ConstructAtmosphere()
G4VPhysicalVolume* MuELossDetectorConstruction::Construct()
{  

  DefineMaterials();
 
  G4cout << "Define the world volume ... " << G4endl;
  //     
  // World
  //

  G4cout << "Constructing the world volume! " << G4endl;
  solidWorld = new G4Tubs("World", 0.0, WorldSizeXY/2, WorldSizeZ/2, 0.0*deg, 360.*deg);

  logicWorld = new G4LogicalVolume(solidWorld,		//its solid
                                  defaultMaterial,	//
                                   "World");		//its name
  
  physiWorld = new G4PVPlacement(0,			//no rotation
  				 G4ThreeVector(),	//at (0,0,0)
                                 "World",		//its name
                                 logicWorld,		//its logical volume				 
                                 0,			//its mother  volume
                                 false,			//no boolean operation
                                 0);			//copy number
  
  //                                 
  // Atmosphere Layers
  //
  
  char Layer[8];
  G4double tubHeight = upperATMHeight;
  G4double tubHeight_prev = 0.;
  G4double airLayerRadius = 0.;
  for (int iLayer = 0; iLayer<NbOfLayers; iLayer++)
    {
      // Make volume
      sprintf(Layer, "Layer%02d", iLayer);   // Maximum number of layer should be less than 99!!!
      tubHeight  = upperATMHeight - iLayer*airLayerThickness - iLayer*1.0*mm;
      airLayerRadius = 0.99*WorldSizeXY/2.0 - iLayer*10.0*mm;
      // 99% of the WorldSize radius and substracted by a small number to avoid overlap
      G4cout << "iLayer: " << iLayer << "  *** tubHeight " << tubHeight << "   *** radius " << airLayerRadius << G4endl;
      airLayer.push_back(new G4Tubs(Layer, 0.0, airLayerRadius, tubHeight/2, 0.0*deg, 360.*deg));
      
      // Define logical volume for the volume
      //sprintf(Layer_logic, "Logic_Layer%02d", iLayer);
      airLayer_logic.push_back(new G4LogicalVolume(airLayer[iLayer], airLayer_Matt[iLayer], Layer));
      
      if (iLayer == 0)
	{
	  airLayer_phys.push_back(new G4PVPlacement(0,	                                                // no rotation
						    G4ThreeVector(0, 0, -0.50*WorldSizeZ + tubHeight/2+1.0*cm),	//at (0,0,0)
						    Layer,		                                                                //its name
						    airLayer_logic[iLayer],	                                                //its logical volume     
						    physiWorld,	                                                                //its mother  volume
						    false,		                                                                //no boolean operation
						    0));		                                                                //copy number
	  tubHeight_prev = tubHeight;
	}
      else 
	{
	  airLayer_phys.push_back(new G4PVPlacement(0,	                                                        // no rotation
						    G4ThreeVector(0, 0, -0.5*tubHeight_prev + 0.5*tubHeight+1.0*cm),	//at (0,0,0)
						    Layer,		                                                                        //its name
						    airLayer_logic.at(iLayer),	                                                //its logical volume				     
						    airLayer_phys.at(iLayer-1),	                                                //its mother  volume
						    false,		                                                                        //no boolean operation
						    0));		                                                                        //copy number
	  tubHeight_prev = tubHeight;
	}
    }
  
  //****************************************************************************
  //Xiaohang commented out the following line to reduce the size of log file. 03/11/2014
  //PrintATMParameters();     
  
  //                                        
  // Visualization attributes
  //
  // logicWorld->SetVisAttributes(G4VisAttributes::Invisible);
  
  G4VisAttributes* simpleWorldVisAtt= new G4VisAttributes(G4Colour(0.,1.0,0.));
  simpleWorldVisAtt->SetVisibility(true);
  simpleWorldVisAtt->SetForceWireframe(true);

  logicWorld->SetVisAttributes(simpleWorldVisAtt);


  G4VisAttributes* simpleLayerVisAtt[NUMBER_LAYER];
  
  for (int iLayer = 0; iLayer < NbOfLayers; iLayer++)
    {
      //   simpleLayerVisAtt[iLayer] = new G4VisAttributes(G4Colour(1.0/(iLayer+1),1.0/(iLayer+1),1.0/(iLayer+1)));
      simpleLayerVisAtt[iLayer] = new G4VisAttributes(G4Colour(0.1, 0.1, (0.2 + 0.01*iLayer)));
      //simpleLayerVisAtt[iLayer] = new G4VisAttributes(G4Colour::Yellow);
      simpleLayerVisAtt[iLayer]->SetVisibility(false);//********************************turn off visulizition
      simpleLayerVisAtt[iLayer]->SetForceSolid(false);
      simpleLayerVisAtt[iLayer]->SetForceWireframe(false);
      (airLayer_logic[iLayer])->SetVisAttributes(simpleLayerVisAtt[iLayer]);
    }
  
  /*
  // Below are vis attributes that permits someone to test / play 
  // with the interactive expansion / contraction geometry system of the
  // vis/OpenInventor driver :
  {G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  simpleBoxVisAtt->SetVisibility(true);
  delete logicCalor->GetVisAttributes();
  logicCalor->SetVisAttributes(simpleBoxVisAtt);}
  
  {G4VisAttributes* atb= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  logicLayer->SetVisAttributes(atb);}
  
  {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  atb->SetForceSolid(true);
  logicAbsorber->SetVisAttributes(atb);}
  
  {//Set opacity = 0.2 then transparency = 1 - 0.2 = 0.8
  G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,0.0,1.0,0.2));
  atb->SetForceSolid(true);
  logicGap->SetVisAttributes(atb);}
  */
  
  auto solidScintNearEarthSurface = new G4Tubs("nearSurfaceScintillator", 0.0, 0.95*WorldSizeXY/2,  1.*cm, 0.0*deg, 360.*deg);
  
  logicScintNearEarthSurface = new G4LogicalVolume(solidScintNearEarthSurface,		//its solid
						   surfaceScintMaterial,	        //its material
						   "logicSurfaceScintillator");		//its name
  
  auto physiScintNearEarthSurface = new G4PVPlacement(0,			          //no rotation
						 //G4ThreeVector(0,0,-30.*km),    //at (0,0,0)
						 G4ThreeVector(0,0,0),	          //at (0,0,0)
						 "surfaceScintillator",	          //its name
						 logicScintNearEarthSurface,      //its logical volume				 
						 //airLayer_phys.at(0),  //its mother  volume
						 airLayer_phys.at(NbOfLayers-1),  //its mother  volume
						 false,			          //no boolean operation
						 0);			          //copy number
  
  G4VisAttributes* scintillatorColor = new G4VisAttributes(G4Colour(0.3, 0.5, 0.5 ));
  scintillatorColor->SetVisibility(true);
  scintillatorColor->SetForceWireframe(true);
  scintillatorColor->SetForceSolid(true);
  
  logicScintNearEarthSurface->SetVisAttributes(scintillatorColor);
  
  if(CldLayerPos==-1) {
  } 
  else {
    
    //Cloud Cover
    solidCloudCover = new G4Tubs("CloudCover", 0.0, 0.95*WorldSizeXY/2, 1000.*m, 0.0*deg, 360.*deg);
    
    logicCloudCover = new G4LogicalVolume(solidCloudCover,		//its solid
					  cloudMaterial,	                //its material
					  "logicCloudCover");		//its name
    
    physiCloudCover = new G4PVPlacement(0,			          //no rotation
					G4ThreeVector(),	          //at (0,0,0)
					"CloudCover",	          //its name
					logicCloudCover,      //its logical volume				 
					airLayer_phys.at((NbOfLayers-1)-CldLayerPos),  //its mother  volume
					false,			          //no boolean operation
					0);			          //copy number
    
    G4VisAttributes* cloudCoverColor = new G4VisAttributes(G4Colour(1.0, 1.0 , 1.0 ));
    cloudCoverColor->SetVisibility(true);
    cloudCoverColor->SetForceWireframe(false);
    cloudCoverColor->SetForceSolid(true);
    
    logicCloudCover->SetVisAttributes(cloudCoverColor);
    
  }

  
  //
  //always return the physical World
  //
  return physiWorld;
  
}

void MuELossDetectorConstruction::ConstructSDandField()
{
  
  // Implement the sensitive detector and add to the SD manager
  auto SDman = G4SDManager::GetSDMpointer();
  auto scintillatorSD = new MuELossScintillatorSD("/scintillatorSD", this);
  SDman->AddNewDetector(scintillatorSD);
  
  logicScintNearEarthSurface->SetSensitiveDetector(scintillatorSD); 
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuELossDetectorConstruction::PrintATMParameters()
{
  
  for (G4int i = 0; i<NbOfLayers; i++) {
    G4cout << "\n------------------------------------------------------------"
	   << "\n---> The atmospher layer " << i  << "  "
	   << airLayer[i]->GetZHalfLength() << " and Outer/Inner radius: "
	   << airLayer[i]->GetOuterRadius() << "  " << airLayer[i]->GetInnerRadius() 
	   << "\n------------------------------------------------------------\n";
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuELossDetectorConstruction::SetNbOfLayers(G4int val)
{
  NbOfLayers = val;
}


void MuELossDetectorConstruction::SetCloud(G4int LayerPos)
{
  CldLayerPos = LayerPos;
}

void MuELossDetectorConstruction::SetatmModel(G4int atmTypeVal)
{
  atmType = atmTypeVal;
}

void MuELossDetectorConstruction::SetDenPerc(G4double denPerVal)
{
  denPer = denPerVal;
}



//
// Here is the ATM model for determining the air density, provided by Kanishka.
// This model is modified to calculate density, temperature and pressure for input height, modified by Xiaohang
//

//make new model*****************************************************************
G4double MuELossDetectorConstruction::GetAirDensity(G4double hin, G4int option, G4int ofoption=0)
{
  G4double temp=0., press=0., density=0., dummy, T=0., Rho=0., R=287.05;  //R unit: J/(kg*K)
  G4int h=0, H=0, opt = option, of=ofoption, lCounter=0; //chose output parameters (temperature 2/pressure 1/density 0)
  G4String info;
  
  if(atmType==0){ // general atm model
    h = 0.001*hin;  // convert to meters
    //fileOut->fout  <<" General atm model is selected " << G4endl;
    if(h>25000){   
      temp = -131.21+(0.00299*h);
      press = 2.488*( pow( (temp+273.1)/216.6 , -11.388) );
      density = (press/( 0.2869*(temp+273.1) ));
      //G4cout << " In function 1: density = " << density << G4endl;
      //return density;
    } 
    else if(h>11000 && h<=25000){   
      //temp = -56.46;
      //lower-stratosphere temperature change
      //if(h>=14000 && h<=18000) temp = -56.46+(216.64*(0+0.00));
      //else temp = -56.46;
      //temp = -56.46+(216.64*(0.));
      temp = -56.46;
      press = 22.65*(exp(1.73-(0.000157*h)));
      density = (press/( 0.2869*(temp+273.1) ));
      //G4cout << " In function 2: density = " << density << G4endl;
      //return density;
    } 
    else if (h<=11000){   
      temp = 15.04-(0.00649*h);
      press = 101.29*( pow( (temp+273.1)/288.08 , 5.256) );
      density = (press/( 0.2869*(temp+273.1) ));
      //density = (press/( 0.2869*(temp+273.1) )) * denPer;
      //G4cout << " In function 3: density = " << density << G4endl;
      //return density;
    } 
    else {
      G4cout << "The atm hieght is WRONG! " << G4endl;
      exit(1);
    }
    if(opt==0) return density;  //mg/cm^3
    else if(opt==1) return (press*10.); //hPa
    else if(opt==2) return (temp+273.1); // Kelvin
    else {
      G4cout << "WRONG option!! " << G4endl;
      exit(1);
    } 
  }
  else if(atmType==1){ //Read atmospheric parameter from Atm_Input1.txt
    //fileOut->fout <<"Type 1 atmosphere is selected " << G4endl;
    fileIn = MuELossInput::instance();
    fileIn->Fopen("Atm_Input1.txt");
    h = 0.000001*hin;  // convert to kilometers
    //G4cout<<"  h "<<h<<endl;
    //******************************************************
    if(h<=100 && h>=0){ 
      if(of==1&&h<=2){
	for(G4int i=0;i<14;i++){
	  getline(fileIn->fin,info);
	  fileOut->fout  <<"OOOOOOOOOO"<<info<<"OOOOOOOOOO"<< G4endl;
	}
      }
      else{
	for(G4int i=0;i<14;i++){
	  getline(fileIn->fin,info);
	}
      }
      while(fileIn->fin >> H >> dummy >> Rho >> T){
	if(H>=h){
	  temp = T;  //Kelvin
	  density = Rho;  // g/cm^3
	  press = (double) 10.*temp*density*R; //hPa
	  //G4cout << " Match and Calculate: H(km) "<<h<<"; Rho(g/cm^3) "<< density<<"; T "<<temp<<"; P " <<press<< G4endl;
	  break;
	}
	else{
	  lCounter++;
	  continue;	  
	}
      }
      // Closing input file
      fileIn->Fclose();
      if(opt==0) return (density*1000.);
      else if(opt==1) return press;
      else if(opt==2) return temp;
      else {
	G4cout << "WRONG parameter option!! " << G4endl;
	exit(1);
      }
      
    }
    else {
      G4cout << "The atm hieght is WRONG! " << G4endl;
      exit(1);
    }
  }
  else if(atmType==2){ //read from file Atm_Input2.txt
    fileIn = MuELossInput::instance();
    fileIn->Fopen("Atm_Input2.txt");
    h = 0.000001*hin;  // convert to lilometers
    //******************************************************
    if(h<=100 && h>=0){
      while(fileIn->fin >> H >> dummy >> Rho >> T){
	if(H>=h){
	  temp = T;  //Kelvin
	  density = Rho;  // g/cm^3
	  press = (double) 10.*temp*density*R; //hPa
	  //G4cout << " Match and Calculate: H(km) "<<h<<"; Rho(g/cm^3) "<< density<<"; T "<<temp<<"; P " <<press<< G4endl;
	  break;
	}
	else{
	  lCounter++;
	  continue;	  
	}
      }
      // Closing input file
      fileIn->Fclose();
      //****************************************************
      if(opt==0) return (density*1000.);
      else if(opt==1) return press;
      else if(opt==2) return temp;
      else {
	G4cout << "WRONG parameter option!! " << G4endl;
	exit(1);
      }
      
    }
    else {
      G4cout << "The atm hieght is WRONG! " << G4endl;
      exit(1);
    }
  }  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

void MuELossDetectorConstruction::SetMagField(G4double fieldValue)
{
  /*
  //apply a global uniform magnetic field along Z axis
  G4FieldManager* fieldMgr
   = G4TransportationManager::GetTransportationManager()->GetFieldManager();

  if(magField) delete magField;		//delete the existing magn field

  if(fieldValue!=0.)			// create a new one if non nul
  { magField = new G4UniformMagField(G4ThreeVector(0.,0.,fieldValue));
    fieldMgr->SetDetectorField(magField);
    fieldMgr->CreateChordFinder(magField);
  } else {
    magField = 0;
    fieldMgr->SetDetectorField(magField);
  }
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuELossDetectorConstruction::UpdateGeometry()
{
  //  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructAtmosphere());
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
