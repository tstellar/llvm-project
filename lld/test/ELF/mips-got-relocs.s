# REQUIRES: mips
# Check R_MIPS_GOT16 relocation calculation.

# RUN: echo "SECTIONS { \
# RUN:         . = 0x20000; .text : { *(.text) } \
# RUN:         . = 0x30000; .got  : { *(.got)  } \
# RUN:       }" > %t.script

# RUN: llvm-mc -filetype=obj -triple=mips-unknown-linux %s -o %t-be.o
# RUN: ld.lld %t-be.o --script %t.script -o %t-be.exe
# RUN: llvm-readobj --sections -r --symbols -A %t-be.exe \
# RUN:   | FileCheck -check-prefix=ELF %s
# RUN: llvm-objdump -d %t-be.exe | FileCheck -check-prefix=DIS %s

# RUN: llvm-mc -filetype=obj -triple=mipsel-unknown-linux %s -o %t-el.o
# RUN: ld.lld %t-el.o --script %t.script -o %t-el.exe
# RUN: llvm-readobj --sections -r --symbols -A %t-el.exe \
# RUN:   | FileCheck -check-prefix=ELF %s
# RUN: llvm-objdump -d %t-el.exe | FileCheck -check-prefix=DIS %s

# RUN: ld.lld -shared %t-be.o --script %t.script -o %t-be.so
# RUN: llvm-readobj --sections -r --symbols -A %t-be.so \
# RUN:   | FileCheck -check-prefix=ELF %s
# RUN: llvm-objdump -d %t-be.so | FileCheck -check-prefix=DIS %s

# RUN: ld.lld -shared %t-el.o --script %t.script -o %t-el.so
# RUN: llvm-readobj --sections -r --symbols -A %t-el.so \
# RUN:   | FileCheck -check-prefix=ELF %s
# RUN: llvm-objdump -d %t-el.so | FileCheck -check-prefix=DIS %s

  .text
  .globl  __start
__start:
  lui $2, %got(v1)

  .data
  .globl v1
v1:
  .word 0

<<<<<<< HEAD
# ELF:      Section {
# ELF:        Name: .got
# ELF:        Flags [
# ELF-NEXT:     SHF_ALLOC
# ELF-NEXT:     SHF_MIPS_GPREL
# ELF-NEXT:     SHF_WRITE
# ELF-NEXT:   ]
#
# ELF:      Relocations [
# ELF-NEXT: ]
#
# ELF:      Symbol {
# ELF:        Name: v1
# ELF-NEXT:   Value: 0x[[V1:[0-9A-F]+]]
#
# ELF:      {{.*}} GOT {
# ELF-NEXT:   Canonical gp value: 0x37FF0
#
# ELF:        Entry {
# ELF:          Address: 0x30008
# ELF-NEXT:     Access: -32744
# ELF-NEXT:     Initial: 0x[[V1]]

# "v1 GOT entry address" - _gp
# 0x30008 - 0x37FF0 = -0x7fe8 == 0x8018 == 32792
# DIS:  lui $2, 32792
=======
# EXE_SYM: Sections:
# EXE_SYM: .got 0000000c 0000000000030010 DATA
# EXE_SYM: SYMBOL TABLE:
# EXE_SYM: 00038000         *ABS*    00000000 .hidden _gp
#          ^-- .got + GP offset (0x7ff0)
# EXE_SYM: 00030000 g       .data		 00000004 v1


# EXE_GOT_BE: Contents of section .got:
# EXE_GOT_BE:  30010 00000000 80000000 00030000
#                    ^        ^        ^-- v1 (0x30000)
#                    |        +-- Module pointer (0x80000000)
#                    +-- Lazy resolver (0x0)

# EXE_GOT_EL: Contents of section .got:
# EXE_GOT_EL:  30010 00000000 00000080 00000300
#                    ^        ^        ^-- v1 (0x30000)
#                    |        +-- Module pointer (0x80000000)
#                    +-- Lazy resolver (0x0)

# v1GotAddr (0x3000c) - _gp (0x37ff4) = -0x7fe8 => 0x8018 = 32792
# EXE_DIS_BE:  20000:  3c 02 80 18  lui $2, 32792
# EXE_DIS_EL:  20000:  18 80 02 3c  lui $2, 32792

# DSO_SYM: Sections:
# DSO_SYM: .got 0000000c 0000000000020010 DATA
# DSO_SYM: SYMBOL TABLE:
# DSO_SYM: 00028000         *ABS*    00000000 .hidden _gp
#          ^-- .got + GP offset (0x7ff0)
# DSO_SYM: 00020000 g       .data		 00000004 v1

# DSO_GOT_BE: Contents of section .got:
# DSO_GOT_BE:  20010 00000000 80000000 00020000
#                    ^        ^        ^-- v1 (0x20000)
#                    |        +-- Module pointer (0x80000000)
#                    +-- Lazy resolver (0x0)

# DSO_GOT_EL: Contents of section .got:
# DSO_GOT_EL:  20010 00000000 00000080 00000200
#                    ^        ^        ^-- v1 (0x20000)
#                    |        +-- Module pointer (0x80000000)
#                    +-- Lazy resolver (0x0)

# v1GotAddr (0x2000c) - _gp (0x27ff4) = -0x7fe8 => 0x8018 = 32792
# DSO_DIS_BE:  10000:  3c 02 80 18  lui $2, 32792
# DSO_DIS_EL:  10000:  18 80 02 3c  lui $2, 32792

# NORELOC:      Relocations [
# NORELOC-NEXT: ]

# SHFLAGS:      Name: .got
# SHFLAGS-NEXT: Type: SHT_PROGBITS
# SHFLAGS-NEXT: Flags [ (0x10000003)
#                        ^-- SHF_MIPS_GPREL | SHF_ALLOC | SHF_WRITE
>>>>>>> origin/release/4.x
