#!/bin/sh
npm start
cd libsnark-test/build/src
./prover
./verifier