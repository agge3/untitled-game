#!/usr/bin/env bash

for cmakefile in CMakeCache.txt cmake_install.cmake CTestTestfile.cmake CMakeFiles Makefile cmake-clean-build.sh CMakeDoxyfile.in CMakeDoxygenDefaults.cmake Doxyfile
do
    rm -rfv $cmakefile
done
