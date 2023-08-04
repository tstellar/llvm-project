//===- InstrOrderFile.h ---- Late IR instrumentation for order file ----===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_INSTRUMENTATION_INSTRORDERFILE_H
#define LLVM_TRANSFORMS_INSTRUMENTATION_INSTRORDERFILE_H

#include "llvm/IR/PassManager.h"
#include "llvm/Support/Compiler.h"

namespace llvm {
class Module;

/// The instrumentation pass for recording function order.
class LLVM_CLASS_ABI InstrOrderFilePass : public PassInfoMixin<InstrOrderFilePass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM);
};

} // end namespace llvm

#endif // LLVM_TRANSFORMS_INSTRUMENTATION_INSTRORDERFILE_H
