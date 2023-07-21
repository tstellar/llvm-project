//===- StripNonLineTableDebugInfo.h - -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_UTILS_STRIPNONLINETABLEDEBUGINFO_H
#define LLVM_TRANSFORMS_UTILS_STRIPNONLINETABLEDEBUGINFO_H

#include "llvm/IR/PassManager.h"
#include "llvm/Support/Compiler.h"

namespace llvm {

class Module;

class LLVM_ABI StripNonLineTableDebugInfoPass
    : public PassInfoMixin<StripNonLineTableDebugInfoPass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM);
};

} // end namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_STRIPNONLINETABLEDEBUGINFO_H
