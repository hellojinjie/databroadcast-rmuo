/*
 * MobileClient.cpp
 *
 *  Created on: 2011-1-10
 */

#include "MobileClient.h"
#include "util.h"
#include <cstring>
#include <cstdlib>
#include <list>
#include <iostream>
#include "Configure.h"

using namespace std;

MobileClient::MobileClient(Server *server, int count, ConfigureItem configure)
{
    this->initId = 0;
    zipf_init(configure.dbsize, configure.theta, &this->z);
    this->setServer(server);
    this->clientCount = count;
    this->configure = configure;
    this->addClients(this->generateClients(this->clientCount, this->configure));
}

MobileClient::~MobileClient()
{

}

list<SimpleRequest> MobileClient::generateClients(int clientCount, ConfigureItem configure)
{
    list<SimpleRequest> clients;
    for (int i = 0; i < clientCount; i++)
    {
        SimpleRequest client;
        // client.id = this->generateId();

        /* 初始化为 -1 表示该客户端还没有请求过 */
        client.lastRequestTime = -1;

        /* 请求周期服从均匀分布 */
        client.period = uniform(configure.queryPeriodMin, configure.queryPeriodMax);

        /* 请求的数据项个数，服从均匀分布，请求的内容服从 zipf 分布 */
        int readSetCount = uniform(configure.queryItemNumberMin, configure.queryItemNumberMax);
        int j = client.readSet.size();
        while (j < readSetCount)
        {
            int item = this->generateItem();cout << item << "  ";
            /* 请求的数据项不能重复 */
            if (isInList(client.readSet, item))
            {
                continue;
            }
            client.readSet.push_back(item);
            j = client.readSet.size();
        } cout << endl;
        clients.push_back(client);
    }
    return clients;
}

int MobileClient::generateRequests(list<SimpleRequest> &requests)
{
    list<SimpleRequest>::iterator iter;
    for (iter = clients.begin(); iter != clients.end(); iter++)
    {
        if (iter->lastRequestTime == -1)
        {
            /* 如果该客户端从没有发送过请求 */
            iter->lastRequestTime = 0;
            iter->arrivalTime = 0;
            requests.push_back(*iter);
        }
        else
        {
            /*
             * TODO 这个注释该怎么写？
             * count = 0 时说明还没到客户端该发请求的时候
             * count = 1 说明客户端可以发请求了
             * count > 1 说明客户端错过了几次发送请求的时机,这count个请求中有 count -1 个已经miss deadline了
             */
            unsigned int interval = this->server->getClock() - iter->lastRequestTime;
            int count = interval / iter->period;
            for (int i = 0; i < count; i++)
            {
                iter->arrivalTime = iter->lastRequestTime + iter->period * (i + 1);
                requests.push_back(*iter);
            }
            iter->lastRequestTime = iter->lastRequestTime + count * iter->period;
        }
    }

    cout << "共生成请求：" << requests.size() << endl;
    return requests.size();
}

int MobileClient::generateItem()
{
    return zipf(z);
}

int MobileClient::generateId()
{
    initId++;
    return initId;
}

void MobileClient::setServer(Server *server)
{
    this->server = server;
}
