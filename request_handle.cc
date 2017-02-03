//Inspired by Boost example for async_client.cpp and its use of async_read_until
//www.boost.org/doc/libs/1_43_0/doc/html/boost_asio/example/http/client/async_client.cpp

#include <boost/asio.hpp>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <utility>
#include "request_handle.h"

using namespace boost;
using namespace boost::system;
using namespace boost::asio;


std::string session::buildResponseStr(const std::string& body)
{
    std::stringstream ssOut;

      //echoes back the request sent by the client
      //if(url == "/")
      //{

         std::string sHTML = body;
         ssOut << "HTTP/1.1 200 OK" << std::endl;
         ssOut << "content-type: text/plain" << std::endl;
         ssOut << "content-length: " << sHTML.length() << std::endl;
         ssOut << std::endl; //\r\n\r\n signals for end of response header.
         //Below is the body of the response.
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
