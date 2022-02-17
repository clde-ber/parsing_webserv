#ifndef SERVER_ATTRIBUTES_H
#define SERVER_ATTRIBUTES_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

struct locations
{
    std::string index;
    std::vector< std::string > methods;
    bool autoIndex;
    std::string upload_dir;
    std::string cgi;
    std::string redirect;
};

class serverConf : public locations
{
    public:
        int listen;
        std::string server_name;
        int clientMaxBodySize;
        std::string errorPages;
        std::vector< locations > location;
        serverConf();
        virtual ~serverConf();
};

#endif