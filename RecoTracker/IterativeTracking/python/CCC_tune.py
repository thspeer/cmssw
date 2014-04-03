import FWCore.ParameterSet.Config as cms

from RecoTracker.IterativeTracking.InitialStep_cff import *

initialStepClusters.doStripChargeCheck = cms.bool(False)
initialStepChi2Est.minGoodStripCharge = cms.double(1724)

from RecoTracker.IterativeTracking.DetachedTripletStep_cff import *

detachedTripletStepClusters.doStripChargeCheck = cms.bool(False)
detachedTripletStepChi2Est.minGoodStripCharge = cms.double(2069)

from RecoTracker.IterativeTracking.LowPtTripletStep_cff import *

lowPtTripletStepChi2Est.minGoodStripCharge = cms.double(2069)

from RecoTracker.IterativeTracking.PixelPairStep_cff import *

pixelPairStepChi2Est.minGoodStripCharge = cms.double(2069)

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
pixelLessStepSeedClusters.doStripChargeCheck = cms.bool(False)

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
tobTecStepSeedClusters.doStripChargeCheck = cms.bool(False)
