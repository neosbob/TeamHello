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
#include "reply_echo.h"


using namespace boost;
using namespace boost::system;
using namespace boost::asio;

class mime_types;   

std::string reply_echo::get_response(std::string echoback, std::string base_path)
   {
      std::stringstream ssOut;

      //echoes back the request sent by the client
      if(url == "/echo")
      {

         std::string sHTML = echoback;
         ssOut << "HTTP/1.1 200 OK" << std::endl;
         ssOut << "content-type: text/plain" << std::endl;
         ssOut << "content-length: " << sHTML.length() << std::endl;
         ssOut << std::endl; //\r\n\r\n signals for end of response header.
         //Below is the body of the response.
         ssOut << sHTML;
      }

      else
      {
         std::string sHTML = "<html><body><p>To use echo, please specify http://localhost:8080/echo</p></body></html>";
         ssOut << "HTTP/1.1 404 Not Found" << std::endl;
         ssOut << "content-type: text/plain" << std::endl;
         ssOut << "content-length: " << sHTML.length() << std::endl;
         ssOut << std::endl;
         ssOut << sHTML;
      }
      return ssOut.str();
   }
   

