# REQUIRES: mips
# Check MIPS N64 ABI GOT relocations

# RUN: llvm-mc -filetype=obj -triple=mips64-unknown-linux \
# RUN:         %p/Inputs/mips-pic.s -o %t.so.o
# RUN: llvm-mc -filetype=obj -triple=mips64-unknown-linux %s -o %t.exe.o
# RUN: echo "SECTIONS { . = 0x30000; .text : { *(.text) } }" > %t.script
# RUN: ld.lld %t.so.o -shared -soname=t.so -o %t.so
# RUN: ld.lld %t.exe.o --script %t.script %t.so -o %t.exe
# RUN: llvm-objdump -d -t --no-show-raw-insn %t.exe | FileCheck %s
# RUN: llvm-readelf -r -s -A %t.exe | FileCheck -check-prefix=GOT %s

# CHECK: {{[0-9a-f]+}}1c8  .text  00000000 foo

# CHECK:      __start:
<<<<<<< HEAD
# CHECK-NEXT:    {{.*}}  ld      $2, -32736($gp)
# CHECK-NEXT:    {{.*}}  daddiu  $2,  $2, 456
# CHECK-NEXT:    {{.*}}  addiu   $2,  $2, -32704
# CHECK-NEXT:    {{.*}}  addiu   $2,  $2, -32720
# CHECK-NEXT:    {{.*}}  addiu   $2,  $2, -32712

# GOT: There are no relocations in this file.

# GOT: Symbol table '.symtab'
# GOT: {{.*}}: [[FOO:[0-9a-f]+]]     {{.*}} foo
# GOT: {{.*}}: [[GP:[0-9a-f]+]]      {{.*}} _gp
# GOT: {{.*}}: [[BAR:[0-9a-f]+]]     {{.*}} bar

# GOT:      Primary GOT:
# GOT-NEXT:  Canonical gp value: [[GP]]
# GOT-EMPTY:
# GOT-NEXT:  Reserved entries:
# GOT-NEXT:  Address     Access          Initial Purpose
# GOT-NEXT:   {{.*}} -32752(gp) 0000000000000000 Lazy resolver
# GOT-NEXT:   {{.*}} -32744(gp) 8000000000000000 Module pointer (GNU extension)
# GOT-EMPTY:
# GOT-NEXT:  Local entries:
# GOT-NEXT:  Address     Access          Initial
# GOT-NEXT:   {{.*}} -32736(gp) 0000000000030000
# GOT-NEXT:   {{.*}} -32728(gp) 0000000000040000
# GOT-NEXT:   {{.*}} -32720(gp) [[BAR]]
# GOT-NEXT:   {{.*}} -32712(gp) [[FOO]]
# GOT-EMPTY:
# GOT-NEXT:  Global entries:
# GOT-NEXT:  Address     Access          Initial         Sym.Val. Type Ndx Name
# GOT-NEXT:   {{.*}} -32704(gp) 0000000000000000 0000000000000000 FUNC UND foo1a
=======

# CHECK-NEXT:    20000:   df 82 80 20   ld      $2, -32736($gp)
# CHECK-NEXT:    20004:   64 42 00 18   daddiu  $2,  $2, 24
# CHECK-NEXT:    20008:   24 42 80 40   addiu   $2,  $2, -32704
# CHECK-NEXT:    2000c:   24 42 80 30   addiu   $2,  $2, -32720
# CHECK-NEXT:    20010:   24 42 80 38   addiu   $2,  $2, -32712

# CHECK: 0000000000020018   .text   00000000 foo
# CHECK: 0000000000020000   .text   00000000 __start
# CHECK: 0000000000020014   .text   00000000 bar

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
# GOT-NEXT:       Initial: 0x20000
# GOT-NEXT:     }
# GOT-NEXT:     Entry {
# GOT-NEXT:       Address:
# GOT-NEXT:       Access: -32728
# GOT-NEXT:       Initial: 0x30000
# GOT-NEXT:     }
# GOT-NEXT:     Entry {
# GOT-NEXT:       Address:
# GOT-NEXT:       Access: -32720
# GOT-NEXT:       Initial: 0x20014
# GOT-NEXT:     }
# GOT-NEXT:     Entry {
# GOT-NEXT:       Address:
# GOT-NEXT:       Access: -32712
# GOT-NEXT:       Initial: 0x20018
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
  .global  __start, bar
__start:
  ld      $v0,%got_page(foo)($gp)             # R_MIPS_GOT_PAGE
  daddiu  $v0,$v0,%got_ofst(foo)              # R_MIPS_GOT_OFST
  addiu   $v0,$v0,%got_disp(foo1a)            # R_MIPS_GOT_DISP
  addiu   $v0,$v0,%got_disp(bar)              # R_MIPS_GOT_DISP
  addiu   $v0,$v0,%got_disp(foo)              # R_MIPS_GOT_DISP

bar:
  nop
foo:
  nop
