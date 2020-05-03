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

#ifndef __CROSSTRAFFIC_JUNCTION_H_
#define __CROSSTRAFFIC_JUNCTION_H_

#include <omnetpp.h>
#include "VehicleMsg_m.h"
#include "JunctionMsg_m.h"

using namespace omnetpp;

class Junction : public cSimpleModule
{
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void handleVehicleMsg(VehicleMsg *veh);
    virtual void handleJunctionMsg(JunctionMsg *jun);
    virtual void handleSelfMsg(cMessage *msg);
    virtual void addToQueue(VehicleMsg *msg, std::string dir);
    virtual void forwardTraffic(std::string dir);
    virtual int nextTrafficGreenPhase();
    virtual void recordStatistics(std::string dir, VehicleMsg* msg);
    virtual void refreshDisplay() const override;
    virtual void recordDirThroughput(std::string dir,double density);
    virtual void sendJunctionMsg();
    virtual void finish();
    virtual void dirForDiagonalDest();
  private:
    cQueue *northQueue, *southQueue, *eastQueue, *westQueue;
    bool greenFlag;
    double phaseTiming;
    simtime_t signalEndTime;
    int trafficFlag,eastSignalCount,westSignalCount,northSignalCount,southSignalCount; //0,1,2,3
    int dirOneLength, dirTwoLength;
    cMessage *selfMsg, *throughputMsg, *signalStart, *signalEnd;
    std::string myName;
    std::list<std::string> junctionsNearMe, endPointsNearMe, dirOfJunctionsNearMe;
    static std::map<std::string, std::string> gateMapping;
    static std::map<std::string, std::list<std::string>> junctionEndPointMapping;
    static std::map<std::string, std::string> endPointJunctionMapping;
    static std::map<std::string, std::list<std::string>> junctionToJunctionMapping;
    static std::map<std::string, std::list<std::string>> junctionDirectionsMapping;
    std::list<int> neighbourQueueLengths;
    static std::map<std::string, int> directionToFlagMapping;
    static std::map<std::string, std::string> rightDirectionMapping, negationDirectionMapping;
    static std::map<int, std::string> intToDirectionMapping;
    simsignal_t eastQueueSignal, westQueueSignal, northQueueSignal, southQueueSignal;
    simsignal_t densityAtJunction, waitTimeAtJunction, junctionUtilization, waitTimeAtJunctionEast
    , waitTimeAtJunctionWest, waitTimeAtJunctionNorth, waitTimeAtJunctionSouth, throughputAtJunction;
    simsignal_t eastThroughputAtJunction, westThroughputAtJunction, northThroughputAtJunction,southThroughputAtJunction;
    double eastDirThroughput ,westDirThroughput ,northDirThroughput ,southDirThroughput,junctionThroughPut ;
};

#endif
