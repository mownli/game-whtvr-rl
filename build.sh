#!/bin/sh

BUILDDIR=build

cmake -B $BUILDDIR
cd $BUILDDIR
cmake --build . -j8 -DNDEBUG
#ctest -j8 --verbose
