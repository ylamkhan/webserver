#include "../Includes/Client.hpp"
#include <sstream>

void Client::Response() 
{
    std::stringstream ss;
    ss << status;
    std::string statusString = ss.str();
    ss.str("");
    ss << message.length();
    std::string messageLength = ss.str();
    std::string response = "HTTP/1.1 " + statusString + "\r\nContent-Type: text/html\r\n\r\n<html><body><h1>" + message + "</h1></body></html>\r\n";
    send(sockfd, response.c_str(), response.length(), 0);
}
