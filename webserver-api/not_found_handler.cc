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
#include "not_found_handler.h"

using namespace boost;
using namespace boost::system;
using namespace boost::asio;

class mime_types;

RequestHandler::Status NotFoundHandler::Init(const std::string& uri_prefix,
                      const NginxConfig& config)
{}

RequestHandler::Status NotFoundHandler::HandleRequest(const Request& request,
                               Response* response)
{
    response->SetStatus(Response::not_found);
    response->AddHeader("content-type", "text/html");
    response->AddHeader("content-length", std::to_string(this->reason_response.length()));
    response->SetBody(this->reason_response);
    return RequestHandler::OK;
}

