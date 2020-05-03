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

#ifndef __CROSSTRAFFIC_ENDPOINT_H_
#define __CROSSTRAFFIC_ENDPOINT_H_

#include <omnetpp.h>
#include "VehicleMsg_m.h"

using namespace omnetpp;

class Endpoint : public cSimpleModule
{
  public:
    virtual ~Endpoint();
  protected:
    virtual void initialize();
    virtual void handleVehicleMsg(VehicleMsg *vh_msg);
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
    virtual void recordStatistics(VehicleMsg *vh_msg);
  private:
    cMessage *spawnTimer = nullptr;
    std::string myName;
    int wrongDestination, totalVehiclesReached;
    int totalVehiclesSpawned;
    static unsigned int vehicleCounter;
    static std::map<char, std::string> directionMapping;
    simsignal_t total_travel_time, total_waiting_time, total_hops_travelled, total_reds_faced;
    simsignal_t singleHopWaitingTime, doubleHopWaitingTime, tripleHopWaitingTime;
    simsignal_t singleHopTravelTime, doubleHopTravelTime, tripleHopTravelTime, destCount;
    static std::map<std::string, std::list<char>> endPointGen;
    void spawnVehicle();
    std::string getDstEndpoint();
};

#endif
