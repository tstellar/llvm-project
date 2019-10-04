//===-------------------------- cxa_demangle.cpp --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// FIXME: (possibly) incomplete list of features that clang mangles that this
// file does not yet support:
//   - C++ modules TS

#include "demangle/ItaniumDemangle.h"
#include "__cxxabi_config.h"
#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <numeric>
#include <utility>

using namespace itanium_demangle;

constexpr const char *itanium_demangle::FloatData<float>::spec;
constexpr const char *itanium_demangle::FloatData<double>::spec;
constexpr const char *itanium_demangle::FloatData<long double>::spec;

// <discriminator> := _ <non-negative number>      # when number < 10
//                 := __ <non-negative number> _   # when number >= 10
//  extension      := decimal-digit+               # at the end of string
const char *itanium_demangle::parse_discriminator(const char *first,
                                                  const char *last) {
  // parse but ignore discriminator
  if (first != last) {
    if (*first == '_') {
      const char *t1 = first + 1;
      if (t1 != last) {
        if (std::isdigit(*t1))
          first = t1 + 1;
        else if (*t1 == '_') {
          for (++t1; t1 != last && std::isdigit(*t1); ++t1)
            ;
          if (t1 != last && *t1 == '_')
            first = t1 + 1;
        }
      }
    } else if (std::isdigit(*first)) {
      const char *t1 = first + 1;
      for (; t1 != last && std::isdigit(*t1); ++t1)
        ;
      if (t1 == last)
        first = last;
    }
  }
  return first;
}

#ifndef NDEBUG
namespace {
struct DumpVisitor {
  unsigned Depth = 0;
  bool PendingNewline = false;

  template<typename NodeT> static constexpr bool wantsNewline(const NodeT *) {
    return true;
  }
  static bool wantsNewline(NodeArray A) { return !A.empty(); }
  static constexpr bool wantsNewline(...) { return false; }

  template<typename ...Ts> static bool anyWantNewline(Ts ...Vs) {
    for (bool B : {wantsNewline(Vs)...})
      if (B)
        return true;
    return false;
  }

  void printStr(const char *S) { fprintf(stderr, "%s", S); }
  void print(StringView SV) {
    fprintf(stderr, "\"%.*s\"", (int)SV.size(), SV.begin());
  }
  void print(const Node *N) {
    if (N)
      N->visit(std::ref(*this));
    else
      printStr("<null>");
  }
  void print(NodeOrString NS) {
    if (NS.isNode())
      print(NS.asNode());
    else if (NS.isString())
      print(NS.asString());
    else
      printStr("NodeOrString()");
  }
  void print(NodeArray A) {
    ++Depth;
    printStr("{");
    bool First = true;
    for (const Node *N : A) {
      if (First)
        print(N);
      else
        printWithComma(N);
      First = false;
    }
    printStr("}");
    --Depth;
  }

  // Overload used when T is exactly 'bool', not merely convertible to 'bool'.
  void print(bool B) { printStr(B ? "true" : "false"); }

  template <class T>
  typename std::enable_if<std::is_unsigned<T>::value>::type print(T N) {
    fprintf(stderr, "%llu", (unsigned long long)N);
  }

  template <class T>
  typename std::enable_if<std::is_signed<T>::value>::type print(T N) {
    fprintf(stderr, "%lld", (long long)N);
  }

  void print(ReferenceKind RK) {
    switch (RK) {
    case ReferenceKind::LValue:
      return printStr("ReferenceKind::LValue");
    case ReferenceKind::RValue:
      return printStr("ReferenceKind::RValue");
    }
  }
  void print(FunctionRefQual RQ) {
    switch (RQ) {
    case FunctionRefQual::FrefQualNone:
      return printStr("FunctionRefQual::FrefQualNone");
    case FunctionRefQual::FrefQualLValue:
      return printStr("FunctionRefQual::FrefQualLValue");
    case FunctionRefQual::FrefQualRValue:
      return printStr("FunctionRefQual::FrefQualRValue");
    }
  }
  void print(Qualifiers Qs) {
    if (!Qs) return printStr("QualNone");
    struct QualName { Qualifiers Q; const char *Name; } Names[] = {
      {QualConst, "QualConst"},
      {QualVolatile, "QualVolatile"},
      {QualRestrict, "QualRestrict"},
    };
    for (QualName Name : Names) {
      if (Qs & Name.Q) {
        printStr(Name.Name);
        Qs = Qualifiers(Qs & ~Name.Q);
        if (Qs) printStr(" | ");
      }
    }
  }
  void print(SpecialSubKind SSK) {
    switch (SSK) {
    case SpecialSubKind::allocator:
      return printStr("SpecialSubKind::allocator");
    case SpecialSubKind::basic_string:
      return printStr("SpecialSubKind::basic_string");
    case SpecialSubKind::string:
      return printStr("SpecialSubKind::string");
    case SpecialSubKind::istream:
      return printStr("SpecialSubKind::istream");
    case SpecialSubKind::ostream:
      return printStr("SpecialSubKind::ostream");
    case SpecialSubKind::iostream:
      return printStr("SpecialSubKind::iostream");
    }
  }
  void print(TemplateParamKind TPK) {
    switch (TPK) {
    case TemplateParamKind::Type:
      return printStr("TemplateParamKind::Type");
    case TemplateParamKind::NonType:
      return printStr("TemplateParamKind::NonType");
    case TemplateParamKind::Template:
      return printStr("TemplateParamKind::Template");
    }
  }

  void newLine() {
    printStr("\n");
    for (unsigned I = 0; I != Depth; ++I)
      printStr(" ");
    PendingNewline = false;
  }

  template<typename T> void printWithPendingNewline(T V) {
    print(V);
    if (wantsNewline(V))
      PendingNewline = true;
  }

  template<typename T> void printWithComma(T V) {
    if (PendingNewline || wantsNewline(V)) {
      printStr(",");
      newLine();
    } else {
      printStr(", ");
    }

    printWithPendingNewline(V);
  }

  struct CtorArgPrinter {
    DumpVisitor &Visitor;

    template<typename T, typename ...Rest> void operator()(T V, Rest ...Vs) {
      if (Visitor.anyWantNewline(V, Vs...))
        Visitor.newLine();
      Visitor.printWithPendingNewline(V);
      int PrintInOrder[] = { (Visitor.printWithComma(Vs), 0)..., 0 };
      (void)PrintInOrder;
    }
  };

  template<typename NodeT> void operator()(const NodeT *Node) {
    Depth += 2;
    fprintf(stderr, "%s(", itanium_demangle::NodeKind<NodeT>::name());
    Node->match(CtorArgPrinter{*this});
    fprintf(stderr, ")");
    Depth -= 2;
  }

  void operator()(const ForwardTemplateReference *Node) {
    Depth += 2;
    fprintf(stderr, "ForwardTemplateReference(");
    if (Node->Ref && !Node->Printing) {
      Node->Printing = true;
      CtorArgPrinter{*this}(Node->Ref);
      Node->Printing = false;
    } else {
      CtorArgPrinter{*this}(Node->Index);
    }
    fprintf(stderr, ")");
    Depth -= 2;
  }
};
}

void itanium_demangle::Node::dump() const {
  DumpVisitor V;
  visit(std::ref(V));
  V.newLine();
}
#endif

namespace {
class BumpPointerAllocator {
  struct BlockMeta {
    BlockMeta* Next;
    size_t Current;
  };

  static constexpr size_t AllocSize = 4096;
  static constexpr size_t UsableAllocSize = AllocSize - sizeof(BlockMeta);

  alignas(long double) char InitialBuffer[AllocSize];
  BlockMeta* BlockList = nullptr;

  void grow() {
    char* NewMeta = static_cast<char *>(std::malloc(AllocSize));
    if (NewMeta == nullptr)
      std::terminate();
    BlockList = new (NewMeta) BlockMeta{BlockList, 0};
  }

  void* allocateMassive(size_t NBytes) {
    NBytes += sizeof(BlockMeta);
    BlockMeta* NewMeta = reinterpret_cast<BlockMeta*>(std::malloc(NBytes));
    if (NewMeta == nullptr)
      std::terminate();
    BlockList->Next = new (NewMeta) BlockMeta{BlockList->Next, 0};
    return static_cast<void*>(NewMeta + 1);
  }

public:
  BumpPointerAllocator()
      : BlockList(new (InitialBuffer) BlockMeta{nullptr, 0}) {}

  void* allocate(size_t N) {
    N = (N + 15u) & ~15u;
    if (N + BlockList->Current >= UsableAllocSize) {
      if (N > UsableAllocSize)
        return allocateMassive(N);
      grow();
    }
    BlockList->Current += N;
    return static_cast<void*>(reinterpret_cast<char*>(BlockList + 1) +
                              BlockList->Current - N);
  }

  void reset() {
    while (BlockList) {
      BlockMeta* Tmp = BlockList;
      BlockList = BlockList->Next;
      if (reinterpret_cast<char*>(Tmp) != InitialBuffer)
        std::free(Tmp);
    }
    BlockList = new (InitialBuffer) BlockMeta{nullptr, 0};
  }

  ~BumpPointerAllocator() { reset(); }
};

class DefaultAllocator {
  BumpPointerAllocator Alloc;

public:
  void reset() { Alloc.reset(); }

  template<typename T, typename ...Args> T *makeNode(Args &&...args) {
    return new (Alloc.allocate(sizeof(T)))
        T(std::forward<Args>(args)...);
  }

  void *allocateNodeArray(size_t sz) {
    return Alloc.allocate(sizeof(Node *) * sz);
  }
};
}  // unnamed namespace

//===----------------------------------------------------------------------===//
// Code beyond this point should not be synchronized with LLVM.
//===----------------------------------------------------------------------===//

using Demangler = itanium_demangle::ManglingParser<DefaultAllocator>;

namespace {
enum : int {
  demangle_invalid_args = -3,
  demangle_invalid_mangled_name = -2,
  demangle_memory_alloc_failure = -1,
  demangle_success = 0,
};
}

namespace __cxxabiv1 {
extern "C" _LIBCXXABI_FUNC_VIS char *
<<<<<<< HEAD
__cxa_demangle(const char *MangledName, char *Buf, size_t *N, int *Status) {
  if (MangledName == nullptr || (Buf != nullptr && N == nullptr)) {
    if (Status)
      *Status = demangle_invalid_args;
    return nullptr;
  }

  int InternalStatus = demangle_success;
  Demangler Parser(MangledName, MangledName + std::strlen(MangledName));
  OutputStream S;

  Node *AST = Parser.parse();

  if (AST == nullptr)
    InternalStatus = demangle_invalid_mangled_name;
  else if (!initializeOutputStream(Buf, N, S, 1024))
    InternalStatus = demangle_memory_alloc_failure;
  else {
    assert(Parser.ForwardTemplateRefs.empty());
    AST->print(S);
    S += '\0';
    if (N != nullptr)
      *N = S.getCurrentPosition();
    Buf = S.getBuffer();
  }

  if (Status)
    *Status = InternalStatus;
  return InternalStatus == demangle_success ? Buf : nullptr;
=======
__cxa_demangle(const char *mangled_name, char *buf, size_t *n, int *status) {
    if (mangled_name == nullptr || (buf != nullptr && n == nullptr))
    {
        if (status)
            *status = invalid_args;
        return nullptr;
    }

    size_t internal_size = buf != nullptr ? *n : 0;
    arena<bs> a;
    Db db(a);
    db.template_param.emplace_back(a);
    int internal_status = success;
    size_t len = std::strlen(mangled_name);
    demangle(mangled_name, mangled_name + len, db,
             internal_status);
    if (internal_status == success && db.fix_forward_references &&
           !db.template_param.empty() && !db.template_param.front().empty())
    {
        db.fix_forward_references = false;
        db.tag_templates = false;
        db.names.clear();
        db.subs.clear();
        demangle(mangled_name, mangled_name + len, db, internal_status);
        if (db.fix_forward_references)
            internal_status = invalid_mangled_name;
    }
    if (internal_status == success)
    {
        size_t sz = db.names.back().size() + 1;
        if (sz > internal_size)
        {
            char* newbuf = static_cast<char*>(std::realloc(buf, sz));
            if (newbuf == nullptr)
            {
                internal_status = memory_alloc_failure;
                buf = nullptr;
            }
            else
            {
                buf = newbuf;
                if (n != nullptr)
                    *n = sz;
            }
        }
        if (buf != nullptr)
        {
            db.names.back().first += db.names.back().second;
            std::memcpy(buf, db.names.back().first.data(), sz-1);
            buf[sz-1] = char(0);
        }
    }
    else
        buf = nullptr;
    if (status)
        *status = internal_status;
    return buf;
>>>>>>> origin/release/4.x
}
}  // __cxxabiv1
