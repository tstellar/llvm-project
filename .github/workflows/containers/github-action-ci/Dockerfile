FROM docker.io/library/ubuntu:22.04 as base
ENV LLVM_SYSROOT=/opt/llvm

FROM base as stage1-toolchain
ENV LLVM_VERSION=19.1.5

RUN apt-get update && apt-get -y install clang
