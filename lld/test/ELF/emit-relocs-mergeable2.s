# REQUIRES: x86
<<<<<<< HEAD

## By default local symbols are discarded from SHF_MERGE sections.
## With --emit-relocs we should keep them.

# RUN: llvm-mc -filetype=obj -triple=x86_64-unknown-linux %s -o %t.o
# RUN: ld.lld --emit-relocs %t.o -o %t.exe
# RUN: llvm-readobj --relocations %t.exe | FileCheck %s

# CHECK: R_X86_64_32S .Lfoo 0x8
=======
# RUN: llvm-mc -filetype=obj -triple=x86_64-unknown-linux %s -o %t.o
# RUN: ld.lld --emit-relocs %t.o -o %t.exe
# RUN: llvm-readelf --relocations %t.exe | FileCheck %s

# CHECK: 0000000000201004  000000010000000b R_X86_64_32S 0000000000200120 .Lfoo + 8
>>>>>>> release/8.x

.globl  _start
_start:
  movq .Lfoo+8, %rax
.section .rodata.cst16,"aM",@progbits,16
.Lfoo:
  .quad 0
  .quad 0
