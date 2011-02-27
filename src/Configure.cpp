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

    string configString;
    char oneLine[1024];
    /* 第一行的不要,我要把开始几个字节的 BOM 给忽略掉 */
    configStream.getline(oneLine, 1024);
    while (!configStream.eof())
    {
        configStream.getline(oneLine, 1024);
        configString += oneLine;
    }

    configStream.close();

    Json::Value root;
    Json::Reader reader;
    reader.parse(configString, root);

    ConfigureItem item;

    item.seed = root["common"]["seed"].asDouble();
    item.clientNumber = root["common"]["clientNumber"].asInt();
    item.totalSlot = root["common"]["totalSlot"].asInt();
    item.dbsize = root["common"]["dbsize"].asInt();
    item.queryPeriodMax = root["common"]["queryPeriodMax"].asInt();
    item.queryPeriodMin = root["common"]["queryPeriodMin"].asInt();
    item.theta = root["common"]["theta"].asDouble();

    item.resultOutputFilename = root["requestDeadlineMissRatio"]["resultOutputFilename"].asString();
    item.enable = root["requestDeadlineMissRatio"]["enable"].asBool();
    for (unsigned int i = 0; i < root["requestDeadlineMissRatio"]["config"].size(); i++)
    {
        item.queryItemNumberMin = root["requestDeadlineMissRatio"]["config"][i]["queryItemNumberMin"].asInt();
        item.queryItemNumberMax = root["requestDeadlineMissRatio"]["config"][i]["queryItemNumberMax"].asInt();

        configure["requestDeadlineMissRatio"].push_back(item);
    }

    item.queryItemNumberMin = root["common"]["queryItemNumberMin"].asInt();
    item.queryItemNumberMax = root["common"]["queryItemNumberMax"].asInt();
    item.resultOutputFilename = root["utilizationAndDeadlineMissRatio"]["resultOutputFilename"].asString();
    item.enable = root["utilizationAndDeadlineMissRatio"]["enable"].asBool();
    item.clientNumberMin = root["utilizationAndDeadlineMissRatio"]["config"]["clientNumberMin"].asInt();
    item.clientNumberMax = root["utilizationAndDeadlineMissRatio"]["config"]["clientNumberMax"].asInt();
    item.clientNumberIncreaseStep = root["utilizationAndDeadlineMissRatio"]["config"]["clientNumberIncreaseStep"].asInt();
    configure["utilizationAndDeadlineMissRatio"].push_back(item);
}
