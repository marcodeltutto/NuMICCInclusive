void display(int entry = 0, string algo = "pandoraNu") {
  
  c1 = new TCanvas("c1","Event Display",200,45,1200,437);
  
  // Creating a view
  TView3D *view = (TView3D*)  TView::CreateView(1);
  view->SetRange(-50,-50,-150,300,1050,150);
  view->ShowAxis();
  int irep;
  view->SetPerspective();
  //view->SetView(0.0,260.0,270.0,irep);
  
  // Draw the detector
  
  double x_d[5] = {0.,      256.35,  256.35,  0,       0};
  double y_d[5] = {-116.5,  -116.5,  -116.5,  -116.5,  -116.5	};
  double z_d[5] = {0,       0,       1036.8,  1036.8,  0};
  
  double x_u[5] = {0.,      256.35,  256.35,  0,       0};
  double y_u[5] = {+116.5,  +116.5,  +116.5,  +116.5,  +116.5};
  double z_u[5] = {0,       0,       1036.8,  1036.8,  0};
  
  
  TPolyLine3D *tpcDown = new TPolyLine3D(5, x_d, z_d, y_d);
  TPolyLine3D *tpcUp = new TPolyLine3D(5, x_u, z_u, y_u);
  
  TPolyLine3D *vert1 = new TPolyLine3D(2);
  TPolyLine3D *vert2 = new TPolyLine3D(2);
  TPolyLine3D *vert3 = new TPolyLine3D(2);
  TPolyLine3D *vert4 = new TPolyLine3D(2);
  
  vert1->SetPoint(0,0,0,-116.5);
  vert2->SetPoint(0,256.35,0,-116.5);
  vert3->SetPoint(0,256.35,1036.8,-116.5);
  vert4->SetPoint(0,0,1036.8,-116.5);
  
  vert1->SetPoint(1,0,0,116.5);
  vert2->SetPoint(1,256.35,0,116.5);
  vert3->SetPoint(1,256.35,1036.8,116.5);
  vert4->SetPoint(1,0,1036.8,116.5);
  
  
  tpcDown->Draw();
  tpcUp->Draw();
  vert1->Draw();
  vert2->Draw();
  vert3->Draw();
  vert4->Draw();
  
  
  //********************************
  
  TTree          *fChain;
  TFile *f = new TFile("/pnfs/uboone/persistent/users/aschu/MC_BNB_Cosmic/prodgenie_bnb_nu_cosmic_uboone_v05_08_00_anatree.root");
  TDirectory * dir = (TDirectory*)f->Get("/pnfs/uboone/persistent/users/aschu/MC_BNB_Cosmic/prodgenie_bnb_nu_cosmic_uboone_v05_08_00_anatree.root:/analysistree");
  dir->GetObject("anatree",fChain);
  
  Short_t        ntracks_pandoraNu;
  Float_t        trkstartx_pandoraNu[32];
  Float_t        trkstarty_pandoraNu[32];
  Float_t        trkstartz_pandoraNu[32];
  Float_t        trkendx_pandoraNu[32];
  Float_t        trkendy_pandoraNu[32];
  Float_t        trkendz_pandoraNu[32];
  
  Short_t        ntracks_pandoraCosmic;
  Float_t        trkstartx_pandoraCosmic[265];
  Float_t        trkstarty_pandoraCosmic[265];
  Float_t        trkstartz_pandoraCosmic[265];
  Float_t        trkendx_pandoraCosmic[265];
  Float_t        trkendy_pandoraCosmic[265];
  Float_t        trkendz_pandoraCosmic[265];

  TBranch        *b_ntracks_pandoraNu;
  TBranch        *b_trkstartx_pandoraNu;
  TBranch        *b_trkstarty_pandoraNu;
  TBranch        *b_trkstartz_pandoraNu;
  TBranch        *b_trkendx_pandoraNu;
  TBranch        *b_trkendy_pandoraNu;
  TBranch        *b_trkendz_pandoraNu;
  
  TBranch        *b_ntracks_pandoraCosmic;
  TBranch        *b_trkstartx_pandoraCosmic;
  TBranch        *b_trkstarty_pandoraCosmic;
  TBranch        *b_trkstartz_pandoraCosmic;
  TBranch        *b_trkendx_pandoraCosmic;
  TBranch        *b_trkendy_pandoraCosmic;
  TBranch        *b_trkendz_pandoraCosmic;

  fChain->SetBranchAddress("ntracks_pandoraNu",   &ntracks_pandoraNu,  &b_ntracks_pandoraNu);
  fChain->SetBranchAddress("trkstartx_pandoraNu", trkstartx_pandoraNu, &b_trkstartx_pandoraNu);
  fChain->SetBranchAddress("trkstarty_pandoraNu", trkstarty_pandoraNu, &b_trkstarty_pandoraNu);
  fChain->SetBranchAddress("trkstartz_pandoraNu", trkstartz_pandoraNu, &b_trkstartz_pandoraNu);
  fChain->SetBranchAddress("trkendx_pandoraNu", trkendx_pandoraNu, &b_trkendx_pandoraNu);
  fChain->SetBranchAddress("trkendy_pandoraNu", trkendy_pandoraNu, &b_trkendy_pandoraNu);
  fChain->SetBranchAddress("trkendz_pandoraNu", trkendz_pandoraNu, &b_trkendz_pandoraNu);

  fChain->SetBranchAddress("ntracks_pandoraCosmic",   &ntracks_pandoraCosmic,  &b_ntracks_pandoraCosmic);
  fChain->SetBranchAddress("trkstartx_pandoraCosmic", trkstartx_pandoraCosmic, &b_trkstartx_pandoraCosmic);
  fChain->SetBranchAddress("trkstarty_pandoraCosmic", trkstarty_pandoraCosmic, &b_trkstarty_pandoraCosmic);
  fChain->SetBranchAddress("trkstartz_pandoraCosmic", trkstartz_pandoraCosmic, &b_trkstartz_pandoraCosmic);
  fChain->SetBranchAddress("trkendx_pandoraCosmic", trkendx_pandoraCosmic, &b_trkendx_pandoraCosmic);
  fChain->SetBranchAddress("trkendy_pandoraCosmic", trkendy_pandoraCosmic, &b_trkendy_pandoraCosmic);
  fChain->SetBranchAddress("trkendz_pandoraCosmic", trkendz_pandoraCosmic, &b_trkendz_pandoraCosmic);

  
  fChain->GetEntry(entry);
  
  TPolyLine3D * track_Nu[100];
  TPolyLine3D * track_Cosmic[100];

  if(algo == "pandoraNu"){
    for (int t = 0; t < ntracks_pandoraNu; t++){
      track_Nu[t] = new TPolyLine3D(2);
      track_Nu[t]->SetPoint(0,trkstartx_pandoraNu[t], trkstartz_pandoraNu[t], trkstarty_pandoraNu[t]);
      track_Nu[t]->SetPoint(1,trkendx_pandoraNu[t],   trkendz_pandoraNu[t],   trkendy_pandoraNu[t]);
      track_Nu[t]->SetLineColor(kRed);
      track_Nu[t]->Draw();
    
    }
  }
 
  if(algo == "pandoraCosmic"){
    for (int t = 0; t < ntracks_pandoraCosmic; t++){
      track_Cosmic[t] = new TPolyLine3D(2);
      track_Cosmic[t]->SetPoint(0,trkstartx_pandoraCosmic[t], trkstartz_pandoraCosmic[t], trkstarty_pandoraCosmic[t]);
      track_Cosmic[t]->SetPoint(1,trkendx_pandoraCosmic[t],   trkendz_pandoraCosmic[t],   trkendy_pandoraCosmic[t]);
      track_Cosmic[t]->SetLineColor(kGreen);
      track_Cosmic[t]->Draw();
    
    }
  }

  
  std::cout << "start x " << trkstartx_pandoraCosmic[0] << std::endl;
  std::cout << "start y " << trkstarty_pandoraCosmic[0] << std::endl;
  std::cout << "start z " << trkstartz_pandoraCosmic[0] << std::endl;
  std::cout << "end x " << trkendx_pandoraCosmic[0] << std::endl;
  std::cout << "end y " << trkendy_pandoraCosmic[0] << std::endl;
  std::cout << "end z " << trkendz_pandoraCosmic[0] << std::endl;

  
  
}
