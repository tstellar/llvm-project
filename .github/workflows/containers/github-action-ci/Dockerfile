FROM docker.io/library/ubuntu:22.04

# swig is needed for lldb docs.

# abi-dumper, autoconf, and pkg-config are required for the abi-dump workflow.
# abi-compliance-checker is needed for the abi-compare workflow.
# python3-setuptools and python3-psutil are required for the release-lit task.
RUN apt-get update && \
    apt-get install -y \
    abi-compliance-checker \
    abi-dumper \
    autoconf \
    cmake \
    doxygen \
    graphviz \
    ninja-build \
    pkg-config \
    python3-github \
    python3-psutil \
    python3-setuptools \
    swig \
    texlive-font-utils
