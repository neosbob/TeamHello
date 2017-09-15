#include "gtest/gtest.h"
#include "proxy_handler.h"
#include "request.h"
#include "response.h"
#include "file_handler.h"
#include "request_handler.h"

#include <string>

TEST(HttpClientTest, SimpleTest)
{
	HTTPClient client;
	bool ret = client.Connect("www.ucla.edu", "80");
	EXPECT_EQ(ret, true);
	ret = client.SendRequest("GET / HTTP/1.0\r\n");
	EXPECT_EQ(ret, true);
}

TEST(HttpClientTest, ObtainResponse)
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

	file_handler.Init("/", config_1);
	Response resp;
	RequestHandler::Status response_status = file_handler.HandleRequest(*req, &resp);
	EXPECT_EQ(response_status, RequestHandler::Status::OK);
}