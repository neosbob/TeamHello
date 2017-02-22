#include "gtest/gtest.h"
#include "echo_handler.h"
#include "request.h"
#include "response.h"

#include <string>

TEST(EchoHandlerTest, HandleRequest)
{
	std::string raw_request = "testcontent";
	//generate header info
	auto req = Request::Parse(raw_request.c_str());
	
	EchoHandler echo_handler;
	Response resp;
	
	echo_handler.HandleRequest(*req, &resp);
	std::string resp_Str = resp.ToString();

	std::string expected_string = "HTTP/1.0 200 OK\r\ncontent-type: text/plain\r\ncontent-length: 11\r\n\r\ntestcontent";
	EXPECT_EQ(resp_Str, expected_string);
}

TEST(EchoHandlerTest, ReturnStatus)
{
	std::string raw_request = "test2";
	//generate header info
	auto req = Request::Parse(raw_request.c_str());
	
	EchoHandler echo_handler;
	Response resp;
	
	RequestHandler::Status s = echo_handler.HandleRequest(*req, &resp);
	
	EXPECT_EQ(s, RequestHandler::OK);
}
