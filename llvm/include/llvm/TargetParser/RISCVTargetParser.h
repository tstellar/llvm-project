//===-- RISCVTargetParser - Parser for target features ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements a target parser to recognise hardware features
// for RISC-V CPUs.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TARGETPARSER_RISCVTARGETPARSER_H
#define LLVM_TARGETPARSER_RISCVTARGETPARSER_H

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Compiler.h"
#include <vector>

namespace llvm {

class Triple;

namespace RISCV {

// We use 64 bits as the known part in the scalable vector types.
static constexpr unsigned RVVBitsPerBlock = 64;

LLVM_FUNC_ABI bool parseCPU(StringRef CPU, bool IsRV64);
LLVM_FUNC_ABI bool parseTuneCPU(StringRef CPU, bool IsRV64);
LLVM_FUNC_ABI StringRef getMArchFromMcpu(StringRef CPU);
LLVM_FUNC_ABI void fillValidCPUArchList(SmallVectorImpl<StringRef> &Values, bool IsRV64);
LLVM_FUNC_ABI void fillValidTuneCPUArchList(SmallVectorImpl<StringRef> &Values, bool IsRV64);

} // namespace RISCV
} // namespace llvm

#endif
