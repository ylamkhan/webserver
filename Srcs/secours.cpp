// if(method == "POST")
//     {
//         std::map<std::string, std::string >::iterator it;
//         it = headers.find("Transfer-Encoding");
//         if( it != headers.end())
//         {   
//             transfer_encoding  = it->second;
//         }
//         //chunked
//         if(transfer_encoding == "chunked")
//         {
            
//             if(hexSize > body.size())
//             { 
//                 if(body.find(ss) != std::string::npos)
//                 {
//                     std::string sub = findSubString(body);
//                     if(!sub.empty())
//                     {
//                         ss.clear();
//                         ss = sub;
//                     }
//                     std::string strime =fonction_to_strime(body, ss);
//                     body.clear();
//                     body = strime;
//                 }
                
//                 if(body.find("\r\n0\r\n") != std::string::npos)
//                 {
//                     std::string put = body.substr(0,body.find("\r\n0\r\n"));
//                     body.clear();
//                     body = put;
//                 }
//                 file.write(body.c_str(), body.size());
//                 bodySize += body.size();
//                 if(bodySize >= (size_t)atoi(headers["Content-Length"].c_str()))
//                 {
                
//                     file.write(body.c_str(), body.size());
//                     flag_in_out = true;
//                     file.close();
//                     return;
//                 }
//             }
//             else 
//             {
//                 bodySize += body.size();
//                 if(body.find("\r\n0\r\n") != std::string::npos)
//                 {
//                     std::string put = body.substr(0,body.find("\r\n0\r\n"));
//                     body.clear();
//                     body = put;
//                 }
//                  if(bodySize >= (size_t)atoi(headers["Content-Length"].c_str()))
//                 {
                
//                     file.write(body.c_str(), body.size());
//                     flag_in_out = true;
//                     file.close();
//                     return;
//                 }
//             }
//         }
//         else // binary
//         { 
//             bodySize += body.size();
//             file.write(body.c_str(), body.size());
//             if(bodySize   >= (size_t)atoi(headers["Content-Length"].c_str()))
//                 {
                    
//                     file.write(body.c_str(), body.size());
//                     flag_in_out = true;
//                     file.close();
//                     return;
//                 }

              
//         }