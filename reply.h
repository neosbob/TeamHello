#ifndef REPLY_H
#define REPLY_H

#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <utility>
#include "mime_types.h"


using namespace boost;
using namespace boost::system;
using namespace boost::asio;

class http_headers
{
   std::string method;
   
   std::string version;
   
   std::map<std::string, std::string> headers;

public:
   std::string url;
   
   virtual std::string get_response(std::string echoback, std::string base_path) = 0;
   
   int content_length();
   
   std::string read_header(std::string line);
   
   std::string read_request_line(std::string line);

   bool setMap(std::map<std::string, std::string> m);

   
};

#endif // REPLY_H
