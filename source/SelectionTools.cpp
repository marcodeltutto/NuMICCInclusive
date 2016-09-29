#define SelectionTools_cxx

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

#include "SelectionTools.hpp"
#include "AnaNuMI.h"


//____________________________________________________________________
void SelectionTools::Test() {

  cout << "This is a test." << endl;

}

//____________________________________________________________________
bool SelectionTools::FlashTag(int &theFlash){

  bool  flashtag = false;
  float flashmax = 0;

  for(int f = 0; f < fanatree->no_flashes; f++) {
    // If the flash is in the beam window and above threshold set flashtag to true                
    if( (fanatree->flash_time[f] > beammin && fanatree->flash_time[f] < beammax) && fanatree->flash_pe[f] > PEthresh ) {
      flashtag = true; //the event does have a flash inside the beam window
      // If the new flash has more PE than the current maximum, replace the maximum
      if(fanatree->flash_pe[f] > flashmax) {
        theFlash = f;
        flashmax = fanatree->flash_pe[f];
      }
    }
  } // flash loop

  if (flashtag) return true;
  return false; 
}

//____________________________________________________________________


void SelectionTools::CreateVertexTrackAssociation(std::map< int,std::vector<int> > &VertexTrackCollection){

  double diststart, distend, TrackRange;

  // Loop over vertices
  for(int v = 0; v < fanatree->nvtx_pandoraNu; v++) {
    // Loop over reco tracks
    for (int j = 0; j < fanatree->ntracks_pandoraNu; j++) {

      // Calculate distances from track start/end to vertex and calculate track length
      diststart = sqrt((fanatree->vtxx_pandoraNu[v] - fanatree->trkstartx_pandoraNu[j])*(fanatree->vtxx_pandoraNu[v] - fanatree->trkstartx_pandoraNu[j]) 
                     + (fanatree->vtxy_pandoraNu[v] - fanatree->trkstarty_pandoraNu[j])*(fanatree->vtxy_pandoraNu[v] - fanatree->trkstarty_pandoraNu[j]) 
                     + (fanatree->vtxz_pandoraNu[v] - fanatree->trkstartz_pandoraNu[j])*(fanatree->vtxz_pandoraNu[v] - fanatree->trkstartz_pandoraNu[j]));
      distend = sqrt((fanatree->vtxx_pandoraNu[v] - fanatree->trkendx_pandoraNu[j])*(fanatree->vtxx_pandoraNu[v] - fanatree->trkendx_pandoraNu[j]) 
                   + (fanatree->vtxy_pandoraNu[v] - fanatree->trkendy_pandoraNu[j])*(fanatree->vtxy_pandoraNu[v] - fanatree->trkendy_pandoraNu[j]) 
                   + (fanatree->vtxz_pandoraNu[v] - fanatree->trkendz_pandoraNu[j])*(fanatree->vtxz_pandoraNu[v] - fanatree->trkendz_pandoraNu[j]));
      TrackRange = sqrt(pow(fanatree->trkstartx_pandoraNu[j] - fanatree->trkendx_pandoraNu[j],2) 
                      + pow(fanatree->trkstarty_pandoraNu[j] - fanatree->trkendy_pandoraNu[j],2) 
                      + pow(fanatree->trkstartz_pandoraNu[j] - fanatree->trkendz_pandoraNu[j],2));
/*
      std::cout << "vtxx is " << fanatree->vtxx_pandoraNu[v] << std::endl;
      std::cout << "vtxy is " << fanatree->vtxy_pandoraNu[v] << std::endl;
      std::cout << "vtxz is " << fanatree->vtxz_pandoraNu[v] << std::endl;
      std::cout << "trkstartx is " << fanatree->trkstartx_pandoraNu[j] << std::endl;
      std::cout << "trkstarty is " << fanatree->trkstarty_pandoraNu[j] << std::endl;
      std::cout << "trkstartz is " << fanatree->trkstartz_pandoraNu[j] << std::endl;
      std::cout << "trkendx is " << fanatree->trkendx_pandoraNu[j] << std::endl;
      std::cout << "trkendy is " << fanatree->trkendy_pandoraNu[j] << std::endl;
      std::cout << "trkendz is " << fanatree->trkendz_pandoraNu[j] << std::endl;
      std::cout << "------------" << std::endl;
      std::cout << diststart << " " << distend << " " << TrackRange << std::endl;
*/
      // If the track vertex distance is within cut, increase track count
      if(diststart < distcut || distend < distcut) {
        VertexTrackCollection.insert(std::pair< int,std::vector<int> >(v,std::vector<int>()));
        VertexTrackCollection.at(v).push_back(j);

      } // end if distance cut
    } // end loop tracks
  } // end loop vertices

}


//____________________________________________________________________
void SelectionTools::SelectVertexTrackForward(std::map< int,std::vector<int> > &VertexTrackCollection, int & vertexCandidate){

  double TrackRange, WeightedCosTheta = 0.0, NormFactor = 0.0;
  double VertexCosTheta = 0.0;
  int VertexID;

  // Loop over the collection of vertices 
  for(auto const& VtxTrack : VertexTrackCollection){
    VertexID = VtxTrack.first;
    WeightedCosTheta = 0.0;
    NormFactor = 0.0;
    // Loop over all associated track IDs of this vertex
    for(auto const& TrackID : VtxTrack.second){
      TrackRange = sqrt(pow(fanatree->trkstartx_pandoraNu[TrackID] - fanatree->trkendx_pandoraNu[TrackID],2) 
                      + pow(fanatree->trkstarty_pandoraNu[TrackID] - fanatree->trkendy_pandoraNu[TrackID],2) 
                      + pow(fanatree->trkstartz_pandoraNu[TrackID] - fanatree->trkendz_pandoraNu[TrackID],2));
      NormFactor += TrackRange;
      WeightedCosTheta += TrackRange*cos(fanatree->trktheta_pandoraNu[TrackID]);

    } // end loop tracks

    // Make average
    WeightedCosTheta /= NormFactor;

    // Check for flatest angle (also backwards pointing)
    if(fabs(WeightedCosTheta) > VertexCosTheta) {
      vertexCandidate = VertexID;
      VertexCosTheta = fabs(WeightedCosTheta);
    }

  } // end loop vertices  

}

//____________________________________________________________________
bool SelectionTools::InFV(std::string type, int vertexCandidate) {

  double x;
  double y;
  double z;

  if (type == "vertex") {
    x = fanatree->vtxx_pandoraNu[vertexCandidate];
    y = fanatree->vtxy_pandoraNu[vertexCandidate];
    z = fanatree->vtxz_pandoraNu[vertexCandidate];
    
    return this->InFV(x,y,z);

  } 
  else if (type == "track") {
    x = fanatree->trkstartx_pandoraNu[vertexCandidate];
    y = fanatree->trkstarty_pandoraNu[vertexCandidate];
    z = fanatree->trkstartz_pandoraNu[vertexCandidate];

    bool start = this->InFV(x,y,z);

    x = fanatree->trkendx_pandoraNu[vertexCandidate];
    y = fanatree->trkendy_pandoraNu[vertexCandidate];
    z = fanatree->trkendz_pandoraNu[vertexCandidate];
  
    bool end = this->InFV(x,y,z);

    if (start && end) return true;
    return false;

  }
  else {
    std::cout << "SelectionTools::InFV   Wrong argument." << std::endl;
    exit(0);
  }

  return false;

}


//____________________________________________________________________
bool SelectionTools::InFV(double x, double y, double z) {

  if(x < (FVx - borderx) && (x > borderx) && (y < (FVy/2. - bordery)) && (y > (-FVy/2. + bordery)) && (z < (FVz - borderz)) && (z > borderz)) return true;
  return false;

}


//____________________________________________________________________
int SelectionTools::GetBestTrack(int vertexCandidate, std::map< int,std::vector<int> > &VertexTrackCollection) {

  double trackRange;
  double trackRangeOld = 0.;
  int trackCandidate = -1;

  for(auto const& TrackID : VertexTrackCollection.find(vertexCandidate)->second){
    trackRange = sqrt(pow(fanatree->trkstartx_pandoraNu[TrackID] - fanatree->trkendx_pandoraNu[TrackID],2) 
                    + pow(fanatree->trkstarty_pandoraNu[TrackID] - fanatree->trkendy_pandoraNu[TrackID],2) 
                    + pow(fanatree->trkstartz_pandoraNu[TrackID] - fanatree->trkendz_pandoraNu[TrackID],2));

    if (trackRange > trackRangeOld) {
      trackCandidate = TrackID;
      trackRangeOld = trackRange;
    }
  }

  return trackCandidate;
}


//____________________________________________________________________
bool SelectionTools::IsFlashMatched(int trackCandidate, int theFlash) {

  int    flash = fanatree->flash_zcenter[theFlash];
  double start = fanatree->trkstartz_pandoraNu[trackCandidate]; 
  double end   = fanatree->trkendz_pandoraNu[trackCandidate];
  double distance;

  if(end >= start) {
    if(flash < end && flash > start) distance = 0;
    else distance = TMath::Min(fabs(flash-start), fabs(flash-end));
  }
  else {
    if(flash > end && flash < start) distance = 0;
    else distance = TMath::Min(fabs(flash-start), fabs(flash-end));
  }

  if (distance < flashwidth) return true;
  return false;

}



//____________________________________________________________________
bool SelectionTools::IsLongTrack(int TrackID) {

  double trackRange = sqrt(pow(fanatree->trkstartx_pandoraNu[TrackID] - fanatree->trkendx_pandoraNu[TrackID],2)
                         + pow(fanatree->trkstarty_pandoraNu[TrackID] - fanatree->trkendy_pandoraNu[TrackID],2)
                         + pow(fanatree->trkstartz_pandoraNu[TrackID] - fanatree->trkendz_pandoraNu[TrackID],2));
  
  if (trackRange > lengthcut) return true;
  return false;
}













