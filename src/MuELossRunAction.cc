// Created on May 12, 2010, Hexc, Kanishka
// Modified on July 1, 2010
//
// Oct 6, 2015: Hexc, Olesya - Adding code to write root output for data
//    analysis.
//
// Nov 28, 2022: Hexc - updated the constructor and the call for G4AnalysisManager
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "MuELossRunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4AnalysisManager.hh"

//#include "g4root.hh"   // This was used in older version

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuELossRunAction::MuELossRunAction()
{
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);
  
  // Create analysis manager
  // The choice of the output format is done via the specified
  // file extension.
  auto analysisManager = G4AnalysisManager::Instance();
  
  // Create directories
  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);

  /*
  // Note: merging ntuples is available only with Root output
  // Open an output file 
  G4String fileName = "MuELoss_Shower.root";
  analysisManager->OpenFile(fileName);
  */
  
  // Creating ntuple
  
  analysisManager->CreateNtuple("MuELoss", "cosmicShower");
  analysisManager->CreateNtupleDColumn("pID");
  analysisManager->CreateNtupleDColumn("kE");
  analysisManager->CreateNtupleDColumn("x");
  analysisManager->CreateNtupleDColumn("y");
  analysisManager->CreateNtupleDColumn("z");
  analysisManager->CreateNtupleDColumn("altitude");
  analysisManager->CreateNtupleDColumn("px");
  analysisManager->CreateNtupleDColumn("py");
  analysisManager->CreateNtupleDColumn("pz");
  analysisManager->CreateNtupleDColumn("gTime");
  analysisManager->CreateNtupleDColumn("localTime");
  analysisManager->CreateNtupleDColumn("properTime");
  analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuELossRunAction::~MuELossRunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuELossRunAction::BeginOfRunAction(const G4Run* aRun)
{ 
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
    
  //initialize cumulative quantities
  //
  aveELoss = 0.;
  aveTrackLength = 0.; 
  aveELoss2 = 0.;
  aveTrackLength2 = 0.; 

  currentRun = aRun;

  // Get the reference pointer for writing the end of run info to the output file
  fileOut = MuELossOutput::instance();

  // Book histograms, ntuple
  
  // Create analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in Analysis.hh

  
  G4cout << "##### Create ECRS analysis manager " << "  " << this << G4endl;
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << " analysis manager" << G4endl;

  // Create directories 
  
  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  
  // Open an output file
  
  G4String fileName = "MuELoss_Shower.root";
  analysisManager->OpenFile(fileName);

  /*
  // Creating ntuple
  
  analysisManager->CreateNtuple("MuELoss", "cosmicShower");
  analysisManager->CreateNtupleDColumn("pID");
  analysisManager->CreateNtupleDColumn("kE");
  analysisManager->CreateNtupleDColumn("x");
  analysisManager->CreateNtupleDColumn("y");
  analysisManager->CreateNtupleDColumn("z");
  analysisManager->CreateNtupleDColumn("altitude");
  analysisManager->CreateNtupleDColumn("px");
  analysisManager->CreateNtupleDColumn("py");
  analysisManager->CreateNtupleDColumn("pz");
  analysisManager->CreateNtupleDColumn("gTime");
  analysisManager->CreateNtupleDColumn("localTime");
  analysisManager->CreateNtupleDColumn("properTime");
  analysisManager->FinishNtuple();
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuELossRunAction::fillPerEvent(G4double totalEnergyDeposit, G4double totalTrackLength)
{
  // Find the number of events being processed so far
  G4int NbOfEvents = currentRun->GetNumberOfEvent();

  // Calculate the average of ELoss and track length using the following formulum
  //     aveX_N = aveX_(N-1) + (x_N - aveX_(N-1))/(N+1) 
  // Note that N starts from zero.  For N = 0, aveX_(0-1) is ZERO.
  //
  aveELoss += (totalEnergyDeposit - aveELoss)/(NbOfEvents + 1);
  aveTrackLength += (totalTrackLength - aveTrackLength)/(NbOfEvents + 1);

  aveELoss2 += (totalEnergyDeposit*totalEnergyDeposit - aveELoss2)/(NbOfEvents + 1);
  aveTrackLength2 += (totalTrackLength*totalTrackLength - aveTrackLength2)/(NbOfEvents + 1);

  // G4cout << " Number of events processed: " << NbOfEvents << " Average ELoss:  " << aveELoss  << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuELossRunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  if (NbOfEvents == 0) return;
  
  //compute statistics: mean and rms
  //

  G4double rmsELoss = aveELoss2 - aveELoss*aveELoss;
  if (rmsELoss >0.) rmsELoss = std::sqrt(rmsELoss); else rmsELoss = 0.;

  G4double rmsTrackLength = aveTrackLength2 - aveTrackLength*aveTrackLength;
  if (rmsTrackLength >0.) rmsTrackLength = std::sqrt(rmsTrackLength); else rmsTrackLength = 0.;  
  
  //print
  //
  G4cout
     << "\n--------------------End of Run------------------------------\n"
     << "\n mean energy loss : " << G4BestUnit(aveELoss,"Energy")
     << " +- "                          << G4BestUnit(rmsELoss,"Energy")  
     << G4endl;
     
  G4cout
     << "\n mean trackLength : " << G4BestUnit(aveTrackLength,"Length")
     << " +- "                               << G4BestUnit(rmsTrackLength,"Length")  
     << "\n------------------------------------------------------------\n"
     << G4endl;
  /*
  // Write to output file
  fileOut->fout
     << "\n--------------------End of Run------------------------------\n"
     << "\n mean energy loss : " << G4BestUnit(aveELoss,"Energy")
     << " +- "                          << G4BestUnit(rmsELoss,"Energy")  
     << G4endl;
     
  fileOut->fout
     << " mean trackLength : " << G4BestUnit(aveTrackLength,"Length")
     << " +- "                               << G4BestUnit(rmsTrackLength,"Length")  
     << "\n------------------------------------------------------------\n"
     << G4endl;
  */
  
  auto analysisManager = G4AnalysisManager::Instance();
  
  // save histograms 
  
  analysisManager->Write();
  analysisManager->CloseFile();
  
  // complete cleanup

  // Commented out on Nov 28, 2020
  //  delete G4AnalysisManager::Instance();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
