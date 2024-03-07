#include "../Includes/Client.hpp"

void Client::remove_directory_file(const std::string& name)
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir(name.c_str());

    if (dir != NULL)
    {
      
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
                    remove_directory_file(full_path);
                }
                else
                {
                    std::ifstream file(full_path.c_str());
                    if (file.is_open())
                    {
                        file.close();
                        if (std::remove(full_path.c_str()) == 0) 
                        {
                            status = 204;
                            message = "204 NO Content";
                        } 
                        else 
                        {
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
                flag_in_out = true;
            }
            else
            {
                struct stat fileStat;
                if(stat(name.c_str(), &fileStat)==0) // success
                {
                    Permissions = fileStat.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
                    if(Permissions & S_IWUSR)
                    {
                        status = 500;
                        message = "500 Internal Server Error";
                        flag_in_out = true;
                    }
                    else 
                    {
                        status = 403;
                        message = "403 Forbiden";
                        flag_in_out = true;
                    }
                }

                
            }
        
    }
    else //  a file
    {  
        std::ifstream file(name.c_str());
        if (file.is_open()) 
            {
                file.close();
                if (std::remove(name.c_str()) == 0)
                {
                    status = 204;
                    flag_in_out = true;
                }
            }
    }        
}


void Client::web_delete()
{
   
    std::string url = "./Srcs/"  + reqURL;
    if (!can_open)
    {
        url = "./Srcs/" + reqURL;
        dir = opendir(url.c_str());
        if (dir != NULL)
        {
            can_open = true;
        }
        else
        {
            a_file.open(url.c_str(), std::ios::in | std::ios::binary);
            if (a_file.is_open())
            {
                can_open = true;
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

