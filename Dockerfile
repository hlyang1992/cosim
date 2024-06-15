
FROM ubuntu:22.04

RUN DEBIAN_FRONTEND="noninteractive"

RUN apt-get update \
  && apt-get install -y build-essential \
    git \
    cmake \
    curl \
    zip \
    unzip \
    tar \
    pkg-config \
    libtbb-dev \
    flex \
    bison \
  && apt-get clean

WORKDIR /home
RUN git clone https://github.com/Microsoft/vcpkg.git
WORKDIR /home/vcpkg
RUN ./bootstrap-vcpkg.sh

WORKDIR /home/cosim
COPY . .
RUN cmake . -B build -DCMAKE_TOOLCHAIN_FILE="/home/vcpkg/scripts/buildsystems/vcpkg.cmake" -DCOSIM_BUILD_EXAMPLES=ON -DCOSIM_BUILD_TESTS=ON -DCMAKE_BUILD_TYPE="Release"
RUN cmake --build build
