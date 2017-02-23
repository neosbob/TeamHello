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
      pThis->socket.async_read_some(boost::asio::buffer(pThis->data), [pThis](const error_code& e, std::size_t s)
      {
          Response response;

          std::string buffer = std::string(pThis->data, s);
          //std::cout<< buffer;
          auto request = Request::Parse(buffer);
	  //std::cout << request->uri()<< std::endl;
          RequestHandler* handler = pThis->GetRequestHandler(request->uri());
	  
	  if(handler != nullptr) {
	      RequestHandler::Status response_status = handler->HandleRequest(*request, &response);
	      if (response_status == RequestHandler::Status::FAILED)
	      {
                  std::string int_serv_error = "Error Status 500.";
	          response.SetStatus(Response::internal_server_error);
                  response.AddHeader("content-type", "text/plain");
                  response.AddHeader("content-length", std::to_string(int_serv_error.length()));
                  response.SetBody(int_serv_error);
              }
	      else if (response_status == RequestHandler::Status::FILE_NOT_FOUND)
              {
                  handler = pThis->default_handler;
                  RequestHandler::Status response_status = handler->HandleRequest(*request, &response);
              }
              //std::cout << "hi";
          }
	  else if (handler == nullptr) {
	      handler = pThis->default_handler;
	      RequestHandler::Status response_status = handler->HandleRequest(*request, &response);
          }    
          
          pThis->write_response(response, pThis);

          //delete handler;
          //request.reset(nullptr);
      });
}

std::string session::write_response(Response& response, std::shared_ptr<session> pThis)
{
    std::shared_ptr<std::string> response_string;
    response_string = std::make_shared<std::string>(response.ToString());
    //std::cout<<response_string->c_str()<<std::endl;
    asio::async_write(pThis->socket, boost::asio::buffer(response_string->c_str(), response_string->length()), [pThis, response_string](const error_code& e, std::size_t s)
    {
          //pThis->socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send);
          //pThis->socket.close();
    });

    return response_string->c_str();
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

