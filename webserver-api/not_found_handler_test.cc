#include "gtest/gtest.h"
#include "not_found_handler.h"
#include "request.h"
#include "response.h"

#include <string>

TEST(NotFoundHandlerTest, SimpleTest)
{
	std::string raw_request = "";
	auto req = Request::Parse(raw_request.c_str());

	FileHandler file_handler("static/");
	Response resp;
	file_handler.HandleRequest(*req, &resp);
	std::string resp_str = resp.ToString();
	
	int RN = resp.find("\r\n");
	EXPECT_EQ(resp_str.substr(0,RN), "HTTP/1.1 404 Not Found");
}
