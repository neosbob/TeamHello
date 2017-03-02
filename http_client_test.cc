#include "gtest/gtest.h"
#include "http_client.h"

#include <string>

TEST(HttpClientTest, SimpleTest)
{
	HTTPClient client;
	bool ret = client.Connect("www.ucla.edu", "80");
	EXPECT_EQ(ret, true);
	ret = client.SendRequest("GET / HTTP/1.0\r\n");
	EXPECT_EQ(ret, true);
}