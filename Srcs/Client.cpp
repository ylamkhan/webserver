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
    sindex = 0;
    lindex = 0;
    flag_in_out = false;
    cgiflag = false;
    flag_open = false;
    Opened = false;
    isDir = false;
    isFile = false;
    startRead = false;
    bodySize = 0;
    flagResponse = false;
    executable = "";
    query = "";
    message = "";
    type = "";
    cgiUrl = "";
    getUrl = "";
    status = 200;
}

Client::~Client() {}

Client &Client::operator=(Client const &other)
{
    flagResponse = other.flagResponse;
    getUrl = other.getUrl;
    cgiUrl = other.cgiUrl;
    cgiflag = other.cgiflag;
    type = other.type;
    startRead = other.startRead;
    isFile = other.isFile;
    isDir = other.isDir;
    Opened = other.Opened;
    query = other.query;
    executable = other.executable;
    flag_open = other.flag_open;
    sockfd = other.sockfd;
    flag_in_out = other.flag_in_out;
    name_path = other.name_path;
    requesta = other.requesta;
    method = other.method;
    path = other.path;
    httpVersion = other.httpVersion;
    headers = other.headers; //
    mime_type = other.mime_type; //
    body = other.body;
    requestStr = other.requestStr;
    headerSet = other.headerSet;
    content_len = other.content_len;
    bodySize = other.bodySize;
    servers = other.servers;
    value_type = other.value_type;
    transfer_encoding = other.transfer_encoding;
    store = other.store; 
    save = other.save;
    shinka = other.shinka;
    sab = other.sab;
    ss = other.ss;
    sindex = other.sindex;
    lindex = other.lindex;
    message = other.message;
    status = other.status;
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

void Client::setPortHost(std::string headerValue)
{
    std::istringstream iss(headerValue);
    std::getline(iss,host,':');
    iss>>port;
}

int Client::mattching(std::string url, std::string pathloc)
{
    if (pathloc[0] != '/')
        pathloc = "/" + pathloc;
    if (url == pathloc)
        return 1;
    return 0;
}

// int Client::checkforCgi(std::string type)
// {
//     std::pair<std::string, std::string> cgi = servers[sindex].getCgi();
//     for (size_t i = 0; i < cgi.size(); i++)
//     {
//         if ()
//     }
// }

void   Client::handl_methodes()
{
    if (method ==       "GET")
    {
        get();
        // flag_in_out = true;

    }
    else if (method ==  "POST")
        post();
    else if (method ==  "DELETE")
        web_delete();
    else
    {
        flag_in_out = true;
        return ;
    }
}

void Client::setFd(int fd) {
    sockfd = fd;
}

void    Client::setSindex(size_t i) {
    sindex = i;
}

bool Client::get_flag_in_out() const{
    return flag_in_out;
}

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

std::string Client::getMessage() const {
    return message;
}

size_t Client::getStatus() const {
    return status;
}

bool Client::getflagResponse() 
{
    return flagResponse;
}
std::ifstream &Client::get_a_file()
{
    return a_file;
}

void Client::setflagResponse(bool t)
{
    flagResponse = t;
}



























