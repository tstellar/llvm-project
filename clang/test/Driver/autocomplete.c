// Test for the --autocompletion flag, which is an API used for shell
// autocompletion. You may have to update tests in this file when you
// add/modify flags, change HelpTexts or the values of some flags.

// Some corner cases.
// Just test that this doesn't crash:
// RUN: %clang --autocomplete=
// RUN: %clang --autocomplete=,
// RUN: %clang --autocomplete==
// RUN: %clang --autocomplete=,,
// RUN: %clang --autocomplete=-

// RUN: %clang --autocomplete=-fsyn | FileCheck %s -check-prefix=FSYN
// FSYN: -fsyntax-only
<<<<<<< HEAD
// RUN: %clang --autocomplete=-std | FileCheck %s -check-prefix=STD
=======
// RUN: %clang --autocomplete=-std= | FileCheck %s -check-prefix=STD
>>>>>>> origin/release/5.x
// STD: -std= Language standard to compile for
// RUN: %clang --autocomplete=foo | FileCheck %s -check-prefix=FOO
// FOO-NOT: foo
// RUN: %clang --autocomplete=-stdlib=,l | FileCheck %s -check-prefix=STDLIB
// STDLIB: libc++
// STDLIB-NEXT: libstdc++
<<<<<<< HEAD
// RUN: %clang --autocomplete=-stdlib= | FileCheck %s -check-prefix=STDLIBALL
=======
// RUN: %clang --autocomplete=-stdlib=, | FileCheck %s -check-prefix=STDLIBALL
>>>>>>> origin/release/5.x
// STDLIBALL: libc++
// STDLIBALL-NEXT: libstdc++
// STDLIBALL-NEXT: platform
// RUN: %clang --autocomplete=-meabi,d | FileCheck %s -check-prefix=MEABI
// MEABI: default
// RUN: %clang --autocomplete=-meabi, | FileCheck %s -check-prefix=MEABIALL
<<<<<<< HEAD
// RUN: %clang --autocomplete=-meabi | FileCheck %s -check-prefix=MEABIALL
=======
>>>>>>> origin/release/5.x
// MEABIALL: 4
// MEABIALL-NEXT: 5
// MEABIALL-NEXT: default
// MEABIALL-NEXT: gnu
// RUN: %clang --autocomplete=-cl-std=,CL2 | FileCheck %s -check-prefix=CLSTD
// CLSTD: CL2.0
<<<<<<< HEAD
// RUN: %clang --autocomplete=-cl-std= | FileCheck %s -check-prefix=CLSTDALL

=======
// RUN: %clang --autocomplete=-cl-std=, | FileCheck %s -check-prefix=CLSTDALL
>>>>>>> origin/release/5.x
// CLSTDALL: cl
// CLSTDALL-NEXT: CL
// CLSTDALL-NEXT: cl1.1
// CLSTDALL-NEXT: CL1.1
// CLSTDALL-NEXT: cl1.2
// CLSTDALL-NEXT: CL1.2
// CLSTDALL-NEXT: cl2.0
// CLSTDALL-NEXT: CL2.0
<<<<<<< HEAD
// CLSTDALL-NEXT: clc++
// CLSTDALL-NEXT: CLC++
// RUN: %clang --autocomplete=-fno-sanitize-coverage=,f | FileCheck %s -check-prefix=FNOSANICOVER
// FNOSANICOVER: func
// RUN: %clang --autocomplete=-fno-sanitize-coverage= | FileCheck %s -check-prefix=FNOSANICOVERALL
=======
// RUN: %clang --autocomplete=-fno-sanitize-coverage=,f | FileCheck %s -check-prefix=FNOSANICOVER
// FNOSANICOVER: func
// RUN: %clang --autocomplete=-fno-sanitize-coverage=, | FileCheck %s -check-prefix=FNOSANICOVERALL
>>>>>>> origin/release/5.x
// FNOSANICOVERALL: 8bit-counters
// FNOSANICOVERALL-NEXT: bb
// FNOSANICOVERALL-NEXT: edge
// FNOSANICOVERALL-NEXT: func
// FNOSANICOVERALL-NEXT: indirect-calls
// FNOSANICOVERALL-NEXT: inline-8bit-counters
// FNOSANICOVERALL-NEXT: no-prune
// FNOSANICOVERALL-NEXT: trace-bb
// FNOSANICOVERALL-NEXT: trace-cmp
// FNOSANICOVERALL-NEXT: trace-div
// FNOSANICOVERALL-NEXT: trace-gep
// FNOSANICOVERALL-NEXT: trace-pc
// FNOSANICOVERALL-NEXT: trace-pc-guard
<<<<<<< HEAD
// RUN: %clang --autocomplete=-ffp-contract= | FileCheck %s -check-prefix=FFPALL
// FFPALL: fast
// FFPALL-NEXT: off
// FFPALL-NEXT: on
// RUN: %clang --autocomplete=-flto= | FileCheck %s -check-prefix=FLTOALL
// FLTOALL: full
// FLTOALL-NEXT: thin
// RUN: %clang --autocomplete=-fveclib= | FileCheck %s -check-prefix=FVECLIBALL
// FVECLIBALL: Accelerate
// FVECLIBALL-NEXT: MASSV
// FVECLIBALL-NEXT: none
// FVECLIBALL-NEXT: SVML
// RUN: %clang --autocomplete=-fshow-overloads= | FileCheck %s -check-prefix=FSOVERALL
// FSOVERALL: all
// FSOVERALL-NEXT: best
// RUN: %clang --autocomplete=-fvisibility= | FileCheck %s -check-prefix=FVISIBILITYALL
// FVISIBILITYALL: default
// FVISIBILITYALL-NEXT: hidden
// RUN: %clang --autocomplete=-mfloat-abi= | FileCheck %s -check-prefix=MFLOATABIALL
// MFLOATABIALL: hard
// MFLOATABIALL-NEXT: soft
// MFLOATABIALL-NEXT: softfp
// RUN: %clang --autocomplete=-mthread-model | FileCheck %s -check-prefix=MTHREADMODELALL
// MTHREADMODELALL: posix
// MTHREADMODELALL-NEXT: single
// RUN: %clang --autocomplete=-mrelocation-model | FileCheck %s -check-prefix=MRELOCMODELALL
=======
// RUN: %clang --autocomplete=-ffp-contract=, | FileCheck %s -check-prefix=FFPALL
// FFPALL: fast
// FFPALL-NEXT: off
// FFPALL-NEXT: on
// RUN: %clang --autocomplete=-flto=, | FileCheck %s -check-prefix=FLTOALL
// FLTOALL: full
// FLTOALL-NEXT: thin
// RUN: %clang --autocomplete=-fveclib=, | FileCheck %s -check-prefix=FVECLIBALL
// FVECLIBALL: Accelerate
// FVECLIBALL-NEXT: none
// FVECLIBALL-NEXT: SVML
// RUN: %clang --autocomplete=-fshow-overloads=, | FileCheck %s -check-prefix=FSOVERALL
// FSOVERALL: all
// FSOVERALL-NEXT: best
// RUN: %clang --autocomplete=-fvisibility=, | FileCheck %s -check-prefix=FVISIBILITYALL
// FVISIBILITYALL: default
// FVISIBILITYALL-NEXT: hidden
// RUN: %clang --autocomplete=-mfloat-abi=, | FileCheck %s -check-prefix=MFLOATABIALL
// MFLOATABIALL: hard
// MFLOATABIALL-NEXT: soft
// MFLOATABIALL-NEXT: softfp
// RUN: %clang --autocomplete=-mthread-model, | FileCheck %s -check-prefix=MTHREADMODELALL
// MTHREADMODELALL: posix
// MTHREADMODELALL-NEXT: single
// RUN: %clang --autocomplete=-mrelocation-model, | FileCheck %s -check-prefix=MRELOCMODELALL
>>>>>>> origin/release/5.x
// MRELOCMODELALL: dynamic-no-pic
// MRELOCMODELALL-NEXT: pic
// MRELOCMODELALL-NEXT: ropi
// MRELOCMODELALL-NEXT: ropi-rwpi
// MRELOCMODELALL-NEXT: rwpi
// MRELOCMODELALL-NEXT: static
<<<<<<< HEAD
// RUN: %clang --autocomplete=-Wma | FileCheck %s -check-prefix=WARNING
// WARNING: -Wmacro-redefined
// WARNING-NEXT: -Wmain
// WARNING-NEXT: -Wmain-return-type
// WARNING-NEXT: -Wmalformed-warning-check
// WARNING-NEXT: -Wmany-braces-around-scalar-init
// WARNING-NEXT: -Wmax-unsigned-zero
// RUN: %clang --autocomplete=-Wno-invalid-pp- | FileCheck %s -check-prefix=NOWARNING
// NOWARNING: -Wno-invalid-pp-token
// RUN: %clang --autocomplete=-analyzer-checker | FileCheck %s -check-prefix=ANALYZER
// ANALYZER: unix.Malloc
// RUN: %clang --autocomplete=-std= | FileCheck %s -check-prefix=STDVAL
// STDVAL: c99
//
// Clang shouldn't autocomplete CC1 options unless -cc1 or -Xclang were provided
=======
>>>>>>> origin/release/5.x
// RUN: %clang --autocomplete=-mrelocation-mode | FileCheck %s -check-prefix=MRELOCMODEL_CLANG
// MRELOCMODEL_CLANG-NOT: -mrelocation-model
// RUN: %clang --autocomplete=-Xclang,-mrelocation-mode | FileCheck %s -check-prefix=MRELOCMODEL_CC1
// RUN: %clang --autocomplete=-cc1,-mrelocation-mode | FileCheck %s -check-prefix=MRELOCMODEL_CC1
// MRELOCMODEL_CC1: -mrelocation-model
<<<<<<< HEAD
// Make sure it ignores passed flags unlesss they are -Xclang or -cc1
// RUN: %clang --autocomplete=foo,bar,,-fsyn | FileCheck %s -check-prefix=FSYN-CORON
// FSYN-CORON: -fsyntax-only
// Check if they can autocomplete values with coron
// RUN: %clang --autocomplete=foo,bar,,,-fno-sanitize-coverage=,f | FileCheck %s -check-prefix=FNOSANICOVER-CORON
// FNOSANICOVER-CORON: func

// Clang should return empty string when no value completion was found, which will fall back to file autocompletion
// RUN: %clang --autocomplete=-fmodule-file= | FileCheck %s -check-prefix=MODULE_FILE_EQUAL
// MODULE_FILE_EQUAL-NOT: -fmodule-file=
// RUN: %clang --autocomplete=-fmodule-file | FileCheck %s -check-prefix=MODULE_FILE
// MODULE_FILE: -fmodule-file=

// RUN: %clang --autocomplete=-Qunused-arguments, | FileCheck %s -check-prefix=QUNUSED_COMMA
// QUNUSED_COMMA-NOT: -Qunused-arguments
// RUN: %clang --autocomplete=-Qunused-arguments | FileCheck %s -check-prefix=QUNUSED
// QUNUSED: -Qunused-arguments
=======
// RUN: %clang --autocomplete=-Wma | FileCheck %s -check-prefix=WARNING
// WARNING: -Wmacro-redefined
// WARNING-NEXT: -Wmain
// WARNING-NEXT: -Wmain-return-type
// WARNING-NEXT: -Wmalformed-warning-check
// WARNING-NEXT: -Wmany-braces-around-scalar-init
// WARNING-NEXT: -Wmax-unsigned-zero
// RUN: %clang --autocomplete=-Wno-invalid-pp- | FileCheck %s -check-prefix=NOWARNING
// NOWARNING: -Wno-invalid-pp-token
>>>>>>> origin/release/5.x
