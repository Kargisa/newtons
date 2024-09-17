#!/bin/bash
cd $(dirname "$0")/..

cmake --preset x64-linux-debug
cmake --build --preset x64-linux-build-debug
