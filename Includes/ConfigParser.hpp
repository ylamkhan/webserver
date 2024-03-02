#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "../Includes/webserver.hpp"
#include "../Includes/Server_Data.hpp"
#include "../Includes/Location.hpp"

class Server;
class Location;

class ConfigParser {

    private:
        std::vector<Server> servers;
    public:
        ConfigParser() {};
        ConfigParser(std::string file);
        void parseConfig(std::string file);
        bool isEmptyLine(const std::string &line);
        void fillServers(std::vector<std::string> lines);
        void fillServer(std::vector<std::string> &lines, size_t *i);
        void fillLocations(std::vector<std::string> &lines, size_t *i, Server &server);
        void fillLocation(std::vector<std::string> lines, size_t *i, Server &server);
        void saveLocRoot(Location &location, std::vector<std::string> &lines, size_t *i);
        void saveUpload(Location &location, std::vector<std::string> &lines, size_t *i);
        void saveAutoindex(Location &location, std::vector<std::string> &lines, size_t *i);
        void saveMethods(Location &location, std::vector<std::string> &lines, size_t *i);
        void saveIndex(Location &location, std::vector<std::string> &lines, size_t *i);
        void saveCgiPaths(Location &location, std::vector<std::string> &lines, size_t *i);
        void saveProxy(Location &location, std::vector<std::string> &lines, size_t *i);
        void fillOther(std::vector<std::string> &lines, size_t *i, Server &server);
        void saveHost(Server &server, std::vector<std::string> &lines, size_t *i);
        void savePort(Server &server, std::vector<std::string> &lines, size_t *i);
        void saveRoot(Server &server, std::vector<std::string> &lines, size_t *i);
        void saveErrorPages(Server &server, std::vector<std::string> &lines, size_t *i);
        void fillLocPath(Location &location, std::vector<std::string> &lines, size_t *i);
        void saveBodySize(Server &server, std::vector<std::string> &lines, size_t *i);
        void saveName(Server &server, std::vector<std::string> &lines, size_t *i);
        void saveRedirUrl(Location &location, std::vector<std::string> lines, size_t *i);
        void saveLocErrorPages(Location &location, std::vector<std::string> lines, size_t *i);
        void saveSRedirUrl(Server &server, std::vector<std::string> lines, size_t *i);
        void saveSCgiPaths(Server &server, std::vector<std::string> lines, size_t *i);
        void saveSMethods(Server &server, std::vector<std::string> &lines, size_t *i);
        void saveSIndex(Server &server, std::vector<std::string> lines, size_t *i);
        void saveSAutoindex(Server &server, std::vector<std::string> &lines, size_t *i);
        void saveList(Location &location, std::vector<std::string> lines, size_t *i);
        std::vector<Server> &getServers();
        
};

#endif