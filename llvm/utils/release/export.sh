#!/bin/bash
#===-- tag.sh - Tag the LLVM release candidates ----------------------------===#
#
# Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#
#===------------------------------------------------------------------------===#
#
# Create branches and release candidates for the LLVM release.
#
#===------------------------------------------------------------------------===#

set -e

projects="llvm clang compiler-rt libcxx libcxxabi libclc clang-tools-extra polly lldb lld openmp libunwind flang"

release=""
rc=""
yyyymmdd=$(date +'%Y%m%d')
snapshot=""

indent() {
  local indentSize=2
  local indent=1
  if [ -n "$1" ]; then indent=$1; fi
  pr -to $(($indent * $indentSize))
}

usage() {
cat <<EOF
Export the Git sources and build tarballs from them.

Usage: $(basename $0) [-release|--release <major>.<minor>.<patch>]
                      [-rc|--rc <num>]
                      [-final|--final]
                      [-snapshot|--snapshot <git-ref>] 

Flags:

  -release  | --release <major>.<minor>.<patch>    The version number of the release
  -rc       | --rc <num>                           The release candidate number
  -final    | --final                              When provided, this option will disable the rc flag
  -snapshot | --snapshot <git-ref>                 (optional) Use <git-ref> to determine the release and don't export the test-suite files

These are the filenames (with <placeholders>) for the artifacts and hard
links for each LLVM component created by this script: 

$(\
      echo "$projects " \
      | sed 's/\([a-z-]\+\) /\1-<RELEASE><RC>.src.tar.xz \1-<YYYYMMDD>.src.tar.xz \n/g' \
      | column -t -o " <- "\
      | indent 2
  )

Additional files being generated:

  * llvm-project-<RELEASE><RC>.src.tar.xz    (the complete LLVM source project)
  * test-suite-<RELEASE><RC>.src.tar.xz      (only when not using --snapshot)

To ease the creation of snapshot builds, we also provide these files

  * llvm-release-<YYYYMMDD>.txt        (contains the <RELEASE> as a text)
  * llvm-rc-<YYYYMMDD>.txt             (contains the rc version passed to the invocation of $(basename $0))
  * llvm-git-revision-<YYYYMMDD>.txt   (contains the current git revision sha1)

Example values for the placeholders:

  * <RELEASE>  -> 13.0.0
  * <YYYYMMDD> -> 20210414
  * <RC>       -> rc4        (will be empty when using --snapshot)
EOF
}

export_sources() {
    tag="llvmorg-$release"

    if [ "$rc" = "final" ]; then
        rc=""
    else
        tag="$tag-$rc"
    fi

    llvm_src_dir=$(readlink -f $(dirname "$(readlink -f "$0")")/../../..)
    [ -d $llvm_src_dir/.git ] || ( echo "No git repository at $llvm_src_dir" ; exit 1 )

    # Determine the release by fetching the version from LLVM's CMakeLists.txt.
    [ -n "$snapshot" ] && release=$(grep -ioP 'set\(\s*LLVM_VERSION_(MAJOR|MINOR|PATCH)\s\K[0-9]+' $llvm_src_dir/llvm/CMakeLists.txt | paste -sd '.')
    
    tag="llvmorg-$release"

    if [ "$rc" = "final" ]; then
        rc=""
    else
        tag="$tag-$rc"
    fi

    target_dir=$(pwd)

    echo "Creating tarball for llvm-project ..."
    pushd $llvm_src_dir/
    tree_id=$tag
    [ -n "$snapshot" ] && tree_id="$snapshot"
    echo "Tree ID to archive: $tree_id"

    # This might be a surprise but a package like clang or compiler-rt don't
    # know about the LLVM version itself. That's why we also export a the
    # llvm-version*-<YYYYMMDD> and llvm-git*-<YYYYMMDD> files.
    git_rev=$(git rev-parse $tree_id)
    echo "git revision: $git_rev"
    echo "$release" > $target_dir/llvm-release-$yyyymmdd.txt
    echo "$rc" > $target_dir/llvm-rc-$yyyymmdd.txt
    echo "$git_rev" > $target_dir/llvm-git-revision-$yyyymmdd.txt
    
    git archive --prefix=llvm-project-$release$rc.src/ $tree_id . | xz >$target_dir/llvm-project-$release$rc.src.tar.xz
    [ -n "$snapshot" ] && ln -fv $target_dir/llvm-project-$release$rc.src.tar.xz $target_dir/llvm-project-$yyyymmdd.src.tar.xz
    popd

    if [ -z "$snapshot" ]; then
        if [ ! -d test-suite-$release$rc.src ]; then
            echo "Fetching LLVM test-suite source ..."
            mkdir -p test-suite-$release$rc.src
            curl -L https://github.com/llvm/test-suite/archive/$tag.tar.gz | \
                tar -C test-suite-$release$rc.src --strip-components=1 -xzf -
        fi
        echo "Creating tarball for test-suite ..."
        tar --sort=name --owner=0 --group=0 \
            --pax-option=exthdr.name=%d/PaxHeaders/%f,delete=atime,delete=ctime \
            -cJf test-suite-$release$rc.src.tar.xz test-suite-$release$rc.src
    fi

    for proj in $projects; do
        echo "Creating tarball for $proj ..."
        pushd $llvm_src_dir/$proj
        git archive --prefix=$proj-$release$rc.src/ $tree_id . | xz >$target_dir/$proj-$release$rc.src.tar.xz
        [ -n "$snapshot" ] && ln -fv $target_dir/$proj-$release$rc.src.tar.xz $target_dir/$proj-$yyyymmdd.src.tar.xz
        popd
    done
}

while [ $# -gt 0 ]; do
    case $1 in
        -release | --release )
            shift
            release=$1
            ;;
        -rc | --rc )
            shift
            rc="rc$1"
            ;;
        -final | --final )
            rc="final"
            ;;
        -snapshot | --snapshot )
            shift
            snapshot="$1"
            ;;
        -h | -help | --help )
            usage
            exit 0
            ;;
        * )
            echo "unknown option: $1"
            usage
            exit 1
            ;;
    esac
    shift
done

if [ -n "$snapshot" ]; then 
    if [[ "$rc" != "" || "$release" != "" ]]; then
        echo "error: must not specify -rc or -release when creating a snapshot"
        exit 1
    fi
elif [ -z "$release" ]; then
    echo "error: need to specify a release version"
    exit 1
fi

# Make sure umask is not overly restrictive.
umask 0022

export_sources
exit 0
