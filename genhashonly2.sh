#!/bin/sh

## new version
## only taking two secret to xor and generate hash
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
        ./xorHash "../data/$i.secret" "../data/$(($i + 1)).secret"
    done
    cd ..
fi