// #include <iostream>
// #include <unistd.h> // Pour usleep()

// int main() {
//     for (int i = 0; i < 3; ++i) {
//         std::cout << "." << std::flush; // Afficher un point sans saut de ligne
//         usleep(500000); // Attendre 500 microsecondes (500000 microsecondes = 500 millisecondes)
//     }
//     std::cout << std::endl; // Aller à la ligne après avoir affiché les trois points
//     return 0;
// }
// #include <iostream>
// #include <cstdint>
// #include <arpa/inet.h>

// int main() {
//     const char* ipAddress = "127.0.0.1";
//     uint32_t ipInteger;

//     // Convert the IP address from text to binary form
//     int result = inet_pton(AF_INET, ipAddress, &ipInteger);
//     if (result == 0) {
//         std::cerr << "Invalid IP address format" << std::endl;
//         return 1;
//     } else if (result == -1) {
//         std::cerr << "Failed to convert IP address" << std::endl;
//         return 1;
//     }

//     // Convert the binary IP address to network byte order
//     ipInteger = htonl(ipInteger);

//     std::cout << "IP address: " << ipAddress << std::endl;
//     std::cout << "Integer representation: " << ipInteger << std::endl;

//     return 0;
// }



#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <errno.h>
#include <map>

#define BUFFERSIZE 1024
#define MAXEVENTS 128

/* Simple single threaded server 
 * utilising epoll I/O event notification mechanism 
 *
 * compile: g++ epollServ.c -o _epoll
 * Usage: _epoll <port>
 * clients connect using telnet localhost <port>
**/

int serverSock_init (char *port)
{
  struct addrinfo hints, *res;
  int status, sfd;

  memset (&hints, 0, sizeof (struct addrinfo));
  hints.ai_family = AF_INET;  // IPV4   
  hints.ai_socktype = SOCK_STREAM; // TCP socket 
  hints.ai_flags = AI_PASSIVE;     // needed for serversocket

  status = getaddrinfo (NULL, port, &hints, &res);  // populates res addrinfo struct ready for socket call
  if (status != 0)
  {
    fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (status));
    return -1;
  }
     
  sfd = socket (res->ai_family, res->ai_socktype, res->ai_protocol); // create endpoint socketFD
  if (sfd == -1) {
    fprintf (stderr, "Socket error\n");
    close (sfd); 
    return -1;
  }

  int optval = 1;
  setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)); // set port reuse opt for debugging

  status = bind (sfd, res->ai_addr, res->ai_addrlen); // bind addr to sfd, addr in this case is INADDR_ANY 
  if (status == -1)
  {
    fprintf (stderr, "Could not bind\n");
    return -1;      
  }  

  freeaddrinfo (res);
  return sfd;
}

int main (int argc, char *argv[])
{
  int sfd, s, efd;
  struct epoll_event event;
  struct epoll_event *events;
  std::map<int,int> clientMap;

  if (argc != 2) {
      fprintf (stderr, "Usage: %s [port]\n", argv[0]);
      exit (EXIT_FAILURE);
  }

  sfd = serverSock_init (argv[1]); 
  if (sfd == -1)
    abort ();

  int flags = fcntl (sfd, F_GETFL, 0);  // change socket fd to be non-blocking
  flags |= O_NONBLOCK;
  fcntl (sfd, F_SETFL, flags);

  s = listen (sfd, SOMAXCONN);  // mark socket as passive socket type 
  if (s == -1)
    {
      perror ("listen");
      abort ();
    }

  efd = epoll_create1 (0);  // create epoll instance 
  if (efd == -1)
    {
      perror ("epoll_create");
      abort ();
    }

  event.data.fd = sfd;
  event.events = EPOLLIN | EPOLLET;  // just interested in read's events using edge triggered mode
  s = epoll_ctl (efd, EPOLL_CTL_ADD, sfd, &event); // Add server socket FD to epoll's watched list
  if (s == -1)
    {
      perror ("epoll_ctl");
      abort ();
    }

  /* Events buffer used by epoll_wait to list triggered events */
  events = (epoll_event*) calloc (MAXEVENTS, sizeof(event));  

  /* The event loop */
  while (1)
    {
      int n, i;

      n = epoll_wait (efd, events, MAXEVENTS, -1);  // Block until some events happen, no timeout
      for (i = 0; i < n; i++)
	{
	  
           /* Error handling */
           if ((events[i].events & EPOLLERR) ||
              (events[i].events & EPOLLHUP) ||
              (!(events[i].events & EPOLLIN)))
	    {
              /* An error has occured on this fd, or the socket is not
                 ready for reading (why were we notified then?) */
	      fprintf (stderr, "epoll error\n");
	      close (events[i].data.fd);  // Closing the fd removes from the epoll monitored list
              clientMap.erase(events[i].data.fd);
	      continue;
	    }

	    /* serverSocket accepting new connections */
            else if (sfd == events[i].data.fd)
	    {
              /* We have a notification on the listening socket, which
                 means one or more incoming connections. */
              while (1)
                {
                  struct sockaddr in_addr;
                  socklen_t in_len;
                  int infd;
                  char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

                  in_len = sizeof in_addr;
                  infd = accept (sfd, &in_addr, &in_len); // create new socket fd from pending listening socket queue
                  if (infd == -1) // error
                    {
                      if ((errno == EAGAIN) ||
                          (errno == EWOULDBLOCK))
                        {
                          /* We have processed all incoming connections. */
                          break;
                        }
                      else
                        {
                          perror ("accept");
                          break;
                        }
                    }

                  int optval = 1;
                  setsockopt(infd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));  // set socket for port reuse
                  
                  /* get the client's IP addr and port num */
                  s = getnameinfo (&in_addr, in_len,
                                   hbuf, sizeof hbuf,
                                   sbuf, sizeof sbuf,
                                   NI_NUMERICHOST | NI_NUMERICSERV);
                  if (s == 0)
                    {
                      printf("Accepted connection on descriptor %d "
                             "(host=%s, port=%s)\n", infd, hbuf, sbuf);
                    }

                  /* Make the incoming socket non-blocking and add it to the
                     list of fds to monitor. */        
                  int flags = fcntl (infd, F_GETFL, 0);
                  flags |= O_NONBLOCK;
                  fcntl (infd, F_SETFL, flags);

                  event.data.fd = infd;
                  event.events = EPOLLIN | EPOLLET;                  

                  s = epoll_ctl (efd, EPOLL_CTL_ADD, infd, &event); 
                  if (s == -1)
                    {
                      perror ("epoll_ctl");
                      abort ();
                    }
                clientMap[event.data.fd]=0;  // init msg counter    
                }
              continue;
            }
          else
            {
              /* We have data on the fd waiting to be read. Read and
                 display it. We must read whatever data is available
                 completely, as we are running in edge-triggered mode
                 and won't get a notification again for the same
                 data. */
              int done = 0;

              while (1)
                {
                  ssize_t count;
                  char buf[BUFFERSIZE];

                  count = read (events[i].data.fd, buf, sizeof buf);  
                  
                  if (count == -1)
                    {
                      /* If errno == EAGAIN, that means we have read all
                         data. So go back to the main loop. */
                      if (errno != EAGAIN)
                        {
                          perror ("read");
                          done = 1;
                        }
                      break;
                    }
                  else if (count == 0)
                    {
                      /* End of file. The remote has closed the
                         connection. */
                      done = 1;
                      break;
                    }
                  
                  buf[count]=0;
                  char wbuf[BUFFERSIZE];
                  int cx=snprintf(wbuf,BUFFERSIZE,"(fd:%d seq:%d) %s",events[i].data.fd, clientMap[events[i].data.fd],buf);

                  /* Write the buffer to standard output */
                  s = write (1, wbuf, cx);
                  if (s == -1)
                    {
                      perror ("write");
                      abort ();
                    }
                }
                // Increment msg counter
                int tmp = clientMap[events[i].data.fd];
                tmp++;
                clientMap[events[i].data.fd]=tmp;
                
              if (done)
                {
                  printf ("Closed connection on descriptor %d\n",
                          events[i].data.fd);

                  /* Closing the descriptor will make epoll remove it
                     from the set of descriptors which are monitored. */
                  close (events[i].data.fd);
                  clientMap.erase(events[i].data.fd);
                }
            }
        }
    }

  free (events);
  close (sfd);

  return EXIT_SUCCESS;
}

// #include <iostream>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netdb.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/epoll.h>
// #include <errno.h>
// #include <map>

// #define BUFFERSIZE 1024
// #define MAXEVENTS 128

// class Server {
// private:
//     int sockfd;
//     int epollfd;
//     std::map<int, int> clientMap;

// public:
//     Server() : sockfd(-1), epollfd(-1) {}

//     ~Server() {
//         if (sockfd != -1)
//             close(sockfd);
//         if (epollfd != -1)
//             close(epollfd);
//     }

//     bool init(const char *port) {
//         struct addrinfo hints, *res;
//         int status;

//         memset(&hints, 0, sizeof(struct addrinfo));
//         hints.ai_family = AF_INET;
//         hints.ai_socktype = SOCK_STREAM;
//         hints.ai_flags = AI_PASSIVE;

//         status = getaddrinfo(NULL, port, &hints, &res);
//         if (status != 0) {
//             fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
//             return false;
//         }

//         sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
//         if (sockfd == -1) {
//             fprintf(stderr, "Socket error\n");
//             return false;
//         }

//         int optval = 1;
//         setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

//         status = bind(sockfd, res->ai_addr, res->ai_addrlen);
//         if (status == -1) {
//             fprintf(stderr, "Could not bind\n");
//             return false;
//         }

//         freeaddrinfo(res);

//         int flags = fcntl(sockfd, F_GETFL, 0);
//         flags |= O_NONBLOCK;
//         fcntl(sockfd, F_SETFL, flags);

//         epollfd = epoll_create1(0);
//         if (epollfd == -1) {
//             perror("epoll_create");
//             return false;
//         }

//         struct epoll_event event;
//         event.data.fd = sockfd;
//         event.events = EPOLLIN | EPOLLET;

//         status = epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &event);
//         if (status == -1) {
//             perror("epoll_ctl");
//             return false;
//         }

//         return true;
//     }

//     void start() {
//         struct epoll_event events[MAXEVENTS];

//         while (true) {
//             int n = epoll_wait(epollfd, events, MAXEVENTS, -1);
//             for (int i = 0; i < n; ++i) {
//                 if (events[i].data.fd == sockfd) {
//                     handleNewConnection();
//                 } else {
//                     handleClient(events[i].data.fd);
//                 }
//             }
//         }
//     }

// private:
//     void handleNewConnection() {
//         while (true) {
//             struct sockaddr in_addr;
//             socklen_t in_len = sizeof in_addr;
//             int infd = accept(sockfd, &in_addr, &in_len);
//             if (infd == -1) {
//                 if (errno == EAGAIN || errno == EWOULDBLOCK) {
//                     break; // No more incoming connections
//                 } else {
//                     perror("accept");
//                     break;
//                 }
//             }

//             int optval = 1;
//             setsockopt(infd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

//             int flags = fcntl(infd, F_GETFL, 0);
//             flags |= O_NONBLOCK;
//             fcntl(infd, F_SETFL, flags);

//             struct epoll_event event;
//             event.data.fd = infd;
//             event.events = EPOLLIN | EPOLLET;

//             epoll_ctl(epollfd, EPOLL_CTL_ADD, infd, &event);
//             clientMap[infd] = 0;
//         }
//     }

//     void handleClient(int clientfd) {
//         char buf[BUFFERSIZE];
//         ssize_t count;

//         while (true) {
//             count = read(clientfd, buf, sizeof(buf));
//             if (count == -1) {
//                 if (errno != EAGAIN) {
//                     perror("read");
//                 }
//                 break;
//             } else if (count == 0) {
//                 std::cout << "Closed connection on descriptor " << clientfd << std::endl;
//                 close(clientfd);
//                 clientMap.erase(clientfd);
//                 break;
//             }

//             buf[count] = '\0';
//             std::cout << "(fd:" << clientfd << " seq:" << clientMap[clientfd] << ") " << buf;
//             clientMap[clientfd]++;
//         }
//     }
// };

// int main(int argc, char *argv[]) {
//     if (argc != 2) {
//         fprintf(stderr, "Usage: %s [port]\n", argv[0]);
//         exit(EXIT_FAILURE);
//     }

//     Server server;
//     if (!server.init(argv[1])) {
//         std::cerr << "Server initialization failed\n";
//         exit(EXIT_FAILURE);
//     }

//     server.start();

//     return EXIT_SUCCESS;
// }
// #include <iostream>
// #include <unistd.h>
// #include <sys/epoll.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <thread>
// #include <vector>

// constexpr int MAX_EVENTS = 10;
// constexpr int MAX_CLIENTS = 10;
// constexpr int PORT = 8080;

// // Function to handle client connections in a separate thread
// void handleClient(int clientFd) {
//     char buffer[1024];

//     while (true) {
//         int bytesRead = read(clientFd, buffer, sizeof(buffer));
//         if (bytesRead <= 0) {
//             break;
//         }
//         write(clientFd, buffer, bytesRead);
//     }

//     close(clientFd);
// }

// int main() {
//     int serverFd, epollFd;
//     struct sockaddr_in serverAddress;
//     struct epoll_event event, events[MAX_EVENTS];

//     // Create socket
//     serverFd = socket(AF_INET, SOCK_STREAM, 0);
//     if (serverFd == -1) {
//         std::cerr << "Failed to create socket." << std::endl;
//         return 1;
//     }

//     // Bind socket to address and port
//     serverAddress.sin_family = AF_INET;
//     serverAddress.sin_addr.s_addr = INADDR_ANY;
//     serverAddress.sin_port = htons(PORT);
//     if (bind(serverFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
//         std::cerr << "Failed to bind socket." << std::endl;
//         close(serverFd);
//         return 1;
//     }

//     // Listen for incoming connections
//     if (listen(serverFd, MAX_CLIENTS) == -1) {
//         std::cerr << "Failed to listen." << std::endl;
//         close(serverFd);
//         return 1;
//     }

//     // Create epoll instance
//     epollFd = epoll_create1(0);
//     if (epollFd == -1) {
//         std::cerr << "Failed to create epoll instance." << std::endl;
//         close(serverFd);
//         return 1;
//     }

//     // Add server socket to epoll
//     event.events = EPOLLIN;
//     event.data.fd = serverFd;
//     if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverFd, &event) == -1) {
//         std::cerr << "Failed to add server socket to epoll instance." << std::endl;
//         close(serverFd);
//         close(epollFd);
//         return 1;
//     }

//     std::cout << "Server started. Listening on port " << PORT << std::endl;

//     while (true) {
//         int numEvents = epoll_wait(epollFd, events, MAX_EVENTS, -1);
//         if (numEvents == -1) {
//             std::cerr << "Failed to wait for events." << std::endl;
//             break;
//         }

//         for (int i = 0; i < numEvents; ++i) {
//             if (events[i].data.fd == serverFd) {
//                 // Accept new client connection
//                 struct sockaddr_in clientAddress;
//                 socklen_t clientAddressLength = sizeof(clientAddress);
//                 int clientFd = accept(serverFd, (struct sockaddr*)&clientAddress, &clientAddressLength);
//                 if (clientFd == -1) {
//                     std::cerr << "Failed to accept client connection." << std::endl;
//                     continue;
//                 }

//                 // Add client socket to epoll
//                 event.events = EPOLLIN;
//                 event.data.fd = clientFd;
//                 if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientFd, &event) == -1) {
//                     std::cerr << "Failed to add client socket to epoll instance." << std::endl;
//                     close(clientFd);
//                     continue;
//                 }

//                 // Create a new thread to handle the client connection
//                 std::thread clientThread(handleClient, clientFd);
//                 clientThread.detach();
//             } else {
//                 // Handle client data
//                 int clientFd = events[i].data.fd;
//                 std::thread clientThread(handleClient, clientFd);
//                 clientThread.detach();
//             }
//         }
//     }

//     close(serverFd);
//     close(epollFd);
//     return 0;
// }

// #include <sys/epoll.h>
// #include <stdio.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/socket.h>
// #include <sys/select.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>

// void handler_events(int epfd,struct epoll_event revs[],int num,int listen_sock)
// {
//     struct epoll_event ev;
//     int i = 0;
//     for( ; i < num; i++ )
//     {
//     int fd = revs[i].data.fd;

//     // If it is listening file descriptor, call accept to accept the new connection

//     if( fd == listen_sock && (revs[i].events & EPOLLIN) )
//     {
//         struct sockaddr_in client;
//         socklen_t len = sizeof(client);
//         int new_sock = accept(fd,(struct sockaddr *)&client,&len);

//         if( new_sock < 0 )
//         {
//         perror("accept fail ... \n");
//         continue;
//         }

//        printf("get a new link![%s:%d]\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));

//        //Because it is just an http protocol: after the connection is successful, the following is the request and response 
//        // Before the server responds: read the content the client wants to request

//        ev.events = EPOLLIN;
//        ev.data.fd = new_sock;
//        epoll_ctl(epfd,EPOLL_CTL_ADD,new_sock,&ev);

//        continue;
//     }

//     // If it is an ordinary file descriptor, call read to provide a service to read data

//     if(revs[i].events & EPOLLIN)
//     {
//         char buf[10240];
//         ssize_t s = read(fd,buf,sizeof(buf)-1);
//         if( s > 0 )// read successfully
//         {
//         buf[s] = 0;
//         printf(" %s ",buf);

//         // After reading successfully, it is necessary to respond to the server
//         // And the events here are read-only events, so they need to be modified

//         ev.events = EPOLLOUT;// write events only
//         ev.data.fd = fd;
//         epoll_ctl(epfd,EPOLL_CTL_MOD,fd,&ev);// where EPOLL_CTL_MOD means modification

//         }

//         else if( s == 0 )
//         {
//         printf(" client quit...\n ");
//         close(fd);// fd here is revs[i].fd
//         epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);// Close the connection, then close the descriptor describing the connection
//         }
//         else// s = -1 failed
//         {   
//         printf("read fai ...\n");
//         close(fd);// fd here is revs[i].fd
//         epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);// Close the connection, then close the descriptor describing the connection
//         }
//         continue;
//     }

//     // The server responds to the client: write

//     if( revs[i].events & EPOLLOUT )
//     {
//         const char* echo = "HTTP/1.1 200 ok \r\n\r\n<html>hello epoll server!!!</html>\r\n";
//         write(fd,echo,strlen(echo));
//         close(fd);
//         epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
//     }
//     }
// }

// int startup( int port )
// {
//     // 1. Create a socket
//     int sock = socket(AF_INET,SOCK_STREAM,0);//The second parameter here means TCP
//     if( sock < 0 )
//     {
//     perror("socket fail...\n");
//     exit(2);
//     }

//     // 2. Solve the problem that the server cannot restart when TIME_WAIT; so that the server can be restarted immediately
//     int opt = 1;
//     setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

//     struct sockaddr_in local;
//     local.sin_family = AF_INET;
//     local.sin_addr.s_addr = htonl(INADDR_ANY);// The address is of any type
//     local.sin_port = htons(port);// The port number here can also be specified directly 8080
//     // 3. Bind the port number

//     if( bind(sock,(struct sockaddr *)&local,sizeof(local)) < 0 )
//     {
//     perror("bind fail...\n");
//     exit(3);
//     }

//     // 4. Obtain the listening socket
//     if( listen(sock,5) < 0 )
//     {
//     perror("listen fail...\n");
//     exit(4);
//     }
//     return sock;
// }

// int main(int argc,char* argv[] )
// {
//     if( argc != 2 )
//     {
//     printf("Usage:%s port\n ",argv[0]);
//     return 1;
//     }

//     // 1. Create an epoll model: return a file descriptor

//     int epfd = epoll_create(256);
//     if( epfd < 0 )
//     {
//     perror("epoll_create fail...\n");
//     return 2;
//     }

//     // 2. Obtain the listening socket

//    int listen_sock = startup(atoi(argv[1]));//The port number is passed in as a string when it is passed in, and it needs to be converted to an integer


//     // 3. Initialize the structure-list of structures to monitor

//     struct epoll_event  ev;
//     ev.events = EPOLLIN;//Concerned about reading events
//     ev.data.fd = listen_sock;// Descriptive file descriptor of interest

//     // 4. Event registration function of epoll --- add the read-only event of the file descriptor to be concerned

//     epoll_ctl(epfd,EPOLL_CTL_ADD,listen_sock,&ev);

//     struct epoll_event revs[128];
//     int n = sizeof(revs)/sizeof(revs[0]);

//     int timeout = 3000;
//     int num = 0;


//     while(1)
//     {

//        // 5. Start calling epoll and wait for the file descriptor set of interest to be ready

//        switch( num = epoll_wait(epfd,revs,n,timeout) )
//        {
//        case 0:// indicates that the timeout period has elapsed before the word state changes
//            printf("timeout...\n");
//            continue;
//        case -1:// Failed
//            printf("epoll_wait fail...\n");
//            continue;
//        default: // succeeded

//            handler_events(epfd,revs,num,listen_sock);
//            break;
//        }
//     }
//     close(epfd);
//     close(listen_sock);
//     return 0;
// }

// #include <sys/epoll.h>
// #include <iostream>
// #include <unistd.h>
// #include <string>
// #include <cstring>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>

// class EpollServer {
// private:
//     int epoll_fd;
//     int listen_sock;
//     int port;
//     static const int MAX_EVENTS = 128;
//     static const int TIMEOUT = 3000;

// public:
//     EpollServer(int port) : epoll_fd(-1), listen_sock(-1), port(port) {}

//     void run() {
//         setup();
//         eventLoop();
//     }

// private:
//     void setup() {
//         epoll_fd = epoll_create(256);
//         if (epoll_fd < 0) {
//             perror("epoll_create fail...\n");
//             exit(2);
//         }

//         listen_sock = startup(port);

//         struct epoll_event ev;
//         ev.events = EPOLLIN;
//         ev.data.fd = listen_sock;

//         epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_sock, &ev);
//     }

//     int startup(int port) {
//         int sock = socket(AF_INET, SOCK_STREAM, 0);
//         if (sock < 0) {
//             perror("socket fail...\n");
//             exit(2);
//         }

//         int opt = 1;
//         setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

//         struct sockaddr_in local;
//         local.sin_family = AF_INET;
//         local.sin_addr.s_addr = htonl(INADDR_ANY);
//         local.sin_port = htons(port);

//         if (bind(sock, (struct sockaddr *)&local, sizeof(local)) < 0) {
//             perror("bind fail...\n");
//             exit(3);
//         }

//         if (listen(sock, 5) < 0) {
//             perror("listen fail...\n");
//             exit(4);
//         }

//         return sock;
//     }

//     void eventLoop() {
//         struct epoll_event revs[MAX_EVENTS];

//         while (true) {
//             int num = epoll_wait(epoll_fd, revs, MAX_EVENTS, TIMEOUT);
//             if (num == 0) {
//                 std::cout << "timeout..." << std::endl;
//                 continue;
//             } else if (num == -1) {
//                 perror("epoll_wait fail...\n");
//                 continue;
//             }

//             handlerEvents(revs, num);
//         }
//     }

//     void handlerEvents(struct epoll_event revs[], int num) {
//         struct epoll_event ev;

//         for (int i = 0; i < num; i++) {
//             int fd = revs[i].data.fd;

//             if (fd == listen_sock && (revs[i].events & EPOLLIN)) {
//                 acceptConnection();
//                 continue;
//             }

//             if (revs[i].events & EPOLLIN) {
//                 readData(fd, ev);
//                 continue;
//             }

//             if (revs[i].events & EPOLLOUT) {
//                 sendResponse(fd);
//             }
//         }
//     }

//     void acceptConnection() {
//         struct sockaddr_in client;
//         socklen_t len = sizeof(client);
//         int new_sock = accept(listen_sock, (struct sockaddr *)&client, &len);

//         if (new_sock < 0) {
//             perror("accept fail ... \n");
//             return;
//         }

//         std::cout << "get a new link! [" << inet_ntoa(client.sin_addr) << ":" << ntohs(client.sin_port) << "]" << std::endl;

//         struct epoll_event ev;
//         ev.events = EPOLLIN;
//         ev.data.fd = new_sock;
//         epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_sock, &ev);
//     }

//     void readData(int fd, struct epoll_event& ev) {
//         char buf[10240];
//         ssize_t s = read(fd, buf, sizeof(buf) - 1);

//         if (s > 0) {
//             buf[s] = '\0';
//             std::cout << buf << std::endl;

//             ev.events = EPOLLOUT;
//             ev.data.fd = fd;
//             epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &ev);
//         } else if (s == 0) {
//             std::cout << "client quit..." << std::endl;
//             close(fd);
//             epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
//         } else {
//             perror("read fail ...\n");
//             close(fd);
//             epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
//         }
//     }

//     void sendResponse(int fd) {
//         const char *echo = "HTTP/1.1 200 OK\r\n\r\n<html>hello epoll server!!!</html>\r\n";
//         write(fd, echo, strlen(echo));
//         close(fd);
//         epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
//     }
// };

// int main(int argc, char *argv[]) {
//     if (argc != 2) {
//         std::cout << "Usage: " << argv[0] << " port" << std::endl;
//         return 1;
//     }

//     int port = std::stoi(argv[1]);
//     EpollServer server(port);
//     server.run();

//     return 0;
// }




// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netdb.h>
// #include <cstring>
// #include <iostream>

// class EpollServer {
// private:
//     int epoll_fd;
//     int listen_sock;
//     int port;
//     std::string ip_address;
//     static const int MAX_EVENTS = 128;
//     static const int TIMEOUT = 3000;

// public:
//     EpollServer(int port, const std::string& ip_address = "") : epoll_fd(-1), listen_sock(-1), port(port), ip_address(ip_address) {}

//     void run() {
//         setup();
//         eventLoop();
//     }

// private:
//     void setup() {
//         epoll_fd = epoll_create(256);
//         if (epoll_fd < 0) {
//             perror("epoll_create fail...\n");
//             exit(2);
//         }

//         listen_sock = startup(port, ip_address);

//         struct epoll_event ev;
//         ev.events = EPOLLIN;
//         ev.data.fd = listen_sock;

//         epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_sock, &ev);
//     }

//     int startup(int port, const std::string& ip_address) {
//         struct addrinfo hints, *res, *p;
//         int sockfd;

//         memset(&hints, 0, sizeof hints);
//         hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
//         hints.ai_socktype = SOCK_STREAM; // TCP
//         hints.ai_flags = AI_PASSIVE; // Use my IP

//         char port_str[10];
//         snprintf(port_str, sizeof(port_str), "%d", port);

//         int status = getaddrinfo(ip_address.empty() ? NULL : ip_address.c_str(), port_str, &hints, &res);
//         if (status != 0) {
//             std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
//             exit(1);
//         }

//         for (p = res; p != nullptr; p = p->ai_next) {
//             sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
//             if (sockfd == -1) {
//                 continue;
//             }

//             if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
//                 close(sockfd);
//                 continue;
//             }

//             break;
//         }

//         if (p == nullptr) {
//             std::cerr << "Failed to bind socket" << std::endl;
//             exit(2);
//         }

//         freeaddrinfo(res);

//         if (listen(sockfd, 5) == -1) {
//             perror("listen");
//             exit(3);
//         }

//         return sockfd;
//     }

    // Rest of the class methods...
//};
