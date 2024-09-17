#!/bin/bash
cd $(dirname "$0")/..

cmake --preset x64-linux-release
cmake --build --preset x64-linux-build-release

