#include "../Includes/Client.hpp"

int Client::matching_servers()
{
    std::string tmp = path;
    int matched = 0;
    
    if (tmp[tmp.size()-1] == '/')
        tmp.erase(tmp.size()-1);
    if (path.empty())
        tmp = "/";
    while (!matched)
    {
        
        for (size_t j = 0; j < servers[sindex].getLocations().size(); j++)
        {
            std::string p = servers[sindex].getLocations()[j].getLocationPath();
            if (p == tmp)
            {
                std::string root = servers[sindex].getLocations()[j].getRoot();
                if (root[root.size()-1] == '/')
                    root.erase(root.size()-1);
                if (tmp == "/")
                    tmp = "";
                reqURL = root + path.substr(tmp.size());
                matched = 1;
                lindex = j;
                return 1;
            }
            if (tmp == "/" && j == servers[sindex].getLocations().size() - 1)
                matched = 1;
        }
        tmp = tmp.substr(0, tmp.find_last_of("/"));
        if (tmp == "")
            tmp = "/";
    }
    return 0;
}

