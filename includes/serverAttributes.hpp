#ifndef SERVER_ATTRIBUTES_H
#define SERVER_ATTRIBUTES_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <cstring>
#include <map>
#include <utility>
#include "unistd.h"

#define TRUE 1
#define FALSE 0

class serverConf
{
    public:
        std::vector< std::string > location_ids;
        std::vector< std::string > general;
        std::vector< std::string > server_ids;
        std::vector< std::map< std::string, std::map< std::string, std::vector< std::string > > > > http; // < server, location >
        serverConf() {}
        serverConf(serverConf const & rhs) : location_ids(std::vector< std::string >(rhs.location_ids)), general(std::vector< std::string >(rhs.general)), http(std::vector< std::map< std::string, std::map< std::string, std::vector< std::string > > > >(rhs.http)) {}
        serverConf operator=(serverConf & rhs) { location_ids = std::vector< std::string >(rhs.location_ids); general = std::vector< std::string >(rhs.general); http = std::vector< std::map< std::string, std::map< std::string, std::vector< std::string > > > >(rhs.http); return *this; }
        virtual ~serverConf() {}
        int parseContent(std::string content);
        void pushServerIds(std::map< std::string, std::vector< std::string > > server);
        void pushLocationIds(std::map< std::string, std::vector< std::string > > location);
        int isValid(std::string content);
        int setIds(std::string content);
        int setValues(std::string content);
        std::string getBlock(std::string content);
        std::string getBlockServer(std::string content);
        std::string getBlockLocation(std::string content);
        int isValidLocation(std::string content);
        int isValidServer(std::string content);
        void printMap();
};

#endif