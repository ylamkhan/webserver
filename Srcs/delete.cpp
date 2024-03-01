#include "../Includes/Client.hpp"


void remove_directory_file(const std::string& name)
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir(name.c_str());
    // its a directory
    if (dir != NULL) {
        std::cout << "Directory: " << name << std::endl;

        while ((entry = readdir(dir)) != NULL) {
            std::string entry_name = entry->d_name;

            if (entry_name != "." && entry_name != "..") {
                std::string full_path = name + "/" + entry_name;

                DIR *sous_directory = opendir(full_path.c_str());

                if (sous_directory != NULL) {
                    closedir(sous_directory);
                    // Directory
                    std::cout << "sous_directoryectory: " << entry_name << std::endl;
                    remove_directory_file(full_path);
                } else
                 {
                    // File
                    std::ifstream file(full_path.c_str());
                    if (file.is_open())
                     {
                        file.close();
                        if (std::remove(full_path.c_str()) == 0) 
                        {
                            std::cout << "file deleted: " << entry_name << std::endl;
                        } 
                        else 
                        {
                            std::cerr << "error delete file: " << entry_name << std::endl;
                        }
                    }
                }
            }
        }

        closedir(dir);

        if (rmdir(name.c_str()) == 0) {
            std::cout << "Directory deleted: " << name << std::endl;
        } else {
            std::cerr << "Error deleting directory: " << name << std::endl;
        }
    } 
    else
     {
        //not direct , its a file 
        std::ifstream file(name.c_str());
        if (file.is_open()) 
        {
            file.close();
            if (std::remove(name.c_str()) == 0) {
                std::cout << "File deleted: " << name << std::endl;
            } else {
                std::cerr << "Error to delete: " << name << std::endl;
            }
        }
         else 
         {
            std::cerr << "Error open file  " << name << std::endl;
        }
    }
}

void Client::web_delete()
{
    std::string url = "/nfs/homes/zsaoud/Desktop/z/Srcs/" + reqURL;
    std::cout << url << "\n";
        remove_directory_file(url);
    flag_in_out = true;
}