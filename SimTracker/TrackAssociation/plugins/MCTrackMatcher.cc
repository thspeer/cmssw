/** \class MCTrackMatcher 
 *
 * \author Luca Lista, INFN
 *
 *
 */
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Association.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorBase.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
using namespace edm;
using namespace std;
using namespace reco;

namespace edm { class ParameterSet; }

class MCTrackMatcher : public edm::EDProducer {
 public:
  /// constructor
  MCTrackMatcher( const edm::ParameterSet & );

 private:
  void produce( edm::Event& evt, const edm::EventSetup& es ) override;
  std::string associator_;
  edm::EDGetTokenT<edm::View<reco::Track> > trToken;
  edm::EDGetTokenT<TrackingParticleCollection> tpToken;
  edm::EDGetTokenT<vector<int> > bcToken;
  edm::EDGetTokenT<GenParticleCollection> gpToken;
  typedef edm::Association<reco::GenParticleCollection> GenParticleMatch;
};


MCTrackMatcher::MCTrackMatcher(const ParameterSet & p) :
  associator_(p.getParameter<string>("associator"))
{
  trToken = consumes<edm::View<reco::Track> >(p.getParameter< edm::InputTag >("tracks"));
  tpToken = consumes<TrackingParticleCollection>(p.getParameter< edm::InputTag >("label_tp"));
  bcToken = consumes<vector<int> >(p.getParameter< edm::InputTag >("label_tr"));
  gpToken = consumes<GenParticleCollection>(p.getParameter< edm::InputTag >("label_tp"));
  produces<GenParticleMatch>();
}

void MCTrackMatcher::produce(Event& evt, const EventSetup& es) {
  ESHandle<TrackAssociatorBase> assoc;  
  es.get<TrackAssociatorRecord>().get(associator_,assoc);
  const TrackAssociatorBase * associator = assoc.product();
  Handle<View<Track> > tracks;
  evt.getByToken(trToken, tracks);
  Handle<TrackingParticleCollection> trackingParticles;
  evt.getByToken(tpToken,trackingParticles);
  Handle<vector<int> > barCodes;
  evt.getByToken(bcToken,barCodes );
  Handle<GenParticleCollection> genParticles;
  evt.getByToken(gpToken, genParticles );
  RecoToSimCollection associations = associator->associateRecoToSim ( tracks, trackingParticles, & evt, &es ); 
  auto_ptr<GenParticleMatch> match(new GenParticleMatch(GenParticleRefProd(genParticles)));
  GenParticleMatch::Filler filler(*match);
  size_t n = tracks->size();
  vector<int> indices(n,-1);
  for (size_t i = 0; i < n; ++ i ) {
    RefToBase<Track> track(tracks, i);
    RecoToSimCollection::const_iterator f = associations.find(track);
    if ( f != associations.end() ) {
      TrackingParticleRef tp = f->val.front().first;
      TrackingParticle::genp_iterator j, b = tp->genParticle_begin(), e = tp->genParticle_end();
      for( j = b; j != e; ++ j ) {
	const reco::GenParticle * p = j->get();
	if (p->status() == 1) {
	  indices[i] = j->key();
	  break;
	}
      }
    }
  }
  filler.insert(tracks, indices.begin(), indices.end());
  filler.fill();
  evt.put(match);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE( MCTrackMatcher );

