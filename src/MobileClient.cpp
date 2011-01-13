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

using namespace std;

MobileClient::MobileClient(Server *server, int count)
{
    this->id = 0;
    zipf_init(1000, 1.0, &this->z);
    this->setServer(server);
    this->clientCount = count;
    this->generateClients();
}

MobileClient::~MobileClient()
{

}

void MobileClient::generateClients()
{
    for (int i = 0; i < this->clientCount; i++)
    {
        SimpleRequest client;
        client.id = this->generateId();

        /* 初始化为 -1 表示该客户端还没有请求过 */
        client.lastRequestTime = -1;

        /* 请求周期服从均匀分布 */
        client.period = uniform(1, 10);

        /* 请求的数据项个数，服从均匀分布，请求的内容服从 zipf 分布 */
        int readSetCount = uniform(1, 10);
        int j = client.readSet.size();
        while (j < readSetCount)
        {
            client.readSet.push_back(zipf(z));
            /* 请求的数据项不能重复 */
            client.readSet.unique();
            j = client.readSet.size();
        }

        this->clients.push_back(client);
    }
    cout << "一共生成客户端：" << clientCount << endl;
}

int MobileClient::generateRequests(list<SimpleRequest> requests)
{
    int requestCount = 0;
    list<SimpleRequest>::iterator iter;
    for (iter = clients.begin(); iter != clients.end(); iter++)
    {
        SimpleRequest request = *iter;
        if (request.lastRequestTime == -1)
        {
            /* 如果该客户端从没有发送过请求 */
            requests.push_back(request);
            request.lastRequestTime = 0;
            requestCount++;
        }
        else
        {
            /*
             * TODO 这个注释该怎么写？
             * count = 0 时说明还没到客户端该发请求的时候
             * count = 1 说明客户端可以发请求了
             * count > 1 说明客户端错过了几次发送请求的时机,这count个请求中有 count -1 个已经miss deadline了
             */
            unsigned int interval = this->server->getClock() - request.lastRequestTime;
            int count = interval / request.period;
            for (int i = 0; i < count; i++)
            {
                requests.push_back(request);
                requestCount++;
            }
            request.lastRequestTime = request.lastRequestTime + count * request.period;
        }
    }

    return requestCount;
}

int MobileClient::generateId()
{
    id++;
    return id;
}

void MobileClient::setServer(Server *server)
{
    this->server = server;
}
