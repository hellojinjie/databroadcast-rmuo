/*
 * RMUOClient.cpp
 *
 *  Created on: 2011-1-10
 */

#include "RMUOClient.h"

RMUOClient::RMUOClient(Server *server, int clientCount)
        : MobileClient(server, clientCount)
{

}

RMUOClient::~RMUOClient()
{
}

int RMUOClient::generateRequests(list<SimpleRequest> &requests)
{
    int count = 0;
    list<SimpleRequest>::iterator iter;
    for (iter = clients.begin(); iter != clients.end(); iter++)
    {
        if (iter->lastRequestTime == -1)
        {
            iter->lastRequestTime = 0;
            iter->arrivalTime = 0;
            count++;
            requests.push_back(*iter);
        }
    }
    if (count > 0)
    {
        cout << "共生成请求个数：" << count << endl;
    }
    return count;
}
