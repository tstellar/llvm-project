# REQUIRES: mips
# Check number of redundant entries in the local part of MIPS GOT.

# RUN: llvm-mc -filetype=obj -triple=mips-unknown-linux %s -o %t.o
# RUN: ld.lld %t.o -shared -o %t.so
# RUN: llvm-readelf -s -A %t.so | FileCheck %s

# CHECK: Symbol table '.symtab'
# CHECK: {{.*}}: [[VAL:[0-9a-f]+]]  {{.*}}  glb1

<<<<<<< HEAD
# CHECK:      Primary GOT:
# CHECK:       Local entries:
# CHECK-NEXT:    Address     Access  Initial
# CHECK-NEXT:     {{.*}} -32744(gp) 00020000
# CHECK-NEXT:     {{.*}} -32740(gp) 00030000
# CHECK-NEXT:     {{.*}} -32736(gp) 00040000
# CHECK-NEXT:     {{.*}} -32732(gp) [[VAL]]
# CHECK-NOT:      {{.*}} -32728(gp)
=======
# CHECK:      Local entries [
# CHECK-NEXT:   Entry {
# CHECK-NEXT:     Address:
# CHECK-NEXT:     Access: -32744
# CHECK-NEXT:     Initial: 0x20000
#                          ^-- loc1
# CHECK-NEXT:   }
# CHECK-NEXT:   Entry {
# CHECK-NEXT:     Address:
# CHECK-NEXT:     Access: -32740
# CHECK-NEXT:     Initial: 0x30000
#                          ^-- loc2, loc3, loc4
# CHECK-NEXT:   }
# CHECK-NEXT:   Entry {
# CHECK-NEXT:     Address:
# CHECK-NEXT:     Access: -32736
# CHECK-NEXT:     Initial: 0x40000
#                          ^-- redundant
# CHECK-NEXT:   }
# CHECK-NEXT:   Entry {
# CHECK-NEXT:     Address:
# CHECK-NEXT:     Access: -32732
# CHECK-NEXT:     Initial: 0x30008
#                          ^-- glb1
# CHECK-NEXT:   }
# CHECK-NEXT: ]
>>>>>>> origin/release/4.x

  .text
  .globl  foo
foo:
  lw      $t0, %got(loc1)($gp)
  addi    $t0, $t0, %lo(loc1)
  lw      $t0, %got(loc2)($gp)
  addi    $t0, $t0, %lo(loc2)
  lw      $t0, %got(loc3)($gp)
  addi    $t0, $t0, %lo(loc3)
  lw      $t0, %got(loc4)($gp)
  addi    $t0, $t0, %lo(loc4)
  lw      $t0, %got(glb1)($gp)
  lw      $t0, %got(glb1)($gp)

  .section .data.1,"aw",%progbits
loc1:
  .space 0x10000
loc2:
  .word 0
loc3:
  .word 0
  .global glb1
  .hidden glb1
glb1:
  .word 0

  .section .data.2,"aw",%progbits
loc4:
  .word 0
