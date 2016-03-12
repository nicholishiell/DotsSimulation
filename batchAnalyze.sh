#!/bin/bash 
# Argument passed is the number of runs in the batch

clear

touch totalHistoData
touch avgHistoData

COUNTER=0
while [  $COUNTER -lt $1 ]; do
    
    echo RUN NUMBER $COUNTER 
    
    avg=$(($2 + 1)) 
    avg=$(sed "$avg!d" ./batchOutput/outputRunNumber$COUNTER/rawData/distanceTravelled.dat)   
    total=$(($2 + 2)) 
    total=$(sed "$total!d" ./batchOutput/outputRunNumber$COUNTER/rawData/distanceTravelled.dat)

    echo "$avg" >> avgHistoData
    echo "$total" >> totalHistoData
 
   let COUNTER=COUNTER+1 
done

mv avgHistoData ./batchOutput/avgHisto.dat
mv totalHistoData ./batchOutput/totalHisto.dat

cp plotScripts/* ./batchOutput/

cd batchOutput
gnuplot avgHisto.p
gnuplot totalHisto.p

cd ..

mv ./batchOutput ../batchOutput

