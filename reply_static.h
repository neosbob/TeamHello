#ifndef REPLY_STATIC_H
#define REPLY_STATIC_H

#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <utility>
#include "mime_types.h"
#include "reply.h"

using namespace boost;
using namespace boost::system;
using namespace boost::asio;

class reply_static: public http_headers
{

public:
   
    reply_static() : http_headers()
    {
    }

    virtual std::string get_response(std::string echoback, std::string base_path);
   
   
};

#endif // REPLY_H
