//===- DivRemPairs.h - Hoist/decompose integer division and remainder -----===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This pass hoists and/or decomposes integer division and remainder
// instructions to enable CFG improvements and better codegen.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_SCALAR_DIVREMPAIRS_H
#define LLVM_TRANSFORMS_SCALAR_DIVREMPAIRS_H

#include "llvm/IR/PassManager.h"
#include "llvm/Support/Compiler.h"

namespace llvm {

/// Hoist/decompose integer division and remainder instructions to enable CFG
/// improvements and better codegen.
struct LLVM_CLASS_ABI DivRemPairsPass : public PassInfoMixin<DivRemPairsPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &);
};

}
#endif // LLVM_TRANSFORMS_SCALAR_DIVREMPAIRS_H

