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

using namespace std;

//____________________________________________________________________________________________________
struct CutPlots {

  Spectrum* Snuenergy_numu;
  Spectrum* Snuenergy_anumu;
  Spectrum* Snuenergy_nue;
  Spectrum* Snuenergy_nc;
  Spectrum* Snuenergy_cosmics;

};

//____________________________________________________________________________________________________
void ActivateBranches(AnaNuMI *anatree) {

  anatree->fChain->SetBranchStatus("*",0);
  anatree->fChain->SetBranchStatus("ccnc_truth",1);
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
  anatree->fChain->SetBranchStatus("trkstartx_pandoraNu",1);
  anatree->fChain->SetBranchStatus("trktheta_pandoraNu",1);
  anatree->fChain->SetBranchStatus("vx_flux",1);
  anatree->fChain->SetBranchStatus("vy_flux",1);
  anatree->fChain->SetBranchStatus("vz_flux",1);
  //anatree->fChain->SetBranchStatus("",1);
}

//____________________________________________________________________________________________________
void InstantiateIntermidiatePlots(std::map<std::string,CutPlots> &cutToPlotsMap, double totalPOT) {

  std::vector<std::string> cutname;
  cutname.resize(5);
  cutname.at(0) = "flashtag";
  cutname.at(1) = "vertexcontained";
  cutname.at(2) = "flashmatch";
  cutname.at(3) = "trackcontained";
  cutname.at(4) = "longtrack";

  for (unsigned int i = 0; i < cutname.size(); i++) {
    CutPlots cutplots;
    cutplots.Snuenergy_numu  = new Spectrum("nuenergy_numu_"+cutname.at(i),  "#nu_{#mu};Neutrino Energy [GeV];Selected Events up to "+cutname.at(i), 100, 0, 10, totalPOT);
    cutplots.Snuenergy_anumu = new Spectrum("nuenergy_anumu_"+cutname.at(i), "#bar{#nu}_{#mu};Neutrino Energy [GeV];Selected Events up to "+cutname.at(i), 100, 0, 10, totalPOT);
    cutplots.Snuenergy_nue   = new Spectrum("nuenergy_nue_"+cutname.at(i),   "#nu_{e}/#bar{#nu}_{e};Neutrino Energy [GeV];Selected Events up to "+cutname.at(i), 100, 0, 10, totalPOT);
    cutplots.Snuenergy_nc    = new Spectrum("nuenergy_nc_"+cutname.at(i),    "NC;Neutrino Energy [GeV];Selected Events up to "+cutname.at(i), 100, 0, 10, totalPOT);

    cutToPlotsMap.emplace(cutname[i],cutplots);
  }
}

//____________________________________________________________________________________________________
void MakeIntermidiatePlots(CutPlots cutplots, AnaNuMI * anatree){

  if(anatree->ccnc_truth[0] == 0 || anatree->nuPDG_truth[0]==14) cutplots.Snuenergy_numu   -> Fill(anatree->enu_truth[0]);
  if(anatree->ccnc_truth[0] == 0 || anatree->nuPDG_truth[0]==-14) cutplots.Snuenergy_anumu -> Fill(anatree->enu_truth[0]);
  if(anatree->ccnc_truth[0] == 0 || anatree->nuPDG_truth[0]==12 || anatree->nuPDG_truth[0]==-12) cutplots.Snuenergy_nue -> Fill(anatree->enu_truth[0]);
  if(anatree->ccnc_truth[0] == 1) cutplots.Snuenergy_nc -> Fill(anatree->enu_truth[0]);

}

//____________________________________________________________________________________________________
void SaveIntermidiatePlots(std::map<std::string,CutPlots> &cutToPlotsMap) {

  TFile *f = new TFile("thstacktest.root","RECREATE");
  f->cd();

  for (std::map<std::string,CutPlots>::iterator it=cutToPlotsMap.begin(); it!=cutToPlotsMap.end(); ++it) {
    std::string label = it->first;
    std::cout << "Saving plot " << label << endl;
    CutPlots cutplots = it->second;

    THStack *hs = new THStack(("hs_"+label).c_str(),(";True Neutrino Energy [GeV];Selected events up to " + label).c_str());
    TH1D* h1 = cutplots.Snuenergy_anumu->ToTH1D();
    h1->SetLineColor(kGray);
    h1->SetFillColor(kGray);
    hs->Add(h1);
    TH1D* h2 = cutplots.Snuenergy_nue  ->ToTH1D();
    h2->SetLineColor(kOrange+3);
    h2->SetFillColor(kOrange+3);
    hs->Add(h2);
    TH1D* h3 = cutplots.Snuenergy_nc   ->ToTH1D();
    h3->SetLineColor(kGreen+2);
    h3->SetFillColor(kGreen+2);
    hs->Add(h3);
    TH1D* h4 = cutplots.Snuenergy_numu ->ToTH1D();
    h4->SetLineColor(kRed+1);
    h4->SetFillColor(kRed+1);
    hs->Add(h4);

    TLegend *leg = new TLegend(0.1,0.7,0.48,0.9);
    leg->AddEntry(h4,"#nu_{#mu}","f");
    leg->AddEntry(h3,"NC","f");
    leg->AddEntry(h2,"#nu_{e}","f");
    leg->AddEntry(h1,"#bar{#nu}_{#mu}","f");

    hs->Write();
    leg->Write("legend");

    cutplots.Snuenergy_numu ->Save();
    cutplots.Snuenergy_anumu->Save();
    cutplots.Snuenergy_nue  ->Save();
    cutplots.Snuenergy_nc   ->Save();

  }
  f->Close();
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

  string pattern;
  if(beam == "numi") pattern = "/data/t2k/lar/uboone/prodgenie_numi_nu_uboone_MCC7/prodgenie_numi_nu_cosmic_uboone_merged_gen_g4_detsim_reco1_reco2_ana.root";
  if(beam == "bnb")  pattern = "/pnfs/uboone/persistent/users/aschu/MC_BNB_Cosmic/prodgenie_bnb_nu_cosmic_uboone_v05_08_00_anatree.root";

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

  // This is a map: cutname <-> spectrum array
  std::map<std::string,CutPlots> cutToPlotsMap;
  InstantiateIntermidiatePlots(cutToPlotsMap,totalPOT);

  if(maxEntries > 0.) evts = maxEntries;

  for(int i = 0; i < evts; i++) {

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

    // Flash tag
    int theFlash = -1;
    if (selection->FlashTag(theFlash) == false) continue;
    MakeIntermidiatePlots(cutToPlotsMap.find("flashtag")->second,anatree);
 
    // Create vertex-track association
    std::map< int,std::vector<int> > VertexTrackCollection;
    selection->CreateVertexTrackAssociation(VertexTrackCollection);  

    // Select the most forward going vertex-track association
    int vertexCandidate = -1;
    selection->SelectVertexTrackForward(VertexTrackCollection, vertexCandidate);

    // Check if vertex candidate is contained in FV
    if (!selection->InFV("vertex", vertexCandidate)) continue;
    MakeIntermidiatePlots(cutToPlotsMap.find("vertexcontained")->second,anatree);

    // Get the longest track associated with the best vertex
    int trackCandidate;
    trackCandidate = selection->GetBestTrack(vertexCandidate, VertexTrackCollection);

    if (trackCandidate == -1) continue;

    // Flash matching
    if (!selection->IsFlashMatched(trackCandidate, theFlash)) continue;
    MakeIntermidiatePlots(cutToPlotsMap.find("flashmatch")->second,anatree);

    // Track must be contained
    if (!selection->InFV("track", trackCandidate)) continue;
    MakeIntermidiatePlots(cutToPlotsMap.find("trackcontained")->second,anatree);

    // Minimum track length
    if (!selection->IsLongTrack(trackCandidate)) continue;
    MakeIntermidiatePlots(cutToPlotsMap.find("longtrack")->second,anatree);

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
 
  SaveIntermidiatePlots(cutToPlotsMap);

  newtree->AutoSave();
  delete oldfile;
  delete newfile;

  cout << endl << endl << "********************************" << endl;
  cout << "Total events:     " << evts << endl;
  cout << "Selected Events:  " << selectedEvents << endl;
  cout << "Ratio:            " << (double)selectedEvents/(double)evts << endl << endl << endl;
  cout << "counter " << counter << endl;





  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  cout << endl << endl << "Computing time: " << elapsed_secs << " seconds = " << elapsed_secs/60. << " minutes." << endl << endl;

  //rootapp->Run();
  //rootapp->Terminate(0);

  return 0;
}
