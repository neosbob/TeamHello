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
  "HTTP/1.1 200 OK\r\n";
  const std::string not_found =
  "HTTP/1.1 404 Not Found\r\n";
  const std::string int_serv_err =
  "HTTP/1.1 500 Internal Server Error\r\n";
  
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
    response_code_ = response_code;
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
        response << response_h.first << ": " << response_h.second << "\r\n";
    }
    response << "\r\n";
    response << this->content;
    

    return response.str();
}

char Response::space = ' ';
std::string Response::header_separator = "\r\n";

std::vector<std::pair<std::string, std::string>> Response::GetHeaders() {
    return response_header;
}

Response::ResponseCode Response::GetResponseCode() {
    return response_code_;
}

std::string Response::GetBody() {
    return content;
}

std::unique_ptr<Response> Response::ParseHeader(std::string header) {
    auto response = std::unique_ptr<Response>(new Response());
    size_t pos = 0, prev = 0;

    // separate status line and headers
    pos = header.find(header_separator);
    std::string status_line = header.substr(0, pos);
    header = header.substr(pos+2);

    // parse status line
    pos = status_line.find(space, prev);
    prev = pos + 1;
    pos = status_line.find(space, prev);
    response->SetStatus(static_cast<ResponseCode >(std::stoi(status_line.substr(prev, pos-prev))));

    // parse headers
    prev = 0;
    size_t mid;
    std::string line, head_, content_;
    while ((pos = header.find(header_separator, prev)) != std::string::npos) {
        line = header.substr(prev, pos-prev);
        if (line.length() == 0) break;
        prev = pos + header_separator.length();
        mid = line.find(":");
        head_ = line.substr(0, mid);
        content_ = line.substr(mid+1);
        content_ = content_.substr(content_.find_first_not_of(' '));
        response->AddHeader(head_, content_);
    }

    return response;
}