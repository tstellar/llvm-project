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
>>>>>>> origin/release/4.x
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

// <string>

// basic_string<charT,traits,Allocator>&
//   operator=(basic_string<charT,traits,Allocator>&& str);

#include <string>
#include <cassert>

#include "test_macros.h"

<<<<<<< HEAD
int main(int, char**)
=======
int main()
>>>>>>> origin/release/4.x
{
  // Test that assignment from {} and {ptr, len} are allowed and are not
  // ambiguous.
  {
    std::string s = "hello world";
    s = {};
    assert(s.empty());
  }
  {
    std::string s = "hello world";
    s = {"abc", 2};
    assert(s == "ab");
  }
<<<<<<< HEAD

  return 0;
=======
>>>>>>> origin/release/4.x
}
