#include "gtest/gtest.h"
#include "response.h"

#include <string>

//test SetStatus, AddHeader, SetBody, and ToString functions with a simple response 
TEST(ResponseTest, simple_response)
{
	Response response;
	response.SetStatus(Response::ok);
	response.AddHeader("Content-Type", "text/html");
	response.SetBody("hello");
	std::string result = response.ToString();
	EXPECT_EQ(result, "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\nhello");

}

