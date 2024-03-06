#include "../Includes/Client.hpp"


void Client::remove_directory_file(const std::string& name)
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir(name.c_str());

    if (dir != NULL)
    {
        Location loc = servers[sindex].getLocations()[lindex];
        if (loc.getCgi().size())
        {
            
            if (loc.isIndexSet())
            {
                std::cout << "-----------\n";
                // for(size_t i = 0; i < loc.getIndex().size(); i++)
                // {
                // getUrl = url;
                // if (url[url.size()-1] != '/')
                //     getUrl += "/";
                // getUrl += loc.getIndex()[i];
                // size_t t = getUrl.rfind(".");
                // if(t != std::string::npos)
                //     type = getUrl.substr(t);
                // if (checkforCgi(type) && !cgiflag)
                //     {
                //         startRead = true;
                //         cgiflag = true;
                //         cgi(getUrl);
                //         flag_in_out = true;
                //         break;
                //     }
                // a_file.open(getUrl.c_str(), std::ios::in | std::ios::binary);
                // if (a_file.is_open())
                //     {
                //         flag_in_out = true;
                //         break;
                //     }
                // }
            }
            else
                {
                    message = "403 Forbiden";
                    status = 403;
                    flag_in_out = true;
                    return ;   
                }
        }
        else // loc has not cgi ;
        {
            std::cout << "Directory: " << name << std::endl;
            while ((entry = readdir(dir)) != NULL)
            {
                std::string entry_name = entry->d_name;
                if (entry_name != "." && entry_name != "..")
                {
                std::string full_path = name + "/" + entry_name;
                DIR *sous_directory = opendir(full_path.c_str());
                if (sous_directory != NULL) 
                {
                    closedir(sous_directory);
                    // Directory
                    remove_directory_file(full_path);
                }
                else
                {
                    // File
                    std::ifstream file(full_path.c_str());
                    if (file.is_open())
                    {
                        file.close();
                        if (std::remove(full_path.c_str()) == 0) 
                        {
                            status = 204;
                            message = "204 NO Content";
                            std::cout << "file deleted: " << entry_name << std::endl;
                        } 
                        else 
                        {

                            std::cerr << "error delete file: " << entry_name << std::endl;
                            flag_in_out = true; ////// ?
                        }
                    }
                    else
                        {
                            status = 404;
                            message = "404 Not Found";
                            flag_in_out = true;
                            /////
                        }
                }
                }
            }

            closedir(dir);

            if (rmdir(name.c_str()) == 0)
            {
                status = 204;
                message = "204 NO Content";
                std::cout << "Directory deleted: " << name << std::endl;
                flag_in_out = true;
            }
            else
            {
            
                status = 403;
                message = "403 Forbiden";
                flag_in_out = true;
                // has write access on folder; ????
                std::cerr << "Error deleting directory: " << name << std::endl;
            }
        }
    }
    else //  a file
    {
        size_t e = name.rfind(".");
        std::string extension;
        extension = name.substr(e);



        Location loc = servers[sindex].getLocations()[lindex];
        if (loc.getCgi().size())
            {
                std::cout << "hheeeeee\n";
                if(checkforCgi(extension) == 1)
                {
                    std::cout << "heeloo \n";
                }
              //  status = 404; ///// >??gha dertha test osf
                flag_in_out = true;
            }
        
        else
            {
            std::ifstream file(name.c_str());
            if (file.is_open()) 
                {
                    file.close();
                    if (std::remove(name.c_str()) == 0)
                    {
                        std::cout << "tees\n";
                        status = 204; ///??????
                       
                        flag_in_out = true;
                    }
                }
            
            }
    }        
}


void Client::web_delete()
{
    std::string url = "./Srcs/"  + reqURL;
    std::cout << url << "\n";
    if (!can_open)
    {
        url = "./Srcs/" + reqURL;
        dir = opendir(url.c_str());
        if (dir != NULL)
        {
            can_open = true;
            // is_dir = true;
        }
        else
        {
            a_file.open(url.c_str(), std::ios::in | std::ios::binary);
            if (a_file.is_open())
            {
                can_open = true;
                // is_file = true;
            }
        }
        if (!can_open)
        {
            flag_in_out = true;
            message = "404 Not Found";
            status = 404;
            return ;
        }

    }
    remove_directory_file(url);
  
}

// nkhdem b stat