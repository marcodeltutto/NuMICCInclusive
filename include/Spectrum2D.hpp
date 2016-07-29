#ifndef Spectrum2D_h
#define Spectrum2D_h

#include <iostream>
#include <iomanip>
#include <string>

#include <TSystem.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TH2D.h>
#include <TLatex.h>

using namespace std;

class Spectrum2D {

  public :

  Spectrum2D(string name, string title, int nbins, double xlow, double xup, int nbinsy, double ylow, double yup, double POT);
  Spectrum2D(TH2D* hin, double POT);
  virtual ~Spectrum2D();
  TH2D* ToTH2D();
  TH2D* ToTH2D(double exposure);
  double GetPOT();
  void Fill(double valuex, double valuey);
  void Save();

  private:

  double fPOT;
  TH2D *h;
  string fname;
  TLatex *latex;
};

#endif
#ifdef Spectrum2D_cxx

Spectrum2D::Spectrum2D(string name, string title, int nbinsx, double xlow, double xup, int nbinsy, double ylow, double yup, double POT) {

  fPOT = POT;
  fname = name;

  //std::ostringstream title;
  //title << fname << " @ " << fPOT << " POT"; 
  //std::string titlestr = title.str();

  h = new TH2D(name.c_str(), title.c_str(), (Int_t)nbinsx, (Double_t)xlow, (Double_t)xup, (Int_t)nbinsy, (Double_t)ylow, (Double_t)yup);

  std::ostringstream ltx;
  ltx << fPOT << " POT";
  std::string ltxstr = ltx.str();

  double x = 0.84;
  double y = 0.52;
  double size = 25;
  int color = 1;
  int font = 43;
  int align = 32;
  latex = new TLatex(x, y, ltxstr.c_str());
  latex->SetNDC();
  latex->SetTextSize(size);
  latex->SetTextColor(color);
  latex->SetTextFont(font);
  latex->SetTextAlign(align);
  //latex->Draw();

}

Spectrum2D::Spectrum2D(TH2D* hin, double POT){

  fPOT = POT;
  h = hin;
}

Spectrum2D::~Spectrum2D() {
  
}

TH2D* Spectrum2D::ToTH2D() {
  return h;
}

TH2D* Spectrum2D::ToTH2D(double exposure) {
  TH2D *clone = (TH2D*) h->Clone("clone");
  clone->Scale(exposure/fPOT);
  return clone;
}

double Spectrum2D::GetPOT() {
  return fPOT;
}

void Spectrum2D::Fill(double valuex, double valuey) {
  h->Fill(valuex, valuey);  
}

void Spectrum2D::Save() {

  gROOT->SetBatch(kTRUE);

  Double_t width = 600;
  Double_t height = 600;
  TCanvas * c1 = new TCanvas("c", "c", width, height);

  h->Draw("histo");
  latex->Draw();

  TString temp = fname;
  c1->SaveAs(temp + ".pdf");
  c1->SaveAs(temp + ".C","C");
}



#endif // #ifdef Spectrum_cxx
