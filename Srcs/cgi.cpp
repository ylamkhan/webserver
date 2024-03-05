#include "../Includes/Client.hpp"

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>

void Client::cgi(std::string u)
{
    std::string a = "CONTENT_LENGTH=" + headers["Content-Length"];
    std::string b = "CONTENT_TYPE=" + headers["Content-Type"];
    std::string c = "PATH_TRANSLATED=" + u;
    std::string d = "REQUEST_METHOD=" + method;
    std::string e = "QUERY_STRING=" + query;
    std::string f = "REDIRECT_STATUS=CGI";
    std::string g = "HTTP_COOKIE=" + headers["Cookie"];
    std::string h = "PATH_INFO=" + path;
    std::cout<<path<<"+++++++++++++\n";

    char *env[]= {
        (char *)a.c_str(),
        (char *)b.c_str(),
        (char *)c.c_str(),
        (char *)d.c_str(),
        (char *)e.c_str(),
        (char *)f.c_str(),
        (char *)g.c_str(),
        (char *)h.c_str(),
        NULL
    };

    int fd[2];
    if (pipe(fd) < 0)
    {
        std::cerr << "Failed to create pipe." << std::endl;
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        std::cerr << "Failed to fork a child process." << std::endl;
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        int fd1 = open(url.c_str(), O_RDONLY);
        if (fd1 < 0)
        {
            std::cerr << "Failed to open file for reading." << std::endl;
            exit(EXIT_FAILURE);
        }
        int fd_result = open((u.substr(u.find_last_of('/')) + "result.txt").c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (fd_result < 0)
        {
            std::cerr << "Failed to open result file for writing." << std::endl;
            exit(EXIT_FAILURE);
        }

        if (dup2(fd_result, STDOUT_FILENO) == -1)
        {
            std::cerr << "Failed to redirect stdout to result file." << std::endl;
            exit(EXIT_FAILURE);
        }
        close(fd_result);

        char *arg[] = {(char *)executable.c_str(), (char *)u.c_str(), NULL};
        if (execve(executable.c_str(), arg, env) < 0)
        {
            std::cerr << "Failed to execute script." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        wait(NULL); 
    }
}


