#ifndef HTTP_REQUEST_HANDLER_H
#define HTTP_REQUEST_HANDLER_H

#include <boost/asio.hpp>
#include <boost/array.hpp>
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
   //reply_static re_static;
   //reply_echo re_echo;
   boost::array<char,8192> buffer_;
   std::map<std::string, RequestHandler*> handlers;
   RequestHandler* default_handler;

   RequestHandler* GetRequestHandler(const std::string& path);

public:
   
   static void read_whole_request(std::shared_ptr<session> pThis);

   std::string write_response(Response& response, std::shared_ptr<session> pThis);

   char data[4096];

   size_t length;

   std::string ss = "";

   std::string echo_map, static_map;

   std::map<std::string, std::string> base_path;

   ip::tcp::socket socket;
   
   session(io_service& io_service, std::map<std::string, RequestHandler*> mapping, RequestHandler* not_found)
      :socket(io_service), handlers(mapping), default_handler(not_found)
   {}
   
   static void read_request(std::shared_ptr<session> pThis);
};


#endif // HTTP_REQUEST_HANDLER_H
