#!/bin/sh
cd $(dirname "$0")

./tests.sh
if [ $? -eq 1 ]; then
    exit 1
fi
./clang.sh
if [ $? -eq 1 ]; then
    exit 1
fi
