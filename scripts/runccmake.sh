#!/bin/bash

CC=clang CXX=clang++ ccmake -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE -G"Ninja Multi-Config" -B build
