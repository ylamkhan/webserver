#include "../Includes/Client.hpp"


int Client::checkforCgi(std::string type)
{
    vecSPair cgis = servers[sindex].getLocations()[lindex].getCgi();

    for (size_t i = 0; i < cgis.size(); i++)
    {
        if ("." + cgis[i].first == type)
        {
            executable = cgis[i].second;
            return 1;
        }
    }

    return 0;
}

void Client::get()
{
    Location loc = servers[sindex].getLocations()[lindex];
    // if (loc.isRedirUrlSet())
    // {
    //     std::string s = reqURL.substr(loc.getRoot().size());
    //     if (s == loc.getRedirUrl())
    //     {
    //         status = 301;
    //         message = loc.getRedirUrl();
    //         flag_in_out = true;
    //         return;
    //     }
    // }
    if (!Opened)
    {
        url = "./Srcs/" + reqURL;
        dir = opendir(url.c_str());
        if (dir != NULL)
        {
            isDir = true;
            Opened = true;
        }
        else
        {
            a_file.open(url.c_str(), std::ios::in | std::ios::binary);
            if (a_file.is_open())
            {
                isFile = true;
                Opened = true;
                a_file.close();
            }
        }
        if (!Opened)
        {
            message = "404 Not Found";
            status = 404;
            flag_in_out = true;
            return ;
        }
    }
    if (isDir)
    {

        if (loc.getAutoIndex())
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
                            break;
                        }
                    }
                    a_file.open(getUrl.c_str(), std::ios::in | std::ios::binary);
                    if (a_file.is_open())
                    {
                        flag_in_out = true;
                        status = 200;
                        message = "200 OK";
                        break;
                    }
                    else
                    {
                        if(loc.getList() == "ON")
                        {
                            listing = true;
                            flag_in_out = true;
                            status = 200;
                            message = "200 OK";
                            return ;
                        }
                        else
                        {
                            message = "403 Forbidden";
                            status = 403;
                            flag_in_out = true;
                            return ;
                        
                        }
                    }
                }
            }
        }
        else
        {
            if(loc.getList() == "ON")
            {
                listing = true;
                flag_in_out = true;
            }
             else
            {
                message = "403 Forbidden";
                status = 403;
                flag_in_out = true;
                return ;
            
            }
        }
    }
    else if (isFile)
    {

        size_t t = url.rfind(".");
        if(t != std::string::npos)
            type = url.substr(t);
        if (loc.getCgi().size())
        {
            if (checkforCgi(type) && !cgiflag)
            {
                getUrl = (url.substr(0,url.find_last_of('/')+1) + "result.txt");
                // startRead = true;
                cgiflag = true;
                cgi(url);
            }
        }
        if (!cgiflag)
        {
            a_file.open(url.c_str(), std::ios::in | std::ios::binary);
            getUrl = url;
        }
        flag_in_out = true;
        status = 200;
        message = "200 OK";

    }
    else
    {
    
        std::cout << "ma dir ma file\n"; //////////////////? 
    }
}

void    Client::listDir()
{
    struct dirent *entry;
    std::string rep;
    std::string response;
    response = "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/html\r\n\r\n";

    while ((entry = readdir(dir)) != NULL) 
    {
        std::string entry_name = entry->d_name;
        std::string name = entry_name;
        if(entry_name != "." && entry_name != "..")
        {
            if (path[path.size()-1] == '/')
            {
                entry_name = path +  entry_name;
            }
            else
                entry_name = path + "/" + entry_name;   
            response += "<li><a href=\"" + entry_name + "\">" + name + "</a></li>";
        }
    }
    response += "</ul>";
    write(sockfd, response.c_str(), response.size());
    close(sockfd);
}


