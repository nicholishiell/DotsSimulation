#!/bin/bash 
clear

rm -r ./batchOutput
mkdir ./batchOutput

COUNTER=0
while [  $COUNTER -lt $1 ]; do
    echo "RUN NUMBER $COUNTER"
    
    ./RUN.sh

    cp -r ./output ./batchOutput/outputRunNumber$COUNTER

    let COUNTER=COUNTER+1 
done

