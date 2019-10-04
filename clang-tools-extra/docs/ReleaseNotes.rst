<<<<<<< HEAD
====================================================
Extra Clang Tools 10.0.0 (In-Progress) Release Notes
====================================================
=======
=====================================
Extra Clang Tools 8.0.0 Release Notes
=====================================
>>>>>>> release/8.x

.. contents::
   :local:
   :depth: 3

Written by the `LLVM Team <https://llvm.org/>`_

<<<<<<< HEAD
.. warning::

   These are in-progress notes for the upcoming Extra Clang Tools 10 release.
   Release notes for previous releases can be found on
   `the Download Page <https://releases.llvm.org/download.html>`_.

=======
>>>>>>> release/8.x
Introduction
============

This document contains the release notes for the Extra Clang Tools, part of the
Clang release 10.0.0. Here we describe the status of the Extra Clang Tools in
some detail, including major improvements from the previous release and new
feature work. All LLVM releases may be downloaded
from the `LLVM releases web site <https://releases.llvm.org/>`_.

For more information about Clang or LLVM, including information about
the latest release, please see the `Clang Web Site <https://clang.llvm.org>`_ or
the `LLVM Web Site <https://llvm.org>`_.

<<<<<<< HEAD
Note that if you are reading this file from a Subversion checkout or the
main Clang web page, this document applies to the *next* release, not
the current one. To see the release notes for a specific release, please
see the `releases page <https://llvm.org/releases/>`_.

What's New in Extra Clang Tools 10.0.0?
=======================================
=======
What's New in Extra Clang Tools 8.0.0?
======================================
>>>>>>> release/8.x

Some of the major new features and improvements to Extra Clang Tools are listed
here. Generic improvements to Extra Clang Tools as a whole or to its underlying
infrastructure are described first, followed by tool-specific sections.


Improvements to clangd
----------------------

- clangd now adds namespace qualifiers in code completion, for example, if you
  type "``vec``", the list of completions will include "``std::vector``".

  See also: `r343248 <https://reviews.llvm.org/rL343248>`__.

- When a :ref:`global index <project-wide-index>` is available, clangd will use it to augment the
  results of "go to definition" and "find references" queries. Global index
  also enables global code completion, which suggests symbols that are not
  imported in the current file and automatically inserts the missing
  ``#include`` directives.

- clangd stores the symbol index on disk in a new compact binary serialization
  format.  It is 10x more compact than YAML and 40% more compact than gzipped
  YAML.

  See also: `r341375 <https://reviews.llvm.org/rL341375>`__.

- clangd has a new efficient symbol index suitable for complex and fuzzy
  queries and large code bases (e.g., LLVM, Chromium).  This index is used for
  code completion, go to definition, and cross-references.  The architecture of
  the index allows for complex and fuzzy retrieval criteria and sophisticated
  scoring.

  See also: `discussion on the mailing list
  <http://lists.llvm.org/pipermail/cfe-dev/2018-July/058487.html>`__, `design
  doc
  <https://docs.google.com/document/d/1C-A6PGT6TynyaX4PXyExNMiGmJ2jL1UwV91Kyx11gOI/edit>`__.

- clangd has a new LSP extension that communicates information about activity
  on clangd's per-file worker thread.  This information can be displayed to
  users to let them know that the language server is busy with something.  For
  example, in clangd, building the AST blocks many other operations.

  More info: :ref:`lsp-extension-file-status`.

- clangd has a new LSP extension that allows the client to supply the
  compilation commands over LSP, instead of finding compile_commands.json on
  disk.

  More info: :ref:`lsp-extension-compilation-commands`.

- clangd has a new LSP extension that allows the client to request fixes to be
  sent together with diagnostics, instead of asynchronously.

  More info: :ref:`lsp-extension-code-actions-in-diagnostics`.

- clangd has a new LSP extension that allows the client to resolve a symbol in
  a light-weight manner, without retrieving further information (like
  definition location, which may require consulting an index).

  More info: :ref:`lsp-extension-symbol-info`.

<<<<<<< HEAD
- :doc:`clang-doc <clang-doc>` now generates documentation in HTML format.
=======
>>>>>>> release/8.x

Improvements to clang-query
---------------------------

The improvements are...

Improvements to clang-tidy
--------------------------

- New :doc:`bugprone-dynamic-static-initializers
  <clang-tidy/checks/bugprone-dynamic-static-initializers>` check.

  Finds instances where variables with static storage are initialized
  dynamically in header files.

- New :doc:`bugprone-infinite-loop
  <clang-tidy/checks/bugprone-infinite-loop>` check.

  Finds obvious infinite loops (loops where the condition variable is not
  changed at all).

- New :doc:`cppcoreguidelines-init-variables
  <clang-tidy/checks/cppcoreguidelines-init-variables>` check.

- New :doc:`darwin-dispatch-once-nonstatic
  <clang-tidy/checks/darwin-dispatch-once-nonstatic>` check.

  Finds declarations of ``dispatch_once_t`` variables without static or global
  storage.

- New :doc:`google-upgrade-googletest-case
  <clang-tidy/checks/google-upgrade-googletest-case>` check.

  Finds uses of deprecated Googletest APIs with names containing ``case`` and
  replaces them with equivalent APIs with ``suite``.

- New :doc:`linuxkernel-must-use-errs
  <clang-tidy/checks/linuxkernel-must-use-errs>` check.

  Checks Linux kernel code to see if it uses the results from the functions in
  ``linux/err.h``.

- New :doc:`llvm-prefer-register-over-unsigned
  <clang-tidy/checks/llvm-prefer-register-over-unsigned>` check.

  Finds historical use of ``unsigned`` to hold vregs and physregs and rewrites
  them to use ``Register``

- New :doc:`objc-missing-hash
  <clang-tidy/checks/objc-missing-hash>` check.

  Finds Objective-C implementations that implement ``-isEqual:`` without also
  appropriately implementing ``-hash``.

- Improved :doc:`bugprone-posix-return
  <clang-tidy/checks/bugprone-posix-return>` check.

  Now also checks if any calls to ``pthread_*`` functions expect negative return
  values.

- The 'objc-avoid-spinlock' check was renamed to :doc:`darwin-avoid-spinlock
  <clang-tidy/checks/darwin-avoid-spinlock>`

<<<<<<< HEAD
Improvements to include-fixer
-----------------------------
=======
  Checks for uses of nested namespaces in the form of
  ``namespace a { namespace b { ... }}`` and offers change to
  syntax introduced in C++17 standard: ``namespace a::b { ... }``.

- New :doc:`modernize-deprecated-ios-base-aliases
  <clang-tidy/checks/modernize-deprecated-ios-base-aliases>` check.

  Detects usage of the deprecated member types of ``std::ios_base`` and replaces
  those that have a non-deprecated equivalent.

- New :doc:`modernize-use-nodiscard
  <clang-tidy/checks/modernize-use-nodiscard>` check.

  Adds ``[[nodiscard]]`` attributes (introduced in C++17) to member functions
  to highlight at compile time which return values should not be ignored.

- New :doc:`readability-const-return-type
  <clang-tidy/checks/readability-const-return-type>` check.

  Checks for functions with a ``const``-qualified return type and recommends
  removal of the ``const`` keyword.

- New :doc:`readability-isolate-decl
  <clang-tidy/checks/readability-isolate-declaration>` check.

  Detects local variable declarations declaring more than one variable and
  tries to refactor the code to one statement per declaration.

- New :doc:`readability-magic-numbers
  <clang-tidy/checks/readability-magic-numbers>` check.

  Detects usage of magic numbers, numbers that are used as literals instead of
  introduced via constants or symbols.

- New :doc:`readability-redundant-preprocessor
  <clang-tidy/checks/readability-redundant-preprocessor>` check.

  Finds potentially redundant preprocessor directives.

- New :doc:`readability-uppercase-literal-suffix
  <clang-tidy/checks/readability-uppercase-literal-suffix>` check.

  Detects when the integral literal or floating point literal has non-uppercase
  suffix, and suggests to make the suffix uppercase. The list of destination
  suffixes can be optionally provided.

- New alias :doc:`cert-dcl16-c
  <clang-tidy/checks/cert-dcl16-c>` to :doc:`readability-uppercase-literal-suffix
  <clang-tidy/checks/readability-uppercase-literal-suffix>`
  added.

- New alias :doc:`cppcoreguidelines-avoid-c-arrays
  <clang-tidy/checks/cppcoreguidelines-avoid-c-arrays>`
  to :doc:`modernize-avoid-c-arrays
  <clang-tidy/checks/modernize-avoid-c-arrays>` added.

- New alias :doc:`cppcoreguidelines-non-private-member-variables-in-classes
  <clang-tidy/checks/cppcoreguidelines-non-private-member-variables-in-classes>`
  to :doc:`misc-non-private-member-variables-in-classes
  <clang-tidy/checks/misc-non-private-member-variables-in-classes>`
  added.

- New alias :doc:`hicpp-avoid-c-arrays
  <clang-tidy/checks/hicpp-avoid-c-arrays>`
  to :doc:`modernize-avoid-c-arrays
  <clang-tidy/checks/modernize-avoid-c-arrays>` added.
>>>>>>> release/8.x

The improvements are...

Improvements to clang-include-fixer
-----------------------------------

The improvements are...

Improvements to modularize
--------------------------

<<<<<<< HEAD
The improvements are...

Improvements to pp-trace
------------------------

The improvements are...

Clang-tidy visual studio plugin
-------------------------------

The clang-tidy-vs plugin has been removed from clang, as
it's no longer maintained. Users should migrate to
`Clang Power Tools <https://marketplace.visualstudio.com/items?itemName=caphyon.ClangPowerTools>`_
instead.
=======
- The :doc:`readability-uppercase-literal-suffix
  <clang-tidy/checks/readability-uppercase-literal-suffix>` check does not warn
  about literal suffixes inside macros anymore by default.
>>>>>>> release/8.x
