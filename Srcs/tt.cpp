// #include <iostream>
// #include <vector>
// #include <string>
// #include <map>
// #include <algorithm>

// // Define a structure for Location block
// struct Location {
//     std::string path;
//     // Other properties...
// };

// // Define a structure for Server block
// class Server {
// private:
//     std::string server_names;
//     int port;
//     std::vector<Location> locations;

// public:
//     // Constructor
//     Server(const std::string& names, int port) : server_names(names), port(port) {}

//     // Method to add a location to the server
//     void addLocation(const Location& loc) {
//         locations.push_back(loc);
//     }

//     // Method to perform matching with the request
//     void matchRequest(const std::string& host, int requestPort, const std::string& requestUri) const {
//         // Check if the host matches the server names
//         std::vector<std::string> names;
//         names.push_back(server_names);
//         std::replace(names.begin(), names.end(), ' ', ','); // Convert space-separated names to comma-separated

//         // Split server names into a vector
//         std::vector<std::string> serverNamesVec;
//         std::string delimiter = ",";
//         size_t pos = 0;
//         std::string token;
//         while ((pos = names.find(delimiter)) != std::string::npos) {
//             token = names.substr(0, pos);
//             serverNamesVec.push_back(token);
//             names.erase(0, pos + delimiter.length());
//         }

//         // Match server name
//         bool serverNameMatched = std::find(serverNamesVec.begin(), serverNamesVec.end(), host) != serverNamesVec.end();

//         // Match port
//         bool portMatched = port == requestPort;

//         // Match URI path
//         for (const auto& location : locations) {
//             if (requestUri.find(location.path) != std::string::npos) {
//                 // Match found, handle accordingly
//                 std::cout << "Match found in server " << server_names << " and location " << location.path << std::endl;
//                 return;
//             }
//         }

//         // No match found
//         std::cout << "No match found for the request." << std::endl;
//     }
// };

// int main() {
//     // Example usage
//     Server server1("youssef", 8080);
//     server1.addLocation({"path1"});
//     server1.addLocation({"path2"});

//     Server server2("zineb", 8081);
//     server2.addLocation({"path3"});
//     server2.addLocation({"path4"});

//     // Perform matching with the request
//     std::string host = "zineb";
//     int port = 8081;
//     std::string requestUri = "/some/path";
//     server2.matchRequest(host, port, requestUri);

//     return 0;
// }


// #include <iostream>
// #include <string>
// #include <vector>

// class HTTPRequest {
// private:
//     std::string name_path;
//     std::string requesta;
//     std::string method;
//     std::string path;
//     std::string httpVersion;
//     std::vector<std::pair<std::string, std::string>> headers;
//     std::string body;

// public:
//     // Constructor and other member functions

//     // Method to calculate and return the Content-Length header
//     std::string calculateContentLength() const {
//         // Calculate the length of the request body
//         size_t bodyLength = body.length();

//         // Convert body length to string
//         std::string contentLengthHeader = "Content-Length: " + std::to_string(bodyLength);

//         return contentLengthHeader;
//     }
// };

// int main() {
//     // Create an instance of HTTPRequest and set its properties
//     HTTPRequest request;
//     request.body = "This is the body of the request.";

//     // Calculate and display the Content-Length header
//     std::string contentLengthHeader = request.calculateContentLength();
//     std::cout << contentLengthHeader << std::endl;

//     return 0;
// }















#include <iostream>
#include <string>
#include <map>
#include <sstream>

class HTTPRequest {
private:
    std::string name_path;
    std::string requesta;
    std::string method;
    std::string path;
    std::string httpVersion;
    std::map<std::string, std::string> headers; // Change to map
    std::string body;

public:
    // Constructor to parse the request string
    HTTPRequest(const std::string& requestString) {
        // Split the request string into lines
        std::istringstream iss(requestString);
        std::string line;

        // Parse the request line
        if (std::getline(iss, line)) {
            std::istringstream requestLineStream(line);
            requestLineStream >> method >> path >> httpVersion;
        }

        // Parse headers
        while (std::getline(iss, line) && !line.empty()) {
            size_t colonPos = line.find(':');
            if (colonPos != std::string::npos) {
                std::string headerName = line.substr(0, colonPos);
                std::string headerValue = line.substr(colonPos + 2); // Skip colon and space
                headers[headerName] = headerValue; // Insert into map
            }
        }

        // Parse body
        body = iss.str().substr(iss.tellg());
    }

    // Method to print the parsed request
    void printRequest() const {
        std::cout << "Method: " << method << std::endl;
        std::cout << "Path: " << path << std::endl;
        std::cout << "HTTP Version: " << httpVersion << std::endl;

        // Print headers
        std::cout << "Headers:" << std::endl;
        for (const auto& header : headers) {
            std::cout << header.first << ": " << header.second << std::endl;
        }

        // Print body
        std::cout << "Body: " << body << std::endl;
    }
};

int main() {
    // Example usage: create an instance of HTTPRequest and parse a request string
    std::string requestString =
        "GET /index.html HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "User-Agent: Mozilla/5.0\r\n"
        "Content-Length: 12\r\n"
        "\r\n"
        "Hello, World!";
    HTTPRequest request(requestString);

    // Print the parsed request
    request.printRequest();

    return 0;
}
