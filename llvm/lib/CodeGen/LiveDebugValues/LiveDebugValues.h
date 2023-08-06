//===- LiveDebugValues.cpp - Tracking Debug Value MIs ---------*- C++ -*---===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_CODEGEN_LIVEDEBUGVALUES_LIVEDEBUGVALUES_H
#define LLVM_LIB_CODEGEN_LIVEDEBUGVALUES_LIVEDEBUGVALUES_H

#include "llvm/Support/Compiler.h"

namespace llvm {
class MachineDominatorTree;
class MachineFunction;
class TargetPassConfig;
class Triple;

// Inline namespace for types / symbols shared between different
// LiveDebugValues implementations.
inline namespace SharedLiveDebugValues {

// Expose a base class for LiveDebugValues interfaces to inherit from. This
// allows the generic LiveDebugValues pass handles to call into the
// implementation.
class LLVM_CLASS_ABI LDVImpl {
public:
  virtual bool ExtendRanges(MachineFunction &MF, MachineDominatorTree *DomTree,
                            TargetPassConfig *TPC, unsigned InputBBLimit,
                            unsigned InputDbgValLimit) = 0;
  virtual ~LDVImpl() = default;
};

} // namespace SharedLiveDebugValues

// Factory functions for LiveDebugValues implementations.
LLVM_FUNC_ABI extern LDVImpl *makeVarLocBasedLiveDebugValues();
LLVM_FUNC_ABI extern LDVImpl *makeInstrRefBasedLiveDebugValues();

LLVM_FUNC_ABI extern bool debuginfoShouldUseDebugInstrRef(const Triple &T);

} // namespace llvm

#endif // LLVM_LIB_CODEGEN_LIVEDEBUGVALUES_LIVEDEBUGVALUES_H
