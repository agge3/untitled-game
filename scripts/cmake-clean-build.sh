#!/bin/bash

for cmakefile in CMakeCache.txt cmake_install.cmake CTestTestfile.cmake CMakeFiles Makefile cmake-clean-build.sh
do
    rm -rfv $cmakefile
done
