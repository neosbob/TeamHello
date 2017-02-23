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
#include "file_handler.h"
#include "not_found_handler.h"

using namespace boost;
using namespace boost::system;
using namespace boost::asio;

class mime_types;

RequestHandler::Status StaticHandler::Init(const std::string& uri_prefix,
                                           const NginxConfig& config)
{   
    if (config.statements_[0]->tokens_[0] == "root" && config.statements_[0]->tokens_.size() == 2)
    {
        this->root_dir = config.statements_[0]->tokens_[1];
        this->prefix = uri_prefix;
        return RequestHandler::Status::OK;
    }

    return RequestHandler::Status::FAILED;
    
}

RequestHandler::Status StaticHandler::HandleRequest(const Request& request,
                                                    Response* response)
{
    std::string uri_path = request.uri();

    std::string request_path, static_path, sfile_dir;
    std::size_t url_found = uri_path.find("/", 1);
    if (url_found != std::string::npos){
        static_path = uri_path.substr(0,url_found);
	request_path = uri_path.substr(url_found);
    }
	       

    //TODO: Errorchecking for empty file url.
      
    if (request_path != "/" && request_path != "") {
      // Determine the file extension.
      std::size_t last_slash_pos = request_path.find_last_of("/");
      std::size_t last_dot_pos = request_path.find_last_of(".");
      std::string extension;
      if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
      {
         extension = request_path.substr(last_dot_pos + 1);
      }
	std::cout << "resource path: " << request_path << std::endl;
      // Open the file to send back.
      std::string full_path = this->root_dir + request_path;
      std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
      if (!is)
      {
         NotFoundHandler not_found_handler;
         return not_found_handler.HandleRequest(request, response);
      }

      // Fill out the reply to be sent to the client.
      std::string content = "";
      mime_types mime;
      char buf[512];
      while (is.read(buf, sizeof(buf)).gcount() > 0)
          content.append(buf, is.gcount());

      response->SetStatus(Response::ok);
      response->AddHeader("content-type", mime.extension_to_type(extension));
      response->AddHeader("content-length", std::to_string(content.length()));
      response->SetBody(content);
      return RequestHandler::Status::OK;
    }

    else if (request_path == "/")
    {
	return RequestHandler::Status::FILE_NOT_FOUND;     
    }

    else {
	return RequestHandler::Status::FILE_NOT_FOUND;
    }

}

