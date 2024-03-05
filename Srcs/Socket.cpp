#include "../Includes/Socket.hpp"
#include <arpa/inet.h>
#include <iostream>
#include<signal.h>
Socket::Socket(const std::vector<Server>& servers):servers(servers)
{
    
    epollfd = epoll_create(1);
    if (epollfd == -1) {
        throw std::runtime_error("Failed to create epoll instance");
    }
    for (size_t i = 0; i < servers.size(); i++) {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            throw std::runtime_error("Failed to create socket");
        }
        int opt = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1 || setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
            close(sockfd);
            throw std::runtime_error("Failed to set socket options");
        }
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(servers[i].getPort());
        if (inet_pton(AF_INET, servers[i].getHost().c_str(), &addr.sin_addr) != 1) {
            close(sockfd);
            throw std::runtime_error("Invalid host or server name");
        }
        if (bind(sockfd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
            close(sockfd);
            throw std::runtime_error("Failed to bind socket");
        }
        if (listen(sockfd, SOMAXCONN) == -1) {
            close(sockfd);
            throw std::runtime_error("Failed to listen on socket");
        }
        event.events = EPOLLIN;
        event.data.fd = sockfd;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &event) == -1) {
            close(sockfd);
            throw std::runtime_error("Failed to add sockfd to epoll");
        }
        serverSockets.push_back(sockfd);
        mapServers[sockfd] = i;
    }
  
                    

}

Socket::~Socket() {}

void makeSocketNonBlocking(int sockfd)
{
	int flags = fcntl(sockfd, F_GETFL, 0);
	if (flags == -1)
	{
		perror("fcntl");
		return;
	}
	if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1)
		perror("fcntl");
}

void Socket::handleConnections()
{
    sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    while (true)
    {
        int numEvents = epoll_wait(epollfd, events, maxEvents, -1);
        if (numEvents == -1) {
            throw std::runtime_error("Error in epoll_wait");
        }

        for (int i = 0; i < numEvents; ++i)
        {
            int sockfd = events[i].data.fd;
            // if ((events[i].events & EPOLLERR)   || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN)))
            // {
            //     std::cerr<<"epoll error\n";
            //     close(events[i].data.fd);
            // }
            // if (numEvents == -1)
            // {
            //     perror("epoll_wait()");
            //     exit(1);
            // }
            if (std::find(serverSockets.begin(), serverSockets.end(), sockfd) != serverSockets.end())
            {
                int clientfd = accept(sockfd, reinterpret_cast<sockaddr*>(&clientAddr), &clientLen);
                if (clientfd == -1)
                    throw std::runtime_error("Failed to accept connection");
                //makeSocketNonBlocking(clientfd);
               if( mapClient.find(clientfd) != mapClient.end())
                    mapClient.erase(clientfd);
                //find() earase fdclient;
                Client c(servers);
                c.setFd(clientfd);
                c.setSindex(mapServers[sockfd]);
                setMapClient(clientfd, c);
                // int flags;
                // flags = fcntl (clientfd, F_GETFL, 0);
                // if(flags == -1)
                //      throw std::runtime_error("Failed to fcntl");
                // flags |= O_NONBLOCK;
                // flags = fcntl(clientfd, F_SETFL, flags);
                // if(flags == -1)
                //      throw std::runtime_error("Failed to fcntl");
                event.events = EPOLLIN | EPOLLOUT;
                event.data.fd = clientfd;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &event) == -1)
                {
                    close(clientfd);
                    throw std::runtime_error("Failed to add clientfd to epoll");
                }
            }
            else
            {
                if(events[i].data.fd & EPOLLIN && !mapClient[events[i].data.fd].get_flag_in_out())
                {
                   
                    char buffer[1024] = {0};
                    // std::cout << "-----------------------------\n";
                    ssize_t   bytesRead = read(events[i].data.fd, buffer, 1023);
                    
                    // std::cout<<events[i].data.fd<<"         :"<<buffer<<"\n";
                    if (bytesRead == 0 || bytesRead == -1)
                    {
                        std::cerr << "Error reading from client\n";
                        close(events[i].data.fd);
                        epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, static_cast<epoll_event*>(0));
                        mapClient.erase(events[i].data.fd);
                    }
                    std::string requestStr(buffer, bytesRead);
                    mapClient[events[i].data.fd].set_request_client(requestStr);
                    
                }
                else if( events[i].data.fd  & EPOLLOUT && mapClient[events[i].data.fd].get_flag_in_out())
                {
                    std::cout << "*****************************************\n";
                    std::cout << "=====================================================\n";
                    mapClient[events[i].data.fd].send_client();
                    //hna khdmna b var sockfd hit ila closina lfd raymchi mn events
                    if (mapClient[sockfd].getClosed())
                        mapClient.erase(sockfd);
                }
                    
            }
        }
    }
}

Socket &Socket::operator=(Socket const &other)
{
    events[maxEvents] = other.events[maxEvents];
    epollfd = other.epollfd;
    event = other.event;
    serverSockets = other.serverSockets;
    mapClient = other.mapClient;
    return *this;
}

Socket::Socket(Socket const &other)
{
    *this = other;
}

void    Socket::setMapClient(int fd, Client &c) {
    mapClient[fd] = c;
}

std::map<int, Client> Socket::getMapClient() const {
    return mapClient;
}




