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
