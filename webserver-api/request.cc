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
#include "request.h"


using namespace boost;
using namespace boost::system;
using namespace boost::asio;

class mime_types;

static unique_ptr<Request> Request::Parse(const std::string& raw_request)
{
    
    std::string line, headers, body;
    std::size_t line_found = raw_request.find("\r\n", 0);
    if (line_found != std::string::npos){
        line = raw_request.substr(0, line_found+1);
	rest = raw_request.substr(line_found+2);
    }
    std::size_t body_found = rest.find("\r\n\r\n", 0);
    if (body_found != std::string::npos){
        headers = rest.substr(0, body_found+3);
	body = rest.substr(body_found+4);
    }

    this->raw_request = raw_request;
    this->body = body;
    this->read_request_line(line);
    this->read_header(headers);
    
}

std::string Request::read_request_line(std::string line)
{
    //This is a check for what url the request wants.

    std::stringstream ssRequestLine(line);
    ssRequestLine >> this->method;
    ssRequestLine >> this->uri;
    ssRequestLine >> this->version;
    
    //std::cout << "request for resource: " << url << std::endl;
    return uri;
}

bool Request::read_header(std::string headers)
{
  std::string temp_headers = headers;
  std::string line;
  while(temp_headers.size() != 0)
  {
    std::size_t header_found = temp_headers.find("\r\n", 0);
    if (header_found != std::string::npos){
        line = temp_headers.substr(0, line_found+1);
	temp_headers = temp_headers.substr(line_found+2);
    }
    std::stringstream ssHeader(line);

    std::string headerName;
    std::getline(ssHeader, headerName, ':');
      
    std::string value;
    std::getline(ssHeader, value, '\r');
    
    std::pair<std::string, std::string> header_pair(headerName, value);
    this->headers_.pushback(header_pair);
  }
  return 1;
	
}

std::string Request::raw_request()
{
    return this->raw_request;
}

std::string Request::method()
{
    return this->method;
}

std::string Request::uri()
{
    return this->uri;
}

std::string Request::version()
{
    return this->version;
}

std::string Request::body()  
{
    return this->body;
}

Headers Request::headers()
{
    return this->headers_;
}


