/*
 * MobileClient.h
 *
 *  Created on: 2011-1-10
 */

#ifndef MOBILECLIENT_H_
#define MOBILECLIENT_H_

#include "util.h"
#include "Server.h"
#include "SimpleRequest.h"
#include "Configure.h"
#include <list>

/* 名称空间不可忘了啊 */
using namespace std;

/* forward declare */
class Server;

/**
 * MobileClient 管理的是对应于某个算法的一组Clients
 */
class MobileClient
{
public:

    /** 初始化一个包含 count 个 Client 的MobileClient */
    MobileClient(Server *server, int count, ConfigureItem configure);
    virtual ~MobileClient();

    /**
     * 生成一组请求，请求的数量是初始化该实例时已经指定的值
     * 返回生成的请求的数量
     */
    virtual int generateRequests(list<SimpleRequest> &requests);

    virtual void setServer(Server *server);

    int generateItem();

protected:

    list<SimpleRequest> clients;

private:

    void generateClients();
    int generateId();

    ConfigureItem configure;

    int clientCount;

    /** 对服务端的引用  */
    Server *server;

    /** 客户端编号 */
    int initId;

    zipf_t z;
};

#endif /* MOBILECLIENT_H_ */
