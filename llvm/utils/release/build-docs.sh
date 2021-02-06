#!/bin/sh
#===-- build-docs.sh - Tag the LLVM release candidates ---------------------===#
#
# Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#
#===------------------------------------------------------------------------===#
#
# Build documentation for LLVM releases.
#
# Required Packages:
# * Fedora:
#   * dnf install doxygen python3-sphinx texlive-epstopdf ghostscript
#   * pip install sphinx-markdown-tables
# * Ubuntu:
#   * apt-get install doxygen sphinx-common python3-recommonmark \
#             ninja-build graphviz texlive-font-utils
#===------------------------------------------------------------------------===#

set -ex

builddir=docs-build

usage() {
  echo "Build the documentation for an LLVM release.  This only needs to be "
  echo "done for -final releases."
  echo "usage: `basename $0`"
  echo " "
  echo " -release <num> The release version to build (required if not passing"
  echo "                -srcdir)"
  echo " -srcdir  <dir> Path to llvm source directory with CMakeLists.txt"
  echo "                (optional) default: ../../"
}

package_doxygen() {

  project=$1
  proj_dir=$2
  output=${project}_doxygen-$release

  mv $builddir/$proj_dir/docs/doxygen/html $output
  tar -cJf $output.tar.xz $output
}


while [ $# -gt 0 ]; do
  case $1 in
    -release )
      shift
      release=$1
      ;;
    -srcdir )
      shift
      srcdir=$1
      ;;
    * )
      echo "unknown option: $1"
      usage
      exit 1
      ;;
   esac
   shift
done

if [ -z "$srcdir$release" ]; then
  echo "error: Must specify either -srcdir or -release options"
  exit 1
fi

# Set default source directory if one is not supplied
if [ -z "$srcdir" ]; then
  git_ref=llvmorg-$release
  if [ -d llvm-project ]; then
    echo "error llvm-project directory already exists"
    exit 1
  fi
  mkdir -p llvm-project
  pushd llvm-project
  curl -L https://github.com/llvm/llvm-project/archive/$git_ref.tar.gz | tar --strip-components=1 -xzf -
  popd
  srcdir="./llvm-project/llvm"
fi

cmake -G Ninja $srcdir -B $builddir \
               -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra;lld;libcxx;polly;flang" \
               -DCMAKE_BUILD_TYPE=Release \
               -DLLVM_ENABLE_DOXYGEN=ON \
               -DLLVM_ENABLE_SPHINX=ON \
               -DLLVM_BUILD_DOCS=ON \
               -DLLVM_DOXYGEN_SVG=ON \
               -DSPHINX_WARNINGS_AS_ERRORS=OFF

ninja -C $builddir \
               docs-clang-html \
               docs-flang-html \
               docs-libcxx-html \
               docs-lld-html \
               docs-llvm-html \
               docs-polly-html \
               doxygen-clang \
               doxygen-clang-tools \
               doxygen-flang \
               doxygen-llvm \
               doxygen-mlir \
               doxygen-polly


package_doxygen llvm .
package_doxygen clang tools/clang
package_doxygen clang-tools-extra tools/clang/tools/extra
package_doxygen flang tools/flang

html_dir=$builddir/html-export/

for d in docs/ tools/clang/docs/ tools/lld/docs/ tools/clang/tools/extra/docs/ projects/libcxx/docs/ tools/polly/docs/ tools/flang/docs/; do
  mkdir -p $html_dir/$d
  mv $builddir/$d/html/* $html_dir/$d/
done
