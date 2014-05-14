#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// DQM include files

#include "DQMServices/Core/interface/MonitorElement.h"

// work on collections
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"

#include "DQMServices/Core/interface/DQMStore.h"

#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/Math/interface/Point3D.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidateFwd.h"
/// EgammaCoreTools
#include "RecoEcal/EgammaCoreTools/interface/EcalEtaPhiRegion.h"

#include "TVector3.h"

#include "DQM/HLTEvF/interface/HLTAlCaMonPi0.h"


#define TWOPI 6.283185308



using namespace std;
using namespace edm;


// ******************************************
// constructors
// *****************************************

HLTAlCaMonPi0::HLTAlCaMonPi0( const edm::ParameterSet& ps ) :
eventCounter_(0)
{
  dbe_ = Service<DQMStore>().operator->();
  folderName_ = ps.getUntrackedParameter<std::string>("FolderName","HLT/AlCaEcalPi0");
  prescaleFactor_ = ps.getUntrackedParameter<int>("prescaleFactor",1);
  productMonitoredEBpi0_= ps.getUntrackedParameter<edm::InputTag>("AlCaStreamEBpi0Tag");
  productMonitoredEBeta_= ps.getUntrackedParameter<edm::InputTag>("AlCaStreamEBetaTag");
  productMonitoredEEpi0_= ps.getUntrackedParameter<edm::InputTag>("AlCaStreamEEpi0Tag");
  productMonitoredEEeta_= ps.getUntrackedParameter<edm::InputTag>("AlCaStreamEEetaTag");

  isMonEBpi0_ = ps.getUntrackedParameter<bool>("isMonEBpi0",false);
  isMonEBeta_ = ps.getUntrackedParameter<bool>("isMonEBeta",false);
  isMonEEpi0_ = ps.getUntrackedParameter<bool>("isMonEEpi0",false);
  isMonEEeta_ = ps.getUntrackedParameter<bool>("isMonEEeta",false);

  saveToFile_=ps.getUntrackedParameter<bool>("SaveToFile",false);
  fileName_=  ps.getUntrackedParameter<std::string>("FileName","MonitorAlCaEcalPi0.root");

  clusSeedThr_ = ps.getParameter<double> ("clusSeedThr");
  clusSeedThrEndCap_ = ps.getParameter<double> ("clusSeedThrEndCap");
  clusEtaSize_ = ps.getParameter<int> ("clusEtaSize");
  clusPhiSize_ = ps.getParameter<int> ("clusPhiSize");
  if ( clusPhiSize_ % 2 == 0 ||  clusEtaSize_ % 2 == 0)
    edm::LogError("AlCaPi0RecHitsProducerError") << "Size of eta/phi for simple clustering should be odd numbers";


  seleXtalMinEnergy_ = ps.getParameter<double>("seleXtalMinEnergy");
  seleXtalMinEnergyEndCap_ = ps.getParameter<double>("seleXtalMinEnergyEndCap");

    ///for Pi0 barrel selection
    selePtGamma_ = ps.getParameter<double> ("selePtGamma");  
    selePtPi0_ = ps.getParameter<double> ("selePtPi0");  
    seleMinvMaxPi0_ = ps.getParameter<double> ("seleMinvMaxPi0");  
    seleMinvMinPi0_ = ps.getParameter<double> ("seleMinvMinPi0");  
    seleS4S9Gamma_ = ps.getParameter<double> ("seleS4S9Gamma");  
    selePi0Iso_ = ps.getParameter<double> ("selePi0Iso");  
    ptMinForIsolation_ = ps.getParameter<double> ("ptMinForIsolation");
    selePi0BeltDR_ = ps.getParameter<double> ("selePi0BeltDR");  
    selePi0BeltDeta_ = ps.getParameter<double> ("selePi0BeltDeta");  

  
    ///for Pi0 endcap selection
    selePtGammaEndCap_ = ps.getParameter<double> ("selePtGammaEndCap");  
    selePtPi0EndCap_ = ps.getParameter<double> ("selePtPi0EndCap");   
    seleS4S9GammaEndCap_ = ps.getParameter<double> ("seleS4S9GammaEndCap");  
    seleMinvMaxPi0EndCap_ = ps.getParameter<double> ("seleMinvMaxPi0EndCap");  
    seleMinvMinPi0EndCap_ = ps.getParameter<double> ("seleMinvMinPi0EndCap");  
    ptMinForIsolationEndCap_ = ps.getParameter<double> ("ptMinForIsolationEndCap");
    selePi0BeltDREndCap_ = ps.getParameter<double> ("selePi0BeltDREndCap");  
    selePi0BeltDetaEndCap_ = ps.getParameter<double> ("selePi0BeltDetaEndCap");  
    selePi0IsoEndCap_ = ps.getParameter<double> ("selePi0IsoEndCap");  


    ///for Eta barrel selection
    selePtGammaEta_ = ps.getParameter<double> ("selePtGammaEta");  
    selePtEta_ = ps.getParameter<double> ("selePtEta");   
    seleS4S9GammaEta_ = ps.getParameter<double> ("seleS4S9GammaEta");  
    seleS9S25GammaEta_ = ps.getParameter<double> ("seleS9S25GammaEta");  
    seleMinvMaxEta_ = ps.getParameter<double> ("seleMinvMaxEta");  
    seleMinvMinEta_ = ps.getParameter<double> ("seleMinvMinEta");  
    ptMinForIsolationEta_ = ps.getParameter<double> ("ptMinForIsolationEta");
    seleEtaIso_ = ps.getParameter<double> ("seleEtaIso");  
    seleEtaBeltDR_ = ps.getParameter<double> ("seleEtaBeltDR");  
    seleEtaBeltDeta_ = ps.getParameter<double> ("seleEtaBeltDeta");  

  
    ///for Eta endcap selection
    selePtGammaEtaEndCap_ = ps.getParameter<double> ("selePtGammaEtaEndCap");  
    selePtEtaEndCap_ = ps.getParameter<double> ("selePtEtaEndCap");   
    seleS4S9GammaEtaEndCap_ = ps.getParameter<double> ("seleS4S9GammaEtaEndCap");  
    seleS9S25GammaEtaEndCap_ = ps.getParameter<double> ("seleS9S25GammaEtaEndCap");  
    seleMinvMaxEtaEndCap_ = ps.getParameter<double> ("seleMinvMaxEtaEndCap");  
    seleMinvMinEtaEndCap_ = ps.getParameter<double> ("seleMinvMinEtaEndCap");  
    ptMinForIsolationEtaEndCap_ = ps.getParameter<double> ("ptMinForIsolationEtaEndCap");
    seleEtaIsoEndCap_ = ps.getParameter<double> ("seleEtaIsoEndCap");  
    seleEtaBeltDREndCap_ = ps.getParameter<double> ("seleEtaBeltDREndCap");  
    seleEtaBeltDetaEndCap_ = ps.getParameter<double> ("seleEtaBeltDetaEndCap");  


  // Parameters for the position calculation:
  edm::ParameterSet posCalcParameters = 
    ps.getParameter<edm::ParameterSet>("posCalcParameters");
  posCalculator_ = PositionCalc(posCalcParameters);

  //set Token(-s)
  productMonitoredEBpi0Token_ = consumes<EcalRecHitCollection>(ps.getUntrackedParameter<edm::InputTag>("AlCaStreamEBpi0Tag"));
  productMonitoredEBetaToken_ = consumes<EcalRecHitCollection>(ps.getUntrackedParameter<edm::InputTag>("AlCaStreamEBetaTag"));
  productMonitoredEEpi0Token_ = consumes<EcalRecHitCollection>(ps.getUntrackedParameter<edm::InputTag>("AlCaStreamEEpi0Tag"));
  productMonitoredEEetaToken_ = consumes<EcalRecHitCollection>(ps.getUntrackedParameter<edm::InputTag>("AlCaStreamEEetaTag"));
}


HLTAlCaMonPi0::~HLTAlCaMonPi0()
{}


//--------------------------------------------------------
void HLTAlCaMonPi0::beginJob(){


  // create and cd into new folder
  dbe_->setCurrentFolder(folderName_);

  // book some histograms 1D

  hiPhiDistrEBpi0_ = dbe_->book1D("iphiDistributionEBpi0", "RechitEB pi0 iphi", 361, 1,361);
  hiPhiDistrEBpi0_->setAxisTitle("i#phi ", 1);
  hiPhiDistrEBpi0_->setAxisTitle("# rechits", 2);

  hiXDistrEEpi0_ = dbe_->book1D("iXDistributionEEpi0", "RechitEE pi0 ix", 100, 0,100);
  hiXDistrEEpi0_->setAxisTitle("ix ", 1);
  hiXDistrEEpi0_->setAxisTitle("# rechits", 2);

  hiPhiDistrEBeta_ = dbe_->book1D("iphiDistributionEBeta", "RechitEB eta iphi", 361, 1,361);
  hiPhiDistrEBeta_->setAxisTitle("i#phi ", 1);
  hiPhiDistrEBeta_->setAxisTitle("# rechits", 2);

  hiXDistrEEeta_ = dbe_->book1D("iXDistributionEEeta", "RechitEE eta ix", 100, 0,100);
  hiXDistrEEeta_->setAxisTitle("ix ", 1);
  hiXDistrEEeta_->setAxisTitle("# rechits", 2);


  hiEtaDistrEBpi0_ = dbe_->book1D("iEtaDistributionEBpi0", "RechitEB pi0 ieta", 171, -85, 86);
  hiEtaDistrEBpi0_->setAxisTitle("i#eta", 1);
  hiEtaDistrEBpi0_->setAxisTitle("#rechits", 2);

  hiYDistrEEpi0_ = dbe_->book1D("iYDistributionEBpi0", "RechitEE pi0 iY", 100, 0, 100);
  hiYDistrEEpi0_->setAxisTitle("iy", 1);
  hiYDistrEEpi0_->setAxisTitle("#rechits", 2);

  hiEtaDistrEBeta_ = dbe_->book1D("iEtaDistributionEBeta", "RechitEB eta ieta", 171, -85, 86);
  hiEtaDistrEBeta_->setAxisTitle("i#eta", 1);
  hiEtaDistrEBeta_->setAxisTitle("#rechits", 2);

  hiYDistrEEeta_ = dbe_->book1D("iYDistributionEBeta", "RechitEE eta iY", 100, 0, 100);
  hiYDistrEEeta_->setAxisTitle("iy", 1);
  hiYDistrEEeta_->setAxisTitle("#rechits", 2);


  hRechitEnergyEBpi0_ = dbe_->book1D("rhEnergyEBpi0","Pi0 rechits energy EB",160,0.,2.0);
  hRechitEnergyEBpi0_->setAxisTitle("energy (GeV) ",1);
  hRechitEnergyEBpi0_->setAxisTitle("#rechits",2);

  hRechitEnergyEEpi0_ = dbe_->book1D("rhEnergyEEpi0","Pi0 rechits energy EE",160,0.,3.0);
  hRechitEnergyEEpi0_->setAxisTitle("energy (GeV) ",1);
  hRechitEnergyEEpi0_->setAxisTitle("#rechits",2);

  hRechitEnergyEBeta_ = dbe_->book1D("rhEnergyEBeta","Eta rechits energy EB",160,0.,2.0);
  hRechitEnergyEBeta_->setAxisTitle("energy (GeV) ",1);
  hRechitEnergyEBeta_->setAxisTitle("#rechits",2);

  hRechitEnergyEEeta_ = dbe_->book1D("rhEnergyEEeta","Eta rechits energy EE",160,0.,3.0);
  hRechitEnergyEEeta_->setAxisTitle("energy (GeV) ",1);
  hRechitEnergyEEeta_->setAxisTitle("#rechits",2);

  hEventEnergyEBpi0_ = dbe_->book1D("eventEnergyEBpi0","Pi0 event energy EB",100,0.,20.0);
  hEventEnergyEBpi0_->setAxisTitle("energy (GeV) ",1);

  hEventEnergyEEpi0_ = dbe_->book1D("eventEnergyEEpi0","Pi0 event energy EE",100,0.,50.0);
  hEventEnergyEEpi0_->setAxisTitle("energy (GeV) ",1);

  hEventEnergyEBeta_ = dbe_->book1D("eventEnergyEBeta","Eta event energy EB",100,0.,20.0);
  hEventEnergyEBeta_->setAxisTitle("energy (GeV) ",1);

  hEventEnergyEEeta_ = dbe_->book1D("eventEnergyEEeta","Eta event energy EE",100,0.,50.0);
  hEventEnergyEEeta_->setAxisTitle("energy (GeV) ",1);

  hNRecHitsEBpi0_ = dbe_->book1D("nRechitsEBpi0","#rechits in pi0 collection EB",100,0.,250.);
  hNRecHitsEBpi0_->setAxisTitle("rechits ",1);
  
  hNRecHitsEEpi0_ = dbe_->book1D("nRechitsEEpi0","#rechits in pi0 collection EE",100,0.,250.);
  hNRecHitsEEpi0_->setAxisTitle("rechits ",1);
  
  hNRecHitsEBeta_ = dbe_->book1D("nRechitsEBeta","#rechits in eta collection EB",100,0.,250.);
  hNRecHitsEBeta_->setAxisTitle("rechits ",1);
  
  hNRecHitsEEeta_ = dbe_->book1D("nRechitsEEeta","#rechits in eta collection EE",100,0.,250.);
  hNRecHitsEEeta_->setAxisTitle("rechits ",1);
  
  hMeanRecHitEnergyEBpi0_ = dbe_->book1D("meanEnergyEBpi0","Mean rechit energy in pi0 collection EB",50,0.,2.);
  hMeanRecHitEnergyEBpi0_->setAxisTitle("Mean Energy [GeV] ",1);
  
  hMeanRecHitEnergyEEpi0_ = dbe_->book1D("meanEnergyEEpi0","Mean rechit energy in pi0 collection EE",100,0.,5.);
  hMeanRecHitEnergyEEpi0_->setAxisTitle("Mean Energy [GeV] ",1);
  
  hMeanRecHitEnergyEBeta_ = dbe_->book1D("meanEnergyEBeta","Mean rechit energy in eta collection EB",50,0.,2.);
  hMeanRecHitEnergyEBeta_->setAxisTitle("Mean Energy [GeV] ",1);
  
  hMeanRecHitEnergyEEeta_ = dbe_->book1D("meanEnergyEEeta","Mean rechit energy in eta collection EE",100,0.,5.);
  hMeanRecHitEnergyEEeta_->setAxisTitle("Mean Energy [GeV] ",1);
  
  hMinvPi0EB_ = dbe_->book1D("Pi0InvmassEB","Pi0 Invariant Mass in EB",100,0.,0.5);
  hMinvPi0EB_->setAxisTitle("Inv Mass [GeV] ",1);

  hMinvPi0EE_ = dbe_->book1D("Pi0InvmassEE","Pi0 Invariant Mass in EE",100,0.,0.5);
  hMinvPi0EE_->setAxisTitle("Inv Mass [GeV] ",1);
  
  hMinvEtaEB_ = dbe_->book1D("EtaInvmassEB","Eta Invariant Mass in EB",100,0.,0.85);
  hMinvEtaEB_->setAxisTitle("Inv Mass [GeV] ",1);

  hMinvEtaEE_ = dbe_->book1D("EtaInvmassEE","Eta Invariant Mass in EE",100,0.,0.85);
  hMinvEtaEE_->setAxisTitle("Inv Mass [GeV] ",1);

  
  hPt1Pi0EB_ = dbe_->book1D("Pt1Pi0EB","Pt 1st most energetic Pi0 photon in EB",100,0.,20.);
  hPt1Pi0EB_->setAxisTitle("1st photon Pt [GeV] ",1);
  
  hPt1Pi0EE_ = dbe_->book1D("Pt1Pi0EE","Pt 1st most energetic Pi0 photon in EE",100,0.,20.);
  hPt1Pi0EE_->setAxisTitle("1st photon Pt [GeV] ",1);

  hPt1EtaEB_ = dbe_->book1D("Pt1EtaEB","Pt 1st most energetic Eta photon in EB",100,0.,20.);
  hPt1EtaEB_->setAxisTitle("1st photon Pt [GeV] ",1);
  
  hPt1EtaEE_ = dbe_->book1D("Pt1EtaEE","Pt 1st most energetic Eta photon in EE",100,0.,20.);
  hPt1EtaEE_->setAxisTitle("1st photon Pt [GeV] ",1);
  
  hPt2Pi0EB_ = dbe_->book1D("Pt2Pi0EB","Pt 2nd most energetic Pi0 photon in EB",100,0.,20.);
  hPt2Pi0EB_->setAxisTitle("2nd photon Pt [GeV] ",1);

  hPt2Pi0EE_ = dbe_->book1D("Pt2Pi0EE","Pt 2nd most energetic Pi0 photon in EE",100,0.,20.);
  hPt2Pi0EE_->setAxisTitle("2nd photon Pt [GeV] ",1);

  hPt2EtaEB_ = dbe_->book1D("Pt2EtaEB","Pt 2nd most energetic Eta photon in EB",100,0.,20.);
  hPt2EtaEB_->setAxisTitle("2nd photon Pt [GeV] ",1);

  hPt2EtaEE_ = dbe_->book1D("Pt2EtaEE","Pt 2nd most energetic Eta photon in EE",100,0.,20.);
  hPt2EtaEE_->setAxisTitle("2nd photon Pt [GeV] ",1);

  
  hPtPi0EB_ = dbe_->book1D("PtPi0EB","Pi0 Pt in EB",100,0.,20.);
  hPtPi0EB_->setAxisTitle("Pi0 Pt [GeV] ",1);

  hPtPi0EE_ = dbe_->book1D("PtPi0EE","Pi0 Pt in EE",100,0.,20.);
  hPtPi0EE_->setAxisTitle("Pi0 Pt [GeV] ",1);

  hPtEtaEB_ = dbe_->book1D("PtEtaEB","Eta Pt in EB",100,0.,20.);
  hPtEtaEB_->setAxisTitle("Eta Pt [GeV] ",1);

  hPtEtaEE_ = dbe_->book1D("PtEtaEE","Eta Pt in EE",100,0.,20.);
  hPtEtaEE_->setAxisTitle("Eta Pt [GeV] ",1);

  hIsoPi0EB_ = dbe_->book1D("IsoPi0EB","Pi0 Iso in EB",50,0.,1.);
  hIsoPi0EB_->setAxisTitle("Pi0 Iso",1);

  hIsoPi0EE_ = dbe_->book1D("IsoPi0EE","Pi0 Iso in EE",50,0.,1.);
  hIsoPi0EE_->setAxisTitle("Pi0 Iso",1);

  hIsoEtaEB_ = dbe_->book1D("IsoEtaEB","Eta Iso in EB",50,0.,1.);
  hIsoEtaEB_->setAxisTitle("Eta Iso",1);

  hIsoEtaEE_ = dbe_->book1D("IsoEtaEE","Eta Iso in EE",50,0.,1.);
  hIsoEtaEE_->setAxisTitle("Eta Iso",1);

  hS4S91Pi0EB_ = dbe_->book1D("S4S91Pi0EB","S4S9 1st most energetic Pi0 photon in EB",50,0.,1.);
  hS4S91Pi0EB_->setAxisTitle("S4S9 of the 1st Pi0 Photon ",1);

  hS4S91Pi0EE_ = dbe_->book1D("S4S91Pi0EE","S4S9 1st most energetic Pi0 photon in EE",50,0.,1.);
  hS4S91Pi0EE_->setAxisTitle("S4S9 of the 1st Pi0 Photon ",1);

  hS4S91EtaEB_ = dbe_->book1D("S4S91EtaEB","S4S9 1st most energetic Eta photon in EB",50,0.,1.);
  hS4S91EtaEB_->setAxisTitle("S4S9 of the 1st Eta Photon ",1);

  hS4S91EtaEE_ = dbe_->book1D("S4S91EtaEE","S4S9 1st most energetic Eta photon in EE",50,0.,1.);
  hS4S91EtaEE_->setAxisTitle("S4S9 of the 1st Eta Photon ",1);

  hS4S92Pi0EB_ = dbe_->book1D("S4S92Pi0EB","S4S9 2nd most energetic Pi0 photon in EB",50,0.,1.);
  hS4S92Pi0EB_->setAxisTitle("S4S9 of the 2nd Pi0 Photon",1);

  hS4S92Pi0EE_ = dbe_->book1D("S4S92Pi0EE","S4S9 2nd most energetic Pi0 photon in EE",50,0.,1.);
  hS4S92Pi0EE_->setAxisTitle("S4S9 of the 2nd Pi0 Photon",1);

  hS4S92EtaEB_ = dbe_->book1D("S4S92EtaEB","S4S9 2nd most energetic Pi0 photon in EB",50,0.,1.);
  hS4S92EtaEB_->setAxisTitle("S4S9 of the 2nd Eta Photon",1);

  hS4S92EtaEE_ = dbe_->book1D("S4S92EtaEE","S4S9 2nd most energetic Pi0 photon in EE",50,0.,1.);
  hS4S92EtaEE_->setAxisTitle("S4S9 of the 2nd Eta Photon",1);

  


}

//--------------------------------------------------------
void HLTAlCaMonPi0::beginRun(const edm::Run& r, const EventSetup& context) {

}

//--------------------------------------------------------
void HLTAlCaMonPi0::beginLuminosityBlock(const LuminosityBlock& lumiSeg, 
     const EventSetup& context) {
  
}

//-------------------------------------------------------------

void HLTAlCaMonPi0::analyze(const Event& iEvent, 
			       const EventSetup& iSetup ){  
 
  if (eventCounter_% prescaleFactor_ ) return; 
  eventCounter_++;

  edm::ESHandle<CaloTopology> theCaloTopology;
  iSetup.get<CaloTopologyRecord>().get(theCaloTopology);


  std::vector<EcalRecHit> seeds;
  seeds.clear();

  std::vector<EBDetId> usedXtals;
  usedXtals.clear();

  detIdEBRecHits.clear(); //// EBDetId
  EBRecHits.clear();  /// EcalRecHit





    
  edm::Handle<EcalRecHitCollection> rhEBpi0;
  edm::Handle<EcalRecHitCollection> rhEBeta;
  edm::Handle<EcalRecHitCollection> rhEEpi0;
  edm::Handle<EcalRecHitCollection> rhEEeta;

 bool GetRecHitsCollectionEBpi0 = true;
 if(isMonEBpi0_) {
   try { 
     iEvent.getByToken(productMonitoredEBpi0Token_, rhEBpi0);
   }catch( cms::Exception& exception ) {
     LogDebug("HLTAlCaPi0DQMSource") << "no EcalRecHits EB_pi0, can not run all stuffs" << iEvent.eventAuxiliary ().event() <<" run: "<<iEvent.eventAuxiliary ().run()  << std::endl;
     GetRecHitsCollectionEBpi0 = false;
   }
 }

 bool GetRecHitsCollectionEBeta = true;
 if(isMonEBeta_) {
   try { 
    if(isMonEBeta_) iEvent.getByToken(productMonitoredEBetaToken_, rhEBeta);
    if(isMonEEpi0_) iEvent.getByToken(productMonitoredEEpi0Token_, rhEEpi0);
    if(isMonEEeta_) iEvent.getByToken(productMonitoredEEetaToken_, rhEEeta);
   }catch( cms::Exception& exception ) {
     LogDebug("HLTAlCaPi0DQMSource") << "no EcalRecHits EB_Eta, can not run all stuffs" << iEvent.eventAuxiliary ().event() <<" run: "<<iEvent.eventAuxiliary ().run()  << std::endl;
     GetRecHitsCollectionEBeta = false;
   }
 }

 bool GetRecHitsCollectionEEpi0 = true;
 if(isMonEEpi0_) {
   try { 
     if(isMonEEpi0_) iEvent.getByToken(productMonitoredEEpi0Token_, rhEEpi0);
   }catch( cms::Exception& exception ) {
     LogDebug("HLTAlCaPi0DQMSource") << "no EcalRecHits EE_Pi0, can not run all stuffs" << iEvent.eventAuxiliary ().event() <<" run: "<<iEvent.eventAuxiliary ().run()  << std::endl;
     GetRecHitsCollectionEEpi0 = false;
   }
 }

 bool GetRecHitsCollectionEEeta = true;
 if(isMonEEeta_) {
   try { 
     if(isMonEEeta_) iEvent.getByToken(productMonitoredEEetaToken_, rhEEeta);
   }catch( cms::Exception& exception ) {
     LogDebug("HLTAlCaPi0DQMSource") << "no EcalRecHits EE_Eta, can not run all stuffs" << iEvent.eventAuxiliary ().event() <<" run: "<<iEvent.eventAuxiliary ().run()  << std::endl;
     GetRecHitsCollectionEEeta = false;
   }
 }




  // Initialize the Position Calc
  const CaloSubdetectorGeometry *geometry_p; 
  const CaloSubdetectorGeometry *geometryEE_p;    
  const CaloSubdetectorGeometry *geometryES_p;
  
  const CaloSubdetectorTopology *topology_p;
  const CaloSubdetectorTopology *topology_ee;
  


  edm::ESHandle<CaloGeometry> geoHandle;
  iSetup.get<CaloGeometryRecord>().get(geoHandle);     
  geometry_p = geoHandle->getSubdetectorGeometry(DetId::Ecal,EcalBarrel);
  geometryEE_p = geoHandle->getSubdetectorGeometry(DetId::Ecal,EcalEndcap);
  geometryES_p = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalPreshower);
  topology_p = theCaloTopology->getSubdetectorTopology(DetId::Ecal,EcalBarrel);
  topology_ee = theCaloTopology->getSubdetectorTopology(DetId::Ecal,EcalEndcap);
  
  
  
  EcalRecHitCollection::const_iterator itb;
  
  // fill EB pi0 histos 
  if(isMonEBpi0_ ){
    if (rhEBpi0.isValid() && (rhEBpi0->size() > 0) && GetRecHitsCollectionEBpi0){


      const EcalRecHitCollection *hitCollection_p = rhEBpi0.product();
      float etot =0;
      for(itb=rhEBpi0->begin(); itb!=rhEBpi0->end(); ++itb){
	
	EBDetId id(itb->id());
	double energy = itb->energy();
	if( energy < seleXtalMinEnergy_) continue; 

	EBDetId det = itb->id();


	detIdEBRecHits.push_back(det);
	EBRecHits.push_back(*itb);

	if (energy > clusSeedThr_) seeds.push_back(*itb);

	hiPhiDistrEBpi0_->Fill(id.iphi());
	hiEtaDistrEBpi0_->Fill(id.ieta());
	hRechitEnergyEBpi0_->Fill(itb->energy());
	
	etot+= itb->energy();	 
      } // Eb rechits
      
      hNRecHitsEBpi0_->Fill(rhEBpi0->size());
      hMeanRecHitEnergyEBpi0_->Fill(etot/rhEBpi0->size());
      hEventEnergyEBpi0_->Fill(etot);

      //      std::cout << " EB RH Pi0 collection: #, mean rh_e, event E "<<rhEBpi0->size()<<" "<<etot/rhEBpi0->size()<<" "<<etot<<std::endl;   


      // Pi0 maker

      //std::cout<< " RH coll size: "<<rhEBpi0->size()<<std::endl;
      //std::cout<< " Pi0 seeds: "<<seeds.size()<<std::endl;

      int nClus;
      std::vector<float> eClus;
      std::vector<float> etClus;
      std::vector<float> etaClus; 
      std::vector<float> thetaClus;
      std::vector<float> phiClus;
      std::vector<EBDetId> max_hit;

      std::vector< std::vector<EcalRecHit> > RecHitsCluster;
      std::vector< std::vector<EcalRecHit> > RecHitsCluster5x5;
      std::vector<float> s4s9Clus;
      std::vector<float> s9s25Clus;


      nClus=0;


      // Make own simple clusters (3x3, 5x5 or clusPhiSize_ x clusEtaSize_)
      sort(seeds.begin(), seeds.end(), ecalRecHitLess());

      for (std::vector<EcalRecHit>::iterator itseed=seeds.begin(); itseed!=seeds.end(); itseed++) {
	EBDetId seed_id = itseed->id();
	std::vector<EBDetId>::const_iterator usedIds;

	bool seedAlreadyUsed=false;
	for(usedIds=usedXtals.begin(); usedIds!=usedXtals.end(); usedIds++){
	  if(*usedIds==seed_id){
	    seedAlreadyUsed=true;
	    //std::cout<< " Seed with energy "<<itseed->energy()<<" was used !"<<std::endl;
	    break; 
	  }
	}
	if(seedAlreadyUsed)continue;
	std::vector<DetId> clus_v = topology_p->getWindow(seed_id,clusEtaSize_,clusPhiSize_);       
	std::vector< std::pair<DetId, float> > clus_used;
	//Reject the seed if not able to build the cluster around it correctly
	//if(clus_v.size() < clusEtaSize_*clusPhiSize_){std::cout<<" Not enough RecHits "<<std::endl; continue;}
	vector<EcalRecHit> RecHitsInWindow;
	vector<EcalRecHit> RecHitsInWindow5x5;

	double simple_energy = 0; 

	for (std::vector<DetId>::iterator det=clus_v.begin(); det!=clus_v.end(); det++) {
	  EBDetId EBdet = *det;
	  //      std::cout<<" det "<< EBdet<<" ieta "<<EBdet.ieta()<<" iphi "<<EBdet.iphi()<<std::endl;
	  bool  HitAlreadyUsed=false;
	  for(usedIds=usedXtals.begin(); usedIds!=usedXtals.end(); usedIds++){
	    if(*usedIds==*det){
	      HitAlreadyUsed=true;
	      break;
	    }
	  }
	  if(HitAlreadyUsed)continue;


	  std::vector<EBDetId>::iterator itdet = find( detIdEBRecHits.begin(),detIdEBRecHits.end(),EBdet);
	  if(itdet == detIdEBRecHits.end()) continue; 
      
	  int nn = int(itdet - detIdEBRecHits.begin());
	  usedXtals.push_back(*det);
	  RecHitsInWindow.push_back(EBRecHits[nn]);
	  clus_used.push_back(std::pair<DetId, float>(*det, 1) );
	  simple_energy = simple_energy + EBRecHits[nn].energy();


	}

	if(simple_energy <= 0) continue;
   
	math::XYZPoint clus_pos = posCalculator_.Calculate_Location(clus_used,hitCollection_p,geometry_p,geometryES_p);
	//std::cout<< "       Simple Clustering: Total energy for this simple cluster : "<<simple_energy<<std::endl; 
	//std::cout<< "       Simple Clustering: eta phi : "<<clus_pos.eta()<<" "<<clus_pos.phi()<<std::endl; 
	//std::cout<< "       Simple Clustering: x y z : "<<clus_pos.x()<<" "<<clus_pos.y()<<" "<<clus_pos.z()<<std::endl; 

	float theta_s = 2. * atan(exp(-clus_pos.eta()));
	//	float p0x_s = simple_energy * sin(theta_s) * cos(clus_pos.phi());
	//float p0y_s = simple_energy * sin(theta_s) * sin(clus_pos.phi());
        //    float p0z_s = simple_energy * cos(theta_s);
	//float et_s = sqrt( p0x_s*p0x_s + p0y_s*p0y_s);
	
        float et_s = simple_energy * sin(theta_s);
	//std::cout << "       Simple Clustering: E,Et,px,py,pz: "<<simple_energy<<" "<<et_s<<" "<<p0x_s<<" "<<p0y_s<<" "<<std::endl;

	//Compute S4/S9 variable
	//We are not sure to have 9 RecHits so need to check eta and phi:
	///check s4s9
	float s4s9_tmp[4];
	for(int i=0;i<4;i++)s4s9_tmp[i]= 0;

	int seed_ieta = seed_id.ieta();
	int seed_iphi = seed_id.iphi();
    
	convxtalid( seed_iphi,seed_ieta);
    
        float e3x3 = 0; 
	float e5x5 = 0; 

	for(unsigned int j=0; j<RecHitsInWindow.size();j++){
	  EBDetId det = (EBDetId)RecHitsInWindow[j].id(); 
      
	  int ieta = det.ieta();
	  int iphi = det.iphi();
      
	  convxtalid(iphi,ieta);
      
	  float en = RecHitsInWindow[j].energy(); 
      
	  int dx = diff_neta_s(seed_ieta,ieta);
	  int dy = diff_nphi_s(seed_iphi,iphi);
      
	  if(dx <= 0 && dy <=0) s4s9_tmp[0] += en; 
	  if(dx >= 0 && dy <=0) s4s9_tmp[1] += en; 
	  if(dx <= 0 && dy >=0) s4s9_tmp[2] += en; 
	  if(dx >= 0 && dy >=0) s4s9_tmp[3] += en; 

	  if(abs(dx)<=1 && abs(dy)<=1) e3x3 += en; 
	  if(abs(dx)<=2 && abs(dy)<=2) e5x5 += en; 

      
	}
    
	if(e3x3 <= 0) continue;

	float s4s9_max = *max_element( s4s9_tmp,s4s9_tmp+4)/e3x3; 


    ///calculate e5x5
    std::vector<DetId> clus_v5x5 = topology_p->getWindow(seed_id,5,5); 
    for( std::vector<DetId>::const_iterator idItr = clus_v5x5.begin(); idItr != clus_v5x5.end(); idItr++){
      EBDetId det = *idItr;
      

      std::vector<EBDetId>::iterator itdet0 = find(usedXtals.begin(),usedXtals.end(),det);

      ///already clustered
      if(itdet0 != usedXtals.end()) continue; 
      
      //inside collections
      std::vector<EBDetId>::iterator itdet = find( detIdEBRecHits.begin(),detIdEBRecHits.end(),det);
      if(itdet == detIdEBRecHits.end()) continue; 

      int nn = int(itdet - detIdEBRecHits.begin());
      
      RecHitsInWindow5x5.push_back(EBRecHits[nn]);
      e5x5 += EBRecHits[nn].energy();
      
    }



	if(e5x5 <= 0) continue;

	    eClus.push_back(simple_energy);
	    etClus.push_back(et_s);
	    etaClus.push_back(clus_pos.eta());
	    thetaClus.push_back(theta_s);
	    phiClus.push_back(clus_pos.phi());
	    s4s9Clus.push_back(s4s9_max);
	    s9s25Clus.push_back(e3x3/e5x5);
	    RecHitsCluster.push_back(RecHitsInWindow);
	    RecHitsCluster5x5.push_back(RecHitsInWindow5x5);
	    
	    //	    std::cout<<" EB pi0 cluster (n,nxt,e,et eta,phi,s4s9) "<<nClus<<" "<<int(RecHitsInWindow.size())<<" "<<eClus[nClus]<<" "<<" "<<etClus[nClus]<<" "<<etaClus[nClus]<<" "<<phiClus[nClus]<<" "<<s4s9Clus[nClus]<<std::endl;

	    nClus++;


  }
    
      // std::cout<< " Pi0 clusters: "<<nClus<<std::endl;

      // Selection, based on Simple clustering
      //pi0 candidates
      int npi0_s=0;


      //      if (nClus <= 1) return;
      for(Int_t i=0 ; i<nClus ; i++){
	for(Int_t j=i+1 ; j<nClus ; j++){
	  //      std::cout<<" i "<<i<<"  etClus[i] "<<etClus[i]<<" j "<<j<<"  etClus[j] "<<etClus[j]<<std::endl;
	  if( etClus[i]>selePtGamma_ && etClus[j]>selePtGamma_ && s4s9Clus[i]>seleS4S9Gamma_ && s4s9Clus[j]>seleS4S9Gamma_){


          float p0x = etClus[i] * cos(phiClus[i]);
          float p1x = etClus[j] * cos(phiClus[j]);
          float p0y = etClus[i] * sin(phiClus[i]);
          float p1y = etClus[j] * sin(phiClus[j]);
          float p0z = eClus[i] * cos(thetaClus[i]);
          float p1z = eClus[j] * cos(thetaClus[j]);
        
        
          float pt_pair = sqrt( (p0x+p1x)*(p0x+p1x) + (p0y+p1y)*(p0y+p1y));

          if (pt_pair < selePtPi0_)continue;

	  float m_inv = sqrt ( (eClus[i] + eClus[j])*(eClus[i] + eClus[j]) - (p0x+p1x)*(p0x+p1x) - (p0y+p1y)*(p0y+p1y) - (p0z+p1z)*(p0z+p1z) );  
	    if ( (m_inv<seleMinvMaxPi0_) && (m_inv>seleMinvMinPi0_) ){

	      //New Loop on cluster to measure isolation:
	      std::vector<int> IsoClus;
	      IsoClus.clear();
	      float Iso = 0;
	      TVector3 pairVect = TVector3((p0x+p1x), (p0y+p1y), (p0z+p1z));
	      for(Int_t k=0 ; k<nClus ; k++){


		if(etClus[k] < ptMinForIsolation_) continue;

		if(k==i || k==j)continue;
              TVector3 ClusVect = TVector3(etClus[k] *cos(phiClus[k]), etClus[k] * sin(phiClus[k]) , eClus[k] * cos(thetaClus[k]));

		float dretacl = fabs(etaClus[k] - pairVect.Eta());
		float drcl = ClusVect.DeltaR(pairVect);
		//      std::cout<< "   Iso: k, E, drclpi0, detaclpi0, dphiclpi0 "<<k<<" "<<eClus[k]<<" "<<drclpi0<<" "<<dretaclpi0<<std::endl;
		if((drcl<selePi0BeltDR_) && (dretacl<selePi0BeltDeta_) ){
		  //              std::cout<< "   ... good iso cluster #: "<<k<<" etClus[k] "<<etClus[k] <<std::endl;
		  Iso = Iso + etClus[k];
		  IsoClus.push_back(k);
		}
	      }

	      //      std::cout<<"  Iso/pt_pi0 "<<Iso/pt_pi0<<std::endl;
	      if(Iso/pt_pair<selePi0Iso_){
		//for(unsigned int Rec=0;Rec<RecHitsCluster[i].size();Rec++)pi0EBRecHitCollection->push_back(RecHitsCluster[i][Rec]);
		//for(unsigned int Rec2=0;Rec2<RecHitsCluster[j].size();Rec2++)pi0EBRecHitCollection->push_back(RecHitsCluster[j][Rec2]);


		hMinvPi0EB_->Fill(m_inv);
		hPt1Pi0EB_->Fill(etClus[i]);
		hPt2Pi0EB_->Fill(etClus[j]);
		hPtPi0EB_->Fill(pt_pair);
		hIsoPi0EB_->Fill(Iso/pt_pair);
		hS4S91Pi0EB_->Fill(s4s9Clus[i]);
		hS4S92Pi0EB_->Fill(s4s9Clus[j]);
		
		//		cout <<"  EB Simple Clustering: pi0 Candidate pt, eta, phi, Iso, m_inv, i, j :   "<<pt_pair<<" "<<pairVect.Eta()<<" "<<pairVect.Phi()<<" "<<Iso<<" "<<m_inv<<" "<<i<<" "<<j<<" "<<std::endl;  

		npi0_s++;
		
	      }
            


          
	    }
	  }
	} // End of the "j" loop over Simple Clusters
      } // End of the "i" loop over Simple Clusters

      //      std::cout<<"  (Simple Clustering) EB Pi0 candidates #: "<<npi0_s<<std::endl;

    } // rhEBpi0.valid() ends

  } //  isMonEBpi0 ends

  //------------------ End of pi0 in EB --------------------------//

  // fill EB eta histos 
  if(isMonEBeta_ ){
    if (rhEBeta.isValid() && (rhEBeta->size() > 0) && GetRecHitsCollectionEBeta){


      const EcalRecHitCollection *hitCollection_p = rhEBeta.product();
      float etot =0;
      for(itb=rhEBeta->begin(); itb!=rhEBeta->end(); ++itb){
	
	EBDetId id(itb->id());
	double energy = itb->energy();
	if( energy < seleXtalMinEnergy_) continue; 

	EBDetId det = itb->id();


	detIdEBRecHits.push_back(det);
	EBRecHits.push_back(*itb);

	if (energy > clusSeedThr_) seeds.push_back(*itb);

	hiPhiDistrEBeta_->Fill(id.iphi());
	hiEtaDistrEBeta_->Fill(id.ieta());
	hRechitEnergyEBeta_->Fill(itb->energy());
	
	etot+= itb->energy();	 
      } // Eb rechits
      
      hNRecHitsEBeta_->Fill(rhEBeta->size());
      hMeanRecHitEnergyEBeta_->Fill(etot/rhEBeta->size());
      hEventEnergyEBeta_->Fill(etot);

      //      std::cout << " EB RH Eta collection: #, mean rh_e, event E "<<rhEBeta->size()<<" "<<etot/rhEBeta->size()<<" "<<etot<<std::endl;   


      // Eta maker

      //std::cout<< " RH coll size: "<<rhEBeta->size()<<std::endl;
      //std::cout<< " Eta seeds: "<<seeds.size()<<std::endl;

      int nClus;
      std::vector<float> eClus;
      std::vector<float> etClus;
      std::vector<float> etaClus; 
      std::vector<float> thetaClus;
      std::vector<float> phiClus;
      std::vector<EBDetId> max_hit;

      std::vector< std::vector<EcalRecHit> > RecHitsCluster;
      std::vector< std::vector<EcalRecHit> > RecHitsCluster5x5;
      std::vector<float> s4s9Clus;
      std::vector<float> s9s25Clus;


      nClus=0;

      // Make own simple clusters (3x3, 5x5 or clusPhiSize_ x clusEtaSize_)
      sort(seeds.begin(), seeds.end(), ecalRecHitLess());

      for (std::vector<EcalRecHit>::iterator itseed=seeds.begin(); itseed!=seeds.end(); itseed++) {
	EBDetId seed_id = itseed->id();
	std::vector<EBDetId>::const_iterator usedIds;

	bool seedAlreadyUsed=false;
	for(usedIds=usedXtals.begin(); usedIds!=usedXtals.end(); usedIds++){
	  if(*usedIds==seed_id){
	    seedAlreadyUsed=true;
	    //std::cout<< " Seed with energy "<<itseed->energy()<<" was used !"<<std::endl;
	    break; 
	  }
	}
	if(seedAlreadyUsed)continue;
	std::vector<DetId> clus_v = topology_p->getWindow(seed_id,clusEtaSize_,clusPhiSize_);       
	std::vector< std::pair<DetId, float> > clus_used;
	//Reject the seed if not able to build the cluster around it correctly
	//if(clus_v.size() < clusEtaSize_*clusPhiSize_){std::cout<<" Not enough RecHits "<<std::endl; continue;}
	vector<EcalRecHit> RecHitsInWindow;
	vector<EcalRecHit> RecHitsInWindow5x5;

	double simple_energy = 0; 

	for (std::vector<DetId>::iterator det=clus_v.begin(); det!=clus_v.end(); det++) {
	  EBDetId EBdet = *det;
	  //      std::cout<<" det "<< EBdet<<" ieta "<<EBdet.ieta()<<" iphi "<<EBdet.iphi()<<std::endl;
	  bool  HitAlreadyUsed=false;
	  for(usedIds=usedXtals.begin(); usedIds!=usedXtals.end(); usedIds++){
	    if(*usedIds==*det){
	      HitAlreadyUsed=true;
	      break;
	    }
	  }
	  if(HitAlreadyUsed)continue;


	  std::vector<EBDetId>::iterator itdet = find( detIdEBRecHits.begin(),detIdEBRecHits.end(),EBdet);
	  if(itdet == detIdEBRecHits.end()) continue; 
      
	  int nn = int(itdet - detIdEBRecHits.begin());
	  usedXtals.push_back(*det);
	  RecHitsInWindow.push_back(EBRecHits[nn]);
	  clus_used.push_back(std::pair<DetId, float>(*det, 1));
	  simple_energy = simple_energy + EBRecHits[nn].energy();


	}

	if(simple_energy <= 0) continue;
   
	math::XYZPoint clus_pos = posCalculator_.Calculate_Location(clus_used,hitCollection_p,geometry_p,geometryES_p);
	//std::cout<< "       Simple Clustering: Total energy for this simple cluster : "<<simple_energy<<std::endl; 
	//std::cout<< "       Simple Clustering: eta phi : "<<clus_pos.eta()<<" "<<clus_pos.phi()<<std::endl; 
	//std::cout<< "       Simple Clustering: x y z : "<<clus_pos.x()<<" "<<clus_pos.y()<<" "<<clus_pos.z()<<std::endl; 

	float theta_s = 2. * atan(exp(-clus_pos.eta()));
	//	float p0x_s = simple_energy * sin(theta_s) * cos(clus_pos.phi());
	//float p0y_s = simple_energy * sin(theta_s) * sin(clus_pos.phi());
        //    float p0z_s = simple_energy * cos(theta_s);
	//float et_s = sqrt( p0x_s*p0x_s + p0y_s*p0y_s);
	
        float et_s = simple_energy * sin(theta_s);
	//std::cout << "       Simple Clustering: E,Et,px,py,pz: "<<simple_energy<<" "<<et_s<<" "<<p0x_s<<" "<<p0y_s<<" "<<std::endl;

	//Compute S4/S9 variable
	//We are not sure to have 9 RecHits so need to check eta and phi:
	///check s4s9
	float s4s9_tmp[4];
	for(int i=0;i<4;i++)s4s9_tmp[i]= 0;

	int seed_ieta = seed_id.ieta();
	int seed_iphi = seed_id.iphi();
    
	convxtalid( seed_iphi,seed_ieta);
    
        float e3x3 = 0; 
	float e5x5 = 0; 

	for(unsigned int j=0; j<RecHitsInWindow.size();j++){
	  EBDetId det = (EBDetId)RecHitsInWindow[j].id(); 
      
	  int ieta = det.ieta();
	  int iphi = det.iphi();
      
	  convxtalid(iphi,ieta);
      
	  float en = RecHitsInWindow[j].energy(); 
      
	  int dx = diff_neta_s(seed_ieta,ieta);
	  int dy = diff_nphi_s(seed_iphi,iphi);
      
	  if(dx <= 0 && dy <=0) s4s9_tmp[0] += en; 
	  if(dx >= 0 && dy <=0) s4s9_tmp[1] += en; 
	  if(dx <= 0 && dy >=0) s4s9_tmp[2] += en; 
	  if(dx >= 0 && dy >=0) s4s9_tmp[3] += en; 

	  if(abs(dx)<=1 && abs(dy)<=1) e3x3 += en; 
	  if(abs(dx)<=2 && abs(dy)<=2) e5x5 += en; 

      
	}
    
	if(e3x3 <= 0) continue;

	float s4s9_max = *max_element( s4s9_tmp,s4s9_tmp+4)/e3x3; 


    ///calculate e5x5
    std::vector<DetId> clus_v5x5 = topology_p->getWindow(seed_id,5,5); 
    for( std::vector<DetId>::const_iterator idItr = clus_v5x5.begin(); idItr != clus_v5x5.end(); idItr++){
      EBDetId det = *idItr;
      

      std::vector<EBDetId>::iterator itdet0 = find(usedXtals.begin(),usedXtals.end(),det);

      ///already clustered
      if(itdet0 != usedXtals.end()) continue; 
      
      //inside collections
      std::vector<EBDetId>::iterator itdet = find( detIdEBRecHits.begin(),detIdEBRecHits.end(),det);
      if(itdet == detIdEBRecHits.end()) continue; 

      int nn = int(itdet - detIdEBRecHits.begin());
      
      RecHitsInWindow5x5.push_back(EBRecHits[nn]);
      e5x5 += EBRecHits[nn].energy();
      
    }



	if(e5x5 <= 0) continue;

	    eClus.push_back(simple_energy);
	    etClus.push_back(et_s);
	    etaClus.push_back(clus_pos.eta());
	    thetaClus.push_back(theta_s);
	    phiClus.push_back(clus_pos.phi());
	    s4s9Clus.push_back(s4s9_max);
	    s9s25Clus.push_back(e3x3/e5x5);
	    RecHitsCluster.push_back(RecHitsInWindow);
	    RecHitsCluster5x5.push_back(RecHitsInWindow5x5);
	    
	    //	    std::cout<<" EB Eta cluster (n,nxt,e,et eta,phi,s4s9) "<<nClus<<" "<<int(RecHitsInWindow.size())<<" "<<eClus[nClus]<<" "<<" "<<etClus[nClus]<<" "<<etaClus[nClus]<<" "<<phiClus[nClus]<<" "<<s4s9Clus[nClus]<<std::endl;

	    nClus++;
	    

  }
    
      // std::cout<< " Eta clusters: "<<nClus<<std::endl;

      // Selection, based on Simple clustering
      //eta candidates
      int npi0_s=0;


      //      if (nClus <= 1) return;
      for(Int_t i=0 ; i<nClus ; i++){
	for(Int_t j=i+1 ; j<nClus ; j++){
	  //      std::cout<<" i "<<i<<"  etClus[i] "<<etClus[i]<<" j "<<j<<"  etClus[j] "<<etClus[j]<<std::endl;
	  if( etClus[i]>selePtGammaEta_ && etClus[j]>selePtGammaEta_ && s4s9Clus[i]>seleS4S9GammaEta_ && s4s9Clus[j]>seleS4S9GammaEta_){


          float p0x = etClus[i] * cos(phiClus[i]);
          float p1x = etClus[j] * cos(phiClus[j]);
          float p0y = etClus[i] * sin(phiClus[i]);
          float p1y = etClus[j] * sin(phiClus[j]);
          float p0z = eClus[i] * cos(thetaClus[i]);
          float p1z = eClus[j] * cos(thetaClus[j]);
        
        
          float pt_pair = sqrt( (p0x+p1x)*(p0x+p1x) + (p0y+p1y)*(p0y+p1y));

          if (pt_pair < selePtEta_)continue;

	  float m_inv = sqrt ( (eClus[i] + eClus[j])*(eClus[i] + eClus[j]) - (p0x+p1x)*(p0x+p1x) - (p0y+p1y)*(p0y+p1y) - (p0z+p1z)*(p0z+p1z) );  
	    if ( (m_inv<seleMinvMaxEta_) && (m_inv>seleMinvMinEta_) ){

	      //New Loop on cluster to measure isolation:
	      std::vector<int> IsoClus;
	      IsoClus.clear();
	      float Iso = 0;
	      TVector3 pairVect = TVector3((p0x+p1x), (p0y+p1y), (p0z+p1z));
	      for(Int_t k=0 ; k<nClus ; k++){


		if(etClus[k] < ptMinForIsolationEta_) continue;

		if(k==i || k==j)continue;
              TVector3 ClusVect = TVector3(etClus[k] *cos(phiClus[k]), etClus[k] * sin(phiClus[k]) , eClus[k] * cos(thetaClus[k]));

		float dretacl = fabs(etaClus[k] - pairVect.Eta());
		float drcl = ClusVect.DeltaR(pairVect);
		//      std::cout<< "   Iso: k, E, drclpi0, detaclpi0, dphiclpi0 "<<k<<" "<<eClus[k]<<" "<<drclpi0<<" "<<dretaclpi0<<std::endl;
		if((drcl<seleEtaBeltDR_) && (dretacl<seleEtaBeltDeta_) ){
		  //              std::cout<< "   ... good iso cluster #: "<<k<<" etClus[k] "<<etClus[k] <<std::endl;
		  Iso = Iso + etClus[k];
		  IsoClus.push_back(k);
		}
	      }

	      //      std::cout<<"  Iso/pt_pi0 "<<Iso/pt_pi0<<std::endl;
	      if(Iso/pt_pair<seleEtaIso_){
		//for(unsigned int Rec=0;Rec<RecHitsCluster[i].size();Rec++)pi0EBRecHitCollection->push_back(RecHitsCluster[i][Rec]);
		//for(unsigned int Rec2=0;Rec2<RecHitsCluster[j].size();Rec2++)pi0EBRecHitCollection->push_back(RecHitsCluster[j][Rec2]);


		hMinvEtaEB_->Fill(m_inv);
		hPt1EtaEB_->Fill(etClus[i]);
		hPt2EtaEB_->Fill(etClus[j]);
		hPtEtaEB_->Fill(pt_pair);
		hIsoEtaEB_->Fill(Iso/pt_pair);
		hS4S91EtaEB_->Fill(s4s9Clus[i]);
		hS4S92EtaEB_->Fill(s4s9Clus[j]);
		
		//		cout <<"  EB Simple Clustering: Eta Candidate pt, eta, phi, Iso, m_inv, i, j :   "<<pt_pair<<" "<<pairVect.Eta()<<" "<<pairVect.Phi()<<" "<<Iso<<" "<<m_inv<<" "<<i<<" "<<j<<" "<<std::endl;  

		npi0_s++;
		
	      }
            


          
	    }
	  }
	} // End of the "j" loop over Simple Clusters
      } // End of the "i" loop over Simple Clusters

      //      std::cout<<"  (Simple Clustering) EB Eta candidates #: "<<npi0_s<<std::endl;

    } // rhEBeta.valid() ends

  } //  isMonEBeta ends

  //------------------ End of Eta in EB --------------------------//



      //----------------- End of the EB --------------------------//




      //----------------- Start of the EE --------------------//

      // fill pi0 EE histos
      if(isMonEEpi0_){
	if (rhEEpi0.isValid() && (rhEEpi0->size() > 0) && GetRecHitsCollectionEEpi0){

	  const EcalRecHitCollection *hitCollection_ee = rhEEpi0.product();
	  float etot =0;


      detIdEERecHits.clear(); //// EEDetId
      EERecHits.clear();  /// EcalRecHit


      std::vector<EcalRecHit> seedsEndCap;
      seedsEndCap.clear();

      std::vector<EEDetId> usedXtalsEndCap;
      usedXtalsEndCap.clear();


	  ////make seeds. 
      EERecHitCollection::const_iterator ite;
      for (ite=rhEEpi0->begin(); ite!=rhEEpi0->end(); ite++) {
	double energy = ite->energy();
	if( energy < seleXtalMinEnergyEndCap_) continue; 
    
	EEDetId det = ite->id();
	EEDetId id(ite->id());

    
	detIdEERecHits.push_back(det);
	EERecHits.push_back(*ite);
    
	hiXDistrEEpi0_->Fill(id.ix());
	hiYDistrEEpi0_->Fill(id.iy());
	hRechitEnergyEEpi0_->Fill(ite->energy());

	if (energy > clusSeedThrEndCap_) seedsEndCap.push_back(*ite);
	
	etot+= ite->energy();	 
      } // EE rechits
      
      hNRecHitsEEpi0_->Fill(rhEEpi0->size());
      hMeanRecHitEnergyEEpi0_->Fill(etot/rhEEpi0->size());
      hEventEnergyEEpi0_->Fill(etot);

      //      std::cout << " EE RH Pi0 collection: #, mean rh_e, event E "<<rhEEpi0->size()<<" "<<etot/rhEEpi0->size()<<" "<<etot<<std::endl;   
  
      int nClusEndCap;
      std::vector<float> eClusEndCap;
      std::vector<float> etClusEndCap;
      std::vector<float> etaClusEndCap;
      std::vector<float> thetaClusEndCap;
      std::vector<float> phiClusEndCap;
      std::vector< std::vector<EcalRecHit> > RecHitsClusterEndCap;
      std::vector< std::vector<EcalRecHit> > RecHitsCluster5x5EndCap;
      std::vector<float> s4s9ClusEndCap;
      std::vector<float> s9s25ClusEndCap;


      nClusEndCap=0;
  
  
      // Make own simple clusters (3x3, 5x5 or clusPhiSize_ x clusEtaSize_)
      sort(seedsEndCap.begin(), seedsEndCap.end(), ecalRecHitLess());
  
      for (std::vector<EcalRecHit>::iterator itseed=seedsEndCap.begin(); itseed!=seedsEndCap.end(); itseed++) {
	EEDetId seed_id = itseed->id();
	std::vector<EEDetId>::const_iterator usedIds;
    
	bool seedAlreadyUsed=false;
	for(usedIds=usedXtalsEndCap.begin(); usedIds!=usedXtalsEndCap.end(); usedIds++){
	  if(*usedIds==seed_id){
	    seedAlreadyUsed=true;
	    break; 
	  }
	}

	if(seedAlreadyUsed)continue;
	std::vector<DetId> clus_v = topology_ee->getWindow(seed_id,clusEtaSize_,clusPhiSize_);      
	std::vector< std::pair<DetId, float> > clus_used;   

	vector<EcalRecHit> RecHitsInWindow;
	vector<EcalRecHit> RecHitsInWindow5x5;
    
	float simple_energy = 0; 

	for (std::vector<DetId>::iterator det=clus_v.begin(); det!=clus_v.end(); det++) {
	  EEDetId EEdet = *det;
      
	  bool  HitAlreadyUsed=false;
	  for(usedIds=usedXtalsEndCap.begin(); usedIds!=usedXtalsEndCap.end(); usedIds++){
	    if(*usedIds==*det){
	      HitAlreadyUsed=true;
	      break;
	    }
	  }
     
	  if(HitAlreadyUsed)continue;
      
	  std::vector<EEDetId>::iterator itdet = find( detIdEERecHits.begin(),detIdEERecHits.end(),EEdet);
	  if(itdet == detIdEERecHits.end()) continue; 
      
      
	  int nn = int(itdet - detIdEERecHits.begin());
	  usedXtalsEndCap.push_back(*det);
	  RecHitsInWindow.push_back(EERecHits[nn]);
	  clus_used.push_back(std::pair<DetId, float>(*det, 1));
	  simple_energy = simple_energy + EERecHits[nn].energy();
      
      
	}
    
	if( simple_energy <= 0) continue;
        
	math::XYZPoint clus_pos = posCalculator_.Calculate_Location(clus_used,hitCollection_ee,geometryEE_p,geometryES_p);
    

	float theta_s = 2. * atan(exp(-clus_pos.eta()));
	float et_s = simple_energy * sin(theta_s);
	//	float p0x_s = simple_energy * sin(theta_s) * cos(clus_pos.phi());
	//float p0y_s = simple_energy * sin(theta_s) * sin(clus_pos.phi());
	//float et_s = sqrt( p0x_s*p0x_s + p0y_s*p0y_s);
    
    
	//Compute S4/S9 variable
	//We are not sure to have 9 RecHits so need to check eta and phi:
	float s4s9_tmp[4];
	for(int i=0;i<4;i++) s4s9_tmp[i]= 0; 
    
	int ixSeed = seed_id.ix();
	int iySeed = seed_id.iy();
	float e3x3 = 0; 
	float e5x5 = 0;

	for(unsigned int j=0; j<RecHitsInWindow.size();j++){
	  EEDetId det_this = (EEDetId)RecHitsInWindow[j].id(); 
	  int dx = ixSeed - det_this.ix();
	  int dy = iySeed - det_this.iy();
      
	  float en = RecHitsInWindow[j].energy(); 
      
	  if(dx <= 0 && dy <=0) s4s9_tmp[0] += en; 
	  if(dx >= 0 && dy <=0) s4s9_tmp[1] += en; 
	  if(dx <= 0 && dy >=0) s4s9_tmp[2] += en; 
	  if(dx >= 0 && dy >=0) s4s9_tmp[3] += en; 

	  if( abs(dx)<=1 && abs(dy)<=1) e3x3 += en; 
	  if( abs(dx)<=2 && abs(dy)<=2) e5x5 += en; 

	}

      
	if(e3x3 <= 0) continue;

	eClusEndCap.push_back(simple_energy);
	etClusEndCap.push_back(et_s);
	etaClusEndCap.push_back(clus_pos.eta());
	thetaClusEndCap.push_back(theta_s);
	phiClusEndCap.push_back(clus_pos.phi());
	s4s9ClusEndCap.push_back(*max_element( s4s9_tmp,s4s9_tmp+4)/e3x3);
	s9s25ClusEndCap.push_back(e3x3/e5x5);
	RecHitsClusterEndCap.push_back(RecHitsInWindow);
	RecHitsCluster5x5EndCap.push_back(RecHitsInWindow5x5);
	

	//	std::cout<<" EE pi0 cluster (n,nxt,e,et eta,phi,s4s9) "<<nClusEndCap<<" "<<int(RecHitsInWindow.size())<<" "<<eClusEndCap[nClusEndCap]<<" "<<" "<<etClusEndCap[nClusEndCap]<<" "<<etaClusEndCap[nClusEndCap]<<" "<<phiClusEndCap[nClusEndCap]<<" "<<s4s9ClusEndCap[nClusEndCap]<<std::endl;


	nClusEndCap++;


      }



      // Selection, based on Simple clustering
      //pi0 candidates
      int npi0_se=0;


      for(Int_t i=0 ; i<nClusEndCap ; i++){
	for(Int_t j=i+1 ; j<nClusEndCap ; j++){
      
	  if( etClusEndCap[i]>selePtGammaEndCap_ && etClusEndCap[j]>selePtGammaEndCap_ && s4s9ClusEndCap[i]>seleS4S9GammaEndCap_ && s4s9ClusEndCap[j]>seleS4S9GammaEndCap_){

          float p0x = etClusEndCap[i] * cos(phiClusEndCap[i]);
          float p1x = etClusEndCap[j] * cos(phiClusEndCap[j]);
          float p0y = etClusEndCap[i] * sin(phiClusEndCap[i]);
          float p1y = etClusEndCap[j] * sin(phiClusEndCap[j]);
          float p0z = eClusEndCap[i] * cos(thetaClusEndCap[i]);
          float p1z = eClusEndCap[j] * cos(thetaClusEndCap[j]);

        
	    float pt_pair = sqrt( (p0x+p1x)*(p0x+p1x) + (p0y+p1y)*(p0y+p1y));
	    if (pt_pair < selePtPi0EndCap_)continue;
	    float m_inv = sqrt ( (eClusEndCap[i] + eClusEndCap[j])*(eClusEndCap[i] + eClusEndCap[j]) - (p0x+p1x)*(p0x+p1x) - (p0y+p1y)*(p0y+p1y) - (p0z+p1z)*(p0z+p1z) );  
 

	    if ( (m_inv<seleMinvMaxPi0EndCap_) && (m_inv>seleMinvMinPi0EndCap_) ){
          

	      //New Loop on cluster to measure isolation:
	      std::vector<int> IsoClus;
	      IsoClus.clear();
	      float Iso = 0;
	      TVector3 pairVect = TVector3((p0x+p1x), (p0y+p1y), (p0z+p1z));
	      for(Int_t k=0 ; k<nClusEndCap ; k++){

		if(etClusEndCap[k] < ptMinForIsolationEndCap_) continue; 
            
            
		if(k==i || k==j)continue;
            
            
              TVector3 clusVect = TVector3(etClusEndCap[k] * cos(phiClusEndCap[k]), etClusEndCap[k] * sin(phiClusEndCap[k]) , eClusEndCap[k] * cos(thetaClusEndCap[k]) ) ;
              float dretacl = fabs(etaClusEndCap[k] - pairVect.Eta());
              float drcl = clusVect.DeltaR(pairVect);
              
              if(drcl < selePi0BeltDREndCap_ && dretacl < selePi0BeltDetaEndCap_ ){
                Iso = Iso + etClusEndCap[k];
                IsoClus.push_back(k);
              }
            }

          
	      if(Iso/pt_pair<selePi0IsoEndCap_){
		//std::cout <<"  EE Simple Clustering: pi0 Candidate pt, eta, phi, Iso, m_inv, i, j :   "<<pt_pair<<" "<<pairVect.Eta()<<" "<<pairVect.Phi()<<" "<<Iso<<" "<<m_inv<<" "<<i<<" "<<j<<" "<<std::endl;  

		hMinvPi0EE_->Fill(m_inv);
		hPt1Pi0EE_->Fill(etClusEndCap[i]);
		hPt2Pi0EE_->Fill(etClusEndCap[j]);
		hPtPi0EE_->Fill(pt_pair);
		hIsoPi0EE_->Fill(Iso/pt_pair);
		hS4S91Pi0EE_->Fill(s4s9ClusEndCap[i]);
		hS4S92Pi0EE_->Fill(s4s9ClusEndCap[j]);

		npi0_se++;
	      }
          
	    }
	  }
	} // End of the "j" loop over Simple Clusters
      } // End of the "i" loop over Simple Clusters

      //      std::cout<<"  (Simple Clustering) EE Pi0 candidates #: "<<npi0_se<<std::endl;

	} //rhEEpi0
      } //isMonEEpi0

      //================End of Pi0 endcap=======================//


      //================ Eta in EE===============================//

      // fill pi0 EE histos
      if(isMonEEeta_){
	if (rhEEeta.isValid() && (rhEEeta->size() > 0) && GetRecHitsCollectionEEeta){

	  const EcalRecHitCollection *hitCollection_ee = rhEEeta.product();
	  float etot =0;

      detIdEERecHits.clear(); //// EEDetId
      EERecHits.clear();  /// EcalRecHit


      std::vector<EcalRecHit> seedsEndCap;
      seedsEndCap.clear();

      std::vector<EEDetId> usedXtalsEndCap;
      usedXtalsEndCap.clear();


	  ////make seeds. 
      EERecHitCollection::const_iterator ite;
      for (ite=rhEEeta->begin(); ite!=rhEEeta->end(); ite++) {
	double energy = ite->energy();
	if( energy < seleXtalMinEnergyEndCap_) continue; 
    
	EEDetId det = ite->id();
	EEDetId id(ite->id());

    
	detIdEERecHits.push_back(det);
	EERecHits.push_back(*ite);
    
	hiXDistrEEeta_->Fill(id.ix());
	hiYDistrEEeta_->Fill(id.iy());
	hRechitEnergyEEeta_->Fill(ite->energy());

	if (energy > clusSeedThrEndCap_) seedsEndCap.push_back(*ite);
	
	etot+= ite->energy();	 
      } // EE rechits
      
      hNRecHitsEEeta_->Fill(rhEEeta->size());
      hMeanRecHitEnergyEEeta_->Fill(etot/rhEEeta->size());
      hEventEnergyEEeta_->Fill(etot);

      //      std::cout << " EE RH Eta collection: #, mean rh_e, event E "<<rhEEeta->size()<<" "<<etot/rhEEeta->size()<<" "<<etot<<std::endl;   
  
      int nClusEndCap;
      std::vector<float> eClusEndCap;
      std::vector<float> etClusEndCap;
      std::vector<float> etaClusEndCap;
      std::vector<float> thetaClusEndCap;
      std::vector<float> phiClusEndCap;
      std::vector< std::vector<EcalRecHit> > RecHitsClusterEndCap;
      std::vector< std::vector<EcalRecHit> > RecHitsCluster5x5EndCap;
      std::vector<float> s4s9ClusEndCap;
      std::vector<float> s9s25ClusEndCap;


      nClusEndCap=0;
  
  
      // Make own simple clusters (3x3, 5x5 or clusPhiSize_ x clusEtaSize_)
      sort(seedsEndCap.begin(), seedsEndCap.end(), ecalRecHitLess());
  
      for (std::vector<EcalRecHit>::iterator itseed=seedsEndCap.begin(); itseed!=seedsEndCap.end(); itseed++) {
	EEDetId seed_id = itseed->id();
	std::vector<EEDetId>::const_iterator usedIds;
    
	bool seedAlreadyUsed=false;
	for(usedIds=usedXtalsEndCap.begin(); usedIds!=usedXtalsEndCap.end(); usedIds++){
	  if(*usedIds==seed_id){
	    seedAlreadyUsed=true;
	    break; 
	  }
	}

	if(seedAlreadyUsed)continue;
	std::vector<DetId> clus_v = topology_ee->getWindow(seed_id,clusEtaSize_,clusPhiSize_);      
	std::vector< std::pair<DetId, float> > clus_used;

	vector<EcalRecHit> RecHitsInWindow;
	vector<EcalRecHit> RecHitsInWindow5x5;
    
	float simple_energy = 0; 

	for (std::vector<DetId>::iterator det=clus_v.begin(); det!=clus_v.end(); det++) {
	  EEDetId EEdet = *det;
      
	  bool  HitAlreadyUsed=false;
	  for(usedIds=usedXtalsEndCap.begin(); usedIds!=usedXtalsEndCap.end(); usedIds++){
	    if(*usedIds==*det){
	      HitAlreadyUsed=true;
	      break;
	    }
	  }
     
	  if(HitAlreadyUsed)continue;
      
	  std::vector<EEDetId>::iterator itdet = find( detIdEERecHits.begin(),detIdEERecHits.end(),EEdet);
	  if(itdet == detIdEERecHits.end()) continue; 
      
      
	  int nn = int(itdet - detIdEERecHits.begin());
	  usedXtalsEndCap.push_back(*det);
	  RecHitsInWindow.push_back(EERecHits[nn]);
	  clus_used.push_back(std::pair<DetId, float>(*det, 1));
	  simple_energy = simple_energy + EERecHits[nn].energy();
      
      
	}
    
	if( simple_energy <= 0) continue;
        
	math::XYZPoint clus_pos = posCalculator_.Calculate_Location(clus_used,hitCollection_ee,geometryEE_p,geometryES_p);
    

	float theta_s = 2. * atan(exp(-clus_pos.eta()));
	float et_s = simple_energy * sin(theta_s);
	//	float p0x_s = simple_energy * sin(theta_s) * cos(clus_pos.phi());
	//float p0y_s = simple_energy * sin(theta_s) * sin(clus_pos.phi());
	//float et_s = sqrt( p0x_s*p0x_s + p0y_s*p0y_s);
    
    
	//Compute S4/S9 variable
	//We are not sure to have 9 RecHits so need to check eta and phi:
	float s4s9_tmp[4];
	for(int i=0;i<4;i++) s4s9_tmp[i]= 0; 
    
	int ixSeed = seed_id.ix();
	int iySeed = seed_id.iy();
	float e3x3 = 0; 
	float e5x5 = 0;

	for(unsigned int j=0; j<RecHitsInWindow.size();j++){
	  EEDetId det_this = (EEDetId)RecHitsInWindow[j].id(); 
	  int dx = ixSeed - det_this.ix();
	  int dy = iySeed - det_this.iy();
      
	  float en = RecHitsInWindow[j].energy(); 
      
	  if(dx <= 0 && dy <=0) s4s9_tmp[0] += en; 
	  if(dx >= 0 && dy <=0) s4s9_tmp[1] += en; 
	  if(dx <= 0 && dy >=0) s4s9_tmp[2] += en; 
	  if(dx >= 0 && dy >=0) s4s9_tmp[3] += en; 

	  if( abs(dx)<=1 && abs(dy)<=1) e3x3 += en; 
	  if( abs(dx)<=2 && abs(dy)<=2) e5x5 += en; 

	}

      
	if(e3x3 <= 0) continue;

	eClusEndCap.push_back(simple_energy);
	etClusEndCap.push_back(et_s);
	etaClusEndCap.push_back(clus_pos.eta());
	thetaClusEndCap.push_back(theta_s);
	phiClusEndCap.push_back(clus_pos.phi());
	s4s9ClusEndCap.push_back(*max_element( s4s9_tmp,s4s9_tmp+4)/e3x3);
	s9s25ClusEndCap.push_back(e3x3/e5x5);
	RecHitsClusterEndCap.push_back(RecHitsInWindow);
	RecHitsCluster5x5EndCap.push_back(RecHitsInWindow5x5);
	
	//	std::cout<<" EE Eta cluster (n,nxt,e,et eta,phi,s4s9) "<<nClusEndCap<<" "<<int(RecHitsInWindow.size())<<" "<<eClusEndCap[nClusEndCap]<<" "<<" "<<etClusEndCap[nClusEndCap]<<" "<<etaClusEndCap[nClusEndCap]<<" "<<phiClusEndCap[nClusEndCap]<<" "<<s4s9ClusEndCap[nClusEndCap]<<std::endl;

	nClusEndCap++;

      }



      // Selection, based on Simple clustering
      //pi0 candidates
      int npi0_se=0;


      for(Int_t i=0 ; i<nClusEndCap ; i++){
	for(Int_t j=i+1 ; j<nClusEndCap ; j++){
      
	  if( etClusEndCap[i]>selePtGammaEtaEndCap_ && etClusEndCap[j]>selePtGammaEtaEndCap_ && s4s9ClusEndCap[i]>seleS4S9GammaEtaEndCap_ && s4s9ClusEndCap[j]>seleS4S9GammaEtaEndCap_){

          float p0x = etClusEndCap[i] * cos(phiClusEndCap[i]);
          float p1x = etClusEndCap[j] * cos(phiClusEndCap[j]);
          float p0y = etClusEndCap[i] * sin(phiClusEndCap[i]);
          float p1y = etClusEndCap[j] * sin(phiClusEndCap[j]);
          float p0z = eClusEndCap[i] * cos(thetaClusEndCap[i]);
          float p1z = eClusEndCap[j] * cos(thetaClusEndCap[j]);

        
	    float pt_pair = sqrt( (p0x+p1x)*(p0x+p1x) + (p0y+p1y)*(p0y+p1y));
	    if (pt_pair < selePtEtaEndCap_)continue;
	    float m_inv = sqrt ( (eClusEndCap[i] + eClusEndCap[j])*(eClusEndCap[i] + eClusEndCap[j]) - (p0x+p1x)*(p0x+p1x) - (p0y+p1y)*(p0y+p1y) - (p0z+p1z)*(p0z+p1z) );  
 

	    if ( (m_inv<seleMinvMaxEtaEndCap_) && (m_inv>seleMinvMinEtaEndCap_) ){
          

	      //New Loop on cluster to measure isolation:
	      std::vector<int> IsoClus;
	      IsoClus.clear();
	      float Iso = 0;
	      TVector3 pairVect = TVector3((p0x+p1x), (p0y+p1y), (p0z+p1z));
	      for(Int_t k=0 ; k<nClusEndCap ; k++){

		if(etClusEndCap[k] < ptMinForIsolationEtaEndCap_) continue; 
            
            
		if(k==i || k==j)continue;
            
            
              TVector3 clusVect = TVector3(etClusEndCap[k] * cos(phiClusEndCap[k]), etClusEndCap[k] * sin(phiClusEndCap[k]) , eClusEndCap[k] * cos(thetaClusEndCap[k]) ) ;
              float dretacl = fabs(etaClusEndCap[k] - pairVect.Eta());
              float drcl = clusVect.DeltaR(pairVect);
              
              if(drcl < seleEtaBeltDREndCap_ && dretacl < seleEtaBeltDetaEndCap_ ){
                Iso = Iso + etClusEndCap[k];
                IsoClus.push_back(k);
              }
            }

          
	      if(Iso/pt_pair<seleEtaIsoEndCap_){
		//	cout <<"  EE Simple Clustering: Eta Candidate pt, eta, phi, Iso, m_inv, i, j :   "<<pt_pair<<" "<<pairVect.Eta()<<" "<<pairVect.Phi()<<" "<<Iso<<" "<<m_inv<<" "<<i<<" "<<j<<" "<<std::endl;  

		hMinvEtaEE_->Fill(m_inv);
		hPt1EtaEE_->Fill(etClusEndCap[i]);
		hPt2EtaEE_->Fill(etClusEndCap[j]);
		hPtEtaEE_->Fill(pt_pair);
		hIsoEtaEE_->Fill(Iso/pt_pair);
		hS4S91EtaEE_->Fill(s4s9ClusEndCap[i]);
		hS4S92EtaEE_->Fill(s4s9ClusEndCap[j]);

		npi0_se++;
	      }
          
	    }
	  }
	} // End of the "j" loop over Simple Clusters
      } // End of the "i" loop over Simple Clusters

      //      std::cout<<"  (Simple Clustering) EE Eta candidates #: "<<npi0_se<<std::endl;

	} //rhEEeta
      } //isMonEEeta

      //================End of Pi0 endcap=======================//




      ////==============End of endcap ===============///


} 




//--------------------------------------------------------
void HLTAlCaMonPi0::endLuminosityBlock(const LuminosityBlock& lumiSeg, 
                                          const EventSetup& context) {
}
//--------------------------------------------------------
void HLTAlCaMonPi0::endRun(const Run& r, const EventSetup& context){

}
//--------------------------------------------------------
void HLTAlCaMonPi0::endJob(){

  if(dbe_) {  
    if (saveToFile_) {
      dbe_->save(fileName_);
    }
  }
}


void HLTAlCaMonPi0::convxtalid(Int_t &nphi,Int_t &neta)
{
  // Barrel only
  // Output nphi 0...359; neta 0...84; nside=+1 (for eta>0), or 0 (for eta<0).
  // neta will be [-85,-1] , or [0,84], the minus sign indicates the z<0 side.
  
  if(neta > 0) neta -= 1;
  if(nphi > 359) nphi=nphi-360;
  
  // final check
  if(nphi >359 || nphi <0 || neta< -85 || neta > 84)
    {
      std::cout <<" unexpected fatal error in HLTPi0RecHitsFilter::convxtalid "<<  nphi <<  " " << neta <<  " " 
		<<std::endl;
      //exit(1);
    }
} //end of convxtalid

int HLTAlCaMonPi0::diff_neta_s(Int_t neta1, Int_t neta2){
  Int_t mdiff;
  mdiff=(neta1-neta2);
  return mdiff;
}

// Calculate the distance in xtals taking into account the periodicity of the Barrel
int HLTAlCaMonPi0::diff_nphi_s(Int_t nphi1,Int_t nphi2) {
  Int_t mdiff;
  if(abs(nphi1-nphi2) < (360-abs(nphi1-nphi2))) {
    mdiff=nphi1-nphi2;
  }
  else {
    mdiff=360-abs(nphi1-nphi2);
    if(nphi1>nphi2) mdiff=-mdiff;
  }
  return mdiff;
}


