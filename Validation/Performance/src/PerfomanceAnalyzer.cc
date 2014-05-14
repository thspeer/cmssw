
#include "Validation/Performance/interface/PerformanceAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
// #include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/HLTReco/interface/ModuleTiming.h"
//#include "DataFormats/Common/interface/ModuleDescription.h"
#include "DataFormats/Provenance/interface/ModuleDescription.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include <iostream>
#include "DQMServices/Core/interface/DQMStore.h"

PerformanceAnalyzer::PerformanceAnalyzer(const edm::ParameterSet& ps)
    : fDBE( edm::Service<DQMStore>().operator->() ),
      fOutputFile( ps.getUntrackedParameter<std::string>("outputFile", "") )
{
  eventTime_Token_ = consumes<edm::EventTime> (
      edm::InputTag("cputest"));

  if ( fDBE != NULL )
  {
    fDBE->setCurrentFolder("PerformanceV/CPUPerformanceTask");

    // is there any way to record CPU Info ???
    // if so, it can be done once - via beginJob()

    fVtxSmeared   = fDBE->book1D( "VtxSmeared",  "VtxSmeared",   100, 0., 0.1 ) ;
    fg4SimHits    = fDBE->book1D( "g4SimHits",   "g4SimHits",    100, 0., 500. ) ;
    fMixing       = fDBE->book1D( "Mixing",      "Mixing",       100, 0., 0.5 ) ;
    fSiPixelDigis = fDBE->book1D( "SiPixelDigis","SiPixelDigis", 100, 0., 0.5 ) ;
    fSiStripDigis = fDBE->book1D( "SiStripDigis","SiStripDigis", 100, 0., 2. ) ;
    fEcalUnsuppDigis = fDBE->book1D( "EcalUnsuppDigis","EcalUnsuppDigis", 100, 0., 2. ) ;
    fEcalZeroSuppDigis = fDBE->book1D( "EcalZeroSuppDigis","EcalZeroSuppDigis", 100, 0., 2. ) ;
    fPreShwZeroSuppDigis = fDBE->book1D( "PreShwZeroSuppDigis","PreShwZeroSuppDigis", 100, 0., 0.1 ) ;
    fHcalUnsuppDigis = fDBE->book1D( "HcalUnsuppDigis","HcalUnsuppDigis", 100, 0., 0.5 ) ;
    fMuonCSCDigis = fDBE->book1D( "MuonCSCDigis", "MuonCSCDigis", 100, 0., 0.1 ) ;
    fMuonDTDigis  = fDBE->book1D( "MuonDTDigis",  "MuonDTDigis",  100, 0., 0.1 ) ;
    fMuonRPCDigis = fDBE->book1D( "MuonRPCDigis", "MuonRPCDigis", 100, 0., 0.1 ) ;
  }
}


PerformanceAnalyzer::~PerformanceAnalyzer()
{
  // don't try to delete any pointers - they're handled by DQM machinery
}


void PerformanceAnalyzer::endJob()
{

  if ( fOutputFile.size() != 0 && fDBE ) fDBE->save(fOutputFile);

  return ;

}


void PerformanceAnalyzer::analyze(const edm::Event& e, const edm::EventSetup& eventSetup)
{

  if ( e.id().event() == 1) return ; // skip 1st event

  edm::Handle<edm::EventTime> EvtTime;
  e.getByToken(eventTime_Token_, EvtTime ) ;

  for ( unsigned int i=0; i<EvtTime->size(); ++i )
  {
    if ( EvtTime->name(i) == "VtxSmeared" )   fVtxSmeared->Fill( EvtTime->time(i) ) ;
    if ( EvtTime->name(i) == "g4SimHits"  )   fg4SimHits->Fill( EvtTime->time(i) ) ;
    if ( EvtTime->name(i) == "mix" )          fMixing->Fill( EvtTime->time(i) ) ;
    if ( EvtTime->name(i) == "siPixelDigis" ) fSiPixelDigis->Fill( EvtTime->time(i) ) ;
    if ( EvtTime->name(i) == "siStripDigis" ) fSiStripDigis->Fill( EvtTime->time(i) ) ;
    if ( EvtTime->name(i) == "ecalUnsuppressedDigis" ) fEcalUnsuppDigis->Fill( EvtTime->time(i) ) ;
    if ( EvtTime->name(i) == "ecalDigis")     fEcalZeroSuppDigis->Fill( EvtTime->time(i) ) ;
    if ( EvtTime->name(i) == "ecalPreshowerDigis") fPreShwZeroSuppDigis->Fill( EvtTime->time(i) ) ;
    if ( EvtTime->name(i) == "hcalDigis" )    fHcalUnsuppDigis->Fill( EvtTime->time(i) ) ;
    if ( EvtTime->name(i) == "muonCSCDigis" ) fMuonCSCDigis->Fill( EvtTime->time(i) ) ;
    if ( EvtTime->name(i) == "muonDTDigis" )  fMuonDTDigis->Fill( EvtTime->time(i) ) ;
    if ( EvtTime->name(i) == "muonRPCDigis" ) fMuonRPCDigis->Fill( EvtTime->time(i) ) ;

  }

  return ;

}

DEFINE_FWK_MODULE(PerformanceAnalyzer);

