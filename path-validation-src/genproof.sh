#!/bin/sh

if [ $1 -eq "1" ]
then
    cd build
    ./xorHash "../data/1.secret"
    ./prover "../data/1.secret" "../data/1.hash" "../data/1.proof"
    cd ..
else
    cd build
    ./xorHash "../data/$1.secret"
    ./prover "../data/new.secret" "../data/$1.hash" "../data/$1.proof"
    
    for i in $(seq $(($1 - 1)) -1 1)
    do  
        ./xorHash "../data/$i.secret" "../data/new.secret"
        ./prover "../data/new.secret" "../data/$i.hash" "../data/$i.proof"
    done
    cd ..
fi