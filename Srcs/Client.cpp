#include <iostream>
#include <string>
#include <cctype>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include "../Includes/Client.hpp"

Client::Client(std::vector<Server> &servers):servers(servers)
{
    flag_in_out = false;
    flag_open = false;
    bodySize = 0;
}

Client::~Client() {}

Client &Client::operator=(Client const &other)
{
    flag_open = other.flag_open;
    sockfd = other.sockfd;
    flag_in_out = other.flag_in_out;
    name_path = other.name_path;
    requesta = other.requesta;
    method = other.method;
    path = other.path;
    httpVersion = other.httpVersion;
    headers = other.headers;
    body = other.body;
    requestStr = other.requestStr;
    headerSet = other.headerSet;
    content_len = other.content_len;
    bodySize = other.bodySize;
    servers = other.servers;
    return *this;
}


Client::Client(Client const &other)
{
    *this = other;
}

int Client::getSocket() const {
    return sockfd;
}

std::string Client::get_file_name() const 
{
    return name_path;
}

void Client::set_request_client(std::string requ)
{
    parseRequest(requ);
}

std::string& ltrim(std::string& str) {
    std::string::iterator it = str.begin();
    while (it != str.end() && std::isspace(*it)) {
        ++it;
    }
    str.erase(str.begin(), it);
    return str;
}

void    Client::parseRequest(const std::string& httpRequest)
{
    if (!headerSet)
        setReqStr(httpRequest);
    if (!headerSet && requestStr.find("\r\n\r\n", 0) != std::string::npos)
    {
        size_t pos = 0;
        size_t end = 0;
        end = requestStr.find(' ');
        method = requestStr.substr(pos, end - pos);
        if(method == "POST")
            open_file();
        pos = end + 1;
        end = requestStr.find(' ', pos);
        path = requestStr.substr(pos, end - pos);
        pos = end + 1;
        end = requestStr.find("\r\n", pos);
        httpVersion = requestStr.substr(pos, end - pos);
        pos = end + 2;
        while ((end = requestStr.find("\r\n", pos)) != std::string::npos)
        {
            std::string line = requestStr.substr(pos, end - pos);
            std::istringstream iss(line);
             std::string headerName, headerValue;  
            std::getline(iss,headerName,':');
            std::getline(iss,headerValue);
            headers[headerName] = ltrim(headerValue);
            pos = end + 2;
        }
        body = requestStr.substr(pos);
        headerSet = true;
    }
    else
        body = httpRequest;
    if(!matching_servers())
    {
        flag_in_out = true;
        std::cerr<<"Error: not found location\n";
        return ;
    }
    handl_methodes();
}

int Client::matching_servers()
{
   std::cout<<servers.size()<<"********\n";
   return 1;
}

void   Client::handl_methodes()
{
    if(method == "GET")
    {
        flag_in_out = true;
    }
    if(method == "POST")
    {
        file.write(body.c_str(), body.size());
        bodySize += body.size();
        if(bodySize >= (size_t)atoi(headers["Content-Length"].c_str()))
        {
            flag_in_out = true;
            file.close();
        }
    }
    else if (method == "DELETE")
    {
        flag_in_out = true;
    }
    else
    {
        flag_in_out = true;
        std::cerr<<"Error: not found method******\n";
        return ;
    }
}

void Client::open_file()
{
    std::string name;
    if (!flag_open)
    {
        name_file += "t";
        name = "Srcs/upload/" + name_file + ".mp4";  
        file.open(name.c_str(), std::ios::app);
        flag_open = true;
    }
}

void Client::setFd(int fd) {
    sockfd = fd;
}


bool Client::get_flag_in_out() const{
    return flag_in_out;
}

// request 

std::string Client::getMethod() const { 
    return method;
}

std::string Client::getPath() const {
    return path;
}

std::string Client::getHTTPVersion() const {
    return httpVersion;
}

std::map<std::string, std::string> Client::getHeaders() const {
    return headers;
}

std::string Client::getBody() const {
    return body;
}


std::string Client::getReqStr() const {
    return requestStr;
}

void Client::setReqStr(std::string s) {
    requestStr += s;
}

