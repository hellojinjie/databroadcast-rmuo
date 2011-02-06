/*
 * Configure.h
 *
 *  Created on: 2011-2-1
 */

#ifndef CONFIGURE_H_
#define CONFIGURE_H_

#include <string>

using namespace std;

class Configure
{
public:
    virtual ~Configure();
    Configure(char *filename);

private:
    void readConfigFiel();

    /* properties */
public:
    /* 数据库大小 */
    int dbsize;
    /* 客户端数量 */
    int clientNumber;
    /* 每次请求访问的数据项的个数的最小值 */
    int queryItemNumberMin;
    /* 每次请求访问的数据项的个数的最大值 */
    int queryItemNumberMax;
    /* 每次请求的周期的最小值 */
    int queryPeriodMin;
    /* 每次请求的周期的最大值 */
    int queryPeriodMax;
    /* zipf 分布函数的参数 */
    int theta;

private:
    char *configFilename;
};

#endif /* CONFIGURE_H_ */
