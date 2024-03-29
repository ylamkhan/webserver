#include "../Includes/Client.hpp"
typedef  std::map<std::string, std::string>::iterator iter_map;


unsigned int Client::hexa_to_dec(const std::string& str)
{
    std::stringstream ss;
    ss << std::hex << str;
    unsigned int result;
    ss >> result;
    return result;
}

std::string& Client::ltrim(std::string& str)
{
    std::string::iterator it = str.begin();
    while (it != str.end() && std::isspace(*it)) {
        ++it;
    }
    str.erase(str.begin(), it);
    return str;
}

void    Client::parseRequest(const std::string& httpRequest)
{
    if (method.empty() && httpRequest.empty())
    {
        flag_in_out = true;
        return ;
    }
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
        pos = end + 1;
        end = requestStr.find(' ', pos);
        std::istringstream iss(requestStr.substr(pos, end - pos));
        std::getline(iss,path,'?');
        iss>>query;
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
        if (!is_req_well_formed())
        {
            // Response();
            flag_in_out = true;
            return ;
        }
    
        std::string hel = requestStr.substr(ch);
        chunks_size = hel.find("\r\n");
        if(chunks_size != std::string::npos)
        {
            chunks = hel.substr(0, chunks_size);
            hexSize = hexa_to_dec(chunks);
            store_hexSize = hexa_to_dec(chunks);
        }
        body = requestStr.substr(ch);

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
    if (headerSet)
    {
        if(!matching_servers())
        {
            std::cout<<"oooooooooooooooooooooooooooo\n";
            flag_in_out = true;
            status = 404;
            message = "404 Not Found";
            return ;
        }
         Location loc = servers[sindex].getLocations()[lindex];
        if (loc.isRedirUrlSet())
        {
            status = 301;
            message = "301 Moved Permanently";
            flag_in_out = true;
            return;
        }
        std::cout<<method<<"***\n";
        if(!checkMethod())
        {
            flag_in_out = true;
            message = "405 Method Not Allowed";
            status = 405;
            return ;
        }
        handl_methodes();
    }
}

int getMethodIndex(std::string method)
{
    if (method == "GET")
        return 0;
    else if (method == "POST")
        return 1;
    else if (method == "DELETE")
        return 2;
    return -1;
}

int Client::checkMethod()
{
    std::vector<int> methods = servers[sindex].getLocations()[lindex].getMethods();
    if(methods.size())
    {
        for (size_t i = 0; i < methods.size(); i++)
        {
            std::cout<<methods.size()<<"---\n";
            if (getMethodIndex(method) == methods[i])
                return 1;
        }
        return 0;
    }
    else
    {
        std::vector<int> methods = servers[sindex].getMethods();
        for (size_t i = 0; i < methods.size(); i++)
        {
            if (getMethodIndex(method) == methods[i])
                return 1;
        }
        return 0;
    }
}

int Client::is_req_well_formed()
{
    iter_map it = headers.find("Transfer-Encoding");
    if(it != headers.end() && method == "POST" && it->second != "chunked")
    {
        message = "501 Not Implemented";
        status = 501;
        return 0;  
    }
    iter_map it1 = headers.find("Content-Length");
    if(method == "POST" && it == headers.end() && it1 == headers.end())
    {
        message = "400 Bad Request";
        status = 400;
        return 0;
    }
    const std::string allowedChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~:/?#[]@!$&'()*+,;=%";
    std::size_t found = path.find_first_not_of(allowedChars);
    std::size_t found1 = query.find_first_not_of(allowedChars);
    if((!path.empty() && found != std::string::npos) || (!query.empty() && found1 != std::string::npos))
    {
        message = "400 Bad Request";
        status = 400;
        return 0;
    }
    if(path.length() + query.length() > 2048)
    {
        message = "414 Request-URL Too Long";
        status = 414;
        return 0;
    }

    if(method == "POST" && it1 != headers.end())
    {
        if((size_t)atoi(it1->second.c_str()) > servers[sindex].getMaxClientBodySize() && servers[sindex].getMaxClientBodySize() != 0)
        {
            message = "413 Request Entity Too Large";
            status = 413;
            return 0;
        }
    }

    return 1; 
}