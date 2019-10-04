<<<<<<< HEAD
; RUN: llc -mtriple thumbv7-windows-itanium -filetype asm -o /dev/null %s -print-after=finalize-isel 2>&1 | FileCheck %s
=======
; RUN: llc -mtriple thumbv7-windows-itanium -filetype asm -o /dev/null %s -print-machineinstrs=expand-isel-pseudos 2>&1 | FileCheck %s
>>>>>>> origin/release/5.x

declare arm_aapcs_vfpcc void @g(i8*) local_unnamed_addr

define arm_aapcs_vfpcc void @f(i32 %i) local_unnamed_addr {
entry:
  %vla = alloca i8, i32 %i, align 1
  call arm_aapcs_vfpcc void @g(i8* nonnull %vla)
  ret void
}

<<<<<<< HEAD
; CHECK: tBL 14, $noreg, &__chkstk, implicit-def $lr, implicit $sp, implicit killed $r4, implicit-def $r4, implicit-def dead $r12, implicit-def dead $cpsr
=======
; CHECK: tBL pred:14, pred:%noreg, <es:__chkstk>, %LR<imp-def>, %SP<imp-use>, %R4<imp-use,kill>, %R4<imp-def>, %R12<imp-def,dead>, %CPSR<imp-def,dead>
>>>>>>> origin/release/5.x

