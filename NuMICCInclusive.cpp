#include <iostream>
#include <ctime>

#include <TRandom1.h>
#include <TSystem.h>
#include <TBrowser.h>
#include <TApplication.h>
#include <TChain.h>

#include "SelectionTools.hpp"
#include "AnaNuMI.h"

using namespace std;


int main(int argc, char* argv[]) {

  clock_t begin = clock();

  TApplication* rootapp = new TApplication("example",&argc, argv);

  TChain *cflux;
  string pattern = "/data/t2k/lar/uboone/prodgenie_numi_nu_uboone_MCC7/prodgenie_numi_nu_cosmic_uboone_merged_gen_g4_detsim_reco1_reco2_ana.root";
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

  #pragma omp parallel num_threads(3)
  {
  #pragma omp for
  for(int i = 0; i < 101; i++) {

    if(i%100 == 0) cout << "\t... " << i << endl;

    cflux->GetEntry(i);
    //AnaNuMI * anatree = new AnaNuMI(cflux_n);
    SelectionTools * selection = new SelectionTools(anatree);

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
  }

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
