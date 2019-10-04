<<<<<<< HEAD
; RUN: llc -mtriple thumbv7-unknown-none-eabi -float-abi soft -filetype asm -o - %s | FileCheck %s
; RUN: llc -mtriple thumbv7-unknown-none-eabi -float-abi hard -filetype asm -o - %s | FileCheck %s
; RUN: llc -mtriple thumbv7-unknown-none-eabihf -float-abi soft -filetype asm -o - %s | FileCheck %s
; RUN: llc -mtriple thumbv7-unknown-none-eabihf -float-abi hard -filetype asm -o - %s | FileCheck %s

; RUN: llc -mtriple thumbv7-unknown-none-gnueabi -float-abi soft -filetype asm -o - %s | FileCheck %s
; RUN: llc -mtriple thumbv7-unknown-none-gnueabi -float-abi hard -filetype asm -o - %s | FileCheck %s
; RUN: llc -mtriple thumbv7-unknown-none-gnueabihf -float-abi soft -filetype asm -o - %s | FileCheck %s
; RUN: llc -mtriple thumbv7-unknown-none-gnueabihf -float-abi hard -filetype asm -o - %s | FileCheck %s

; RUN: llc -mtriple thumbv7-unknown-none-musleabi -float-abi soft -filetype asm -o - %s | FileCheck %s
; RUN: llc -mtriple thumbv7-unknown-none-musleabi -float-abi hard -filetype asm -o - %s | FileCheck %s
; RUN: llc -mtriple thumbv7-unknown-none-musleabihf -float-abi soft -filetype asm -o - %s | FileCheck %s
; RUN: llc -mtriple thumbv7-unknown-none-musleabihf -float-abi hard -filetype asm -o - %s | FileCheck %s

declare float @llvm.powi.f32(float, i32)

define float @powi_f32(float %f, i32 %i) {
=======
; RUN: llc -mtriple thumbv7-unknown-none-eabi -float-abi soft -filetype asm -o - %s | FileCheck %s -check-prefix CHECK-MATCH
; RUN: llc -mtriple thumbv7-unknown-none-eabi -float-abi hard -filetype asm -o - %s | FileCheck %s -check-prefix CHECK-MISMATCH -check-prefix CHECK-TO-SOFT
; RUN: llc -mtriple thumbv7-unknown-none-eabihf -float-abi soft -filetype asm -o - %s | FileCheck %s -check-prefix CHECK-MISMATCH -check-prefix CHECK-TO-HARD
; RUN: llc -mtriple thumbv7-unknown-none-eabihf -float-abi hard -filetype asm -o - %s | FileCheck %s -check-prefix CHECK-MATCH

; RUN: llc -mtriple thumbv7-unknown-none-gnueabi -float-abi soft -filetype asm -o - %s | FileCheck %s -check-prefix CHECK-MATCH
; RUN: llc -mtriple thumbv7-unknown-none-gnueabi -float-abi hard -filetype asm -o - %s | FileCheck %s -check-prefix CHECK-MISMATCH -check-prefix CHECK-TO-SOFT
; RUN: llc -mtriple thumbv7-unknown-none-gnueabihf -float-abi soft -filetype asm -o - %s | FileCheck %s -check-prefix CHECK-MISMATCH -check-prefix CHECK-TO-HARD
; RUN: llc -mtriple thumbv7-unknown-none-gnueabihf -float-abi hard -filetype asm -o - %s | FileCheck %s -check-prefix CHECK-MATCH

; RUN: llc -mtriple thumbv7-unknown-none-musleabi -float-abi soft -filetype asm -o - %s | FileCheck %s -check-prefix CHECK-MATCH
; RUN: llc -mtriple thumbv7-unknown-none-musleabi -float-abi hard -filetype asm -o - %s | FileCheck %s -check-prefix CHECK-MISMATCH -check-prefix CHECK-TO-SOFT
; RUN: llc -mtriple thumbv7-unknown-none-musleabihf -float-abi soft -filetype asm -o - %s | FileCheck %s -check-prefix CHECK-MISMATCH -check-prefix CHECK-TO-HARD
; RUN: llc -mtriple thumbv7-unknown-none-musleabihf -float-abi hard -filetype asm -o - %s | FileCheck %s -check-prefix CHECK-MATCH

declare float @llvm.powi.f32(float, i32)

define float @f(float %f, i32 %i) {
>>>>>>> origin/release/4.x
entry:
  %0 = call float @llvm.powi.f32(float %f, i32 %i)
  ret float %0
}

<<<<<<< HEAD
; CHECK: b __powisf2

declare double @llvm.powi.f64(double, i32)

define double @powi_f64(double %d, i32 %i) {
=======
; CHECK-MATCH: b __powisf2
; CHECK-MISMATCH: bl __powisf2
; CHECK-TO-SOFT: vmov s0, r0
; CHECK-TO-HARD: vmov r0, s0

declare double @llvm.powi.f64(double, i32)

define double @g(double %d, i32 %i) {
>>>>>>> origin/release/4.x
entry:
  %0 = call double @llvm.powi.f64(double %d, i32 %i)
  ret double %0
}

<<<<<<< HEAD
; CHECK: b __powidf2

declare float @llvm.floor.f32(float)

define float @floor_f32(float %f) {
entry:
  %0 = call float @llvm.floor.f32(float %f)
  ret float %0
}

; CHECK: b floorf

declare double @llvm.floor.f64(double)

define double @floor_f64(double %d) {
entry:
  %0 = call double @llvm.floor.f64(double %d)
  ret double %0
}

; CHECK: b floor
=======
; CHECK-MATCH: b __powidf2
; CHECK-MISMATCH: bl __powidf2
; CHECK-TO-SOFT: vmov d0, r0, r1
; CHECK-TO-HARD: vmov r0, r1, d0

>>>>>>> origin/release/4.x
