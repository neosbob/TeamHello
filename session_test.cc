#include "gtest/gtest.h"
//#include "gmock/gmock.h"
#include "session.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace boost;
using namespace boost::system;
using namespace boost::asio;

class session;

class RequestTest : public testing::Test {

    protected:



};
/*
TEST_F(RequestTest, read_whole_request)
{
	io_service io_service;
   	ip::tcp::endpoint endpoint{ip::tcp::v4(), 8080};
	std::map<std::string, std::string> url_map;
	std::map<std::string, RequestHandler*> mapping;
	RequestHandler* not_found;
	std::shared_ptr<session> sesh = std::make_shared<session>(io_service, mapping, not_found);
	io_service.run();

	char data[] = "Hello\r\n";

	//doesn't really write to socket since it is on same side as read.
        asio::async_write(sesh->socket, asio::buffer(data, 7) ,[sesh](const error_code& e, std::size_t s)
      	{ });

	sesh->read_whole_request(sesh);	

	//Since socket is empty, string ss is not augmented
	EXPECT_EQ(sesh->ss, "");
}
*/

TEST_F(RequestTest, write_response)
{
	io_service io_service;
   	ip::tcp::endpoint endpoint{ip::tcp::v4(), 8080};
	std::map<std::string, std::string> url_map;
	std::map<std::string, RequestHandler*> mapping;
	RequestHandler* not_found;
	std::string logFileName = "log.txt";
	std::shared_ptr<session> sesh = std::make_shared<session>(io_service, mapping, not_found, logFileName);
	io_service.run();

	char data[] = "Hello\r\n";

	//doesn't really write to socket since it is on same side as read.
        asio::async_write(sesh->socket, asio::buffer(data, 7) ,[sesh](const error_code& e, std::size_t s)
      	{ });

	Response response;
	std::string int_serv_error = "Error Status 500.";
	response.SetStatus(Response::internal_server_error);
        response.AddHeader("content-type", "text/plain");
        response.AddHeader("content-length", std::to_string(int_serv_error.length()));
        response.SetBody(int_serv_error);
	

	std::string response_string;
	response_string = sesh->write_response(response, sesh);	

	//Since socket is empty, string ss is not augmented
	EXPECT_EQ(response_string, "HTTP/1.1 500 Internal Server Error\r\ncontent-type: text/plain\r\ncontent-length: 17\r\n\r\nError Status 500.");
}



/*
TEST_F(RequestTest, read_request)
{
	io_service io_service;
   	ip::tcp::endpoint endpoint{ip::tcp::v4(), 8080};
   	ip::tcp::acceptor acceptor{io_service, endpoint};
        std::map<std::string, std::string> url_map;
	std::map<std::string, RequestHandler*> mapping;
	RequestHandler* not_found;
	std::shared_ptr<session> sesh = std::make_shared<session>(io_service, mapping, not_found);
	io_service.run();

	char data[] = "Hello\r\n";

	//doesn't really write to socket since it is on same side as read.
        asio::async_write(sesh->socket, asio::buffer(data, 7) ,[sesh](const error_code& e, std::size_t s)
      	{ });

	sesh->read_request(sesh);	

	//Since socket is empty, string ss is not augmented
	EXPECT_EQ(sesh->ss, "");
}

TEST_F(RequestTest, GetRequestHandler)
{
	io_service io_service;
   	ip::tcp::endpoint endpoint{ip::tcp::v4(), 8080};
   	ip::tcp::acceptor acceptor{io_service, endpoint};
        std::map<std::string, std::string> url_map;
	std::map<std::string, RequestHandler*> mapping;
	RequestHandler* not_found;
	std::shared_ptr<session> sesh = std::make_shared<session>(io_service, mapping, not_found);
	io_service.run();

	char data[] = "Hello\r\n";

	//doesn't really write to socket since it is on same side as read.
        asio::async_write(sesh->socket, asio::buffer(data, 7) ,[sesh](const error_code& e, std::size_t s)
      	{ });

	std::string path = "/static/apple.jpg"
	sesh->GetRequestHandler(path);	

	//Since socket is empty, string ss is not augmented
	EXPECT_EQ(sesh->ss, "");
}

*/


