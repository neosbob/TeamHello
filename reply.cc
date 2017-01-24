//Inspired by minimal server design by phillipvoyle
//https://www.codeproject.com/Articles/1007262/A-minimal-http-web-server-using-boost-asio#_comments

#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <utility>
#include "reply.h"

using namespace boost;
using namespace boost::system;
using namespace boost::asio;

   
std::string http_headers::get_response(std::string echoback)
   {
      std::stringstream ssOut;

      //echoes back the request sent by the client
      //if(url == "/")
      //{

         std::string sHTML = echoback;
         ssOut << "HTTP/1.1 200 OK" << std::endl;
         ssOut << "content-type: text/plain" << std::endl;
         ssOut << "content-length: " << sHTML.length() << std::endl;
         ssOut << std::endl;
         ssOut << sHTML;
      //}
/*
      else
      {
         std::string sHTML = "<html><body><p>There's nothing here.</p></body></html>";
         ssOut << "HTTP/1.1 404 Not Found" << std::endl;
         ssOut << "content-type: text/plain" << std::endl;
         ssOut << "content-length: " << sHTML.length() << std::endl;
         ssOut << std::endl;
         ssOut << sHTML;
      }*/
      return ssOut.str();
   }
   
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
   
void http_headers::read_header(std::string line)
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
   }
   
void http_headers::read_request_line(std::string line)
   {
      //This is a check for what url the request wants.

      std::stringstream ssRequestLine(line);
      ssRequestLine >> method;
      ssRequestLine >> url;
      ssRequestLine >> version;
      
      std::cout << "request for resource: " << url << std::endl;
   }
