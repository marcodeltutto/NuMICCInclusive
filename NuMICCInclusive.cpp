#include <iostream>
#include <ctime>

#include <TRandom1.h>
#include <TSystem.h>
#include <TBrowser.h>
#include <TApplication.h>
#include <TChain.h>
#include "TThread.h"

#include "SelectionTools.hpp"
#include "AnaNuMI.h"
#include "Spectrum.hpp"

using namespace std;



int main(int argc, char* argv[]) {

  clock_t begin = clock();

  TApplication* rootapp = new TApplication("ROOT Application",&argc, argv);

  string pattern = "/data/t2k/lar/uboone/prodgenie_numi_nu_uboone_MCC7/prodgenie_numi_nu_cosmic_uboone_merged_gen_g4_detsim_reco1_reco2_ana.root";

  bool evalPOT = true;
  double totalPOT = 0.;
  if (evalPOT) {

    cout << " ----- " << endl;
    cout << "| Calculating simulated POT." << endl;
    cout << "| " << endl;
   
    TChain *cpot;
    cpot = new TChain("analysistree/pottree");  
    cpot->Add(pattern.c_str());
    cout << "| Number of entries in the pot tree: " << cpot->GetEntries() << endl;
    Double_t pot;
    cpot->SetBranchAddress("pot", &pot);
    for (int potEntry = 0; potEntry < cpot->GetEntries(); potEntry++) {
      cpot->GetEntry(potEntry);
      totalPOT += pot;
    } // end loop entries
    cout << "| Simulated POT: " << totalPOT << endl;
    cout << " ----- " << endl << endl;
  } // end if evalPOT

  TChain *cflux;
  cflux = new TChain("analysistree/anatree");
  cflux->Add(pattern.c_str());

  int Nfiles = cflux->GetNtrees();
  cout << "Number of files: " << Nfiles << endl;

  int evts = cflux -> GetEntries();
  cout << "Number of events used is: " << evts << endl;

  AnaNuMI * anatree = new AnaNuMI(cflux);

  std::string TrackProdName  = "pandoraNu";
  std::string VertexProdName = "pandoraNu";

  int selectedEvents = 0.;
  int counter = 0.;  

  Spectrum* Sflashtime      = new Spectrum("flash_time",      3000, -3000, 3000, totalPOT);
  Spectrum* Sflashpe        = new Spectrum("flash_pe",        1000, 0, 100000, totalPOT);
  Spectrum* Sflashycenter   = new Spectrum("flash_ycenter",   1000, -150, 200, totalPOT);
  Spectrum* Sflashzcenter   = new Spectrum("flash_zcenter",   1000, 0, 15000, totalPOT);
  Spectrum* Sflashtimewidth = new Spectrum("flash_timewidth", 1000, 0, 0.6, totalPOT);


  for(int i = 0; i < 201; i++) {

    if(i%100 == 0) cout << "\t... " << i << endl;

    cflux->GetEntry(i);
    SelectionTools * selection = new SelectionTools(anatree);

    // ************************
    //
    // Preliminary distributions
    //
    // ************************

    // Flashes
    for (int fls = 0; fls < anatree->no_flashes; fls++) {
      Sflashtime      ->Fill(anatree->flash_time[fls]);
      Sflashpe        ->Fill(anatree->flash_pe[fls]);
      Sflashycenter   ->Fill(anatree->flash_ycenter[fls]);
      Sflashzcenter   ->Fill(anatree->flash_zcenter[fls]);
      Sflashtimewidth ->Fill(anatree->flash_timewidth[fls]);
    }

    
    // ************************
    //
    //  Selection
    //
    // ************************

    int theFlash = -1;
    if (selection->FlashTag(theFlash) == false) continue;

    counter++;
 
    // Create vertex-track association
    std::map< int,std::vector<int> > VertexTrackCollection;
    selection->CreateVertexTrackAssociation(VertexTrackCollection);  

    // Select the most forward going vertex-track association
    int vertexCandidate = -1;
    selection->SelectVertexTrackForward(VertexTrackCollection, vertexCandidate);

    // Check if vertex candidate is contained in FV
    if (!selection->InFV("vertex", vertexCandidate)) continue;

    // Get the longest track associated with the best vertex
    int trackCandidate;
    trackCandidate = selection->GetBestTrack(vertexCandidate, VertexTrackCollection);

    if (trackCandidate == -1) continue;

    // Flash matching
    if (!selection->IsFlashMatched(trackCandidate, theFlash)) continue;

    // Track must be contained
    if (!selection->InFV("track", trackCandidate)) continue;

    // Minimum track length
    if (!selection->IsLongTrack(trackCandidate)) continue;

    selectedEvents++;
 
    delete selection;
  }

  Sflashtime      ->Save();
  Sflashpe        ->Save();
  Sflashycenter   ->Save();
  Sflashzcenter   ->Save();
  Sflashtimewidth ->Save();

  cout << endl << endl << "********************************" << endl;
  cout << "Total events:     " << evts << endl;
  cout << "Selected Events:  " << selectedEvents << endl;
  cout << "Ratio:            " << (double)selectedEvents/(double)evts << endl << endl << endl;
  cout << "counter " << counter << endl;







  //rootapp->Run();
  //rootapp->Terminate(0);

  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  cout << endl << endl << "Computing time: " << elapsed_secs << " seconds." << endl << endl;
  return 0;
}
