#!/usr/bin/env bash
echo "WrongDestVehicles"
scavetool export files/*.sca -o WrongDestVehicles.csv -f "itervar:interval(*) AND name(WrongDestVehicles)" -F CSV-S

echo "TotalVehiclesSpawned"
scavetool export files/*.sca -o TotalVehiclesSpawned.csv -f "itervar:interval(*) AND name(TotalVehiclesSpawned), name(TotalVehiclesReached)" -F CSV-S

echo "AllQueueLength"
scavetool export files/*.sca -o AllQueueLength.csv -f "itervar:interval(*) AND name(SouthQueueSignal:mean), name(NorthQueueSignal:mean), name(EastQueueSignal:mean), name(WestQueueSignal:mean)" -F CSV-S

echo "TotalTravelTime"
scavetool export files/*.sca -o TotalTravelTime.csv -f "itervar:interval(*) AND name(TotalTravelTime:mean)" -F CSV-S

echo "DensityAtJunction"
scavetool export files/*.sca -o DensityAtJunction.csv -f "itervar:interval(*) AND name(DensityAtJunction:mean)" -F CSV-S

echo "DensityAtJunctionVector"
perl ../opp_vec2csv.pl -A iterationvars$interval -A repetition -F DensityAtJunction:vector files/*.vec > DensityAtJunctionVector.csv

echo "DestinationCountVector"
perl ../opp_vec2csv.pl -A iterationvars$interval -A repetition -F DestinationCount:vector files/*.vec > DestinationCountVector.csv

echo "WaitTimeAtJunction"
scavetool export files/*.sca -o WaitTimeAtJunction.csv -f "itervar:interval(*) AND name(WaitTimeAtJunction:mean)" -F CSV-S

echo "WaitTimeAtJunctionEast"
scavetool export files/*.sca -o WaitTimeAtJunctionEast.csv -f "itervar:interval(*) AND name(WaitTimeAtJunctionEast:mean)" -F CSV-S
echo "WaitTimeAtJunctionWest"
scavetool export files/*.sca -o WaitTimeAtJunctionWest.csv -f "itervar:interval(*) AND name(WaitTimeAtJunctionWest:mean)" -F CSV-S
echo "WaitTimeAtJunctionNorth"
scavetool export files/*.sca -o WaitTimeAtJunctionNorth.csv -f "itervar:interval(*) AND name(WaitTimeAtJunctionNorth:mean)" -F CSV-S
echo "WaitTimeAtJunctionSouth"
scavetool export files/*.sca -o WaitTimeAtJunctionSouth.csv -f "itervar:interval(*) AND name(WaitTimeAtJunctionSouth:mean)" -F CSV-S


echo "WaitTimeAtJunctionVector"
perl ../opp_vec2csv.pl -A iterationvars$interval -A repetition -F WaitTimeAtJunction:vector files/*.vec > WaitTimeAtJunctionVector.csv

echo "WaitTimeAtJunctionEastVector"
perl ../opp_vec2csv.pl -A iterationvars$interval -A repetition -F WaitTimeAtJunctionEast:vector files/*.vec > WaitTimeAtJunctionEastVector.csv
echo "WaitTimeAtJunctionWestVector"
perl ../opp_vec2csv.pl -A iterationvars$interval -A repetition -F WaitTimeAtJunctionWest:vector files/*.vec > WaitTimeAtJunctionWestVector.csv
echo "WaitTimeAtJunctionNorthVector"
perl ../opp_vec2csv.pl -A iterationvars$interval -A repetition -F WaitTimeAtJunctionNorth:vector files/*.vec > WaitTimeAtJunctionNorthVector.csv
echo "WaitTimeAtJunctionSouthVector"
perl ../opp_vec2csv.pl -A iterationvars$interval -A repetition -F WaitTimeAtJunctionSouth:vector files/*.vec > WaitTimeAtJunctionSouthVector.csv

#echo "TotalRedsFaced"
#perl ../opp_vec2csv.pl -A iterationvars$interval -A repetition -F TotalRedsFaced:vector files/*.vec > TotalRedsFaced.csv

echo "TotalWaitingTime"
perl ../opp_vec2csv.pl -A iterationvars$interval -A repetition -F TotalWaitingTime:vector files/*.vec > TotalWaitingTimeVector.csv

echo "TotalTravelTime"
perl ../opp_vec2csv.pl -A iterationvars$interval -A repetition -F TotalTravelTime:vector files/*.vec > TotalTravelTimeVector.csv

echo "SingleHopWaitingTime"
perl ../opp_vec2csv.pl -A iterationvars$interval -A repetition -F SingleHopWaitingTime:vector files/*.vec > SingleHopWaitingTimeVector.csv

echo "DoubleHopWaitingTime"
perl ../opp_vec2csv.pl -A iterationvars$interval -A repetition -F DoubleHopWaitingTime:vector files/*.vec > DoubleHopWaitingTimeVector.csv

echo "TripleHopWaitingTime"
perl ../opp_vec2csv.pl -A iterationvars$interval -A repetition -F TripleHopWaitingTime:vector files/*.vec > TripleHopWaitingTimeVector.csv

echo "SingleHopTravelTime"
perl ../opp_vec2csv.pl -A iterationvars$interval -A repetition -F SingleHopTravelTime:vector files/*.vec > SingleHopTravelTimeVector.csv

echo "DoubleHopTravelTime"
perl ../opp_vec2csv.pl -A iterationvars$interval -A repetition -F DoubleHopTravelTime:vector files/*.vec > DoubleHopTravelTimeVector.csv

echo "TripleHopTravelTime"
perl ../opp_vec2csv.pl -A iterationvars$interval -A repetition -F TripleHopTravelTime:vector files/*.vec > TripleHopTravelTimeVector.csv

echo "ThroughputAtJunction"
perl ../opp_vec2csv.pl -v -A iterationvars$interval -A repetition -F ThroughputAtJunction:vector files/*.vec > ThroughputAtJunction.csv

echo "EastSignalCount"
scavetool export files/*.sca -o EastSignalCount.csv -f "itervar:interval(*) AND name(EastSignalCount)" -F CSV-S

echo "WestSignalCount"
scavetool export files/*.sca -o WestSignalCount.csv -f "itervar:interval(*) AND name(WestSignalCount)" -F CSV-S

echo "NorthSignalCount"
scavetool export files/*.sca -o NorthSignalCount.csv -f "itervar:interval(*) AND name(NorthSignalCount)" -F CSV-S

echo "SouthSignalCount"
scavetool export files/*.sca -o SouthSignalCount.csv -f "itervar:interval(*) AND name(SouthSignalCount)" -F CSV-S

echo "EastThroughputAtJunction"
perl ../opp_vec2csv.pl -A iterationvars$interval -A repetition -F EastThroughputAtJunction:vector files/*.vec > EastThroughputAtJunction.csv
 
echo "WestThroughputAtJunction"
perl ../opp_vec2csv.pl -A iterationvars$interval -A repetition -F WestThroughputAtJunction:vector files/*.vec > WestThroughputAtJunction.csv
 
echo "NorthThroughputAtJunction"
perl ../opp_vec2csv.pl -A iterationvars$interval -A repetition -F NorthThroughputAtJunction:vector files/*.vec > NorthThroughputAtJunction.csv
 
echo "SouthThroughputAtJunction"
perl ../opp_vec2csv.pl -A iterationvars$interval -A repetition -F SouthThroughputAtJunction:vector files/*.vec > SouthThroughputAtJunction.csv

echo "\nHello World!"
