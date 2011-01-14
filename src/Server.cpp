/*
 * Server.cpp
 *
 *  Created on: 2011-1-10
 */

#include "Server.h"
#include <list>
#include <iostream>

using namespace std;

Server::Server()
{
    this->clock = 0;
}

Server::~Server()
{

}

void Server::startSimulation()
{
    while (true)
    {
        list<SimpleRequest> requests;
        this->client->generateRequests(requests);
        this->scheduler->addRequests(requests);
        this->scheduler->doSchedule();

        cout << "server clock: " << this->getClock() << endl;

        if (this->getClock() >= 30)
        {
            break;
        }
    }
}

Scheduler *Server::getScheduler() const
{
    return scheduler;
}

void Server::setScheduler(Scheduler *scheduler)
{
    this->scheduler = scheduler;
}

unsigned int Server::getClock()
{
    return this->clock;
}

unsigned int Server::incrementAndGetClock()
{
    this->clock++;
    return this->clock;
}

unsigned int Server::getAndIncrementClock()
{
    unsigned int tmp = this->clock;
    this->clock++;
    return tmp;
}

void Server::setClient(MobileClient *client)
{
    this->client = client;
}






