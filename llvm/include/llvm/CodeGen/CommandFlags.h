//===-- CommandFlags.h - Command Line Flags Interface -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains codegen-specific flags that are shared between different
// command line tools. The tools "llc" and "opt" both use this file to prevent
// flag duplication.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CODEGEN_COMMANDFLAGS_H
#define LLVM_CODEGEN_COMMANDFLAGS_H

#include "llvm/ADT/FloatingPointMode.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Target/TargetOptions.h"
#include <optional>
#include <string>
#include <vector>

namespace llvm {

class Module;
class AttrBuilder;
class Function;
class Triple;

namespace codegen {

LLVM_FUNC_ABI std::string getMArch();

LLVM_FUNC_ABI std::string getMCPU();

LLVM_FUNC_ABI std::vector<std::string> getMAttrs();

LLVM_FUNC_ABI Reloc::Model getRelocModel();
LLVM_FUNC_ABI std::optional<Reloc::Model> getExplicitRelocModel();

LLVM_FUNC_ABI ThreadModel::Model getThreadModel();

LLVM_FUNC_ABI CodeModel::Model getCodeModel();
LLVM_FUNC_ABI std::optional<CodeModel::Model> getExplicitCodeModel();

LLVM_FUNC_ABI llvm::ExceptionHandling getExceptionModel();

LLVM_FUNC_ABI std::optional<CodeGenFileType> getExplicitFileType();

LLVM_FUNC_ABI CodeGenFileType getFileType();

LLVM_FUNC_ABI FramePointerKind getFramePointerUsage();

LLVM_FUNC_ABI bool getEnableUnsafeFPMath();

LLVM_FUNC_ABI bool getEnableNoInfsFPMath();

LLVM_FUNC_ABI bool getEnableNoNaNsFPMath();

LLVM_FUNC_ABI bool getEnableNoSignedZerosFPMath();

LLVM_FUNC_ABI bool getEnableApproxFuncFPMath();

LLVM_FUNC_ABI bool getEnableNoTrappingFPMath();

LLVM_FUNC_ABI DenormalMode::DenormalModeKind getDenormalFPMath();
LLVM_FUNC_ABI DenormalMode::DenormalModeKind getDenormalFP32Math();

LLVM_FUNC_ABI bool getEnableHonorSignDependentRoundingFPMath();

LLVM_FUNC_ABI llvm::FloatABI::ABIType getFloatABIForCalls();

LLVM_FUNC_ABI llvm::FPOpFusion::FPOpFusionMode getFuseFPOps();

LLVM_FUNC_ABI SwiftAsyncFramePointerMode getSwiftAsyncFramePointer();

LLVM_FUNC_ABI bool getDontPlaceZerosInBSS();

LLVM_FUNC_ABI bool getEnableGuaranteedTailCallOpt();

LLVM_FUNC_ABI bool getEnableAIXExtendedAltivecABI();

LLVM_FUNC_ABI bool getDisableTailCalls();

LLVM_FUNC_ABI bool getStackSymbolOrdering();

LLVM_FUNC_ABI unsigned getOverrideStackAlignment();

LLVM_FUNC_ABI bool getStackRealign();

LLVM_FUNC_ABI std::string getTrapFuncName();

LLVM_FUNC_ABI bool getUseCtors();

LLVM_FUNC_ABI bool getRelaxELFRelocations();

LLVM_FUNC_ABI bool getDataSections();
LLVM_FUNC_ABI std::optional<bool> getExplicitDataSections();

LLVM_FUNC_ABI bool getFunctionSections();
LLVM_FUNC_ABI std::optional<bool> getExplicitFunctionSections();

LLVM_FUNC_ABI bool getIgnoreXCOFFVisibility();

LLVM_FUNC_ABI bool getXCOFFTracebackTable();

LLVM_FUNC_ABI std::string getBBSections();

LLVM_FUNC_ABI unsigned getTLSSize();

LLVM_FUNC_ABI bool getEmulatedTLS();
LLVM_FUNC_ABI std::optional<bool> getExplicitEmulatedTLS();

LLVM_FUNC_ABI bool getUniqueSectionNames();

LLVM_FUNC_ABI bool getUniqueBasicBlockSectionNames();

LLVM_FUNC_ABI llvm::EABI getEABIVersion();

LLVM_FUNC_ABI llvm::DebuggerKind getDebuggerTuningOpt();

LLVM_FUNC_ABI bool getEnableStackSizeSection();

LLVM_FUNC_ABI bool getEnableAddrsig();

LLVM_FUNC_ABI bool getEmitCallSiteInfo();

LLVM_FUNC_ABI bool getEnableMachineFunctionSplitter();

LLVM_FUNC_ABI bool getEnableDebugEntryValues();

LLVM_FUNC_ABI bool getValueTrackingVariableLocations();
LLVM_FUNC_ABI std::optional<bool> getExplicitValueTrackingVariableLocations();

LLVM_FUNC_ABI bool getForceDwarfFrameSection();

LLVM_FUNC_ABI bool getXRayFunctionIndex();

LLVM_FUNC_ABI bool getDebugStrictDwarf();

LLVM_FUNC_ABI unsigned getAlignLoops();

LLVM_FUNC_ABI bool getJMCInstrument();

LLVM_FUNC_ABI bool getXCOFFReadOnlyPointers();

/// Create this object with static storage to register codegen-related command
/// line options.
struct LLVM_CLASS_ABI RegisterCodeGenFlags {
  RegisterCodeGenFlags();
};

LLVM_FUNC_ABI llvm::BasicBlockSection getBBSectionsMode(llvm::TargetOptions &Options);

/// Common utility function tightly tied to the options listed here. Initializes
/// a TargetOptions object with CodeGen flags and returns it.
/// \p TheTriple is used to determine the default value for options if
///    options are not explicitly specified. If those triple dependant options
///    value do not have effect for your component, a default Triple() could be
///    passed in.
LLVM_FUNC_ABI TargetOptions InitTargetOptionsFromCodeGenFlags(const llvm::Triple &TheTriple);

LLVM_FUNC_ABI std::string getCPUStr();

LLVM_FUNC_ABI std::string getFeaturesStr();

LLVM_FUNC_ABI std::vector<std::string> getFeatureList();

LLVM_FUNC_ABI void renderBoolStringAttr(AttrBuilder &B, StringRef Name, bool Val);

/// Set function attributes of function \p F based on CPU, Features, and command
/// line flags.
LLVM_FUNC_ABI void setFunctionAttributes(StringRef CPU, StringRef Features, Function &F);

/// Set function attributes of functions in Module M based on CPU,
/// Features, and command line flags.
LLVM_FUNC_ABI void setFunctionAttributes(StringRef CPU, StringRef Features, Module &M);

/// Should value-tracking variable locations / instruction referencing be
/// enabled by default for this triple?
LLVM_FUNC_ABI bool getDefaultValueTrackingVariableLocations(const llvm::Triple &T);
} // namespace codegen
} // namespace llvm

#endif // LLVM_CODEGEN_COMMANDFLAGS_H
