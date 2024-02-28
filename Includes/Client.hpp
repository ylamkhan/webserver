#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../Includes/httpRequest.hpp"

#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <fstream>
#include "../Includes/Server_Data.hpp"
#include "../Includes/ConfigParser.hpp"


static std::string name_file = "out";
class Socket;

class Client {
    private:
        int sockfd;
        std::ofstream file;

        std::vector<Server> servers;

        bool flag_open;
        bool flag_in_out;
        bool headerSet;

        std::string name_path;
        std::string requesta;
        std::string method;
        std::string path;
        std::string httpVersion;
        std::map<std::string, std::string>headers;
        std::string body;
        std::string requestStr;
        size_t content_len;
        size_t bodySize;

    public:
        Client(){};
        Client(std::vector<Server> &servers);
        ~Client();
        Client &operator=(Client const &other);
        Client(Client const &other);

        bool get_flag_in_out() const;

        int getSocket() const;
        void setFd(int fd);
        void open_file();
        std::string get_file_name() const ;
        void parseRequest(const std::string& httpRequest);
        void set_request_client(std::string requ);
        std::string getMethod() const;
        std::string getPath() const;
        std::string getHTTPVersion() const;
        std::map<std::string, std::string> getHeaders() const;
        std::string getBody() const;
        std::string getReqStr() const;
        void    setReqStr(std::string s);
        void    handl_methodes();
        int     matching_servers();
};

#endif