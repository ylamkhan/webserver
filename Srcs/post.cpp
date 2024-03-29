#include "../Includes/Client.hpp"

void Client::open_file()
{
    Location loc = servers[sindex].getLocations()[lindex];
    if(servers[sindex].getLocations()[lindex].getUpload() == "ON")
    {
        std::map<std::string, std::string >::iterator it;
        it = headers.find("Content-Type");
        if( it != headers.end())
        {
            value_type  = it->second;
        }
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
            name = "./Srcs/upload/" + name_file + extension; 
            file.open(name.c_str(), std::ios::app);
            flag_open = true;
        }
        status =  201;
        message = "201 Created";

    }
    else
    {
        if(isDirectory(("./Srcs/" + reqURL).c_str()))
        {
            if (loc.isIndexSet())
            {
                for (size_t i = 0; i < loc.getIndex().size(); i++)
                {
                    getUrl = url;
                    if (url[url.size()-1] != '/')
                        getUrl += "/";
                    getUrl += loc.getIndex()[i];
                    size_t t = getUrl.rfind(".");
                    if(t != std::string::npos)
                        type = getUrl.substr(t);
                    if (loc.getCgi().size())
                    {
                        
                        if (checkforCgi(type) && !cgiflag)
                        {
                            cgiflag = true;
                            cgi(getUrl);
                            flag_in_out = true;
                            status = 200;
                            message = "200 OK";
                            return;
                        }
                    }
                    
                }
                status = 403;
                message = "403 Forbidden";
                flag_in_out = true;
                return;
                    
            }
            else 
            {
                status = 403;
                message = "403 Forbidden";
                flag_in_out = true;
                return;
            }
        }
        else
        {
            Location loc = servers[sindex].getLocations()[lindex];
            if (loc.getCgi().size())
            {
                size_t t = reqURL.rfind(".");
                if(t != std::string::npos)
                    type = reqURL.substr(t);
                if (checkforCgi(type))
                {
                    file.write(body.c_str(), body.size());
                    file.close();
                    cgi("./Srcs/" + reqURL);
                    cgiflag = true;
                    flag_in_out = true;
                    return ;
                }
            }
            else 
            {
                status = 403;
                message = "403 Forbidden";
                flag_in_out = true;
                return;
            }
        }

    }
   
     
}

bool Client::isDirectory(const char* path) {
    struct stat pathStat;
    if (stat(path, &pathStat) == 0) {
        return S_ISDIR(pathStat.st_mode);
    } else {
        std::cerr << "Error: Unable to get file information" << std::endl;
        return false; 
    }
}

void Client::post()
{
    open_file();
    if (!isDirectory(("./Srcs/" + reqURL).c_str()))
    {
        Location loc = servers[sindex].getLocations()[lindex];
        if (loc.getCgi().size())
        {
            size_t t = reqURL.rfind(".");
            if(t != std::string::npos)
                type = reqURL.substr(t);
            if (checkforCgi(type))
            {
                file.write(body.c_str(), body.size());
                file.close();
                cgi("./Srcs/" + reqURL);
                cgiflag = true;
                flag_in_out = true;
                status = 200;
                message = "200 OK";
                return ;
            }
            else 
            {
                status = 403;
                message = "403 Forbidden";
                flag_in_out = true;
                return;
            }
        }
        else 
        {
            status = 403;
            message = "403 Forbidden";
            flag_in_out = true;
            return;
        }

    }
    else if(isDirectory(("./Srcs/" + reqURL).c_str()))
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
                        sab = sub2.substr(posa2 + 2);
                        save.clear();
                        save += sab;
                    }
                    else
                    {
                        save.clear();
                        save += sub2;
                    }
                    
                }
                if(body.find("\r\n0\r\n") != std::string::npos)
                {
                    std::string put = save.substr(0,save.find("\r\n0\r\n")); 
                    file.write(put.c_str(), put.size());
                    flag_in_out = true;
                    file.close();
                    return;
                }
        
            }
            else
            {
                size_t f = body.find("\r\n0\r\n") ;
                if( f != std::string::npos)
                {
                    std::string put = body.substr(0,body.find("\r\n0\r\n"));
                    body.clear();
                    body = put;
                    file.write(body.c_str(), body.size());
                    flag_in_out = true;
                    file.close();
                    return;


                }
                else
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
    else
    {
        status = 404;
        message = "404 Not Found";
        flag_in_out = true;
    }
}