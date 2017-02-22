#ifndef HTTP_REQUEST_HANDLER_H
#define HTTP_REQUEST_HANDLER_H

#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <utility>
#include "request_handler.h"
#include "echo_handler.h"
#include "file_handler.h"
#include "not_found_handler.h"
#include "request.h"
#include "response.h"

using namespace boost;
using namespace boost::system;
using namespace boost::asio;


/// The common handler for all incoming requests.

class session
{
   asio::streambuf buff;
   reply_static re_static;
   reply_echo re_echo;
   boost::array<char,8192> buffer_;

public:
   
   static void read_whole_request(std::shared_ptr<session> pThis);

   std::string write_response(Response& response);

   char data[4096];

   size_t length;

   std::string ss = "";

   std::string echo_map, static_map;

   std::map<std::string, std::string> base_path;

   ip::tcp::socket socket;
   
   session(io_service& io_service, std::map<std::string,std::string> base_dir, std::string echo_path, std::string static_path)
      :socket(io_service)
   {
	this->base_path = base_dir;
	this->echo_map = echo_path;
	this->static_map = static_path;
	
   }
   
   static void read_request(std::shared_ptr<session> pThis);
};


#endif // HTTP_REQUEST_HANDLER_H
