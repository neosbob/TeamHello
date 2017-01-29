#include "gtest/gtest.h"
#include "reply.h"

TEST(ReplyTest, GetResponse)
{
	http_headers h;
	std::string s = "test string";
	std::stringstream ssOut;

	ssOut << "HTTP/1.1 200 OK" << std::endl;
        ssOut << "content-type: text/plain" << std::endl;
        ssOut << "content-length: " << s.length() << std::endl;
        ssOut << std::endl; 
        ssOut << s;

	std::string s2 = h.get_response(s);
	EXPECT_EQ(ssOut.str(),s2);
}

TEST(ReplyTest, ReadHeader)
{
	http_headers h;
	std::stringstream ssOut;
        ssOut << "content-type: text/plain" << std::endl;
	std::string s = ssOut.str();
	std::string header_value = h.read_header(s);
	EXPECT_EQ(header_value,"content-type: text/plain");
	
}
