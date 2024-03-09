#include "../Includes/Client.hpp"
#include<signal.h>

void Client::parse_header(std::string headersCgi)
{
    std::istringstream iss(headersCgi);
    std::string line;
    while (std::getline(iss, line)) {
        if (line.substr(0, 11) == "Set-Cookie:") {
            size_t pos = line.find("=");
            std::string name = line.substr(12, pos - 12);
            size_t end_pos = line.find("; expires=");
            std::string value = line.substr(pos + 1, end_pos - pos - 1);
            cookies.push_back(std::make_pair(name, value));
        }else if (line.substr(0, 13) == "Content-type:") {
            content_type = line.substr(14, line.rfind(";")-14);
        }
    }
}

void    Client::sendGet()
{
    signal(SIGPIPE,SIG_IGN);

    if (listing)
    {
        listDir();}
    else
    {
        if (cgiflag)
        {
            isCgi = true;
            a_file.open((getUrl.substr(0,getUrl.find_last_of('/')+1 )+ "result.txt").c_str(), std::ios::in | std::ios::binary);
            cgiflag = false;
        }
        if (Opened)
        {
            size_t t = getUrl.rfind(".");
            std::string typa;
            if(t != std::string::npos)
            {
                typa = getUrl.substr(t);
            }
            store_type();
            std::string Content_typa; 
            std::map<std::string, std::string>::iterator it;
            for(it = mime_type.begin() ; it != mime_type.end(); ++it)
            {
                std::string first = it->first;
                std::string second = it->second;
                if(second == typa )
                {
                    Content_typa = first;
                    break;
                }
            }
            char buffer[1024] = {0};
            a_file.read(buffer, sizeof(buffer) - 1);
            std::string s(buffer, sizeof(buffer));
            if (isCgi && s.find("\r\n\r\n") != std::string::npos)
            {
                headersCgi = s.substr(0, s.find("\r\n\r\n"));
                s = s.substr(s.find("\r\n\r\n")+4);
                parse_header(headersCgi);
                Content_typa = content_type;
            }
            std::string response = "";
            if (!flagResponse)
            {
                response = "HTTP/1.1 200 OK\r\n"
                                "Content-Type: " + Content_typa + "\r\n\r\n";
                flagResponse = true;
                s = response + s;
            }
            int a = write(sockfd, s.c_str(), a_file.gcount() + response.size());
            if(a <= 0)
            {
                perror("er2: ");
            }
            if (a_file.eof() || a_file.fail() || a_file.gcount() == 0)
            {
                a_file.close();
                close(sockfd);
                closed = true;
            }
        }
        else
        {
            close(sockfd);
            closed = true;
        }
    }
}

void Client::sendPost()
{
    //add body
    std::string response;
    response = "HTTP/1.1 201 Created\r\nContent-Type: text/html\r\n\r\n";
    write(sockfd, response.c_str(), response.size());
    close(sockfd);
    closed = true;
}

void Client::sendDelete()
{
    std::string response;
    response = "HTTP/1.1 204\r\nContent-Type: text/html\r\n\r\n";
    write(sockfd, response.c_str(), response.size());
    close(sockfd);
    closed = true;
}

void    Client::readDefault()
{
    std::cout<<"oooooooooo\n";
    std::string responseBody = "<html><head><style>body { display: flex; justify-content: center; align-items: center; height: 100vh; }</style></head><body><div><h1>" + message + "</h1></div></body></html>";
    std::string response = "HTTP/1.1 " + message + "\r\nContent-Type: text/html\r\n\r\n" + responseBody;
    write(sockfd, response.c_str(), response.size());
    close(sockfd);
    closed = true;
}

void    Client::readPage()
{
    char buffer[1024] = {0};
    a_file.read(buffer, sizeof(buffer) - 1);
    std::string s(buffer, sizeof(buffer));
    std::string response="";
    if (!flagResponse)
    {
        response = "HTTP/1.1 " + message + "\r\n"
                        "Content-Type: text/html\r\n\r\n";
        flagResponse = true;
        s = response + s;
    }
    write(sockfd, s.c_str(), a_file.gcount()+response.size());
    if (a_file.eof() || a_file.fail() || a_file.gcount() == 0)
    {
    std::cout<<buffer<<"==\n";
        a_file.close();
        close(sockfd);
        closed = true;
    }
}

void    Client::ft_open(std::string opath)
{
    a_file.open(opath.c_str(), std::ios::in | std::ios::binary);
    if (a_file.is_open())
        readPage();
    else
        readDefault();
}

void Client::ft_read(int error_code, std::string ppppath)
{
    Location loc = servers[sindex].getLocations()[lindex];
    mapErr er = servers[sindex].getLocations()[lindex].getErrorPages();

    if (loc.isErrorPagesSet())
    {
        mapErr::iterator it = er.find(error_code);
        if (it != er.end())
        {
            ft_open(er[error_code]);
        }
        else
            ft_open(ppppath);
    }
    else
        ft_open(ppppath);
}

void Client::send_client()
{
    if (status == 200)
        sendGet();
    else if (status == 201)
        sendPost();
    else if (status == 204)
        sendDelete();
    else if (status == 301)
    {
        std::string newLocation = "Location: " + servers[sindex].getLocations()[lindex].getRedirUrl() + "\r\n";
        std::string response = "HTTP/1.1 301 Moved Permanently\r\n" + newLocation + "Content-Length: 0\r\n\r\n";
        write(sockfd, response.c_str(), response.size());
        close(sockfd);
    }
    else if (status == 400)
        ft_read(400, "./error_pages/400.html");
    else if (status == 403)
        ft_read(403, "./error_pages/403.html");
    else if (status == 404)
        ft_read(404, "./error_pages/404.html");
    else if (status == 405)
         ft_read(405, "./error_pages/405.html");
    else if (status == 413)
        ft_read(413, "./error_pages/413.html");
    else if (status == 414)
        ft_read(414, "./error_pages/414.html");
    else if (status == 500)
        ft_read(500, "./error_pages/500.html");
    else if (status == 501)
        ft_read(501, "./error_pages/501.html");
    else if (status == 502)
        ft_read(502, "./error_pages/502.html");
    else if (status == 504)
        ft_read(504, "./error_pages/504.html");
}