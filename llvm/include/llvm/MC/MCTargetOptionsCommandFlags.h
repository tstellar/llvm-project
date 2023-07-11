//===-- MCTargetOptionsCommandFlags.h --------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains machine code-specific flags that are shared between
// different command line tools.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_MC_MCTARGETOPTIONSCOMMANDFLAGS_H
#define LLVM_MC_MCTARGETOPTIONSCOMMANDFLAGS_H

#include "llvm/Support/Compiler.h"
#include <optional>
#include <string>

namespace llvm {

class MCTargetOptions;
enum class EmitDwarfUnwindType;

namespace mc {

LLVM_ABI bool getRelaxAll();
LLVM_ABI std::optional<bool> getExplicitRelaxAll();

LLVM_ABI bool getIncrementalLinkerCompatible();

LLVM_ABI int getDwarfVersion();

LLVM_ABI bool getDwarf64();

LLVM_ABI EmitDwarfUnwindType getEmitDwarfUnwind();

LLVM_ABI bool getEmitCompactUnwindNonCanonical();

LLVM_ABI bool getShowMCInst();

LLVM_ABI bool getFatalWarnings();

LLVM_ABI bool getNoWarn();

LLVM_ABI bool getNoDeprecatedWarn();

LLVM_ABI bool getNoTypeCheck();

LLVM_ABI std::string getABIName();

LLVM_ABI std::string getAsSecureLogFile();

/// Create this object with static storage to register mc-related command
/// line options.
struct LLVM_ABI RegisterMCTargetOptionsFlags {
  RegisterMCTargetOptionsFlags();
};

LLVM_ABI MCTargetOptions InitMCTargetOptionsFromFlags();

} // namespace mc

} // namespace llvm

#endif
