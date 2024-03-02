#include "../Includes/Client.hpp"

int Client::matching_servers() {
    std::cout << path<<"*\n";
    //std::vector<Location> vecl = servers[sindex].getLocations();
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

// std::string check_block(std::string s)
// {
//     std::string tmp;
//     std::istringstream iss(s);
//     if (s == "" || s == "/")
//         return "";
//     std::getline(iss, tmp, '/');
//     return tmp;
// }
// int Client::matching_servers()
// {
//     std::vector<Location> vecl = servers[sindex].getLocations();
//     int matched = 0;
//     std::string block;
//     std::string sub;
//     if (path == "/")
//     {
//         block = "/";
//         sub = "";
//     }
//     else 
//     {
//         block = "/" + check_block(path.substr(1));
//         sub = path.substr(block.size());
//     }
//     std::string c;
//     bool rl = 0;
//     int index = 0;
//     std::string root;

//     while (!matched)
//     {
//         for (size_t j = 0; j < vecl.size(); j++)
//         {
//             std::string p = vecl[j].getLocationPath();
//             if (p == "/")
//             {
//                 rl = 1;
//                 index = j;
//             }
//             if(mattching(block, p))
//             {
//                 root = vecl[j].getRoot();
//                 if (root[root.size()-1] == '/')
//                     reqURL = root + path.substr(block.size()+1);
//                 else
//                     reqURL = root + path.substr(block.size());
//                 matched = 1;
//                 return 1;
//             }
//         }
//         c = check_block(sub.substr(1));
//         if (c == "")
//         {
//             if (rl)
//             {
//                 root = vecl[index].getRoot();
//                 if (root[root.size()-1] == '/' && path != "/")
//                     reqURL = root + path.substr(1);
//                 else if (path == "/")
//                     reqURL = root;
//                 else
//                     reqURL = root + path;
//                 matched = 1;
//                 return 1;
//             }
//             else
//                 return 0;
//         }
//         block = block + "/" + c;
//         sub = sub.substr(c.size());

//     }
//     return 0;
// }



/*bool isLocationMatch(std::string location, std::string url)
{
    if(location[location.size()-1] != '/')
        location.push_back('/');
    if(url[url.size()-1] != '/')
        url.push_back('/');
    if(url.find(location) != std::string::npos)
        return 1;
    return 0;
}

void replaceHomeWithPath(std::string &url, std::string homeSubstring, std::string root)
{

    size_t pos = url.find(homeSubstring);
    std::cout<<homeSubstring<<"\n";
    if (pos != std::string::npos)
        url.replace(pos, homeSubstring.length(),root);
}

int Client::matching_servers()
{
    // int slashl = 0;
    // int index = 0;
    std::vector<Location> vecl = servers[sindex].getLocations();
    for (size_t j = 0; j < vecl.size(); j++)
    {
        // if (vecl[j].getLocationPath() == "/" && slashl == 0)
        // {
        //     slashl = 1;
        //     index = j;
        // }
        if(isLocationMatch(vecl[j].getLocationPath(), path))
        {
            std::string root = vecl[j].getRoot();
            if(root[root.size()-1] == '/')
               root.erase(root.size()-1, 1);
            replaceHomeWithPath(path,vecl[j].getLocationPath(),root);
            std::cout<<path<<"      *********************\n";
            return 1;
        }
    }
    // if (slashl)
    // {
    //     if(isLocationMatch(vecl[index].getLocationPath(), path))
    //     {
    //         std::string root = vecl[index].getRoot();
    //         if(root[root.size()-1] == '/')
    //            root.erase(root.size()-1, 1);
    //         replaceHomeWithPath(path,vecl[index].getLocationPath(),root);
    //         std::cout<<path<<"      *********************\n";
    //         return 1;
    //     }
    // }
    return 0;
}*/
