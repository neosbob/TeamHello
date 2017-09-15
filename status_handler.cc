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
#include "status_handler.h"
#include "webserver.h"

using namespace boost;
using namespace boost::system;
using namespace boost::asio;

class mime_types;

RequestHandler::Status StatusHandler::Init(const std::string& uri_prefix,
                      const NginxConfig& config)
{
    return RequestHandler::Status::OK;
}

RequestHandler::Status StatusHandler::HandleRequest(const Request& request,
                               Response* response)
{
    std::string body = "";
    Server* server = Server::getServerInstance();
    if (!server)
    {
        return RequestHandler::FAILED;
    }
    std::map<std::string, int> url_requested_count, response_code_count;
    server->getStats(url_requested_count, response_code_count);
    std::map<std::string, std::vector<std::string> > uri_prefix2request_handler_name = server->getUriPrefixRequestHandlerMap();
    body += "Some statistics about this server: \n";
    body += "url requested count: \n";
    std::map<std::string, int>::iterator it = url_requested_count.begin();
    if (it == url_requested_count.end())
    {
        body += "no requests so far\n";
    }
    for (; it != url_requested_count.end(); ++it)
    {
        body = body + it->first + ": " + std::to_string(it->second) + "\n";
    }
    body += "response code count: \n";
    std::map<std::string, int>::iterator it2 = response_code_count.begin();
    if (it2 == response_code_count.end())
    {
        body += "no requests so far\n";
    }
    for (; it2 != response_code_count.end(); ++it2)
    {
        body = body + it2->first + ": " + std::to_string(it2->second) + "\n";
    }
    body += "\nThe request handlers exist: \n";
    for (std::map<std::string, std::vector<std::string> >::iterator it3 = uri_prefix2request_handler_name.begin(); it3 != uri_prefix2request_handler_name.end(); ++it3)
    {
        body = body + it3->first + " has uri prefixes " + it3->second[0];
        for (int i = 1; i < it3->second.size(); i++)
        {
            body = body + ", " + it3->second[i];
        }
        body += ".\n";
    }
    response->SetStatus(Response::ok);
    response->AddHeader("content-type", "text/plain");
    response->AddHeader("content-length", std::to_string(body.size()));
    response->SetBody(body);
    
    return RequestHandler::OK;
}

