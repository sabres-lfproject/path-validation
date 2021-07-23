#!/bin/sh

if [ $1 -eq "1" ]
then
    cd build
    ./xorHash "../data/1.secret"
    cd ..
else
    cd build
    ./xorHash "../data/$1.secret"
    
    for i in $(seq $(($1 - 1)) -1 1)
    do  
        ./xorHash "../data/$i.secret" "../data/new.secret"
    done
    cd ..
fi