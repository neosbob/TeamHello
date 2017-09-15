#include "gtest/gtest.h"
#include "proxy_handler.h"
#include "request.h"
#include "response.h"
#include "file_handler.h"
#include "request_handler.h"

#include <string>

TEST(ProxyHandlerTest, NormalTest)
{
	std::string raw_request = "GET / HTTP/1.0\r\n";
	auto req = Request::Parse(raw_request.c_str());

	ProxyHandler file_handler;
	NginxConfig config_1, config_2;

	NginxConfigParser parser;
	std::stringstream config_stream_1("host www.ucla.edu;");
	parser.Parse(&config_stream_1, &config_1);
	EXPECT_EQ(config_1.statements_[0]->tokens_[1],"www.ucla.edu");
	EXPECT_EQ(config_1.statements_[0]->tokens_[0],"host");

	std::stringstream config_stream_2("port 80;");
	parser.Parse(&config_stream_2, &config_2);
	EXPECT_EQ(config_2.statements_[0]->tokens_[1],"80");
	EXPECT_EQ(config_2.statements_[0]->tokens_[0],"port");

	file_handler.Init("/", config_1);
	file_handler.Init("/", config_2);
	Response resp;
	RequestHandler::Status response_status = file_handler.HandleRequest(*req, &resp);
	EXPECT_EQ(response_status, RequestHandler::Status::OK);
}

TEST(ProxyHandlerTest, MoveTemporarilyTest)
{
	std::string raw_request = "GET / HTTP/1.0\r\n";
	auto req = Request::Parse(raw_request.c_str());

	ProxyHandler file_handler;
	NginxConfig config_1, config_2;

	NginxConfigParser parser;
	std::stringstream config_stream_1("host ucla.edu;");
	parser.Parse(&config_stream_1, &config_1);
	EXPECT_EQ(config_1.statements_[0]->tokens_[1],"ucla.edu");
	EXPECT_EQ(config_1.statements_[0]->tokens_[0],"host");

	file_handler.Init("/", config_1);
	Response resp;
	RequestHandler::Status response_status = file_handler.HandleRequest(*req, &resp);
	EXPECT_EQ(response_status, RequestHandler::Status::OK);
}
