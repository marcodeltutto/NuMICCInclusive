void makeClass() {

  TFile *f = new TFile("/data/t2k/lar/uboone/prodgenie_numi_nu_uboone_MCC7/prodgenie_numi_nu_cosmic_uboone_merged_gen_g4_detsim_reco1_reco2_ana.root");
  //f->cd("analysistree");
  std::cout << "here" << std::endl;
  TTree *v = (TTree*)f->Get("analysistree/anatree");
  std::cout << "here2" << std::endl;
  v->MakeClass("AnaNuMI");
}
