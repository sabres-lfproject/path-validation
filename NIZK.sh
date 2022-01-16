#!/bin/sh
cd libsnark-test/build/src
./verifier
./prover

i=$(date +%s%N)
echo "$i" >> time-NIZK.txt
