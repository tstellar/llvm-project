; RUN: llc -filetype=obj %s -o - | obj2yaml | FileCheck %s

target triple = "wasm32-unknown-unknown"

define void @test() {
  call void @foo()
  call void @plain()
  ret void
}

declare void @foo() #0
declare void @plain()

attributes #0 = { "wasm-import-module"="bar" "wasm-import-name"="qux" }

; CHECK:        - Type:            IMPORT
<<<<<<< HEAD
; CHECK-NEXT:     Imports:
=======
; CHECK-NEXT:     Imports:         
>>>>>>> release/8.x
; CHECK:            - Module:          bar
; CHECK-NEXT:         Field:           qux
; CHECK-NEXT:         Kind:            FUNCTION

; CHECK:            - Module:          env
; CHECK-NEXT:         Field:           plain
; CHECK-NEXT:         Kind:            FUNCTION

; CHECK:        - Type:            CUSTOM
; CHECK:              Name:            foo
<<<<<<< HEAD
; CHECK-NEXT:         Flags:           [ UNDEFINED, EXPLICIT_NAME ]
=======
; CHECK-NEXT:         Flags:           [ UNDEFINED ]
>>>>>>> release/8.x

; CHECK:              Name:            plain
; CHECK-NEXT:         Flags:           [ UNDEFINED ]
