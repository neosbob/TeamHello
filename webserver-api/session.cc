//Inspired by Boost example for async_client.cpp and its use of async_read_until
//www.boost.org/doc/libs/1_43_0/doc/html/boost_asio/example/http/client/async_client.cpp

#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <utility>
#include "session.h"

using namespace boost;
using namespace boost::system;
using namespace boost::asio;

void session::read_whole_request(std::shared_ptr<session> pThis)
{
      asio::async_read(pThis->socket, boost::asio::buffer(buffer_), [pThis](const error_code& e, std::size_t s)
      {
          Response response;

          std::string buffer = std::string(buffer_, s);
          auto request = Request::Parse(buffer);

          request.uri();
          
          RequestHandler::Status response_status = HandleRequest(*request, &response);
          write_response(response);
      });
}

std::string session::write_response(Response& response)
{
    std::string response_string = response.ToString();
    asio::async_write(pThis->socket, boost::asio::buffer(response_string, response_string.length()), [pThis, str](const error_code& e, std::size_t s)
    {
          
    });

    return response_string;
}

void session::read_request(std::shared_ptr<session> pThis)
{
    pThis->read_whole_request(pThis);
}
