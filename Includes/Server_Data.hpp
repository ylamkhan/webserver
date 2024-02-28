#ifndef SERVER_HPP
#define SERVER_HPP

#include "../Includes/webserver.hpp"
#include "../Includes/Location.hpp"

class Location;
class Client;
typedef std::vector<Location> vecLoc;
class Server {
    private:
        std::string root;
        vecSPair cgi_paths;
        vecStr index;
        bool auto_index;
        std::string redirUrl;
        int port_as_int;
        vecInt imethods;
        mapErr error_pages;
        std::string server_names;
        size_t client_max_body_size;
        std::string host;
        std::vector<Location> locations;

        bool hostSet;
        bool serverNamesSet;

        bool rootSet;
        bool indexSet;
        bool maxClientBodySizeSet;
        bool methodsSet;
        bool errorPagesSet;
        bool redirUrlSet;
        bool autoIndexSet;
        bool cgiPathsSet;    

    public:
        Server();
        Server(const Server &s);
        Server &operator=(const Server &s);
        ~Server();

        std::string getRoot() const;
        vecStr getIndex() const;
        size_t getMaxClientBodySize() const;
        vecInt getMethods() const;
        mapErr getErrorPages() const;
        std::string getRedirUrl() const;
        bool getAutoIndex() const;
        std::string getUpload() const;
        int getPort() const;
        std::string getHost() const;
        std::string getServerNames() const;
        vecSPair getCgi() const;
        vecLoc getLocations() const;

        void setCgiPaths(pairStr p);
        void setRoot(std::string r);
        void setHost(std::string h);
        void setPort(int p);
        void setServerName(std::string s);
        void setIndex(std::string i);
        void setMaxClientBodySize(size_t m);
        void setIMethods(int i);
        void setErrorPages(int code, std::string page);
        void setAutoIndex(bool b);
        void setRedirUrl(std::string s);
        void setUpload(std::string up);
        void setLocation(Location loc);

        void setrootSet(bool b);
        void setindexSet(bool b);
        void setmaxClientBodySizeSet(bool b);
        void setmethodsSet(bool b);
        void seterrorPagesSet(bool b);
        void setredirUrlSet(bool b);
        void setautoIndexSet(bool b);
        void setcgiPathsSet(bool b);
        void sethostSet(bool b);
        void setServerNamesSet(bool b);

        bool isRootSet() const;
        bool isIndexSet() const;
        bool isMaxClientBodySet() const;
        bool isMethodsSet() const;
        bool isErrorPagesSet() const;
        bool isRedirUrlSet() const;
        bool isAutoIndexSet() const;
        bool isCgiPathsSet() const;
        bool isHostSet() const;
        bool isServerNamesSet() const;
        void printInfo() const {
            std::cout << "Server Information:" << std::endl;
            std::cout << "Host: " << host << std::endl;
            std::cout << "Port: " << port_as_int << std::endl;
            std::cout << "Root: " << root << std::endl;
            std::cout << "Auto Index: " << (auto_index ? "Enabled" : "Disabled") << std::endl;
            std::cout << "Redir Url: " << redirUrl << std::endl;
            std::cout << "Server Names: " << server_names << std::endl;
            std::cout << "Max Client Body Size: " << client_max_body_size << std::endl;

            // Print error pages
            std::cout << "Error Pages:" << std::endl;
            for (mapErr::const_iterator it = error_pages.begin(); it != error_pages.end(); ++it) {
                std::cout << "   " << it->first << ": " << it->second << std::endl;
            }

            // Print CGI paths
            std::cout << "CGI Paths:" << std::endl;
            for (size_t i = 0; i < cgi_paths.size(); ++i) {
                std::cout << "   " << cgi_paths[i].first << ": " << cgi_paths[i].second << std::endl;
            }

            // Print locations
            std::cout << "Locations:" << std::endl;
            for (size_t i = 0; i < locations.size(); ++i) {
                const Location& loc = locations[i];
                std::cout<<"dgfvdhgbfdhngfbdhgvb\n";
                std::cout << "   Location " << i + 1 << ":" << std::endl;
                loc.printInfo(); // Assuming Location class has a printInfo function
            }
        }
};

#endif