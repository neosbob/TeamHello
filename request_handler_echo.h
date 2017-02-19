#ifndef REPLY_ECHO_H
#define REPLY_ECHO_H

#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <utility>
#include "mime_types.h"
#include "request_handler.h"


using namespace boost;
using namespace boost::system;
using namespace boost::asio;

class reply_echo : public request_handler
{

public:
   
   reply_echo() : request_handler (){}

   virtual std::string get_response(std::string echoback, std::string base_path);


   
};

#endif // REPLY_ECHO_H
