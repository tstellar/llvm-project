// REQUIRES: ppc

// RUN: llvm-mc -filetype=obj -triple=powerpc64le-unknown-linux %s -o %t.o
// RUN: llvm-mc -filetype=obj -triple=powerpc64le-unknown-linux %p/Inputs/shared-ppc64.s -o %t2.o
// RUN: llvm-mc -filetype=obj -triple=powerpc64le-unknown-linux %p/Inputs/ppc64-func.s -o %t3.o
// RUN: ld.lld -shared -soname=t2.so %t2.o -o %t2.so
// RUN: ld.lld %t.o %t2.so %t3.o -o %t
// RUN: llvm-objdump -d --no-show-raw-insn %t | FileCheck %s

// RUN: llvm-mc -filetype=obj -triple=powerpc64-unknown-linux %s -o %t.o
// RUN: llvm-mc -filetype=obj -triple=powerpc64-unknown-linux %p/Inputs/shared-ppc64.s -o %t2.o
// RUN: llvm-mc -filetype=obj -triple=powerpc64-unknown-linux %p/Inputs/ppc64-func.s -o %t3.o
// RUN: ld.lld -shared -soname=t2.so %t2.o -o %t2.so
// RUN: ld.lld %t.o %t2.so %t3.o -o %t
// RUN: llvm-objdump -d --no-show-raw-insn %t | FileCheck %s

    .text
    .abiversion 2
.global bar_local
bar_local:
  li 3, 2
  blr

# Calling external function foo in a shared object needs a nop.
# Calling local function bar_local doe not need a nop.
.global _start
_start:
  bl foo
  nop
  bl bar_local
// CHECK-LABEL: _start:
<<<<<<< HEAD
// CHECK-NEXT:  100102c8:       bl .+64
// CHECK-NEXT:  100102cc:       ld 2, 24(1)
// CHECK-NEXT:  100102d0:       bl .-16
=======
// CHECK-NEXT:  10010008:       bl .+64
// CHECK-NEXT:  1001000c:       ld 2, 24(1)
// CHECK-NEXT:  10010010:       bl .-16
>>>>>>> release/8.x
// CHECK-EMPTY:

# Calling a function in another object file which will have same
# TOC base does not need a nop. If nop present, do not rewrite to
# a toc restore
.global diff_object
_diff_object:
  bl foo_not_shared
  bl foo_not_shared
  nop
// CHECK-LABEL: _diff_object:
<<<<<<< HEAD
// CHECK-NEXT:  100102d4:       bl .+28
// CHECK-NEXT:  100102d8:       bl .+24
// CHECK-NEXT:  100102dc:       nop
=======
// CHECK-NEXT:  10010014:       bl .+28
// CHECK-NEXT:  10010018:       bl .+24
// CHECK-NEXT:  1001001c:       nop
>>>>>>> release/8.x

# Branching to a local function does not need a nop
.global noretbranch
noretbranch:
  b bar_local
// CHECK-LABEL: noretbranch:
<<<<<<< HEAD
// CHECK:       100102e0:        b .+67108832
=======
// CHECK:       10010020:        b .+67108832
>>>>>>> release/8.x
// CHECK-EMPTY:

// This should come last to check the end-of-buffer condition.
.global last
last:
  bl foo
  nop
// CHECK-LABEL: last:
<<<<<<< HEAD
// CHECK-NEXT:  100102e4:       bl .+36
// CHECK-NEXT:  100102e8:       ld 2, 24(1)
=======
// CHECK-NEXT:  10010024:       bl .+36
// CHECK-NEXT:  10010028:       ld 2, 24(1)
>>>>>>> release/8.x
