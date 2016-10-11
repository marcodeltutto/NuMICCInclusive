#include <iostream>
#include <string>
#include <ctime>
#include <vector>

#include <TRandom1.h>
#include <TSystem.h>
#include <TBrowser.h>
#include <TApplication.h>
#include <TChain.h>
#include "TThread.h"
#include "THStack.h"
#include "TLegend.h"

#include "SelectionTools.hpp"
#include "AnaNuMI.h"
#include "Spectrum.hpp"
#include "Spectrum2D.hpp"
#include "PlotHandler.hpp"

using namespace std;

bool isCosmic = false;
bool isSignal = false;
bool trackCandidatePandoraCosmicNotContained = false;
int cosmicBkg=0;
int Signal=0;
int cosmicBkgAndNoCosmicTrack=0;
int signalAndNoCosmicTrack=0;
int cosmicBkgAndCosmicTrackNotContained=0;
int signalAndCosmicTrackNotContained=0;

//____________________________________________________________________________________________________
void ActivateBranches(AnaNuMI *anatree) {

  anatree->fChain->SetBranchStatus("*",0);
  anatree->fChain->SetBranchStatus("ccnc_truth",1);
  anatree->fChain->SetBranchStatus("nuPDG_truth",1);
  anatree->fChain->SetBranchStatus("nuvtxx_truth",1);
  anatree->fChain->SetBranchStatus("nuvtxy_truth",1);
  anatree->fChain->SetBranchStatus("nuvtxz_truth",1);
  anatree->fChain->SetBranchStatus("enu_truth",1);
  anatree->fChain->SetBranchStatus("flash_time",1);
  anatree->fChain->SetBranchStatus("flash_pe",1);
  anatree->fChain->SetBranchStatus("flash_ycenter",1);
  anatree->fChain->SetBranchStatus("flash_zcenter",1);
  anatree->fChain->SetBranchStatus("flash_timewidth",1);
  anatree->fChain->SetBranchStatus("geant_list_size",1);
  anatree->fChain->SetBranchStatus("origin",1);
  anatree->fChain->SetBranchStatus("process_primary",1);
  anatree->fChain->SetBranchStatus("StartT",1);
  anatree->fChain->SetBranchStatus("no_flashes",1);
  anatree->fChain->SetBranchStatus("nvtx_pandoraNu",1);
  anatree->fChain->SetBranchStatus("ntracks_pandoraNu",1);
  anatree->fChain->SetBranchStatus("vtxx_pandoraNu",1);
  anatree->fChain->SetBranchStatus("vtxy_pandoraNu",1);
  anatree->fChain->SetBranchStatus("vtxz_pandoraNu",1);
  anatree->fChain->SetBranchStatus("trkendx_pandoraNu",1);
  anatree->fChain->SetBranchStatus("trkendy_pandoraNu",1);
  anatree->fChain->SetBranchStatus("trkendz_pandoraNu",1);
  anatree->fChain->SetBranchStatus("trkstartx_pandoraNu",1);
  anatree->fChain->SetBranchStatus("trkstarty_pandoraNu",1);
  anatree->fChain->SetBranchStatus("trkstartz_pandoraNu",1);
  anatree->fChain->SetBranchStatus("trktheta_pandoraNu",1);
  anatree->fChain->SetBranchStatus("trkendx_pandoraCosmic",1);
  anatree->fChain->SetBranchStatus("trkendy_pandoraCosmic",1);
  anatree->fChain->SetBranchStatus("trkendz_pandoraCosmic",1);
  anatree->fChain->SetBranchStatus("trkstartx_pandoraCosmic",1);
  anatree->fChain->SetBranchStatus("trkstarty_pandoraCosmic",1);
  anatree->fChain->SetBranchStatus("trkstartz_pandoraCosmic",1);
  anatree->fChain->SetBranchStatus("trkpidbestplane_pandoraNu",1);
  anatree->fChain->SetBranchStatus("trkorigin_pandoraNu",1);
  anatree->fChain->SetBranchStatus("vx_flux",1);
  anatree->fChain->SetBranchStatus("vy_flux",1);
  anatree->fChain->SetBranchStatus("vz_flux",1);
  anatree->fChain->SetBranchStatus("ntrkhits_pandoraNu",1);
  anatree->fChain->SetBranchStatus("trktheta_pandoraNu",1);
  //anatree->fChain->SetBranchStatus("",1);
}


double CalcLength(const double& x_1, const double& y_1, const double& z_1, const double& x_2, const double& y_2, const double& z_2) {
    return sqrt(pow(x_1-x_2, 2) + pow(y_1-y_2, 2) + pow(z_1-z_2, 2));
}

//____________________________________________________________________________________________________
//____________________________________________________________________________________________________
//____________________________________________________________________________________________________
int main(int argc, char* argv[]) {

  clock_t begin = clock();

  if (argc != 4) {
    cout << "Provide 3 arguments!" << endl;
    cout << "./NuMICCInclusive bnb pot maxentries." << endl;
    exit(0);
  }

  string beam       = argv[1];
  string dopot      = argv[2];
  int    maxEntries = atoi(argv[3]);
  cout << endl << "Working with the " << beam << " beam." << endl;

  TApplication* rootapp = new TApplication("ROOT Application",&argc, argv);
  gROOT->SetBatch(kTRUE);
  gROOT->ProcessLine( "gErrorIgnoreLevel = 2001;"); // 1001: INFO, 2001: WARNINGS, 3001: ERRORS

  string pattern;
  if(beam == "numi") pattern = "/data/t2k/lar/uboone/prodgenie_numi_nu_uboone_MCC7/prodgenie_numi_nu_cosmic_uboone_merged_gen_g4_detsim_reco1_reco2_ana.root";
  //if(beam == "bnb")  pattern = "/pnfs/uboone/persistent/users/aschu/MC_BNB_Cosmic/prodgenie_bnb_nu_cosmic_uboone_v05_08_00_anatree.root";
  if(beam == "bnb")  pattern = "~/pnfs/aschu/MC_BNB_Cosmic/prodgenie_bnb_nu_cosmic_uboone_v05_08_00_anatree.root";

  bool evalPOT = false;
  double totalPOT = 0.;
  if (dopot == "pot") evalPOT = true;
  if (maxEntries > 0) evalPOT = false;

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
  else totalPOT = -1.;

  TChain *cflux;
  cflux = new TChain("analysistree/anatree");
  cflux->Add(pattern.c_str());

  cout << "Using file: " << pattern << endl;

  int Nfiles = cflux->GetNtrees();
  cout << "Number of files: " << Nfiles << endl;

  int evts = cflux -> GetEntries();
  cout << "Number of events used is: " << evts << endl;

  AnaNuMI * anatree = new AnaNuMI(cflux);
  ActivateBranches(anatree);

  std::string TrackProdName  = "pandoraNu";
  std::string VertexProdName = "pandoraNu";

  //Create a new file + a clone of old tree in new file
  TFile *oldfile = new TFile(pattern.c_str());
  TTree *oldtree = (TTree*)oldfile->Get("analysistree/anatree");
  TFile *newfile = new TFile("selectedEntries.root","recreate");
  TTree *newtree = oldtree->CloneTree(0);


  int selectedEvents = 0.;
  int counter = 0.;  
  int EventsWithFlash = 0, EventsVtxInFV = 0, EventsFlashMatched = 0, EventsTracksInFV = 0, EventsTrackLong = 0;
  int noHitsOnUplane = 0, noHitsOnVplane = 0, noHitsOnWplane = 0;

  Spectrum* Sflashtime      = new Spectrum("flash_time",      ";Flash Time [#mus];Entries per bin",       300000, -3000, 3000, totalPOT);
  Spectrum* Sflashtime50pe  = new Spectrum("flash_time_50",   ";Flash Time [#mus];Entries per bin",       300000, -3000, 3000, totalPOT);    
  Spectrum* Sflashpe        = new Spectrum("flash_pe",        ";Flash PE;Entries per bin",                1000, 0, 100000, totalPOT);
  Spectrum* Sflashycenter   = new Spectrum("flash_ycenter",   ";Flash Y Center [cm];Entries per bin",     1000, -150, 200, totalPOT);
  Spectrum* Sflashzcenter   = new Spectrum("flash_zcenter",   ";Flash Z Center [cm];Entries per bin",     1000, 0, 15000, totalPOT);
  Spectrum* Sflashtimewidth = new Spectrum("flash_timewidth", ";Flash Time Width [#mus];Entries per bin", 1000, 0, 0.6, totalPOT);

  Spectrum* Sgeanttruetime_neutrino  = new Spectrum("geant_true_time_neutrino", ";Geant True Time [ns];Entries per bin",    30000, 0., 20000, totalPOT);
  Spectrum* Sgeanttruetime_cosmic    = new Spectrum("geant_true_time_cosmic",   ";Geant True Time [ns];Entries per bin",    30000, -5000000, 4000000, totalPOT);

  Spectrum2D* Sfls_timeVSpe = new Spectrum2D("fls_timeVSpe",  ";Flash Time [#mus];Flash PE",             2000, -10, 40, 5000, 0, 5000, totalPOT);

  Spectrum2D* Sfls_timeVSnu_distance = new Spectrum2D("fls_timeVSnu_distance", ";Flash Time [#mus];#nu flight path [cm]",100,0,30,1000,0,80000, totalPOT);

  Spectrum* Strk_range = new Spectrum("trk_range", ";Track Range [cm]; Selected Events",20,0,1000, totalPOT);

  // This is a map: cutname <-> spectrum array
  //std::map<std::string,CutPlots> cutToPlotsMap;
  PlotHandler ph;
  ph.InstantiateIntermidiatePlots(totalPOT);

  if(maxEntries > 0.) evts = maxEntries;

  for(int i = 0; i < evts; i++) {

    if(i%100 == 0) cout << "\t... " << i << endl;

    cflux->GetEntry(i);
    SelectionTools * selection = new SelectionTools(anatree);

    //cout << "***** Event " << i << endl;

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
      if (anatree->flash_pe[fls] > 50) {
        Sflashtime50pe->Fill(anatree->flash_time[fls]);
        int k = 0;
        double distance = sqrt(pow(54.999*100-anatree->vx_flux[k],2)+pow(74.461*100-anatree->vy_flux[k],2)+pow(677.611*100-anatree->vz_flux[k],2)); 
        Sfls_timeVSnu_distance->Fill(anatree->flash_time[fls],distance);
      }
      Sfls_timeVSpe->Fill(anatree->flash_time[fls], anatree->flash_pe[fls]);
    }
    bool doneForThisEvent = false;
    bool doneForThisEvent_cosmic = false;
    for (int geantpar = 0; geantpar < anatree->geant_list_size; geantpar++) {
      if (!doneForThisEvent && anatree->origin[geantpar] == 1 && anatree->process_primary[geantpar]==1) {
        Sgeanttruetime_neutrino->Fill(anatree->StartT[geantpar]);
        doneForThisEvent = true;
      }
      if (!doneForThisEvent_cosmic &&anatree->origin[geantpar] == 2 && anatree->process_primary[geantpar]==1) {
        Sgeanttruetime_cosmic->Fill(anatree->StartT[geantpar]);
        doneForThisEvent_cosmic = true;
      }
    }

    
    // ************************
    //
    //  Selection
    //
    // ************************

    isCosmic = isSignal = trackCandidatePandoraCosmicNotContained = false;

    // Flash tag
    int theFlash = -1;
    if (selection->FlashTag(theFlash) == false) continue;
    EventsWithFlash++;
    std::string cutname = "flashtag";
    ph.MakeIntermidiatePlots(cutname,anatree,-1);

    // Create vertex-track association
    std::map< int,std::vector<int> > VertexTrackCollection;
    selection->CreateVertexTrackAssociation(VertexTrackCollection);  

    // Select the most forward going vertex-track association
    int vertexCandidate = -1;
    selection->SelectVertexTrackForward(VertexTrackCollection, vertexCandidate);

    //cout << "                                        Number of vertices: " << anatree->nvtx_pandoraNu << endl;

    // Check if vertex candidate is contained in FV
    if (!selection->InFV("vertex", vertexCandidate)) continue;
    EventsVtxInFV++;
    ph.MakeIntermidiatePlots("vertexcontained",anatree,-1);

    // Get the longest track associated with the best vertex
    int trackCandidate;
    trackCandidate = selection->GetBestTrack(vertexCandidate, VertexTrackCollection);

    if (trackCandidate == -1) continue;
    ph.MakeIntermidiatePlots("selectbesttrack",anatree,trackCandidate);
    //cout << "Best vertex is " << vertexCandidate << "   Best track is " << trackCandidate << endl;

    // Flash matching
    if (!selection->IsFlashMatched(trackCandidate, theFlash)) continue;
    EventsFlashMatched++;
    ph.MakeIntermidiatePlots("flashmatch",anatree,trackCandidate);

    // Track must be contained
    if (!selection->InFV("track", trackCandidate)) continue;
    EventsTracksInFV++;
    ph.MakeIntermidiatePlots("trackcontained",anatree,trackCandidate);

    // Check with pandoraCosmic
    int trackCandidatePandoraCosmic = selection->GetEquivalentTrackWithPandoraCosmic(trackCandidate);
    //cout << "{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}   trackCandidatePandoraCosmic is " << trackCandidatePandoraCosmic << endl;
    //if (selection->InFV("trackCosmic", trackCandidatePandoraCosmic)) cout << "Is contained!" << endl;
    if(trackCandidatePandoraCosmic > -1 && !selection->InFV("trackCosmic", trackCandidatePandoraCosmic)) trackCandidatePandoraCosmicNotContained=true; 

    // Minimum track length
    if (!selection->IsLongTrack(trackCandidate)) continue;
    EventsTrackLong++;
    std::string status = ph.MakeIntermidiatePlots("longtrack",anatree,trackCandidate);
    //cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Is selected." << endl;

    if (status == "isSignal") isSignal = true;
    if (status == "isCosmic") isCosmic = true;

    if (isCosmic) cosmicBkg++;
    if (isCosmic && trackCandidatePandoraCosmic == -1) cosmicBkgAndNoCosmicTrack++;
    if (isCosmic && trackCandidatePandoraCosmicNotContained) cosmicBkgAndCosmicTrackNotContained++;
    if (isSignal) Signal++;
    if (isSignal && trackCandidatePandoraCosmic == -1) signalAndNoCosmicTrack++;
    if (isSignal && trackCandidatePandoraCosmicNotContained) signalAndCosmicTrackNotContained++;

    // For this selected track, verify if I have hits on all planes.
    if (anatree->ntrkhits_pandoraNu[trackCandidate][0] < 0) noHitsOnUplane++;
    if (anatree->ntrkhits_pandoraNu[trackCandidate][1] < 0) noHitsOnVplane++;
    if (anatree->ntrkhits_pandoraNu[trackCandidate][2] < 0) noHitsOnWplane++;
    /*  cout << "Track "<< trackCandidate<<endl;
      cout << "Number of hits on U plane " << anatree->ntrkhits_pandoraNu[trackCandidate][0]<<endl;
      cout << "               on V plane " << anatree->ntrkhits_pandoraNu[trackCandidate][1]<<endl;
      cout << "               on W plane " << anatree->ntrkhits_pandoraNu[trackCandidate][2]<<endl;
*/
    Strk_range->Fill(CalcLength(anatree->trkstartx_pandoraNu[trackCandidate], 
                                anatree->trkstarty_pandoraNu[trackCandidate], 
                                anatree->trkstartz_pandoraNu[trackCandidate], 
                                anatree->trkendx_pandoraNu[trackCandidate], 
                                anatree->trkendy_pandoraNu[trackCandidate], 
                                anatree->trkendz_pandoraNu[trackCandidate]));

    selectedEvents++;
    newtree->Fill();
 
    delete selection;
  }

  Sflashtime      ->Save();
  Sflashtime50pe  ->Save();
  Sflashpe        ->Save();
  Sflashycenter   ->Save();
  Sflashzcenter   ->Save();
  Sflashtimewidth ->Save();
  Sfls_timeVSpe   ->Save();
  Sfls_timeVSnu_distance ->Save();

  Sgeanttruetime_neutrino  ->Save();
  Sgeanttruetime_cosmic    ->Save();
 
  Strk_range ->Save();

  ph.SaveIntermidiatePlots();

  
  ph.MakeRatioPlots("longtrack","trackcontained");
  ph.MakeRatioPlots("trackcontained","flashmatch");
  ph.MakeRatioPlots("flashmatch","selectbesttrack");
  ph.MakeRatioPlots("selectbesttrack","vertexcontained");
  ph.MakeRatioPlots("vertexcontained","flashtag");

  newtree->AutoSave();
  delete oldfile;
  delete newfile;

  cout << endl << endl << "********************************" << endl;
  cout << "Total events:     " << evts << endl;
  cout << "Selected Events:  " << selectedEvents << endl;
  cout << "Ratio:            " << (double)selectedEvents/(double)evts << endl << endl << endl;

  std::cout << "number of events with flash > 50 PE : " << EventsWithFlash << endl;
  std::cout << "number of events with vtx in FV : " << EventsVtxInFV << endl;
  std::cout << "number of events with tracks matched within 80cm to flash : " << EventsFlashMatched << std::endl;
  std::cout << "number of events with contained tracks : " << EventsTracksInFV << std::endl;
  std::cout << "number of events with longest track > 75cm : " << EventsTrackLong << std::endl;

  std::cout <<"noHitsOnUplane = " << noHitsOnUplane << endl;
  std::cout <<"noHitsOnVplane = " << noHitsOnVplane << endl;
  std::cout <<"noHitsOnWplane = " << noHitsOnWplane << endl << endl << endl;

  std::cout <<"cosmicBkg                 = " << cosmicBkg << endl;
  std::cout <<"cosmicBkgAndNoCosmicTrack = " << cosmicBkgAndNoCosmicTrack << endl;
  std::cout <<"signal                    = " << Signal << endl;
  std::cout <<"signalAndNoCosmicTrack    = " << signalAndNoCosmicTrack << endl;
  std::cout <<"cosmicBkgAndCosmicTrackNotContained = " << cosmicBkgAndCosmicTrackNotContained << endl;
  std::cout <<"signalAndCosmicTrackNotContained = " << signalAndCosmicTrackNotContained << endl;

  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  cout << endl << endl << "Computing time: " << elapsed_secs << " seconds = " << elapsed_secs/60. << " minutes." << endl << endl;

  //rootapp->Run();
  //rootapp->Terminate(0);

  return 0;
}
