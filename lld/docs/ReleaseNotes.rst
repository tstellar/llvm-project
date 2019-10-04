<<<<<<< HEAD
========================
lld 10.0.0 Release Notes
========================
=======
=======================
lld 5.0.0 Release Notes
=======================
>>>>>>> origin/release/5.x

.. contents::
    :local:

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
.. warning::
   These are in-progress notes for the upcoming LLVM 10.0.0 release.
   Release notes for previous releases can be found on
   `the Download Page <https://releases.llvm.org/download.html>`_.

Introduction
============

This document contains the release notes for the lld linker, release 10.0.0.
Here we describe the status of lld, including major improvements
from the previous release. All lld releases may be downloaded
from the `LLVM releases web site <https://llvm.org/releases/>`_.
=======
Introduction
============

lld is a high-performance linker that supports ELF (Unix), COFF (Windows),
Mach-O (macOS), MinGW and WebAssembly. lld is command-line-compatible with
GNU linkers and Microsoft link.exe and is significantly faster than the
system default linkers.

lld 8.0.0 has lots of feature improvements and bug fixes.
>>>>>>> release/8.x
=======
Introduction
============

lld is a high-performance linker that supports ELF (Unix), COFF (Windows),
Mach-O (macOS), MinGW and WebAssembly. lld is command-line-compatible with GNU
linkers and Microsoft link.exe, and is significantly faster than the system
default linkers.

lld 7 for ELF, COFF and MinGW are production-ready.

* lld/ELF can build the entire FreeBSD/{AMD64,ARMv7} and will be the default
  linker of the next version of the operating system.

* lld/COFF is being used to create official builds of large popular programs
  such as Chrome and Firefox.

* lld/MinGW is being used by Firefox for their MinGW builds. lld/MinGW still
  needs a sysroot specifically built for lld, with llvm-dlltool, though.

* lld/WebAssembly is used as the default (only) linker in Emscripten when using
  the upstream LLVM compiler.

* lld/Mach-O is still experimental.
>>>>>>> release/7.x
=======
Introduction
============

lld is a linker from the LLVM project. It supports ELF (Unix), COFF (Windows)
and Mach-O (macOS), and it is generally faster than the GNU bfd or gold linkers
or the MSVC linker.

lld is designed to be a drop-in replacement for the system linkers, so that
users don't need to change their build systems other than swapping the linker
command.

All lld releases may be downloaded from the `LLVM releases web site
<http://llvm.org/releases/>`_.
>>>>>>> origin/release/5.x

Non-comprehensive list of changes in this release
=================================================

ELF Improvements
----------------

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
* ...
=======
* lld now supports RISC-V. (`r339364
  <https://reviews.llvm.org/rL339364>`_)

* Default image base address has changed from 65536 to 2 MiB for i386
  and 4 MiB for AArch64 to make lld-generated executables work better
  with automatic superpage promotion. FreeBSD can promote contiguous
  non-superpages to a superpage if they are aligned to the superpage
  size. (`r342746 <https://reviews.llvm.org/rL342746>`_)

* lld now attempts to place a ``.note`` segment in the first page of a
  generated file, so that you can find some important information
  (``.note.gnu.build-id`` in particular) in a core file even if a core
  file is truncated by ulimit.
  (`r349524 <https://reviews.llvm.org/rL349524>`_)

* lld now reports an error if ``_GLOBAL_OFFSET_TABLE_`` symbol is
  defined by an input object file, as the symbol is supposed to be
  synthesized by the linker.
  (`r347854 <https://reviews.llvm.org/rL347854>`_)

* lld/Hexagon can now link Linux kernel and musl libc for Qualcomm
  Hexagon ISA.

* Initial MSP430 ISA support has landed.

* lld now uses the ``sigrie`` instruction as a trap instruction for
  MIPS targets.

* lld now creates a TLS segment for AArch64 with a slightly larger
  alignment requirement, so that the loader makes a few bytes room
  before each TLS segment at runtime. The aim of this change is to
  make room to accomodate nonstandard Android TLS slots while keeping
  the compatibility with the standard AArch64 ABI.
  (`r350681 <https://reviews.llvm.org/rL350681>`_)

* The following flags have been added: ``--call-graph-profile``,
  ``--no-call-graph-profile``, ``--warn-ifunc-textrel``,
  ``-z interpose``, ``-z global``, ``-z nodefaultlib``
>>>>>>> release/8.x
=======
* Fixed a lot of long-tail compatibility issues with GNU linkers.

* Added ``-z retpolineplt`` to emit a PLT entry that doesn't contain an indirect
  jump instruction to mitigate Spectre v2 vulnerability.

* Added experimental support for `SHT_RELR sections
  <https://groups.google.com/forum/#!topic/generic-abi/bX460iggiKg>`_ to create a
  compact dynamic relocation table.

* Added support for `split stacks <https://gcc.gnu.org/wiki/SplitStacks>`_.

* Added support for address significance table (section with type
  SHT_LLVM_ADDRSIG) to improve Identical Code Folding (ICF). Combined with the
  ``-faddrsig`` compiler option added to Clang 7, lld's ``--icf=all`` can now
  safely merge functions and data to generate smaller outputs than before.

* Improved ``--gdb-index`` so that it is faster (`r336790
  <https://reviews.llvm.org/rL336790>`_) and uses less memory (`r336672
  <https://reviews.llvm.org/rL336672>`_).

* Reduced memory usage of ``--compress-debug-sections`` (`r338913
  <https://reviews.llvm.org/rL338913>`_).

* Added linker script OVERLAY support (`r335714 <https://reviews.llvm.org/rL335714>`_).

* Added ``--warn-backref`` to make it easy to identify command line option order
  that doesn't work with GNU linkers (`r329636 <https://reviews.llvm.org/rL329636>`_)

* Added ld.lld.1 man page (`r324512 <https://reviews.llvm.org/rL324512>`_).

* Added support for multi-GOT.

* Added support for MIPS position-independent executable (PIE).

* Fixed MIPS TLS GOT entries for local symbols in shared libraries.

* Fixed calculation of MIPS GP relative relocations in case of relocatable
  output.

* Added support for PPCv2 ABI.

* Removed an incomplete support of PPCv1 ABI.

* Added support for Qualcomm Hexagon ISA.

* Added the following flags: ``--apply-dynamic-relocs``, ``--check-sections``,
  ``--cref``, ``--just-symbols``, ``--keep-unique``,
  ``--no-allow-multiple-definition``, ``--no-apply-dynamic-relocs``,
  ``--no-check-sections``, ``--no-gnu-unique, ``--no-pic-executable``,
  ``--no-undefined-version``, ``--no-warn-common``, ``--pack-dyn-relocs=relr``,
  ``--pop-state``, ``--print-icf-sections``, ``--push-state``,
  ``--thinlto-index-only``, ``--thinlto-object-suffix-replace``,
  ``--thinlto-prefix-replace``, ``--warn-backref``, ``-z combreloc``, ``-z
  copyreloc``, ``-z initfirst``, ``-z keep-text-section-prefix``, ``-z lazy``,
  ``-z noexecstack``, ``-z relro``, ``-z retpolineplt``, ``-z text``
>>>>>>> release/7.x
=======
* First and foremost, a lot of compatibility issues and bugs have been fixed.
  Linker script support has significantly improved. As a result, we believe you
  are very likely to be able to link your programs with lld without experiencing
  any problem now.

* Error message format has changed in order to improve readability.
  Traditionally, linker's error messages are concise and arguably too terse.
  This is an example of lld 4.0.0's error message (they are actually in one line)::

    /ssd/clang/bin/ld.lld: error: /ssd/llvm-project/lld/ELF/Writer.cpp:207:
      undefined symbol 'lld::elf::EhFrameSection::addSection()'

  It is not easy to read because too much information is packed into a single line
  and the embedded text, particularly a symbol name, is sometimes too long.
  In lld 5.0.0, we use more vertical space to print out error messages in a more
  structured manner like this::

    bin/ld.lld: error: undefined symbol: lld::elf::EhFrameSection::addSection()
    >>> Referenced by Writer.cpp:207 (/ssd/llvm-project/lld/ELF/Writer.cpp:207)
    >>>               Writer.cpp.o in archive lib/liblldELF.a

  As a bonus, the new error message contains source code location of the error
  if it is available from debug info.

* ``./configure`` scripts generated by GNU autoconf determines whether a linker
  supports modern GNU-compatible features or not by searching for "GNU" in the
  ``--help`` message. To be compatible with the scripts, we decided to add a
  string "(compatible with GNU linkers)" to our ``--help`` message. This is a
  hack, but just like the web browser's User-Agent string (which everyone still
  claim they are "Mozilla/5.0"), we had no choice other than doing this to claim
  that we accept GNU-compatible options.
>>>>>>> origin/release/5.x

* The ``-Map`` option is added. The option is to make the linker to print out how
  input files are mapped to the output file. Here is an example::

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
* ...
=======
* PDB GUID is set to hash of PDB contents instead to a random byte
  sequence for build reproducibility.

* ``/pdbsourcepath:`` is now also used to make ``"cwd"``, ``"exe"``, ``"pdb"``
  in the env block of PDB outputs absolute if they are relative, and to make
  paths to obj files referenced in PDB outputs absolute if they are relative.
  Together with the previous item, this makes it possible to generate
  executables and PDBs that are fully deterministic and independent of the
  absolute path to the build directory, so that different machines building
  the same code in different directories can produce exactly the same output.

* The following flags have been added: ``/force:multiple``

* lld now can link against import libraries produced by GNU tools.

* lld can create thunks for ARM and ARM64, to allow linking larger images
  (over 16 MB for ARM and over 128 MB for ARM64)

* Several speed and memory usage improvements.

* lld now creates debug info for typedefs.

* lld can now link obj files produced by ``cl.exe /Z7 /Yc``.

* lld now understands ``%_PDB%`` and ``%_EXT%`` in ``/pdbaltpath:``.

* Undefined symbols are now printed in demangled form in addition to raw form.
>>>>>>> release/8.x

MinGW Improvements
------------------

* ...

* lld now supports COFF embedded directives for linking to nondefault
  libraries, just like for the normal COFF target.

* Actually generate a codeview build id signature, even if not creating a PDB.
  Previously, the ``--build-id`` option did not actually generate a build id
  unless ``--pdb`` was specified.

WebAssembly Improvements
------------------------

* `__data_end` and `__heap_base` are no longer exported by default,
  as it's best to keep them internal when possible. They can be
  explicitly exported with `--export=__data_end` and
  `--export=__heap_base`, respectively.
=======
* Improved correctness of exporting mangled stdcall symbols.

* Completed support for ARM64 relocations.

* Added support for outputting PDB debug info for MinGW targets.

* Improved compatibility of output binaries with GNU binutils objcopy/strip.

* Sped up PDB file creation.

* Changed section layout to improve compatibility with link.exe.

* `/subsystem` inference is improved to cover more corner cases.

* Added the following flags: ``--color-diagnostics={always,never,auto}``,
  ``--no-color-diagnostics``, ``/brepro``, ``/debug:full``, ``/debug:ghash``,
  ``/guard:cf``, ``/guard:longjmp``, ``/guard:nolongjmp``, ``/integritycheck``,
  ``/order``, ``/pdbsourcepath``, ``/timestamp``
>>>>>>> release/7.x
=======
    Address          Size             Align Out     In      Symbol
    00000000016d84d8 00000000008f8f50     8 .eh_frame
    00000000016d84d8 00000000008f8f50     8         <internal>:(.eh_frame)
    0000000001fd2000 00000000034b3bd0    16 .text
    0000000001fd2000 000000000000002a     1         /usr/lib/x86_64-linux-gnu/crt1.o:(.text)
    0000000001fd2000 0000000000000000     0                 _start
    0000000001fd202a 0000000000000000     1         /usr/lib/x86_64-linux-gnu/crti.o:(.text)
    0000000001fd2030 00000000000000bd    16         /usr/lib/gcc/x86_64-linux-gnu/4.8/crtbegin.o:(.text)
    0000000001fd2030 0000000000000000     0                 deregister_tm_clones
    0000000001fd2060 0000000000000000     0                 register_tm_clones

  This format is not the same as GNU linkers as our linker internal data
  structure is different from them but contains the same amount of information
  and should be more readable than their outputs.

  As with other lld features, the ``-Map`` option is designed with speed in mind.
  The option would generate a hundred megabyte text file if you link a large
  program with it. lld can usually do that in a few seconds, and it is generally
  a few times faster than the GNU gold's ``-Map`` option.

* lld's ``--gdb-index`` option used to be slow, but we sped it up so that it is
  at least as fast as the GNU gold.

* Some nonstandard relocations, such as R_X86_64_8 or R_X86_64_16, are supported.
  They are not used for 32/64-bit applications, but some 16-bit bootloaders need
  them.

* Paddings in executable text sections are now filled with trap instructions
  (such as INT3) instead of being left as null bytes. This change improves
  disassembler outputs because it now prints out trap instructions instead of
  trying to decode 0x00 as an instruction. It also makes debugging of some type
  of program easier because when the control reaches a padding, the program
  immediately raises an error.

* The following options are added: ``-M``, ``-Map``,
  ``-compress-debug-sections``, ``-emit-relocs``,
  ``-error-unresolved-symbols``, ``-exclude-libs``, ``-filter``,
  ``-no-dynamic-linker``, ``-no-export-dynamic``, ``-no-fatal-warnings``,
  ``-print-map``, ``-warn-unresolved-symbols``, ``-z nocopyreloc``,
  ``-z notext``, ``-z rodynamic``


Contributors to lld 5.0
=======================

We had 63 individuals contribute to lld 5.0. Thank you so much!

- Adrian McCarthy
- Alberto Magni
- Alexander Richardson
- Andre Vieira
- Andrew Ng
- Anton Korobeynikov
- Bob Haarman
- David Blaikie
- Davide Italiano
- David L. Jones
- Dmitry Mikulin
- Ed Maste
- Ed Schouten
- Eric Beckmann
- Eric Fiselier
- Eugene Leviant
- Evgeniy Stepanov
- Galina Kistanova
- George Rimar
- Hans Wennborg
- Igor Kudrin
- Ismail Donmez
- Jake Ehrlich
- James Henderson
- Joel Jones
- Jon Chesterfield
- Kamil Rytarowski
- Kevin Enderby
- Konstantin Zhuravlyov
- Kyungwoo Lee
- Leslie Zhai
- Mark Kettenis
- Martell Malone
- Martin Storsjo
- Meador Inge
- Mehdi Amini
- Michal Gorny
- NAKAMURA Takumi
- Paul Robinson
- Pavel Labath
- Petar Jovanovic
- Peter Collingbourne
- Peter Smith
- Petr Hosek
- Rafael Espindola
- Reid Kleckner
- Richard Smith
- Robert Clarke
- Rui Ueyama
- Saleem Abdulrasool
- Sam Clegg
- Sean Eveson
- Sean Silva
- Shankar Easwaran
- Shoaib Meenai
- Simon Atanasyan
- Simon Dardis
- Simon Tatham
- Sylvestre Ledru
- Tom Stellard
- Vitaly Buka
- Yuka Takahashi
- Zachary Turner
>>>>>>> origin/release/5.x
