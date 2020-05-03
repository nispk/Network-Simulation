import pandas as pd
import csv
import matplotlib.pyplot as plt
import numpy as np
import math
from decimal import Decimal
from mpl_toolkits.axes_grid1.inset_locator import zoomed_inset_axes 
from mpl_toolkits.axes_grid1.inset_locator import mark_inset

import pdb
tws = pd.read_csv("TotalVehiclesSpawned.csv")
tws = tws.groupby(['Module', 'Name'], as_index=False).mean()
tws['Module'] = tws['Module'].apply(lambda x: x[-5:])
unique_ends = tws['Name'].unique()
tws.pivot(index='Module', columns='Name', values='Unnamed: 4').plot(kind='bar')
plt.xlabel("Endpoints")
plt.xticks(rotation=360)
plt.ylabel("Number of vehicles")
plt.savefig("Collisions.pdf")
plt.clf()

wdv = pd.read_csv("WrongDestVehicles.csv")
unique_reps = wdv['repetition'].unique()
rows_to_plot = wdv.groupby(['repetition', 'Module'], as_index=False).mean()
rows_to_plot['Module'] = rows_to_plot['Module'].apply(lambda x: x[-5:])
rows_to_plot = rows_to_plot.rename(columns={'Module': 'Endpoints'})
rows_to_plot.pivot(index='repetition', columns='Endpoints', values='Unnamed: 4').plot(kind='bar')
plt.ylim(0,5)
plt.xlabel("Repetitions")
plt.ylabel("Number of vehicles reached wrong destination")
plt.savefig("WrongDestination.pdf")
plt.clf()

daj = pd.read_csv("DensityAtJunction.csv")
rows_for_interval = daj.groupby(['repetition', 'Module'], as_index=False).mean()
rows_for_interval['Module'] = rows_for_interval['Module'].apply(lambda x: x[-6:])
unique_juncs = rows_for_interval['Module'].unique()
mean_list = []
std_list = []
xlabels = []
for junction in unique_juncs:
	rows_for_jun = rows_for_interval[rows_for_interval['Module'] == junction]
	xlabels.append(junction)
	mean_list.append(rows_for_jun['Unnamed: 4'].mean())
	std_list.append(rows_for_jun['Unnamed: 4'].std())
left = np.arange(len(unique_juncs))
plt.bar(left, mean_list, tick_label=xlabels, color="darkblue", width=0.3, yerr=std_list, align='center', alpha=0.5, ecolor='black', capsize=4)
plt.xlabel("Junctions")
plt.ylabel("Density of vehicles")
plt.savefig("DensityAtJunction.pdf")
plt.clf()

daj = pd.read_csv("WaitTimeAtJunction.csv")
daj['Module'] = daj['Module'].apply(lambda x: x[-6:])
#rows_for_interval = daj.groupby(['repetition', 'Module'], as_index=False).mean()
unique_juncs = daj['Module'].unique()
mean_list = []
std_list = []
xlabels = []
for junction in unique_juncs:
	rows_for_jun = daj[daj['Module'] == junction]
	xlabels.append(junction)
	mean_list.append(rows_for_jun['Unnamed: 4'].mean())
	std_list.append(rows_for_jun['Unnamed: 4'].std())
left = np.arange(len(unique_juncs))
plt.bar(left, mean_list, tick_label=xlabels, color="blue", width=0.3, yerr=std_list, align='center', alpha=0.5, ecolor='black', capsize=4)
#rows_for_interval.pivot(index='repetition', columns='Module', values='Unnamed: 4').plot(kind='bar')
plt.xlabel("Junctions")
plt.ylabel("Average Waiting time (seconds)")
plt.savefig("WaitTimeAtJunction.pdf")
plt.clf()


twv = pd.read_csv("ThroughputAtJunction.csv", sep="\t")
twv = twv.groupby(['repetition', 'node'], as_index=False).mean()
twv['node'] = twv['node'].apply(lambda x: x[-6:])
unique_juncs = twv['node'].unique()
mean_list = []
std_list = []
xlabels = []
for junction in unique_juncs:
	rows_for_jun = twv[twv['node'] == junction]
	xlabels.append(junction)
	mean_list.append(rows_for_jun['ThroughputAtJunction:vector'].mean())
	std_list.append(rows_for_jun['ThroughputAtJunction:vector'].std())
plt.bar(left, mean_list, tick_label=xlabels, color="blue", width=0.3, yerr=std_list, align='center', alpha=0.5, ecolor='black', capsize=4)
#rows_to_plot = twv.groupby(['repetition', 'node'], as_index=False).mean()
#rows_to_plot.pivot(index='repetition', columns='node', values='ThroughputAtJunction:vector').plot(kind='bar')
plt.xlabel("Junctions")
plt.ylabel("Average number of vehicles leaving a junction.")
plt.savefig("ThroughputAtJunction.pdf")
plt.clf()

twv = pd.read_csv("SingleHopWaitingTimeVector.csv", sep="\t")
dhv = pd.read_csv("DoubleHopWaitingTimeVector.csv", sep="\t")
thv = pd.read_csv("TripleHopWaitingTimeVector.csv", sep="\t")
twv['node'] = twv['node'].apply(lambda x: x[-5:])
dhv['node'] = dhv['node'].apply(lambda x: x[-5:])
thv['node'] = thv['node'].apply(lambda x: x[-5:])
twvv = twv.groupby(['node', 'repetition'], as_index=False).mean()
twvv = twvv.groupby(['node'], as_index=False).mean()
twvv['std'] = twv.groupby(['node'])['SingleHopWaitingTime:vector'].std().reset_index()['SingleHopWaitingTime:vector']
dhvv = dhv.groupby(['node', 'repetition'], as_index=False).mean()
dhvv = dhvv.groupby(['node'], as_index=False).mean()
dhvv['std'] = dhv.groupby(['node'])['DoubleHopWaitingTime:vector'].std().reset_index()['DoubleHopWaitingTime:vector']
thvv = thv.groupby(['node', 'repetition'], as_index=False).mean()
thvv = thvv.groupby(['node'], as_index=False).mean()
thvv['std'] = thv.groupby(['node'])['TripleHopWaitingTime:vector'].std().reset_index()['TripleHopWaitingTime:vector']

twvv['iterationvars'] = 'OneHop'
twvv = twvv.rename(columns={'SingleHopWaitingTime:vector': 'vector', 'node':'Endpoints'})

dhvv['iterationvars'] = 'TwoHop'
dhvv = dhvv.rename(columns={'DoubleHopWaitingTime:vector': 'vector', 'node':'Endpoints'})

thvv['iterationvars'] = 'ThreeHop'
thvv = thvv.rename(columns={'TripleHopWaitingTime:vector': 'vector', 'node':'Endpoints'})
list_to_plot = [dhvv, thvv]
con = pd.concat(list_to_plot)
con.pivot(index='iterationvars', columns='Endpoints', values='vector').plot(kind='bar', yerr=con.pivot(index='iterationvars', columns='Endpoints', values='std'))
plt.xticks([0,1],['ThreeJunctions','TwoJunctions'])
plt.xlabel("Via number of junctions travelled")
plt.xticks(rotation=360)
plt.ylabel("Average waiting time (seconds)")
plt.savefig("WaitingTime.pdf")
plt.clf()
plt.clf()
twv['iterationvars'] = 'OneHop'
dhv['iterationvars'] = 'TwoHop'
thv['iterationvars'] = 'ThreeHop'
mean_list = []
mean_list.append(twv['SingleHopWaitingTime:vector'].mean())
std_list = []
std_list.append(twv['SingleHopWaitingTime:vector'].std())
mean_list.append(dhv['DoubleHopWaitingTime:vector'].mean())
std_list.append(dhv['DoubleHopWaitingTime:vector'].std())
mean_list.append(thv['TripleHopWaitingTime:vector'].mean())
std_list.append(thv['TripleHopWaitingTime:vector'].std())
plt.bar(np.arange(3), mean_list, tick_label=['OneHop', 'TwoHop', 'ThreeHop'], yerr = std_list, align='center', alpha=0.5, ecolor='black', capsize=4, color='blue', width=0.3)
plt.savefig("WaitingTimeCombined.pdf")
plt.clf()


ewt = pd.read_csv("WaitTimeAtJunctionEast.csv")
wwt = pd.read_csv("WaitTimeAtJunctionWest.csv")
nwt = pd.read_csv("WaitTimeAtJunctionNorth.csv")
swt = pd.read_csv("WaitTimeAtJunctionSouth.csv")
ewt['Name'] = 'East'
ewt['Module'] = ewt['Module'].apply(lambda x: x[-6:])
ewtt = ewt.groupby(['Module', 'Name'], as_index=False).mean()
ewtt['std'] = ewt.groupby(['Module', 'Name'])['Unnamed: 4'].std().reset_index()['Unnamed: 4']
wwt['Name'] = 'West'
wwt['Module'] = wwt['Module'].apply(lambda x: x[-6:])
wwtt = wwt.groupby(['Module', 'Name'], as_index=False).mean()
wwtt['std'] = wwt.groupby(['Module', 'Name'])['Unnamed: 4'].std().reset_index()['Unnamed: 4']
nwt['Name'] = 'North'
nwt['Module'] = nwt['Module'].apply(lambda x: x[-6:])
nwtt = nwt.groupby(['Module', 'Name'], as_index=False).mean()
nwtt['std'] = nwt.groupby(['Module', 'Name'])['Unnamed: 4'].std().reset_index()['Unnamed: 4']
swt['Name'] = 'South'
swt['Module'] = swt['Module'].apply(lambda x: x[-6:])
swtt = swt.groupby(['Module', 'Name'], as_index=False).mean()
swtt['std'] = swt.groupby(['Module', 'Name'])['Unnamed: 4'].std().reset_index()['Unnamed: 4']
list_to_plot = [ewtt, wwtt, nwtt, swtt]
con = pd.concat(list_to_plot)
con = con.rename(columns={'Name': 'Directions'})
#con = con.groupby(['Module', 'Name'], as_index=False).mean()
con.pivot(index='Module', columns='Directions', values='Unnamed: 4').plot(kind='bar', yerr=con.pivot(index='Module', columns='Directions', values='std'))
plt.xlabel("Junctions")
plt.xticks(rotation=360)
plt.ylabel("Average waiting time (seconds)")
plt.savefig("WaitTimePerDirPerJunc.pdf")
plt.clf()

twv = pd.read_csv("SingleHopTravelTimeVector.csv", sep="\t")
dhv = pd.read_csv("DoubleHopTravelTimeVector.csv", sep="\t")
thv = pd.read_csv("TripleHopTravelTimeVector.csv", sep="\t")
twv['node'] = twv['node'].apply(lambda x: x[-5:])
dhv['node'] = dhv['node'].apply(lambda x: x[-5:])
thv['node'] = thv['node'].apply(lambda x: x[-5:])

twvv = twv.groupby(['node', 'repetition'], as_index=False).mean()
twvv = twvv.groupby(['node'], as_index=False).mean()
twvv['std'] = twv.groupby(['node'])['SingleHopTravelTime:vector'].std().reset_index()['SingleHopTravelTime:vector']
dhvv = dhv.groupby(['node', 'repetition'], as_index=False).mean()
dhvv = dhvv.groupby(['node'], as_index=False).mean()
dhvv['std'] = dhv.groupby(['node'])['DoubleHopTravelTime:vector'].std().reset_index()['DoubleHopTravelTime:vector']
thvv = thv.groupby(['node', 'repetition'], as_index=False).mean()
thvv = thvv.groupby(['node'], as_index=False).mean()
thvv['std'] = thv.groupby(['node'])['TripleHopTravelTime:vector'].std().reset_index()['TripleHopTravelTime:vector']

twvv['iterationvars'] = 'OneHop'
twvv = twvv.rename(columns={'SingleHopTravelTime:vector': 'vector', 'node':'Endpoints'})

dhvv['iterationvars'] = 'TwoHop'
dhvv = dhvv.rename(columns={'DoubleHopTravelTime:vector': 'vector', 'node':'Endpoints'})

thvv['iterationvars'] = 'ThreeHop'
thvv = thvv.rename(columns={'TripleHopTravelTime:vector': 'vector', 'node':'Endpoints'})

list_to_plot = [dhvv, thvv]
con = pd.concat(list_to_plot)
con.pivot(index='iterationvars', columns='Endpoints', values='vector').plot(kind='bar', yerr=con.pivot(index='iterationvars', columns='Endpoints', values='std'))
plt.xticks([0,1],['ThreeJunctions','TwoJunctions'])
plt.xlabel("")
plt.xticks(rotation=360)
plt.legend(loc="upper right")
plt.xlabel("Via number of junctions travelled")
plt.ylabel("Average travel time (seconds)")
plt.savefig("TravelTime.pdf")
plt.clf()
twv['iterationvars'] = 'OneHop'
dhv['iterationvars'] = 'TwoHop'
thv['iterationvars'] = 'ThreeHop'
mean_list = []
mean_list.append(twv['SingleHopTravelTime:vector'].mean())
std_list = []
std_list.append(twv['SingleHopTravelTime:vector'].std())
mean_list.append(dhv['DoubleHopTravelTime:vector'].mean())
std_list.append(dhv['DoubleHopTravelTime:vector'].std())
mean_list.append(thv['TripleHopTravelTime:vector'].mean())
std_list.append(thv['TripleHopTravelTime:vector'].std())
plt.bar(np.arange(3), mean_list, tick_label=['OneHop', 'TwoHop', 'ThreeHop'], yerr = std_list, align='center', alpha=0.5, ecolor='black', capsize=4, color='blue', width=0.3)
plt.savefig("TravelTimeCombined.pdf")
plt.clf()

destMap = {
	0: "EndWN",
        1: "EndNW",
        2: "EndNE",
        3: "EndEN",
        4: "EndES",
        5: "EndSE",
        6: "EndSW",
        7: "EndWS"
}
dc = pd.read_csv("DestinationCountVector.csv", sep="\t")
dc = dc.groupby('DestinationCount:vector', as_index=False).count()
dc['repetition'] = dc['repetition'].apply(lambda x: x/5)
dc['DestinationCount:vector'] = dc['DestinationCount:vector'].apply(lambda x: destMap[x])
left = np.arange(len(destMap))
plt.bar(left, dc['repetition'], tick_label=destMap.values(), width=0.3, align='center', alpha=0.5, ecolor='black', capsize=4)
plt.xlabel("End points")
plt.ylabel("Number of vehicles generated")
plt.savefig("DestinationCount.pdf")
plt.clf()

east = pd.read_csv("EastSignalCount.csv")
west = pd.read_csv("WestSignalCount.csv")
north = pd.read_csv("NorthSignalCount.csv")
south = pd.read_csv("SouthSignalCount.csv")
east.columns = ['run','repetition','module','name','values']
west.columns = ['run','repetition','module','name','values']
north.columns = ['run','repetition','module','name','values']
south.columns = ['run','repetition','module','name','values']
eastdir = east.groupby(['module'], as_index=False).mean()
eastdir['std'] = east.groupby(['module'])['values'].std().reset_index()['values']
westdir = west.groupby(['module'], as_index=False).mean()
westdir['std'] = west.groupby(['module'])['values'].std().reset_index()['values']
northdir = north.groupby(['module'], as_index=False).mean()
northdir['std'] = north.groupby(['module'])['values'].std().reset_index()['values']
southdir = south.groupby(['module'], as_index=False).mean()
southdir['std'] = south.groupby(['module'])['values'].std().reset_index()['values']
eastdir['name']='East'
westdir['name']='West'
northdir['name']='North'
southdir['name']='South'
list_to_plot = [eastdir, westdir, northdir, southdir]
con = pd.concat(list_to_plot)
con = con.rename(columns={'name': 'Directions'})
con['module'] = con['module'].apply(lambda x: x[-6:])
index = ['a','b','c','d']
con.pivot(index='module', columns='Directions', values='values').plot(kind='bar',yerr=con.pivot(index='module', columns='Directions', values='std'))
plt.xlabel("Junctions")
plt.xticks(rotation=360)
plt.legend(loc="upper right")
plt.ylabel("Average green signal phases")
plt.savefig("GreenSignalPhase.pdf")
plt.clf()

dcv = pd.read_csv("DestinationCountVector.csv", sep="\t")
dcv = dcv.groupby(['repetition', 'node', 'DestinationCount:vector'], as_index=False).count()
dcv['node'] = dcv['node'].apply(lambda x: x[-5:])
dcv = dcv.groupby(['node', 'DestinationCount:vector'], as_index=False).mean()
dcv['DestinationCount:vector'] = dcv['DestinationCount:vector'].apply(lambda x: destMap[x])
dcv = dcv.rename(columns={'DestinationCount:vector': 'Endpoints'})
dcv.pivot(index='node', columns='Endpoints', values='event').plot(kind='bar')
plt.xticks(rotation=360)
plt.xlabel("Endpoints")
plt.ylabel("#An EndPoint is chosen as a destination")
plt.savefig("DestinationCountPerEndPoint.pdf")
plt.clf()

east = pd.read_csv("EastThroughputAtJunction.csv", sep="\t")
west = pd.read_csv("WestThroughputAtJunction.csv", sep="\t")
north = pd.read_csv("NorthThroughputAtJunction.csv", sep="\t")
south = pd.read_csv("SouthThroughputAtJunction.csv", sep="\t")
east.columns = ['event','time','node','iterationvars','repetition','vector']
west.columns = ['event','time','node','iterationvars','repetition','vector']
north.columns = ['event','time','node','iterationvars','repetition','vector']
south.columns = ['event','time','node','iterationvars','repetition','vector']
east = east.groupby(['node'], as_index=False).mean()
west = west.groupby(['node'], as_index=False).mean()
north = north.groupby(['node'], as_index=False).mean()
south = south.groupby(['node'], as_index=False).mean()
east['name']='East'
west['name']='West'
north['name']='North'
south['name']='South'
list_to_plot = [east, west, north, south]
con = pd.concat([east])
con = pd.concat([con, west])
con = pd.concat([con, north])
con = pd.concat([con, south])
con['node'] = con['node'].apply(lambda x: x[-6:])
con = con.rename(columns={'name': 'Directions'})
index = ['a','b','c','d']
con.pivot(index='node', columns='Directions', values='vector').plot(kind='bar')
plt.xlabel("Junctions")
plt.xticks(rotation=360)
plt.ylabel("Number of vechiles per green phase")
plt.savefig("JunctionDirThroughput.pdf")
plt.clf()

print ("Bingo!")
