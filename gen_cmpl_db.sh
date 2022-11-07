#!/bin/sh

# generate compilation database for Clang tools.
make -s clean
bear -- make -s all
