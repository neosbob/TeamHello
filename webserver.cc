
#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <cstdlib>
#include "config_parser.h"
#include <utility>
#include "request_handle.h"

using namespace boost;
using namespace boost::system;
using namespace boost::asio;

class session;

void server(ip::tcp::acceptor& acceptor, io_service& io_service)
{
   std::shared_ptr<session> sesh = std::make_shared<session>(io_service);
   acceptor.async_accept(sesh->socket, [sesh, &acceptor, &io_service](const error_code& accept_error)
   {
      server(acceptor, io_service);
      if(!accept_error)
      {
         session::read_request(sesh);
      }
   });
}

int main(int argc, const char * argv[])
{
   short unsigned int port;
  
   try {
     if (argc != 2)
     {
       std::cerr<< "Usage ./webserver <path to config file>\n";
       return 1;
     }
     NginxConfigParser config_parser;
     NginxConfig config_out;

     if (config_parser.Parse(argv[1], &config_out)) {
        port = std::stoi(config_out.statements_[0]->tokens_[1]);
     }
     else {
         std::cerr << "Could not parse config file.\n";
         return 1;
     }

   }

   catch(std::exception& error) {
       std::cerr<< "Exception: "<< error.what() << "\n";
   }
    
   io_service io_service;
   ip::tcp::endpoint endpoint{ip::tcp::v4(), port};
   ip::tcp::acceptor acceptor{io_service, endpoint};
   
   acceptor.listen();
   server(acceptor, io_service);
   
   io_service.run();
   return 0;

}

      
      
