#include "../Includes/httpResponse.hpp"
#include <sstream>

Response::Response(const std::string& status, const std::string& body):status(status),body(body) {}

std::string Response::toString() const 
{
    std::ostringstream oss;
    oss << "HTTP/1.1 " << status << "\r\n";
    oss << "Content-Length: " << body.length() << "\r\n";
    oss << "\r\n";
    oss << body;
    return oss.str();
}
