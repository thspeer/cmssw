#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "TrackingTools/KalmanUpdators/interface/Chi2ChargeMeasurementEstimator.h"
#include "TrackingTools/PatternTools/interface/MeasurementExtractor.h"

#include "DataFormats/TrackerRecHit2D/interface/BaseTrackerRecHit.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2D.h"
#include "AnalysisDataFormats/SiStripClusterInfo/interface/SiStripClusterInfo.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"


bool Chi2ChargeMeasurementEstimator::checkClusterCharge(const OmniClusterRef::ClusterStripRef cluster, float chargeCut) const
{
  int clusCharge=accumulate( cluster->amplitudes().begin(), cluster->amplitudes().end(), uint16_t(0));
  return (clusCharge>chargeCut);
}

bool Chi2ChargeMeasurementEstimator::checkCharge(const TransientTrackingRecHit& aRecHit,
	int subdet, float chargeCut) const
{
    
  if (subdet<3) {
    const SiPixelRecHit *hit = dynamic_cast<const SiPixelRecHit *>(aRecHit.hit());
    if (likely(hit!=0)) return (hit->cluster()->charge()>chargeCut);
    else{
      const std::type_info &type = typeid(aRecHit);
      throw cms::Exception("Unknown RecHit Type") << "checkCharge: Wrong recHit type: " << type.name()
	<< " (SiPixelRecHit) expected";
    }
  } else {
    if (aRecHit.transientHits().size() == 2) { //  This is a matched RecHit
        const SiStripMatchedRecHit2D *matchHit = static_cast<const SiStripMatchedRecHit2D *>(aRecHit.hit());
        return (checkClusterCharge(matchHit->monoClusterRef().cluster_strip(), chargeCut)
          &&checkClusterCharge(matchHit->stereoClusterRef().cluster_strip(), chargeCut));
    } else {
        const BaseTrackerRecHit* hit = dynamic_cast<const BaseTrackerRecHit*>(aRecHit.hit());
        if likely(hit!=0) return checkClusterCharge(hit->firstClusterRef().cluster_strip(), chargeCut);//    cluster = &*(hit->cluster());
	else{
	  const std::type_info &type = typeid(aRecHit);
	  throw cms::Exception("Unknown RecHit Type") << "checkCharge: Wrong recHit type: " << type.name();
	}
    }
  }

  throw cms::Exception("Unknown RecHit Type") << "checkCharge: Unknown hit";

}



float Chi2ChargeMeasurementEstimator::sensorThickness (const DetId& detid) const
{
  if (detid.subdetId()>2) {
    SiStripDetId siStripDetId = detid(); 
    if (siStripDetId.subdetId()==SiStripDetId::TOB) return 0.047;
    if (siStripDetId.moduleGeometry()==SiStripDetId::W5 || siStripDetId.moduleGeometry()==SiStripDetId::W6 ||
        siStripDetId.moduleGeometry()==SiStripDetId::W7)
	return 0.047;
    return 0.029; // so it is TEC ring 1-4 or TIB or TOB;
  } else if (detid.subdetId()==1) return 0.0285;
  else return 0.027;
}


std::pair<bool,double> 
Chi2ChargeMeasurementEstimator::estimate(const TrajectoryStateOnSurface& tsos,
				   const TransientTrackingRecHit& aRecHit) const {



  std::pair<bool,double> estimateResult = Chi2MeasurementEstimator::estimate(tsos, aRecHit);
  if ( !estimateResult.first || (!(cutOnStripCharge_||cutOnStripCharge_))) return estimateResult;

  SiStripDetId detid = aRecHit.geographicalId(); 
  uint32_t subdet = detid.subdetId();
  if((detid.det()==1) && (((subdet>2)&&cutOnStripCharge_) || ((subdet<3)&&cutOnPixelCharge_)))   {

    float theDxdz = tsos.localParameters().dxdz();
    float theDydz = tsos.localParameters().dydz();
    float chargeCut = (float) minGoodCharge(subdet) * sensorThickness(detid)*(sqrt(1. + theDxdz*theDxdz + theDydz*theDydz));

    if (!checkCharge(aRecHit, subdet, chargeCut)) return HitReturnType(false,0.);
    else return estimateResult;
  }

  return estimateResult;
}

