#!/bin/bash
# Prerequisites:
# - Bash shell
# - git
# - cmake

# Setup
export MY_INSTALL_DIR=$HOME/.local
mkdir -p $MY_INSTALL_DIR
export PATH="$MY_INSTALL_DIR/bin:$PATH"

# Check if git is installed
if ! command -v git &> /dev/null; then
  echo "Error: git is not installed. Please install git before proceeding."
  exit 1
fi

# Check if cmake is installed
if ! command -v cmake &> /dev/null; then
  echo "Error: cmake is not installed. Please install cmake before proceeding."
  exit 1
fi

# Install required tools
sudo apt update
sudo apt install -y build-essential autoconf libtool pkg-config

# Check if build-essential, autoconf, libtool, and pkg-config were installed successfully
if [ $? -ne 0 ]; then
  echo "Error: Failed to install required tools."
  exit 1
fi

# Clone grpc repo
git clone --recurse-submodules -b v1.58.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc

# Check if the git clone was successful
if [ $? -ne 0 ]; then
  echo "Error: Failed to clone the grpc repository."
  exit 1
fi

cd grpc
mkdir -p cmake/build
pushd cmake/build

# Configure and build grpc
cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR \
      ../..

# Check if cmake configuration was successful
if [ $? -ne 0 ]; then
  echo "Error: Failed to configure grpc using cmake."
  exit 1
fi

make -j$((`nproc`-1))

# Check if make was successful
if [ $? -ne 0 ]; then
  echo "Error: Failed to build grpc using make."
  exit 1
fi

make install

# Check if make install was successful
if [ $? -ne 0 ]; then
  echo "Error: Failed to install grpc using make install."
  exit 1
fi

popd
