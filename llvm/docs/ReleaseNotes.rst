=========================
LLVM 10.0.0 Release Notes
=========================

.. contents::
    :local:

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
.. warning::
   These are in-progress notes for the upcoming LLVM 10 release.
   Release notes for previous releases can be found on
   `the Download Page <https://releases.llvm.org/download.html>`_.

=======
>>>>>>> release/7.x
=======
>>>>>>> origin/release/5.x

=======
>>>>>>> release/8.x
Introduction
============

This document contains the release notes for the LLVM Compiler Infrastructure,
release 10.0.0.  Here we describe the status of LLVM, including major improvements
from the previous release, improvements in various subprojects of LLVM, and
some of the current users of the code.  All LLVM releases may be downloaded
<<<<<<< HEAD
from the `LLVM releases web site <https://releases.llvm.org/>`_.
=======
from the `LLVM releases web site <https://llvm.org/releases/>`_.
>>>>>>> release/7.x

For more information about LLVM, including information about the latest
release, please check out the `main LLVM web site <https://llvm.org/>`_.  If you
have questions or comments, the `LLVM Developer's Mailing List
<https://lists.llvm.org/mailman/listinfo/llvm-dev>`_ is a good place to send
them.

<<<<<<< HEAD
<<<<<<< HEAD
Minimum Required Compiler Version
=================================
As `discussed on the mailing list
<https://lists.llvm.org/pipermail/llvm-dev/2019-January/129452.html>`_,
building LLVM will soon require more recent toolchains as follows:

============= ====
Clang         3.5
Apple Clang   6.0
GCC           5.1
Visual Studio 2017
============= ====

A new CMake check when configuring LLVM provides a soft-error if your
toolchain will become unsupported soon. You can opt out of the soft-error by
setting the ``LLVM_TEMPORARILY_ALLOW_OLD_TOOLCHAIN`` CMake variable to
``ON``.

=======
Non-comprehensive list of changes in this release
=================================================

* The Windows installer no longer includes a Visual Studio integration.
  Instead, a new
  `LLVM Compiler Toolchain Visual Studio extension <https://marketplace.visualstudio.com/items?itemName=LLVMExtensions.llvm-toolchain>`_
  is available on the Visual Studio Marketplace. The new integration
  supports Visual Studio 2017.

* The LoopInstSimplify pass (``-loop-instsimplify``) has been removed.
>>>>>>> release/7.x

Known Issues
============

These are issues that couldn't be fixed before the release. See the bug reports
for the latest status.

* `PR40547 <https://llvm.org/pr40547>`_ Clang gets miscompiled by trunk GCC.

<<<<<<< HEAD
* `PR40761 <https://llvm.org/pr40761>`_ "asan-dynamic" doesn't work on FreeBSD.

=======
* Optimization of floating-point casts is improved. This may cause surprising
  results for code that is relying on the undefined behavior of overflowing
  casts. The optimization can be disabled by specifying a function attribute:
  ``"strict-float-cast-overflow"="false"``. This attribute may be created by the
  clang option ``-fno-strict-float-cast-overflow``.
  Code sanitizers can be used to detect affected patterns. The clang option for
  detecting this problem alone is ``-fsanitize=float-cast-overflow``:
>>>>>>> release/7.x

Non-comprehensive list of changes in this release
=================================================

<<<<<<< HEAD
* The ISD::FP_ROUND_INREG opcode and related code was removed from SelectionDAG.
* Enabled MemorySSA as a loop dependency.
=======
* The **llvm-cov** tool can now export lcov trace files using the
  `-format=lcov` option of the `export` command.

* The ``add_llvm_loadable_module`` CMake macro has been removed.  The
  ``add_llvm_library`` macro with the ``MODULE`` argument now provides the same
  functionality.  See `Writing an LLVM Pass
  <WritingAnLLVMPass.html#setting-up-the-build-environment>`_.
>>>>>>> release/8.x

* For MinGW, references to data variables that might need to be imported
  from a dll are accessed via a stub, to allow the linker to convert it to
  a dllimport if needed.

* Added support for labels as offsets in ``.reloc`` directive.

* Support for precise identification of X86 instructions with memory operands,
  by using debug information. This supports profile-driven cache prefetching.
  It is enabled with the ``-x86-discriminate-memops`` LLVM Flag.

* Support for profile-driven software cache prefetching on X86. This is part of
  a larger system, consisting of: an offline cache prefetches recommender,
  AutoFDO tooling, and LLVM. In this system, a binary compiled with
  ``-x86-discriminate-memops`` is run under the observation of the recommender.
  The recommender identifies certain memory access instructions by their binary
  file address, and recommends a prefetch of a specific type (NTA, T0, etc) be
  performed at a specified fixed offset from such an instruction's memory
  operand. Next, this information needs to be converted to the AutoFDO syntax
  and the resulting profile may be passed back to the compiler with the LLVM
  flag ``-prefetch-hints-file``, together with the exact same set of
  compilation parameters used for the original binary. More information is
  available in the `RFC
  <https://lists.llvm.org/pipermail/llvm-dev/2018-November/127461.html>`_.

* Windows support for libFuzzer (x86_64).
=======
Non-comprehensive list of changes in this release
=================================================

* LLVM's ``WeakVH`` has been renamed to ``WeakTrackingVH`` and a new ``WeakVH``
  has been introduced.  The new ``WeakVH`` nulls itself out on deletion, but
  does not track values across RAUW.

* A new library named ``BinaryFormat`` has been created which holds a collection
  of code which previously lived in ``Support``.  This includes the
  ``file_magic`` structure and ``identify_magic`` functions, as well as all the
  structure and type definitions for DWARF, ELF, COFF, WASM, and MachO file
  formats.

* The tool ``llvm-pdbdump`` has been renamed ``llvm-pdbutil`` to better reflect
  its nature as a general purpose PDB manipulation / diagnostics tool that does
  more than just dumping contents.

* The ``BBVectorize`` pass has been removed. It was fully replaced and no
  longer used back in 2014 but we didn't get around to removing it. Now it is
  gone. The SLP vectorizer is the suggested non-loop vectorization pass.

* A new tool opt-viewer.py has been added to visualize optimization remarks in
  HTML.  The tool processes the YAML files produced by clang with the
  -fsave-optimization-record option.

* A new CMake macro ``LLVM_REVERSE_ITERATION`` has been added. If enabled, all
  supported unordered LLVM containers would be iterated in reverse order. This
  is useful for uncovering non-determinism caused by iteration of unordered
  containers. Currently, it supports reverse iteration of SmallPtrSet and
  DenseMap.

* A new tool ``llvm-dlltool`` has been added to create short import libraries
  from GNU style definition files. The tool utilizes the PE COFF SPEC Import
  Library Format and PE COFF Auxiliary Weak Externals Format to achieve
  compatibility with LLD and MSVC LINK.

>>>>>>> origin/release/5.x

Changes to the LLVM IR
----------------------

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
* Unnamed function arguments now get printed with their automatically
  generated name (e.g. "i32 %0") in definitions. This may require front-ends
  to update their tests; if so there is a script utils/add_argument_names.py
  that correctly converted 80-90% of Clang tests. Some manual work will almost
  certainly still be needed.
=======
    clang -O1 ftrunc.c -fsanitize=float-cast-overflow ; ./a.out
    ftrunc.c:5:15: runtime error: 4.29497e+09 is outside the range of representable values of type 'int'
    junk in the ftrunc: 0.000000
>>>>>>> release/7.x


Changes to building LLVM
------------------------
=======
* Function attribute ``speculative_load_hardening`` has been introduced to
  allow indicating that `Speculative Load Hardening
  <SpeculativeLoadHardening.html>`_ must be enabled for the function body.
>>>>>>> release/8.x


Changes to the JIT APIs
-----------------------

<<<<<<< HEAD
The ORC (On Request Compilation) JIT APIs have been updated to support
concurrent compilation. The existing (non-concurrent) ORC layer classes and
related APIs are deprecated, have been renamed with a "Legacy" prefix (e.g.
LegacyIRCompileLayer). The deprecated clasess will be removed in LLVM 9.

An example JIT stack using the concurrent ORC APIs, called LLJIT, has been
added (see include/llvm/ExecutionEngine/Orc/LLJIT.h). The lli tool has been
updated to use LLJIT.

MCJIT and ExecutionEngine continue to be supported, though ORC should be
preferred for new projects.

Changes to the C++ APIs
-----------------------

Three of the IR library methods related to debugging information for
functions and methods have changed their prototypes:

  DIBuilder::createMethod
  DIBuilder::createFunction
  DIBuilder::createTempFunctionFwdDecl
=======
* Early support for UBsan, X-Ray instrumentation and libFuzzer (x86 and x86_64)
  for OpenBSD. Support for MSan (x86_64), X-Ray instrumentation and libFuzzer
  (x86 and x86_64) for FreeBSD.

* ``SmallVector<T, 0>`` shrank from ``sizeof(void*) * 4 + sizeof(T)`` to
  ``sizeof(void*) + sizeof(unsigned) * 2``, smaller than ``std::vector<T>`` on
  64-bit platforms.  The maximum capacity is now restricted to ``UINT32_MAX``.
  Since SmallVector doesn't have the exception-safety pessimizations some
  implementations saddle ``std::vector`` with and is better at using ``realloc``,
  it's now a better choice even on the heap (although when ``TinyPtrVector`` works,
  that's even smaller).

* Preliminary/experimental support for DWARF v5 debugging information,
  including the new ``.debug_names`` accelerator table. DWARF emitted at ``-O0``
  should be fully DWARF v5 compliant. Type units and split DWARF are known
  not to be compliant, and higher optimization levels will still emit some
  information in v4 format.

* Added support for the ``.rva`` assembler directive for COFF targets.

* The :program:`llvm-rc` tool (Windows Resource Compiler) has been improved
  a bit. There are still known missing features, but it is generally usable
  in many cases. (The tool still doesn't preprocess input files automatically,
  but it can now handle leftover C declarations in preprocessor output, if
  given output from a preprocessor run externally.)

* CodeView debug info can now be emitted for MinGW configurations, if requested.

* The :program:`opt` tool now supports the ``-load-pass-plugin`` option for
  loading pass plugins for the new PassManager.

* Support for profiling JITed code with perf.

>>>>>>> release/7.x

In all cases, several individual parameters were removed, and replaced
by a single 'SPFlags' (subprogram flags) parameter. The individual
parameters are: 'isLocalToUnit'; 'isDefinition'; 'isOptimized'; and
for 'createMethod', 'Virtuality'.  The new 'SPFlags' parameter has a
default value equivalent to passing 'false' for the three 'bool'
parameters, and zero (non-virtual) to the 'Virtuality' parameter.  For
any old-style API call that passed 'true' or a non-zero virtuality to
these methods, you will need to substitute the correct 'SPFlags' value.
The helper method 'DISubprogram::toSPFlags()' might be useful in making
this conversion.

<<<<<<< HEAD
Changes to the AArch64 Target
-----------------------------

* Support for Speculative Load Hardening has been added.

* Initial support for the Tiny code model, where code and its statically
  defined symbols must live within 1MB of each other.

* Added support for the ``.arch_extension`` assembler directive, just like
  on ARM.


Changes to the Hexagon Target
-----------------------------

* Added support for Hexagon/HVX V66 ISA.
=======
* The signatures for the builtins ``@llvm.memcpy``, ``@llvm.memmove``, and
  ``@llvm.memset`` have changed. Alignment is no longer an argument, and are
  instead conveyed as parameter attributes.

* ``invariant.group.barrier`` has been renamed to ``launder.invariant.group``.

* ``invariant.group`` metadata can now refer only to empty metadata nodes.

Changes to the AArch64 Target
-----------------------------

* The ``.inst`` assembler directive is now usable on both COFF and Mach-O
  targets, in addition to ELF.
>>>>>>> release/7.x

* Support for most remaining COFF relocations has been added.

* Support for TLS on Windows has been added.

* Assembler and disassembler support for the ARM Scalable Vector Extension has
  been added.

Changes to the ARM Target
-------------------------

* The ``.inst`` assembler directive is now usable on both COFF and Mach-O
  targets, in addition to ELF. For Thumb, it can now also automatically
  deduce the instruction size, without having to specify it with
  e.g. ``.inst.w`` as before.

Changes to the Hexagon Target
-----------------------------

* Hexagon now supports auto-vectorization for HVX. It is disabled by default
  and can be turned on with ``-fvectorize``. For auto-vectorization to take
  effect, code generation for HVX needs to be enabled with ``-mhvx``.
  The complete set of options should include ``-fvectorize``, ``-mhvx``,
  and ``-mhvx-length={64b|128b}``.

* The support for Hexagon ISA V4 is deprecated and will be removed in the
  next release.
=======
* The datalayout string may now indicate an address space to use for
  the pointer type of ``alloca`` rather than the default of 0.

* Added ``speculatable`` attribute indicating a function which has no
  side-effects which could inhibit hoisting of calls.

Changes to the Arm Targets
--------------------------

During this release the AArch64 target has:

* A much improved Global ISel at O0.
* Support for ARMv8.1 8.2 and 8.3 instructions.
* New scheduler information for ThunderX2.
* Some SVE type changes but not much more than that.
* Made instruction fusion more aggressive, resulting in speedups
  for code making use of AArch64 AES instructions. AES fusion has been
  enabled for most Cortex-A cores and the AArch64MacroFusion pass was moved
  to the generic MacroFusion pass.
* Added preferred function alignments for most Cortex-A cores.
* OpenMP "offload-to-self" base support.

During this release the ARM target has:

* Improved, but still mostly broken, Global ISel.
* Scheduling models update, new schedule for Cortex-A57.
* Hardware breakpoint support in LLDB.
* New assembler error handling, with spelling corrections and multiple
  suggestions on how to fix problems.
* Improved mixed ARM/Thumb code generation. Some cases in which wrong
  relocations were emitted have been fixed.
* Added initial support for mixed ARM/Thumb link-time optimization, using the
  thumb-mode target feature.
>>>>>>> origin/release/5.x

Changes to the MIPS Target
--------------------------

<<<<<<< HEAD
<<<<<<< HEAD
* Improved support of GlobalISel instruction selection framework.

* Implemented emission of ``R_MIPS_JALR`` and ``R_MICROMIPS_JALR``
  relocations. These relocations provide hints to a linker for optimization
  of jumps to protected symbols.

* ORC JIT has been supported for MIPS and MIPS64 architectures.

* Assembler now suggests alternative MIPS instruction mnemonics when
  an invalid one is specified.

* Improved support for MIPS N32 ABI.

* Added new instructions (``pll.ps``, ``plu.ps``, ``cvt.s.pu``,
  ``cvt.s.pl``, ``cvt.ps``, ``sigrie``).

* Numerous bug fixes and code cleanups.
=======
During this release the MIPS target has:

* Added support for Virtualization, Global INValidate ASE,
  and CRC ASE instructions.

* Introduced definitions of ``[d]rem``, ``[d]remu``,
  and microMIPSR6 ``ll/sc`` instructions.

* Shrink-wrapping is now supported and enabled by default (except for ``-O0``).

* Extended size reduction pass by the LWP and SWP instructions.

* Gained initial support of GlobalISel instruction selection framework.

* Updated the P5600 scheduler model not to use instruction itineraries.

* Added disassembly support for comparison and fused (negative) multiply
  ``add/sub`` instructions.

* Improved the selection of multiple instructions.

* Load/store ``lb``, ``sb``, ``ld``, ``sd``, ``lld``, ... instructions
  now support 32/64-bit offsets.

* Added support for ``y``, ``M``, and ``L`` inline assembler operand codes.

* Extended list of relocations supported by the ``.reloc`` directive

* Fixed using a wrong register class for creating an emergency
  spill slot for mips3 / n64 ABI.
>>>>>>> release/7.x
=======
* The microMIPS64R6 backend is deprecated and will be removed in the next
  release.

* The MIPS backend now directly supports vector types for arguments and return
  values (previously this required ABI specific LLVM IR).

* Added documentation for how the MIPS backend handles address lowering.

* Added a GCC compatible option -m(no-)madd4 to control the generation of four
  operand multiply addition/subtraction instructions.

* Added basic support for the XRay instrumentation system.

* Added support for more assembly aliases and macros.

* Added support for the ``micromips`` and ``nomicromips`` function attributes
  which control micromips code generation on a per function basis.

* Added the ``long-calls`` feature for non-pic environments. This feature is
  used where the callee is out of range of the caller using a standard call
  sequence. It must be enabled specifically.

* Added support for performing microMIPS code generation via function
  attributes.

* Added experimental support for the static relocation model for the N64 ABI.

* Added partial support for the MT ASE.

* Added basic support for code size reduction for microMIPS.

* Fixed numerous bugs including: multi-precision arithmetic support, various
  vectorization bugs, debug information for thread local variables, debug
  sections lacking the correct flags, crashing when disassembling sections
  whose size is not a multiple of two or four.
>>>>>>> origin/release/5.x

* MIPS relocation types were generated for microMIPS code.

* Corrected definitions of multiple instructions (``lwp``, ``swp``, ``ctc2``,
  ``cfc2``, ``sync``, ``synci``, ``cvt.d.w``, ...).

* Fixed atomic operations at ``-O0`` level.

* Fixed local dynamic TLS with Sym64

Changes to the PowerPC Target
-----------------------------

<<<<<<< HEAD
<<<<<<< HEAD
* Switched to non-PIC default
=======
* Additional support and exploitation of POWER ISA 3.0: vabsdub, vabsduh,
  vabsduw, modsw, moduw, modsd, modud, lxv, stxv, vextublx, vextubrx, vextuhlx,
  vextuhrx, vextuwlx, vextuwrx, vextsb2w, vextsb2d, vextsh2w, vextsh2d, and
  vextsw2d

* Implemented Optimal Code Sequences from The PowerPC Compiler Writer's Guide.

* Enable -fomit-frame-pointer by default.

* Improved handling of bit reverse intrinsic.

* Improved handling of memcpy and memcmp functions.

* Improved handling of branches with static branch hints.

* Improved codegen for atomic load_acquire.

* Improved block placement during code layout

* Many improvements to instruction selection and code generation

>>>>>>> origin/release/5.x

* Deprecated Darwin support

<<<<<<< HEAD
* Enabled Out-of-Order scheduling for P9

* Better overload rules for compatible vector type parameter

* Support constraint 'wi', modifier 'x' and VSX registers in inline asm

* More ``__float128`` support

* Added new builtins like vector int128 ``pack``/``unpack`` and
  ``stxvw4x.be``/``stxvd2x.be``

* Provided significant improvements to the automatic vectorizer

* Code-gen improvements (especially for Power9)
=======
* Added initial AMD Ryzen (znver1) scheduler support.

* Added support for Intel Goldmont CPUs.

* Add support for avx512vpopcntdq instructions.

* Added heuristics to convert CMOV into branches when it may be profitable.

* More aggressive inlining of memcmp calls.

* Improve vXi64 shuffles on 32-bit targets.

* Improved use of PMOVMSKB for any_of/all_of comparision reductions.

* Improved Silvermont, Sandybridge, and Jaguar (btver2) schedulers.

* Improved support for AVX512 vector rotations.

* Added support for AMD Lightweight Profiling (LWP) instructions.

* Avoid using slow LEA instructions.

* Use alternative sequences for multiply by constant.

* Improved lowering of strided shuffles.

* Improved the AVX512 cost model used by the vectorizer.

* Fix scalar code performance when AVX512 is enabled by making i1's illegal.

* Fixed many inline assembly bugs.

* Preliminary support for tracing NetBSD processes and core files with a single
  thread in LLDB.
>>>>>>> origin/release/5.x

* Fixed some long-standing bugs in the back end

* Added experimental prologue/epilogue improvements

* Enabled builtins tests in compiler-rt

* Add ``___fixunstfti``/``floattitf`` in compiler-rt to support conversion
  between IBM double-double and unsigned int128

* Disable randomized address space when running the sanitizers on Linux ppc64le

* Completed support in LLD for ELFv2

* Enabled llvm-exegesis latency mode for PPC


Changes to the SystemZ Target
-----------------------------

<<<<<<< HEAD
* A number of bugs related to C/C++ language vector extension support were
  fixed: the ``-mzvector`` option now actually enables the ``__vector`` and
  ``__bool`` keywords, the ``vec_step`` intrinsic now works, and the
  ``vec_insert_and_zero`` and ``vec_orc`` intrinsics now generate correct code.

* The ``__float128`` keyword, which had been accidentally enabled in some
  earlier releases, is now no longer supported.  On SystemZ, the ``long double``
  data type itself already uses the IEEE 128-bit floating-point format.

* When the compiler inlines ``strcmp`` or ``memcmp``, the generated code no
  longer returns ``INT_MIN`` as the negative result value under any
  circumstances.

* Various code-gen improvements, in particular related to improved
  auto-vectorization, inlining, and instruction scheduling.


Changes to the X86 Target
-------------------------

 During this release ...
=======
* Initial gfx9 support
>>>>>>> origin/release/5.x

* Less than 128 bit vector types, v2i32, v4i16, v2i16, v8i8, v4i8, and v2i8, are
  now stored in the lower bits of an xmm register and the upper bits are
  undefined. Previously the elements were spread apart with undefined bits in
  between them.
* v32i8 and v64i8 vectors with AVX512F enabled, but AVX512BW disabled will now
  be passed in ZMM registers for calls and returns. Previously they were passed
  in two YMM registers. Old behavior can be enabled by passing
  -x86-enable-old-knl-abi
* -mprefer-vector-width=256 is now the default behavior skylake-avx512 and later
  Intel CPUs. This tries to limit the use of 512-bit registers which can cause a
  decrease in CPU frequency on these CPUs. This can be re-enabled by passing
  -mprefer-vector-width=512 to clang or passing -mattr=-prefer-256-bit to llc.

* New AVX512F gather and scatter intrinsics were added that take a <X x i1> mask
  instead of a scalar integer. This removes the need for a bitcast in IR. The
  new intrinsics are named like the old intrinsics with ``llvm.avx512.``
  replaced with ``llvm.avx512.mask.``. The old intrinsics will be removed in a
  future release.

<<<<<<< HEAD
Changes to the AVR Target
=======
During this release the PowerPC target has:

<<<<<<< HEAD
* Replaced the list scheduler for post register allocation with the machine scheduler.

* Added support for ``coldcc`` calling convention.

* Added support for ``symbol@high`` and ``symbol@higha`` symbol modifiers.

* Added support for quad-precision floating point type (``__float128``) under the llvm option ``-enable-ppc-quad-precision``.

* Added dump function to ``LatencyPriorityQueue``.

* Completed the Power9 scheduler model.

* Optimized TLS code generation.

* Improved MachineLICM for hoisting constant stores.

* Improved code generation to reduce register use by using more register + immediate instructions.

* Improved code generation to better exploit rotate-and-mask instructions.

* Fixed the bug in dynamic loader for JIT which crashed NNVM.

* Numerous bug fixes and code cleanups.

Changes to the SystemZ Target
>>>>>>> release/7.x
-----------------------------
=======
* Added ``cascadelake`` as a CPU name for -march. This is ``skylake-avx512``
  with the addition of the ``avx512vnni`` instruction set.

* ADCX instruction will no longer be emitted. This instruction is rarely better
  than the legacy ADC instruction and just increased code size.
>>>>>>> release/8.x


* Deprecated the mpx feature flag for the Intel MPX instructions. There were no
  intrinsics for this feature. This change only this effects the results
  returned by getHostCPUFeatures on CPUs that implement the MPX instructions.

Changes to the WebAssembly Target
---------------------------------
=======
This release consists mainly of bugfixes and implementations of features
required for compiling basic Rust programs.

* Enable the branch relaxation pass so that we don't crash on large
  stack load/stores

* Add support for lowering bit-rotations to the native ``ror`` and ``rol``
  instructions

* Fix bug where function pointers were treated as pointers to RAM and not
  pointers to program memory

* Fix broken code generation for shift-by-variable expressions

* Support zero-sized types in argument lists; this is impossible in C,
  but possible in Rust
>>>>>>> origin/release/5.x

During this release the SystemZ target has:

* Added support for vector registers in inline asm statements.

* Added support for stackmaps, patchpoints, and the anyregcc
  calling convention.

* Changed the default function alignment to 16 bytes.

* Improved codegen for condition code handling.

* Improved instruction scheduling and microarchitecture tuning for z13/z14.

* Fixed support for generating GCOV coverage data.

* Fixed some codegen bugs.

Changes to the X86 Target
-------------------------

<<<<<<< HEAD
* The calling convention for the ``f80`` data type on MinGW targets has been
  fixed. Normally, the calling convention for this type is handled within clang,
  but if an intrinsic is used, which LLVM expands into a libcall, the
  proper calling convention needs to be supported in LLVM as well. (Note,
  on Windows, this data type is only used for long doubles in MinGW
  environments - in MSVC environments, long doubles are the same size as
  normal doubles.)


<<<<<<< HEAD
Changes to the Nios2 Target
---------------------------
=======
* Removed ``add_bb_vectorize``.
>>>>>>> release/7.x

* The Nios2 target was removed from this release.


<<<<<<< HEAD
Changes to LLDB
===============

* Printed source code is now syntax highlighted in the terminal (only for C
  languages).

* The expression command now supports tab completing expressions.


Changes to LLDB
===============

External Open Source Projects Using LLVM 10
===========================================
=======
Zig Programming Language
------------------------

`Zig <http://ziglang.org>`_  is an open-source programming language designed
for robustness, optimality, and clarity. It integrates closely with C and is
intended to eventually take the place of C. It uses LLVM to produce highly
optimized native code and to cross-compile for any target out of the box. Zig
is in alpha; with a beta release expected in September.
>>>>>>> origin/release/5.x

LDC - the LLVM-based D compiler
-------------------------------

`D <http://dlang.org>`_ is a language with C-like syntax and static typing. It
pragmatically combines efficiency, control, and modeling power, with safety and
programmer productivity. D supports powerful concepts like Compile-Time Function
Execution (CTFE) and Template Meta-Programming, provides an innovative approach
to concurrency and offers many classical paradigms.

`LDC <http://wiki.dlang.org/LDC>`_ uses the frontend from the reference compiler
combined with LLVM as backend to produce efficient native code. LDC targets
x86/x86_64 systems like Linux, OS X, FreeBSD and Windows and also Linux on ARM
and PowerPC (32/64 bit). Ports to other architectures like AArch64 and MIPS64
are underway.
<<<<<<< HEAD

Open Dylan Compiler
-------------------

`Dylan <https://opendylan.org/>`_ is a multi-paradigm functional
and object-oriented programming language.  It is dynamic while
providing a programming model designed to support efficient machine
code generation, including fine-grained control over dynamic and
static behavior. Dylan also features a powerful macro facility for
expressive metaprogramming.

The Open Dylan compiler can use LLVM as one of its code-generating
back-ends, including full support for debug info generation. (Open
Dylan generates LLVM bitcode directly using a native Dylan IR and
bitcode library.) Development of a Dylan debugger and interactive REPL
making use of the LLDB libraries is in progress.
=======
* Removed ``LLVMAddBBVectorizePass``. The implementation was removed and the C
  interface was made a deprecated no-op in LLVM 5. Use
  ``LLVMAddSLPVectorizePass`` instead to get the supported SLP vectorizer.

* Expanded the OrcJIT APIs so they can register event listeners like debuggers
  and profilers.

Changes to the DAG infrastructure
---------------------------------
* ``ADDC``/``ADDE``/``SUBC``/``SUBE`` are now deprecated and will default to expand. Backends
  that wish to continue to use these opcodes should explicitely request to do so
  using ``setOperationAction`` in their ``TargetLowering``. New backends
  should use ``UADDO``/``ADDCARRY``/``USUBO``/``SUBCARRY`` instead of the deprecated opcodes.

* The ``SETCCE`` opcode has now been removed in favor of ``SETCCCARRY``.

* TableGen now supports multi-alternative pattern fragments via the ``PatFrags``
  class.  ``PatFrag`` is now derived from ``PatFrags``, which may require minor
  changes to backends that directly access ``PatFrag`` members.

>>>>>>> release/7.x

Zig Programming Language
------------------------

<<<<<<< HEAD
`Zig <https://ziglang.org>`_  is a system programming language intended to be
an alternative to C. It provides high level features such as generics, compile
time function execution, and partial evaluation, while exposing low level LLVM
IR features such as aliases and intrinsics. Zig uses Clang to provide automatic
import of .h symbols, including inline functions and simple macros. Zig uses
LLD combined with lazily building compiler-rt to provide out-of-the-box
cross-compiling for all supported targets.
=======
Zig Programming Language
------------------------

`Zig <https://ziglang.org>`_  is an open-source programming language designed
for robustness, optimality, and clarity. Zig is an alternative to C, providing
high level features such as generics, compile time function execution, partial
evaluation, and LLVM-based coroutines, while exposing low level LLVM IR
features such as aliases and intrinsics. Zig uses Clang to provide automatic
import of .h symbols - even inline functions and macros. Zig uses LLD combined
with lazily building compiler-rt to provide out-of-the-box cross-compiling for
all supported targets.
>>>>>>> release/7.x
=======
>>>>>>> origin/release/5.x


Additional Information
======================

A wide variety of additional information is available on the `LLVM web page
<https://llvm.org/>`_, in particular in the `documentation
<https://llvm.org/docs/>`_ section.  The web page also contains versions of the
API documentation which is up-to-date with the Subversion version of the source
code.  You can access versions of these documents specific to this release by
going into the ``llvm/docs/`` directory in the LLVM tree.

If you have any questions or comments about LLVM, please feel free to contact
us via the `mailing lists <https://llvm.org/docs/#mailing-lists>`_.
