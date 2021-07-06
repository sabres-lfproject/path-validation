#!/bin/sh

cd build
./xorHash "../data/$1" "../data/$2"
cd ..