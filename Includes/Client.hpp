#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <fstream>
// typename check if ... 
#include <map>
#include <fstream>
#include <string>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include<dirent.h>
#include <filesystem>
#include "../Includes/Server_Data.hpp"
#include "../Includes/ConfigParser.hpp"


static std::string name_file = "out";
class Socket;

class Client {
    private:
        int sockfd;
        std::ofstream file;
        std::ifstream a_file;
        std::string url;
        DIR *dir;

        std::vector<Server> servers;

        bool flag_open;
        bool flag_in_out;
        bool headerSet;
        bool Opened;
        bool isDir;
        bool isFile;
        bool startRead;
        bool cgiflag;
        bool flagResponse;

        std::string name_path;
        std::string cgiUrl;
        std::string getUrl;
        std::string type;
        std::string requesta;
        std::string method;
        std::string path;
        std::string query;
        std::string httpVersion;
        std::map<std::string, std::string>headers;
        std::map<std::string, std::string>mime_type;
        std::string body;
        std::string requestStr;
        size_t content_len;
        size_t bodySize;
        size_t hexSize;
        size_t store_hexSize;
        size_t sindex;
        size_t lindex;
        std::string executable;
        size_t status;

        int port;
        std::string host;
        std::string URL;
        ///
        std::string value_type;
        size_t chunks_size; // find 
        std::string chunks;
        std::string transfer_encoding;
        std::string ss;
        std::string save;
        std::string store;
        std::string shinka;
        ////
        std::string sab;
        std::string reqURL;
        std::string message;
        bool listing;
        bool closed;

    public:
        Client(){};
        Client(std::vector<Server> &servers);
        ~Client();
        Client &operator=(Client const &other);
        Client(Client const &other);


        bool get_flag_in_out() const;
        void cgi(std::string u);
        int getSocket() const;
        void setFd(int fd);
        void open_file();
        std::string get_file_name() const ;
        void parseRequest(const std::string& httpRequest);
        void set_request_client(std::string requ);
        std::string getMethod() const;
        std::string getPath() const;
        std::string getHTTPVersion() const;
        std::map<std::string, std::string> getHeaders() const;
        std::string getBody() const;
        std::string getReqStr() const;
        std::string getMessage() const;
        size_t getStatus() const;
        
        void    setReqStr(std::string s);
        void    handl_methodes();
        int     matching_servers();
        void    setPortHost(std::string headerValue);
        void	store_type(void);
        void    setSindex(size_t i);
        int     mattching(std::string url, std::string pathloc);
        unsigned int hexa_to_dec(const std::string& str);
        std::string& ltrim(std::string& str);
        int checkforCgi(std::string type);


        void post();
        void get();
        void web_delete();

        void Response();
        std::string toString();
        int checkMethod();
        int is_req_well_formed();

        void    readFile(std::string u);
        void    listDir();
        bool getflagResponse();
        void setflagResponse(bool t);
        std::ifstream &get_a_file();
        void send_client();
        bool getClosed() const;

        /**********************************/
};

#endif
