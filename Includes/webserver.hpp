#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <iostream>
#include <string>



#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>

#include "../Includes/Socket.hpp"
#include "../Includes/Server_Data.hpp"
#include "../Includes/Client.hpp"
#include "../Includes/httpResponse.hpp"
#include "../Includes/ConfigParser.hpp"
#include "../Includes/Location.hpp"

// typedef std::vector<std::string>					vecStr;
// typedef std::map<std::string, std::string>			mapStr;
// typedef std::map<int, std::string>					mapErr;
// typedef	std::vector<int>							vecInt;
// typedef	std::map<int, int>							mapSock;
// typedef std::vector< std::pair<std::string, int> >	vecFiles;
// typedef std::pair<std::string, std::string> 		pairStr;
// typedef std::vector<std::pair<std::string, std::string> > vecSPair;
// typedef struct dirent fileInfo;
// typedef DIR	directory;
// class Webserver
// {
//     public:
//         std::vector<ServerData> servers;
//         std::vector<Server *> boxServers;
//         Webserver();
//         ~Webserver();
//         void startWork();
// };
#endif