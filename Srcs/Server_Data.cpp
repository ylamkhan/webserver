#include "../Includes/Server_Data.hpp"

Server::Server() {
    root = "/";
    redirUrl = "";
    host = "";
    maxClientBodySizeSet = 1;
    rootSet = false;
    indexSet = false;
    maxClientBodySizeSet = false;
    methodsSet = false;
    errorPagesSet = false;
    redirUrlSet = false;
    autoIndexSet = false;
    cgiPathsSet = false;
}

Server::Server(const Server &copy) {
    *this = copy;
}

Server &Server::operator=(const Server &other) {
    if (this != &other) {
        index.clear();
        imethods.clear();
        error_pages.clear();
        cgi_paths.clear();

        root = other.root;
        port_as_int = other.port_as_int;
        auto_index = other.auto_index;
        imethods = other.imethods;
        error_pages = other.error_pages;
        cgi_paths = other.cgi_paths;
        client_max_body_size = other.client_max_body_size;
        redirUrl = other.redirUrl;
        host = other.host;
        server_names = other.server_names;
        locations = other.locations;

        rootSet = other.rootSet;
        indexSet = other.indexSet;
        maxClientBodySizeSet = other.maxClientBodySizeSet;
        methodsSet = other.methodsSet;
        errorPagesSet = other.errorPagesSet;
        redirUrlSet = other.redirUrlSet;
        autoIndexSet = other.autoIndexSet;
        // sizel = other.sizel;
    }
    return (*this);
}

Server::~Server() {

}

/****************GETTERS********************/

vecLoc Server::getLocations() const {
    std::cout<<locations.size()<<"***************\n";
    return locations;
}

std::string Server::getHost() const {
    return host;
}

int Server::getPort() const {
    return port_as_int;
}

std::string Server::getServerNames() const {
    return server_names;
}

std::string Server::getRoot() const {
    return root;
}

vecStr Server::getIndex() const {
    return index;
}

size_t Server::getMaxClientBodySize() const {
    return maxClientBodySizeSet;
}

vecInt Server::getMethods() const {
    return imethods;
}

mapErr Server::getErrorPages() const {
    return error_pages;
}

std::string Server::getRedirUrl() const {
    return redirUrl;
}

bool Server::getAutoIndex() const {
    return auto_index;
}

vecSPair Server::getCgi() const {
    return cgi_paths;
}

bool Server::isRootSet() const {
    return rootSet;
}

bool Server::isIndexSet() const {
    return indexSet;
}

bool Server::isMaxClientBodySet() const {
    return maxClientBodySizeSet;
}

bool Server::isMethodsSet() const {
    return methodsSet;
}

bool Server::isErrorPagesSet() const {
    return errorPagesSet;
}

bool Server::isRedirUrlSet() const {
    return redirUrlSet;
}

bool Server::isAutoIndexSet() const {
    return autoIndexSet;
}

bool Server::isCgiPathsSet() const {
    return cgiPathsSet;
}

bool Server::isHostSet() const {
    return hostSet;
}

bool Server::isServerNamesSet() const {
    return serverNamesSet;
}

/*****************SETTERS********************/

void Server::setLocation(Location loc)
{
    locations.push_back(loc);
}

void Server::setCgiPaths(pairStr p) {
    cgi_paths.push_back(p);
}

void Server::setRoot(std::string r) {
    root = r;
}

void Server::setPort(int p) {
    port_as_int = p;
}

void Server::setHost(std::string h) {
    host = h;
}

void Server::setServerName(std::string s) {
    server_names = s;
}

void Server::setIndex(std::string i) {
    index.push_back(i);
}

void Server::setMaxClientBodySize(size_t m) {
    client_max_body_size = m;
}

void Server::setIMethods(int i) {
    imethods.push_back(i);
}

void Server::setErrorPages(int code, std::string page) {
    error_pages[code] = page;
}

void Server::setAutoIndex(bool b) {
    auto_index = b;
}

void Server::setRedirUrl(std::string s) {
    redirUrl = s;
}

void Server::setrootSet(bool b) {
    rootSet = b;
}

void Server::setindexSet(bool b) {
    indexSet = b;
}

void Server::setmaxClientBodySizeSet(bool b) {
    maxClientBodySizeSet = b;
}

void Server::setmethodsSet(bool b) {
    methodsSet = b;
}

void Server::seterrorPagesSet(bool b) {
    errorPagesSet = b;
}

void Server::setredirUrlSet(bool b) {
    redirUrlSet = b;
}

void Server::setautoIndexSet(bool b) {
    autoIndexSet = b;
}

void Server::setcgiPathsSet(bool b) {
    cgiPathsSet = b;
}

void Server::sethostSet(bool b) {
    hostSet = b;
}

void Server::setServerNamesSet(bool b) {
    serverNamesSet = b;
}
