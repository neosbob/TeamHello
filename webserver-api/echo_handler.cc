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
#include "echo_handler.h"

using namespace boost;
using namespace boost::system;
using namespace boost::asio;

class mime_types;

RequestHandler::Status EchoHandler::Init(const std::string& uri_prefix,
                      const NginxConfig& config)
{}

RequestHandler::Status EchoHandler::HandleRequest(const Request& request,
                               Response* response)
{
    response->SetStatus(Response::ok);
    response->AddHeader("content-type", "text/plain");
    response->AddHeader("content-length", std::to_string(request.raw_request().length()));
    response->SetBody(request.raw_request());
    return RequestHandler::OK;
}

