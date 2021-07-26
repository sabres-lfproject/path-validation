#!/bin/sh

if [ $# -eq 1 ]
then
    cd build
    ./prover "../data/$1.secret" "../data/$1.hash" "../data/$1.proof"
    cd ..
else
    cd build
    ./xorHash "../data/$1.secret" "../data/$2.secret"
    ./prover "../data/new.secret" "../data/$1.hash" "../data/$1.proof"
    cd ..
fi
