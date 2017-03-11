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
#include "cpp-markdown/markdown.h"

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
    std::string full_path;
    std::size_t url_found = uri_path.find(this->prefix, 0);
    if (url_found != std::string::npos){
        //static_path = uri_path.substr(0,url_found);
	request_path = uri_path.erase(url_found, this->prefix.length());
        //std::cout<< request_path;
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
      if(request_path.substr(0,1) != "/")
          request_path = "/" + request_path;
      full_path = this->root_dir + request_path;
      std::cout<< full_path;
      std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
      if (!is)
      {
         return RequestHandler::Status::FILE_NOT_FOUND; 
      }

      // Fill out the reply to be sent to the client.
      std::string content = "";
      mime_types mime;
      char buf[512];
      while (is.read(buf, sizeof(buf)).gcount() > 0)
          content.append(buf, is.gcount());

      response->SetStatus(Response::ok);
			//get the content type
			std::string extension_type = mime.extension_to_type(extension);

			//check if the content type is markdown. set to html
			if (extension_type == "text/markdown")
			{
				response->AddHeader("content-type", "text/html");
			}
			else
			{
      	response->AddHeader("content-type", extension_type);
      	response->AddHeader("content-length", std::to_string(content.length()));
			}

			//set response body
			if (extension_type == "text/markdown")
			{
				markdown::Document doc;
				doc.read(content);
				std::ostringstream stream;
				doc.write(stream);

				std::string markdown = stream.str();

				

				response->AddHeader("content-length",std::to_string(markdown.length()));
				response->SetBody(markdown);
			}
			else
			{
				response->SetBody(content);
			}
			
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

