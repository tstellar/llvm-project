//===--- AlignOf.h - Portable calculation of type alignment -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the AlignedCharArrayUnion class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_SUPPORT_ALIGNOF_H
#define LLVM_SUPPORT_ALIGNOF_H

#include "llvm/Support/Compiler.h"
#include <cstddef>

namespace llvm {

namespace detail {

template <typename T, typename... Ts> class AlignerImpl {
  T t;
  AlignerImpl<Ts...> rest;
  AlignerImpl() = delete;
};

template <typename T> class AlignerImpl<T> {
  T t;
  AlignerImpl() = delete;
};

template <typename T, typename... Ts> union SizerImpl {
  char arr[sizeof(T)];
  SizerImpl<Ts...> rest;
};

<<<<<<< HEAD
template <typename T> union SizerImpl<T> { char arr[sizeof(T)]; };
} // end namespace detail

/// A suitably aligned and sized character array member which can hold elements
/// of any type.
///
/// These types may be arrays, structs, or any other types. This exposes a
/// `buffer` member which can be used as suitable storage for a placement new of
/// any of these types.
template <typename T, typename... Ts> struct AlignedCharArrayUnion {
  alignas(::llvm::detail::AlignerImpl<T, Ts...>) char buffer[sizeof(
      llvm::detail::SizerImpl<T, Ts...>)];
};

=======

// The rest of these are provided with a __declspec(align(...)) and we simply
// can't pass them by-value as function arguments on MSVC.

#define LLVM_ALIGNEDCHARARRAY_TEMPLATE_ALIGNMENT(x) \
  template<std::size_t Size> \
  struct AlignedCharArray<x, Size> { \
    __declspec(align(x)) char buffer[Size]; \
  };

LLVM_ALIGNEDCHARARRAY_TEMPLATE_ALIGNMENT(16)
LLVM_ALIGNEDCHARARRAY_TEMPLATE_ALIGNMENT(32)
LLVM_ALIGNEDCHARARRAY_TEMPLATE_ALIGNMENT(64)
LLVM_ALIGNEDCHARARRAY_TEMPLATE_ALIGNMENT(128)

#undef LLVM_ALIGNEDCHARARRAY_TEMPLATE_ALIGNMENT

#endif // _MSC_VER

namespace detail {
template <typename T1,
          typename T2 = char, typename T3 = char, typename T4 = char,
          typename T5 = char, typename T6 = char, typename T7 = char,
          typename T8 = char, typename T9 = char, typename T10 = char>
class AlignerImpl {
  T1 t1; T2 t2; T3 t3; T4 t4; T5 t5; T6 t6; T7 t7; T8 t8; T9 t9; T10 t10;

  AlignerImpl() = delete;
};

template <typename T1,
          typename T2 = char, typename T3 = char, typename T4 = char,
          typename T5 = char, typename T6 = char, typename T7 = char,
          typename T8 = char, typename T9 = char, typename T10 = char>
union SizerImpl {
  char arr1[sizeof(T1)], arr2[sizeof(T2)], arr3[sizeof(T3)], arr4[sizeof(T4)],
       arr5[sizeof(T5)], arr6[sizeof(T6)], arr7[sizeof(T7)], arr8[sizeof(T8)],
       arr9[sizeof(T9)], arr10[sizeof(T10)];
};
} // end namespace detail

/// \brief This union template exposes a suitably aligned and sized character
/// array member which can hold elements of any of up to ten types.
///
/// These types may be arrays, structs, or any other types. The goal is to
/// expose a char array buffer member which can be used as suitable storage for
/// a placement new of any of these types. Support for more than ten types can
/// be added at the cost of more boilerplate.
template <typename T1,
          typename T2 = char, typename T3 = char, typename T4 = char,
          typename T5 = char, typename T6 = char, typename T7 = char,
          typename T8 = char, typename T9 = char, typename T10 = char>
struct AlignedCharArrayUnion : llvm::AlignedCharArray<
    alignof(llvm::detail::AlignerImpl<T1, T2, T3, T4, T5,
                                      T6, T7, T8, T9, T10>),
    sizeof(::llvm::detail::SizerImpl<T1, T2, T3, T4, T5,
                                     T6, T7, T8, T9, T10>)> {
};
>>>>>>> origin/release/4.x
} // end namespace llvm

#endif // LLVM_SUPPORT_ALIGNOF_H
