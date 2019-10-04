//===- Writer.cpp ---------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Writer.h"
#include "Config.h"
#include "InputChunks.h"
#include "InputEvent.h"
#include "InputGlobal.h"
#include "OutputSections.h"
#include "OutputSegment.h"
#include "Relocations.h"
#include "SymbolTable.h"
#include "SyntheticSections.h"
#include "WriterUtils.h"
#include "lld/Common/ErrorHandler.h"
#include "lld/Common/Memory.h"
#include "lld/Common/Strings.h"
#include "lld/Common/Threads.h"
#include "llvm/ADT/DenseSet.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/BinaryFormat/Wasm.h"
#include "llvm/Object/WasmTraits.h"
#include "llvm/Support/FileOutputBuffer.h"
#include "llvm/Support/Format.h"
#include "llvm/Support/FormatVariadic.h"
#include "llvm/Support/LEB128.h"

#include <cstdarg>
#include <map>

#define DEBUG_TYPE "lld"

using namespace llvm;
using namespace llvm::wasm;
using namespace lld;
using namespace lld::wasm;

<<<<<<< HEAD
static constexpr int stackAlignment = 16;
=======
static constexpr int StackAlignment = 16;
static constexpr const char *FunctionTableName = "__indirect_function_table";
const char *lld::wasm::DefaultModule = "env";
>>>>>>> release/8.x

namespace {

// The writer writes a SymbolTable result to a file.
class Writer {
public:
  void run();

private:
  void openFile();

  void createInitMemoryFunction();
  void createApplyRelocationsFunction();
  void createCallCtorsFunction();
  void createInitTLSFunction();

  void assignIndexes();
  void populateSymtab();
  void populateProducers();
  void populateTargetFeatures();
  void calculateInitFunctions();
  void calculateImports();
  void calculateExports();
  void calculateCustomSections();
  void calculateTypes();
  void createOutputSegments();
  void layoutMemory();
  void createHeader();

  void addSection(OutputSection *sec);

  void addSections();

  void createCustomSections();
  void createSyntheticSections();
  void finalizeSections();

  // Custom sections
  void createRelocSections();

  void writeHeader();
  void writeSections();

  uint64_t fileSize = 0;

  std::vector<WasmInitEntry> initFunctions;
  llvm::StringMap<std::vector<InputSection *>> customSectionMapping;

  // Elements that are used to construct the final output
  std::string header;
  std::vector<OutputSection *> outputSections;

  std::unique_ptr<FileOutputBuffer> buffer;

  std::vector<OutputSegment *> segments;
  llvm::SmallDenseMap<StringRef, OutputSegment *> segmentMap;
};

} // anonymous namespace

<<<<<<< HEAD
=======
void Writer::createImportSection() {
  uint32_t NumImports = ImportedSymbols.size();
  if (Config->ImportMemory)
    ++NumImports;
  if (Config->ImportTable)
    ++NumImports;

  if (NumImports == 0)
    return;

  SyntheticSection *Section = createSyntheticSection(WASM_SEC_IMPORT);
  raw_ostream &OS = Section->getStream();

  writeUleb128(OS, NumImports, "import count");

  if (Config->ImportMemory) {
    WasmImport Import;
    Import.Module = DefaultModule;
    Import.Field = "memory";
    Import.Kind = WASM_EXTERNAL_MEMORY;
    Import.Memory.Flags = 0;
    Import.Memory.Initial = NumMemoryPages;
    if (MaxMemoryPages != 0) {
      Import.Memory.Flags |= WASM_LIMITS_FLAG_HAS_MAX;
      Import.Memory.Maximum = MaxMemoryPages;
    }
    if (Config->SharedMemory)
      Import.Memory.Flags |= WASM_LIMITS_FLAG_IS_SHARED;
    writeImport(OS, Import);
  }

  if (Config->ImportTable) {
    uint32_t TableSize = TableBase + IndirectFunctions.size();
    WasmImport Import;
    Import.Module = DefaultModule;
    Import.Field = FunctionTableName;
    Import.Kind = WASM_EXTERNAL_TABLE;
    Import.Table.ElemType = WASM_TYPE_FUNCREF;
    Import.Table.Limits = {0, TableSize, 0};
    writeImport(OS, Import);
  }

  for (const Symbol *Sym : ImportedSymbols) {
    WasmImport Import;
    if (auto *F = dyn_cast<UndefinedFunction>(Sym)) {
      Import.Field = F->ImportName;
      Import.Module = F->ImportModule;
    } else if (auto *G = dyn_cast<UndefinedGlobal>(Sym)) {
      Import.Field = G->ImportName;
      Import.Module = G->ImportModule;
    } else {
      Import.Field = Sym->getName();
      Import.Module = DefaultModule;
    }

    if (auto *FunctionSym = dyn_cast<FunctionSymbol>(Sym)) {
      Import.Kind = WASM_EXTERNAL_FUNCTION;
      Import.SigIndex = lookupType(*FunctionSym->Signature);
    } else if (auto *GlobalSym = dyn_cast<GlobalSymbol>(Sym)) {
      Import.Kind = WASM_EXTERNAL_GLOBAL;
      Import.Global = *GlobalSym->getGlobalType();
    } else {
      auto *EventSym = cast<EventSymbol>(Sym);
      Import.Kind = WASM_EXTERNAL_EVENT;
      Import.Event.Attribute = EventSym->getEventType()->Attribute;
      Import.Event.SigIndex = lookupType(*EventSym->Signature);
    }
    writeImport(OS, Import);
  }
}

void Writer::createTypeSection() {
  SyntheticSection *Section = createSyntheticSection(WASM_SEC_TYPE);
  raw_ostream &OS = Section->getStream();
  writeUleb128(OS, Types.size(), "type count");
  for (const WasmSignature *Sig : Types)
    writeSig(OS, *Sig);
}

void Writer::createFunctionSection() {
  if (InputFunctions.empty())
    return;

  SyntheticSection *Section = createSyntheticSection(WASM_SEC_FUNCTION);
  raw_ostream &OS = Section->getStream();

  writeUleb128(OS, InputFunctions.size(), "function count");
  for (const InputFunction *Func : InputFunctions)
    writeUleb128(OS, lookupType(Func->Signature), "sig index");
}

void Writer::createMemorySection() {
  if (Config->ImportMemory)
    return;

  SyntheticSection *Section = createSyntheticSection(WASM_SEC_MEMORY);
  raw_ostream &OS = Section->getStream();

  bool HasMax = MaxMemoryPages != 0;
  writeUleb128(OS, 1, "memory count");
  unsigned Flags = 0;
  if (HasMax)
    Flags |= WASM_LIMITS_FLAG_HAS_MAX;
  if (Config->SharedMemory)
    Flags |= WASM_LIMITS_FLAG_IS_SHARED;
  writeUleb128(OS, Flags, "memory limits flags");
  writeUleb128(OS, NumMemoryPages, "initial pages");
  if (HasMax)
    writeUleb128(OS, MaxMemoryPages, "max pages");
}

void Writer::createGlobalSection() {
  unsigned NumGlobals = InputGlobals.size() + DefinedFakeGlobals.size();
  if (NumGlobals == 0)
    return;

  SyntheticSection *Section = createSyntheticSection(WASM_SEC_GLOBAL);
  raw_ostream &OS = Section->getStream();

  writeUleb128(OS, NumGlobals, "global count");
  for (const InputGlobal *G : InputGlobals)
    writeGlobal(OS, G->Global);
  for (const DefinedData *Sym : DefinedFakeGlobals) {
    WasmGlobal Global;
    Global.Type = {WASM_TYPE_I32, false};
    Global.InitExpr.Opcode = WASM_OPCODE_I32_CONST;
    Global.InitExpr.Value.Int32 = Sym->getVirtualAddress();
    writeGlobal(OS, Global);
  }
}

// The event section contains a list of declared wasm events associated with the
// module. Currently the only supported event kind is exceptions. A single event
// entry represents a single event with an event tag. All C++ exceptions are
// represented by a single event. An event entry in this section contains
// information on what kind of event it is (e.g. exception) and the type of
// values contained in a single event object. (In wasm, an event can contain
// multiple values of primitive types. But for C++ exceptions, we just throw a
// pointer which is an i32 value (for wasm32 architecture), so the signature of
// C++ exception is (i32)->(void), because all event types are assumed to have
// void return type to share WasmSignature with functions.)
void Writer::createEventSection() {
  unsigned NumEvents = InputEvents.size();
  if (NumEvents == 0)
    return;

  SyntheticSection *Section = createSyntheticSection(WASM_SEC_EVENT);
  raw_ostream &OS = Section->getStream();

  writeUleb128(OS, NumEvents, "event count");
  for (InputEvent *E : InputEvents) {
    E->Event.Type.SigIndex = lookupType(E->Signature);
    writeEvent(OS, E->Event);
  }
}

void Writer::createTableSection() {
  if (Config->ImportTable)
    return;

  // Always output a table section (or table import), even if there are no
  // indirect calls.  There are two reasons for this:
  //  1. For executables it is useful to have an empty table slot at 0
  //     which can be filled with a null function call handler.
  //  2. If we don't do this, any program that contains a call_indirect but
  //     no address-taken function will fail at validation time since it is
  //     a validation error to include a call_indirect instruction if there
  //     is not table.
  uint32_t TableSize = TableBase + IndirectFunctions.size();

  SyntheticSection *Section = createSyntheticSection(WASM_SEC_TABLE);
  raw_ostream &OS = Section->getStream();

  writeUleb128(OS, 1, "table count");
  WasmLimits Limits = {WASM_LIMITS_FLAG_HAS_MAX, TableSize, TableSize};
  writeTableType(OS, WasmTable{WASM_TYPE_FUNCREF, Limits});
}

void Writer::createExportSection() {
  if (!Exports.size())
    return;

  SyntheticSection *Section = createSyntheticSection(WASM_SEC_EXPORT);
  raw_ostream &OS = Section->getStream();

  writeUleb128(OS, Exports.size(), "export count");
  for (const WasmExport &Export : Exports)
    writeExport(OS, Export);
}

>>>>>>> release/8.x
void Writer::calculateCustomSections() {
  log("calculateCustomSections");
  bool stripDebug = config->stripDebug || config->stripAll;
  for (ObjFile *file : symtab->objectFiles) {
    for (InputSection *section : file->customSections) {
      StringRef name = section->getName();
      // These custom sections are known the linker and synthesized rather than
      // blindly copied
      if (name == "linking" || name == "name" || name == "producers" ||
          name == "target_features" || name.startswith("reloc."))
        continue;
      // .. or it is a debug section
      if (stripDebug && name.startswith(".debug_"))
        continue;
      customSectionMapping[name].push_back(section);
    }
  }
}

void Writer::createCustomSections() {
  log("createCustomSections");
  for (auto &pair : customSectionMapping) {
    StringRef name = pair.first();
    LLVM_DEBUG(dbgs() << "createCustomSection: " << name << "\n");

    OutputSection *sec = make<CustomSection>(name, pair.second);
    if (config->relocatable || config->emitRelocs) {
      auto *sym = make<OutputSectionSymbol>(sec);
      out.linkingSec->addToSymtab(sym);
      sec->sectionSym = sym;
    }
    addSection(sec);
  }
}

// Create relocations sections in the final output.
// These are only created when relocatable output is requested.
void Writer::createRelocSections() {
  log("createRelocSections");
  // Don't use iterator here since we are adding to OutputSection
  size_t origSize = outputSections.size();
  for (size_t i = 0; i < origSize; i++) {
    LLVM_DEBUG(dbgs() << "check section " << i << "\n");
    OutputSection *sec = outputSections[i];

    // Count the number of needed sections.
    uint32_t count = sec->getNumRelocations();
    if (!count)
      continue;

    StringRef name;
    if (sec->type == WASM_SEC_DATA)
      name = "reloc.DATA";
    else if (sec->type == WASM_SEC_CODE)
      name = "reloc.CODE";
    else if (sec->type == WASM_SEC_CUSTOM)
      name = saver.save("reloc." + sec->name);
    else
      llvm_unreachable(
          "relocations only supported for code, data, or custom sections");

    addSection(make<RelocSection>(name, sec));
  }
}

<<<<<<< HEAD
void Writer::populateProducers() {
  for (ObjFile *file : symtab->objectFiles) {
    const WasmProducerInfo &info = file->getWasmObj()->getProducerInfo();
    out.producersSec->addInfo(info);
  }
=======
static uint32_t getWasmFlags(const Symbol *Sym) {
  uint32_t Flags = 0;
  if (Sym->isLocal())
    Flags |= WASM_SYMBOL_BINDING_LOCAL;
  if (Sym->isWeak())
    Flags |= WASM_SYMBOL_BINDING_WEAK;
  if (Sym->isHidden())
    Flags |= WASM_SYMBOL_VISIBILITY_HIDDEN;
  if (Sym->isUndefined())
    Flags |= WASM_SYMBOL_UNDEFINED;
  if (auto *F = dyn_cast<UndefinedFunction>(Sym)) {
    if (F->getName() != F->ImportName)
      Flags |= WASM_SYMBOL_EXPLICIT_NAME;
  } else if (auto *G = dyn_cast<UndefinedGlobal>(Sym)) {
    if (G->getName() != G->ImportName)
      Flags |= WASM_SYMBOL_EXPLICIT_NAME;
  }
  return Flags;
}

// Some synthetic sections (e.g. "name" and "linking") have subsections.
// Just like the synthetic sections themselves these need to be created before
// they can be written out (since they are preceded by their length). This
// class is used to create subsections and then write them into the stream
// of the parent section.
class SubSection {
public:
  explicit SubSection(uint32_t Type) : Type(Type) {}

  void writeTo(raw_ostream &To) {
    OS.flush();
    writeUleb128(To, Type, "subsection type");
    writeUleb128(To, Body.size(), "subsection size");
    To.write(Body.data(), Body.size());
  }

private:
  uint32_t Type;
  std::string Body;

public:
  raw_string_ostream OS{Body};
};

// Create the custom "dylink" section containing information for the dynamic
// linker.
// See
// https://github.com/WebAssembly/tool-conventions/blob/master/DynamicLinking.md
void Writer::createDylinkSection() {
  SyntheticSection *Section = createSyntheticSection(WASM_SEC_CUSTOM, "dylink");
  raw_ostream &OS = Section->getStream();

  writeUleb128(OS, MemSize, "MemSize");
  writeUleb128(OS, MemAlign, "MemAlign");
  writeUleb128(OS, IndirectFunctions.size(), "TableSize");
  writeUleb128(OS, 0, "TableAlign");
  writeUleb128(OS, 0, "Needed");  // TODO: Support "needed" shared libraries
}

// Create the custom "linking" section containing linker metadata.
// This is only created when relocatable output is requested.
void Writer::createLinkingSection() {
  SyntheticSection *Section =
      createSyntheticSection(WASM_SEC_CUSTOM, "linking");
  raw_ostream &OS = Section->getStream();

  writeUleb128(OS, WasmMetadataVersion, "Version");

  if (!SymtabEntries.empty()) {
    SubSection Sub(WASM_SYMBOL_TABLE);
    writeUleb128(Sub.OS, SymtabEntries.size(), "num symbols");

    for (const Symbol *Sym : SymtabEntries) {
      assert(Sym->isDefined() || Sym->isUndefined());
      WasmSymbolType Kind = Sym->getWasmType();
      uint32_t Flags = getWasmFlags(Sym);

      writeU8(Sub.OS, Kind, "sym kind");
      writeUleb128(Sub.OS, Flags, "sym flags");

      if (auto *F = dyn_cast<FunctionSymbol>(Sym)) {
        writeUleb128(Sub.OS, F->getFunctionIndex(), "index");
        if (Sym->isDefined() ||
            (Flags & WASM_SYMBOL_EXPLICIT_NAME) != 0)
          writeStr(Sub.OS, Sym->getName(), "sym name");
      } else if (auto *G = dyn_cast<GlobalSymbol>(Sym)) {
        writeUleb128(Sub.OS, G->getGlobalIndex(), "index");
        if (Sym->isDefined() ||
            (Flags & WASM_SYMBOL_EXPLICIT_NAME) != 0)
          writeStr(Sub.OS, Sym->getName(), "sym name");
      } else if (auto *E = dyn_cast<EventSymbol>(Sym)) {
        writeUleb128(Sub.OS, E->getEventIndex(), "index");
        if (Sym->isDefined() ||
            (Flags & WASM_SYMBOL_EXPLICIT_NAME) != 0)
          writeStr(Sub.OS, Sym->getName(), "sym name");
      } else if (isa<DataSymbol>(Sym)) {
        writeStr(Sub.OS, Sym->getName(), "sym name");
        if (auto *DataSym = dyn_cast<DefinedData>(Sym)) {
          writeUleb128(Sub.OS, DataSym->getOutputSegmentIndex(), "index");
          writeUleb128(Sub.OS, DataSym->getOutputSegmentOffset(),
                       "data offset");
          writeUleb128(Sub.OS, DataSym->getSize(), "data size");
        }
      } else {
        auto *S = cast<SectionSymbol>(Sym);
        writeUleb128(Sub.OS, S->getOutputSectionIndex(), "sym section index");
      }
    }

    Sub.writeTo(OS);
  }

  if (Segments.size()) {
    SubSection Sub(WASM_SEGMENT_INFO);
    writeUleb128(Sub.OS, Segments.size(), "num data segments");
    for (const OutputSegment *S : Segments) {
      writeStr(Sub.OS, S->Name, "segment name");
      writeUleb128(Sub.OS, S->Alignment, "alignment");
      writeUleb128(Sub.OS, 0, "flags");
    }
    Sub.writeTo(OS);
  }

  if (!InitFunctions.empty()) {
    SubSection Sub(WASM_INIT_FUNCS);
    writeUleb128(Sub.OS, InitFunctions.size(), "num init functions");
    for (const WasmInitEntry &F : InitFunctions) {
      writeUleb128(Sub.OS, F.Priority, "priority");
      writeUleb128(Sub.OS, F.Sym->getOutputSymbolIndex(), "function index");
    }
    Sub.writeTo(OS);
  }

  struct ComdatEntry {
    unsigned Kind;
    uint32_t Index;
  };
  std::map<StringRef, std::vector<ComdatEntry>> Comdats;

  for (const InputFunction *F : InputFunctions) {
    StringRef Comdat = F->getComdatName();
    if (!Comdat.empty())
      Comdats[Comdat].emplace_back(
          ComdatEntry{WASM_COMDAT_FUNCTION, F->getFunctionIndex()});
  }
  for (uint32_t I = 0; I < Segments.size(); ++I) {
    const auto &InputSegments = Segments[I]->InputSegments;
    if (InputSegments.empty())
      continue;
    StringRef Comdat = InputSegments[0]->getComdatName();
#ifndef NDEBUG
    for (const InputSegment *IS : InputSegments)
      assert(IS->getComdatName() == Comdat);
#endif
    if (!Comdat.empty())
      Comdats[Comdat].emplace_back(ComdatEntry{WASM_COMDAT_DATA, I});
  }

  if (!Comdats.empty()) {
    SubSection Sub(WASM_COMDAT_INFO);
    writeUleb128(Sub.OS, Comdats.size(), "num comdats");
    for (const auto &C : Comdats) {
      writeStr(Sub.OS, C.first, "comdat name");
      writeUleb128(Sub.OS, 0, "comdat flags"); // flags for future use
      writeUleb128(Sub.OS, C.second.size(), "num entries");
      for (const ComdatEntry &Entry : C.second) {
        writeU8(Sub.OS, Entry.Kind, "entry kind");
        writeUleb128(Sub.OS, Entry.Index, "entry index");
      }
    }
    Sub.writeTo(OS);
  }
}

// Create the custom "name" section containing debug symbol names.
void Writer::createNameSection() {
  unsigned NumNames = NumImportedFunctions;
  for (const InputFunction *F : InputFunctions)
    if (!F->getName().empty() || !F->getDebugName().empty())
      ++NumNames;

  if (NumNames == 0)
    return;

  SyntheticSection *Section = createSyntheticSection(WASM_SEC_CUSTOM, "name");

  SubSection Sub(WASM_NAMES_FUNCTION);
  writeUleb128(Sub.OS, NumNames, "name count");

  // Names must appear in function index order.  As it happens ImportedSymbols
  // and InputFunctions are numbered in order with imported functions coming
  // first.
  for (const Symbol *S : ImportedSymbols) {
    if (auto *F = dyn_cast<FunctionSymbol>(S)) {
      writeUleb128(Sub.OS, F->getFunctionIndex(), "func index");
      writeStr(Sub.OS, toString(*S), "symbol name");
    }
  }
  for (const InputFunction *F : InputFunctions) {
    if (!F->getName().empty()) {
      writeUleb128(Sub.OS, F->getFunctionIndex(), "func index");
      if (!F->getDebugName().empty()) {
        writeStr(Sub.OS, F->getDebugName(), "symbol name");
      } else {
        writeStr(Sub.OS, maybeDemangleSymbol(F->getName()), "symbol name");
      }
    }
  }

  Sub.writeTo(Section->getStream());
>>>>>>> release/8.x
}

void Writer::writeHeader() {
  memcpy(buffer->getBufferStart(), header.data(), header.size());
}

void Writer::writeSections() {
  uint8_t *buf = buffer->getBufferStart();
  parallelForEach(outputSections, [buf](OutputSection *s) {
    assert(s->isNeeded());
    s->writeTo(buf);
  });
}

// Fix the memory layout of the output binary.  This assigns memory offsets
// to each of the input data sections as well as the explicit stack region.
// The default memory layout is as follows, from low to high.
//
//  - initialized data (starting at Config->globalBase)
//  - BSS data (not currently implemented in llvm)
//  - explicit stack (Config->ZStackSize)
//  - heap start / unallocated
//
// The --stack-first option means that stack is placed before any static data.
// This can be useful since it means that stack overflow traps immediately
// rather than overwriting global data, but also increases code size since all
// static data loads and stores requires larger offsets.
void Writer::layoutMemory() {
  uint32_t memoryPtr = 0;

  auto placeStack = [&]() {
    if (config->relocatable || config->isPic)
      return;
<<<<<<< HEAD
    memoryPtr = alignTo(memoryPtr, stackAlignment);
    if (config->zStackSize != alignTo(config->zStackSize, stackAlignment))
      error("stack size must be " + Twine(stackAlignment) + "-byte aligned");
    log("mem: stack size  = " + Twine(config->zStackSize));
    log("mem: stack base  = " + Twine(memoryPtr));
    memoryPtr += config->zStackSize;
    auto *sp = cast<DefinedGlobal>(WasmSym::stackPointer);
    sp->global->global.InitExpr.Value.Int32 = memoryPtr;
    log("mem: stack top   = " + Twine(memoryPtr));
=======
    MemoryPtr = alignTo(MemoryPtr, StackAlignment);
    if (Config->ZStackSize != alignTo(Config->ZStackSize, StackAlignment))
      error("stack size must be " + Twine(StackAlignment) + "-byte aligned");
    log("mem: stack size  = " + Twine(Config->ZStackSize));
    log("mem: stack base  = " + Twine(MemoryPtr));
    MemoryPtr += Config->ZStackSize;
    auto *SP = cast<DefinedGlobal>(WasmSym::StackPointer);
    SP->Global->Global.InitExpr.Value.Int32 = MemoryPtr;
    log("mem: stack top   = " + Twine(MemoryPtr));
>>>>>>> release/8.x
  };

  if (config->stackFirst) {
    placeStack();
  } else {
    memoryPtr = config->globalBase;
    log("mem: global base = " + Twine(config->globalBase));
  }

  if (WasmSym::globalBase)
    WasmSym::globalBase->setVirtualAddress(memoryPtr);
  if (WasmSym::definedMemoryBase)
    WasmSym::definedMemoryBase->setVirtualAddress(memoryPtr);

  uint32_t dataStart = memoryPtr;

  // Arbitrarily set __dso_handle handle to point to the start of the data
  // segments.
  if (WasmSym::dsoHandle)
    WasmSym::dsoHandle->setVirtualAddress(dataStart);

  out.dylinkSec->memAlign = 0;
  for (OutputSegment *seg : segments) {
    out.dylinkSec->memAlign = std::max(out.dylinkSec->memAlign, seg->alignment);
    memoryPtr = alignTo(memoryPtr, 1ULL << seg->alignment);
    seg->startVA = memoryPtr;
    log(formatv("mem: {0,-15} offset={1,-8} size={2,-8} align={3}", seg->name,
                memoryPtr, seg->size, seg->alignment));
    memoryPtr += seg->size;

    if (WasmSym::tlsSize && seg->name == ".tdata") {
      auto *tlsSize = cast<DefinedGlobal>(WasmSym::tlsSize);
      tlsSize->global->global.InitExpr.Value.Int32 = seg->size;

      auto *tlsAlign = cast<DefinedGlobal>(WasmSym::tlsAlign);
      tlsAlign->global->global.InitExpr.Value.Int32 = 1U << seg->alignment;
    }
  }

  // Make space for the memory initialization flag
  if (WasmSym::initMemoryFlag) {
    memoryPtr = alignTo(memoryPtr, 4);
    WasmSym::initMemoryFlag->setVirtualAddress(memoryPtr);
    log(formatv("mem: {0,-15} offset={1,-8} size={2,-8} align={3}",
                "__wasm_init_memory_flag", memoryPtr, 4, 4));
    memoryPtr += 4;
  }

  if (WasmSym::dataEnd)
    WasmSym::dataEnd->setVirtualAddress(memoryPtr);

  log("mem: static data = " + Twine(memoryPtr - dataStart));

  if (config->shared) {
    out.dylinkSec->memSize = memoryPtr;
    return;
  }

  if (!config->stackFirst)
    placeStack();

  // Set `__heap_base` to directly follow the end of the stack or global data.
  // The fact that this comes last means that a malloc/brk implementation
  // can grow the heap at runtime.
  log("mem: heap base   = " + Twine(memoryPtr));
  if (WasmSym::heapBase)
    WasmSym::heapBase->setVirtualAddress(memoryPtr);

  if (config->initialMemory != 0) {
    if (config->initialMemory != alignTo(config->initialMemory, WasmPageSize))
      error("initial memory must be " + Twine(WasmPageSize) + "-byte aligned");
    if (memoryPtr > config->initialMemory)
      error("initial memory too small, " + Twine(memoryPtr) + " bytes needed");
    else
      memoryPtr = config->initialMemory;
  }
  out.dylinkSec->memSize = memoryPtr;
  out.memorySec->numMemoryPages =
      alignTo(memoryPtr, WasmPageSize) / WasmPageSize;
  log("mem: total pages = " + Twine(out.memorySec->numMemoryPages));

  // Check max if explicitly supplied or required by shared memory
  if (config->maxMemory != 0 || config->sharedMemory) {
    if (config->maxMemory != alignTo(config->maxMemory, WasmPageSize))
      error("maximum memory must be " + Twine(WasmPageSize) + "-byte aligned");
    if (memoryPtr > config->maxMemory)
      error("maximum memory too small, " + Twine(memoryPtr) + " bytes needed");
    out.memorySec->maxMemoryPages = config->maxMemory / WasmPageSize;
    log("mem: max pages   = " + Twine(out.memorySec->maxMemoryPages));
  }
}

void Writer::addSection(OutputSection *sec) {
  if (!sec->isNeeded())
    return;
  log("addSection: " + toString(*sec));
  sec->sectionIndex = outputSections.size();
  outputSections.push_back(sec);
}

// If a section name is valid as a C identifier (which is rare because of
// the leading '.'), linkers are expected to define __start_<secname> and
// __stop_<secname> symbols. They are at beginning and end of the section,
// respectively. This is not requested by the ELF standard, but GNU ld and
// gold provide the feature, and used by many programs.
static void addStartStopSymbols(const OutputSegment *seg) {
  StringRef name = seg->name;
  if (!isValidCIdentifier(name))
    return;
  LLVM_DEBUG(dbgs() << "addStartStopSymbols: " << name << "\n");
  uint32_t start = seg->startVA;
  uint32_t stop = start + seg->size;
  symtab->addOptionalDataSymbol(saver.save("__start_" + name), start);
  symtab->addOptionalDataSymbol(saver.save("__stop_" + name), stop);
}

void Writer::addSections() {
  addSection(out.dylinkSec);
  addSection(out.typeSec);
  addSection(out.importSec);
  addSection(out.functionSec);
  addSection(out.tableSec);
  addSection(out.memorySec);
  addSection(out.globalSec);
  addSection(out.eventSec);
  addSection(out.exportSec);
  addSection(out.startSec);
  addSection(out.elemSec);
  addSection(out.dataCountSec);

  addSection(make<CodeSection>(out.functionSec->inputFunctions));
  addSection(make<DataSection>(segments));

  createCustomSections();

  addSection(out.linkingSec);
  if (config->emitRelocs || config->relocatable) {
    createRelocSections();
  }

  addSection(out.nameSec);
  addSection(out.producersSec);
  addSection(out.targetFeaturesSec);
}

void Writer::finalizeSections() {
  for (OutputSection *s : outputSections) {
    s->setOffset(fileSize);
    s->finalizeContents();
    fileSize += s->getSize();
  }
}

void Writer::populateTargetFeatures() {
  StringMap<std::string> used;
  StringMap<std::string> required;
  StringMap<std::string> disallowed;
  SmallSet<std::string, 8> &allowed = out.targetFeaturesSec->features;
  bool tlsUsed = false;

  // Only infer used features if user did not specify features
  bool inferFeatures = !config->features.hasValue();

  if (!inferFeatures) {
    auto &explicitFeatures = config->features.getValue();
    allowed.insert(explicitFeatures.begin(), explicitFeatures.end());
    if (!config->checkFeatures)
      return;
  }

  // Find the sets of used, required, and disallowed features
  for (ObjFile *file : symtab->objectFiles) {
    StringRef fileName(file->getName());
    for (auto &feature : file->getWasmObj()->getTargetFeatures()) {
      switch (feature.Prefix) {
      case WASM_FEATURE_PREFIX_USED:
        used.insert({feature.Name, fileName});
        break;
      case WASM_FEATURE_PREFIX_REQUIRED:
        used.insert({feature.Name, fileName});
        required.insert({feature.Name, fileName});
        break;
      case WASM_FEATURE_PREFIX_DISALLOWED:
        disallowed.insert({feature.Name, fileName});
        break;
      default:
        error("Unrecognized feature policy prefix " +
              std::to_string(feature.Prefix));
      }
    }

    // Find TLS data segments
    auto isTLS = [](InputSegment *segment) {
      StringRef name = segment->getName();
      return segment->live &&
             (name.startswith(".tdata") || name.startswith(".tbss"));
    };
    tlsUsed = tlsUsed ||
              std::any_of(file->segments.begin(), file->segments.end(), isTLS);
  }

  if (inferFeatures)
    allowed.insert(used.keys().begin(), used.keys().end());

  if (allowed.count("atomics") && !config->sharedMemory) {
    if (inferFeatures)
      error(Twine("'atomics' feature is used by ") + used["atomics"] +
            ", so --shared-memory must be used");
    else
      error("'atomics' feature is used, so --shared-memory must be used");
  }

  if (!config->checkFeatures)
    return;

  if (disallowed.count("atomics") && config->sharedMemory)
    error("'atomics' feature is disallowed by " + disallowed["atomics"] +
          ", so --shared-memory must not be used");

  if (!allowed.count("atomics") && config->sharedMemory)
    error("'atomics' feature must be used in order to use shared "
          "memory");

  if (!allowed.count("bulk-memory") && config->sharedMemory)
    error("'bulk-memory' feature must be used in order to use shared "
          "memory");

  if (!allowed.count("bulk-memory") && tlsUsed)
    error("'bulk-memory' feature must be used in order to use thread-local "
          "storage");

  // Validate that used features are allowed in output
  if (!inferFeatures) {
    for (auto &feature : used.keys()) {
      if (!allowed.count(feature))
        error(Twine("Target feature '") + feature + "' used by " +
              used[feature] + " is not allowed.");
    }
  }

  // Validate the required and disallowed constraints for each file
  for (ObjFile *file : symtab->objectFiles) {
    StringRef fileName(file->getName());
    SmallSet<std::string, 8> objectFeatures;
    for (auto &feature : file->getWasmObj()->getTargetFeatures()) {
      if (feature.Prefix == WASM_FEATURE_PREFIX_DISALLOWED)
        continue;
      objectFeatures.insert(feature.Name);
      if (disallowed.count(feature.Name))
        error(Twine("Target feature '") + feature.Name + "' used in " +
              fileName + " is disallowed by " + disallowed[feature.Name] +
              ". Use --no-check-features to suppress.");
    }
    for (auto &feature : required.keys()) {
      if (!objectFeatures.count(feature))
        error(Twine("Missing target feature '") + feature + "' in " + fileName +
              ", required by " + required[feature] +
              ". Use --no-check-features to suppress.");
    }
  }
}

void Writer::calculateImports() {
  for (Symbol *sym : symtab->getSymbols()) {
    if (!sym->isUndefined())
      continue;
    if (sym->isWeak() && !config->relocatable)
      continue;
    if (!sym->isLive())
      continue;
    if (!sym->isUsedInRegularObj)
      continue;
    // We don't generate imports for data symbols. They however can be imported
    // as GOT entries.
    if (isa<DataSymbol>(sym))
      continue;

    LLVM_DEBUG(dbgs() << "import: " << sym->getName() << "\n");
    out.importSec->addImport(sym);
  }
}

void Writer::calculateExports() {
  if (config->relocatable)
    return;

  if (!config->relocatable && !config->importMemory)
    out.exportSec->exports.push_back(
        WasmExport{"memory", WASM_EXTERNAL_MEMORY, 0});

<<<<<<< HEAD
  if (!config->relocatable && config->exportTable)
    out.exportSec->exports.push_back(
        WasmExport{functionTableName, WASM_EXTERNAL_TABLE, 0});
=======
  if (!Config->Relocatable && Config->ExportTable)
    Exports.push_back(WasmExport{FunctionTableName, WASM_EXTERNAL_TABLE, 0});
>>>>>>> release/8.x

  unsigned globalIndex =
      out.importSec->getNumImportedGlobals() + out.globalSec->numGlobals();

  for (Symbol *sym : symtab->getSymbols()) {
    if (!sym->isExported())
      continue;
    if (!sym->isLive())
      continue;

    StringRef name = sym->getName();
    WasmExport export_;
    if (auto *f = dyn_cast<DefinedFunction>(sym)) {
      export_ = {name, WASM_EXTERNAL_FUNCTION, f->getFunctionIndex()};
    } else if (auto *g = dyn_cast<DefinedGlobal>(sym)) {
      // TODO(sbc): Remove this check once to mutable global proposal is
      // implement in all major browsers.
      // See: https://github.com/WebAssembly/mutable-global
      if (g->getGlobalType()->Mutable) {
        // Only __stack_pointer and __tls_base should ever be create as mutable.
        assert(g == WasmSym::stackPointer || g == WasmSym::tlsBase);
        continue;
      }
      export_ = {name, WASM_EXTERNAL_GLOBAL, g->getGlobalIndex()};
    } else if (auto *e = dyn_cast<DefinedEvent>(sym)) {
      export_ = {name, WASM_EXTERNAL_EVENT, e->getEventIndex()};
    } else {
      auto *d = cast<DefinedData>(sym);
      out.globalSec->dataAddressGlobals.push_back(d);
      export_ = {name, WASM_EXTERNAL_GLOBAL, globalIndex++};
    }

    LLVM_DEBUG(dbgs() << "Export: " << name << "\n");
    out.exportSec->exports.push_back(export_);
  }
}

void Writer::populateSymtab() {
  if (!config->relocatable && !config->emitRelocs)
    return;

  for (Symbol *sym : symtab->getSymbols())
    if (sym->isUsedInRegularObj && sym->isLive())
      out.linkingSec->addToSymtab(sym);

<<<<<<< HEAD
  for (ObjFile *file : symtab->objectFiles) {
    LLVM_DEBUG(dbgs() << "Local symtab entries: " << file->getName() << "\n");
    for (Symbol *sym : file->getSymbols())
      if (sym->isLocal() && !isa<SectionSymbol>(sym) && sym->isLive())
        out.linkingSec->addToSymtab(sym);
  }
=======
  unsigned SymbolIndex = SymtabEntries.size();

  auto AddSymbol = [&](Symbol *Sym) {
    if (auto *S = dyn_cast<SectionSymbol>(Sym)) {
      StringRef Name = S->getName();
      if (CustomSectionMapping.count(Name) == 0)
        return;

      auto SSI = SectionSymbolIndices.find(Name);
      if (SSI != SectionSymbolIndices.end()) {
        Sym->setOutputSymbolIndex(SSI->second);
        return;
      }

      SectionSymbolIndices[Name] = SymbolIndex;
      CustomSectionSymbols[Name] = cast<SectionSymbol>(Sym);

      Sym->markLive();
    }

    // (Since this is relocatable output, GC is not performed so symbols must
    // be live.)
    assert(Sym->isLive());
    Sym->setOutputSymbolIndex(SymbolIndex++);
    SymtabEntries.emplace_back(Sym);
  };

  for (Symbol *Sym : Symtab->getSymbols())
    if (!Sym->isLazy())
      AddSymbol(Sym);

  for (ObjFile *File : Symtab->ObjectFiles) {
    LLVM_DEBUG(dbgs() << "Local symtab entries: " << File->getName() << "\n");
    for (Symbol *Sym : File->getSymbols())
      if (Sym->isLocal())
        AddSymbol(Sym);
  }
}

uint32_t Writer::lookupType(const WasmSignature &Sig) {
  auto It = TypeIndices.find(Sig);
  if (It == TypeIndices.end()) {
    error("type not found: " + toString(Sig));
    return 0;
  }
  return It->second;
}

uint32_t Writer::registerType(const WasmSignature &Sig) {
  auto Pair = TypeIndices.insert(std::make_pair(Sig, Types.size()));
  if (Pair.second) {
    LLVM_DEBUG(dbgs() << "type " << toString(Sig) << "\n");
    Types.push_back(&Sig);
  }
  return Pair.first->second;
>>>>>>> release/8.x
}

void Writer::calculateTypes() {
  // The output type section is the union of the following sets:
  // 1. Any signature used in the TYPE relocation
  // 2. The signatures of all imported functions
  // 3. The signatures of all defined functions
  // 4. The signatures of all imported events
  // 5. The signatures of all defined events

  for (ObjFile *file : symtab->objectFiles) {
    ArrayRef<WasmSignature> types = file->getWasmObj()->types();
    for (uint32_t i = 0; i < types.size(); i++)
      if (file->typeIsUsed[i])
        file->typeMap[i] = out.typeSec->registerType(types[i]);
  }

  for (const Symbol *sym : out.importSec->importedSymbols) {
    if (auto *f = dyn_cast<FunctionSymbol>(sym))
      out.typeSec->registerType(*f->signature);
    else if (auto *e = dyn_cast<EventSymbol>(sym))
      out.typeSec->registerType(*e->signature);
  }

  for (const InputFunction *f : out.functionSec->inputFunctions)
    out.typeSec->registerType(f->signature);

  for (const InputEvent *e : out.eventSec->inputEvents)
    out.typeSec->registerType(e->signature);
}

static void scanRelocations() {
  for (ObjFile *file : symtab->objectFiles) {
    LLVM_DEBUG(dbgs() << "scanRelocations: " << file->getName() << "\n");
    for (InputChunk *chunk : file->functions)
      scanRelocations(chunk);
    for (InputChunk *chunk : file->segments)
      scanRelocations(chunk);
    for (auto &p : file->customSections)
      scanRelocations(p);
  }
}

void Writer::assignIndexes() {
  // Seal the import section, since other index spaces such as function and
  // global are effected by the number of imports.
  out.importSec->seal();

  for (InputFunction *func : symtab->syntheticFunctions)
    out.functionSec->addFunction(func);

  for (ObjFile *file : symtab->objectFiles) {
    LLVM_DEBUG(dbgs() << "Functions: " << file->getName() << "\n");
    for (InputFunction *func : file->functions)
      out.functionSec->addFunction(func);
  }

  for (InputGlobal *global : symtab->syntheticGlobals)
    out.globalSec->addGlobal(global);

  for (ObjFile *file : symtab->objectFiles) {
    LLVM_DEBUG(dbgs() << "Globals: " << file->getName() << "\n");
    for (InputGlobal *global : file->globals)
      out.globalSec->addGlobal(global);
  }

  for (ObjFile *file : symtab->objectFiles) {
    LLVM_DEBUG(dbgs() << "Events: " << file->getName() << "\n");
    for (InputEvent *event : file->events)
      out.eventSec->addEvent(event);
  }

  out.globalSec->assignIndexes();
}

static StringRef getOutputDataSegmentName(StringRef name) {
  // With PIC code we currently only support a single data segment since
  // we only have a single __memory_base to use as our base address.
  if (config->isPic)
    return ".data";
  // We only support one thread-local segment, so we must merge the segments
  // despite --no-merge-data-segments.
  // We also need to merge .tbss into .tdata so they share the same offsets.
  if (name.startswith(".tdata") || name.startswith(".tbss"))
    return ".tdata";
  if (!config->mergeDataSegments)
    return name;
  if (name.startswith(".text."))
    return ".text";
  if (name.startswith(".data."))
    return ".data";
  if (name.startswith(".bss."))
    return ".bss";
  if (name.startswith(".rodata."))
    return ".rodata";
  return name;
}

void Writer::createOutputSegments() {
  for (ObjFile *file : symtab->objectFiles) {
    for (InputSegment *segment : file->segments) {
      if (!segment->live)
        continue;
      StringRef name = getOutputDataSegmentName(segment->getName());
      OutputSegment *&s = segmentMap[name];
      if (s == nullptr) {
        LLVM_DEBUG(dbgs() << "new segment: " << name << "\n");
        s = make<OutputSegment>(name);
        if (config->sharedMemory || name == ".tdata")
          s->initFlags = WASM_SEGMENT_IS_PASSIVE;
        segments.push_back(s);
      }
      s->addInputSegment(segment);
      LLVM_DEBUG(dbgs() << "added data: " << name << ": " << s->size << "\n");
    }
  }

  // Sort segments by type, placing .bss last
  std::stable_sort(segments.begin(), segments.end(),
                   [](const OutputSegment *a, const OutputSegment *b) {
                     auto order = [](StringRef name) {
                       return StringSwitch<int>(name)
                           .StartsWith(".rodata", 0)
                           .StartsWith(".data", 1)
                           .StartsWith(".tdata", 2)
                           .StartsWith(".bss", 4)
                           .Default(3);
                     };
                     return order(a->name) < order(b->name);
                   });

  for (size_t i = 0; i < segments.size(); ++i)
    segments[i]->index = i;
}

static void createFunction(DefinedFunction *func, StringRef bodyContent) {
  std::string functionBody;
  {
    raw_string_ostream os(functionBody);
    writeUleb128(os, bodyContent.size(), "function size");
    os << bodyContent;
  }
  ArrayRef<uint8_t> body = arrayRefFromStringRef(saver.save(functionBody));
  cast<SyntheticFunction>(func->function)->setBody(body);
}

void Writer::createInitMemoryFunction() {
  LLVM_DEBUG(dbgs() << "createInitMemoryFunction\n");
  assert(WasmSym::initMemoryFlag);
  uint32_t flagAddress = WasmSym::initMemoryFlag->getVirtualAddress();
  std::string bodyContent;
  {
    raw_string_ostream os(bodyContent);
    writeUleb128(os, 0, "num locals");

    if (segments.size()) {
      // Initialize memory in a thread-safe manner. The thread that successfully
      // increments the flag from 0 to 1 is is responsible for performing the
      // memory initialization. Other threads go sleep on the flag until the
      // first thread finishing initializing memory, increments the flag to 2,
      // and wakes all the other threads. Once the flag has been set to 2,
      // subsequently started threads will skip the sleep. All threads
      // unconditionally drop their passive data segments once memory has been
      // initialized. The generated code is as follows:
      //
      // (func $__wasm_init_memory
      //  (if
      //   (i32.atomic.rmw.cmpxchg align=2 offset=0
      //    (i32.const $__init_memory_flag)
      //    (i32.const 0)
      //    (i32.const 1)
      //   )
      //   (then
      //    (drop
      //     (i32.atomic.wait align=2 offset=0
      //      (i32.const $__init_memory_flag)
      //      (i32.const 1)
      //      (i32.const -1)
      //     )
      //    )
      //   )
      //   (else
      //    ( ... initialize data segments ... )
      //    (i32.atomic.store align=2 offset=0
      //     (i32.const $__init_memory_flag)
      //     (i32.const 2)
      //    )
      //    (drop
      //     (i32.atomic.notify align=2 offset=0
      //      (i32.const $__init_memory_flag)
      //      (i32.const -1u)
      //     )
      //    )
      //   )
      //  )
      //  ( ... drop data segments ... )
      // )

      // Atomically check whether this is the main thread.
      writeI32Const(os, flagAddress, "flag address");
      writeI32Const(os, 0, "expected flag value");
      writeI32Const(os, 1, "flag value");
      writeU8(os, WASM_OPCODE_ATOMICS_PREFIX, "atomics prefix");
      writeUleb128(os, WASM_OPCODE_I32_RMW_CMPXCHG, "i32.atomic.rmw.cmpxchg");
      writeMemArg(os, 2, 0);
      writeU8(os, WASM_OPCODE_IF, "IF");
      writeU8(os, WASM_TYPE_NORESULT, "blocktype");

      // Did not increment 0, so wait for main thread to initialize memory
      writeI32Const(os, flagAddress, "flag address");
      writeI32Const(os, 1, "expected flag value");
      writeI64Const(os, -1, "timeout");
      writeU8(os, WASM_OPCODE_ATOMICS_PREFIX, "atomics prefix");
      writeUleb128(os, WASM_OPCODE_I32_ATOMIC_WAIT, "i32.atomic.wait");
      writeMemArg(os, 2, 0);
      writeU8(os, WASM_OPCODE_DROP, "drop");

      writeU8(os, WASM_OPCODE_ELSE, "ELSE");

      // Did increment 0, so conditionally initialize passive data segments
      for (const OutputSegment *s : segments) {
        if (s->initFlags & WASM_SEGMENT_IS_PASSIVE && s->name != ".tdata") {
          // destination address
          writeI32Const(os, s->startVA, "destination address");
          // source segment offset
          writeI32Const(os, 0, "segment offset");
          // memory region size
          writeI32Const(os, s->size, "memory region size");
          // memory.init instruction
          writeU8(os, WASM_OPCODE_MISC_PREFIX, "bulk-memory prefix");
          writeUleb128(os, WASM_OPCODE_MEMORY_INIT, "memory.init");
          writeUleb128(os, s->index, "segment index immediate");
          writeU8(os, 0, "memory index immediate");
        }
      }

      // Set flag to 2 to mark end of initialization
      writeI32Const(os, flagAddress, "flag address");
      writeI32Const(os, 2, "flag value");
      writeU8(os, WASM_OPCODE_ATOMICS_PREFIX, "atomics prefix");
      writeUleb128(os, WASM_OPCODE_I32_ATOMIC_STORE, "i32.atomic.store");
      writeMemArg(os, 2, 0);

      // Notify any waiters that memory initialization is complete
      writeI32Const(os, flagAddress, "flag address");
      writeI32Const(os, -1, "number of waiters");
      writeU8(os, WASM_OPCODE_ATOMICS_PREFIX, "atomics prefix");
      writeUleb128(os, WASM_OPCODE_ATOMIC_NOTIFY, "atomic.notify");
      writeMemArg(os, 2, 0);
      writeU8(os, WASM_OPCODE_DROP, "drop");

      writeU8(os, WASM_OPCODE_END, "END");

      // Unconditionally drop passive data segments
      for (const OutputSegment *s : segments) {
        if (s->initFlags & WASM_SEGMENT_IS_PASSIVE && s->name != ".tdata") {
          // data.drop instruction
          writeU8(os, WASM_OPCODE_MISC_PREFIX, "bulk-memory prefix");
          writeUleb128(os, WASM_OPCODE_DATA_DROP, "data.drop");
          writeUleb128(os, s->index, "segment index immediate");
        }
      }
    }
    writeU8(os, WASM_OPCODE_END, "END");
  }

  createFunction(WasmSym::initMemory, bodyContent);
}

// For -shared (PIC) output, we create create a synthetic function which will
// apply any relocations to the data segments on startup.  This function is
// called __wasm_apply_relocs and is added at the beginning of __wasm_call_ctors
// before any of the constructors run.
void Writer::createApplyRelocationsFunction() {
  LLVM_DEBUG(dbgs() << "createApplyRelocationsFunction\n");
  // First write the body's contents to a string.
  std::string bodyContent;
  {
    raw_string_ostream os(bodyContent);
    writeUleb128(os, 0, "num locals");
    for (const OutputSegment *seg : segments)
      for (const InputSegment *inSeg : seg->inputSegments)
        inSeg->generateRelocationCode(os);
    writeU8(os, WASM_OPCODE_END, "END");
  }

  createFunction(WasmSym::applyRelocs, bodyContent);
}

// Create synthetic "__wasm_call_ctors" function based on ctor functions
// in input object.
void Writer::createCallCtorsFunction() {
  if (!WasmSym::callCtors->isLive())
    return;

  // First write the body's contents to a string.
  std::string bodyContent;
  {
    raw_string_ostream os(bodyContent);
    writeUleb128(os, 0, "num locals");

    if (config->isPic) {
      writeU8(os, WASM_OPCODE_CALL, "CALL");
      writeUleb128(os, WasmSym::applyRelocs->getFunctionIndex(),
                   "function index");
    }

    // Call constructors
    for (const WasmInitEntry &f : initFunctions) {
      writeU8(os, WASM_OPCODE_CALL, "CALL");
      writeUleb128(os, f.sym->getFunctionIndex(), "function index");
    }
    writeU8(os, WASM_OPCODE_END, "END");
  }

  createFunction(WasmSym::callCtors, bodyContent);
}

void Writer::createInitTLSFunction() {
  if (!WasmSym::initTLS->isLive())
    return;

  std::string bodyContent;
  {
    raw_string_ostream os(bodyContent);

    OutputSegment *tlsSeg = nullptr;
    for (auto *seg : segments) {
      if (seg->name == ".tdata") {
        tlsSeg = seg;
        break;
      }
    }

    writeUleb128(os, 0, "num locals");
    if (tlsSeg) {
      writeU8(os, WASM_OPCODE_LOCAL_GET, "local.get");
      writeUleb128(os, 0, "local index");

      writeU8(os, WASM_OPCODE_GLOBAL_SET, "global.set");
      writeUleb128(os, WasmSym::tlsBase->getGlobalIndex(), "global index");

      writeU8(os, WASM_OPCODE_LOCAL_GET, "local.get");
      writeUleb128(os, 0, "local index");

      writeI32Const(os, 0, "segment offset");

      writeI32Const(os, tlsSeg->size, "memory region size");

      writeU8(os, WASM_OPCODE_MISC_PREFIX, "bulk-memory prefix");
      writeUleb128(os, WASM_OPCODE_MEMORY_INIT, "MEMORY.INIT");
      writeUleb128(os, tlsSeg->index, "segment index immediate");
      writeU8(os, 0, "memory index immediate");
    }
    writeU8(os, WASM_OPCODE_END, "end function");
  }

  createFunction(WasmSym::initTLS, bodyContent);
}

// Populate InitFunctions vector with init functions from all input objects.
// This is then used either when creating the output linking section or to
// synthesize the "__wasm_call_ctors" function.
void Writer::calculateInitFunctions() {
  if (!config->relocatable && !WasmSym::callCtors->isLive())
    return;

  for (ObjFile *file : symtab->objectFiles) {
    const WasmLinkingData &l = file->getWasmObj()->linkingData();
    for (const WasmInitFunc &f : l.InitFunctions) {
      FunctionSymbol *sym = file->getFunctionSymbol(f.Symbol);
      // comdat exclusions can cause init functions be discarded.
      if (sym->isDiscarded())
        continue;
      assert(sym->isLive());
      if (*sym->signature != WasmSignature{{}, {}})
        error("invalid signature for init func: " + toString(*sym));
      LLVM_DEBUG(dbgs() << "initFunctions: " << toString(*sym) << "\n");
      initFunctions.emplace_back(WasmInitEntry{sym, f.Priority});
    }
  }

  // Sort in order of priority (lowest first) so that they are called
  // in the correct order.
  llvm::stable_sort(initFunctions,
                    [](const WasmInitEntry &l, const WasmInitEntry &r) {
                      return l.priority < r.priority;
                    });
}

void Writer::createSyntheticSections() {
  out.dylinkSec = make<DylinkSection>();
  out.typeSec = make<TypeSection>();
  out.importSec = make<ImportSection>();
  out.functionSec = make<FunctionSection>();
  out.tableSec = make<TableSection>();
  out.memorySec = make<MemorySection>();
  out.globalSec = make<GlobalSection>();
  out.eventSec = make<EventSection>();
  out.exportSec = make<ExportSection>();
  out.startSec = make<StartSection>(segments.size());
  out.elemSec = make<ElemSection>();
  out.dataCountSec = make<DataCountSection>(segments.size());
  out.linkingSec = make<LinkingSection>(initFunctions, segments);
  out.nameSec = make<NameSection>();
  out.producersSec = make<ProducersSection>();
  out.targetFeaturesSec = make<TargetFeaturesSection>();
}

void Writer::run() {
  if (config->relocatable || config->isPic)
    config->globalBase = 0;

  // For PIC code the table base is assigned dynamically by the loader.
  // For non-PIC, we start at 1 so that accessing table index 0 always traps.
  if (!config->isPic) {
    config->tableBase = 1;
    if (WasmSym::definedTableBase)
      WasmSym::definedTableBase->setVirtualAddress(config->tableBase);
  }

  log("-- createOutputSegments");
  createOutputSegments();
  log("-- createSyntheticSections");
  createSyntheticSections();
  log("-- populateProducers");
  populateProducers();
  log("-- populateTargetFeatures");
  populateTargetFeatures();
  log("-- calculateImports");
  calculateImports();
  log("-- layoutMemory");
  layoutMemory();

  if (!config->relocatable) {
    // Create linker synthesized __start_SECNAME/__stop_SECNAME symbols
    // This has to be done after memory layout is performed.
    for (const OutputSegment *seg : segments)
      addStartStopSymbols(seg);
  }

  log("-- scanRelocations");
  scanRelocations();
  log("-- assignIndexes");
  assignIndexes();
  log("-- calculateInitFunctions");
  calculateInitFunctions();

  if (!config->relocatable) {
    // Create linker synthesized functions
    if (config->sharedMemory)
      createInitMemoryFunction();
    if (config->isPic)
      createApplyRelocationsFunction();
    createCallCtorsFunction();
  }

  if (!config->relocatable && config->sharedMemory && !config->shared)
    createInitTLSFunction();

  if (errorCount())
    return;

  log("-- calculateTypes");
  calculateTypes();
  log("-- calculateExports");
  calculateExports();
  log("-- calculateCustomSections");
  calculateCustomSections();
  log("-- populateSymtab");
  populateSymtab();
  log("-- addSections");
  addSections();

  if (errorHandler().verbose) {
    log("Defined Functions: " + Twine(out.functionSec->inputFunctions.size()));
    log("Defined Globals  : " + Twine(out.globalSec->numGlobals()));
    log("Defined Events   : " + Twine(out.eventSec->inputEvents.size()));
    log("Function Imports : " +
        Twine(out.importSec->getNumImportedFunctions()));
    log("Global Imports   : " + Twine(out.importSec->getNumImportedGlobals()));
    log("Event Imports    : " + Twine(out.importSec->getNumImportedEvents()));
    for (ObjFile *file : symtab->objectFiles)
      file->dumpInfo();
  }

  createHeader();
  log("-- finalizeSections");
  finalizeSections();

  log("-- openFile");
  openFile();
  if (errorCount())
    return;

  writeHeader();

  log("-- writeSections");
  writeSections();
  if (errorCount())
    return;

  if (Error e = buffer->commit())
    fatal("failed to write the output file: " + toString(std::move(e)));
}

// Open a result file.
void Writer::openFile() {
  log("writing: " + config->outputFile);

  Expected<std::unique_ptr<FileOutputBuffer>> bufferOrErr =
      FileOutputBuffer::create(config->outputFile, fileSize,
                               FileOutputBuffer::F_executable);

  if (!bufferOrErr)
    error("failed to open " + config->outputFile + ": " +
          toString(bufferOrErr.takeError()));
  else
    buffer = std::move(*bufferOrErr);
}

void Writer::createHeader() {
  raw_string_ostream os(header);
  writeBytes(os, WasmMagic, sizeof(WasmMagic), "wasm magic");
  writeU32(os, WasmVersion, "wasm version");
  os.flush();
  fileSize += header.size();
}

void lld::wasm::writeResult() { Writer().run(); }
