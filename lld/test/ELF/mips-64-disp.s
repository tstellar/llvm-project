# REQUIRES: mips
# Check R_MIPS_GOT_DISP relocations against various kind of symbols.

# RUN: llvm-mc -filetype=obj -triple=mips64-unknown-linux \
# RUN:         %p/Inputs/mips-pic.s -o %t.so.o
# RUN: llvm-mc -filetype=obj -triple=mips64-unknown-linux %s -o %t.exe.o
# RUN: ld.lld %t.so.o -shared -soname=t.so -o %t.so
# RUN: ld.lld %t.exe.o %t.so -o %t.exe
# RUN: llvm-objdump -d -t --no-show-raw-insn %t.exe | FileCheck %s
# RUN: llvm-readelf -r -s -A %t.exe | FileCheck -check-prefix=GOT %s

# CHECK:      __start:
# CHECK-NEXT:    {{.*}}:  addiu   $2, $2, -32704
# CHECK-EMPTY:
# CHECK-NEXT: b4:
# CHECK-NEXT:    {{.*}}:  addiu   $2, $2, -32736
# CHECK-EMPTY:
# CHECK-NEXT: b8:
# CHECK-NEXT:    {{.*}}:  addiu   $2, $2, -32728
# CHECK-EMPTY:
# CHECK-NEXT: b12:
# CHECK-NEXT:    {{.*}}:  addiu   $2, $2, -32720
# CHECK-NEXT:    {{.*}}:  addiu   $2, $2, -32712

<<<<<<< HEAD
# GOT: Symbol table '.symtab'
# GOT: {{.*}} [[B12:[0-9a-f]+]] {{.*}} b12
# GOT: {{.*}} [[B04:[0-9a-f]+]] {{.*}} b4
# GOT: {{.*}} [[B08:[0-9a-f]+]] {{.*}} b8
# GOT: {{.*}} [[FOO:[0-9a-f]+]] {{.*}} foo

# GOT:      Primary GOT:
# GOT-NEXT:  Canonical gp value:
# GOT-EMPTY:
# GOT-NEXT:  Reserved entries:
# GOT-NEXT:  Address     Access          Initial Purpose
# GOT-NEXT:   {{.*}} -32752(gp) 0000000000000000 Lazy resolver
# GOT-NEXT:   {{.*}} -32744(gp) 8000000000000000 Module pointer (GNU extension)
# GOT-EMPTY:
# GOT-NEXT:  Local entries:
# GOT-NEXT:  Address     Access          Initial
# GOT-NEXT:   {{.*}} -32736(gp) [[FOO]]
# GOT-NEXT:   {{.*}} -32728(gp) [[B04]]
# GOT-NEXT:   {{.*}} -32720(gp) [[B08]]
# GOT-NEXT:   {{.*}} -32712(gp) [[B12]]
# GOT-EMPTY:
# GOT-NEXT:  Global entries:
# GOT-NEXT:  Address     Access          Initial         Sym.Val. Type Ndx Name
# GOT-NEXT:   {{.*}} -32704(gp) 0000000000000000 0000000000000000 FUNC UND foo1a
=======
# CHECK: 0000000000020014     .text   00000000 foo
# CHECK: 0000000000020000     .text   00000000 __start
# CHECK: 0000000000000000 g F *UND*   00000000 foo1a

# GOT:      Relocations [
# GOT-NEXT: ]
# GOT-NEXT: Primary GOT {
# GOT-NEXT:   Canonical gp value:
# GOT-NEXT:   Reserved entries [
# GOT-NEXT:     Entry {
# GOT-NEXT:       Address:
# GOT-NEXT:       Access: -32752
# GOT-NEXT:       Initial: 0x0
# GOT-NEXT:       Purpose: Lazy resolver
# GOT-NEXT:     }
# GOT-NEXT:     Entry {
# GOT-NEXT:       Address:
# GOT-NEXT:       Access: -32744
# GOT-NEXT:       Initial: 0x8000000000000000
# GOT-NEXT:       Purpose: Module pointer (GNU extension)
# GOT-NEXT:     }
# GOT-NEXT:   ]
# GOT-NEXT:   Local entries [
# GOT-NEXT:     Entry {
# GOT-NEXT:       Address:
# GOT-NEXT:       Access: -32736
# GOT-NEXT:       Initial: 0x20014
# GOT-NEXT:     }
# GOT-NEXT:     Entry {
# GOT-NEXT:       Address:
# GOT-NEXT:       Access: -32728
# GOT-NEXT:       Initial: 0x20004
# GOT-NEXT:     }
# GOT-NEXT:     Entry {
# GOT-NEXT:       Address:
# GOT-NEXT:       Access: -32720
# GOT-NEXT:       Initial: 0x20008
# GOT-NEXT:     }
# GOT-NEXT:     Entry {
# GOT-NEXT:       Address:
# GOT-NEXT:       Access: -32712
# GOT-NEXT:       Initial: 0x2000C
# GOT-NEXT:     }
# GOT-NEXT:   ]
# GOT-NEXT:   Global entries [
# GOT-NEXT:     Entry {
# GOT-NEXT:       Address:
# GOT-NEXT:       Access: -32704
# GOT-NEXT:       Initial: 0x0
# GOT-NEXT:       Value: 0x0
# GOT-NEXT:       Type: Function
# GOT-NEXT:       Section: Undefined
# GOT-NEXT:       Name: foo1a
# GOT-NEXT:     }
# GOT-NEXT:   ]
# GOT-NEXT:   Number of TLS and multi-GOT entries: 0
# GOT-NEXT: }
>>>>>>> origin/release/4.x

  .text
  .global  __start
__start:
  addiu   $v0,$v0,%got_disp(foo1a)            # R_MIPS_GOT_DISP
b4:
  addiu   $v0,$v0,%got_disp(foo)              # R_MIPS_GOT_DISP
b8:
  addiu   $v0,$v0,%got_disp(.text+4)          # R_MIPS_GOT_DISP
b12:
  addiu   $v0,$v0,%got_disp(.text+8)          # R_MIPS_GOT_DISP
  addiu   $v0,$v0,%got_disp(.text+12)         # R_MIPS_GOT_DISP

foo:
  nop
