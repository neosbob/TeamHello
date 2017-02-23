#include "gtest/gtest.h"
#include "not_found_handler.h"
#include "request.h"
#include "response.h"
#include "file_handler.h"
#include "request_handler.h"

#include <string>

TEST(NotFoundHandlerTest, SimpleTest)
{
	std::string raw_request = "";
	auto req = Request::Parse(raw_request.c_str());

	StaticHandler file_handler;
	NginxConfig config;

	NginxConfigParser parser;
	std::stringstream config_stream("root ./test_folder2;");
	parser.Parse(&config_stream, &config);

	EXPECT_EQ(config.statements_[0]->tokens_[1],"./test_folder2");
	EXPECT_EQ(config.statements_[0]->tokens_[0],"root");

	file_handler.Init("/static2", config);

	Response resp;
	file_handler.HandleRequest(*req, &resp);
	std::string resp_str = resp.ToString();
	
	int RN = resp_str.find("\r\n");
	EXPECT_EQ(resp_str.substr(0,RN), "HTTP/1.1 404 Not Found");
}
