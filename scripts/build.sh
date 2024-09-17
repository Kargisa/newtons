#!/bin/bash
cd $(dirname "$0")/..

if [ "$1" = "debug" ]
then

	./scripts/debug-build.sh

else

	./scripts/release-build.sh

fi
