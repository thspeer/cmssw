#ifndef DQM_SiPixelMonitorClient_SiPixelDaqInfo_H
#define DQM_SiPixelMonitorClient_SiPixelDaqInfo_H

// system include files
#include <memory>
#include <iostream>
#include <fstream>

// FWCore
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

//DQM
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"

class SiPixelDaqInfo : public edm::EDAnalyzer {
public:
  explicit SiPixelDaqInfo(const edm::ParameterSet&);
  ~SiPixelDaqInfo();
  

private:
  virtual void beginJob() ;
  virtual void beginLuminosityBlock(const edm::LuminosityBlock& , const  edm::EventSetup&);
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endLuminosityBlock(const edm::LuminosityBlock& , const  edm::EventSetup&);
  virtual void endRun(const edm::Run& , const  edm::EventSetup&);
  virtual void endJob() ;
  
  DQMStore *dbe_;  
  
  MonitorElement*  Fraction_;
  MonitorElement*  FractionBarrel_;
  MonitorElement*  FractionEndcap_;

  std::pair<int,int> FEDRange_;

  int  NumberOfFeds_;
  
  int NEvents_;
  int nFEDsBarrel_;
  int nFEDsEndcap_;
  std::string daqSource_;
  int FEDs_[40];

  //define Token(-s)
  edm::EDGetTokenT<FEDRawDataCollection> daqSourceToken_;
};

#endif
