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

#include "Endpoint.h"
#include "VehicleMsg_m.h"

Define_Module(Endpoint);

unsigned int Endpoint::vehicleCounter = 0;

std::map<char, std::string> Endpoint::directionMapping = {
        {'0', "EndWN"},
        {'1', "EndNW"},
        {'2', "EndNE"},
        {'3', "EndEN"},
        {'4', "EndES"},
        {'5', "EndSE"},
        {'6', "EndSW"},
        {'7', "EndWS"},
};

std::map<std::string, std::list<char>> Endpoint::endPointGen = {
        {"E", {'W', 'N', 'S'}},
        {"W", {'E', 'N', 'S'}},
        {"N", {'W', 'E', 'S'}},
        {"S", {'W', 'N', 'E'}},
};

Endpoint::~Endpoint() {
    cancelAndDelete(spawnTimer);
}

void Endpoint::initialize() {
    myName = getName();
    wrongDestination = 0;
    totalVehiclesReached = 0;
    totalVehiclesSpawned = 0;
    spawnTimer = new cMessage("spawnTimer");
    total_travel_time = registerSignal("TotalTravelTime");
    total_waiting_time = registerSignal("TotalWaitingTime");
    total_hops_travelled = registerSignal("TotalHopsTravelled");
    total_reds_faced = registerSignal("TotalRedsFaced");

    singleHopWaitingTime = registerSignal("SingleHopWaitingTime");
    doubleHopWaitingTime = registerSignal("DoubleHopWaitingTime");
    tripleHopWaitingTime = registerSignal("TripleHopWaitingTime");

    singleHopTravelTime = registerSignal("SingleHopTravelTime");
    doubleHopTravelTime = registerSignal("DoubleHopTravelTime");
    tripleHopTravelTime = registerSignal("TripleHopTravelTime");

    destCount = registerSignal("DestinationCount");

    simtime_t spawnInterval = par("spawnInterval");
    if(spawnInterval > 0) {
        scheduleAt(simTime() + par("spawnInterval"), spawnTimer);
    }
}

void Endpoint::handleMessage(cMessage *msg) {
    if (VehicleMsg* vh_msg = dynamic_cast<VehicleMsg*>(msg)) {
            handleVehicleMsg(vh_msg);
    } else if(msg == spawnTimer) {
        spawnVehicle();
        scheduleAt(simTime() + par("spawnInterval"), spawnTimer);
    }
}

void Endpoint::handleVehicleMsg(VehicleMsg *vh_msg) {
    std::cout << getName() << " received msg with destination - " << vh_msg->getDstEndpoint() << endl;
    if (strcmp(vh_msg->getDstEndpoint(), myName.c_str()) != 0){
        wrongDestination++;
    } else {
        totalVehiclesReached++;
    }
    vh_msg->setEndTime(simTime());
    vh_msg->setTravelTime(vh_msg->getEndTime().dbl() - vh_msg->getStartTime().dbl());
    recordStatistics(vh_msg);
    delete vh_msg;
}

void Endpoint::recordStatistics(VehicleMsg *vh_msg){
    if (vh_msg->getHops() == 1){
        emit(singleHopTravelTime, vh_msg->getTravelTime());
        emit(singleHopWaitingTime, vh_msg->getWaitTime());
    } else if (vh_msg->getHops() == 2){
        emit(doubleHopTravelTime, vh_msg->getTravelTime());
        emit(doubleHopWaitingTime, vh_msg->getWaitTime());
    } else {
        emit(tripleHopTravelTime, vh_msg->getTravelTime());
        emit(tripleHopWaitingTime, vh_msg->getWaitTime());
    }
    emit(total_travel_time, vh_msg->getTravelTime());
    emit(total_waiting_time, vh_msg->getWaitTime());
    emit(total_hops_travelled, vh_msg->getHops());
    emit(total_reds_faced, vh_msg->getTotalRedFaced());
}

void Endpoint::spawnVehicle() {
    totalVehiclesSpawned++;
    VehicleMsg *veh = new VehicleMsg();
    veh->setVehNumber(vehicleCounter++);
    veh->setId(vehicleCounter);
    veh->setSrcEndpoint(getName());
    std::string dstEndpoint = getDstEndpoint();
    veh->setDstEndpoint(dstEndpoint.c_str());
    veh->setName(dstEndpoint.c_str());
    veh->setStartTime(simTime());
    veh->setTempStartTime(simTime());
    std::string temp = "";
    temp.push_back(myName.at(3));
    veh->setTravellingDirection(temp.c_str());
    double delay = par("endPointToJunctionDelay");
    sendDelayed(veh, delay, "conn$o");
}

std::string Endpoint::getDstEndpoint() {
    /* Assign a random destination in directionMapping variable.
     * Random destination should never be equal to the current end-point */
    nameloop: std::string dstName;
    //random number for spawning new vehicles
    int x = intrand(8);
    std::string s = std::to_string(x);
    EV<<"Random number "<<x<<"\n";
    dstName = directionMapping.at(s[0]);
    if (myName.compare(dstName) == 0)
        goto nameloop; // When the generated destination is the current end-point, choose another destination.
    ASSERT(myName.size() == dstName.size());
    emit(destCount, (double) x);
    return dstName;
}

void Endpoint::finish(){
    recordScalar("WrongDestVehicles", wrongDestination);
    recordScalar("TotalVehiclesSpawned", totalVehiclesSpawned);
    recordScalar("TotalVehiclesReached", totalVehiclesReached);
}
