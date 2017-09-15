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
	EXPECT_EQ(result, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\nhello");
	EXPECT_EQ(response.GetBody(), "hello");
}

TEST(ResponseTest, ParseResponse){
	std::string head = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n";
	auto resp = Response::ParseHeader(head);
	auto h  = resp->GetHeaders();
	EXPECT_EQ(h[0].first, "Content-Type");
	EXPECT_EQ(h[0].second, "text/html");
	EXPECT_EQ(Response::ok, resp->GetResponseCode());
}