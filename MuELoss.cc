//   This is the main program for running simulations of muons passing 
//   atmospere.

//   Date created:  5/12/2010, hexc, Kanishka
//   Updated on:  6/21/2010, hexc, Kanishka
//   Updated: remove physics list, use FTFP_BERT.hh, 11/20/2013, xiaohang
//
//   Updated: Nov 26, 2018, hexc
//            Modified the code to use Qt for event/detector display
//
//   Updated: Nov 29, 2020, hexc
//           Updating the code for running multi-threaded mode
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/*
#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif
*/

#include "G4RunManagerFactory.hh"


//#include "G4RunManager.hh"

#include "Randomize.hh"

#include "MuELossDetectorConstruction.hh"

//#include "MuELossPhysicsList.hh"
//#include "FTFP_BERT.hh"
//#include "QGSP_BERT.hh"
#include "QGSP_BERT_HP.hh"

#include "MuELossPrimaryGeneratorAction.hh"
#include "MuELossRunAction.hh"
#include "MuELossEventAction.hh"
#include "MuELossSteppingAction.hh"
#include "MuELossSteppingVerbose.hh"
#include "MuELossTrackingAction.hh"
#include "MuELossActionInitialization.hh"

#include "MuELossOutput.hh"
#include "MuELossInput.hh"

#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


namespace {
  void PrintUsage() {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " MuELoss [-m macro ] [-u UIsession] [-t nThreads]" << G4endl;
    G4cerr << "   note: -t option is available only for multi-threaded mode."
           << G4endl;
  }
}

int main(int argc,char** argv)
{
  // Create the Output file object (a singleton object)
  MuELossOutput*  myOut = MuELossOutput::instance();

  if (argc==4)  myOut->Fopen(argv[2]); // batch mode
  if (argc==2)  myOut->Fopen("MuELoss_Output.txt"); // batch mode
  if (argc==1) {
    //    ui = new G4UIExecutive(argc, argv);
    myOut->Fopen("MuELoss_Output.txt"); // interactive mode : define visualization UI terminal
  }
  
  // Choose the Random engine
  //
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  //xiaohang added three lines to change the random engine seed, hopefully it will work.
  G4long seed=time(0); 
  CLHEP::HepRandom::setTheSeed(seed);
  if (argc == 4) CLHEP::HepRandom::setTheSeed(seed+atol(argv[3]));
  CLHEP::HepRandom::showEngineStatus();

  // User Verbose output class
  //
  G4VSteppingVerbose::SetInstance(new MuELossSteppingVerbose);
     
  // Construct the default run manager
  //
  /*
    #ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
    G4cout << "<<<<<<Running multi-threaded mode.>>>>>>" << G4endl;
    runManager->SetNumberOfThreads(2);
    #else
    G4RunManager* runManager = new G4RunManager;
    G4cout << "<<<<<<Running single-threaded mode.>>>>>>" << G4endl;
    #endif
  */

  G4String macro;
  G4String session;
#ifdef G4MULTITHREADED
  G4int nThreads = 4;
#endif
  for ( G4int i=1; i<argc; i=i+2 ) {
    if      ( G4String(argv[i]) == "-m" ) macro = argv[i+1];
    else if ( G4String(argv[i]) == "-u" ) session = argv[i+1];
#ifdef G4MULTITHREADED
    else if ( G4String(argv[i]) == "-t" ) {
      nThreads = G4UIcommand::ConvertToInt(argv[i+1]);
    }
#endif
    else {
      PrintUsage();
      return 1;
    }
  }

  // Detect interactive mode (if no macro provided) and define UI session
  //
  G4UIExecutive* ui = nullptr;
  if ( ! macro.size() ) {
    ui = new G4UIExecutive(argc, argv, session);
  }
  
  // Construct the default run manager
  //
  auto* runManager =
    G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
#ifdef G4MULTITHREADED
  if ( nThreads > 0 ) { 
    runManager->SetNumberOfThreads(nThreads);
  }  
#endif
  
  // Set mandatory initialization classes
  //
  MuELossDetectorConstruction* detector = new MuELossDetectorConstruction;
  runManager->SetUserInitialization(detector);
  
  // Pick a physics process list
  //  G4VModularPhysicsList* physicsList = new FTFP_BERT;
  G4VModularPhysicsList* physicsList = new QGSP_BERT_HP;
  runManager->SetUserInitialization(physicsList);

  // Set user action initialization
  //
  runManager->SetUserInitialization(new MuELossActionInitialization(detector));
  
  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
  
  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  
  // Process macro or start UI session
  //
  if ( ! ui ) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  delete visManager;
  delete runManager;
  
  // Closing output file
  myOut->Fclose();
  
  return 0;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
