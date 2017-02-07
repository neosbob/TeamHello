#ifndef HTTP_REQUEST_HANDLER_H
#define HTTP_REQUEST_HANDLER_H

#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <utility>
#include "reply.h"
#include "reply_echo.h"
#include "reply_static.h"

using namespace boost;
using namespace boost::system;
using namespace boost::asio;


/// The common handler for all incoming requests.

class session
{
   asio::streambuf buff;
   reply_static re_static;
   reply_echo re_echo;

public:
   
   static void read_body(std::shared_ptr<session> pThis);
   
   static void read_next_line(std::shared_ptr<session> pThis);
   
   static void read_first_line(std::shared_ptr<session> pThis);
   
   std::string read_first_line_text(std::istream & stream, std::string & line, std::string & ignore);

   std::string read_next_line_text(std::istream & stream, std::string & line, std::string & ignore);

   char data[4096];

   size_t length;

   std::string ss = "";

   std::string base_path, echo_map, static_map;

   ip::tcp::socket socket;
   
   session(io_service& io_service, std::string base_dir, std::string echo_path, std::string static_path)
      :socket(io_service)
   {
	this->base_path = base_dir;
	this->echo_map = echo_path;
	this->static_map = static_path;
	
   }
   
   static void read_request(std::shared_ptr<session> pThis);
};


#endif // HTTP_REQUEST_HANDLER_H
