//Inspired by Boost example for async_client.cpp and its use of async_read_until
//www.boost.org/doc/libs/1_43_0/doc/html/boost_asio/example/http/client/async_client.cpp

#include <boost/asio.hpp>
#include <iostream>
#include <cstdlib>
#include "session.h"
#include "webserver.h"

using namespace boost;
using namespace boost::system;
using namespace boost::asio;

session::session(io_service& io_service, std::map<std::string, RequestHandler*> mapping, RequestHandler* not_found, std::string* ret_log)
: socket(io_service)
, handlers(mapping)
, default_handler(not_found)
, ret_log(ret_log)
{}

void session::writeToLog(std::string msg)
{
    *ret_log = msg;
}

void session::read_whole_request(std::shared_ptr<session> pThis)
{
      pThis->socket.async_read_some(boost::asio::buffer(pThis->data), [pThis](const error_code& e, std::size_t s)
      {
          Response response;
          std::string buffer = std::string(pThis->data, s);
          auto request = Request::Parse(buffer);
          RequestHandler* handler = pThis->GetRequestHandler(request->uri());
	      int response_code = 200;
	      
	  if(handler != nullptr) {
	      RequestHandler::Status response_status = handler->HandleRequest(*request, &response);
	      if (response_status == RequestHandler::Status::FAILED)
	      {
                  std::string int_serv_error = "Error Status 500.";
	          response.SetStatus(Response::internal_server_error);
                  response.AddHeader("content-type", "text/plain");
                  response.AddHeader("content-length", std::to_string(int_serv_error.length()));
                  response.SetBody(int_serv_error);
                  response_code = 500;
          }
	      else if (response_status == RequestHandler::Status::FILE_NOT_FOUND)
          {
              handler = pThis->default_handler;
              RequestHandler::Status response_status = handler->HandleRequest(*request, &response);
              response_code = 404;
          }
      }
	  else if (handler == nullptr) {
	      handler = pThis->default_handler;
	      RequestHandler::Status response_status = handler->HandleRequest(*request, &response);
	      response_code = 404;
      }     
          pThis->write_response(response, pThis);
          std::string logMsg = Server::constructLogMsg(request->uri(), response_code);
          pThis->writeToLog(logMsg);
      });
}

std::string session::write_response(Response& response, std::shared_ptr<session> pThis)
{
    std::shared_ptr<std::string> response_string;
    response_string = std::make_shared<std::string>(response.ToString());
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
    std::string temp_path = path;
    while(temp_path.length() > 0) {
      for(auto& handlerPair: handlers) {	
	if (temp_path == handlerPair.first) {
            return handlerPair.second;
        }
      }
      std::size_t slash_found = temp_path.find_last_of("/");
      if (slash_found == 0 && temp_path == "/")
          break;
      temp_path = temp_path.substr(0, slash_found);
      if (slash_found == 0)
          temp_path = "/";
    }
    return nullptr;
}

