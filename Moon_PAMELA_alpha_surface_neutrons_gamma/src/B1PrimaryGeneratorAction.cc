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
//
/// \file B1PrimaryGeneratorAction.cc
/// \brief Implementation of the B1PrimaryGeneratorAction class

#include "B1PrimaryGeneratorAction.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include <iostream>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1PrimaryGeneratorAction::B1PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0)
//  fEnvelopeBox(0)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="alpha");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,-1,0));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1PrimaryGeneratorAction::~B1PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
//68-bin PAMELA alpha spectrum between 0.14 and 52 GeV by AIT
double A[]= {0, 0.031688464,
0.036361417,
0.04103437,
0.043808936,
0.045999383,
0.046729532,
0.047021591,
0.046145412,
0.044977174,
0.044100995,
0.042494668,
0.040596281,
0.038551864,
0.036930933,
0.035295399,
0.033207173,
0.030958315,
0.028928501,
0.027103128,
0.025365374,
0.023379369,
0.021729232,
0.019787036,
0.018312135,
0.016851837,
0.015391539,
0.014048065,
0.012719194,
0.011726192,
0.010616365,
0.009667172,
0.008761787,
0.007914814,
0.007170062,
0.006454517,
0.005592941,
0.004497717,
0.003655126,
0.002932278,
0.002362762,
0.001901308,
0.001502646,
0.001195984,
0.000955035,
0.000765196,
0.000598722,
0.000476057,
0.000379677,
0.000294396,
0.000231895,
0.000185458,
0.000142525,
0.000111275,
8.91E-05,
6.80E-05,
5.37E-05,
4.18E-05,
3.29E-05,
2.61E-05,
1.99E-05,
1.52E-05,
1.18E-05,
9.20E-06,
7.30E-06,
5.55E-06,
4.23E-06,
3.50E-06,
2.48E-06,
}; 
//Discrete energies
double B[]= {0.0, 140,
150,
170,
180,
200,
210,
230,
250,
270,
300,
320,
350,
380,
410,
440,
470,
510,
550,
600,
640,
690,
740,
800,
850,
920,
980,
1050,
1120,
1200,
1280,
1370,
1460,
1560,
1660,
1770,
1940,
2190,
2460,
2770,
3100,
3470,
3880,
4330,
4820,
5360,
5960,
6610,
7330,
8120,
8980,
9930,
10980,
12120,
13370,
14750,
16260,
17910,
19730,
21720,
23900,
26290,
28920,
31800,
34950,
38420,
42210,
46370,
52120};
double Grid[69];
double sum=0;
  for(int x=0; x < sizeof(Grid)/sizeof(Grid[0]); x++){
  sum=sum+A[x];
  Grid[x]=sum;
  std::ofstream GridFile;
  GridFile.open("Probability_grid.txt", std::ios::app);
  GridFile <<  Grid[x] << G4endl;
  GridFile.close();
  }  
  for (int n_particle = 1; n_particle < 100000; n_particle++){
  G4double y0 = 3*m;
  G4double z0 = 10*cm;
  G4double x0 = 10*cm; 
  x0 = -x0+2*x0*G4UniformRand();
  z0 = -z0+2*z0*G4UniformRand();
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  G4double Energy; //Just for initialization
  G4double pseudo=G4UniformRand();
  for (int i=0; i < sizeof(Grid)/sizeof(Grid[0]); i++){
  if(pseudo > Grid[i] && pseudo <= Grid[i+1]){
  Energy=B[i+1];
  std::ofstream EnergyFile;
  EnergyFile.open("Energy.txt", std::ios::app);
  EnergyFile <<  Energy << G4endl;
  EnergyFile.close();
  }
  }   
  fParticleGun->SetParticleEnergy(Energy);
  fParticleGun->GeneratePrimaryVertex(anEvent);
 }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

