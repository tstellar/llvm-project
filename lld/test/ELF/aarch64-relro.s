# REQUIRES: aarch64
# RUN: llvm-mc -filetype=obj -triple=aarch64-unknown-freebsd %s -o %t
# RUN: ld.lld %t -o %t2
<<<<<<< HEAD
# RUN: llvm-readobj -l %t2 | FileCheck %s

# CHECK:      Type: PT_GNU_RELRO
# CHECK-NEXT: Offset:
# CHECK-NEXT: VirtualAddress: 0x220190
# CHECK-NEXT: PhysicalAddress:
# CHECK-NEXT: FileSize:
# CHECK-NEXT: MemSize: 3696
=======
# RUN: llvm-readobj -program-headers %t2 | FileCheck %s

# CHECK:      Type: PT_GNU_RELRO
# CHECK-NEXT: Offset:
# CHECK-NEXT: VirtualAddress:
# CHECK-NEXT: PhysicalAddress:
# CHECK-NEXT: FileSize:
# CHECK-NEXT: MemSize: 4096
>>>>>>> origin/release/4.x

.section .data.rel.ro,"aw",%progbits
.byte 1
