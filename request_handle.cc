//Inspired by Boost example for async_client.cpp and its use of async_read_until
//www.boost.org/doc/libs/1_43_0/doc/html/boost_asio/example/http/client/async_client.cpp

#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <utility>
#include "request_handle.h"

using namespace boost;
using namespace boost::system;
using namespace boost::asio;

 
void session::read_body(std::shared_ptr<session> pThis)
   {
      int nbuffer = 1000;
      std::shared_ptr<std::vector<char>> buffer_ptr = std::make_shared<std::vector<char>>(nbuffer);
      //reads the body of the incoming request.
      asio::async_read(pThis->socket, boost::asio::buffer(*buffer_ptr, nbuffer), [pThis](const error_code& e, std::size_t s)
      {
      });
   }
   
void session::read_next_line(std::shared_ptr<session> pThis)
   {
      //reads the next line of an incoming request
      asio::async_read_until(pThis->socket, pThis->buff, '\r', [pThis](const error_code& e, std::size_t s)
      {
         std::string line, ignore;
         std::istream stream {&pThis->buff};

         pThis->ss += pThis->read_next_line_text(stream, line, ignore);
	 pThis->re_static.read_header(line);
         pThis->re_echo.read_header(line);
	 std::string echoback = pThis->ss;
         
         if(line.length() != 0)
         {
            pThis->read_next_line(pThis);
         }
         //This signals end of request
         else if(line.length() == 0)
         {
            if(pThis->re_static.content_length() != 0 && pThis->re_echo.content_length())
            {
               pThis->read_body(pThis);
            }
            //This signals end of request's body
            else
            {
	       std::string request_path, static_path, request_echo, echo_path, sfile_dir;
               std::size_t url_found = pThis->re_static.url.find("/", 1);
	       if (url_found != std::string::npos){
	           static_path = pThis->re_static.url.substr(0,url_found);
		   request_path = pThis->re_static.url.substr(url_found);
                   std::map<std::string, std::string>::iterator it;
	           it = pThis->base_path.find(static_path);
	           if (it != pThis->base_path.end())
	               sfile_dir = pThis->base_path.find(static_path)->second;
               }
	       std::size_t echo_found = pThis->re_static.url.find("/echo");
               if ( echo_found != std::string::npos){
	           request_echo = pThis->re_static.url.substr(0,5);
		   echo_path = pThis->re_static.url.substr(5);
               }
               
	       
	       std::shared_ptr<std::string> str;
	       
               if(sfile_dir != "")
		   str = std::make_shared<std::string>(pThis->re_static.get_response(echoback, sfile_dir));
	       else if (request_echo == pThis->echo_map)
		   str = std::make_shared<std::string>(pThis->re_echo.get_response(echoback, "/"));
	       else
		   str = std::make_shared<std::string>(pThis->re_static.get_response(echoback, ""));
                   asio::async_write(pThis->socket, boost::asio::buffer(str->c_str(), str->length()), [pThis, str](const error_code& e, std::size_t s)
               {
                  //return str->c_str();
               });
            }
         }
      });
   }

std::string session::read_next_line_text(std::istream & stream, std::string & line, std::string & ignore)
   {
      	std::getline(stream, line, '\r');
        std::getline(stream, ignore, '\n');
        
        std::string current_line;
	current_line += line + "\r\n";      

	return current_line; 
   }

void session::read_first_line(std::shared_ptr<session> pThis)
   {
      asio::async_read_until(pThis->socket, pThis->buff, '\r', [pThis](const error_code& e, std::size_t s)
      {
         std::string line, ignore;
         std::istream stream {&pThis->buff};
 
	 pThis->ss += pThis->read_first_line_text(stream, line, ignore);
	 pThis->re_static.read_request_line(line);
	 pThis->re_echo.read_request_line(line);
         pThis->read_next_line(pThis);
      });
      
   }

std::string session::read_first_line_text(std::istream & stream, std::string & line, std::string & ignore)
   {
      	std::getline(stream, line, '\r');
        std::getline(stream, ignore, '\n');
        std::string first_line;
        first_line += line + "\r\n";

	return first_line; 
   }
   

   
void session::read_request(std::shared_ptr<session> pThis)
   {
      pThis->read_first_line(pThis);
   }

