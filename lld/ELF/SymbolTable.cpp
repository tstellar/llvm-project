//===- SymbolTable.cpp ----------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Symbol table is a bag of all known symbols. We put all symbols of
// all input files to the symbol table. The symbol table is basically
// a hash table with the logic to resolve symbol name conflicts using
// the symbol types.
//
//===----------------------------------------------------------------------===//

#include "SymbolTable.h"
#include "Config.h"
#include "LinkerScript.h"
#include "Symbols.h"
#include "SyntheticSections.h"
#include "lld/Common/ErrorHandler.h"
#include "lld/Common/Memory.h"
#include "lld/Common/Strings.h"
#include "llvm/ADT/STLExtras.h"

using namespace llvm;
using namespace llvm::object;
using namespace llvm::ELF;

using namespace lld;
using namespace lld::elf;

<<<<<<< HEAD
SymbolTable *elf::symtab;
=======
// All input object files must be for the same architecture
// (e.g. it does not make sense to link x86 object files with
// MIPS object files.) This function checks for that error.
template <class ELFT> static bool isCompatible(InputFile *F) {
  if (!isa<ELFFileBase<ELFT>>(F) && !isa<BitcodeFile>(F))
    return true;

  if (F->EKind == Config->EKind && F->EMachine == Config->EMachine) {
    if (Config->EMachine != EM_MIPS)
      return true;
    if (isMipsN32Abi(F) == Config->MipsN32Abi)
      return true;
  }

  if (!Config->Emulation.empty())
    error(toString(F) + " is incompatible with " + Config->Emulation);
  else
    error(toString(F) + " is incompatible with " + toString(Config->FirstElf));
  return false;
}

// Add symbols in File to the symbol table.
template <class ELFT> void SymbolTable<ELFT>::addFile(InputFile *File) {
  if (!Config->FirstElf && isa<ELFFileBase<ELFT>>(File))
    Config->FirstElf = File;

  if (!isCompatible<ELFT>(File))
    return;

  // Binary file
  if (auto *F = dyn_cast<BinaryFile>(File)) {
    BinaryFiles.push_back(F);
    F->parse<ELFT>();
    return;
  }

  // .a file
  if (auto *F = dyn_cast<ArchiveFile>(File)) {
    F->parse<ELFT>();
    return;
  }

  // Lazy object file
  if (auto *F = dyn_cast<LazyObjectFile>(File)) {
    F->parse<ELFT>();
    return;
  }

  if (Config->Trace)
    message(toString(File));

  // .so file
  if (auto *F = dyn_cast<SharedFile<ELFT>>(File)) {
    // DSOs are uniquified not by filename but by soname.
    F->parseSoName();
    if (ErrorCount || !SoNames.insert(F->SoName).second)
      return;
    SharedFiles.push_back(F);
    F->parseRest();
    return;
  }

  // LLVM bitcode file
  if (auto *F = dyn_cast<BitcodeFile>(File)) {
    BitcodeFiles.push_back(F);
    F->parse<ELFT>(ComdatGroups);
    return;
  }

  // Regular object file
  auto *F = cast<ObjectFile<ELFT>>(File);
  ObjectFiles.push_back(F);
  F->parse(ComdatGroups);
}

// This function is where all the optimizations of link-time
// optimization happens. When LTO is in use, some input files are
// not in native object file format but in the LLVM bitcode format.
// This function compiles bitcode files into a few big native files
// using LLVM functions and replaces bitcode symbols with the results.
// Because all bitcode files that consist of a program are passed
// to the compiler at once, it can do whole-program optimization.
template <class ELFT> void SymbolTable<ELFT>::addCombinedLTOObject() {
  if (BitcodeFiles.empty())
    return;

  // Compile bitcode files and replace bitcode symbols.
  LTO.reset(new BitcodeCompiler);
  for (BitcodeFile *F : BitcodeFiles)
    LTO->add(*F);

  for (InputFile *File : LTO->compile()) {
    ObjectFile<ELFT> *Obj = cast<ObjectFile<ELFT>>(File);
    DenseSet<CachedHashStringRef> DummyGroups;
    Obj->parse(DummyGroups);
    ObjectFiles.push_back(Obj);
  }
}

template <class ELFT>
DefinedRegular *SymbolTable<ELFT>::addAbsolute(StringRef Name,
                                               uint8_t Visibility,
                                               uint8_t Binding) {
  Symbol *Sym =
      addRegular(Name, Visibility, STT_NOTYPE, 0, 0, Binding, nullptr, nullptr);
  return cast<DefinedRegular>(Sym->body());
}

// Add Name as an "ignored" symbol. An ignored symbol is a regular
// linker-synthesized defined symbol, but is only defined if needed.
template <class ELFT>
DefinedRegular *SymbolTable<ELFT>::addIgnored(StringRef Name,
                                              uint8_t Visibility) {
  SymbolBody *S = find(Name);
  if (!S || S->isInCurrentDSO())
    return nullptr;
  return addAbsolute(Name, Visibility);
}

// Set a flag for --trace-symbol so that we can print out a log message
// if a new symbol with the same name is inserted into the symbol table.
template <class ELFT> void SymbolTable<ELFT>::trace(StringRef Name) {
  Symtab.insert({CachedHashStringRef(Name), {-1, true}});
}

// Rename SYM as __wrap_SYM. The original symbol is preserved as __real_SYM.
// Used to implement --wrap.
template <class ELFT> void SymbolTable<ELFT>::addSymbolWrap(StringRef Name) {
  SymbolBody *B = find(Name);
  if (!B)
    return;
  Symbol *Sym = B->symbol();
  Symbol *Real = addUndefined(Saver.save("__real_" + Name));
  Symbol *Wrap = addUndefined(Saver.save("__wrap_" + Name));

  // Tell LTO not to eliminate this symbol
  Wrap->IsUsedInRegularObj = true;

  Config->RenamedSymbols[Real] = {Sym, Real->Binding};
  Config->RenamedSymbols[Sym] = {Wrap, Sym->Binding};
}

// Creates alias for symbol. Used to implement --defsym=ALIAS=SYM.
template <class ELFT>
void SymbolTable<ELFT>::addSymbolAlias(StringRef Alias, StringRef Name) {
  SymbolBody *B = find(Name);
  if (!B) {
    error("-defsym: undefined symbol: " + Name);
    return;
  }
  Symbol *Sym = B->symbol();
  Symbol *AliasSym = addUndefined(Alias);

  // Tell LTO not to eliminate this symbol
  Sym->IsUsedInRegularObj = true;
  Config->RenamedSymbols[AliasSym] = {Sym, AliasSym->Binding};
}

// Apply symbol renames created by -wrap and -defsym. The renames are created
// before LTO in addSymbolWrap() and addSymbolAlias() to have a chance to inform
// LTO (if LTO is running) not to include these symbols in IPO. Now that the
// symbols are finalized, we can perform the replacement.
template <class ELFT> void SymbolTable<ELFT>::applySymbolRenames() {
  for (auto &KV : Config->RenamedSymbols) {
    Symbol *Dst = KV.first;
    Symbol *Src = KV.second.Target;
    Dst->body()->copy(Src->body());
    Dst->Binding = KV.second.OriginalBinding;
  }
}

static uint8_t getMinVisibility(uint8_t VA, uint8_t VB) {
  if (VA == STV_DEFAULT)
    return VB;
  if (VB == STV_DEFAULT)
    return VA;
  return std::min(VA, VB);
}

// Find an existing symbol or create and insert a new one.
template <class ELFT>
std::pair<Symbol *, bool> SymbolTable<ELFT>::insert(StringRef Name) {
  // <name>@@<version> means the symbol is the default version. In that
  // case <name>@@<version> will be used to resolve references to <name>.
  size_t Pos = Name.find("@@");
  if (Pos != StringRef::npos)
    Name = Name.take_front(Pos);

  auto P = Symtab.insert(
      {CachedHashStringRef(Name), SymIndex((int)SymVector.size(), false)});
  SymIndex &V = P.first->second;
  bool IsNew = P.second;

  if (V.Idx == -1) {
    IsNew = true;
    V = SymIndex((int)SymVector.size(), true);
  }
>>>>>>> origin/release/5.x

void SymbolTable::wrap(Symbol *sym, Symbol *real, Symbol *wrap) {
  // Swap symbols as instructed by -wrap.
  int &idx1 = symMap[CachedHashStringRef(sym->getName())];
  int &idx2 = symMap[CachedHashStringRef(real->getName())];
  int &idx3 = symMap[CachedHashStringRef(wrap->getName())];

<<<<<<< HEAD
  idx2 = idx1;
  idx1 = idx3;

  // Now renaming is complete. No one refers Real symbol. We could leave
  // Real as-is, but if Real is written to the symbol table, that may
  // contain irrelevant values. So, we copy all values from Sym to Real.
  StringRef s = real->getName();
  memcpy(real, sym, sizeof(SymbolUnion));
  real->setName(s);
}

// Find an existing symbol or create a new one.
Symbol *SymbolTable::insert(StringRef name) {
  // <name>@@<version> means the symbol is the default version. In that
  // case <name>@@<version> will be used to resolve references to <name>.
  //
  // Since this is a hot path, the following string search code is
  // optimized for speed. StringRef::find(char) is much faster than
  // StringRef::find(StringRef).
  size_t pos = name.find('@');
  if (pos != StringRef::npos && pos + 1 < name.size() && name[pos + 1] == '@')
    name = name.take_front(pos);

  auto p = symMap.insert({CachedHashStringRef(name), (int)symVector.size()});
  int &symIndex = p.first->second;
  bool isNew = p.second;

  if (!isNew)
    return symVector[symIndex];

  Symbol *sym = reinterpret_cast<Symbol *>(make<SymbolUnion>());
  symVector.push_back(sym);

  // *sym was not initialized by a constructor. Fields that may get referenced
  // when it is a placeholder must be initialized here.
  sym->setName(name);
  sym->symbolKind = Symbol::PlaceholderKind;
  sym->versionId = VER_NDX_GLOBAL;
  sym->visibility = STV_DEFAULT;
  sym->isUsedInRegularObj = false;
  sym->exportDynamic = false;
  sym->inDynamicList = false;
  sym->canInline = true;
  sym->referenced = false;
  sym->traced = false;
  sym->scriptDefined = false;
  sym->partition = 1;
  return sym;
}

Symbol *SymbolTable::addSymbol(const Symbol &newSym) {
  Symbol *sym = symtab->insert(newSym.getName());
  sym->resolve(newSym);
  return sym;
}

Symbol *SymbolTable::find(StringRef name) {
  auto it = symMap.find(CachedHashStringRef(name));
  if (it == symMap.end())
=======
// Using .symver foo,foo@@VER unfortunately creates two symbols: foo and
// foo@@VER. We want to effectively ignore foo, so give precedence to
// foo@@VER.
// FIXME: If users can transition to using
// .symver foo,foo@@@VER
// we can delete this hack.
static int compareVersion(Symbol *S, StringRef Name) {
  if (Name.find("@@") != StringRef::npos &&
      S->body()->getName().find("@@") == StringRef::npos)
    return 1;
  if (Name.find("@@") == StringRef::npos &&
      S->body()->getName().find("@@") != StringRef::npos)
    return -1;
  return 0;
}

// We have a new defined symbol with the specified binding. Return 1 if the new
// symbol should win, -1 if the new symbol should lose, or 0 if both symbols are
// strong defined symbols.
static int compareDefined(Symbol *S, bool WasInserted, uint8_t Binding,
                          StringRef Name) {
  if (WasInserted)
    return 1;
  SymbolBody *Body = S->body();
<<<<<<< HEAD
  if (!Body->isInCurrentDSO())
=======
  if (Body->isLazy() || !Body->isInCurrentDSO())
>>>>>>> origin/release/4.x
    return 1;

  if (int R = compareVersion(S, Name))
    return R;

  if (Binding == STB_WEAK)
    return -1;
  if (S->isWeak())
    return 1;
  return 0;
}

// We have a new non-common defined symbol with the specified binding. Return 1
// if the new symbol should win, -1 if the new symbol should lose, or 0 if there
// is a conflict. If the new symbol wins, also update the binding.
template <typename ELFT>
static int compareDefinedNonCommon(Symbol *S, bool WasInserted, uint8_t Binding,
                                   bool IsAbsolute, typename ELFT::uint Value,
                                   StringRef Name) {
  if (int Cmp = compareDefined(S, WasInserted, Binding, Name)) {
    if (Cmp > 0)
      S->Binding = Binding;
    return Cmp;
  }
  SymbolBody *B = S->body();
  if (isa<DefinedCommon>(B)) {
    // Non-common symbols take precedence over common symbols.
    if (Config->WarnCommon)
      warn("common " + S->body()->getName() + " is overridden");
    return 1;
  } else if (auto *R = dyn_cast<DefinedRegular>(B)) {
    if (R->Section == nullptr && Binding == STB_GLOBAL && IsAbsolute &&
        R->Value == Value)
      return -1;
  }
  return 0;
}

template <class ELFT>
Symbol *SymbolTable<ELFT>::addCommon(StringRef N, uint64_t Size,
                                     uint32_t Alignment, uint8_t Binding,
                                     uint8_t StOther, uint8_t Type,
                                     InputFile *File) {
  Symbol *S;
  bool WasInserted;
  std::tie(S, WasInserted) = insert(N, Type, getVisibility(StOther),
                                    /*CanOmitFromDynSym*/ false, File);
  int Cmp = compareDefined(S, WasInserted, Binding, N);
  if (Cmp > 0) {
    S->Binding = Binding;
    replaceBody<DefinedCommon>(S, N, Size, Alignment, StOther, Type, File);
  } else if (Cmp == 0) {
    auto *C = dyn_cast<DefinedCommon>(S->body());
    if (!C) {
      // Non-common symbols take precedence over common symbols.
      if (Config->WarnCommon)
        warn("common " + S->body()->getName() + " is overridden");
      return S;
    }

    if (Config->WarnCommon)
      warn("multiple common of " + S->body()->getName());

    Alignment = C->Alignment = std::max(C->Alignment, Alignment);
    if (Size > C->Size)
      replaceBody<DefinedCommon>(S, N, Size, Alignment, StOther, Type, File);
  }
  return S;
}

static void warnOrError(const Twine &Msg) {
  if (Config->AllowMultipleDefinition)
    warn(Msg);
  else
    error(Msg);
}

static void reportDuplicate(SymbolBody *Sym, InputFile *NewFile) {
  warnOrError("duplicate symbol: " + toString(*Sym) + "\n>>> defined in " +
              toString(Sym->File) + "\n>>> defined in " + toString(NewFile));
}

template <class ELFT>
static void reportDuplicate(SymbolBody *Sym, InputSectionBase *ErrSec,
                            typename ELFT::uint ErrOffset) {
  DefinedRegular *D = dyn_cast<DefinedRegular>(Sym);
  if (!D || !D->Section || !ErrSec) {
    reportDuplicate(Sym, ErrSec ? ErrSec->getFile<ELFT>() : nullptr);
    return;
  }

  // Construct and print an error message in the form of:
  //
  //   ld.lld: error: duplicate symbol: foo
  //   >>> defined at bar.c:30
  //   >>>            bar.o (/home/alice/src/bar.o)
  //   >>> defined at baz.c:563
  //   >>>            baz.o in archive libbaz.a
  auto *Sec1 = cast<InputSectionBase>(D->Section);
  std::string Src1 = Sec1->getSrcMsg<ELFT>(D->Value);
  std::string Obj1 = Sec1->getObjMsg<ELFT>(D->Value);
  std::string Src2 = ErrSec->getSrcMsg<ELFT>(ErrOffset);
  std::string Obj2 = ErrSec->getObjMsg<ELFT>(ErrOffset);

  std::string Msg = "duplicate symbol: " + toString(*Sym) + "\n>>> defined at ";
  if (!Src1.empty())
    Msg += Src1 + "\n>>>            ";
  Msg += Obj1 + "\n>>> defined at ";
  if (!Src2.empty())
    Msg += Src2 + "\n>>>            ";
  Msg += Obj2;
  warnOrError(Msg);
}

template <typename ELFT>
Symbol *SymbolTable<ELFT>::addRegular(StringRef Name, uint8_t StOther,
                                      uint8_t Type, uint64_t Value,
                                      uint64_t Size, uint8_t Binding,
                                      SectionBase *Section, InputFile *File) {
  Symbol *S;
  bool WasInserted;
  std::tie(S, WasInserted) = insert(Name, Type, getVisibility(StOther),
                                    /*CanOmitFromDynSym*/ false, File);
  int Cmp = compareDefinedNonCommon<ELFT>(S, WasInserted, Binding,
                                          Section == nullptr, Value, Name);
  if (Cmp > 0)
    replaceBody<DefinedRegular>(S, Name, /*IsLocal=*/false, StOther, Type,
                                Value, Size, Section, File);
  else if (Cmp == 0)
    reportDuplicate<ELFT>(S->body(),
                          dyn_cast_or_null<InputSectionBase>(Section), Value);
  return S;
}

template <typename ELFT>
void SymbolTable<ELFT>::addShared(SharedFile<ELFT> *File, StringRef Name,
                                  const Elf_Sym &Sym,
                                  const typename ELFT::Verdef *Verdef) {
  // DSO symbols do not affect visibility in the output, so we pass STV_DEFAULT
  // as the visibility, which will leave the visibility in the symbol table
  // unchanged.
  Symbol *S;
  bool WasInserted;
  std::tie(S, WasInserted) = insert(Name, Sym.getType(), STV_DEFAULT,
                                    /*CanOmitFromDynSym*/ true, File);
  // Make sure we preempt DSO symbols with default visibility.
  if (Sym.getVisibility() == STV_DEFAULT)
    S->ExportDynamic = true;
<<<<<<< HEAD

  SymbolBody *Body = S->body();
  // An undefined symbol with non default visibility must be satisfied
  // in the same DSO.
  if (WasInserted ||
      (isa<Undefined>(Body) && Body->getVisibility() == STV_DEFAULT)) {
    replaceBody<SharedSymbol>(S, File, Name, Sym.st_other, Sym.getType(), &Sym,
                              Verdef);
=======
  if (WasInserted || isa<Undefined<ELFT>>(S->body())) {
    replaceBody<SharedSymbol<ELFT>>(S, F, Name, Sym, Verdef);
>>>>>>> origin/release/4.x
    if (!S->isWeak())
      File->IsUsed = true;
  }
}

template <class ELFT>
Symbol *SymbolTable<ELFT>::addBitcode(StringRef Name, uint8_t Binding,
                                      uint8_t StOther, uint8_t Type,
                                      bool CanOmitFromDynSym, BitcodeFile *F) {
  Symbol *S;
  bool WasInserted;
  std::tie(S, WasInserted) =
      insert(Name, Type, getVisibility(StOther), CanOmitFromDynSym, F);
  int Cmp = compareDefinedNonCommon<ELFT>(S, WasInserted, Binding,
                                          /*IsAbs*/ false, /*Value*/ 0, Name);
  if (Cmp > 0)
    replaceBody<DefinedRegular>(S, Name, /*IsLocal=*/false, StOther, Type, 0, 0,
                                nullptr, F);
  else if (Cmp == 0)
    reportDuplicate(S->body(), F);
  return S;
}

template <class ELFT> SymbolBody *SymbolTable<ELFT>::find(StringRef Name) {
  auto It = Symtab.find(CachedHashStringRef(Name));
  if (It == Symtab.end())
>>>>>>> origin/release/5.x
    return nullptr;
  Symbol *sym = symVector[it->second];
  if (sym->isPlaceholder())
    return nullptr;
<<<<<<< HEAD
  return sym;
=======
  return SymVector[V.Idx]->body();
}

template <class ELFT>
SymbolBody *SymbolTable<ELFT>::findInCurrentDSO(StringRef Name) {
  if (SymbolBody *S = find(Name))
    if (S->isInCurrentDSO())
      return S;
  return nullptr;
}

template <class ELFT>
void SymbolTable<ELFT>::addLazyArchive(ArchiveFile *F,
                                       const object::Archive::Symbol Sym) {
  Symbol *S;
  bool WasInserted;
  StringRef Name = Sym.getName();
  std::tie(S, WasInserted) = insert(Name);
  if (WasInserted) {
    replaceBody<LazyArchive>(S, *F, Sym, SymbolBody::UnknownType);
    return;
  }
  if (!S->body()->isUndefined())
    return;

  // Weak undefined symbols should not fetch members from archives. If we were
  // to keep old symbol we would not know that an archive member was available
  // if a strong undefined symbol shows up afterwards in the link. If a strong
  // undefined symbol never shows up, this lazy symbol will get to the end of
  // the link and must be treated as the weak undefined one. We already marked
  // this symbol as used when we added it to the symbol table, but we also need
  // to preserve its type. FIXME: Move the Type field to Symbol.
  if (S->isWeak()) {
    replaceBody<LazyArchive>(S, *F, Sym, S->body()->Type);
    return;
  }
  std::pair<MemoryBufferRef, uint64_t> MBInfo = F->getMember(&Sym);
  if (!MBInfo.first.getBuffer().empty())
    addFile(createObjectFile(MBInfo.first, F->getName(), MBInfo.second));
>>>>>>> origin/release/4.x
}

// Initialize demangledSyms with a map from demangled symbols to symbol
// objects. Used to handle "extern C++" directive in version scripts.
//
// The map will contain all demangled symbols. That can be very large,
// and in LLD we generally want to avoid do anything for each symbol.
// Then, why are we doing this? Here's why.
//
// Users can use "extern C++ {}" directive to match against demangled
// C++ symbols. For example, you can write a pattern such as
// "llvm::*::foo(int, ?)". Obviously, there's no way to handle this
// other than trying to match a pattern against all demangled symbols.
// So, if "extern C++" feature is used, we need to demangle all known
// symbols.
StringMap<std::vector<Symbol *>> &SymbolTable::getDemangledSyms() {
  if (!demangledSyms) {
    demangledSyms.emplace();
    for (Symbol *sym : symVector) {
      if (!sym->isDefined() && !sym->isCommon())
        continue;
      (*demangledSyms)[demangleItanium(sym->getName())].push_back(sym);
    }
  }
  return *demangledSyms;
}

std::vector<Symbol *> SymbolTable::findByVersion(SymbolVersion ver) {
  if (ver.isExternCpp)
    return getDemangledSyms().lookup(ver.name);
  if (Symbol *b = find(ver.name))
    if (b->isDefined() || b->isCommon())
      return {b};
  return {};
}

std::vector<Symbol *> SymbolTable::findAllByVersion(SymbolVersion ver) {
  std::vector<Symbol *> res;
  StringMatcher m(ver.name);

  if (ver.isExternCpp) {
    for (auto &p : getDemangledSyms())
      if (m.match(p.first()))
        res.insert(res.end(), p.second.begin(), p.second.end());
    return res;
  }

  for (Symbol *sym : symVector)
    if ((sym->isDefined() || sym->isCommon()) && m.match(sym->getName()))
      res.push_back(sym);
  return res;
}

// Handles -dynamic-list.
void SymbolTable::handleDynamicList() {
  for (SymbolVersion &ver : config->dynamicList) {
    std::vector<Symbol *> syms;
    if (ver.hasWildcard)
      syms = findAllByVersion(ver);
    else
      syms = findByVersion(ver);

    for (Symbol *sym : syms)
      sym->inDynamicList = true;
  }
}

// Set symbol versions to symbols. This function handles patterns
// containing no wildcard characters.
void SymbolTable::assignExactVersion(SymbolVersion ver, uint16_t versionId,
                                     StringRef versionName) {
  if (ver.hasWildcard)
    return;

  // Get a list of symbols which we need to assign the version to.
  std::vector<Symbol *> syms = findByVersion(ver);
  if (syms.empty()) {
    if (!config->undefinedVersion)
      error("version script assignment of '" + versionName + "' to symbol '" +
            ver.name + "' failed: symbol not defined");
    return;
  }

  auto getName = [](uint16_t ver) -> std::string {
    if (ver == VER_NDX_LOCAL)
      return "VER_NDX_LOCAL";
    if (ver == VER_NDX_GLOBAL)
      return "VER_NDX_GLOBAL";
    return ("version '" + config->versionDefinitions[ver].name + "'").str();
  };

  // Assign the version.
  for (Symbol *sym : syms) {
    // Skip symbols containing version info because symbol versions
    // specified by symbol names take precedence over version scripts.
    // See parseSymbolVersion().
    if (sym->getName().contains('@'))
      continue;

    // If the version has not been assigned, verdefIndex is -1. Use an arbitrary
    // number (0) to indicate the version has been assigned.
    if (sym->verdefIndex == UINT32_C(-1)) {
      sym->verdefIndex = 0;
      sym->versionId = versionId;
    }
    if (sym->versionId == versionId)
      continue;

    warn("attempt to reassign symbol '" + ver.name + "' of " +
         getName(sym->versionId) + " to " + getName(versionId));
  }
}

void SymbolTable::assignWildcardVersion(SymbolVersion ver, uint16_t versionId) {
  // Exact matching takes precendence over fuzzy matching,
  // so we set a version to a symbol only if no version has been assigned
  // to the symbol. This behavior is compatible with GNU.
<<<<<<< HEAD
  for (Symbol *sym : findAllByVersion(ver))
    if (sym->verdefIndex == UINT32_C(-1)) {
      sym->verdefIndex = 0;
      sym->versionId = versionId;
    }
}

// This function processes version scripts by updating the versionId
// member of symbols.
// If there's only one anonymous version definition in a version
// script file, the script does not actually define any symbol version,
// but just specifies symbols visibilities.
void SymbolTable::scanVersionScript() {
=======
  for (SymbolBody *B : findAllByVersion(Ver))
    if (B->symbol()->VersionId == Config->DefaultSymbolVersion)
      B->symbol()->VersionId = VersionId;
}

// This function processes version scripts by updating VersionId
// member of symbols.
template <class ELFT> void SymbolTable<ELFT>::scanVersionScript() {
  // Handle edge cases first.
  handleAnonymousVersion();

  // Now we have version definitions, so we need to set version ids to symbols.
  // Each version definition has a glob pattern, and all symbols that match
  // with the pattern get that version.

>>>>>>> origin/release/5.x
  // First, we assign versions to exact matching symbols,
  // i.e. version definitions not containing any glob meta-characters.
  for (VersionDefinition &v : config->versionDefinitions)
    for (SymbolVersion &pat : v.patterns)
      assignExactVersion(pat, v.id, v.name);

  // Next, assign versions to wildcards that are not "*". Note that because the
  // last match takes precedence over previous matches, we iterate over the
  // definitions in the reverse order.
  for (VersionDefinition &v : llvm::reverse(config->versionDefinitions))
    for (SymbolVersion &pat : v.patterns)
      if (pat.hasWildcard && pat.name != "*")
        assignWildcardVersion(pat, v.id);

  // Then, assign versions to "*". In GNU linkers they have lower priority than
  // other wildcards.
  for (VersionDefinition &v : config->versionDefinitions)
    for (SymbolVersion &pat : v.patterns)
      if (pat.hasWildcard && pat.name == "*")
        assignWildcardVersion(pat, v.id);

  // Symbol themselves might know their versions because symbols
  // can contain versions in the form of <name>@<version>.
  // Let them parse and update their names to exclude version suffix.
  for (Symbol *sym : symVector)
    sym->parseSymbolVersion();

  // isPreemptible is false at this point. To correctly compute the binding of a
  // Defined (which is used by includeInDynsym()), we need to know if it is
  // VER_NDX_LOCAL or not. Compute symbol versions before handling
  // --dynamic-list.
  handleDynamicList();
}
