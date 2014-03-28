import FWCore.ParameterSet.Config as cms

from RecoTracker.IterativeTracking.MixedTripletStep_cff import *

mixedTripletStepChi2Est.minGoodStripCharge = cms.double(2069)

import RecoPixelVertexing.PixelLowPtUtilities.ClusterShapeHitFilterESProducer_cfi
mixedTripletStepClusterShapeHitFilter  = RecoPixelVertexing.PixelLowPtUtilities.ClusterShapeHitFilterESProducer_cfi.ClusterShapeHitFilterESProducer.clone(
	ComponentName = cms.string('mixedTripletStepClusterShapeHitFilter'),
        PixelShapeFile= cms.string('RecoPixelVertexing/PixelLowPtUtilities/data/pixelShape.par'),
	minGoodStripCharge = cms.double(2069)
	)

mixedTripletStepSeedsA.SeedComparitorPSet.ClusterShapeHitFilterName = cms.string('mixedTripletStepClusterShapeHitFilter')
mixedTripletStepSeedsB.SeedComparitorPSet.ClusterShapeHitFilterName = cms.string('mixedTripletStepClusterShapeHitFilter')
