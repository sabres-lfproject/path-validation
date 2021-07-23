#!/bin/sh

cd build
./verifier "../data/$1.hash" "../data/$1.proof"
cd ..
