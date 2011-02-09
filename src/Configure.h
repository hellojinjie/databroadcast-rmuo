/*
 * Configure.h
 *
 *  Created on: 2011-2-1
 */

#ifndef CONFIGURE_H_
#define CONFIGURE_H_

#include <string>
#include <map>
#include <list>

using namespace std;

typedef struct TagConfigureItem
{
    /* 数据库大小 */
    int dbsize;
    /* 总的时槽数 */
    int totalSlot;
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
    double theta;
    /* 实验数据的结果输出到那个文件 */
    string resultOutputFilename;

    bool operator< (const TagConfigureItem& a) const
    {
        return queryPeriodMin < a.queryItemNumberMin;
    }

} ConfigureItem;

class Configure
{
public:
    virtual ~Configure();
    static list<ConfigureItem>& getInstance(string algorithmName);
    Configure();

private:
    void readConfigFile();

public:
    static string configFilename;

private:
    map<string, list<ConfigureItem> > configure;
};

#endif /* CONFIGURE_H_ */
