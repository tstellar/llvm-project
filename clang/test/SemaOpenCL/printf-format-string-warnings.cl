// RUN: %clang_cc1 %s -verify -pedantic -fsyntax-only -cl-std=CL2.0 -finclude-default-header

// FIXME: Make sure warnings are produced based on printf format strings.

<<<<<<< HEAD
=======
// expected-no-diagnostics

>>>>>>> release/8.x
kernel void format_string_warnings(__constant char* arg) {

  printf("%d", arg);

  printf("not enough arguments %d %d", 4);

  printf("too many arguments", 4);
}
