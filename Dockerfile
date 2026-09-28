FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

# Install g++/clang, cmake, ninja, ccache, and gdb
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    ninja-build \
    ccache \
    clang \
    gdb \
    && rm -rf /var/lib/apt/lists/*

# Set Clang as the default compiler for fast C++20 builds
ENV CC=clang
ENV CXX=clang++

# Enable ccache
ENV PATH="/usr/lib/ccache:$PATH"
ENV CCACHE_DIR=/root/.ccache

WORKDIR /hlm

CMD ["/bin/bash"]