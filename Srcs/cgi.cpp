#include "../Includes/Client.hpp"

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>

// void Client::setCGIEnvironment() {
//     int index = 0;
//     envp[index++] = strdup("SERVER_PROTOCOL=HTTP/1.1");
//     envp[index++] = strdup("SERVER_SOFTWARE=MyWebServer/1.0"); 
//     envp[index++] = strdup(method); 
//     envp[index++] = strdup("QUERY_STRING=");      
//     envp[index++] = strdup(headers["Content-Length"]);    
//     envp[index++] = strdup(headers["Content-Type"]);  
//     envp[index++] = strdup(url);
//     envp[index++] = strdup(host);  
//     envp[index++] = strdup(path);
//     envp[index++] = strdup(servers[sindex].getServerNames());
//     envp[index++] = strdup("HTTP_COOKIE=sessionid=abc123");
//     envp[index] = NULL;
// }

void Client::cgi()
{
    // char* envp[50];
    // setCGIEnvironment(envp, sockfd);

    // int fd[2];
    // if (pipe(fd) < 0)
    // {
    //     std::cerr << "Failed to create pipe." << std::endl;
    //     exit(EXIT_FAILURE);
    // }

    // pid_t pid = fork();
    // if (pid < 0)
    // {
    //     std::cerr << "Failed to fork a child process." << std::endl;
    //     exit(EXIT_FAILURE);
    // }
    // else if (pid == 0)
    // {
    //     // close(fd[0]);

    //     // if (dup2(fd[1], STDOUT_FILENO) == -1)
    //     // {
    //     //     std::cerr << "Failed to redirect stdout to pipe." << std::endl;
    //     //     exit(EXIT_FAILURE);
    //     // }
    //     // close(fd[1]);

    //     int fd1 = open(url.c_str(), O_RDONLY);
    //     if (fd1 < 0)
    //     {
    //         std::cerr << "Failed to open file for reading." << std::endl;
    //         exit(EXIT_FAILURE);
    //     }

    //     // if (dup2(fd1, STDIN_FILENO) == -1)
    //     // {
    //     //     std::cerr << "Failed to redirect stdin to file." << std::endl;
    //     //     exit(EXIT_FAILURE);
    //     // }
    //     // close(fd1);

    //     int fd_result = open("result.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    //     if (fd_result < 0)
    //     {
    //         std::cerr << "Failed to open result file for writing." << std::endl;
    //         exit(EXIT_FAILURE);
    //     }

    //     if (dup2(fd_result, STDOUT_FILENO) == -1)
    //     {
    //         std::cerr << "Failed to redirect stdout to result file." << std::endl;
    //         exit(EXIT_FAILURE);
    //     }
    //     close(fd_result);

    //     if (execve("/usr/bin/php", const_cast<char* const*>("/usr/bin/php"), NULL) < 0)
    //     {
    //         std::cerr << "Failed to execute script." << std::endl;
    //         exit(EXIT_FAILURE);
    //     }
    // }
    // else
    // {
    //     close(fd[1]);
    //     waitpid(); 
    // }
}


