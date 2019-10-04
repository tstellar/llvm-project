// REQUIRES: x86

// RUN: llvm-mc -filetype=obj -triple=x86_64-unknown-linux %s -o %t
// RUN: ld.lld %t -o %t.exe
<<<<<<< HEAD
// RUN: llvm-readobj --sections %t.exe | FileCheck %s
=======
// RUN: llvm-readobj -sections %t.exe | FileCheck %s
>>>>>>> origin/release/4.x

// CHECK:      Name: .init_array
// CHECK-NEXT: Type: SHT_INIT_ARRAY
// CHECK:      Name: .fini_array
// CHECK-NEXT: Type: SHT_FINI_ARRAY

.globl _start
_start:
  nop

.section .init_array.100, "aw", @progbits
  .byte 0
.section .fini_array.100, "aw", @progbits
  .byte 0
