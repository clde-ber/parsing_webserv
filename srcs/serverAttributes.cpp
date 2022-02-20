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
    server.insert(std::pair< std::string, std::vector< std::string > >("error_page", std::vector< std::string >()));
    server.insert(std::pair< std::string, std::vector< std::string > >("root", std::vector< std::string >()));
    server.insert(std::pair< std::string, std::vector< std::string > >("index", std::vector< std::string >()));
    server.insert(std::pair< std::string, std::vector< std::string > >("return", std::vector< std::string >()));
    server.insert(std::pair< std::string, std::vector< std::string > >("cgi", std::vector< std::string >()));
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
    location.insert(std::pair< std::string, std::vector< std::string > >("method", std::vector< std::string >()));
    location.insert(std::pair< std::string, std::vector< std::string > >("default", std::vector< std::string >()));
    location.insert(std::pair< std::string, std::vector< std::string > >("upload", std::vector< std::string >()));
    location.insert(std::pair< std::string, std::vector< std::string > >("dirList", std::vector< std::string >()));
}

int serverConf::setLocationId(std::string name)
{
    //size_t i = 0;
    //size_t pos = 0;
    std::map< std::string, std::vector< std::string > > location;

    pushLocationIds(location);
    //while (std::isspace(static_cast<unsigned char>(content.at(pos))))
    //    pos++;
    //while (i < server_ids.size() && (pos = content.find(server_ids[i], pos)) == std::string::npos)
    //    i++;
    //if (server_ids[i] == "" && pos == std::string::npos)
    //    return FALSE;
    //std::map< std::string, std::map< std::string, std::vector< std::string > > > map_ids;
    http.data()[http.size() - 1].insert(std::make_pair("location " + name, location));
    //http.push_back(map_ids);
    return TRUE;
}

int serverConf::isValidLocation(std::string content, std::string location_name)
{
    size_t i = 0;
    size_t pos = 0;
    size_t idx = 0;
    size_t j = 0;
    std::string key = "";
    std::string category = "";
    size_t next_id = 0;
    std::string id_key = "";

    while (pos != content.length())
    {
        i = 0;
        next_id = 0;
        while (i < location_ids.size())
        {
            //std::cout << "remaining content" << &content[pos] << std::endl;
            if (content.find(location_ids[i], pos) != std::string::npos && isspace(content.at(content.find(location_ids[i], pos) + location_ids[i].length())))
            {
                if (location_ids[i] != "location" && (content.find(location_ids[i], pos) < next_id || next_id == 0))
                {
                    id_key = location_ids[i];
                    next_id = content.find(id_key, pos);
                }
            }
            i++;
        }
        if (http.data()[http.size() - 1]["location " + location_name][id_key].empty())
        {
            pos = content.find(id_key, pos);
            category = "location " + location_name;
            key = id_key;
        }
        while (pos + j < content.length() && isspace(content.at(pos + j)))
            j++;
        std::cout << "pos + j " << pos + j << std::endl;
        std::cout << "content length " << content.length() << std::endl;
        if (pos + j == content.length())
            return TRUE;
        else if (content.find(";", pos) != std::string::npos)
            idx = content.find(";", pos);
        else
        {
            std::cout << "c ici que ca merde" << std::endl;
            return FALSE;
        }
        j = 0;
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
    size_t next_id = 0;
    std::string id_key = "";
    std::string location_name = "";

    while (pos != content.length())
    {
        is_location = 0;
        i = 0;
        next_id = 0;
        while (i < server_ids.size())
        {
            //std::cout << "remaining content" << &content[pos] << std::endl;
            if (content.find(server_ids[i], pos) != std::string::npos && isspace(content.at(content.find(server_ids[i], pos) + server_ids[i].length())))
            {
                if (content.find(server_ids[i], pos) < next_id || next_id == 0)
                {
                    id_key = server_ids[i];
                    next_id = content.find(id_key, pos);
                }
            }
            i++;
        }
        std::cout << "id_key!!!!!!!!!!!!!!!" << id_key << std::endl;
        if (id_key == "location")
        {
            location_name = content.substr(content.find(id_key, pos) + id_key.length(), content.substr(content.find(id_key, pos) + id_key.length()).find("{", 0));
            location_name = location_name.substr(location_name.find_first_not_of("\t\n\r\v\f "), location_name.find_last_not_of("\t\n\r\v\f "));
            std::cout << "location name " << location_name << "*" << std::endl;
            setLocationId(location_name);
            if (content.find("{", pos) != std::string::npos)
                blockLocation = getBlockLocation(&content[content.find("{", pos) + 1]);
            if (isValidLocation(blockLocation, location_name) == FALSE)
            {
                std::cout << "1hey la" << std::endl;
                return FALSE;
            }
            std::cout << "BLOCKLOCATION" << blockLocation <<  std::endl;
            if (content.find("}", pos) != std::string::npos)
                pos = content.find("}", pos) + 1;
            std::cout << "CONTENT = " << &content[pos] << std::endl;
            while (pos + j < content.length() && isspace(content.at(pos + j)))
            {
                std::cout << "posat pos + j " << content.at(pos + j);
                j++;
            }
            is_location = 1;
            if (pos + j == content.length())
                return TRUE;
            j = 0;
        }
        else
        {
            if (content.find(id_key, pos) != std::string::npos)
                pos = content.find(id_key, pos);
            category = "server";
            key = id_key;
        }
        if (is_location == 0)
        {
            std::cout << "pos" << pos << std::endl;
            std::cout << "j" << j << std::endl;
            std::cout << "&content[pos + j] " << content.substr(pos + j, content.length() - (pos + j)) << std::endl;
        while (pos + j < content.length() && isspace(content.at(pos + j)))
        {
            std::cout << "posat pos + j " << content.at(pos + j);
            j++;
        }
        //std::cout << "pos + j" << pos + j << std::endl;
        std::cout << "content length" << content.length() << std::endl;
        if (pos + j == content.length())
            return TRUE;
        else if (content.find(";", pos) != std::string::npos)
            idx = content.find(";", pos);
        else
        {
            std::cout << "2 heyla" << std::endl;
            return FALSE;
        }
        j = 0;
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
    //pushLocationIds(location);
    //while (std::isspace(static_cast<unsigned char>(content.at(pos))))
    //    pos++;
    //while (i < server_ids.size() && (pos = content.find(server_ids[i], pos)) == std::string::npos)
    //    i++;
    //if (server_ids[i] == "" && pos == std::string::npos)
    //    return FALSE;
    std::map< std::string, std::map< std::string, std::vector< std::string > > > map_ids;
    map_ids.insert(std::make_pair("server", server));
    //map_ids.insert(std::make_pair("location", location));
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
    while (server_idx != content.length())
    {
        if ((server_idx = content.find("server", server_idx)) != std::string::npos)
        {
            //server_idx = content.find("server", server_idx);
            if (content.find("{", server_idx + 7) != std::string::npos)
                blockServer = getBlockServer(&content[content.find("{", server_idx + 7) + 1]);
            //server_idx = content.find(blockServer, server_idx) + blockServer.length();
            server_idx = content.find("{", server_idx + 7) + blockServer.length();
            if (setValues(blockServer) == FALSE)
                return FALSE;
            if (isValidServer(blockServer) == FALSE)
            {
                std::cout << "ici" << std::endl;
                return FALSE;
            }
            std::cout << "BLOCKSERVER *********" << blockServer << std::endl;
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
        else
            return TRUE;
    }
    return TRUE;
}

void serverConf::printMap()
{
    size_t i = 0;
    size_t j = 0;
    //http.data()[http.size() - 1][category].insert(std::make_pair(key, value));
    while (i < http.size())
    {
        std::cout << "indice : [" << i << "]" << std::endl;
        std::cout << "*************" << std::endl;
        for (std::map< std::string, std::map< std::string, std::vector< std::string > > >::iterator it = http.data()[i].begin(); it != http.data()[i].end(); it++)
        //while (j < http.data()[i]["server"].size())
        {
            std::cout << "clé générale : [" << it->first << "]";
            //std::cout << "clé : [" << server_ids[j] << "]";
            for (std::map< std::string, std::vector< std::string > >::iterator itk = http.data()[i][it->first].begin(); itk != http.data()[i][it->first].end(); itk++)
            {
                std::cout << " | clé [" << itk->first << "]";
                while (j < itk->second.size())
                {
                    std::cout << " | valeur [" << itk->second[j] << "]";
                    j++;
                }
                j = 0;
            }
            std::cout << std::endl;
        }
        std::cout << "*************" << std::endl;
        std::cout << std::endl;
        i++;
    }
}

int serverConf::getData()
{
    std::cout << "Pour chaque serveur, récupérer son port (crea socket) " << std::endl;
    size_t i = 0;
    size_t j = 0;

    while (i < http.size())
    {
        while (j < http.data()[i]["server"]["listen"].size())
        {
            std::cout << http.data()[i]["server"]["listen"][j] << std::endl;
            j++;
        }
        j = 0;
        i++;
    }
    std::cout << "Pour chaque serveur, récupérer son nbr max de connexion(taille) (crea socket) " << std::endl;
    i = 0;
    j = 0;

    while (i < http.size())
    {
        while (j < http.data()[i]["server"]["client_max_body_size"].size())
        {
            std::cout << http.data()[i]["server"]["client_max_body_size"][j] << std::endl;
            j++;
        }
        j = 0;
        i++;
    }
    std::cout << "Pour chaque serveur, recupérer son nom (check socket) " << std::endl;
    i = 0;
    j = 0;

    while (i < http.size())
    {
        while (j < http.data()[i]["server"]["server_name"].size())
        {
            std::cout << http.data()[i]["server"]["server_name"][j] << std::endl;
            j++;
        }
        j = 0;
        i++;
    }
    std::cout << "pour un serveur, recupere le port (gestion requete) " << std::endl;
    i = 2; //index du serveur;
    j = 0;

    while (j < http.data()[i]["server"]["listen"].size())
    {
        std::cout << http.data()[i]["server"]["listen"][j] << std::endl;
        j++;
    }
    std::cout << "pour un serveur, recuperer error page(gestion requete) " << std::endl;
    i = 0; //index du serveur;
    j = 0;

    while (j < http.data()[i]["server"]["error_page"].size())
    {
        std::cout << http.data()[i]["server"]["error_page"][j] << std::endl;
        j++;
    }
    std::cout << "pour un serveur, recuperer  la ou les roots (gestion requete) " << std::endl;
    i = 0; //index du serveur;
    j = 0;

    while (j < http.data()[i]["server"]["root"].size())
    {
        std::cout << http.data()[i]["server"]["root"][j] << std::endl;
        j++;
    }
    std::cout << "pour un serveur et une location, recuperer l'index " << std::endl;
    i = 0;
    j = 0;
    std::map< std::string, std::map< std::string, std::vector< std::string > > >::iterator it = http.data()[i].begin();
    std::map< std::string, std::map< std::string, std::vector< std::string > > >::iterator ite = http.data()[i].end();

    for(; it != ite; it++)
    {
        if (it->first == "server")
        {
            while (j < http.data()[i][it->first]["index"].size())
            {
                std::cout << http.data()[i][it->first]["index"][j] << std::endl;
                j++;
            }
        }
        else
        {
            j = 0;
            std::cout << "nom de la location : " << it->first << std::endl;
            while (j < http.data()[i][it->first]["index"].size())
            {
                std::cout << http.data()[i]["server"]["client_max_body_size"][j] << std::endl;
                j++;
            }
        }
    }
    //pour un serveur et une location, recuperer autoindex
    //pour un serveur et une location, recuperer l'upload dir
    //pour un serveur et une location, recuperer cgi
    //pour un serveur et une location, recuperer redirect
    // -> pareil
    return 0;
}

int main(void)
{
    serverConf conf;
    std::vector< std::string > empty;

    conf.server_ids.push_back("listen");
    conf.server_ids.push_back("server_name");
    conf.server_ids.push_back("location");
    conf.server_ids.push_back("error_page");
    conf.server_ids.push_back("client_max_body_size");
    conf.server_ids.push_back("root");
    conf.server_ids.push_back("index");
    conf.server_ids.push_back("return");
    conf.server_ids.push_back("cgi");
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
    conf.location_ids.push_back("method");
    conf.location_ids.push_back("default");
    conf.location_ids.push_back("upload");
    conf.location_ids.push_back("dirList");
    std::string file = "example.conf";
    std::string output = "";
    output += getContent(file);
    std::cout << output << std::endl;
    std::cout << "bool is valid : " << conf.parseContent(output) << std::endl;
    conf.printMap();
    conf.getData();
    return TRUE;
}