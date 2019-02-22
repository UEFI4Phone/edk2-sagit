#!/bin/bash
set -e
sudo apt install -y build-essential uuid-dev iasl git nasm gcc-aarch64-linux-gnu
curdir="$PWD"
cd ..
git clone https://github.com/tianocore/edk2.git --recursive
git clone https://github.com/tianocore/edk2-platforms.git
cd "$curdir"
./firstrun.sh
./build.sh
