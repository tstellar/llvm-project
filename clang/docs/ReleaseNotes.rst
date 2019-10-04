<<<<<<< HEAD
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
=======
=========================
Clang 5.0.0 Release Notes
=========================
>>>>>>> origin/release/5.x

.. contents::
   :local:
   :depth: 2

Written by the `LLVM Team <https://llvm.org/>`_
<<<<<<< HEAD

<<<<<<< HEAD
<<<<<<< HEAD
.. warning::

   These are in-progress notes for the upcoming Clang 10 release.
   Release notes for previous releases can be found on
   `the Download Page <https://releases.llvm.org/download.html>`_.
=======
>>>>>>> release/7.x

=======
>>>>>>> origin/release/5.x
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
<<<<<<< HEAD
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
=======
latest release, please see the `Clang Web Site <http://clang.llvm.org>`_ or the
`LLVM Web Site <http://llvm.org>`_.

What's New in Clang 5.0.0?
>>>>>>> origin/release/5.x
==========================
>>>>>>> release/8.x

Some of the major new features and improvements to Clang are listed
here. Generic improvements to Clang as a whole or to its underlying
infrastructure are described first, followed by language-specific
sections with improvements to Clang's support for those languages.

Major New Features
------------------

<<<<<<< HEAD
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
=======
C++ coroutines
^^^^^^^^^^^^^^
`C++ coroutines TS
<http://open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4680.pdf>`_
implementation has landed. Use ``-fcoroutines-ts -stdlib=libc++`` to enable
coroutine support. Here is `an example
<https://wandbox.org/permlink/Dth1IO5q8Oe31ew2>`_ to get you started.

>>>>>>> origin/release/5.x

Improvements to Clang's diagnostics
-----------------------------------
>>>>>>> release/7.x

    * These options don't affect volatile stack variables.

<<<<<<< HEAD
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
=======
-  ``-Wcast-qual`` was implemented for C++. C-style casts are now properly
   diagnosed.

-  ``-Wunused-lambda-capture`` warns when a variable explicitly captured
   by a lambda is not used in the body of the lambda.

-  ``-Wstrict-prototypes`` is a new warning that warns about non-prototype
   function and block declarations and types in C and Objective-C.

-  ``-Wunguarded-availability`` is a new warning that warns about uses of new
   APIs that were introduced in a system whose version is newer than the
   deployment target version. A new Objective-C expression ``@available`` has
   been introduced to perform system version checking at runtime. This warning
   is off by default to prevent unexpected warnings in existing projects.
   However, its less strict sibling ``-Wunguarded-availability-new`` is on by
   default. It warns about unguarded uses of APIs only when they were introduced
   in or after macOS 10.13, iOS 11, tvOS 11 or watchOS 4.

-  The ``-Wdocumentation`` warning now allows the use of ``\param`` and
   ``\returns`` documentation directives in the documentation comments for
   declarations with a function or a block pointer type.

-  The compiler no longer warns about unreachable ``__builtin_unreachable``
   statements.

New Compiler Flags
------------------

- ``--autocomplete`` was implemented to obtain a list of flags and its arguments.
  This is used for shell autocompletion.
>>>>>>> origin/release/5.x

- When using a custom stack alignment, the ``stackrealign`` attribute is now
  implicitly set on the main function.

<<<<<<< HEAD
The following options are deprecated and ignored. They will be removed in
future versions of Clang.

<<<<<<< HEAD
- -mmpx used to enable the __MPX__ preprocessor define for the Intel MPX
  instructions. There were no MPX intrinsics.
- -mno-mpx used to disable -mmpx and is the default behavior.
=======
- ``-fslp-vectorize-aggressive`` used to enable the BB vectorizing pass. They have been superseeded
  by the normal SLP vectorizer.
- ``-fno-slp-vectorize-aggressive`` used to be the default behavior of clang.
>>>>>>> origin/release/5.x

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
=======
- Clang now supports the ``clang attribute`` pragma that allows users to apply
  an attribute to multiple declarations.

- ``pragma pack`` directives that are included in a precompiled header are now
  applied correctly to the declarations in the compilation unit that includes
  that precompiled header.
>>>>>>> origin/release/5.x

Attribute Changes in Clang
--------------------------

<<<<<<< HEAD
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
=======
-  The ``overloadable`` attribute now allows at most one function with a given
   name to lack the ``overloadable`` attribute. This unmarked function will not
   have its name mangled.
-  The ``ms_abi`` attribute and the ``__builtin_ms_va_list`` types and builtins
   are now supported on AArch64.
>>>>>>> origin/release/5.x

C Language Changes in Clang
---------------------------

<<<<<<< HEAD
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
=======
- Added near complete support for implicit scalar to vector conversion, a GNU
  C/C++ language extension. With this extension, the following code is
  considered valid:

.. code-block:: c

    typedef unsigned v4i32 __attribute__((vector_size(16)));

    v4i32 foo(v4i32 a) {
      // Here 5 is implicitly casted to an unsigned value and replicated into a
      // vector with as many elements as 'a'.
      return a + 5;
    }

The implicit conversion of a scalar value to a vector value--in the context of
a vector expression--occurs when:

- The type of the vector is that of a ``__attribute__((vector_size(size)))``
  vector, not an OpenCL ``__attribute__((ext_vector_type(size)))`` vector type.

- The scalar value can be casted to that of the vector element's type without
  the loss of precision based on the type of the scalar and the type of the
  vector's elements.

- For compile time constant values, the above rule is weakened to consider the
  value of the scalar constant rather than the constant's type. However,
  for compatibility with GCC, floating point constants with precise integral
  representations are not implicitly converted to integer values.

Currently the basic integer and floating point types with the following
operators are supported: ``+``, ``/``, ``-``, ``*``, ``%``, ``>``, ``<``,
``>=``, ``<=``, ``==``, ``!=``, ``&``, ``|``, ``^`` and the corresponding
assignment operators where applicable.

>>>>>>> origin/release/5.x

- Allow zero assignment and comparisons between variables of ``queue_t`` type.

<<<<<<< HEAD
- ...
=======
- Improved diagnostics of formatting specifiers and argument promotions for
  vector types in ``printf``.
>>>>>>> release/8.x

<<<<<<< HEAD
- Fixed return type of enqueued kernel and pipe builtins.

- Fixed address space of ``clk_event_t`` generated in the IR.

- Fixed address space when passing/returning structs.
=======
- We expect this to be the last Clang release that defaults to ``-std=gnu++98``
  when using the GCC-compatible ``clang++`` driver. From Clang 6 onwards we
  expect to use ``-std=gnu++14`` or a later standard by default, to match the
  behavior of recent GCC releases. Users are encouraged to change their build
  files to explicitly specify their desired C++ standard.

- Support for the C++17 standard has been completed. This mode can be enabled
  using ``-std=c++17`` (the old flag ``-std=c++1z`` is still supported for
  compatibility).

- When targeting a platform that uses the Itanium C++ ABI, Clang implements a
  `recent change to the ABI`__ that passes objects of class type indirectly if they
  have a non-trivial move constructor. Previous versions of Clang only
  considered the copy constructor, resulting in an ABI change in rare cases,
  but GCC has already implemented this change for several releases.
  This affects all targets other than Windows and PS4. You can opt out of this
  ABI change with ``-fclang-abi-compat=4.0``.

- As mentioned in `C Language Changes in Clang`_, Clang's support for
  implicit scalar to vector conversions also applies to C++. Additionally
  the following operators are also supported: ``&&`` and ``||``.

.. __: https://github.com/itanium-cxx-abi/cxx-abi/commit/7099637aba11fed6bdad7ee65bf4fd3f97fbf076
>>>>>>> origin/release/5.x

Header file fixes:

- Added missing extension guards around several builtin function overloads.

- Fixed serialization support when registering vendor extensions using pragmas.
=======
Objective-C Language Changes in Clang
-------------------------------------

<<<<<<< HEAD
Clang now supports the GNUstep Objective-C ABI v2 on ELF platforms.  This is
enabled with the ``-fobjc-runtime=gnustep-2.0`` flag.  The new ABI is incompatible
with the older GNUstep ABIs, which were incremental changes on the old GCC ABI.
The new ABI provides richer reflection metadata and allows the linker to remove
duplicate selector and protocol definitions, giving smaller binaries.  Windows
support for the new ABI is underway, but was not completed in time for the LLVM
7.0.0 release.
=======
- Clang now guarantees that a ``readwrite`` property is synthesized when an
  ambiguous property (i.e. a property that's declared in multiple protocols)
  is synthesized. The ``-Wprotocol-property-synthesis-ambiguity`` warning that
  warns about incompatible property types is now promoted to an error when
  there's an ambiguity between ``readwrite`` and ``readonly`` properties.

- Clang now prohibits synthesis of ambiguous properties with incompatible
  explicit property attributes. The following property attributes are
  checked for differences: ``copy``, ``retain``/``strong``, ``atomic``,
  ``getter`` and ``setter``.
>>>>>>> origin/release/5.x

OpenCL C/C++ Language Changes in Clang
--------------------------------------

Miscellaneous changes in OpenCL C:

- Added ``cles_khr_int64`` extension.

- Added bug fixes and simplifications to Clang blocks in OpenCL mode.

- Added compiler flag ``-cl-uniform-work-group-size`` to allow extra compile time optimisation.

- Propagate ``denorms-are-zero`` attribute to IR if ``-cl-denorms-are-zero`` is passed to the compiler.

- Separated ``read_only`` and ``write_only`` pipe IR types.

<<<<<<< HEAD
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
=======
Various bug fixes and improvements:

-  Extended OpenCL-related Clang tests.

-  Improved diagnostics across several areas: scoped address space
   qualified variables, function pointers, atomics, type rank for overloading,
   block captures, ``reserve_id_t``.
>>>>>>> origin/release/5.x

-  Several address space related fixes for constant address space function scope variables,
   IR generation, mangling of ``generic`` and alloca (post-fix from general Clang
   refactoring of address spaces).

<<<<<<< HEAD
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
=======
-  Several improvements in extensions: fixed OpenCL version for ``cl_khr_mipmap_image``,
   added missing ``cl_khr_3d_image_writes``.

-  Improvements in ``enqueue_kernel``, especially the implementation of ``ndrange_t`` and blocks.

-  OpenCL type related fixes: global samplers, the ``pipe_t`` size, internal type redefinition,
   and type compatibility checking in ternary and other operations.

-  The OpenCL header has been extended with missing extension guards, and direct mapping of ``as_type``
   to ``__builtin_astype``.

-  Fixed ``kernel_arg_type_qual`` and OpenCL/SPIR version in metadata.

-  Added proper use of the kernel calling convention to various targets.

The following new functionalities have been added:

-  Added documentation on OpenCL to Clang user manual.

-  Extended Clang builtins with required ``cl_khr_subgroups`` support.

-  Add ``intel_reqd_sub_group_size`` attribute support.

-  Added OpenCL types to ``CIndex``.
>>>>>>> origin/release/5.x

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

<<<<<<< HEAD
- Added support for the reductions across the teams.
=======
  +---------------------+---------------------+
  | Before              | After               |
  +=====================+=====================+
  | .. code-block:: c++ | .. code-block:: c++ |
  |                     |                     |
  |   namespace A {     |   namespace A {     |
  |   int i;            |   int i;            |
  |   int j;            |   int j;            |
  |   }                 |   } // namespace A  |
  +---------------------+---------------------+
>>>>>>> origin/release/5.x

- Extended number of constructs that can be executed in SPMD mode.

- Fixed support for lastprivate/reduction variables in SPMD constructs.

<<<<<<< HEAD
- New collapse clause scheme to avoid expensive remainder operations.
>>>>>>> release/8.x

- New default schedule for distribute and parallel constructs.
=======
- Libclang now provides code-completion results for more C++ constructs
  and keywords. The following keywords/identifiers are now included in the
  code-completion results: ``static_assert``, ``alignas``, ``constexpr``,
  ``final``, ``noexcept``, ``override`` and ``thread_local``.

- Libclang now provides code-completion results for members from dependent
  classes. For example:

  .. code-block:: c++

    template<typename T>
    void appendValue(std::vector<T> &dest, const T &value) {
        dest. // Relevant completion results are now shown after '.'
    }

  Note that code-completion results are still not provided when the member
  expression includes a dependent base expression. For example:

  .. code-block:: c++

    template<typename T>
    void appendValue(std::vector<std::vector<T>> &dest, const T &value) {
        dest.at(0). // Libclang fails to provide completion results after '.'
    }
>>>>>>> origin/release/5.x

- Simplified code generation for distribute and parallel in SPMD mode.

<<<<<<< HEAD
- Flag (``-fopenmp_optimistic_collapse``) for user to limit collapsed
  loop counter width when safe to do so.
=======
- The static analyzer now supports using the
  `z3 theorem prover <https://github.com/z3prover/z3>`_ from Microsoft Research
  as an external constraint solver. This allows reasoning over more complex
  queries, but performance is ~15x slower than the default range-based
  constraint solver. To enable the z3 solver backend, clang must be built with
  the ``CLANG_ANALYZER_BUILD_Z3=ON`` option, and the
  ``-Xanalyzer -analyzer-constraints=z3`` arguments passed at runtime.
>>>>>>> origin/release/5.x

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
<<<<<<< HEAD
libclang
--------
=======
- The Undefined Behavior Sanitizer has a new check for pointer overflow. This
  check is on by default. The flag to control this functionality is
  ``-fsanitize=pointer-overflow``.
>>>>>>> origin/release/5.x

- ...

<<<<<<< HEAD
=======
- UBSan has several new checks which detect violations of nullability
  annotations. These checks are off by default. The flag to control this group
  of checks is ``-fsanitize=nullability``. The checks can be individially enabled
  by ``-fsanitize=nullability-arg`` (which checks calls),
  ``-fsanitize=nullability-assign`` (which checks assignments), and
  ``-fsanitize=nullability-return`` (which checks return statements).
>>>>>>> origin/release/5.x

Static Analyzer
---------------

- ...
=======
- General performance improvement.

<<<<<<< HEAD
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
=======

Python Binding Changes
----------------------

Python bindings now support both Python 2 and Python 3.

The following methods have been added:

- ``is_scoped_enum`` has been added to ``Cursor``.

- ``exception_specification_kind`` has been added to ``Cursor``.

- ``get_address_space`` has been added to ``Type``.

- ``get_typedef_name`` has been added to ``Type``.

- ``get_exception_specification_kind`` has been added to ``Type``.

>>>>>>> origin/release/5.x

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
