========================
lld 10.0.0 Release Notes
========================

.. contents::
    :local:

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

Non-comprehensive list of changes in this release
=================================================

ELF Improvements
----------------

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

COFF Improvements
-----------------

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
