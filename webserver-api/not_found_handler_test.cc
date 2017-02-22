#include "gtest/gtest.h"
#include "not_found_handler.h"
#include "request.h"
#include "response.h"
#include "file_handler.h"

#include <string>

TEST(NotFoundHandlerTest, SimpleTest)
{
	std::string raw_request = "";
	auto req = Request::Parse(raw_request.c_str());

	FileHandler file_handler("./test_folder");
	Response resp;
	file_handler.HandleRequest(*req, &resp);
	std::string resp_str = resp.ToString();
	
	int RN = resp_str.find("\r\n");
	EXPECT_EQ(resp_str.substr(0,RN), "HTTP/1.0 404 Not Found");
}
