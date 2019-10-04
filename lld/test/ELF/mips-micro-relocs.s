# REQUIRES: mips
# Check handling of microMIPS relocations.

# RUN: echo "SECTIONS { \
# RUN:         . = 0x20000;  .text ALIGN(0x100) : { *(.text) } \
# RUN:         . = 0x20300;  .plt  : { *(.plt) } \
# RUN:         . = 0x30000;  .data : { *(.data) } \
# RUN:       }" > %t.script

# RUN: llvm-mc -filetype=obj -triple=mips-unknown-linux \
# RUN:         -mattr=micromips %S/Inputs/mips-micro.s -o %t1eb.o
# RUN: llvm-mc -filetype=obj -triple=mips-unknown-linux \
# RUN:         -mattr=micromips %s -o %t2eb.o
# RUN: ld.lld -o %teb.exe -script %t.script %t1eb.o %t2eb.o
# RUN: llvm-objdump -d -t -s -mattr=micromips --no-show-raw-insn %teb.exe \
# RUN:   | FileCheck --check-prefixes=ASM,EB %s
# RUN: llvm-readelf -h %teb.exe | FileCheck --check-prefix=ELF %s

# RUN: llvm-mc -filetype=obj -triple=mipsel-unknown-linux \
# RUN:         -mattr=micromips %S/Inputs/mips-micro.s -o %t1el.o
# RUN: llvm-mc -filetype=obj -triple=mipsel-unknown-linux \
# RUN:         -mattr=micromips %s -o %t2el.o
<<<<<<< HEAD
# RUN: ld.lld -o %tel.exe -script %t.script %t1el.o %t2el.o
# RUN: llvm-objdump -d -t -s -mattr=micromips --no-show-raw-insn %tel.exe \
# RUN:   | FileCheck --check-prefixes=ASM,EL %s
# RUN: llvm-readelf -h %tel.exe | FileCheck --check-prefix=ELF %s

# ASM: 00038000         .got   00000000 .hidden _gp
# ASM: 00020100 g F     .text  00000000 0x80 foo
# ASM: 00020110         .text  00000000 0x80 __start

# EB:      Contents of section .data:
# EB-NEXT:  30000 fffe8111

# EB:      Contents of section .debug_info
# EB-NEXT:  0000 00020111

# EL:      Contents of section .data:
# EL-NEXT:  30000 1181feff

# EL:      Contents of section .debug_info
# EL-NEXT:  0000 11010200

# ASM:      __start:
# ASM-NEXT:      20110:  lui     $3, 1
# ASM-NEXT:              addiu   $3, $3, 32495
# ASM-NEXT:              lw      $3, -32744($gp)
# ASM-NEXT:              lw      $3, -32744($3)
# ASM-NEXT:              beqz16  $6, -32
# ASM-NEXT:              sll     $3, $fp, 0
# ASM-NEXT:              b16     -40
# ASM-NEXT:              nop
# ASM-NEXT:              b       -44

# ELF: Entry point address: 0x20111
=======
# RUN: ld.lld -o %tel.exe %t1el.o %t2el.o
# RUN: llvm-objdump -d -t -mattr=micromips %tel.exe \
# RUN:   | FileCheck --check-prefixes=EL,SYM %s

# EB:      __start:
# EB-NEXT:      20010:       41 a3 00 01     lui     $3, 1
# EB-NEXT:      20014:       30 63 7f df     addiu   $3, $3, 32735
# EB-NEXT:      20018:       fc 7c 80 18     lw      $3, -32744($gp)
# EB-NEXT:      2001c:       fc 63 80 18     lw      $3, -32744($3)
# EB-NEXT:      20020:       8f 70           beqz16  $6, -32
# EB-NEXT:      20022:       00 7e 00 00     sll     $3, $fp, 0
# EB-NEXT:      20026:       cf ec           b16     -40
# EB-NEXT:      20028:       00 00 00 00     nop
# EB-NEXT:      2002c:       94 00 ff e8     b       -44

# EL:      __start:
# EL-NEXT:      20010:       a3 41 01 00     lui     $3, 1
# EL-NEXT:      20014:       63 30 df 7f     addiu   $3, $3, 32735
# EL-NEXT:      20018:       7c fc 18 80     lw      $3, -32744($gp)
# EL-NEXT:      2001c:       63 fc 18 80     lw      $3, -32744($3)
# EL-NEXT:      20020:       70 8f           beqz16  $6, -32
# EL-NEXT:      20022:       7e 00 00 00     sll     $3, $fp, 0
# EL-NEXT:      20026:       ec cf           b16     -40
# EL-NEXT:      20028:       00 00 00 00     nop
# EL-NEXT:      2002c:       00 94 e8 ff     b       -44

# SYM: 00037ff0         .got   00000000 .hidden _gp
# SYM: 00020000 g F     .text  00000000 0x80 foo
# SYM: 00020010         .text  00000000 0x80 __start
>>>>>>> release/8.x

  .text
  .set micromips
  .global __start
__start:
  lui     $3, %hi(_gp_disp)       # R_MICROMIPS_HI16
  addiu   $3, $3, %lo(_gp_disp)   # R_MICROMIPS_LO16

  lw      $3, %call16(foo)($gp)   # R_MICROMIPS_CALL16
  lw      $3, %got(foo)($3)       # R_MICROMIPS_GOT16

  beqz16  $6, foo                 # R_MICROMIPS_PC7_S1
  b16     foo                     # R_MICROMIPS_PC10_S1
  b       foo                     # R_MICROMIPS_PC16_S1

  .data
  .gpword __start                 # R_MIPS_GPREL32

  .section .debug_info
  .word __start                   # R_MIPS_32
