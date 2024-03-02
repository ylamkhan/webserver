#include "../Includes/webserver.hpp"

Location::Location() {
    root = "/";
    redirUrl = "";
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

Location::Location(const Location &copy) {
    *this = copy;
}

Location &Location::operator=(const Location &other) {
    if (this != &other) {
        index.clear();
        imethods.clear();
        methods.clear();
        error_pages.clear();
        cgi_paths.clear();

        root = other.root;
        location_path = other.location_path;
        auto_index = other.auto_index;
        upload = other.upload;
        methods = other.methods;
        imethods = other.imethods;
        error_pages = other.error_pages;
        cgi_paths = other.cgi_paths;
        proxy_read_timeout = other.proxy_read_timeout;
        client_max_body_size = other.client_max_body_size;
        redirUrl = other.redirUrl;

        rootSet = other.rootSet;
        indexSet = other.indexSet;
        maxClientBodySizeSet = other.maxClientBodySizeSet;
        methodsSet = other.methodsSet;
        errorPagesSet = other.errorPagesSet;
        redirUrlSet = other.redirUrlSet;
        autoIndexSet = other.autoIndexSet;
    }
    return (*this);
}

Location::~Location() {

}

/****************GETTERS********************/

std::string Location::getLocationPath() const {
    return location_path;
}

vecSPair Location::getCgi() const {
    return cgi_paths;
}

std::string Location::getUpload() const {
    return upload;
}

std::string Location::getRoot() const {
    return root;
}

vecStr Location::getIndex() const {
    return index;
}

size_t Location::getMaxClientBodySize() const {
    return maxClientBodySizeSet;
}

vecInt Location::getMethods() const {
    return imethods;
}

mapErr Location::getErrorPages() const {
    return error_pages;
}

std::string Location::getRedirUrl() const {
    return redirUrl;
}

bool Location::getAutoIndex() const {
    return auto_index;
}

bool Location::isRootSet() const {
    return rootSet;
}

bool Location::isIndexSet() const {
    return indexSet;
}

bool Location::isMaxClientBodySet() const {
    return maxClientBodySizeSet;
}

bool Location::isMethodsSet() const {
    return methodsSet;
}

bool Location::isErrorPagesSet() const {
    return errorPagesSet;
}

bool Location::isRedirUrlSet() const {
    return redirUrlSet;
}

bool Location::isAutoIndexSet() const {
    return autoIndexSet;
}

bool Location::isCgiPathsSet() const {
    return cgiPathsSet;
}

/*****************SETTERS********************/

void Location::setLocPath(std::string path) {
    if (path[path.size()-1] == '/' && path != "/")
        path.erase(path.size()-1);
    location_path = path;
}

void Location::setCgiPaths(pairStr p) {
    cgi_paths.push_back(p);
}

void Location::setRoot(std::string r) {
    root = r;
}

void Location::setIndex(std::string i) {
    index.push_back(i);
}

void Location::setMaxClientBodySize(size_t m) {
    client_max_body_size = m;
}

void Location::setMethods(std::string method) {
    methods.push_back(method);
}

void Location::setIMethods(int i) {
    imethods.push_back(i);
}

void Location::setErrorPages(int code, std::string page) {
    error_pages[code] = page;
}

void Location::setAutoIndex(bool b) {
    auto_index = b;
}

void Location::setRedirUrl(std::string s) {
    redirUrl = s;
}

void Location::setUpload(std::string up) {
    upload = up;
}

void Location::setProxyTimeout(size_t p) {
    proxy_read_timeout = p;
}

void Location::setrootSet(bool b) {
    rootSet = b;
}

void Location::setindexSet(bool b) {
    indexSet = b;
}

void Location::setmaxClientBodySizeSet(bool b) {
    maxClientBodySizeSet = b;
}

void Location::setmethodsSet(bool b) {
    methodsSet = b;
}

void Location::seterrorPagesSet(bool b) {
    errorPagesSet = b;
}

void Location::setredirUrlSet(bool b) {
    redirUrlSet = b;
}

void Location::setautoIndexSet(bool b) {
    autoIndexSet = b;
}

void Location::setcgiPathsSet(bool b) {
    cgiPathsSet = b;
}
