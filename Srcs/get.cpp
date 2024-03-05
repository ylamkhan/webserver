#include "../Includes/Client.hpp"


// std::string to_string(int value)
// {
//     std::stringstream ss;
//     ss << value;
//     return ss.str();
// }

// void    write_in_socket(std::string source, int sockfd)
// {

  
//    std::ifstream a_file(source.c_str());

//     if (!a_file.is_open()) 
//     {
//         std::cout << "not open \n";
//         return ;
//     }
//         char stri[1024];
//         while (true)
// 	    {
            
// 			a_file.read(stri, sizeof(stri) - 1);
// 			if (a_file.fail() && !a_file.eof())
// 			{
// 				std::perror("read");
// 				return ;
// 			}
// 			if (a_file.gcount() == 0)
// 				{
// 					break ;
// 				}
// 			stri[a_file.gcount()] = '\0';
// 			std::string bref(stri, a_file.gcount());
//             write(sockfd,stri,  a_file.gcount());
// 	    }

//         a_file.close();            

// }

// void    send_to_client( std::string cmp_req, std::string  source ,int  sockfd , std::string  Content_type)
// {
//     std::string response;
//      response = "HTTP/1.1 200 OK\n"
//                         "Content-Type: " + Content_type + "\n"
//                         "Content-Length: " + to_string(cmp_req.size()) + "\n\n"; 
    
//     write(sockfd, response.c_str(), response.size());
//     write_in_socket(source, sockfd);
// }
// int Client::checkforCgi(std::string type)
// {
//     vecSPair cgis = servers[sindex].getLocations()[lindex].getCgi();

//     for (size_t i = 0; i < cgis.size(); i++)
//     {
//         if ("." + cgis[i].first == type)
//         {
//             executable = cgis[i].second;
//             return 1;
//         }
//     }

//     return 0;
// }
// void Client::get()
// {
//     // std::cout<<servers[sindex].getLocations()[lindex].getUpload()<<"-*----\n";
//     url = "./Srcs/" + reqURL;
//     //haydna slash mn lekher dyal url
//     // if (url[url.size()-1] == '/')
//     //     url.erase(url.size()-1);

//     DIR *dir;
//      struct dirent *entry;

//     std::string rep;
//     dir = opendir(url.c_str());
//     std::string lien;

//     if(dir != NULL )
//     {
//         while ((entry = readdir(dir)) != NULL) 
//         {
//             std::string entry_name = entry->d_name;
//             std::string name = entry_name;
//             if(entry_name != "." && entry_name != "..")
//             {
//                 entry_name =  entry_name;
//                 // std::cout << entry_name << "\n";
//                 rep += "<li><a href=\"" + entry_name + "\">" + name + "</a></li>";
//             }
//         }
//         rep += "</ul>";
//         std::string response;
//         response = "HTTP/1.1 200 OK\n"
//                         "Content-Type: text/html\n"
//                         "Content-Length: " + to_string(rep.size()) + "\n\n" + rep; 
    
    
//         if(write(sockfd, response.c_str(), response.size())<0)
//         {
//             // std::cout<<"vvvvvvvvvvvvvvvvvvvvvvvvvv";
//             exit(0);
//         }

//         closedir(dir);
//         flag_in_out = true;

    
//     }
//     else
//     {
//         std::ifstream file(url.c_str());
//         if (!file.is_open())
//         {
//             std::cout << "error to opeb file \n";
//             std::string response = "HTTP/1.1 404 Not Found\n"
//                             "Content-Type: text/html\n"
//                             "Content-Length: 0\n\n";
//             send(sockfd, response.c_str(), response.size(), 0);
//             close(sockfd);
//             return ;
//         }
        
//         std::string type;
//         size_t t = url.rfind(".");
//         if(t != std::string::npos)
//         {
//             type = url.substr(t);
//         }
//         //hadchi li zdt dyal cgi
//         if(servers[sindex].getLocations()[lindex].getCgi().size())
//         {
//             if (checkforCgi(type))
//                 cgi();
//         }
//         char body[1024];
//         std::string cmp_req;
//         while (true)
//         {
//             file.read(body, sizeof(body) - 1);
//             if (file.fail() && !file.eof())
//             {
//                 std::cerr << "error read file\n";
//                 return ;
//             }
//             if (file.gcount() == 0)
//                 {
//                     break ;
//                 }
//             body[file.gcount()] = '\0';
//             std::string p_req(body, file.gcount());
//             cmp_req += p_req;
//         }
//         file.close();

//         store_type();
//         std::string Content_type; 
//         std::map<std::string, std::string>::iterator it;
//         for(it = mime_type.begin() ; it != mime_type.end(); ++it)
//         {
//             std::string first = it->first;
//             std::string second = it->second;
//             if(second == type )
//             {
//                 Content_type = first;
//                 send_to_client(cmp_req, url , sockfd , Content_type);
//                 break;
//             }
//         }
//         flag_in_out = true;

//     }
// }

/********************************************************************************************************/





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

// void    Client::readFile(std::string u)
// {
//     (void)u;
//     char buffer[1024];

//     if (!flagResponse)
//     {
//         std::string response;
//         response = "HTTP/1.1 200 OK\r\n"
//                         "Content-Type: text/html\r\n\r\n";
//         write(sockfd, response.c_str(), response.size());
//         flagResponse = true;
//         return ;
//     }
//     a_file.read(buffer, sizeof(buffer) - 1);
//     if (a_file.eof())
//     {
//         std::cout << "close fd " << std::endl;
//         a_file.close();
//         flag_in_out = true;
//     }
//     write(sockfd, buffer, sizeof(buffer));

// }

void Client::get()
{
    if (!Opened)
    {
        url = "./Srcs/" + reqURL;
        dir = opendir(url.c_str());
        if (dir != NULL)
        {
            std::cout << "Dir " << url << std::endl;
            isDir = true;
            Opened = true;
            closedir (dir);
        }
        else
        {
            a_file.open(url.c_str(), std::ios::in | std::ios::binary);
            std::cout << "file: " << url << std::endl;
            if (a_file.is_open())
            {
                isFile = true;
                Opened = true;
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
        Location loc = servers[sindex].getLocations()[lindex];
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
                            startRead = true;
                            cgiflag = true;
                            cgi(getUrl);
                            flag_in_out = true;
                            break;
                        }
                    }
                    a_file.open(getUrl.c_str(), std::ios::in | std::ios::binary);
                    if (a_file.is_open())
                    {
                        std::cout<<"lllllllllllllllllllllllllllllllllllll\n";
                        // startRead = true;
                        flag_in_out = true;
                        break;
                    }
                    
                }
            }

        }
        else if (loc.getList() == "ON")
        {
            exit(12);
            listDir();
        }
        else
        {
            exit(21);
            message = "404 Not Found";
            status = 404;
            flag_in_out = true;
            return ;
            flag_in_out = true;
        }
    }
    else if (isFile)
    {

        exit(45);
        flag_in_out = true;
    }
    else
        exit(48);
}

void    Client::listDir()
{
    struct dirent *entry;
    std::string rep;

    while ((entry = readdir(dir)) != NULL) 
    {
        std::string entry_name = entry->d_name;
        std::string name = entry_name;
        if(entry_name != "." && entry_name != "..")
        {
            entry_name =  entry_name;
            // std::cout << entry_name << "\n";
            rep += "<li><a href=\"" + entry_name + "\">" + name + "</a></li>";
        }
    }
    rep += "</ul>";
    std::string response;
    response = "HTTP/1.1 200 OK\n"
                    "Content-Type: text/html\n";


    // if(write(sockfd, response.c_str(), response.size())<0)
    // {
    //     // std::cout<<"vvvvvvvvvvvvvvvvvvvvvvvvvv";
    //     exit(0);
    // }
}