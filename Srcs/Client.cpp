#include <iostream>
#include <string>
#include <cctype>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include "../Includes/Client.hpp"


void	Client::store_type(void)
{
	mime_type["text/html"] = ".html";
	mime_type["text/css"] = ".css";
	mime_type["text/xml"] = ".rss";
	mime_type["image/gif"] = ".gif";
	mime_type["image/jpeg"] = ".jpeg";
	mime_type["application/x-javascript"] = ".js";
	mime_type["text/plain"] = ".txt";
	mime_type["text/x-component"] = ".htc";
	mime_type["text/mathml"] = ".mml";
	mime_type["image/png"] = ".png";
	mime_type["image/x-icon"] = ".ico";
	mime_type["image/x-jng"] = ".jng";
	mime_type["image/vnd.wap.wbmp"] = ".wbmp";
	mime_type["application/java-archive"] = ".jar";
	mime_type["application/mac-binhex40"] = ".hqx";
	mime_type["application/pdf"] = ".pdf";
	mime_type["application/x-cocoa"] = ".cco";
	mime_type["application/x-java-archive-diff"] = ".jardiff";
	mime_type["application/x-java-jnlp-file"] = ".jnlp";
	mime_type["application/x-makeself"] = ".run";
	mime_type["application/x-perl"] = ".pm";
	mime_type["application/x-pilot"] = ".prc";
	mime_type["application/x-rar-compressed"] = ".rar";
	mime_type["application/x-redhat-package-manager"] = ".rpm";
	mime_type["application/x-sea"] = ".sea";
	mime_type["application/x-shockwave-flash"] = ".swf";
	mime_type["application/x-stuffit"] = ".sit";
	mime_type["application/x-tcl"] = ".tk";
	mime_type["application/x-x509-ca-cert"] = ".crt";
	mime_type["application/x-xpinstall"] = ".xpi";
	mime_type["application/zip"] = ".zip";
	mime_type["application/octet-stream"] = ".deb";
	mime_type["application/octet-stream"] = ".bin";
	mime_type["application/octet-stream"] = ".dmg";
	mime_type["application/octet-stream"] = ".eot";
	mime_type["application/octet-stream"] = ".img";
	mime_type["application/octet-stream"] = ".msi";
	mime_type["audio/mpeg"] = ".mp3";
	mime_type["audio/x-realaudio"] = ".ra";
	mime_type["video/mpeg"] = ".mpeg";
	mime_type["video/quicktime"] = ".mov";
	mime_type["video/x-flv"] = ".flv";
	mime_type["video/x-msvideo"] = ".avi";
	mime_type["video/x-ms-wmv"] = ".wmv";
	mime_type["video/x-ms-asf"] = ".asx";
	mime_type["video/x-mng"] = ".mng";
	mime_type["video/mp4"] = ".mp4";
	mime_type["application/x-www-form-urlencoded"] = ".txt";
}



unsigned int hexa_to_dec(const std::string& str) {
    std::stringstream ss;
    ss << std::hex << str;
    unsigned int result;
    ss >> result;
    return result;
}

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
    headers = other.headers; //
    mime_type = other.mime_type; //
    body = other.body;
    requestStr = other.requestStr;
    headerSet = other.headerSet;
    content_len = other.content_len;
    bodySize = other.bodySize;
    servers = other.servers;
    name_type = other.name_type;
    value_type = other.value_type;
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
        size_t ch = requestStr.find("\r\n\r\n");
        if(ch != std::string::npos)
        {
            ch = ch + 4;
        }
        // transfer chunked;
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
            if(headerName == "Host")
                setPortHost(headerValue);
        }
        
    //hna kanet pos blast ch;
        body = requestStr.substr(ch);
    

        // if(transfer_encoding) kayen
        chunks_size = body.find("\r\n");
        if(chunks_size != std::string::npos)
        {
            chunks = body.substr(0, 5);
            hexSize = hexa_to_dec(chunks);

        }
        body = requestStr.substr(pos);
        headerSet = true;
    }
    else
    {
        body = httpRequest;
    }
    // if(!matching_servers())
    // {
    //     flag_in_out = true;
    //     std::cerr<<"Error: not found location\n";
    //     return ;
    // }
    handl_methodes();
}

void Client::setPortHost(std::string headerValue)
{
    std::istringstream iss(headerValue);
    std::getline(iss,host,':');
    iss>>port;
}


int Client::matching_servers()
{
    
    // size_t i = 0 ;
    // for (size_t i = 0; i < servers.size();i++)
    // {
    //     if(servers[i].getPort() != port || servers[i].getHost() != host)
    //         break;
    //     i++;
    // }
    // if(i != servers.size())
    // {
    //     for (size_t i = 0; i < servers.size(); i++)
    //     {
    //         if(servers[i].getPort()==port && servers[i].getHost() == host)
    //         {
    //             for (std::vector<Location>::iterator itl = servers[i].getLocations().begin(); itl != servers[i].getLocations().end(); ++itl)
    //             {
    //                 //std::cout<<*(it->getLocations().begin())<<"\n";
    //                 std::cout<<"Path: location---->"<<itl->getRoot()<<"\n";
    //                 //std::cout<<it->getLocations().size();
    //                 exit(0);
    //             }
                
    //         }
    //     }
    // }
    // else
    // {

    // }
    
    
//    std::cout<<servers.size()<<"\n";
//    std::cout<<host<<":";
//    std::cout<<port<<"\n";

   
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
        // std::string transfer_encoding = "chunked";
        // if(transfer_encoding == "chunked")
        // {
        //     if(hexSize > body.size())
        //     {
        //         std::size_t pos = body.find("\r\n10000");
        //         if( pos!= std::string::npos)
        //         {
        //             stock.append(body.substr(0, pos), pos);// append 
        //             lichat.append(body.substr( pos + 9));///
        //             file.write(stock.c_str(), stock.size());
        //             stock.clear();
        //             stock.append(lichat, lichat.size());
        //             lichat.clear();
        //         }
        //         else 
        //         {
        //             stock.append(body, body.size());
        //         }
                bodySize += body.size();
                if(bodySize >= (size_t)atoi(headers["Content-Length"].c_str()))
                {
                    std::cout << body << "\n";
                    flag_in_out = true;
                    file.close();
                    return;
                }
        //     }  
        // }  
    
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
    // store_type();
    // std::string extension;
    // std::map<std::string, std::string>::iterator it;
    // it = mime_type.find("video/mp4");
    // if( it != mime_type.end())
    // {
    //     extension = it->second;
    // }
    std::string name;
    
    if (!flag_open)
    {
        name_file += "t";
        name = "Srcs/upload/" + name_file + ".jpeg";  
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































// size = 10;

// size -= 2.;



// \r\n5\r\n
// 12





// /*


// 34
// \r\n0\r\n\r\n


// */