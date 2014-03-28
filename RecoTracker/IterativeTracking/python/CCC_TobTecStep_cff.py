import FWCore.ParameterSet.Config as cms

from RecoTracker.IterativeTracking.TobTecStep_cff import *

tobTecStepChi2Est.minGoodStripCharge = cms.double(2069)

import RecoPixelVertexing.PixelLowPtUtilities.ClusterShapeHitFilterESProducer_cfi
tobTecStepClusterShapeHitFilter  = RecoPixelVertexing.PixelLowPtUtilities.ClusterShapeHitFilterESProducer_cfi.ClusterShapeHitFilterESProducer.clone(
	ComponentName = cms.string('tobTecStepClusterShapeHitFilter'),
        PixelShapeFile= cms.string('RecoPixelVertexing/PixelLowPtUtilities/data/pixelShape.par'),
	minGoodStripCharge = cms.double(2069)
	)

tobTecStepSeedsTripl.SeedComparitorPSet.FilterStripHits = cms.bool(True)
tobTecStepSeedsTripl.SeedComparitorPSet.ClusterShapeHitFilterName = cms.string('tobTecStepClusterShapeHitFilter')
tobTecStepSeedsPair.SeedComparitorPSet.FilterStripHits = cms.bool(True)
tobTecStepSeedsPair.SeedComparitorPSet.ClusterShapeHitFilterName = cms.string('tobTecStepClusterShapeHitFilter')
tobTecStepSeedClusters.Common.minGoodStripCharge = cms.double(60.0)
