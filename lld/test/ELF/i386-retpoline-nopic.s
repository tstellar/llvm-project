// REQUIRES: x86
// RUN: llvm-mc -filetype=obj -triple=i386-unknown-linux %s -o %t1.o
// RUN: llvm-mc -filetype=obj -triple=i386-unknown-linux %p/Inputs/shared.s -o %t2.o
<<<<<<< HEAD
// RUN: ld.lld -shared -soname=t2.so %t2.o -o %t2.so

// RUN: ld.lld %t1.o %t2.so -o %t.exe -z retpolineplt
// RUN: llvm-objdump -d -s --no-show-raw-insn %t.exe | FileCheck %s

// CHECK:      Contents of section .got.plt:
// CHECK-NEXT: 40224000 00000000 00000000 10124000
// CHECK-NEXT: 30124000

// CHECK:      Disassembly of section .plt:
// CHECK-EMPTY:
// CHECK-NEXT: .plt:
// CHECK-NEXT: 4011d0:       pushl   4207276
// CHECK-NEXT: 4011d6:       pushl   %eax
// CHECK-NEXT: 4011d7:       movl    4207280, %eax
// CHECK-NEXT: 4011dc:       calll   15 <.plt+0x20>
// CHECK-NEXT: 4011e1:       pause
// CHECK-NEXT: 4011e3:       lfence
// CHECK-NEXT: 4011e6:       jmp     -7 <.plt+0x11>
// CHECK-NEXT: 4011e8:       int3
// CHECK-NEXT: 4011e9:       int3
// CHECK-NEXT: 4011ea:       int3
// CHECK-NEXT: 4011eb:       int3
// CHECK-NEXT: 4011ec:       int3
// CHECK-NEXT: 4011ed:       int3
// CHECK-NEXT: 4011ee:       int3
// CHECK-NEXT: 4011ef:       int3
// CHECK-NEXT: 4011f0:       movl    %ecx, (%esp)
// CHECK-NEXT: 4011f3:       movl    4(%esp), %ecx
// CHECK-NEXT: 4011f7:       movl    %eax, 4(%esp)
// CHECK-NEXT: 4011fb:       movl    %ecx, %eax
// CHECK-NEXT: 4011fd:       popl    %ecx
// CHECK-NEXT: 4011fe:       retl
// CHECK-NEXT: 4011ff:       int3
// CHECK-NEXT: 401200:       pushl   %eax
// CHECK-NEXT: 401201:       movl    4207284, %eax
// CHECK-NEXT: 401206:       calll   -27 <.plt+0x20>
// CHECK-NEXT: 40120b:       jmp     -47 <.plt+0x11>
// CHECK-NEXT: 401210:       pushl   $0
// CHECK-NEXT: 401215:       jmp     -74 <.plt>
// CHECK-NEXT: 40121a:       int3
// CHECK-NEXT: 40121b:       int3
// CHECK-NEXT: 40121c:       int3
// CHECK-NEXT: 40121d:       int3
// CHECK-NEXT: 40121e:       int3
// CHECK-NEXT: 40121f:       int3
// CHECK-NEXT: 401220:       pushl   %eax
// CHECK-NEXT: 401221:       movl    4207288, %eax
// CHECK-NEXT: 401226:       calll   -59 <.plt+0x20>
// CHECK-NEXT: 40122b:       jmp     -79 <.plt+0x11>
// CHECK-NEXT: 401230:       pushl   $8
// CHECK-NEXT: 401235:       jmp     -106 <.plt>
// CHECK-NEXT: 40123a:       int3
// CHECK-NEXT: 40123b:       int3
// CHECK-NEXT: 40123c:       int3
// CHECK-NEXT: 40123d:       int3
// CHECK-NEXT: 40123e:       int3
// CHECK-NEXT: 40123f:       int3
=======
// RUN: ld.lld -shared %t2.o -o %t2.so

// RUN: ld.lld %t1.o %t2.so -o %t.exe -z retpolineplt
// RUN: llvm-objdump -d -s %t.exe | FileCheck %s

// CHECK:      Disassembly of section .plt:
// CHECK-NEXT: .plt:
// CHECK-NEXT: 11010:       ff 35 04 20 01 00       pushl   73732
// CHECK-NEXT: 11016:       50      pushl   %eax
// CHECK-NEXT: 11017:       a1 08 20 01 00  movl    73736, %eax
// CHECK-NEXT: 1101c:       e8 0f 00 00 00  calll   15 <.plt+0x20>
// CHECK-NEXT: 11021:       f3 90   pause
// CHECK-NEXT: 11023:       0f ae e8        lfence
// CHECK-NEXT: 11026:       eb f9   jmp     -7 <.plt+0x11>
// CHECK-NEXT: 11028:       cc      int3
// CHECK-NEXT: 11029:       cc      int3
// CHECK-NEXT: 1102a:       cc      int3
// CHECK-NEXT: 1102b:       cc      int3
// CHECK-NEXT: 1102c:       cc      int3
// CHECK-NEXT: 1102d:       cc      int3
// CHECK-NEXT: 1102e:       cc      int3
// CHECK-NEXT: 1102f:       cc      int3
// CHECK-NEXT: 11030:       89 0c 24        movl    %ecx, (%esp)
// CHECK-NEXT: 11033:       8b 4c 24 04     movl    4(%esp), %ecx
// CHECK-NEXT: 11037:       89 44 24 04     movl    %eax, 4(%esp)
// CHECK-NEXT: 1103b:       89 c8   movl    %ecx, %eax
// CHECK-NEXT: 1103d:       59      popl    %ecx
// CHECK-NEXT: 1103e:       c3      retl
// CHECK-NEXT: 1103f:       cc      int3
// CHECK-NEXT: 11040:       50      pushl   %eax
// CHECK-NEXT: 11041:       a1 0c 20 01 00  movl    73740, %eax
// CHECK-NEXT: 11046:       e8 e5 ff ff ff  calll   -27 <.plt+0x20>
// CHECK-NEXT: 1104b:       e9 d1 ff ff ff  jmp     -47 <.plt+0x11>
// CHECK-NEXT: 11050:       68 00 00 00 00  pushl   $0
// CHECK-NEXT: 11055:       e9 b6 ff ff ff  jmp     -74 <.plt>
// CHECK-NEXT: 1105a:       cc      int3
// CHECK-NEXT: 1105b:       cc      int3
// CHECK-NEXT: 1105c:       cc      int3
// CHECK-NEXT: 1105d:       cc      int3
// CHECK-NEXT: 1105e:       cc      int3
// CHECK-NEXT: 1105f:       cc      int3
// CHECK-NEXT: 11060:       50      pushl   %eax
// CHECK-NEXT: 11061:       a1 10 20 01 00  movl    73744, %eax
// CHECK-NEXT: 11066:       e8 c5 ff ff ff  calll   -59 <.plt+0x20>
// CHECK-NEXT: 1106b:       e9 b1 ff ff ff  jmp     -79 <.plt+0x11>
// CHECK-NEXT: 11070:       68 08 00 00 00  pushl   $8
// CHECK-NEXT: 11075:       e9 96 ff ff ff  jmp     -106 <.plt>
// CHECK-NEXT: 1107a:       cc      int3
// CHECK-NEXT: 1107b:       cc      int3
// CHECK-NEXT: 1107c:       cc      int3
// CHECK-NEXT: 1107d:       cc      int3
// CHECK-NEXT: 1107e:       cc      int3
// CHECK-NEXT: 1107f:       cc      int3

// CHECK:      Contents of section .got.plt:
// CHECK-NEXT: 00300100 00000000 00000000 50100100
// CHECK-NEXT: 70100100
>>>>>>> origin/release/5.x

.global _start
_start:
  jmp bar@PLT
  jmp zed@PLT
