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

simple Endpoint
{
    parameters:
        @signal[TotalTravelTime](type="double");
        @statistic[TotalTravelTime](title="TotalTravelTime"; source="TotalTravelTime"; record=vector,sum,mean,min,max,count; interpolationmode=none);
        @signal[TotalWaitingTime](type="double");
        @statistic[TotalWaitingTime](title="TotalWaitingTime"; source="TotalWaitingTime"; record=vector,sum,mean,min,max,count; interpolationmode=none);
        @signal[TotalHopsTravelled](type="long");
        @statistic[TotalHopsTravelled](title="TotalHopsTravelled"; source="TotalHopsTravelled"; record=vector,sum,mean,min,max,count; interpolationmode=none);
        @signal[TotalRedsFaced](type="long");
        @statistic[TotalRedsFaced](title="TotalRedsFaced"; source="TotalRedsFaced"; record=vector,sum,mean,min,max,count; interpolationmode=none);
        
        @signal[SingleHopWaitingTime](type="double");
        @statistic[SingleHopWaitingTime](title="SingleHopWaitingTime"; source="SingleHopWaitingTime"; record=vector,sum,mean,min,max,count; interpolationmode=none);
        @signal[DoubleHopWaitingTime](type="double");
        @statistic[DoubleHopWaitingTime](title="DoubleHopWaitingTime"; source="DoubleHopWaitingTime"; record=vector,sum,mean,min,max,count; interpolationmode=none);
        @signal[TripleHopWaitingTime](type="double");
        @statistic[TripleHopWaitingTime](title="TripleHopWaitingTime"; source="TripleHopWaitingTime"; record=vector,sum,mean,min,max,count; interpolationmode=none);
        
        @signal[SingleHopTravelTime](type="double");
        @statistic[SingleHopTravelTime](title="SingleHopTravelTime"; source="SingleHopTravelTime"; record=vector,sum,mean,min,max,count; interpolationmode=none);
        @signal[DoubleHopTravelTime](type="double");
        @statistic[DoubleHopTravelTime](title="DoubleHopTravelTime"; source="DoubleHopTravelTime"; record=vector,sum,mean,min,max,count; interpolationmode=none);
        @signal[TripleHopTravelTime](type="double");
        @statistic[TripleHopTravelTime](title="TripleHopTravelTime"; source="TripleHopTravelTime"; record=vector,sum,mean,min,max,count; interpolationmode=none);
        
        @signal[DestinationCount](type="double");
        @statistic[DestinationCount](title="DestinationCount"; source="DestinationCount"; record=vector; interpolationmode=none);
        
        @display("i=block/rxtx");
        volatile double genid @unit() = default(1);
        volatile double spawnInterval @unit(s) = default(1s);
        volatile double endPointToJunctionDelay @unit() = default(3);
    gates:
        inout conn;
}
