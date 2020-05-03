//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package crosstraffic;

simple Junction
{
    parameters:
        @signal[EastQueueSignal](type="long");
        @statistic[EastQueueSignal](title="EastQueueSignal"; source="EastQueueSignal"; record=vector,sum,mean,min,max,count; interpolationmode=none);
        @signal[WestQueueSignal](type="long");
        @statistic[WestQueueSignal](title="WestQueueSignal"; source="WestQueueSignal"; record=vector,sum,mean,min,max,count; interpolationmode=none);
        @signal[NorthQueueSignal](type="long");
        @statistic[NorthQueueSignal](title="NorthQueueSignal"; source="NorthQueueSignal"; record=vector,sum,mean,min,max,count; interpolationmode=none);
        @signal[SouthQueueSignal](type="long");
        @statistic[SouthQueueSignal](title="SouthQueueSignal"; source="SouthQueueSignal"; record=vector,sum,mean,min,max,count; interpolationmode=none);
        
        @signal[DensityAtJunction](type="double");
        @statistic[DensityAtJunction](title="DensityAtJunction"; source="DensityAtJunction"; record=vector,sum,mean,min,max,count; interpolationmode=none);

        @signal[WaitTimeAtJunctionEast](type="double");
        @statistic[WaitTimeAtJunctionEast](title="WaitTimeAtJunctionEast"; source="WaitTimeAtJunctionEast"; record=vector,sum,mean,min,max,count; interpolationmode=none);
        @signal[WaitTimeAtJunctionWest](type="double");
        @statistic[WaitTimeAtJunctionWest](title="WaitTimeAtJunctionWest"; source="WaitTimeAtJunctionWest"; record=vector,sum,mean,min,max,count; interpolationmode=none);
        @signal[WaitTimeAtJunctionNorth](type="double");
        @statistic[WaitTimeAtJunctionNorth](title="WaitTimeAtJunctionNorth"; source="WaitTimeAtJunctionNorth"; record=vector,sum,mean,min,max,count; interpolationmode=none);
        @signal[WaitTimeAtJunctionSouth](type="double");
        @statistic[WaitTimeAtJunctionSouth](title="WaitTimeAtJunctionSouth"; source="WaitTimeAtJunctionSouth"; record=vector,sum,mean,min,max,count; interpolationmode=none);
        @signal[WaitTimeAtJunction](type="double");
        @statistic[WaitTimeAtJunction](title="WaitTimeAtJunction"; source="WaitTimeAtJunction"; record=vector,sum,mean,min,max,count; interpolationmode=none);
        
        @signal[ThroughputAtJunction](type="double");
        @statistic[ThroughputAtJunction](title="ThroughputAtJunction"; source="ThroughputAtJunction"; record=vector,sum,mean,min,max,count; interpolationmode=none);
        
        @signal[WestThroughputAtJunction](type="double");
        @statistic[WestThroughputAtJunction](title="WestThroughputAtJunction"; source="WestThroughputAtJunction"; record=vector,sum,mean,min,max,count; interpolationmode=none);
        @signal[EastThroughputAtJunction](type="double");
        @statistic[EastThroughputAtJunction](title="EastThroughputAtJunction"; source="EastThroughputAtJunction"; record=vector,sum,mean,min,max,count; interpolationmode=none);
        @signal[NorthThroughputAtJunction](type="double");
        @statistic[NorthThroughputAtJunction](title="NorthThroughputAtJunction"; source="NorthThroughputAtJunction"; record=vector,sum,mean,min,max,count; interpolationmode=none);
        @signal[SouthThroughputAtJunction](type="double");
        @statistic[SouthThroughputAtJunction](title="SouthThroughputAtJunction"; source="SouthThroughputAtJunction"; record=vector,sum,mean,min,max,count; interpolationmode=none);
        
        
        @display("i=block/routing");
        volatile double trafficChangeInterval @unit(s) = default(-1s);
        volatile double accelerationDelay @unit() = default(0.1);
        volatile double junctionToJunctionDelay = default(0.1);
        double phaseChangeDelay @unit(s) = default(1s);
        double phaseTiming @unit(s) = default(3s);
    gates:
        inout north;
        inout east;
        inout south;
        inout west;
}
