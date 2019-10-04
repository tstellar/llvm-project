; RUN: llc -march=hexagon -filetype=obj %s -o - \
; RUN: | llvm-objdump -d - | FileCheck %s

define i32 @foo (i1 %a, i32 %b, i32 %c)
{
  %1 = select i1 %a, i32 %b, i32 %c
  ret i32 %1
}

<<<<<<< HEAD
; CHECK: 00 c0 00 85 8500c000 {  p0 = tstbit(r0,#0) } 
; CHECK: 00 42 01 f4 f4014200 {  r0 = mux(p0,r1,r2)
; CHECK: 00 c0 9f 52 529fc000    jumpr r31 } 
=======
; CHECK: 00 40 40 85 85404000
; CHECK: 00 40 9f 52 529f4000
; CHECK: 00 60 01 74 74016000
; CHECK: 00 e0 82 74 7482e000
>>>>>>> origin/release/4.x
