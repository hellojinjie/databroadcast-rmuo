/*
 * Configure.cpp
 *
 *  Created on: 2011-2-1
 */

#include "Configure.h"
#include <json/json.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string Configure::configFilename("simulation.conf.js");

Configure::Configure()
{
    this->readConfigFile();
}

list<ConfigureItem>& Configure::getInstance(string algorithmName)
{
    static Configure instance;
    return instance.configure[algorithmName];
}

Configure::~Configure()
{

}

/**
 * 在这个函数中不检查配置文件的正确性，所以在修改配置文件的时候，
 * 要自己保证修改是正确的，不然会发生什么错误就不知道了
 */
void Configure::readConfigFile()
{
    fstream configStream;
    configStream.open(configFilename.c_str(), fstream::in);

    Json::Value root;
    Json::Reader reader;
    reader.parse(configStream, root);

    ConfigureItem item;

    item.clientNumber = root["common"]["clientNumber"].asInt();
    item.totalSlot = root["common"]["totalSlot"].asInt();
    item.dbsize = root["common"]["dbsize"].asInt();
    item.queryPeriodMax = root["common"]["queryPeriodMax"].asInt();
    item.queryPeriodMin = root["common"]["queryPeriodMin"].asInt();
    item.theta = root["common"]["theta"].asDouble();

    item.resultOutputFilename = root["requestDeadlineMissRatio"]["resultOutputFilename"].asString();
    for (unsigned int i = 0; i < root["requestDeadlineMissRatio"]["config"].size(); i++)
    {
        item.queryItemNumberMin = root["requestDeadlineMissRatio"]["config"][i]["queryItemNumberMin"].asInt();
        item.queryItemNumberMax = root["requestDeadlineMissRatio"]["config"][i]["queryItemNumberMax"].asInt();

        configure["requestDeadlineMissRatio"].push_back(item);
    }

    configStream.close();
}
