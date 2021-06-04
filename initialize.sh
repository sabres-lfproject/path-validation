#!/bin/sh

cd build
for i in $(seq 1 $1); do ./gensecret $i; done
cd ..