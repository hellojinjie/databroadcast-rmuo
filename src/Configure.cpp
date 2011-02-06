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

Configure::Configure(char *filename)
{
    configFilename = filename;
    this->readConfigFiel();
}

Configure::~Configure()
{

}

/**
 * 在这个函数中不检查配置文件的正确性，所以在修改配置文件的时候，
 * 要自己保证修改是正确的，不然会发生什么错误就不知道了
 */
void Configure::readConfigFiel()
{
    fstream configStream;
    configStream.open(configFilename, fstream::in);

    Json::Value root;
    Json::Reader reader;
    reader.parse(configStream, root);

    this->clientNumber = root["common"]["clientNumber"].asInt();
    this->dbsize = root["common"]["dbsize"].asInt();
    this->queryItemNumberMax = root["common"]["queryItemNumberMax"].asInt();
    this->queryItemNumberMin = root["common"]["queryItemNumberMin"].asInt();
    this->queryPeriodMax = root["common"]["queryPeriodMax"].asInt();
    this->queryPeriodMin = root["common"]["queryPeriodMin"].asInt();
    this->theta = root["common"]["theta"].asInt();
}
