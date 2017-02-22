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
      asio::async_read(pThis->socket, pThis->buff, [pThis](const error_code& e, std::size_t s)
      {
          Response response;

          std::istream stream {&pThis->buff};

          std::string buffer = std::string(std::istreambuf_iterator<char>(stream), {});
          auto request = Request::Parse(buffer);

          RequestHandler* handler = pThis->GetRequestHandler(request->uri());

	  if(handler != nullptr) {
	      RequestHandler::Status response_status = handler->HandleRequest(*request, &response);
          }       
      
          
          pThis->write_response(response, pThis);
      });
}

std::string session::write_response(Response& response, std::shared_ptr<session> pThis)
{
    std::string response_string = response.ToString();
    asio::async_write(pThis->socket, boost::asio::buffer(response_string, response_string.length()), [pThis](const error_code& e, std::size_t s)
    {
          //return str->c_str();
    });

    return response_string;
}

void session::read_request(std::shared_ptr<session> pThis)
{
    pThis->read_whole_request(pThis);
}

RequestHandler* session::GetRequestHandler(const std::string& path)
{
    for(auto& handlerPair: handlers) {
	std::size_t second_slash_pos = path.find("/", 1);
	std::string search_path = path.substr(0, second_slash_pos);
	
	if ( search_path == handlerPair.first) {
            return handlerPair.second;
        }
    }

    return nullptr;

}

