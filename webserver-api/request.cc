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

std::unique_ptr<Request> Request::Parse(const std::string& raw_request)
{
    std::unique_ptr<Request> request(new Request());

    std::string line, headers, body, rest;
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

    request->raw_request_ = raw_request;
    request->body_ = body;
    request->read_request_line(line);
    request->read_header(headers);
    
    return request;
}

std::string Request::read_request_line(std::string line)
{
    //This is a check for what url the request wants.

    std::stringstream ssRequestLine(line);
    ssRequestLine >> this->method_;
    ssRequestLine >> this->uri_;
    ssRequestLine >> this->version_;
    
    //std::cout << "request for resource: " << url << std::endl;
    return this->uri_;
}

bool Request::read_header(std::string headers)
{
  std::string temp_headers = headers;
  std::string line;
  while(temp_headers.size() > 0 && temp_headers != "\r" && temp_headers != "\n")
  {
    std::size_t header_found = temp_headers.find("\r\n", 0);
    if (header_found != std::string::npos){
        line = temp_headers.substr(0, header_found+1);
	temp_headers = temp_headers.substr(header_found+2);
    }
    std::stringstream ssHeader(line);

    std::string headerName;
    std::getline(ssHeader, headerName, ':');
	
	//skip the white space
	ssHeader>>std::ws;
      
    std::string value;
    std::getline(ssHeader, value, '\r');
    
    std::pair<std::string, std::string> header_pair(headerName, value);
    this->headers_.push_back(header_pair);
  }
  return 1;
	
}

std::string Request::raw_request() const
{
    return this->raw_request_;
}

std::string Request::method() const
{
    return this->method_;
}

std::string Request::uri() const
{
    return this->uri_;
}

std::string Request::version() const
{
    return this->version_;
}

std::string Request::body() const
{
    return this->body_;
}

Headers Request::headers() const
{
    return this->headers_;
}


