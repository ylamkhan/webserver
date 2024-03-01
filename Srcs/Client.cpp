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
    sindex = 0;
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
    value_type = other.value_type;
    transfer_encoding = other.transfer_encoding;
    store = other.store; 
    save = other.save;
    shinka = other.shinka;
    sab = other.sab;
    ss = other.ss;
    sindex = other.sindex;
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
        end = requestStr.find(' ');
        method = requestStr.substr(pos, end - pos);
        //// 
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
        if(method == "POST")
            open_file();
        std::string hel = requestStr.substr(ch);
        chunks_size = hel.find("\r\n");
        if(chunks_size != std::string::npos)
        {
            chunks = hel.substr(0, chunks_size);
            hexSize = hexa_to_dec(chunks);
            store_hexSize = hexa_to_dec(chunks);
        }
        body = requestStr.substr(ch );

        std::map<std::string, std::string >::iterator it;
        it = headers.find("Transfer-Encoding");
        if( it != headers.end())
        {   
            transfer_encoding  = it->second;
            if(transfer_encoding == "chunked")
            {
                body = requestStr.substr(ch + chunks.size() + 2); // chunks
                ss = "\r\n" + chunks + "\r\n";

            }
        }
        headerSet = true;
    }
    else
    {
        body = httpRequest;
    }
    if(!matching_servers())
    {
        flag_in_out = true;
        std::cerr<<"Error: not found location\n";
        return ;
    }
    handl_methodes();
}

std::string check_block(std::string s)
{
    std::string tmp;
    std::istringstream iss(s);
    if (s == "" || s == "/")
        return "";
    std::getline(iss, tmp, '/');
    return tmp;
}
int Client::matching_servers()
{
    std::vector<Location> vecl = servers[sindex].getLocations();
    int matched = 0;
    std::string block = "/" + check_block(path.substr(1));
    std::string sub = path.substr(block.size());
    std::string c;
    bool rl = 0;
    int index = 0;
    std::string root;

    while (!matched)
    {
        for (size_t j = 0; j < vecl.size(); j++)
        {
            std::string p = vecl[j].getLocationPath();
            if (p == "/")
            {
                rl = 1;
                index = j;
            }
            if(mattching(block, p))
            {
                root = vecl[j].getRoot();
                if (root[root.size()-1] == '/')
                    reqURL = root + path.substr(block.size()+1);
                else
                    reqURL = root + path.substr(block.size()); 
                matched = 1;
                return 1;
            }
        }
        c = check_block(sub.substr(1));
        if (c == "")
        {
            if (rl)
            {
                root = vecl[index].getRoot();
                if (root[root.size()-1] == '/')
                    reqURL = root + path.substr(1);
                else
                    reqURL = root + path;
                return 1;
            }
            else
                return 0;
        }
        block = block + "/" + c;
        sub = sub.substr(c.size());
    }
    return 0;
}

// bool isLocationMatch(std::string location, std::string url)
// {
//     if(location[location.size()-1] != '/')
//         location.push_back('/');
//     if(url[url.size()-1] != '/')
//         url.push_back('/');
//     if(url.find(location) != std::string::npos)
//         return 1;
//     return 0;
// }

// void replaceHomeWithPath(std::string &url, std::string homeSubstring, std::string root)
// {

//     size_t pos = url.find(homeSubstring);
//     std::cout<<homeSubstring<<"\n";
//     if (pos != std::string::npos)
//         url.replace(pos, homeSubstring.length(),root);
// }

// int Client::matching_servers()
// {
//     std::vector<Location> vecl = servers[sindex].getLocations();
//     for (size_t j = 0; j < vecl.size(); j++)
//     {
//         if(isLocationMatch(vecl[j].getLocationPath(), path))
//         {
//             std::string root = vecl[j].getRoot();
//             if(root[root.size()-1] == '/')
//                root.erase(root.size()-1, 1);
//             replaceHomeWithPath(path,vecl[j].getLocationPath(),root);
//             std::cout<<path<<"      *********************\n";
//             return 1;
//         }
//     }
//     return 0;
// }


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


std::string fonction_to_strime(std::string str, std::string substr)
{

    std::string result = str;
    std::size_t pos = std::string::npos;
    
    while ((pos = result.find(substr)) != std::string::npos)
    {
        result.erase(pos, substr.length());
    }
    
    return result;
}

void   Client::handl_methodes()
{
    if (method == "GET")
    {
        flag_in_out = true;
    }
    else if (method == "POST")
    {
        std::map<std::string, std::string >::iterator it;
        it = headers.find("Transfer-Encoding");
        if( it != headers.end())
        {   
            transfer_encoding  = it->second;
        }
        //chunked
        if(transfer_encoding == "chunked")
        {
            if(store_hexSize > body.size())
            {
              std::cout << body  << "\n";
                if(save.size() <= hexSize)
                {
                    save += body;

                }
                if(save.size() > hexSize)
                {
                    store = save.substr(0, hexSize);
                    file.write(store.c_str(), store.size());
                    std::string sub2 = save.substr(hexSize);
                    size_t posa1 = sub2.find("\r\n");
                    size_t posa2;
                    if(posa1 != std::string::npos)
                    {
                        posa2 = sub2.find("\r\n", posa1 + 2 );
                        if(posa2 != std::string::npos)
                        {
                            shinka = sub2.substr(posa1 + 2, posa2 - posa1 - 2);
                            hexSize = hexa_to_dec(shinka);
                        }
                    }
                    sab = sub2.substr(posa2 + 2);
                    save.clear();
                    save += sab;
                }
                bodySize += body.size();
                if(bodySize >= (size_t)atoi(headers["Content-Length"].c_str()))
                {
                   
                    file.write(body.c_str(), body.size());
                    flag_in_out = true;
                    file.close();
                    return;
                }
            
            }
            else 
            {
                bodySize += body.size();
                if(body.find("\r\n0\r\n") != std::string::npos)
                {
                    std::string put = body.substr(0,body.find("\r\n0\r\n"));
                    body.clear();
                    body = put;
                }
                 if(bodySize >= (size_t)atoi(headers["Content-Length"].c_str()))
                {
                
                    file.write(body.c_str(), body.size());
                    flag_in_out = true;
                    file.close();
                    return;
                }
            }
        }
        else // binary
        { 
            bodySize += body.size();
            file.write(body.c_str(), body.size());
            if(bodySize   >= (size_t)atoi(headers["Content-Length"].c_str()))
                {
                    
                    file.write(body.c_str(), body.size());
                    flag_in_out = true;
                    file.close();
                    return;
                }

              
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
    
 
        std::map<std::string, std::string >::iterator it;
        it = headers.find("Content-Type");
        if( it != headers.end())
        {   
            value_type  = it->second;
        }
    // compare // bouandry 
    //    int result = str1.compare(0, 6, str2, 0, 6);
    // if(value_type.compare("multipart/form-data") ==  0)// kayna
    // {
    //     std::string form_data = it->second;
    // }
    store_type();
    std::string extension;
    std::map<std::string, std::string>::iterator at;
    at = mime_type.find(value_type);
    if( at != mime_type.end())
    {
        extension = at->second;
    }
    std::string name;
    if (!flag_open)
    {
        name_file += "t";
        name = "Srcs/upload/" + name_file + extension;  
        file.open(name.c_str(), std::ios::app);
        flag_open = true;
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



























