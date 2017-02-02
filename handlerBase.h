#ifndef HANDLERBASE_H
#define HANDLERBASE_H

#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <utility>
#include "reply.h"

class HandlerBase
{
public:
    HandlerBase(boost::asio::io_service& io_service)
      : ss("")
      , socket(io_service)
    {
    }
   
    static void read_first_line(std::shared_ptr<HandlerBase> pThis);
   
    static void read_next_line(std::shared_ptr<HandlerBase> pThis);
    
    static void read_request(std::shared_ptr<HandlerBase> pThis);
    
    virtual std::string buildResponseStr(std::string body) = 0;
    
    std::string read_first_line_text(std::istream & stream, std::string & line, std::string & ignore);

    std::string read_next_line_text(std::istream & stream, std::string & line, std::string & ignore);
    
    static void read_body(std::shared_ptr<HandlerBase> pThis);

    size_t length;

    std::string ss;
    
    boost::asio::streambuf buff;
    
    http_headers headers;

    boost::asio::ip::tcp::socket socket;
    
private:    
    
};


#endif  // #HANDLERBASE_H
