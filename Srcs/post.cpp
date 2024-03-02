#include "../Includes/Client.hpp"


void Client::open_file()
{
    
 
        std::map<std::string, std::string >::iterator it;
        it = headers.find("Content-Type");
        if( it != headers.end())
        {   
            value_type  = it->second;
        }
    // compare // bouandry 
    //    int result = str1.compare(0, 6, str2, 0, 6);
    // if(value_type.compare("multipart/form-data") ==  0)// kayna
    // {
    //     std::string form_data = it->second;
    // }
    store_type();
    std::string extension;
    std::map<std::string, std::string>::iterator at;
    at = mime_type.find(value_type);
    if( at != mime_type.end())
    {
        extension = at->second;
    }
    std::string name;
    if (!flag_open)
    {
        name_file += "t";
        name = "Srcs/upload/" + name_file + extension;  
        file.open(name.c_str(), std::ios::app);
        flag_open = true;
    }
}

void Client::post()
{
    std::map<std::string, std::string >::iterator it;
    it = headers.find("Transfer-Encoding");
    if( it != headers.end())
    {   
        transfer_encoding  = it->second;
    }
    //chunked
    if(transfer_encoding == "chunked")
    {
        if(store_hexSize > body.size())
        {
           
            if(save.size() <= hexSize)
            {
                save += body;
               
            }
           
            // std::cout << "....." << save.size() << std::endl;
            // std::cout << "....." << hexSize << "\n";
            
            if(save.size() > hexSize)
            {
                
                store = save.substr(0, hexSize);
                file.write(store.c_str(), store.size());
                std::string sub2 = save.substr(hexSize);
                // std::cout << "---------------------------------------------------------------------\n";
                // std::cout << sub2 << "\n";
                // std::cout << "---------------------------------------------------------------------\n";

                size_t posa1 = sub2.find("\r\n");
                size_t posa2;
                if(posa1 != std::string::npos)
                {
                    posa2 = sub2.find("\r\n", posa1 + 2 );
                    if(posa2 != std::string::npos)
                    {
                        shinka = sub2.substr(posa1 + 2, posa2 - posa1 - 2);
                        hexSize = hexa_to_dec(shinka);
                    }
                        sab = sub2.substr(posa2 + 2);
                        save.clear();
                        save += sab;

                    // std::cout << "*********************************************************************************\n";
                    // std::cout << sab << "\n";
                    // std::cout << "*********************************************************************************\n";
                }
                else 
                {
                    save.clear();
                    save += sub2;
                }
            }
            
            bodySize += body.size();
            if(bodySize >= (size_t)atoi(headers["Content-Length"].c_str()))
            {
                file.write(body.c_str(), body.size());
                flag_in_out = true;
                file.close();
                return;
            }
        
        }
        else 
        {
            bodySize += body.size();
            if(body.find("\r\n0\r\n") != std::string::npos)
            {
                std::string put = body.substr(0,body.find("\r\n0\r\n"));
                body.clear();
                body = put;
            }
                if(bodySize >= (size_t)atoi(headers["Content-Length"].c_str()))
            {
            
                file.write(body.c_str(), body.size());
                flag_in_out = true;
                file.close();
                return;
            }
        }
    }
    else // binary
    { 
        bodySize += body.size();
        file.write(body.c_str(), body.size());
        if(bodySize   >= (size_t)atoi(headers["Content-Length"].c_str()))
            {
                
                file.write(body.c_str(), body.size());
                flag_in_out = true;
                file.close();
                return;
            }

            
    }
}
