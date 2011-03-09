/*
 * SimpleRequest.h
 *
 *  Created on: 2011-1-13
 */

#ifndef SIMPLEREQUEST_H_
#define SIMPLEREQUEST_H_

#include <list>

using namespace std;

/**
 * 该数据结构用来表示一个客户端，
 * 同时他也用来表示一个客户端发出的请求，
 */
typedef struct TagSimpleRequest
{
    /** 请求编号，用来唯一的标识当前的请求 */
    int id;

    /** 请求的数据项集合*/
    list<int> readSet;

    /** 请求的周期 */
    int period;

    /**
     * 客户端最后一次请求的时刻， -1 表示还没请求过
     * 这个字段是针对一个客户端的
     */
    int lastRequestTime;

    /**
     * 该请求到达服务器的时刻， 这个字段是针对一个客户端发出的请求的
     * lastRequestTime = arrivalTime + period * n , n >= 0
     */
    int arrivalTime;

    /** 已经收到的数据项 */
    list<int> receivedItem;

    bool operator==(TagSimpleRequest a)
    {
        return a.id == id;
    }

    bool operator<(TagSimpleRequest a)
    {
        return period < a.period;
    }

} SimpleRequest;

#endif /* SIMPLEREQUEST_H_ */
