<<<<<<< HEAD
<<<<<<< HEAD
========================================
Clang 10.0.0 (In-Progress) Release Notes
========================================
=======
=========================
Clang 8.0.0 Release Notes
=========================
>>>>>>> release/8.x
=======
=========================
Clang 7.0.0 Release Notes
=========================
>>>>>>> release/7.x

.. contents::
   :local:
   :depth: 2

Written by the `LLVM Team <https://llvm.org/>`_
<<<<<<< HEAD

<<<<<<< HEAD
.. warning::

   These are in-progress notes for the upcoming Clang 10 release.
   Release notes for previous releases can be found on
   `the Download Page <https://releases.llvm.org/download.html>`_.
=======
>>>>>>> release/7.x

Introduction
============

This document contains the release notes for the Clang C/C++/Objective-C
frontend, part of the LLVM Compiler Infrastructure, release 10.0.0. Here we
=======
Introduction
============

This document contains the release notes for the Clang C/C++/Objective-C/OpenCL
frontend, part of the LLVM Compiler Infrastructure, release 8.0.0. Here we
>>>>>>> release/8.x
describe the status of Clang in some detail, including major
improvements from the previous release and new feature work. For the
general LLVM release notes, see `the LLVM
documentation <https://llvm.org/docs/ReleaseNotes.html>`_. All LLVM
<<<<<<< HEAD
releases may be downloaded
from the `LLVM releases web site <https://releases.llvm.org/>`_.
=======
releases may be downloaded from the `LLVM releases web
site <https://llvm.org/releases/>`_.
>>>>>>> release/7.x

For more information about Clang or LLVM, including information about the
latest release, please see the `Clang Web Site <https://clang.llvm.org>`_ or the
`LLVM Web Site <https://llvm.org>`_.
<<<<<<< HEAD

<<<<<<< HEAD
Note that if you are reading this file from a Subversion checkout or the
main Clang web page, this document applies to the *next* release, not
the current one. To see the release notes for a specific release, please
see the `releases page <https://llvm.org/releases/>`_.
=======
>>>>>>> release/7.x

What's New in Clang 10.0.0?
===========================
=======
What's New in Clang 8.0.0?
==========================
>>>>>>> release/8.x

Some of the major new features and improvements to Clang are listed
here. Generic improvements to Clang as a whole or to its underlying
infrastructure are described first, followed by language-specific
sections with improvements to Clang's support for those languages.

Major New Features
------------------

<<<<<<< HEAD
- ...
=======
- Clang supports use of a profile remapping file, which permits
  profile data captured for one version of a program to be applied
  when building another version where symbols have changed (for
  example, due to renaming a class or namespace).
  See the :ref:`UsersManual <profile_remapping>` for details.

<<<<<<< HEAD
- Clang has new options to initialize automatic variables with a pattern. The default is still that automatic variables are uninitialized. This isn't meant to change the semantics of C and C++. Rather, it's meant to be a last resort when programmers inadvertently have some undefined behavior in their code. These options aim to make undefined behavior hurt less, which security-minded people will be very happy about. Notably, this means that there's no inadvertent information leak when:

    * The compiler re-uses stack slots, and a value is used uninitialized.

    * The compiler re-uses a register, and a value is used uninitialized.

    * Stack structs / arrays / unions with padding are copied.

  These options only address stack and register information leaks.

  Caveats:

    * Variables declared in unreachable code and used later aren't initialized. This affects goto statements, Duff's device, and other objectionable uses of switch statements. This should instead be a hard-error in any serious codebase.
=======
- Preliminary/experimental support for DWARF v5 debugging information. If you
  compile with ``-gdwarf-5 -O0`` you should get fully conforming DWARF v5
  information, including the new .debug_names accelerator table. Type units
  and split DWARF are known not to conform, and higher optimization levels
  will likely get a mix of v4 and v5 formats.

Improvements to Clang's diagnostics
-----------------------------------
>>>>>>> release/7.x

    * These options don't affect volatile stack variables.

    * Padding isn't fully handled yet.

  How to use it on the command line:

    * ``-ftrivial-auto-var-init=uninitialized`` (the default)

    * ``-ftrivial-auto-var-init=pattern``

  There is also a new attribute to request a variable to not be initialized, mainly to disable initialization of large stack arrays when deemed too expensive:

    * ``int dont_initialize_me __attribute((uninitialized));``

>>>>>>> release/8.x

Improvements to Clang's diagnostics
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- -Wtautological-overlap-compare will warn on negative numbers and non-int
  types.
- -Wtautological-compare for self comparisons and
  -Wtautological-overlap-compare will now look through member and array
  access to determine if two operand expressions are the same.

Non-comprehensive list of changes in this release
-------------------------------------------------

<<<<<<< HEAD
- For X86 target, -march=skylake-avx512, -march=icelake-client,
  -march=icelake-server, -march=cascadelake, -march=cooperlake will default to
  not using 512-bit zmm registers in vectorized code unless 512-bit intrinsics
  are used in the source code. 512-bit operations are known to cause the CPUs
  to run at a lower frequency which can impact performance. This behavior can be
  changed by passing -mprefer-vector-width=512 on the command line.
=======
- Clang binary and libraries have been renamed from 7.0 to 7.
  For example, the ``clang`` binary will be called ``clang-7``
  instead of ``clang-7.0``.

- The optimization flag to merge constants (``-fmerge-all-constants``) is no
  longer applied by default.

- Clang implements a collection of recent fixes to the C++ standard's definition
  of "standard-layout". In particular, a class is only considered to be
  standard-layout if all base classes and the first data member (or bit-field)
  can be laid out at offset zero.

- Clang's handling of the GCC ``packed`` class attribute in C++ has been fixed
  to apply only to non-static data members and not to base classes. This fixes
  an ABI difference between Clang and GCC, but creates an ABI difference between
  Clang 7 and earlier versions. The old behavior can be restored by setting
  ``-fclang-abi-compat`` to ``6`` or lower.

- Clang implements the proposed resolution of LWG issue 2358, along with the
  `corresponding change to the Itanium C++ ABI
  <https://github.com/itanium-cxx-abi/cxx-abi/pull/51>`_, which make classes
  containing only unnamed non-zero-length bit-fields be considered non-empty.
  This is an ABI break compared to prior Clang releases, but makes Clang
  generate code that is ABI-compatible with other compilers. The old
  behavior can be restored by setting ``-fclang-abi-compat`` to ``6`` or
  lower.

- An existing tool named ``diagtool`` has been added to the release. As the
  name suggests, it helps with dealing with diagnostics in ``clang``, such as
  finding out the warning hierarchy, and which of them are enabled by default
  or for a particular compiler invocation.

- By default, Clang emits an address-significance table into
  every ELF object file when using the integrated assembler.
  Address-significance tables allow linkers to implement `safe ICF
  <https://research.google.com/pubs/archive/36912.pdf>`_ without the false
  positives that can result from other implementation techniques such as
  relocation scanning. The ``-faddrsig`` and ``-fno-addrsig`` flags can be
  used to control whether to emit the address-significance table.

- The integrated assembler is enabled by default on OpenBSD / FreeBSD
  for MIPS 64-bit targets.

- On MIPS FreeBSD, default CPUs have been changed to ``mips2``
  for 32-bit targets and ``mips3`` for 64-bit targets.

>>>>>>> release/7.x

- The integrated assembler is used now by default for all MIPS targets.

- Improved support for MIPS N32 ABI and MIPS R6 target triples.

<<<<<<< HEAD
- Clang now includes builtin functions for bitwise rotation of common value
  sizes, such as: `__builtin_rotateleft32
  <LanguageExtensions.html#builtin-rotateleft>`_
=======
  When converting a floating-point value to int and the value is not
  representable in the destination integer type,
  the code has undefined behavior according to the language standard. By
  default, Clang will not guarantee any particular result in that case. With the
  'no-strict' option, Clang attempts to match the overflowing behavior of the
  target's native float-to-int conversion instructions.
>>>>>>> release/7.x

- Improved optimization for the corresponding MSVC compatibility builtins such
  as ``_rotl()``.

<<<<<<< HEAD
New Compiler Flags
------------------

<<<<<<< HEAD
- ...
=======
- ``-mspeculative-load-hardening`` Clang now has an option to enable
  Speculative Load Hardening.

- ``-fprofile-filter-files=[regexes]`` and ``-fprofile-exclude-files=[regexes]``.

  Clang has now options to filter or exclude some files when
  instrumenting for gcov-based profiling.
  See the `UsersManual <UsersManual.html#cmdoption-fprofile-filter-files>`_ for details.
>>>>>>> release/8.x

- When using a custom stack alignment, the ``stackrealign`` attribute is now
  implicitly set on the main function.

<<<<<<< HEAD
The following options are deprecated and ignored. They will be removed in
future versions of Clang.

- -mmpx used to enable the __MPX__ preprocessor define for the Intel MPX
  instructions. There were no MPX intrinsics.
- -mno-mpx used to disable -mmpx and is the default behavior.

- ...
=======
- Emission of ``R_MIPS_JALR`` and ``R_MICROMIPS_JALR`` relocations can now
  be controlled by the ``-mrelax-pic-calls`` and ``-mno-relax-pic-calls``
  options.
>>>>>>> release/8.x
=======
  In order to improve devirtualization, forces emission of vtables even in
  modules where it isn't necessary. It causes more inline virtual functions
  to be emitted.

- Added the ``-mcrc`` and ``-mno-crc`` flags to enable/disable using
  of MIPS Cyclic Redundancy Check instructions.

- Added the ``-mvirt`` and ``-mno-virt`` flags to enable/disable using
  of MIPS Virtualization instructions.

- Added the ``-mginv`` and ``-mno-ginv`` flags to enable/disable using
  of MIPS Global INValidate instructions.

>>>>>>> release/7.x

Modified Compiler Flags
-----------------------

<<<<<<< HEAD
<<<<<<< HEAD
- ...
=======
- As of clang 8, ``alignof`` and ``_Alignof`` return the ABI alignment of a type,
  as opposed to the preferred alignment. ``__alignof`` still returns the
  preferred alignment. ``-fclang-abi-compat=7`` (and previous) will make
  ``alignof`` and ``_Alignof`` return preferred alignment again.

>>>>>>> release/8.x

New Pragmas in Clang
--------------------

- ...
=======
- Before Clang 7, we prepended the `#` character to the ``--autocomplete``
  argument to enable cc1 flags. For example, when the ``-cc1`` or ``-Xclang`` flag
  is in the :program:`clang` invocation, the shell executed
  ``clang --autocomplete=#-<flag to be completed>``. Clang 7 now requires the
  whole invocation including all flags to be passed to the ``--autocomplete`` like
  this: ``clang --autocomplete=-cc1,-xc++,-fsyn``.

>>>>>>> release/7.x

Attribute Changes in Clang
--------------------------

<<<<<<< HEAD
* Clang now supports enabling/disabling speculative load hardening on a
  per-function basis using the function attribute
  ``speculative_load_hardening``/``no_speculative_load_hardening``.
=======
- Clang now supports function multiversioning with attribute 'target' on ELF
  based x86/x86-64 environments by using indirect functions. This implementation
  has a few minor limitations over the GCC implementation for the sake of AST
  sanity, however it is otherwise compatible with existing code using this
  feature for GCC. Consult the `documentation for the target attribute
  <AttributeReference.html#target-gnu-target>`_ for more information.
>>>>>>> release/7.x

Windows Support
---------------

<<<<<<< HEAD
- Previous Clang versions contained a work-around to avoid an issue with the
  standard library headers in Visual Studio 2019 versions prior to 16.3. This
  work-around has now been removed, and users of Visual Studio 2019 are
  encouraged to upgrade to 16.3 or later, otherwise they may see link errors as
  below:

  .. code-block:: console

<<<<<<< HEAD
    error LNK2005: "bool const std::_Is_integral<int>" (??$_Is_integral@H@std@@3_NB) already defined

=======
   - The ``/Ycfoo.h`` and ``/Yufoo.h`` flags can now be used without ``/FIfoo.h`` when
     foo.h is instead included by an explicit ``#include`` directive. This means
     Visual Studio's default stdafx.h setup now uses precompiled headers with
     clang-cl.

- The alternative entry point names
  (``wmain``/``WinMain``/``wWinMain``/``DllMain``) now are properly mangled
  as plain C names in C++ contexts when targeting MinGW, without having to
  explicit specify ``extern "C"``. (This was already the case for MSVC
  targets.)
>>>>>>> release/7.x

=======
- clang-cl now supports the use of the precompiled header options ``/Yc`` and ``/Yu``
  without the filename argument. When these options are used without the
  filename, a `#pragma hdrstop` inside the source marks the end of the
  precompiled code.

<<<<<<< HEAD
- clang-cl has a new command-line option, ``/Zc:dllexportInlines-``, similar to
  ``-fvisibility-inlines-hidden`` on non-Windows, that makes class-level
  `dllexport` and `dllimport` attributes not apply to inline member functions.
  This can significantly reduce compile and link times. See the `User's Manual
  <UsersManual.html#the-zc-dllexportinlines-option>`_ for more info.
>>>>>>> release/8.x

- For MinGW, ``-municode`` now correctly defines ``UNICODE`` during
  preprocessing.

- For MinGW, clang now produces vtables and RTTI for dllexported classes
  without key functions. This fixes building Qt in debug mode.

- Allow using Address Sanitizer and Undefined Behaviour Sanitizer on MinGW.

<<<<<<< HEAD
C11 Feature Support
^^^^^^^^^^^^^^^^^^^
=======
- Structured Exception Handling support for ARM64 Windows. The ARM64 Windows
  target is in pretty good shape now.

>>>>>>> release/8.x

OpenCL Kernel Language Changes in Clang
---------------------------------------

Misc:

<<<<<<< HEAD
- The behaviour of the `gnu_inline` attribute now matches GCC, for cases
  where used without the `extern` keyword. As this is a change compared to
  how it behaved in previous Clang versions, a warning is emitted for this
  combination.
=======
- Improved address space support with Clang builtins.
>>>>>>> release/8.x

- Improved various diagnostics for vectors with element types from extensions;
  values used in attributes; duplicate address spaces.

- Allow blocks to capture arrays.

- Allow zero assignment and comparisons between variables of ``queue_t`` type.

<<<<<<< HEAD
- ...
=======
- Improved diagnostics of formatting specifiers and argument promotions for
  vector types in ``printf``.
>>>>>>> release/8.x

- Fixed return type of enqueued kernel and pipe builtins.

- Fixed address space of ``clk_event_t`` generated in the IR.

- Fixed address space when passing/returning structs.

Header file fixes:

- Added missing extension guards around several builtin function overloads.

- Fixed serialization support when registering vendor extensions using pragmas.
=======
Objective-C Language Changes in Clang
-------------------------------------

Clang now supports the GNUstep Objective-C ABI v2 on ELF platforms.  This is
enabled with the ``-fobjc-runtime=gnustep-2.0`` flag.  The new ABI is incompatible
with the older GNUstep ABIs, which were incremental changes on the old GCC ABI.
The new ABI provides richer reflection metadata and allows the linker to remove
duplicate selector and protocol definitions, giving smaller binaries.  Windows
support for the new ABI is underway, but was not completed in time for the LLVM
7.0.0 release.

OpenCL C/C++ Language Changes in Clang
--------------------------------------

Miscellaneous changes in OpenCL C:

- Added ``cles_khr_int64`` extension.

- Added bug fixes and simplifications to Clang blocks in OpenCL mode.

- Added compiler flag ``-cl-uniform-work-group-size`` to allow extra compile time optimisation.

- Propagate ``denorms-are-zero`` attribute to IR if ``-cl-denorms-are-zero`` is passed to the compiler.

- Separated ``read_only`` and ``write_only`` pipe IR types.

- Fixed address space for the ``__func__`` predefined macro.

- Improved diagnostics of kernel argument types.


Started OpenCL C++ support:

- Added ``-std/-cl-std=c++``.

- Added support for keywords.
>>>>>>> release/7.x

- Fixed OpenCL version in declarations of builtin functions with sampler-less
  image accesses.

New vendor extensions added:

- ``cl_intel_planar_yuv``

- ``cl_intel_device_side_avc_motion_estimation``


C++ for OpenCL:

<<<<<<< HEAD
- Added support of address space conversions in C style casts.

- Enabled address spaces for references.
=======
- Clang gained basic support for OpenMP 4.5 offloading for NVPTX target.

  To compile your program for NVPTX target use the following options:
  ``-fopenmp -fopenmp-targets=nvptx64-nvidia-cuda`` for 64 bit platforms or
  ``-fopenmp -fopenmp-targets=nvptx-nvidia-cuda`` for 32 bit platform.

- Passing options to the OpenMP device offloading toolchain can be done using
  the ``-Xopenmp-target=<triple> -opt=val`` flag. In this way the ``-opt=val``
  option will be forwarded to the respective OpenMP device offloading toolchain
  described by the triple. For example passing the compute capability to
  the OpenMP NVPTX offloading toolchain can be done as follows:
  ``-Xopenmp-target=nvptx64-nvidia-cuda -march=sm_60``. For the case when only one
  target offload toolchain is specified under the ``-fopenmp-targets=<triples>``
  option, then the triple can be skipped: ``-Xopenmp-target -march=sm_60``.
>>>>>>> release/7.x

- Fixed use of address spaces in templates: address space deduction and diagnostics.

- Changed default address space to work with C++ specific concepts: class members,
  template parameters, etc.

- Added generic address space by default to the generated hidden 'this' parameter.

- Extend overload ranking rules for address spaces.


ABI Changes in Clang
--------------------

<<<<<<< HEAD
- gcc passes vectors of __int128 in memory on X86-64. Clang historically
  broke the vectors into multiple scalars using two 64-bit values for each
  element. Clang now matches the gcc behavior on Linux and NetBSD. You can
  switch back to old API behavior with flag: -fclang-abi-compat=9.0.

OpenMP Support in Clang
-----------------------

- ...

CUDA Support in Clang
---------------------

<<<<<<< HEAD
- ...
=======
- Clang will now try to locate the CUDA installation next to :program:`ptxas`
  in the `PATH` environment variable. This behavior can be turned off by passing
  the new flag ``--cuda-path-ignore-env``.

- Clang now supports generating object files with relocatable device code. This
  feature needs to be enabled with ``-fcuda-rdc`` and may result in performance
  penalties compared to whole program compilation. Please note that NVIDIA's
  :program:`nvcc` must be used for linking.
>>>>>>> release/7.x

Internal API Changes
--------------------

These are major API changes that have happened since the 9.0.0 release of
Clang. If upgrading an external codebase that uses Clang as a library,
this section should help get you past the largest hurdles of upgrading.

<<<<<<< HEAD
- libTooling APIs that transfer ownership of `FrontendAction` objects now pass
  them by `unique_ptr`, making the ownership transfer obvious in the type
  system. `FrontendActionFactory::create()` now returns a
  `unique_ptr<FrontendAction>`. `runToolOnCode`, `runToolOnCodeWithArgs`,
  `ToolInvocation::ToolInvocation()` now take a `unique_ptr<FrontendAction>`.

Build System Changes
--------------------

These are major changes to the build system that have happened since the 9.0.0
release of Clang. Users of the build system should adjust accordingly.
=======
- The methods ``getLocStart``, ``getStartLoc`` and ``getLocEnd`` in the AST
  classes are deprecated.  New APIs ``getBeginLoc`` and ``getEndLoc`` should
  be used instead.  While the old methods remain in this release, they will
  not be present in the next release of Clang.
>>>>>>> release/7.x

- In 8.0.0 and below, the install-clang-headers target would install clang's
  resource directory headers. This installation is now performed by the
  install-clang-resource-headers target. Users of the old install-clang-headers
  target should switch to the new install-clang-resource-headers target. The
  install-clang-headers target now installs clang's API headers (corresponding
  to its libraries), which is consistent with the install-llvm-headers target.

<<<<<<< HEAD
- In 9.0.0 and later Clang added a new target, clang-cpp, which generates a
  shared library comprised of all the clang component libraries and exporting
  the clang C++ APIs. Additionally the build system gained the new
  "CLANG_LINK_CLANG_DYLIB" option, which defaults Off, and when set to On, will
  force clang (and clang-based tools) to link the clang-cpp library instead of
  statically linking clang's components. This option will reduce the size of
  binary distributions at the expense of compiler performance.

- ...
=======
- ``_Alignof`` and ``alignof`` now return the ABI alignment of a type, as opposed
  to the preferred alignment.

  - This is more in keeping with the language of the standards, as well as
    being compatible with gcc
  - ``__alignof`` and ``__alignof__`` still return the preferred alignment of
    a type
  - This shouldn't break any ABI except for things that explicitly ask for
    ``alignas(alignof(T))``.
  - If you have interfaces that break with this change, you may wish to switch
    to ``alignas(__alignof(T))``, instead of using the ``-fclang-abi-compat``
    switch.

OpenMP Support in Clang
----------------------------------

- OpenMP 5.0 features

  - Support relational-op != (not-equal) as one of the canonical forms of random
    access iterator.
  - Added support for mapping of the lambdas in target regions.
  - Added parsing/sema analysis for the requires directive.
  - Support nested declare target directives.
  - Make the `this` pointer implicitly mapped as `map(this[:1])`.
  - Added the `close` *map-type-modifier*.

- Various bugfixes and improvements.
=======
- Clang-format will now support detecting and formatting code snippets in raw
  string literals.  This is configured through the ``RawStringFormats`` style
  option.

Static Analyzer
---------------

- The new `MmapWriteExec` checker had been introduced to detect attempts to map pages both writable and executable.
>>>>>>> release/7.x

New features supported for Cuda devices:

- Added support for the reductions across the teams.

- Extended number of constructs that can be executed in SPMD mode.

- Fixed support for lastprivate/reduction variables in SPMD constructs.

- New collapse clause scheme to avoid expensive remainder operations.
>>>>>>> release/8.x

- New default schedule for distribute and parallel constructs.

- Simplified code generation for distribute and parallel in SPMD mode.

- Flag (``-fopenmp_optimistic_collapse``) for user to limit collapsed
  loop counter width when safe to do so.

<<<<<<< HEAD
- The ``Standard`` style option specifies which version of C++ should be used
  when parsing and formatting C++ code. The set of allowed values has changed:
  - ``Latest`` will always enable new C++ language features.
  - ``c++03``, ``c++11``, ``c++14``, ``c++17``, ``c++20`` will pin to exactly
    that language version.
  - ``Auto`` is the default and detects style from the code (this is unchanged).
 The previous values of ``Cpp03`` and ``Cpp11`` are deprecated. Note that
 ``Cpp11`` is treated as ``Latest``, as this was always clang-format's behavior.
 (One motivation for this change is the new name describes the behavior better).

<<<<<<< HEAD
libclang
--------

- ...


Static Analyzer
---------------

- ...
=======
- General performance improvement.

>>>>>>> release/8.x

.. _release-notes-ubsan:

Undefined Behavior Sanitizer (UBSan)
------------------------------------

- ...

=======

libc++ Changes
==============
Users that wish to link together translation units built with different
versions of libc++'s headers into the same final linked image should define the
`_LIBCPP_HIDE_FROM_ABI_PER_TU` macro to `1` when building those translation
units. In a future release, not defining `_LIBCPP_HIDE_FROM_ABI_PER_TU` to `1`
and linking translation units built with different versions of libc++'s headers
together may lead to ODR violations and ABI issues.

>>>>>>> release/7.x

Additional Information
======================

A wide variety of additional information is available on the `Clang web
page <https://clang.llvm.org/>`_. The web page contains versions of the
API documentation which are up-to-date with the Subversion version of
the source code. You can access versions of these documents specific to
this release by going into the "``clang/docs/``" directory in the Clang
tree.

If you have any questions or comments about Clang, please feel free to
contact us via the `mailing
list <https://lists.llvm.org/mailman/listinfo/cfe-dev>`_.
