#!/bin/bash

getopts  "p:v:x:" flag
case $flag in
  p)
    echo "proofer generates $OPTARG proofs"
    ./prover $OPTARG
    ;;
  v)
    echo "verifier verifies #$OPTARG proof"
    ./verifier $OPTARG
    ;;
  x)
    echo "exclusive_or $OPTARG secrets and show its hash"
    ./testXorHash $OPTARG
    ;;
  *)
    echo "please input a flag -p, -v or -x"
    ;;
esac
