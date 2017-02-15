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
#include "request_handler.h"


using namespace boost;
using namespace boost::system;
using namespace boost::asio;

class mime_types;   
/*
std::string http_headers::get_response(std::string echoback)
   {
      std::stringstream ssOut;
      //echoes back the request sent by the client
      if(url == "/")
      {
         std::string sHTML = echoback;
         ssOut << "HTTP/1.1 200 OK" << std::endl;
         ssOut << "content-type: text/plain" << std::endl;
         ssOut << "content-length: " << sHTML.length() << std::endl;
         ssOut << std::endl; //\r\n\r\n signals for end of response header.
         //Below is the body of the response.
         ssOut << sHTML;
	 return ssOut.str();
      }
      std::string request_path;
      request_path = url;
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
      std::string full_path = "/home/bob" + request_path;
      std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
      if (!is)
      {
         std::string sHTML = "<html><body><p>Image not found</p></body></html>";
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
   */
int http_headers::content_length()
   {
      auto request = headers.find("content-length");
      //This only applies if a header of "content-length" is specified
      //If found, don't enter if statement.
      if(request == headers.end())
          return 0;
      else if(request != headers.end())
      {
         std::stringstream ssLength(request->second);
         int content_length;
         ssLength >> content_length;
         return content_length;
      }
   }
   
std::string http_headers::read_header(std::string line)
   {  
      //only applies for lines after the first line
      //Maps specific headers to a certain value
      //Ex. Host: localhost. Host maps to localhost.
    
      std::stringstream ssHeader(line);
      std::string headerName;
      std::getline(ssHeader, headerName, ':');
      
      std::string value;
      std::getline(ssHeader, value);
      headers[headerName] = value;
	
	return headerName+":"+value;	
   }
   
std::string http_headers::read_request_line(std::string line)
   {
      //This is a check for what url the request wants.

      std::stringstream ssRequestLine(line);
      ssRequestLine >> method;
      ssRequestLine >> url;
      ssRequestLine >> version;
      
      std::cout << "request for resource: " << url << std::endl;
      return url;
   }



bool http_headers::setMap(std::map<std::string, std::string> m)
{
	headers=m;
	if (headers.empty()) 
		return false;
	else
		return true;
}
