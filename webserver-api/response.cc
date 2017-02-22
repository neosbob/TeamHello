//Inspired by minimal server design by phillipvoyle
//https://www.codeproject.com/Articles/1007262/A-minimal-http-web-server-using-boost-asio#_comments

#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <utility>
#include "response.h"

using namespace boost;
using namespace boost::system;
using namespace boost::asio;

class mime_types;

namespace status_string 
{  
  const std::string ok =
  "HTTP/1.0 200 OK\r\n";
  const std::string not_found =
  "HTTP/1.0 404 Not Found\r\n";
  const std::string int_serv_err =
  "HTTP/1.0 500 Internal Server Error\r\n";
  
}

void Response::SetStatus(const ResponseCode response_code)
{
    switch(response_code)
    {
        case Response::ok:
            this->response_status_string = status_string::ok;
            break;
        case Response::not_found:
            this->response_status_string = status_string::not_found;
            break;
        case Response::internal_server_error:
            this->response_status_string = status_string::int_serv_err;
            break;
    }
}
void Response::AddHeader(const std::string& header_name, const std::string& header_value)
{
    this->response_header.push_back(std::make_pair(header_name, header_value));
}
void Response::SetBody(const std::string& body)
{
    this->content = body;
}

std::string Response::ToString()
{
    std::stringstream response;
    response << this->response_status_string;
    for(auto response_h : this->response_header)
    {
        response << response_h.first << ": " << response_h.second << "/r/n";
    }
    response << "/r/n";
    response << this->content;
    

    return response.str();
}
