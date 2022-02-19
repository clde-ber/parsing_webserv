#include "serverAttributes.hpp"

std::string getContent(std::string file)
{
    std::string ret = "";
    std::ifstream is (file.c_str(), std::ifstream::binary);
    if (is) {
    // get length of file:
    is.seekg(0, is.end);
    int length = is.tellg();
    is.seekg(0, is.beg);

    char *buffer = new char[length];

    std::cout << "Reading " << length << " characters... ";
    // read data as a block:
    is.read(buffer,length);

    if (is)
      std::cout << "all characters read successfully.";
    else
      std::cout << "error: only " << is.gcount() << " could be read";
    is.close();

    // ...buffer contains the entire file...

    ret = std::string(buffer, length);
    delete [] buffer;
  }
  return ret;
}

void serverConf::pushServerIds(std::map< std::string, std::vector< std::string > > server)
{
    server.insert(std::pair< std::string, std::vector< std::string > >("location", std::vector< std::string >()));
    server.insert(std::pair< std::string, std::vector< std::string > >("listen", std::vector< std::string >()));
    server.insert(std::pair< std::string, std::vector< std::string > >("server_name", std::vector< std::string >()));
    server.insert(std::pair< std::string, std::vector< std::string > >("client_max_body_size", std::vector< std::string >()));
    server.insert(std::pair< std::string, std::vector< std::string > >("error_pages", std::vector< std::string >()));
    server.insert(std::pair< std::string, std::vector< std::string > >("root", std::vector< std::string >()));
    server.insert(std::pair< std::string, std::vector< std::string > >("index", std::vector< std::string >()));
    server.insert(std::pair< std::string, std::vector< std::string > >("return", std::vector< std::string >()));
}

void serverConf::pushLocationIds(std::map< std::string, std::vector< std::string > > location)
{
    location.insert(std::pair< std::string, std::vector< std::string > >("root", std::vector< std::string >()));
    location.insert(std::pair< std::string, std::vector< std::string > >("index", std::vector< std::string >()));
    location.insert(std::pair< std::string, std::vector< std::string > >("methods", std::vector< std::string >()));
    location.insert(std::pair< std::string, std::vector< std::string > >("autoindex", std::vector< std::string >()));
    location.insert(std::pair< std::string, std::vector< std::string > >("upload_dir", std::vector< std::string >()));
    location.insert(std::pair< std::string, std::vector< std::string > >("cgi", std::vector< std::string >()));
    location.insert(std::pair< std::string, std::vector< std::string > >("redirect", std::vector< std::string >()));
    location.insert(std::pair< std::string, std::vector< std::string > >("return", std::vector< std::string >()));
    location.insert(std::pair< std::string, std::vector< std::string > >("try_files", std::vector< std::string >()));
    location.insert(std::pair< std::string, std::vector< std::string > >("proxy_set_headers", std::vector< std::string >()));
    location.insert(std::pair< std::string, std::vector< std::string > >("proxy_buffers", std::vector< std::string >()));
    location.insert(std::pair< std::string, std::vector< std::string > >("proxy_buffer_size", std::vector< std::string >()));
    location.insert(std::pair< std::string, std::vector< std::string > >("proxy_pass", std::vector< std::string >()));
}

int serverConf::isValidLocation(std::string content)
{
    size_t i = 0;
    size_t pos = 0;
    size_t idx = 0;
    size_t j = 0;
    std::string key = "";
    std::string category = "";

    while (pos != content.length())
    {
        i = 0;
        while (i < location_ids.size())
        {
            if (content.find(location_ids[i], pos) != std::string::npos && isspace(content.at(content.find(location_ids[i], pos) + location_ids[i].length())))
            {
                if (http.data()[http.size() - 1]["location"][location_ids[i]].empty())
                {
                    pos = content.find(location_ids[i], pos);
                    category = "location";
                    key = location_ids[i];
                    break ;
                }
            }
            i++;
        }
        while (pos + j < content.length() && isspace(content.at(pos + j)))
            j++;
        if (pos + j == content.length())
            return TRUE;
        else if (content.find(";", pos) != std::string::npos)
            idx = content.find(";", pos);
        else
            return FALSE;
        std::vector< std::string > value;
        std::string raw_content = content.substr(pos + key.length(), idx - (pos + key.length()));
        std::string trim_content = raw_content.substr(raw_content.find_first_not_of("\t\n\r\v\f "), raw_content.length() - raw_content.find_first_not_of("\t\n\r\v\f "));
        value.push_back(trim_content);
        pos = idx + 1;
        http.data()[http.size() - 1][category][key].push_back(trim_content);
        i = 0;
    }
    return TRUE;
}

int serverConf::isValidServer(std::string content)
{
    size_t i = 0;
    size_t pos = 0;
    size_t idx = 0;
    size_t j = 0;
    std::string key = "";
    std::string category = "";
    std::string blockLocation = "";
    bool is_location = 0;

    while (pos != content.length())
    {
        i = 0;
        while (i < server_ids.size())
        {
            is_location = 0;
            if (content.find(server_ids[i], pos) != std::string::npos && isspace(content.at(content.find(server_ids[i], pos) + server_ids[i].length())))
            {
                if (server_ids[i] == "location")
                {
                    if (content.find("{", pos) != std::string::npos)
                        blockLocation = getBlockLocation(&content[content.find("{", pos) + 1]);
                    if (isValidLocation(blockLocation) == FALSE)
                        return FALSE;
                    pos = content.find("}", pos) + 1;
                    is_location = 1;
                    break ;
                }
                else if (http.data()[http.size() - 1]["server"][server_ids[i]].empty())
                {
                    pos = content.find(server_ids[i], pos);
                    category = "server";
                    key = server_ids[i];
                    break ;
                }
            }
            i++;
        }
        if (is_location == 0)
        {
        while (pos + j < content.length() && isspace(content.at(pos + j)))
            j++;
        if (pos + j == content.length())
            return TRUE;
        else if (content.find(";", pos) != std::string::npos)
            idx = content.find(";", pos);
        else
            return FALSE;
        std::vector< std::string > value;
        std::string raw_content = content.substr(pos + key.length(), idx - (pos + key.length()));
        std::string trim_content = raw_content.substr(raw_content.find_first_not_of("\t\n\r\v\f "), raw_content.length() - raw_content.find_first_not_of("\t\n\r\v\f "));
        value.push_back(trim_content);
        pos = idx + 1;
        http.data()[http.size() - 1][category][key].push_back(trim_content);
        i = 0;
        }
    }
    return TRUE;
}

int serverConf::setIds(std::string content)
{
    (void)content;
    //size_t i = 0;
    //size_t pos = 0;
    std::map< std::string, std::vector< std::string > > server;
    std::map< std::string, std::vector< std::string > > location;

    pushServerIds(server);
    pushLocationIds(location);
    //while (std::isspace(static_cast<unsigned char>(content.at(pos))))
    //    pos++;
    //while (i < server_ids.size() && (pos = content.find(server_ids[i], pos)) == std::string::npos)
    //    i++;
    //if (server_ids[i] == "" && pos == std::string::npos)
    //    return FALSE;
    std::map< std::string, std::map< std::string, std::vector< std::string > > > map_ids;
    map_ids.insert(std::make_pair("server", server));
    map_ids.insert(std::make_pair("location", location));
    http.push_back(map_ids);
    return TRUE;
}

int serverConf::setValues(std::string content)
{
    //size_t i = 0;
    //size_t pos = 0;

    setIds(content);
    //while (std::isspace(static_cast<unsigned char>(content.at(pos))))
    //    pos++;
   /* while (i < http.size())
    {
        std::map< std::string, std::map< std::string, std::vector< std::string > > >::iterator it = http[i].begin();
        std::map< std::string, std::map< std::string, std::vector< std::string > > >::iterator ite = http[i].end();
        std::cout << "it->first" << it->first << std::endl;
        while (it != ite && (pos = content.find(it->first, pos)) == std::string::npos)
            it++;
        i++;
    }*/
    //if (general[i] == "" && pos == std::string::npos)
    //    return FALSE;
    return TRUE;
}

int serverConf::isValid(std::string content)
{
    size_t i = 0;
    size_t pos = 0;
    size_t idx = 0;
    size_t j = 0;
    std::string key = "";
    std::string category = "";

    while (pos != content.length())
    {
        category = "";
        while (i < server_ids.size())
        {
            if (content.find(server_ids[i], pos) != std::string::npos && isspace(content.at(content.find(server_ids[i], pos) + server_ids[i].length())))
            {
                if (http.data()[http.size() - 1]["server"][server_ids[i]].empty())
                {
                    pos = content.find(server_ids[i], pos);
                    category = "server";
                    key = server_ids[i];
                    break ;
                }
            }
            i++;
        }
        i = 0;
        while (category == "" && i < location_ids.size())
        {
            if (content.find(location_ids[i], pos) != std::string::npos && isspace(content.at(content.find(location_ids[i], pos) + location_ids[i].length())))
            {
                if (http.data()[http.size() - 1]["location"][location_ids[i]].empty())
                {
                    pos = content.find(location_ids[i], pos);
                    category = "location";
                    key = location_ids[i];
                    break ;
                }
            }
            i++;
        }
        while (pos + j < content.length() && isspace(content.at(pos + j)))
            j++;
        if (pos + j == content.length())
            return TRUE;
        else if (content.find(";", pos) != std::string::npos)
            idx = content.find(";", pos);
        else
            return FALSE;
        std::vector< std::string > value;
        std::string raw_content = content.substr(pos + key.length(), idx - (pos + key.length()));
        std::string trim_content = raw_content.substr(raw_content.find_first_not_of("\t\n\r\v\f "), raw_content.length() - raw_content.find_first_not_of("\t\n\r\v\f "));
        value.push_back(content.substr(pos + key.length(), idx - (pos + key.length())));
        pos = idx + 1;
        http.data()[http.size() - 1][category].insert(std::make_pair(key, value));
        i = 0;
    }
    return TRUE;
}

std::string serverConf::getBlockServer(std::string content)
{
    size_t i = 0;
    size_t count1 = 0;
    size_t count2 = 0;

    while (i < content.length())
    {
        if (content.at(i) == '{')
            count1++;
        if (content.at(i) == '}')
            count2++;
        if (count2 > count1)
            return content.substr(0, i);
        i++;
    }
    return content.substr(0, i);
}

std::string serverConf::getBlockLocation(std::string content)
{
    size_t i = 0;

    while (i < content.length())
    {
        if (content.at(i) == '}')
            return content.substr(0, i);
        i++;
    }
    return content.substr(0, i);
}

int serverConf::parseContent(std::string content)
{
    size_t posStart = 0;
    size_t posEnd = content.length();
    //check wheter there is an even number of {/}
    while (posStart != posEnd)
    {
        if ((posStart = content.find("{", posStart)) == std::string::npos)
        {
            if ((posEnd = content.rfind("}", posEnd)) == std::string::npos)
                break ;
            else
                return FALSE;
        }
        if ((posEnd = content.rfind("}", posEnd)) == std::string::npos)
            return FALSE;
        posStart++;
        posEnd--;
    }
    size_t server_idx = 0;
    std::string blockServer = "";
    std::string blockLocation = "";
    size_t location_idx = 0;
    while (server_idx != content.length())
    {
        if (content.find("server", server_idx) != std::string::npos)
        {
            server_idx = content.find("server", server_idx);
            if (content.find("{", server_idx) != std::string::npos)
                blockServer = getBlockServer(&content[content.find("{", server_idx) + 1]);
            if (setValues(blockServer) == FALSE)
                return FALSE;
            if (isValidServer(blockServer) == FALSE)
                return FALSE;
            //location_idx = server_idx;
            /*while (blockServer.find("location", location_idx) != std::string::npos && isspace(blockServer.at(blockServer.find("location", location_idx) + 9)))
            {
                if (blockServer.find("{", location_idx) != std::string::npos)
                    blockLocation = getBlockLocation(&blockServer[blockServer.find("{", location_idx) + 1]);
                std::cout << "block loc => " << blockLocation << std::endl;
                if (isValidLocation(blockLocation) == FALSE)
                    return FALSE;
                location_idx += blockServer.find("{", location_idx) + blockLocation.length();
            }*/
        }
        server_idx += content.find("{", server_idx) + location_idx;
    }
    return TRUE;
}

void serverConf::printMap()
{
    size_t i = 0;
    size_t j = 0;
    size_t k = 0;
    //http.data()[http.size() - 1][category].insert(std::make_pair(key, value));
    while (i < http.size())
    {
        std::cout << "indice : [" << i << "]" << std::endl;
        std::cout << "*************" << std::endl;
        std::cout << "serveur : " << std::endl;
        while (j < http.data()[i]["server"].size())
        {
            std::cout << "clé : [" << server_ids[j] << "]";
            while (k < http.data()[i]["server"][server_ids[j]].size())
            {
                std::cout << " | valeur [" << k << "] : " << http.data()[i]["server"][server_ids[j]][k] << std::endl;
                k++;
            }
            if (k == 0)
                std::cout << " | valeur [0] : none" << std::endl;
            k = 0;
            j++;
        }
        j = 0;
        std::cout << "*************" << std::endl;
        std::cout << "location : " << std::endl;
        while (j < http.data()[i]["location"].size())
        {
            std::cout << "clé : [" << location_ids[j] << "]";
            while (k < http.data()[i]["location"][location_ids[j]].size())
            {
                std::cout << " | valeur [" << k << "] : " << http.data()[i]["location"][location_ids[j]][k] << std::endl;
                k++;
            }
            if (k == 0)
                std::cout << " | valeur [0] : none" << std::endl;
            k = 0;
            j++;
        }
        i++;
        std::cout << "*************" << std::endl;
        std::cout << std::endl;
    }
}

int main(void)
{
    serverConf conf;
    std::vector< std::string > empty;

    conf.server_ids.push_back("listen");
    conf.server_ids.push_back("server_name");
    conf.server_ids.push_back("client_max_body_size");
    conf.server_ids.push_back("error_pages");
    conf.server_ids.push_back("root");
    conf.server_ids.push_back("index");
    conf.server_ids.push_back("return");
    conf.server_ids.push_back("location");
    conf.location_ids.push_back("root");
    conf.location_ids.push_back("index");
    conf.location_ids.push_back("methods");
    conf.location_ids.push_back("autoindex");
    conf.location_ids.push_back("upload_dir");
    conf.location_ids.push_back("cgi");
    conf.location_ids.push_back("redirect");
    conf.location_ids.push_back("return");
    conf.location_ids.push_back("try_files");
    conf.location_ids.push_back("proxy_set_header");
    conf.location_ids.push_back("proxy_buffers");
    conf.location_ids.push_back("proxy_buffer_size");
    conf.location_ids.push_back("proxy_pass");
    std::string file = "conf_example_copy.conf";
    std::string output = "";
    output += getContent(file);
    std::cout << output << std::endl;
    std::cout << "bool is valid : " << conf.parseContent(output) << std::endl;
    conf.printMap();
    return TRUE;
}