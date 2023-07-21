//===- llvm/InitializePasses.h - Initialize All Passes ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations for the pass initialization routines
// for the entire LLVM project.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_INITIALIZEPASSES_H
#define LLVM_INITIALIZEPASSES_H

#include "llvm/Support/Compiler.h"

namespace llvm {

class PassRegistry;

/// Initialize all passes linked into the Core library.
LLVM_ABI void initializeCore(PassRegistry&);

/// Initialize all passes linked into the TransformUtils library.
LLVM_ABI void initializeTransformUtils(PassRegistry&);

/// Initialize all passes linked into the ScalarOpts library.
LLVM_ABI void initializeScalarOpts(PassRegistry&);

/// Initialize all passes linked into the Vectorize library.
LLVM_ABI void initializeVectorization(PassRegistry&);

/// Initialize all passes linked into the InstCombine library.
LLVM_ABI void initializeInstCombine(PassRegistry&);

/// Initialize all passes linked into the IPO library.
LLVM_ABI void initializeIPO(PassRegistry&);

/// Initialize all passes linked into the Analysis library.
LLVM_ABI void initializeAnalysis(PassRegistry&);

/// Initialize all passes linked into the CodeGen library.
LLVM_ABI void initializeCodeGen(PassRegistry&);

/// Initialize all passes linked into the GlobalISel library.
LLVM_ABI void initializeGlobalISel(PassRegistry&);

/// Initialize all passes linked into the CodeGen library.
LLVM_ABI void initializeTarget(PassRegistry&);

LLVM_ABI void initializeAAEvalLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeAAResultsWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeAlwaysInlinerLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeAssignmentTrackingAnalysisPass(PassRegistry &);
LLVM_ABI void initializeAssumeBuilderPassLegacyPassPass(PassRegistry &);
LLVM_ABI void initializeAssumptionCacheTrackerPass(PassRegistry&);
LLVM_ABI void initializeAtomicExpandPass(PassRegistry&);
LLVM_ABI void initializeBasicBlockSectionsProfileReaderPass(PassRegistry &);
LLVM_ABI void initializeBasicBlockSectionsPass(PassRegistry &);
LLVM_ABI void initializeBarrierNoopPass(PassRegistry&);
LLVM_ABI void initializeBasicAAWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeBlockFrequencyInfoWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeBranchFolderPassPass(PassRegistry&);
LLVM_ABI void initializeBranchProbabilityInfoWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeBranchRelaxationPass(PassRegistry&);
LLVM_ABI void initializeBreakCriticalEdgesPass(PassRegistry&);
LLVM_ABI void initializeBreakFalseDepsPass(PassRegistry&);
LLVM_ABI void initializeCanonicalizeFreezeInLoopsPass(PassRegistry &);
LLVM_ABI void initializeCFGOnlyPrinterLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeCFGOnlyViewerLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeCFGPrinterLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeCFGSimplifyPassPass(PassRegistry&);
LLVM_ABI void initializeCFGuardPass(PassRegistry&);
LLVM_ABI void initializeCFGuardLongjmpPass(PassRegistry&);
LLVM_ABI void initializeCFGViewerLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeCFIFixupPass(PassRegistry&);
LLVM_ABI void initializeCFIInstrInserterPass(PassRegistry&);
LLVM_ABI void initializeCallBrPreparePass(PassRegistry &);
LLVM_ABI void initializeCallGraphDOTPrinterPass(PassRegistry&);
LLVM_ABI void initializeCallGraphPrinterLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeCallGraphViewerPass(PassRegistry&);
LLVM_ABI void initializeCallGraphWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeCheckDebugMachineModulePass(PassRegistry &);
LLVM_ABI void initializeCodeGenPreparePass(PassRegistry&);
LLVM_ABI void initializeComplexDeinterleavingLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeConstantHoistingLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeCostModelAnalysisPass(PassRegistry&);
LLVM_ABI void initializeCycleInfoWrapperPassPass(PassRegistry &);
LLVM_ABI void initializeDAEPass(PassRegistry&);
LLVM_ABI void initializeDAHPass(PassRegistry&);
LLVM_ABI void initializeDCELegacyPassPass(PassRegistry&);
LLVM_ABI void initializeDeadMachineInstructionElimPass(PassRegistry&);
LLVM_ABI void initializeDebugifyMachineModulePass(PassRegistry &);
LLVM_ABI void initializeDelinearizationPass(PassRegistry&);
LLVM_ABI void initializeDependenceAnalysisWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeDetectDeadLanesPass(PassRegistry&);
LLVM_ABI void initializeDomOnlyPrinterWrapperPassPass(PassRegistry &);
LLVM_ABI void initializeDomOnlyViewerWrapperPassPass(PassRegistry &);
LLVM_ABI void initializeDomPrinterWrapperPassPass(PassRegistry &);
LLVM_ABI void initializeDomViewerWrapperPassPass(PassRegistry &);
LLVM_ABI void initializeDominanceFrontierWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeDominatorTreeWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeDwarfEHPrepareLegacyPassPass(PassRegistry &);
LLVM_ABI void initializeEarlyCSELegacyPassPass(PassRegistry&);
LLVM_ABI void initializeEarlyCSEMemSSALegacyPassPass(PassRegistry&);
LLVM_ABI void initializeEarlyIfConverterPass(PassRegistry&);
LLVM_ABI void initializeEarlyIfPredicatorPass(PassRegistry &);
LLVM_ABI void initializeEarlyMachineLICMPass(PassRegistry&);
LLVM_ABI void initializeEarlyTailDuplicatePass(PassRegistry&);
LLVM_ABI void initializeEdgeBundlesPass(PassRegistry&);
LLVM_ABI void initializeEHContGuardCatchretPass(PassRegistry &);
LLVM_ABI void initializeExpandLargeFpConvertLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeExpandLargeDivRemLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeExpandMemCmpPassPass(PassRegistry&);
LLVM_ABI void initializeExpandPostRAPass(PassRegistry&);
LLVM_ABI void initializeExpandReductionsPass(PassRegistry&);
LLVM_ABI void initializeExpandVectorPredicationPass(PassRegistry &);
LLVM_ABI void initializeMakeGuardsExplicitLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeExternalAAWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeFEntryInserterPass(PassRegistry&);
LLVM_ABI void initializeFinalizeISelPass(PassRegistry&);
LLVM_ABI void initializeFinalizeMachineBundlesPass(PassRegistry&);
LLVM_ABI void initializeFixIrreduciblePass(PassRegistry &);
LLVM_ABI void initializeFixupStatepointCallerSavedPass(PassRegistry&);
LLVM_ABI void initializeFlattenCFGLegacyPassPass(PassRegistry &);
LLVM_ABI void initializeFuncletLayoutPass(PassRegistry&);
LLVM_ABI void initializeGCMachineCodeAnalysisPass(PassRegistry&);
LLVM_ABI void initializeGCModuleInfoPass(PassRegistry&);
LLVM_ABI void initializeGVNLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeGlobalMergePass(PassRegistry&);
LLVM_ABI void initializeGlobalsAAWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeGuardWideningLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeHardwareLoopsLegacyPass(PassRegistry&);
LLVM_ABI void initializeMIRProfileLoaderPassPass(PassRegistry &);
LLVM_ABI void initializeIRSimilarityIdentifierWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeIRTranslatorPass(PassRegistry&);
LLVM_ABI void initializeIVUsersWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeIfConverterPass(PassRegistry&);
LLVM_ABI void initializeImmutableModuleSummaryIndexWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeImplicitNullChecksPass(PassRegistry&);
LLVM_ABI void initializeIndirectBrExpandPassPass(PassRegistry&);
LLVM_ABI void initializeInferAddressSpacesPass(PassRegistry&);
LLVM_ABI void initializeInstCountLegacyPassPass(PassRegistry &);
LLVM_ABI void initializeInstSimplifyLegacyPassPass(PassRegistry &);
LLVM_ABI void initializeInstructionCombiningPassPass(PassRegistry&);
LLVM_ABI void initializeInstructionSelectPass(PassRegistry&);
LLVM_ABI void initializeInterleavedAccessPass(PassRegistry&);
LLVM_ABI void initializeInterleavedLoadCombinePass(PassRegistry &);
LLVM_ABI void initializeIntervalPartitionPass(PassRegistry&);
LLVM_ABI void initializeJMCInstrumenterPass(PassRegistry&);
LLVM_ABI void initializeKCFIPass(PassRegistry &);
LLVM_ABI void initializeLCSSAVerificationPassPass(PassRegistry&);
LLVM_ABI void initializeLCSSAWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeLazyBlockFrequencyInfoPassPass(PassRegistry&);
LLVM_ABI void initializeLazyBranchProbabilityInfoPassPass(PassRegistry&);
LLVM_ABI void initializeLazyMachineBlockFrequencyInfoPassPass(PassRegistry&);
LLVM_ABI void initializeLazyValueInfoPrinterPass(PassRegistry&);
LLVM_ABI void initializeLazyValueInfoWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeLegacyLICMPassPass(PassRegistry&);
LLVM_ABI void initializeLegacyLoopSinkPassPass(PassRegistry&);
LLVM_ABI void initializeLegalizerPass(PassRegistry&);
LLVM_ABI void initializeGISelCSEAnalysisWrapperPassPass(PassRegistry &);
LLVM_ABI void initializeGISelKnownBitsAnalysisPass(PassRegistry &);
LLVM_ABI void initializeLiveDebugValuesPass(PassRegistry&);
LLVM_ABI void initializeLiveDebugVariablesPass(PassRegistry&);
LLVM_ABI void initializeLiveIntervalsPass(PassRegistry&);
LLVM_ABI void initializeLiveRangeShrinkPass(PassRegistry&);
LLVM_ABI void initializeLiveRegMatrixPass(PassRegistry&);
LLVM_ABI void initializeLiveStacksPass(PassRegistry&);
LLVM_ABI void initializeLiveVariablesPass(PassRegistry &);
LLVM_ABI void initializeLoadStoreOptPass(PassRegistry &);
LLVM_ABI void initializeLoadStoreVectorizerLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeLocalStackSlotPassPass(PassRegistry&);
LLVM_ABI void initializeLocalizerPass(PassRegistry&);
LLVM_ABI void initializeLoopDataPrefetchLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeLoopExtractorLegacyPassPass(PassRegistry &);
LLVM_ABI void initializeLoopGuardWideningLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeLoopInfoWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeLoopInstSimplifyLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeLoopPassPass(PassRegistry&);
LLVM_ABI void initializeLoopPredicationLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeLoopRotateLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeLoopSimplifyCFGLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeLoopSimplifyPass(PassRegistry&);
LLVM_ABI void initializeLoopStrengthReducePass(PassRegistry&);
LLVM_ABI void initializeLoopUnrollPass(PassRegistry&);
LLVM_ABI void initializeLowerAtomicLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeLowerConstantIntrinsicsPass(PassRegistry&);
LLVM_ABI void initializeLowerEmuTLSPass(PassRegistry&);
LLVM_ABI void initializeLowerExpectIntrinsicPass(PassRegistry&);
LLVM_ABI void initializeLowerGlobalDtorsLegacyPassPass(PassRegistry &);
LLVM_ABI void initializeLowerGuardIntrinsicLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeLowerWidenableConditionLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeLowerIntrinsicsPass(PassRegistry&);
LLVM_ABI void initializeLowerInvokeLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeLowerSwitchLegacyPassPass(PassRegistry &);
LLVM_ABI void initializeKCFIPass(PassRegistry &);
LLVM_ABI void initializeMIRAddFSDiscriminatorsPass(PassRegistry &);
LLVM_ABI void initializeMIRCanonicalizerPass(PassRegistry &);
LLVM_ABI void initializeMIRNamerPass(PassRegistry &);
LLVM_ABI void initializeMIRPrintingPassPass(PassRegistry&);
LLVM_ABI void initializeMachineBlockFrequencyInfoPass(PassRegistry&);
LLVM_ABI void initializeMachineBlockPlacementPass(PassRegistry&);
LLVM_ABI void initializeMachineBlockPlacementStatsPass(PassRegistry&);
LLVM_ABI void initializeMachineBranchProbabilityInfoPass(PassRegistry&);
LLVM_ABI void initializeMachineCFGPrinterPass(PassRegistry &);
LLVM_ABI void initializeMachineCSEPass(PassRegistry&);
LLVM_ABI void initializeMachineCombinerPass(PassRegistry&);
LLVM_ABI void initializeMachineCopyPropagationPass(PassRegistry&);
LLVM_ABI void initializeMachineCycleInfoPrinterPassPass(PassRegistry &);
LLVM_ABI void initializeMachineCycleInfoWrapperPassPass(PassRegistry &);
LLVM_ABI void initializeMachineDominanceFrontierPass(PassRegistry&);
LLVM_ABI void initializeMachineDominatorTreePass(PassRegistry&);
LLVM_ABI void initializeMachineFunctionPrinterPassPass(PassRegistry&);
LLVM_ABI void initializeMachineFunctionSplitterPass(PassRegistry &);
LLVM_ABI void initializeMachineLateInstrsCleanupPass(PassRegistry&);
LLVM_ABI void initializeMachineLICMPass(PassRegistry&);
LLVM_ABI void initializeMachineLoopInfoPass(PassRegistry&);
LLVM_ABI void initializeMachineModuleInfoWrapperPassPass(PassRegistry &);
LLVM_ABI void initializeMachineOptimizationRemarkEmitterPassPass(PassRegistry&);
LLVM_ABI void initializeMachineOutlinerPass(PassRegistry&);
LLVM_ABI void initializeMachinePipelinerPass(PassRegistry&);
LLVM_ABI void initializeMachinePostDominatorTreePass(PassRegistry&);
LLVM_ABI void initializeMachineRegionInfoPassPass(PassRegistry&);
LLVM_ABI void initializeMachineSanitizerBinaryMetadataPass(PassRegistry &);
LLVM_ABI void initializeMachineSchedulerPass(PassRegistry&);
LLVM_ABI void initializeMachineSinkingPass(PassRegistry&);
LLVM_ABI void initializeMachineTraceMetricsPass(PassRegistry&);
LLVM_ABI void initializeMachineUniformityInfoPrinterPassPass(PassRegistry &);
LLVM_ABI void initializeMachineUniformityAnalysisPassPass(PassRegistry &);
LLVM_ABI void initializeMachineVerifierPassPass(PassRegistry&);
LLVM_ABI void initializeMemoryDependenceWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeMemorySSAWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeMergeICmpsLegacyPassPass(PassRegistry &);
LLVM_ABI void initializeMergedLoadStoreMotionLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeModuleSummaryIndexWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeModuloScheduleTestPass(PassRegistry&);
LLVM_ABI void initializeNaryReassociateLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeObjCARCContractLegacyPassPass(PassRegistry &);
LLVM_ABI void initializeOptimizationRemarkEmitterWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeOptimizePHIsPass(PassRegistry&);
LLVM_ABI void initializePEIPass(PassRegistry&);
LLVM_ABI void initializePHIEliminationPass(PassRegistry&);
LLVM_ABI void initializePartiallyInlineLibCallsLegacyPassPass(PassRegistry&);
LLVM_ABI void initializePatchableFunctionPass(PassRegistry&);
LLVM_ABI void initializePeepholeOptimizerPass(PassRegistry&);
LLVM_ABI void initializePhiValuesWrapperPassPass(PassRegistry&);
LLVM_ABI void initializePhysicalRegisterUsageInfoPass(PassRegistry&);
LLVM_ABI void initializePlaceBackedgeSafepointsLegacyPassPass(PassRegistry &);
LLVM_ABI void initializePostDomOnlyPrinterWrapperPassPass(PassRegistry &);
LLVM_ABI void initializePostDomOnlyViewerWrapperPassPass(PassRegistry &);
LLVM_ABI void initializePostDomPrinterWrapperPassPass(PassRegistry &);
LLVM_ABI void initializePostDomViewerWrapperPassPass(PassRegistry &);
LLVM_ABI void initializePostDominatorTreeWrapperPassPass(PassRegistry&);
LLVM_ABI void initializePostMachineSchedulerPass(PassRegistry&);
LLVM_ABI void initializePostRAHazardRecognizerPass(PassRegistry&);
LLVM_ABI void initializePostRAMachineSinkingPass(PassRegistry&);
LLVM_ABI void initializePostRASchedulerPass(PassRegistry&);
LLVM_ABI void initializePreISelIntrinsicLoweringLegacyPassPass(PassRegistry&);
LLVM_ABI void initializePredicateInfoPrinterLegacyPassPass(PassRegistry&);
LLVM_ABI void initializePrintFunctionPassWrapperPass(PassRegistry&);
LLVM_ABI void initializePrintModulePassWrapperPass(PassRegistry&);
LLVM_ABI void initializeProcessImplicitDefsPass(PassRegistry&);
LLVM_ABI void initializeProfileSummaryInfoWrapperPassPass(PassRegistry&);
LLVM_ABI void initializePromoteLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeRABasicPass(PassRegistry&);
LLVM_ABI void initializePseudoProbeInserterPass(PassRegistry &);
LLVM_ABI void initializeRAGreedyPass(PassRegistry&);
LLVM_ABI void initializeReachingDefAnalysisPass(PassRegistry&);
LLVM_ABI void initializeReassociateLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeRedundantDbgInstEliminationPass(PassRegistry&);
LLVM_ABI void initializeRegAllocEvictionAdvisorAnalysisPass(PassRegistry &);
LLVM_ABI void initializeRegAllocFastPass(PassRegistry&);
LLVM_ABI void initializeRegAllocPriorityAdvisorAnalysisPass(PassRegistry &);
LLVM_ABI void initializeRegAllocScoringPass(PassRegistry &);
LLVM_ABI void initializeRegBankSelectPass(PassRegistry&);
LLVM_ABI void initializeRegToMemLegacyPass(PassRegistry&);
LLVM_ABI void initializeRegUsageInfoCollectorPass(PassRegistry&);
LLVM_ABI void initializeRegUsageInfoPropagationPass(PassRegistry&);
LLVM_ABI void initializeRegionInfoPassPass(PassRegistry&);
LLVM_ABI void initializeRegionOnlyPrinterPass(PassRegistry&);
LLVM_ABI void initializeRegionOnlyViewerPass(PassRegistry&);
LLVM_ABI void initializeRegionPrinterPass(PassRegistry&);
LLVM_ABI void initializeRegionViewerPass(PassRegistry&);
LLVM_ABI void initializeRegisterCoalescerPass(PassRegistry&);
LLVM_ABI void initializeRemoveRedundantDebugValuesPass(PassRegistry&);
LLVM_ABI void initializeRenameIndependentSubregsPass(PassRegistry&);
LLVM_ABI void initializeReplaceWithVeclibLegacyPass(PassRegistry &);
LLVM_ABI void initializeResetMachineFunctionPass(PassRegistry&);
LLVM_ABI void initializeSCEVAAWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeSROALegacyPassPass(PassRegistry&);
LLVM_ABI void initializeSafeStackLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeSafepointIRVerifierPass(PassRegistry&);
LLVM_ABI void initializeSelectOptimizePass(PassRegistry &);
LLVM_ABI void initializeScalarEvolutionWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeScalarizeMaskedMemIntrinLegacyPassPass(PassRegistry &);
LLVM_ABI void initializeScalarizerLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeScavengerTestPass(PassRegistry&);
LLVM_ABI void initializeScopedNoAliasAAWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeSeparateConstOffsetFromGEPLegacyPassPass(PassRegistry &);
LLVM_ABI void initializeShadowStackGCLoweringPass(PassRegistry&);
LLVM_ABI void initializeShrinkWrapPass(PassRegistry&);
LLVM_ABI void initializeSimpleLoopUnswitchLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeSingleLoopExtractorPass(PassRegistry&);
LLVM_ABI void initializeSinkingLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeSjLjEHPreparePass(PassRegistry&);
LLVM_ABI void initializeSlotIndexesPass(PassRegistry&);
LLVM_ABI void initializeSpeculativeExecutionLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeSpillPlacementPass(PassRegistry&);
LLVM_ABI void initializeStackColoringPass(PassRegistry&);
LLVM_ABI void initializeStackFrameLayoutAnalysisPassPass(PassRegistry &);
LLVM_ABI void initializeStackMapLivenessPass(PassRegistry&);
LLVM_ABI void initializeStackProtectorPass(PassRegistry&);
LLVM_ABI void initializeStackSafetyGlobalInfoWrapperPassPass(PassRegistry &);
LLVM_ABI void initializeStackSafetyInfoWrapperPassPass(PassRegistry &);
LLVM_ABI void initializeStackSlotColoringPass(PassRegistry&);
LLVM_ABI void initializeStraightLineStrengthReduceLegacyPassPass(PassRegistry &);
LLVM_ABI void initializeStripDebugMachineModulePass(PassRegistry &);
LLVM_ABI void initializeStripGCRelocatesLegacyPass(PassRegistry &);
LLVM_ABI void initializeStructurizeCFGLegacyPassPass(PassRegistry &);
LLVM_ABI void initializeTailCallElimPass(PassRegistry&);
LLVM_ABI void initializeTailDuplicatePass(PassRegistry&);
LLVM_ABI void initializeTargetLibraryInfoWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeTargetPassConfigPass(PassRegistry&);
LLVM_ABI void initializeTargetTransformInfoWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeTLSVariableHoistLegacyPassPass(PassRegistry &);
LLVM_ABI void initializeTwoAddressInstructionPassPass(PassRegistry&);
LLVM_ABI void initializeTypeBasedAAWrapperPassPass(PassRegistry&);
LLVM_ABI void initializeTypePromotionLegacyPass(PassRegistry&);
LLVM_ABI void initializeUniformityInfoWrapperPassPass(PassRegistry &);
LLVM_ABI void initializeUnifyFunctionExitNodesLegacyPassPass(PassRegistry &);
LLVM_ABI void initializeUnifyLoopExitsLegacyPassPass(PassRegistry &);
LLVM_ABI void initializeUnpackMachineBundlesPass(PassRegistry&);
LLVM_ABI void initializeUnreachableBlockElimLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeUnreachableMachineBlockElimPass(PassRegistry&);
LLVM_ABI void initializeVerifierLegacyPassPass(PassRegistry&);
LLVM_ABI void initializeVirtRegMapPass(PassRegistry&);
LLVM_ABI void initializeVirtRegRewriterPass(PassRegistry&);
LLVM_ABI void initializeWasmEHPreparePass(PassRegistry&);
LLVM_ABI void initializeWinEHPreparePass(PassRegistry&);
LLVM_ABI void initializeWriteBitcodePassPass(PassRegistry&);
LLVM_ABI void initializeXRayInstrumentationPass(PassRegistry&);

} // end namespace llvm

#endif // LLVM_INITIALIZEPASSES_H
