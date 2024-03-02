#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "../Includes/webserver.hpp"

typedef std::vector<std::string>					vecStr;
typedef std::map<std::string, std::string>			mapStr;
typedef std::map<int, std::string>					mapErr;
typedef	std::vector<int>							vecInt;
typedef	std::map<int, int>							mapSock;
typedef std::vector< std::pair<std::string, int> >	vecFiles;
typedef std::pair<std::string, std::string> 		pairStr;
typedef std::vector<std::pair<std::string, std::string> > vecSPair;

class Location {
        private:
                std::string location_path;
                bool auto_index;
                vecStr index;
                std::string root;
                std::string upload;
                std::string list;
                vecStr methods;
                vecInt imethods;
                mapErr error_pages;
                vecSPair cgi_paths;
                size_t proxy_read_timeout;
                size_t client_max_body_size;
                std::string redirUrl;

                bool rootSet;
                bool indexSet;
                bool maxClientBodySizeSet;
                bool methodsSet;
                bool errorPagesSet;
                bool redirUrlSet;
                bool autoIndexSet;
                bool cgiPathsSet;
                
        public:
                Location();
                Location(const Location &copy);
                Location &operator=(const Location &other);
                ~Location();

                std::string getRoot() const;
                vecStr getIndex() const;
                size_t getMaxClientBodySize() const;
                vecInt getMethods() const;
                mapErr getErrorPages() const;
                std::string getRedirUrl() const;
                bool getAutoIndex() const;
                std::string getUpload() const;
                vecSPair getCgi() const;
                std::string getLocationPath() const;
                std::string getList() const;

                void setCgiPaths(pairStr p);
                void setRoot(std::string r);
                void setIndex(std::string i);
                void setMaxClientBodySize(size_t m);
                void setMethods(std::string method);
                void setIMethods(int i);
                void setErrorPages(int code, std::string page);
                void setAutoIndex(bool b);
                void setRedirUrl(std::string s);
                void setUpload(std::string up);
                void setProxyTimeout(size_t p);
                void setLocPath(std::string path);
                void setList(std::string l);

                void setrootSet(bool b);
                void setindexSet(bool b);
                void setmaxClientBodySizeSet(bool b);
                void setmethodsSet(bool b);
                void seterrorPagesSet(bool b);
                void setredirUrlSet(bool b);
                void setautoIndexSet(bool b);
                void setcgiPathsSet(bool b);

                bool isRootSet() const;
                bool isIndexSet() const;
                bool isMaxClientBodySet() const;
                bool isMethodsSet() const;
                bool isErrorPagesSet() const;
                bool isRedirUrlSet() const;
                bool isAutoIndexSet() const;
                bool isCgiPathsSet() const;
                void printInfo() const {
                    std::cout << "Location Path: " << location_path << std::endl;
                    std::cout << "Root: " << root << std::endl;
                    std::cout << "Upload: " << upload << std::endl;
                    std::cout << "Auto Index: " << (auto_index ? "Enabled" : "Disabled") << std::endl;
                    std::cout << "Methods: ";
                    for (size_t i = 0; i < methods.size(); ++i) {
                        std::cout << methods[i] << " ";
                    }
                    std::cout << std::endl;
                    std::cout << "Max Client Body Size: " << client_max_body_size << std::endl;
                    std::cout << "Redir Url: " << redirUrl << std::endl;

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

                    // Print other attributes as needed
                    // ...
                }
        
};

#endif