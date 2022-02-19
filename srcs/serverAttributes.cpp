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
    server.insert(std::pair< std::string, std::vector< std::string > >("listen", std::vector< std::string >()));
    server.insert(std::pair< std::string, std::vector< std::string > >("server_name", std::vector< std::string >()));
    server.insert(std::pair< std::string, std::vector< std::string > >("client_max_body_size", std::vector< std::string >()));
    server.insert(std::pair< std::string, std::vector< std::string > >("error_pages", std::vector< std::string >()));
    server.insert(std::pair< std::string, std::vector< std::string > >("root", std::vector< std::string >()));
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
    std::string key = "";
    std::string category = "";
    while (pos != content.length())
    {
        if (setValues(content) == FALSE)
        {
            std::cout << "error1" << std::endl;
            return FALSE;
        }
        std::cout << "server ids size()" << server_ids.size() << std::endl;
        while (i < server_ids.size())
        {
            if (content.find(server_ids[i], pos) != std::string::npos)
            {
                pos = content.find(server_ids[i], pos) + 1;
                std::cout << "server_ids[i]" << server_ids[i] << std::endl;
                category = "server";
                key = server_ids[i];
                break ;
            }
            i++;
        }
        i = 0;
        std::cout << "location ids size()" << location_ids.size() << std::endl;
        while (category == "" && i < location_ids.size())
        {
            if (content.find(location_ids[i], pos) != std::string::npos)
            {
                pos = content.find(location_ids[i], pos) + 1;
                std::cout << "location_ids[i]" << location_ids[i] << std::endl;
                category = "location";
                key = location_ids[i];
                break ;
            }
            i++;
        }
        std::cout << "KEY " << key << std::endl;
        std::cout << "CATEGORY" << category << std::endl;
        if (key != "location" && content.find(";", pos) != std::string::npos)
            idx = content.find(";", pos);
        else if (content.find("{", pos) != std::string::npos)
            idx = content.find("{", pos);
        else
            return FALSE;
        if ((key != "location" && idx == std::string::npos) || (key == "location" && idx == std::string::npos))
        {
            std::cout << "error2" << std::endl;
            write(1, "error2", strlen("error2"));
            return FALSE;
        }
        std::cout << "len!" << idx << std::endl;
        std::cout << "pos!" << pos << std::endl;
        std::cout << "pos of content" << pos + key.length() << std::endl;
        std::vector< std::string > value;
        std::string raw_content = content.substr(pos + key.length(), idx - (pos + key.length()));
        std::cout << "raw_content" << raw_content << std::endl;
        std::string trim_content = raw_content.substr(raw_content.find_first_not_of("\t\n\r\v\f "), raw_content.length() - raw_content.find_first_not_of("\t\n\r\v\f "));
        std::cout << "content = " << trim_content << "&" << std::endl;
        value.push_back(content.substr(pos + key.length(), idx - (pos + key.length())));
        pos = idx + 1;
        std::cout << "pos after" << pos << std::endl;
        std::cout << "http size" << http.size() << std::endl;
        http.data()[http.size() - 1][category].insert(std::make_pair(key, value));
        std::cout << "content at pos" << content.at(pos) << std::endl;
        i = 0;
    }
    return TRUE;
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
    if (isValid(content) == FALSE)
    {
        std::cout << "********content : " << content.substr(posStart, content.length() - posStart) << "********" << std::endl;
        std::cout << "fail2" << std::endl;
        return FALSE;
    }
    return TRUE;
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
    conf.location_ids.push_back("location");
    conf.location_ids.push_back("root");
    conf.location_ids.push_back("index");
    conf.location_ids.push_back("methods");
    conf.location_ids.push_back("autoindex");
    conf.location_ids.push_back("upload_dir");
    conf.location_ids.push_back("cgi");
    conf.location_ids.push_back("redirect");
    conf.location_ids.push_back("return");
    std::string file = "conf_example_copy.conf";
    std::string output = "";
    output += getContent(file);
    std::cout << output << std::endl;
    std::cout << "is valid {} : " << conf.parseContent(output) << std::endl;
    return TRUE;
}