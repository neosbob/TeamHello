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
#include "configTestingHandler.h"
#include "webserver.h"

using namespace boost;
using namespace boost::system;
using namespace boost::asio;

class mime_types;

RequestHandler::Status ConfigTestingHandler::Init(const std::string& uri_prefix, const NginxConfig& config)
{
    return RequestHandler::Status::OK;
}

RequestHandler::Status ConfigTestingHandler::HandleRequest(const Request& request, Response* response)
{
    std::string body = "";
    Server* server = Server::getServerInstance();
    if (!server)
    {
        return RequestHandler::FAILED;
    }
    std::string configFileName = server->getConfigFileName();
    std::string append = "path /static3 StaticHandler {root ./test_folder;}";
    std::ofstream config(configFileName, std::ios::app | std::ios::out);
    config << append;
    config.close();
    body += "Config changed successfully!\n";
    response->SetStatus(Response::ok);
    response->AddHeader("content-type", "text/plain");
    response->AddHeader("content-length", std::to_string(body.size()));
    response->SetBody(body);
    
    return RequestHandler::OK;
}

