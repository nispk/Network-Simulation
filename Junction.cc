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

#include <omnetpp.h>
#include <string.h>
#include "Junction.h"
#include "VehicleMsg_m.h"
#include "JunctionMsg_m.h"

Define_Module(Junction);

std::map<std::string, std::string> Junction::gateMapping = {
    {"N", "north$o"},
    {"S", "south$o"},
    {"E", "east$o"},
    {"W", "west$o"},
};

std::map<std::string, std::list<std::string>> Junction::junctionEndPointMapping = {
    //JUNCTION - [DIRONE, DIRTWO]
    {"JuncNW", {"EndWN", "EndNW"}},
    {"JuncNE", {"EndEN", "EndNE"}},
    {"JuncSW", {"EndWS", "EndSW"}},
    {"JuncSE", {"EndES", "EndSE"}},
};

std::map<std::string, std::string> Junction::endPointJunctionMapping = {
        {"EndWN", "JuncNW"},
        {"EndNW", "JuncNW"},
        {"EndNE", "JuncNE"},
        {"EndEN", "JuncNE"},
        {"EndSW", "JuncSW"},
        {"EndWS", "JuncSW"},
        {"EndSE", "JuncSE"},
        {"EndES", "JuncSE"},
};

std::map<std::string, std::list<std::string>> Junction::junctionToJunctionMapping = {
        //JUNCTION - [DIRONE, DIRTWO]
        {"JuncNW", {"JuncNE", "JuncSW"}},
        {"JuncNE", {"JuncNW", "JuncSE"}},
        {"JuncSW", {"JuncSE", "JuncNW"}},
        {"JuncSE", {"JuncSW", "JuncNE"}},
};

std::map<std::string, std::list<std::string>> Junction::junctionDirectionsMapping = {
        //JUNCTION - [DIRONE, DIRTWO]
        {"JuncNW", {"E", "S"}},
        {"JuncNE", {"W", "S"}},
        {"JuncSW", {"E", "N"}},
        {"JuncSE", {"W", "N"}},
};

std::map<std::string, std::string> Junction::rightDirectionMapping = {
        {"W", "S"},
        {"S", "E"},
        {"E", "N"},
        {"N", "W"},
};

std::map<std::string, std::string> Junction::negationDirectionMapping = {
        {"E", "W"},
        {"W", "E"},
        {"S", "N"},
        {"N", "S"},
};

std::map<std::string, int> Junction::directionToFlagMapping = {
        {"E", 0},
        {"W", 1},
        {"N", 2},
        {"S", 3},
};

std::map<int, std::string> Junction::intToDirectionMapping = {
        {0, "E"},
        {1, "W"},
        {2, "N"},
        {3, "S"},
};

void Junction::initialize()
{
    phaseTiming = par("phaseTiming");
    northQueue = new cQueue();
    southQueue = new cQueue();
    eastQueue = new cQueue();
    westQueue = new cQueue();
    greenFlag = true;
    trafficFlag = 0; //0 - East, 1 - West, 2 - North, 3 - South
    WATCH(trafficFlag);
    signalEndTime = simTime();
    eastSignalCount = 0;
    westSignalCount = 0;
    junctionThroughPut = 0;
    northSignalCount = 0;
    southSignalCount = 0;
    WATCH(eastSignalCount);
    WATCH(westSignalCount);
    WATCH(northSignalCount);
    WATCH(southSignalCount);
    selfMsg = new cMessage("start");
    myName = getName();
    neighbourQueueLengths = {0, 0};
    dirOneLength = 0;
    dirTwoLength = 0;
    eastDirThroughput = 0;
    westDirThroughput = 0;
    northDirThroughput = 0;
    southDirThroughput = 0;
    throughputMsg = new cMessage("Throughput");
    signalStart = new cMessage("SignalStart");
    signalEnd = new cMessage("SignalEnd");
    scheduleAt(simTime() + phaseTiming, signalStart);
    junctionsNearMe = junctionToJunctionMapping[getName()];
    dirOfJunctionsNearMe = junctionDirectionsMapping[getName()];
    endPointsNearMe = junctionEndPointMapping[getName()];
    //For statistics
    eastQueueSignal = registerSignal("EastQueueSignal");
    westQueueSignal = registerSignal("WestQueueSignal");
    northQueueSignal = registerSignal("NorthQueueSignal");
    southQueueSignal = registerSignal("SouthQueueSignal");
    waitTimeAtJunction = registerSignal("WaitTimeAtJunction");
    densityAtJunction = registerSignal("DensityAtJunction");
    waitTimeAtJunctionEast = registerSignal("WaitTimeAtJunctionEast");
    waitTimeAtJunctionWest = registerSignal("WaitTimeAtJunctionWest");
    waitTimeAtJunctionNorth = registerSignal("WaitTimeAtJunctionNorth");
    waitTimeAtJunctionSouth = registerSignal("WaitTimeAtJunctionSouth");
    throughputAtJunction = registerSignal("ThroughputAtJunction");
    eastThroughputAtJunction = registerSignal("EastThroughputAtJunction");
    westThroughputAtJunction = registerSignal("WestThroughputAtJunction");
    northThroughputAtJunction = registerSignal("NorthThroughputAtJunction");
    southThroughputAtJunction = registerSignal("SouthThroughputAtJunction");
    scheduleAt(simTime() + 5, throughputMsg);
}

void Junction::sendJunctionMsg(){
    /*
     * Sends the required queue length for a neighboring junctions when
     * invoked. */
    if (strcmp(myName.c_str(), "JuncNW") == 0){
        JunctionMsg *jmsg = new JunctionMsg();
        jmsg->setName(myName.c_str());
        jmsg->setQueueLength(southQueue->getLength());
        std::string dir1(1, myName.at(5));
        send(jmsg, gateMapping[negationDirectionMapping[dir1.c_str()].c_str()].c_str());

        jmsg = new JunctionMsg();
        jmsg->setName(myName.c_str());
        jmsg->setQueueLength(eastQueue->getLength());
        std::string dir2(1, myName.at(4));
        send(jmsg, gateMapping[negationDirectionMapping[dir2.c_str()].c_str()].c_str());

    } else if (strcmp(myName.c_str(), "JuncNE") == 0){
        JunctionMsg *jmsg = new JunctionMsg();
        jmsg->setName(myName.c_str());
        jmsg->setQueueLength(southQueue->getLength());
        std::string dir1(1, myName.at(5));
        send(jmsg, gateMapping[negationDirectionMapping[dir1.c_str()].c_str()].c_str());

        jmsg = new JunctionMsg();
        jmsg->setName(myName.c_str());
        jmsg->setQueueLength(westQueue->getLength());
        std::string dir2(1, myName.at(4));
        send(jmsg, gateMapping[negationDirectionMapping[dir2.c_str()].c_str()].c_str());
    } else if (strcmp(myName.c_str(), "JuncSE") == 0){
        JunctionMsg *jmsg = new JunctionMsg();
        jmsg->setName(myName.c_str());
        jmsg->setQueueLength(northQueue->getLength());
        std::string dir1(1, myName.at(5));
        send(jmsg, gateMapping[negationDirectionMapping[dir1.c_str()].c_str()].c_str());

        jmsg = new JunctionMsg();
        jmsg->setName(myName.c_str());
        jmsg->setQueueLength(westQueue->getLength());
        std::string dir2(1, myName.at(4));
        send(jmsg, gateMapping[negationDirectionMapping[dir2.c_str()].c_str()].c_str());

    } else if (strcmp(myName.c_str(), "JuncSW") == 0){
        JunctionMsg *jmsg = new JunctionMsg();
        jmsg->setName(myName.c_str());
        jmsg->setQueueLength(northQueue->getLength());
        std::string dir1(1, myName.at(5));
        send(jmsg, gateMapping[negationDirectionMapping[dir1.c_str()].c_str()].c_str());

        jmsg = new JunctionMsg();
        jmsg->setName(myName.c_str());
        jmsg->setQueueLength(eastQueue->getLength());
        std::string dir2(1, myName.at(4));
        send(jmsg, gateMapping[negationDirectionMapping[dir2.c_str()].c_str()].c_str());

    }
}

void Junction::addToQueue(VehicleMsg *msg, std::string dir){
    /*
     * Adds the vehicle into the queue based on the direction received.*/

    msg->setTotalRedFaced(msg->getTotalRedFaced() + 1);
    msg->setTempJunctionWaitTime(simTime());
    if (strcmp(dir.c_str(), "E") == 0){
        EV << myName << " - adding vehicle with destination - " << msg->getDstEndpoint() << " to east queue." << endl;
        eastQueue->insert(msg);
        emit(eastQueueSignal, eastQueue->getLength());
    } else if (strcmp(dir.c_str(), "W") == 0){
        EV << myName << " - adding vehicle with destination - " << msg->getDstEndpoint() << " to west queue." << endl;
        westQueue->insert(msg);
        emit(westQueueSignal, westQueue->getLength());
    } else if (strcmp(dir.c_str(), "N") == 0){
        EV << myName << " - adding vehicle with destination - " << msg->getDstEndpoint() << " to north queue." << endl;
        northQueue->insert(msg);
        emit(northQueueSignal, northQueue->getLength());
    } else if (strcmp(dir.c_str(), "S") == 0){
        EV << myName << " - adding vehicle with destination - " << msg->getDstEndpoint() << " to south queue." << endl;
        southQueue->insert(msg);
        emit(southQueueSignal, southQueue->getLength());
    }
}

void Junction::handleVehicleMsg(VehicleMsg *vh_msg){
    /*
     * Based on the traffic phase and direction of travel, the received vehicle
     * is added to the queue or forwarded via the respective out gate.*/

    std::string dest = vh_msg->getDstEndpoint();
    std::string destJunction = endPointJunctionMapping[dest];
    vh_msg->setHops(vh_msg->getHops() + 1);
    if (strcmp(destJunction.c_str(), myName.c_str()) == 0){
        std::string oneOfTheEndPoints = endPointsNearMe.front();
        //When end-point is in the current junction
        if (dest.at(3) == oneOfTheEndPoints.at(4)){
            std::string dir(1, myName.at(4));
            //Eligible for free right turn if direction of destination is to right and the signal
            //of the right direction is not green
            if (strcmp(rightDirectionMapping[vh_msg->getTravellingDirection()].c_str(), dir.c_str()) == 0 &&
                    trafficFlag != directionToFlagMapping[dir.c_str()]){
                EV << myName << " - Eligible for free right turn[" << vh_msg->getDstEndpoint() << "]." << endl;
                //If a vehicle is traveling south then it is coming from north. So we store the negation of the direction.
                //This variable is helpful in calculating right turns in other junctions
                vh_msg->setTravellingDirection(negationDirectionMapping[dir.c_str()].c_str());
                double junToJunDelay = par("junctionToJunctionDelay");
                sendDelayed(vh_msg, junToJunDelay, gateMapping[dir.c_str()].c_str());
            } else {
                EV << myName << " - EndJun - " << vh_msg->getDstEndpoint() << " is added to queue in direction - " << dir << endl;
                addToQueue(vh_msg, dir);
            }
        } else {
            std::string dir(1, myName.at(5));
            //Eligible for free right turn if direction of destination is to right and the signal
            //of the right direction is not green
            if (strcmp(rightDirectionMapping[vh_msg->getTravellingDirection()].c_str(), dir.c_str()) == 0 &&
                    trafficFlag != directionToFlagMapping[dir.c_str()]){
                EV << myName << " - Eligible for free right turn[" << vh_msg->getDstEndpoint() << "]." << endl;
                //If a vehicle is traveling south then it is coming from north. So we store the negation of the direction.
                //This variable is helpful in calculating right turns in other junctions
                vh_msg->setTravellingDirection(negationDirectionMapping[dir.c_str()].c_str());
                double junToJunDelay = par("junctionToJunctionDelay");
                sendDelayed(vh_msg, junToJunDelay, gateMapping[dir.c_str()].c_str());
            } else {
                EV << myName << " - EndJun - " << vh_msg->getDstEndpoint() << " is added to queue in direction - " << dir << endl;
                addToQueue(vh_msg, dir);
            }
        }
    } else {
        //When end-point is other than current junction.
        if (strcmp(destJunction.c_str(), junctionsNearMe.front().c_str()) == 0){
            std::string directionToTravel = dirOfJunctionsNearMe.front().c_str();
            //Eligible for free right turn if direction of destination is to right and the signal
            //of the right direction is not green
            if (strcmp(rightDirectionMapping[vh_msg->getTravellingDirection()].c_str(), directionToTravel.c_str()) == 0 &&
                    trafficFlag != directionToFlagMapping[directionToTravel.c_str()]){
                EV << myName << " - Eligible for free right turn[" << vh_msg->getDstEndpoint() << "]." << endl;
                //If a vehicle is traveling south then it is coming from north. So we store the negation of the direction.
                //This variable is helpful in calculating right turns in other junctions
                vh_msg->setTravellingDirection(negationDirectionMapping[directionToTravel.c_str()].c_str());
                double junToJunDelay = par("junctionToJunctionDelay");
                sendDelayed(vh_msg, junToJunDelay, gateMapping[directionToTravel].c_str());
            } else {
                EV << myName << " - EndJun - " << vh_msg->getDstEndpoint() << " is added to queue in direction - " << directionToTravel << endl;
                addToQueue(vh_msg, dirOfJunctionsNearMe.front().c_str());
            }
        } else if (strcmp(destJunction.c_str(), junctionsNearMe.back().c_str()) == 0){
            std::string directionToTravel = dirOfJunctionsNearMe.back().c_str();
            //Eligible for free right turn if direction of destination is to right and the signal
            //of the right direction is not green
            if (strcmp(rightDirectionMapping[vh_msg->getTravellingDirection()].c_str(), directionToTravel.c_str()) == 0 &&
                    trafficFlag != directionToFlagMapping[directionToTravel.c_str()]){
                EV << myName << " - Eligible for free right turn[" << vh_msg->getDstEndpoint() << "]." << endl;
                //If a vehicle is traveling south then it is coming from north. So we store the negation of the direction.
                //This variable is helpful in calculating right turns in other junctions
                vh_msg->setTravellingDirection(negationDirectionMapping[directionToTravel.c_str()].c_str());
                double junToJunDelay = par("junctionToJunctionDelay");
                sendDelayed(vh_msg, junToJunDelay, gateMapping[directionToTravel].c_str());
            } else {
                EV << myName << " - EndJun - " << vh_msg->getDstEndpoint() << " is added to queue in direction - " << directionToTravel << endl;
                addToQueue(vh_msg, dirOfJunctionsNearMe.back().c_str());
            }
        } else {
            //When vehicle's destination is in the diagonal junction, then it is
            //added to the queue in which the total vehicles in the path is less.
            EV << myName << " - junction destination is diagonal." << endl;
//            addToQueue(vh_msg, dirOfJunctionsNearMe.back().c_str());
            dirForDiagonalDest();
            if (dirOneLength > 0 && dirOneLength < dirTwoLength){
                EV << myName << " - EndJun - " << vh_msg->getDstEndpoint() << " is added to queue in direction - " << dirOfJunctionsNearMe.front().c_str() << endl;
                addToQueue(vh_msg, dirOfJunctionsNearMe.front().c_str());
            } else {
                EV << myName << " - EndJun - " << vh_msg->getDstEndpoint() << " is added to queue in direction - " << dirOfJunctionsNearMe.back().c_str() << endl;
                addToQueue(vh_msg, dirOfJunctionsNearMe.back().c_str());
            }
        }
    }
}

void Junction::dirForDiagonalDest(){
    if (strcmp(myName.c_str(), "JuncNW") == 0){
        dirOneLength += eastQueue->getLength();
        dirTwoLength += southQueue->getLength();
    } else if (strcmp(myName.c_str(), "JuncNE") == 0){
        dirOneLength += westQueue->getLength();
        dirTwoLength += southQueue->getLength();
    } else if (strcmp(myName.c_str(), "JuncSE") == 0){
        dirOneLength += westQueue->getLength();
        dirTwoLength += northQueue->getLength();
    } else if (strcmp(myName.c_str(), "JuncSW") == 0){
        dirOneLength += eastQueue->getLength();
        dirTwoLength += northQueue->getLength();
    }
}

void Junction::handleJunctionMsg(JunctionMsg *jn_msg){
    /*
     * When junction message is received, store the respective queue lengths in local variables.*/
    if (strcmp(jn_msg->getName(), junctionsNearMe.front().c_str()) == 0){
        dirOneLength = jn_msg->getQueueLength();
    } else if (strcmp(jn_msg->getName(), junctionsNearMe.back().c_str()) == 0){
        dirTwoLength = jn_msg->getQueueLength();
    }
    delete jn_msg;
}

void Junction::recordStatistics(std::string dir, VehicleMsg* msg){
    if (strcmp(dir.c_str(), "E") == 0){
        emit(waitTimeAtJunctionEast, simTime().dbl() - msg->getTempJunctionWaitTime().dbl());
    } else if (strcmp(dir.c_str(), "W") == 0){
        emit(waitTimeAtJunctionWest, simTime().dbl() - msg->getTempJunctionWaitTime().dbl());
    } else if (strcmp(dir.c_str(), "N") == 0){
        emit(waitTimeAtJunctionNorth, simTime().dbl() - msg->getTempJunctionWaitTime().dbl());
    } else if (strcmp(dir.c_str(), "S") == 0){
        emit(waitTimeAtJunctionSouth, simTime().dbl() - msg->getTempJunctionWaitTime().dbl());
    }
}

void Junction::recordDirThroughput(std::string dir, double density){
    if (strcmp(dir.c_str(), "E") == 0){
        eastDirThroughput +=  density - eastQueue->getLength();
    }
    else if (strcmp(dir.c_str(), "W") == 0){
        westDirThroughput +=  density - westQueue->getLength();
    }
    else if (strcmp(dir.c_str(), "N") == 0){
        northDirThroughput =  density - northQueue->getLength();
    }
    else if (strcmp(dir.c_str(), "S") == 0){
        southDirThroughput =  density - southQueue->getLength();
    }
}

void Junction::handleSelfMsg(cMessage *msg){
    /* Based in the trafficFlag release the vehicles in the respective queues.
     * Two way traffic is enabled here. If east direction is given green signal so is for west direction.*/
    if (strcmp(msg->getName(), signalStart->getName()) == 0){
        double density = eastQueue->getLength() + westQueue->getLength() + northQueue->getLength() + southQueue->getLength();
        emit(densityAtJunction, density);
        scheduleAt(simTime() + phaseTiming, signalEnd);
        greenFlag = true;
        if (trafficFlag == 0){
            EV << myName << " - enabling east queue." << endl;
            eastSignalCount++;
            forwardTraffic("E");
        } else if (trafficFlag == 1){
            EV << myName << " - enabling west queue." << endl;
            westSignalCount++;
            forwardTraffic("W");
        } else if (trafficFlag == 2){
            EV << myName << " - enabling north queue." << endl;
            northSignalCount++;
            forwardTraffic("N");
        } else if (trafficFlag == 3){
            EV << myName << " - enabling south queue." << endl;
            southSignalCount++;
            forwardTraffic("S");
        }

    } else if (strcmp(msg->getName(), signalEnd->getName()) == 0){
        greenFlag = false;
        sendJunctionMsg();
        if (trafficFlag == 0){
            trafficFlag = 1;
        } else if (trafficFlag == 1){
            trafficFlag = 2;
        } else if (trafficFlag == 2){
            trafficFlag = 3;
        } else if (trafficFlag == 3){
            trafficFlag = 0;
        }

        //For dynamic signaling, uncomment the following code.
//        int previousTrafficFlag = trafficFlag;
//        trafficFlag = nextTrafficGreenPhase();
//        if (trafficFlag == previousTrafficFlag)
//            scheduleAt(simTime() + 0.2, signalStart);
//        else
            scheduleAt(simTime() + par("phaseChangeDelay"), signalStart);
    }
}

void Junction::forwardTraffic(std::string dir){
    /* Empty the respective queue based on the received dir.
     * Ex. if dir is "E", then send all vehicles in eastQueue via east gate.*/

    if (strcmp(dir.c_str(), "E") == 0){
        EV << myName << " - emptying east queue." << endl;
        while (greenFlag && eastQueue->getLength() > 0){
            VehicleMsg *tempMsg = dynamic_cast<VehicleMsg*>(eastQueue->pop());
            tempMsg->setWaitTime(tempMsg->getWaitTime() + (simTime().dbl() - tempMsg->getTempJunctionWaitTime().dbl()));
            tempMsg->setTempStartTime(simTime());
            emit(waitTimeAtJunction, simTime().dbl() - tempMsg->getTempJunctionWaitTime().dbl());
            recordStatistics("E", tempMsg);
            double delay = par("accelerationDelay");
            double junToJunDelay = par("junctionToJunctionDelay");
            delay += junToJunDelay;
            sendDelayed(tempMsg, delay, gateMapping[dir.c_str()].c_str());
            eastDirThroughput++;
            emit(eastQueueSignal, eastQueue->getLength());
        }
    }

    else if (strcmp(dir.c_str(), "W") == 0){
        EV << myName << " - emptying west queue." << endl;
        while (greenFlag && westQueue->getLength() > 0){
            VehicleMsg *tempMsg = dynamic_cast<VehicleMsg*>(westQueue->pop());
            tempMsg->setWaitTime(tempMsg->getWaitTime() + (simTime().dbl() - tempMsg->getTempJunctionWaitTime().dbl()));
            tempMsg->setTempStartTime(simTime());
            emit(waitTimeAtJunction, simTime().dbl() - tempMsg->getTempJunctionWaitTime().dbl());
            recordStatistics("W", tempMsg);
            double delay = par("accelerationDelay");
            double junToJunDelay = par("junctionToJunctionDelay");
            delay += junToJunDelay;
            sendDelayed(tempMsg, delay, gateMapping[dir.c_str()].c_str());
            westDirThroughput++;
            emit(westQueueSignal, westQueue->getLength());
        }
    }

    else if (strcmp(dir.c_str(), "N") == 0){
        EV << myName << " - emptying north queue." << endl;
        while (greenFlag && northQueue->getLength() > 0){
            VehicleMsg *tempMsg = dynamic_cast<VehicleMsg*>(northQueue->pop());
            tempMsg->setWaitTime(tempMsg->getWaitTime() + (simTime().dbl() - tempMsg->getTempJunctionWaitTime().dbl()));
            tempMsg->setTempStartTime(simTime());
            emit(waitTimeAtJunction, simTime().dbl() - tempMsg->getTempJunctionWaitTime().dbl());
            recordStatistics("N", tempMsg);
            double delay = par("accelerationDelay");
            double junToJunDelay = par("junctionToJunctionDelay");
            delay += junToJunDelay;
            sendDelayed(tempMsg, delay, gateMapping[dir.c_str()].c_str());
            northDirThroughput++;
            emit(northQueueSignal, northQueue->getLength());
        }
    }

    else if (strcmp(dir.c_str(), "S") == 0){
        EV << myName << " - emptying south queue." << endl;
        while (greenFlag && southQueue->getLength() > 0){
            VehicleMsg *tempMsg = dynamic_cast<VehicleMsg*>(southQueue->pop());
            tempMsg->setWaitTime(tempMsg->getWaitTime() + (simTime().dbl() - tempMsg->getTempJunctionWaitTime().dbl()));
            tempMsg->setTempStartTime(simTime());
            emit(waitTimeAtJunction, simTime().dbl() - tempMsg->getTempJunctionWaitTime().dbl());
            recordStatistics("S", tempMsg);
            double delay = par("accelerationDelay");
            double junToJunDelay = par("junctionToJunctionDelay");
            delay += junToJunDelay;
            sendDelayed(tempMsg, delay, gateMapping[dir.c_str()].c_str());
            southDirThroughput++;
            emit(southQueueSignal, southQueue->getLength());
        }
    }
}

int Junction::nextTrafficGreenPhase(){
    /* Give green signal based on the size of the queues.
     * Next green is given to the queue having the maximum vehicles.*/

    std::string temp = "";
    if (eastQueue->getLength() > westQueue->getLength())
        temp = "E";
    else
        temp = "W";

    if (temp == "E"){
        if (northQueue->getLength() > eastQueue->getLength())
            temp = "N";
        if (temp == "E" && southQueue->getLength() > eastQueue->getLength())
            temp = "S";
        if (temp == "N" && southQueue->getLength() > northQueue->getLength())
            temp = "S";
    } else {
        if (northQueue->getLength() > westQueue->getLength())
            temp = "N";
        if (temp == "W" && southQueue->getLength() > westQueue->getLength())
            temp = "S";
        if (temp == "N" && southQueue->getLength() > northQueue->getLength())
            temp = "S";
    }

    EV << "EastQueue - " << eastQueue->getLength() << ", WestQueue - " << westQueue->getLength() <<
            ", NorthQueue - " << northQueue->getLength() << ", SouthQueue - " << southQueue->getLength() << endl;
    return directionToFlagMapping[temp];
}

void Junction::handleMessage(cMessage *msg)
{
    if (VehicleMsg* vh_msg = dynamic_cast<VehicleMsg*>(msg)) {
        handleVehicleMsg(vh_msg);
    } else if (JunctionMsg* jn_msg = dynamic_cast<JunctionMsg*>(msg)){
        handleJunctionMsg(jn_msg);
    } else if (strcmp(msg->getName(), "Throughput") == 0){
        emit(eastThroughputAtJunction, eastDirThroughput);
        emit(westThroughputAtJunction, westDirThroughput);
        emit(northThroughputAtJunction, northDirThroughput);
        emit(southThroughputAtJunction, southDirThroughput);
        junctionThroughPut = eastDirThroughput + westDirThroughput + northDirThroughput + southDirThroughput;
        emit(throughputAtJunction, junctionThroughPut);
        eastDirThroughput = 0;
        westDirThroughput = 0;
        northDirThroughput = 0;
        southDirThroughput = 0;
        junctionThroughPut = 0;
        throughputMsg = new cMessage("Throughput");
        scheduleAt(simTime() + 5, throughputMsg);
    } else {
        handleSelfMsg(msg);
    }
}

void Junction::refreshDisplay() const{
    char buf[40];
    sprintf(buf, "GreenTowards: %s",
            intToDirectionMapping[trafficFlag].c_str());
    getDisplayString().setTagArg("t", 0, buf);
}

void Junction::finish(){
    EV<<"Signal count for "<<myName<<" "<<eastSignalCount<<"\n";
    EV<<"Signal count for "<<myName<<" "<<westSignalCount<<"\n";
    EV<<"Signal count for "<<myName<<" "<<northSignalCount<<"\n";
    EV<<"Signal count for "<<myName<<" "<<southSignalCount<<"\n";
    recordScalar("EastSignalCount", eastSignalCount);
    recordScalar("WestSignalCount", westSignalCount);
    recordScalar("NorthSignalCount", northSignalCount);
    recordScalar("SouthSignalCount", southSignalCount);
}
