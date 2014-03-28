import FWCore.ParameterSet.Config as cms

from RecoTracker.IterativeTracking.PixelLessStep_cff import *

pixelLessStepChi2Est.minGoodStripCharge = cms.double(2069)

import RecoPixelVertexing.PixelLowPtUtilities.ClusterShapeHitFilterESProducer_cfi
pixelLessStepClusterShapeHitFilter  = RecoPixelVertexing.PixelLowPtUtilities.ClusterShapeHitFilterESProducer_cfi.ClusterShapeHitFilterESProducer.clone(
	ComponentName = cms.string('pixelLessStepClusterShapeHitFilter'),
        PixelShapeFile= cms.string('RecoPixelVertexing/PixelLowPtUtilities/data/pixelShape.par'),
	minGoodStripCharge = cms.double(2069)
	)

pixelLessStepSeeds.SeedComparitorPSet.FilterStripHits = cms.bool(True)
pixelLessStepSeeds.SeedComparitorPSet.ClusterShapeHitFilterName = cms.string('pixelLessStepClusterShapeHitFilter')

pixelLessStepSeedClusters.Common.minGoodStripCharge = cms.double(60.0)
