// #include "../Includes/httpRequest.hpp"
// #include <sstream>
// #include <iostream>
// #include <fstream>


// Request::Request(): headerSet(false)
// {
   
//     //erros
// }

// Request::Request(Request const &other)
// {
//     *this = other;

// }

// Request &Request::operator=(const Request &other)
// {
//     if(&other != this)
//     {
//         method = other.method;
//         path = other.path;
//         headers = other.headers;
//         body   = other.body;
//         requestStr = other.requestStr;
//         headerSet = other.headerSet;
        
//     }
//     return *this;
// }

// Request::~Request()
// {
//     method.clear();
//     path.clear();
//     body.clear();
//     headers.clear();
// }

// // void    Request::parseRequest(const std::string& httpRequest)
// // {
// //     if (!headerSet)
// //         setReqStr(httpRequest);
// //     if (!headerSet && requestStr.find("\r\n\r\n", 0) != std::string::npos)
// //     {
// //         size_t pos = 0;
// //         size_t end = 0;
// //         end = requestStr.find(' ');
// //         method = requestStr.substr(pos, end - pos);
// //         pos = end + 1;
// //         end = requestStr.find(' ', pos);
// //         path = requestStr.substr(pos, end - pos);
// //         pos = end + 1;
// //         end = requestStr.find("\r\n", pos);
// //         httpVersion = requestStr.substr(pos, end - pos);
// //         pos = end + 2;
// //         while ((end = requestStr.find("\r\n", pos)) != std::string::npos)
// //         {
// //             std::string line = requestStr.substr(pos, end - pos);
// //             if (line.empty())
// //                 break;
// //             size_t separatorPos = line.find(':');
// //             if (separatorPos != std::string::npos) {
// //                 std::string headerName = line.substr(0, separatorPos);
// //                 std::string headerValue = line.substr(separatorPos + 2);
// //                 headers.push_back(std::make_pair(headerName, headerValue));
// //             }
// //             pos = end + 2;
// //         }
// //         body = requestStr.substr(pos);
// //         headerSet = true;
// //     }
// //     else /*if (headerSet)*/{
// // //         body = httpRequest;
// // //     }
// // //     handl_methodes();
// // // }
// // // void   Request::handl_methodes()
// // // {
    
// // //     // if(method == "GET")
// // //     // {
        
// // //     // }
// // //     //----clinet------
// // //     // if (flagopen)
// // //     // {
// // //     //     openFIle(path);
// // //     // }
// // //     // --------------------------
// // //     // rondom name /upload/ + name + mp4 
// // //     //

// // //     //  if(method == "POST")
// // //     // {
// // //         //std::cout << body << "\n";
// // //         //if(!body.empty())
// // //         out_file.write(body.c_str(), body.size());
       
// // //     // }
// // //     // else if (method == "DELETE")
// // //     // {
        
// // //     // }
// // //     // else
// // //     // {
// // //     //     std::cerr<<"Error: not found method\n";
// // //     //     return ;
// // //     // }
// // // }

// // std::string Request::getMethod() const { 
// //     return method;
// // }

// // std::string Request::getPath() const {
// //     return path;
// // }

// // std::string Request::getHTTPVersion() const {
// //     return httpVersion;
// // }

// // std::vector<std::pair<std::string, std::string> > Request::getHeaders() const {
// //     return headers;
// // }

// // std::string Request::getBody() const {
// //     return body;
// // }


// // std::string Request::getReqStr() const {
// //     return requestStr;
// // }

// // void Request::setReqStr(std::string s) {
// //     requestStr += s;
// // }

