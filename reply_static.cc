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
#include "reply.h"
#include "reply_static.h"

using namespace boost;
using namespace boost::system;
using namespace boost::asio;

class mime_types;   

std::string reply_static::get_response(std::string echoback, std::string base_path)
   {
      std::stringstream ssOut;

    std::string request_path, request_static;
    if(url.size() > 7){
      request_static = url.substr(0,8);
      request_path = url.substr(7);
    }

    //TODO: Errorchecking for empty file url.
      
    if (request_static == "/static/") {
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
      std::string full_path = base_path + request_path;
      std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
      if (!is)
      {
         std::string sHTML = "<html><body><p>File not found, can't open.</p></body></html>";
         ssOut << "HTTP/1.1 404 Not Found" << std::endl;
         ssOut << "content-type: text/html" << std::endl;
         ssOut << "content-length: " << sHTML.length() << std::endl;
         ssOut << std::endl;
         ssOut << sHTML;

	 return ssOut.str();
      }

      // Fill out the reply to be sent to the client.
      std::string content = "";
      mime_types mime;
      char buf[512];
      while (is.read(buf, sizeof(buf)).gcount() > 0)
          content.append(buf, is.gcount());

      ssOut << "HTTP/1.1 200 OK" << std::endl;
      ssOut << "content-type: " << mime.extension_to_type(extension) << std::endl;
      ssOut << "content-length: " << content.length() << std::endl;
      ssOut << std::endl;
      ssOut << content;

      return ssOut.str();
    }

    else {
	std::string sHTML = "<html><body><p>URL does not start with \"/static/\" </p></body></html>";
        ssOut << "HTTP/1.1 404 Not Found" << std::endl;
        ssOut << "content-type: text/html" << std::endl;
        ssOut << "content-length: " << sHTML.length() << std::endl;
        ssOut << std::endl;
        ssOut << sHTML;

	return ssOut.str();
    }
   }
   

