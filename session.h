#ifndef HTTP_REQUEST_HANDLER_H
#define HTTP_REQUEST_HANDLER_H

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <string>
#include <memory>
#include <utility>
#include <fstream>
#include "request_handler.h"
#include "request.h"
#include "response.h"

using namespace boost;
using namespace boost::system;
using namespace boost::asio;

const int max_length = 8192;

// The common networking interface for all incoming requests.
class session
{
public:
   static void read_whole_request(std::shared_ptr<session> pThis);
   std::string write_response(Response& response, std::shared_ptr<session> pThis);
   session(io_service& io_service, std::map<std::string, RequestHandler*> mapping, RequestHandler* not_found, std::fstream& logFile);
   static void read_request(std::shared_ptr<session> pThis);
   
   ip::tcp::socket socket;
   char data[max_length];
   
private:
   //asio::streambuf buff;
   //boost::array<char,max_length> buffer_;
   

   std::map<std::string, RequestHandler*> handlers;
   RequestHandler* default_handler;
   std::fstream& logFile;

   RequestHandler* GetRequestHandler(const std::string& path);
   void writeToLog(std::string msg);
};


#endif // HTTP_REQUEST_HANDLER_H
