//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file electromagnetic/TestEm17/src/SteppingAction.cc
/// \brief Implementation of the SteppingAction class
//
// $Id: SteppingAction.cc 67268 2013-02-13 11:38:40Z ihrivnac $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SteppingAction.hh"
#include "RunAction.hh"
#include "HistoManager.hh"
#include "G4SystemOfUnits.hh"

#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(RunAction* RuAct, HistoManager* Hist)
  :G4UserSteppingAction(),fRunAction(RuAct), fHistoManager(Hist)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  if (aStep->GetTrack()->GetParentID() == 0) {
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    G4StepPoint *endPoint = aStep->GetPostStepPoint();
    G4String procName = endPoint->GetProcessDefinedStep()->GetProcessName();

    fRunAction->CountProcesses(procName);
    G4StepPoint *startPoint = aStep->GetPreStepPoint();
    G4double E0 = startPoint->GetKineticEnergy();
    G4double E2 = endPoint->GetKineticEnergy();
    G4double totalEnergyDeposit = E0 - E2;
    G4double stepLength = aStep->GetStepLength();
    G4double deDx = 0.;
    if (totalEnergyDeposit > 0.)
      deDx = (totalEnergyDeposit / MeV) * (cm / stepLength);

    G4int id = 0;
    if ((procName == "muIoni") || (procName == "muPairProd") || (procName == "muBrems") || (procName == "muonNuclear"))
      id = 1;
    fHistoManager->FillHisto(id, deDx);

    if (totalEnergyDeposit > 0.0 && stepLength > 0.0) {
      analysisManager->FillNtupleDColumn(0, (E0 + E2) / 2.0 / MeV);
      analysisManager->FillNtupleDColumn(1, deDx);
      analysisManager->AddNtupleRow();
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
