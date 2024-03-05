#include "../Includes/ConfigParser.hpp"
#include <climits>

ConfigParser::ConfigParser(std::string file)
{
    parseConfig(file);
}

bool ConfigParser::isEmptyLine(const std::string& line)
{
    return line.find_first_not_of(" \t\n") == std::string::npos;
}

void ConfigParser::parseConfig(std::string filename)
{
    std::ifstream   file(filename.c_str());
    std::vector<std::string> lines;
    int i = 0;

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(1);
    }

    std::string line;
    while(std::getline(file, line))
    {
        lines.push_back(line);
        i++;
    }
    if (i == 0) {
        std::cerr << "Error: empty file " << filename << std::endl;
        exit(1);
    }

    fillServers(lines);
    file.close();
}

void ConfigParser::fillServers(std::vector<std::string> lines)
{
    size_t i = 0;
    size_t count = 0;
    
    while (i < lines.size())
    {
        if (isEmptyLine(lines[i]))
        {
            count++;
            i++;
            if (count == lines.size())
            {
                std::cerr << "Invalid Config File!!!" << std::endl;
                exit(1);
            }
            continue;
        }
        if (lines[i] == "servers:")
        {
            i++;
            while(isEmptyLine(lines[i]))
                i++;
            if (lines[i] == "  - listen:")
                fillServer(lines, &i);
            else
            {
                std::cerr << "Invalid Config File!!!" << std::endl;
                exit(1);
            }
        }
        i++;
    }
}


void ConfigParser::fillServer(std::vector<std::string> &lines, size_t *i)
{
    Server server;

    (*i)++;
    while((*i) < lines.size())
    {
        if (isEmptyLine(lines[*i]))
        {
            (*i)++;
            continue;
        }
        else if (lines[*i] == "    locations:")
            fillLocations(lines, i, server);
        else if (lines[*i] == "  - listen:")
            fillServer(lines, i);
        else
            fillOther(lines, i, server);
        (*i)++;
    }
    servers.push_back(server);
}

void ConfigParser::fillLocations(std::vector<std::string> &lines, size_t *i, Server &server)
{
    (*i)++;
    while((*i) < lines.size())
    {
        if (isEmptyLine(lines[*i]))
        {
            (*i)++;
            continue;
        }
        else if (lines[*i].substr(0, 13) == "      - path:")
            fillLocation(lines, i, server);
        else
        {
            std::cerr << "Invalid Config File" << std::endl;
            exit(1);
        }
        (*i)++;
    }
}

void ConfigParser::fillLocation(std::vector<std::string> lines, size_t *i, Server &server)
{
    Location location;

    fillLocPath(location, lines, i);
    (*i)++;
    while((*i) < lines.size())
    {
        if (isEmptyLine(lines[*i]))
        {
            (*i)++;
            continue;
        }
        else if (lines[*i].compare(0, 13, "        root:") == 0)
            saveLocRoot(location, lines, i);
        else if (lines[*i].compare(0, 15, "        upload:") == 0)
            saveUpload(location, lines, i);
        else if (lines[*i].compare(0, 13, "        list:") == 0)
            saveList(location, lines, i);
        else if (lines[*i].compare(0, 18, "        autoindex:") == 0)
            saveAutoindex(location, lines, i);
        else if (lines[*i].compare(0, 16, "        methods:") == 0)
            saveMethods(location, lines, i);
        else if (lines[*i].compare(0, 14, "        index:") == 0)
            saveIndex(location, lines, i);
        else if (lines[*i].compare(0, 18, "        cgi_paths:") == 0)
            saveCgiPaths(location, lines, i);
        else if (lines[*i].compare(0, 27, "        proxy_read_timeout:") == 0)
            saveProxy(location, lines, i);
        else if (lines[*i].compare(0, 20, "        error_pages:") == 0)
            saveLocErrorPages(location, lines, i);
        else if (lines[*i].compare(0, 15, "        return:") == 0)
            saveRedirUrl(location, lines, i);
        else if (lines[*i].compare(0, 13, "      - path:") == 0)
            fillLocation(lines, i, server);
        else if (lines[*i].compare(0, 11, "  - listen:") == 0)
            fillServer(lines, i);
        else
        {
            std::cerr << "Invalid Config File$" << std::endl;
            exit(1);
        }
        (*i)++;
    }
    server.setLocation(location);
}

void    ConfigParser::saveList(Location &location, std::vector<std::string> lines, size_t *i) {
    std::string prefix = "        list: ";
    std::string line = lines[*i];

    location.setList(line.substr(prefix.length()));
    std::string list = location.getList();
    std::transform(list.begin(), list.end(), list.begin(), ::toupper);
    // exit(0);
    if (list != "ON" && list != "OFF" && list.size() != 0) {
        std::cerr << "Zineb: Invalid value for List. It should be either 'ON' or 'OFF'." << std::endl;
        exit(1);
    }
}

void ConfigParser::saveRedirUrl(Location &location, std::vector<std::string> lines, size_t *i) {
    std::string prefix = "        return: ";
    std::string line = lines[*i];
    
    location.setRedirUrl(line.substr(prefix.length()));
    if (location.getRedirUrl().find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890./_-:") != std::string::npos) {
        std::cerr << "Zineb: Invalid characters detected after the reurn url!" << std::endl;
        exit(1);
    }
    location.setredirUrlSet(true);
}

void ConfigParser::saveSRedirUrl(Server &server, std::vector<std::string> lines, size_t *i) {
    std::string prefix = "    return: ";
    std::string line = lines[*i];

    server.setRedirUrl(line.substr(prefix.length()));
    if (server.getRedirUrl().find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890./_-:") != std::string::npos) {
        std::cerr << "Zineb: Invalid characters detected after the reurn url!" << std::endl;
        exit(1);
    }
    server.setredirUrlSet(true);
}

void ConfigParser::saveLocErrorPages(Location &location, std::vector<std::string> lines, size_t *i) {
    std::string prefix = "        error_pages:";
    std::string line = lines[*i];
        
    (*i)++;
    line = lines[*i];

    while (line.find("          - ") == 0) {
        std::istringstream iss(line.substr(12));
        std::string code;
        int errorCode;
        std::string pagePath;
        char colon;

        std::getline(iss, code, ':');
        iss >> colon;
        std::getline(iss, pagePath);
        // std::cout << "++++++++"<<colon<<"+*************++\n";

        //iss >> errorCode >> colon >> pagePath;
        if (code.length() != 3 || code.find_first_not_of("0123456789") != std::string::npos)
        {
            std::cerr << "Zineb: Invalid error code!" << std::endl;
            exit(1);
        }
        
        // std::cout << "++++++++"<<pagePath<<"+++++++++++++++++++++++++++++\n";
        if (pagePath.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890./_") != std::string::npos)
        {
            std::cerr << "Zineb: Invalid characters detected in the error page path!" << std::endl;
            exit(1);
        }

        errorCode = std::atoi(code.c_str());
        if (errorCode < 100 || errorCode > 999 || pagePath.empty()) {
            std::cerr << "Zineb: Invalid format for error_pages entry." << std::endl;
            exit(1);
        }
        location.setErrorPages(errorCode, pagePath);
        (*i)++;
        line = lines[*i];
    }
    (*i)--;
    location.seterrorPagesSet(true);
}

void ConfigParser::fillLocPath(Location &location, std::vector<std::string> &lines, size_t *i) {
    std::string prefix = "      - path:";
    std::string line = lines[*i];    

    std::string path = line.substr(prefix.length());

    path.erase(0, path.find_first_not_of(" \t"));
    path.erase(path.find_last_not_of(" \t") + 1);

    if (path.empty()) {
        std::cerr << "Error: Invalid path value." << std::endl;
        exit(1);
    }

    size_t lastQuotePos = path.find_last_of('"');
    size_t lastSpacePos = path.find_last_of(" \t");

    if (lastQuotePos != std::string::npos && lastQuotePos != path.length() - 1) {
        std::cerr << "Zineb: Additional characters detected after the path." << std::endl;
        exit(1);
    }

    if (lastSpacePos != std::string::npos && lastSpacePos != path.length() - 1) {
        std::cerr << "Zineb: Additional characters detected after the path." << std::endl;
        exit(1);
    }

    location.setLocPath(path);
}



void ConfigParser::saveLocRoot(Location &location, std::vector<std::string> &lines, size_t *i)
{
    std::string prefix = "        root: ";
    std::string line = lines[*i];
    
    location.setRoot(line.substr(prefix.length()));
    if (location.getRoot().find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890./_-") != std::string::npos) {
        std::cerr << "Zineb: Invalid characters detected after the root directory path!" << std::endl;
        exit(1);
    }
    location.setrootSet(true);
}

void ConfigParser::saveUpload(Location &location, std::vector<std::string> &lines, size_t *i)
{
    std::string prefix = "        upload: ";
    std::string line = lines[*i];

    location.setUpload(line.substr(prefix.length()));
    std::string upload = location.getUpload();
    std::transform(upload.begin(), upload.end(), upload.begin(), ::toupper);
    // exit(0);
    if (upload != "ON" && upload != "OFF" && upload.size() != 0) {
        std::cerr << "Zineb: Invalid value for upload. It should be either 'ON' or 'OFF'." << std::endl;
        exit(1);
    }
}

void ConfigParser::saveAutoindex(Location &location, std::vector<std::string> &lines, size_t *i)
{
    std::string prefix = "        autoindex: ";
    std::string line = lines[*i];
    std::string s;

    s = line.substr(prefix.length());
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    if (s != "ON" && s != "OFF") {
        std::cerr << "Zineb: Invalid value for autoindex. It should be either 'ON' or 'OFF'." << std::endl;
        exit(1);
    }
    else if (s == "ON")
        location.setAutoIndex(true);
    else if (s == "OFF")
        location.setAutoIndex(false);
    location.setautoIndexSet(true);
}

void ConfigParser::saveSAutoindex(Server &server, std::vector<std::string> &lines, size_t *i)
{
    std::string prefix = "    autoindex: ";
    std::string line = lines[*i];
    std::string s;

    s = line.substr(prefix.length());
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    if (s != "ON" && s != "OFF") {
        std::cerr << "Zineb: Invalid value for autoindex. It should be either 'ON' or 'OFF'." << std::endl;
        exit(1);
    }
    else if (s == "ON")
        server.setAutoIndex(true);
    else if (s == "OFF")
        server.setAutoIndex(false);
    server.setautoIndexSet(true);
}

void ConfigParser::saveMethods(Location &location, std::vector<std::string> &lines, size_t *i) {
    std::string prefix = "        methods: [";
    std::string line = lines[*i];
    
    if (line.find(prefix) == 0) {
        size_t closingBracketPos = line.find_last_of("]");
        
        if (closingBracketPos != std::string::npos) {
            std::string methods = line.substr(prefix.length(), closingBracketPos - prefix.length());

            std::istringstream iss(methods);
            std::string method;
            while (std::getline(iss, method, ',')) {
                method.erase(0, method.find_first_not_of(" \t"));
                method.erase(method.find_last_not_of(" \t") + 1);

                std::transform(method.begin(), method.end(), method.begin(), ::toupper);

                if (method != "POST" && method != "GET" && method != "DELETE") {
                    std::cerr << "Zineb: Invalid method detected! Methods should be POST, GET, or DELETE." << std::endl;
                    exit(1);
                }

                location.setMethods(method);
                if (method == "GET")
                    location.setIMethods(0);
                else if (method == "POST")
                    location.setIMethods(1);
                else if (method == "DELETE")
                    location.setIMethods(2);
                else {
                    std::cerr << "Zineb: Invalid method." << std::endl;
                    exit(1);
                }
            }
        } else {
            std::cerr << "Zineb: Missing closing bracket in methods entry." << std::endl;
            exit(1);
        }
    } else {
        std::cerr << "Zineb: Incorrect format for methods entry." << std::endl;
        exit(1);
    }
    location.setmethodsSet(true);
}

void ConfigParser::saveSMethods(Server &server, std::vector<std::string> &lines, size_t *i) {
    std::string prefix = "    methods: [";
    std::string line = lines[*i];
    
    if (line.find(prefix) == 0) {
        size_t closingBracketPos = line.find_last_of("]");
        
        if (closingBracketPos != std::string::npos) {
            std::string methods = line.substr(prefix.length(), closingBracketPos - prefix.length());

            std::istringstream iss(methods);
            std::string method;
            while (std::getline(iss, method, ',')) {
                method.erase(0, method.find_first_not_of(" \t"));
                method.erase(method.find_last_not_of(" \t") + 1);

                std::transform(method.begin(), method.end(), method.begin(), ::toupper);

                if (method != "POST" && method != "GET" && method != "DELETE") {
                    std::cerr << "Zineb: Invalid method detected! Methods should be POST, GET, or DELETE." << std::endl;
                    exit(1);
                }

                if (method == "GET")
                    server.setIMethods(0);
                else if (method == "POST")
                    server.setIMethods(1);
                else if (method == "DELETE")
                    server.setIMethods(2);
            }
        } else {
            std::cerr << "Zineb: Missing closing bracket in methods entry." << std::endl;
            exit(1);
        }
    } else {
        std::cerr << "Zineb: Incorrect format for methods entry." << std::endl;
        exit(1);
    }
    server.setmethodsSet(true);
}

void ConfigParser::saveIndex(Location &location, std::vector<std::string> &lines, size_t *i) {
    std::string prefix = "        index: [";
    std::string line = lines[*i];
    
    if (line.find(prefix) == 0) {
        size_t closingBracketPos = line.find_last_of("]");
        
        if (closingBracketPos != std::string::npos) {
            std::string indexString = line.substr(prefix.length(), closingBracketPos - prefix.length());

            std::istringstream iss(indexString);
            std::string index;
            while (std::getline(iss, index, ',')) {
                index.erase(0, index.find_first_not_of(" \t"));
                index.erase(index.find_last_not_of(" \t") + 1);

                if (index.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./_-") != std::string::npos) {
                    std::cerr << "Zineb: Invalid characters detected in index file name." << std::endl;
                    exit(1);
                }

                location.setIndex(index);
            }
        } else {
            std::cerr << "Zineb: Missing closing bracket in index entry." << std::endl;
            exit(1);
        }
    } else {
        std::cerr << "Zineb: Incorrect format for index entry." << std::endl;
        exit(1);
    }
    location.setindexSet(true);
}

void ConfigParser::saveCgiPaths(Location &location, std::vector<std::string> &lines, size_t *i)
{
    std::string prefix = "          - [";
    std::string line = lines[*i];

    (*i)++;
    line = lines[*i];

    while (line.find(prefix) == 0) {
        size_t openingBracketPos = line.find("[");
        size_t closingBracketPos = line.find("]", openingBracketPos);

        if (openingBracketPos != std::string::npos && closingBracketPos != std::string::npos && closingBracketPos > openingBracketPos) {
            std::istringstream iss(line.substr(openingBracketPos + 1, closingBracketPos - openingBracketPos - 1));
            std::string cgiPath, executable;
            char delimiter;
            iss >> delimiter;

            std::getline(iss, cgiPath, ',');
            std::getline(iss, executable, ']');

            cgiPath.erase(0, cgiPath.find_first_not_of(" \t"));
            cgiPath.erase(cgiPath.find_last_not_of(" \t") + 1);
            executable.erase(0, executable.find_first_not_of(" \t"));
            executable.erase(executable.find_last_not_of(" \t") + 1);

            if (cgiPath.empty() || executable.empty() ||
                cgiPath.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./_-") != std::string::npos ||
                executable.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./_-") != std::string::npos) {
                std::cerr << "Zineb: Invalid characters detected in cgi_path or executable." << std::endl;
                exit(1);
            }

            location.setCgiPaths(std::make_pair(cgiPath, executable));

            size_t afterClosingBracketPos = line.find_first_not_of(" \t", closingBracketPos + 1);
            if (afterClosingBracketPos != std::string::npos) {
                std::cerr << "Zineb: Characters found after the closing bracket." << std::endl;
                exit(1);
            }
            (*i)++;
            if (*i >= lines.size())
                break;
            line = lines[*i];
            
        } else {
            std::cerr << "Zineb: Missing or mismatched opening/closing brackets." << std::endl;
            exit(1);
        }
    }
    location.setcgiPathsSet(true);
    (*i)--;
}

void ConfigParser::saveProxy(Location &location, std::vector<std::string> &lines, size_t *i)
{
    std::string prefix = "        proxy_read_timeout:";
    std::string line = lines[*i];

    std::string value = line.substr(prefix.length());

    value.erase(0, value.find_first_not_of(" \t"));
    value.erase(value.find_last_not_of(" \t") + 1);

    if (value.find_first_not_of("0123456789s") != std::string::npos) {
        std::cerr << "Zineb: Invalid characters detected in proxy_read_timeout value." << std::endl;
        exit(1);
    }
    
    size_t p;
    std::istringstream iss(value);
    iss >> p;
    location.setProxyTimeout(p);
}

void ConfigParser::fillOther(std::vector<std::string> &lines, size_t *i, Server &server)
{
    if (lines[*i].compare(0, 17, "    server_name: ") == 0)
        saveName(server, lines, i);
    else if (lines[*i].compare(0, 10, "    port: ") == 0)
        savePort(server, lines, i);
    else if (lines[*i].compare(0, 10, "    host: ") == 0)
        saveHost(server, lines, i);
    else if (lines[*i].compare(0, 10, "    root: ") == 0)
        saveRoot(server, lines, i);
    else if (lines[*i].compare(0, 16, "    error_pages:") == 0)
        saveErrorPages(server, lines, i);
    else if (lines[*i].compare(0, 26, "    client_max_body_size: ") == 0)
        saveBodySize(server, lines, i);
    else if (lines[*i].compare(0, 11, "    index: ") == 0)
        saveSIndex(server, lines, i);
    else if (lines[*i].compare(0, 14, "    cgi_paths:") == 0)
        saveSCgiPaths(server, lines, i);
    else if (lines[*i].compare(0, 15, "    autoindex: ") == 0)
        saveSAutoindex(server, lines, i);
    else if (lines[*i].compare(0, 12, "    return: ") == 0)
        saveSRedirUrl(server, lines, i);
    else if (lines[*i].compare(0, 13, "    methods: ") == 0)
        saveSMethods(server, lines, i);
    else
    {
        std::cerr << "Invalid Config File." << std::endl;
        exit(1);
    }
}

void ConfigParser::saveSIndex(Server &server, std::vector<std::string> lines, size_t *i)
{
    std::string prefix = "    index: [";
    std::string line = lines[*i];
    
    if (line.find(prefix) == 0) {
        size_t closingBracketPos = line.find_last_of("]");
        
        if (closingBracketPos != std::string::npos) {
            std::string indexString = line.substr(prefix.length(), closingBracketPos - prefix.length());

            std::istringstream iss(indexString);
            std::string index;
            while (std::getline(iss, index, ',')) {
                index.erase(0, index.find_first_not_of(" \t"));
                index.erase(index.find_last_not_of(" \t") + 1);

                if (index.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./_-") != std::string::npos) {
                    std::cerr << "Zineb: Invalid characters detected in index file name." << std::endl;
                    exit(1);
                }

                server.setIndex(index);
            }
        } else {
            std::cerr << "Zineb: Missing closing bracket in index entry." << std::endl;
            exit(1);
        }
    } else {
        std::cerr << "Zineb: Incorrect format for index entry." << std::endl;
        exit(1);
    }
    server.setindexSet(true);
}

void    ConfigParser::saveSCgiPaths(Server &server, std::vector<std::string> lines, size_t *i)
{
    std::string prefix = "      - [";
    std::string line = lines[*i];

    (*i)++;
    line = lines[*i];

    while (line.find(prefix) == 0) {
        size_t openingBracketPos = line.find("[");
        size_t closingBracketPos = line.find("]", openingBracketPos);

        if (openingBracketPos != std::string::npos && closingBracketPos != std::string::npos && closingBracketPos > openingBracketPos) {
            std::istringstream iss(line.substr(openingBracketPos + 1, closingBracketPos - openingBracketPos - 1));
            std::string cgiPath, executable;

            std::getline(iss, cgiPath, ',');
            std::getline(iss, executable, ']');

            cgiPath.erase(0, cgiPath.find_first_not_of(" \t"));
            cgiPath.erase(cgiPath.find_last_not_of(" \t") + 1);
            executable.erase(0, executable.find_first_not_of(" \t"));
            executable.erase(executable.find_last_not_of(" \t") + 1);

            if (cgiPath.empty() || executable.empty() ||
                cgiPath.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./_-") != std::string::npos ||
                executable.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./_-") != std::string::npos) {
                std::cerr << "Zineb: Invalid characters detected in cgi_path or executable." << std::endl;
                exit(1);
            }

            server.setCgiPaths(std::make_pair(cgiPath, executable));

            size_t afterClosingBracketPos = line.find_first_not_of(" \t", closingBracketPos + 1);
            if (afterClosingBracketPos != std::string::npos) {
                std::cerr << "Zineb: Characters found after the closing bracket." << std::endl;
                exit(1);
            }
            (*i)++;
            if (*i >= lines.size())
                break;
            line = lines[*i];
            
        } else {
            std::cerr << "Zineb: Missing or mismatched opening/closing brackets." << std::endl;
            exit(1);
        }
    }
    server.setcgiPathsSet(true);
    (*i)--;
}

void ConfigParser::saveName(Server &server, std::vector<std::string> &lines, size_t *i)
{
    std::string prefix = "    server_name:";
    std::string line = lines[*i];

    std::string value = line.substr(prefix.length());
    
    value.erase(0, value.find_first_not_of(" \t"));
    value.erase(value.find_last_not_of(" \t") + 1);
    
    if (value.empty() || value.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./_-") != std::string::npos) {
        std::cerr << "Zineb: Invalid server_name value." << std::endl;
        exit(1);
    }

    if (server.getServerNames().empty()) {
        server.setServerName(value);
    } else {
        std::cerr << "Zineb: Multiple server name values specified." << std::endl;
        exit(1);
    }
}

void ConfigParser::saveBodySize(Server &server, std::vector<std::string> &lines, size_t *i) {
    std::string prefix = "    client_max_body_size:";
    std::string line = lines[*i];

    std::string value = line.substr(prefix.length());

    value.erase(0, value.find_first_not_of(" \t"));
    value.erase(value.find_last_not_of(" \t") + 1);

    size_t multiplier = 0;
    char suffix = value[value.size()-1];
    if (suffix == 'g' || suffix == 'G') {
        multiplier = 1024 * 1024 * 1024;
    } else if (suffix == 'm' || suffix == 'M') {
        multiplier = 1024 * 1024;
    } else if (suffix == 'k' || suffix == 'K') {
        multiplier = 1024;
    } else if (suffix != 'b' && suffix != 'B') {
        multiplier = 1;
    } else {
        std::cerr << "Zineb: Invalid suffix in client_max_body_size value." << std::endl;
        exit(1);
    }

    if (multiplier != 0) {
        if (!value.empty()) {
            value.erase(value.size() - 1);
        }
    }

    if (value.find_first_not_of("0123456789") != std::string::npos) {
        std::cerr << "Zineb: Invalid characters detected in client_max_body_size value." << std::endl;
        exit(1);
    }
    unsigned long long sizeInBytes = strtoull(value.c_str(), NULL, 10) * multiplier;
    //unsigned long long sizeInBytes = std::stoull(value) * multiplier;

    if (sizeInBytes > ULLONG_MAX) {
        std::cerr << "Zineb: Value exceeds the limits of unsigned long long." << std::endl;
        exit(1);
    }

    server.setMaxClientBodySize(sizeInBytes);
    server.setmaxClientBodySizeSet(true);
}


void ConfigParser::savePort(Server &server, std::vector<std::string> &lines, size_t *i)
{
    if (!server.isPortSet())
    {
        std::string prefix = "    port:";
        std::string line = lines[*i];

        std::string value = line.substr(prefix.length());
        
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);
        
        if (value.empty() || value.find_first_not_of("0123456789") != std::string::npos) {
            std::cerr << "Zineb: Invalid port value." << std::endl;
            exit(1);
        }

        int p = std::atoi(value.c_str());
        if (p > USHRT_MAX)
        {
            std::cerr << "Value exceeds the limits of unsigned short!" << std::endl;
            exit(1);
        }
        server.setPort(p);
        server.setportSet(true);
    }
    else {
            std::cerr << "Zineb: Multiple port values specified." << std::endl;
            exit(1);
    }
}

void ConfigParser::saveHost(Server &server, std::vector<std::string> &lines, size_t *i) {
    if (!server.isHostSet())
    {
        std::string prefix = "    host:";
        std::string line = lines[*i];
        std::string value = line.substr(prefix.length());
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        std::transform(value.begin(), value.end(), value.begin(), ::toupper);
        if (value == "LOCALHOST")
            value = "127.0.0.1";
        if (value.empty()) {
            std::cerr << "Zineb: Invalid host value." << std::endl;
            exit(1);
        }

        std::istringstream iss(value);
        std::string segment;
        std::vector<std::string> segments;
        while (std::getline(iss, segment, '.')) {
            segments.push_back(segment);
        }

        if (segments.size() != 4) {
            std::cerr << "Zineb: Invalid host value. Must contain 4 segments separated by dots or localhost." << std::endl;
            exit(1);
        }

        size_t ip_address = 0;
        for (size_t i = 0; i < segments.size(); ++i) {
            const std::string& segment = segments[i];
            if (segment.empty() || segment.find_first_not_of("0123456789") != std::string::npos) {
                std::cerr << "Zineb: Invalid host value. Each segment must be a number." << std::endl;
                exit(1);
            }
            int num = std::atoi(segment.c_str());
            if (num < 0 || num > 255) {
                std::cerr << "Zineb: Invalid host value. Each segment must be between 0 and 255." << std::endl;
                exit(1);
            }
            ip_address = (ip_address * 256) + num;
        }

        server.setHost(value);
        server.sethostSet(true);
    }
    else {
        std::cerr << "Zineb: Multiple host values specified." << std::endl;
        exit(1);
    }
}

void ConfigParser::saveRoot(Server &server, std::vector<std::string> &lines, size_t *i)
{
    if (!server.isRootSet())
    {
        std::string prefix = "    root: ";
        std::string line = lines[*i];

        if (server.getRoot().find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890./_-") != std::string::npos) {
            std::cerr << "Zineb: Invalid characters detected after the root directory path!" << std::endl;
            exit(1);
        }
        server.setRoot(line.substr(prefix.length()));
        server.setrootSet(true);
    }
    else {
        std::cerr << "Zineb: Multiple root values specified!!!." << std::endl;
        exit(1);
    }
}

void ConfigParser::saveErrorPages(Server &server, std::vector<std::string> &lines, size_t *i) {
    std::string prefix = "    error_pages:";
    std::string line = lines[*i];
        
    (*i)++;
    line = lines[*i];

    while (line.find("      - ") == 0) {
        std::istringstream iss(line.substr(7));
        int errorCode;
        std::string pagePath;
        char colon;

        iss >> errorCode >> colon >> pagePath;

        if (iss.fail() || colon != ':' || errorCode < 100 || errorCode > 999 || pagePath.empty()) {
            std::cerr << "Zineb: Invalid format for error_pages entry." << std::endl;
            exit(1);
        }

        server.setErrorPages(errorCode, pagePath);
        server.seterrorPagesSet(true);
        (*i)++;
        line = lines[*i];
    }
}

std::vector<Server> &ConfigParser::getServers()
{
    return servers;
}