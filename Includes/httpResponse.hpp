#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <string>

class Response {
    private:
        std::string status;
        std::string body;

    public:
        Response(const std::string& status, const std::string& body);
        std::string toString() const;
};

#endif
