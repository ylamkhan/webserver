#include "../Includes/Socket.hpp"
#include <arpa/inet.h>
#include <iostream>

Socket::Socket(const std::vector<Server>& servers):servers(servers)
{
    
    epollfd = epoll_create(1);
    if (epollfd == -1) {
        throw std::runtime_error("Failed to create epoll instance");
    }
    for (std::vector<Server>::const_iterator it = servers.begin(); it != servers.end(); ++it) {
        const Server& server = *it;
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            throw std::runtime_error("Failed to create socket");
        }
        int opt = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
            close(sockfd);
            throw std::runtime_error("Failed to set socket options");
        }
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(server.getPort());
        if (inet_pton(AF_INET, server.getHost().c_str(), &addr.sin_addr) != 1) {
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
    }
  
                    

}

Socket::~Socket() {}

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

            if (std::find(serverSockets.begin(), serverSockets.end(), sockfd) != serverSockets.end())
            {

                int clientfd = accept(sockfd, reinterpret_cast<sockaddr*>(&clientAddr), &clientLen);
                Client c(servers);
                c.setFd(clientfd);
                setMapClient(clientfd, c);
                if (clientfd == -1)
                    throw std::runtime_error("Failed to accept connection");
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
                    ssize_t   bytesRead = recv(events[i].data.fd, buffer, 1023,0);
                
                    if (bytesRead == 0 || bytesRead == -1)
                    {
                        std::cerr << "Error reading from client\n";
                        close(sockfd);
                        epoll_ctl(epollfd, EPOLL_CTL_DEL, sockfd, static_cast<epoll_event*>(0));
                        mapClient.erase(sockfd);
                    }
                    std::string requestStr(buffer, bytesRead);
                    mapClient[sockfd].set_request_client(requestStr);
                }
                else if( events[i].data.fd  & EPOLLOUT && mapClient[events[i].data.fd].get_flag_in_out())
                {
                    const char* response =
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/html\r\n"
                        "\r\n"
                        "<html><body><h1>Hello, World!</h1></body></html>\r\n";
                    send(events[i].data.fd , response, strlen(response), 0);
                    close(sockfd);
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
