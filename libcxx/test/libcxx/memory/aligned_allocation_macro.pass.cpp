//===----------------------------------------------------------------------===//
//
<<<<<<< HEAD
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
=======
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
>>>>>>> release/7.x
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14

<<<<<<< HEAD
// AppleClang <= 10 enables aligned allocation regardless of the deployment
// target, so this test would fail.
// UNSUPPORTED: apple-clang-9, apple-clang-10

// XFAIL: availability=macosx10.13
// XFAIL: availability=macosx10.12
// XFAIL: availability=macosx10.11
// XFAIL: availability=macosx10.10
// XFAIL: availability=macosx10.9
// XFAIL: availability=macosx10.8
// XFAIL: availability=macosx10.7

#include <new>

#include "test_macros.h"

=======
// aligned allocation functions are not provided prior to macosx10.13
// XFAIL: macosx10.12
// XFAIL: macosx10.11
// XFAIL: macosx10.10
// XFAIL: macosx10.9
// XFAIL: macosx10.8
// XFAIL: macosx10.7

#include <new>

>>>>>>> release/7.x

#ifdef _LIBCPP_HAS_NO_ALIGNED_ALLOCATION
#   error "libc++ should have aligned allocation in C++17 and up when targeting a platform that supports it"
#endif

<<<<<<< HEAD
int main(int, char**) { 
  return 0;
}
=======
int main() { }
>>>>>>> release/7.x
