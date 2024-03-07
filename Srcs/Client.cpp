#include <iostream>
#include <string>
#include <cctype>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include "../Includes/Client.hpp"
#include<signal.h>

std::string to_string(int value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

Client::Client(std::vector<Server> &servers):servers(servers)
{
    sindex = 0;
    lindex = 0;
    Permissions = 0;
    flag_in_out = false;
    isCgi = false;
    cgiflag = false;
    flag_open = false;
    Opened = false;
    isDir = false;
    isFile = false;
    startRead = false;
    bodySize = 0;
    flagResponse = false;
    listing = false;
    closed = false;
    executable = "";
    query = "";
    message = "";
    type = "";
    cgiUrl = "";
    getUrl = "";
    reqURL = "";
    status = 200;
    can_open = false;
}

Client::~Client() {}

Client &Client::operator=(Client const &other)
{
    reqURL = other.reqURL;
    isCgi = other.isCgi;
    closed = other.closed;
    listing = other.listing;
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
    headers = other.headers; 
    mime_type = other.mime_type; 
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
    hcgi = other.hcgi;
    Permissions = other.Permissions;
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

bool Client::getClosed() const {
    return closed;
}

void Client::setflagResponse(bool t)
{
    flagResponse = t;
}

// std::string Client::parseFile(std::string f)
// {
//     std::ifstream fil(f);

//     if (fil.is_open())
//     {
//         std::getline(fil, headersCgi, "\r\n\r\n");
//         std::cout<<headersCgi<<"*****************************************\n";
//     }
//     return f;
// }

// void Client::parse_headers(std::string headersCgi)
// {
//     // std::istringstream iss(headersCgi);
//     // while (1)
//     // { 
//     //     std::getline(iss,hcgi.first,':');
//     //     std::getline(iss,hcgi.second,';');
//     //     if (hcgi.first == "Content-type")
//     //         Content_typa = ltrim(hcgi.second);
//     //     std::string temp;
//     //     iss >>temp;
//     //     if(temp.empty())
//     //         break;
//     // }
// }

void Client::send_client()
{
    if (method == "GET")
    {
        signal(SIGPIPE,SIG_IGN);
        if (listing)
        {
            listDir();}
        else
        {
            if (cgiflag)
            {
                isCgi = true;
                a_file.open((getUrl.substr(0,getUrl.find_last_of('/')+1 )+ "result.txt").c_str(), std::ios::in | std::ios::binary);
                cgiflag = false;
            }
            if (Opened)
            {
                size_t t = getUrl.rfind(".");
                std::string typa;
                if(t != std::string::npos)
                {
                    typa = getUrl.substr(t);
                }
                store_type();
                std::string Content_typa; 
                std::map<std::string, std::string>::iterator it;
                for(it = mime_type.begin() ; it != mime_type.end(); ++it)
                {
                    std::string first = it->first;
                    std::string second = it->second;
                    if(second == typa )
                    {
                        Content_typa = first;
                        break;
                    }
                }
                char buffer[1024] = {0};

                a_file.read(buffer, sizeof(buffer) - 1);

                std::string s(buffer, sizeof(buffer));
                //std::cout << "-----------------------------------\n" << buffer << "-----------\n";

                if (isCgi && s.find("\r\n\r\n") != std::string::npos)
                {
                    headersCgi = s.substr(0, s.find("\r\n\r\n"));
                    s = s.substr(s.find("\r\n\r\n")+4);
                    //parse_headers(headersCgi);
                    Content_typa = "text/html";
                }
                if (!flagResponse)
                {
                    std::cout<<Content_typa<<"\n";
                    std::string response;
                    response = "HTTP/1.1 200 OK\r\n"
                                    "Content-Type: " + Content_typa + "\r\n\r\n";
                    int a = write(sockfd, response.c_str(), response.size());
                    if(a <= 0)
                    {
                        perror("er1: ");
                    }
                    flagResponse = true;
                    // return;
                }
                int a = write(sockfd, s.c_str(), a_file.gcount());
                if(a <= 0)
                {
                    perror("er2: ");
                }
                if (a_file.eof() || a_file.fail() || a_file.gcount() == 0)
                {
                    a_file.close();
                    close(sockfd);
                    closed = true;
                }
            }
            else
            {
                close(sockfd);
                closed = true;
            }
        }
    }
    else if (method == "POST")
    {
        //poooooost ghalta ... rah dik ok ghe f succes;
        std::string msg;
        std::string response;
        if(status == 201)
        {
            msg = "Sucess ";
            response = "HTTP/1.1 200 OK \r\nContent-Type: text/html\r\n\r\n<html><body><h1>" + msg + "</h1></body></html>\r\n";

        }
        else 
            msg = "Failed";
        write(sockfd, response.c_str(), response.size());
        close(sockfd);
        closed = true;
    }
    else if (method == "DELETE")
    {       
        std::cout << status << "\n";
        std::string msg = "succes";
        std::string response;
        std::cout << status << "\n";
        
        if(status == 204) 
        {
            response = "HTTP/1.1 " + to_string(status) + " \r\nContent-Type: text/html\r\n\r\n";
        }       
        else 
        {
            response = "HTTP/1.1 " + to_string(status) + " \r\nContent-Type: text/html\r\n\r\n";
        }
        write(sockfd, response.c_str(), response.size());
        close(sockfd);
        closed = true;
    }
    else if (method.empty())
    {
        closed = true;
        close(sockfd);
    }
}

























