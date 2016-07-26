#ifndef Spectrum_h
#define Spectrum_h

#include <iostream>
#include <iomanip>
#include <string>

#include <TH1D.h>

using namespace std;

class Spectrum {

  public :

  Spectrum(string name, int nbins, double xlow, double xup, double POT);
  Spectrum(TH1D* hin, double POT);
  virtual ~Spectrum();
  TH1D* ToTH1D();
  TH1D* ToTH1D(double exposure);
  double GetPOT();
  void Fill(double value);
  void Save();

  private:

  double fPOT;
  TH1D *h;
  string fname;

};

#endif
#ifdef Spectrum_cxx

Spectrum::Spectrum(string name, int nbins, double xlow, double xup, double POT) {

  fPOT = POT;
  fname = name;

  h = new TH1D(name.c_str(), name.c_str(), (Int_t)nbins, (Double_t)xlow, (Double_t)xup);

}

Spectrum::Spectrum(TH1D* hin, double POT){

  fPOT = POT;
  h = hin;
}

Spectrum::~Spectrum() {
  
}

TH1D* Spectrum::ToTH1D() {
  return h;
}

TH1D* Spectrum::ToTH1D(double exposure) {
  TH1D *clone = (TH1D*) h->Clone("clone");
  clone->Scale(exposure/fPOT);
  return clone;
}

double Spectrum::GetPOT() {
  return fPOT;
}

void Spectrum::Fill(double value) {
  h->Fill(value);  
}

void Spectrum::Save() {

  TString temp = fname;
  h->SaveAs(temp + ".pdf","pdf");
  h->SaveAs(temp + ".C","C");
}



#endif // #ifdef Spectrum_cxx
