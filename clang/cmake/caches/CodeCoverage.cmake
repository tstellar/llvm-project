# This cache file creates a 2-stage build for generating code coverage
# statistics for the LLVM project.
# Example Usage:
#
# cmake -G Ninja -S llvm -B build -C clang/cmake/caches/CodeCoverage.cmake
# ninja -C build stage2-generate-coverage-report
#


# Stage 1: Build clang, llvm tools, and compiler-rt
set(CMAKE_BUILD_TYPE Release CACHE STRING "")
set(LLVM_ENABLE_PROJECTS "clang;lld" CACHE STRING "")
set(LLVM_ENABLE_RUNTIMES compiler-rt CACHE STRING "")
set(LLVM_TARGETS_TO_BUILD Native CACHE STRING "")
set(CLANG_ENABLE_BOOTSTRAP ON CACHE BOOL "")
set(CLANG_BOOTSTRAP_EXTRA_DEPS "llvm-cov;llvm-profdata" CACHE STRING "")
set(CLANG_BOOTSTRAP_TARGETS generate-coverage-report CACHE STRING "")
# Need to set this, otherwise the llvm tools will get built since compiler-rt
# depends on them when LLVM_INCLUDE_TESTS is ON.
set(LLVM_INCLUDE_TESTS OFF CACHE BOOL "")

# Stage 2
set(BOOTSTRAP_LLVM_BUILD_INSTRUMENTED_COVERAGE ON CACHE BOOL "")
set(BOOTSTRAP_COMPILER_RT_INCLUDE_TESTS OFF CACHE BOOL "")
set(BOOTSTRAP_LLVM_COVERAGE_REPORT_DEPENDS "check-all" CACHE BOOL "")
set(BOOTSTRAP_LLVM_ENABLE_PROJECTS "clang;lld;clang-tools-extra" CACHE STRING "")
set(BOOTSTRAP_LLVM_LINK_LLVM_DYLIB ON CACHE BOOL "")
set(BOOTSTRAP_LLVM_ENABLE_LLD ON CACHE BOOL "")
set(BOOTSTRAP_LLVM_CODE_COVERAGE_TARGETS "LLVM;libclang;clang-cpp;lld;clangd;clang-tidy;clang-format" CACHE STRING "")
