/*
 * SimpleRequest.h
 *
 *  Created on: 2011-1-13
 */

#ifndef SIMPLEREQUEST_H_
#define SIMPLEREQUEST_H_

#include <list>

using namespace std;

typedef struct TagSimpleRequest
{
    /** 请求编号，用来唯一的标识当前的请求 */
    int id;

    /** 请求的数据项集合*/
    list<int> readSet;

    /** 请求的周期 */
    int period;

    /** 最后一次请求的时刻， -1 表示还没请求过 */
    int lastRequestTime;
} SimpleRequest;

#endif /* SIMPLEREQUEST_H_ */
