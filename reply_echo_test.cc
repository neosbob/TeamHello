#include "gtest/gtest.h"
#include "reply_echo.h"

TEST(ReplyEchoTest, GetResponse)
{
	reply_echo h;
	std::string s = "test string";
	std::stringstream ssOut;

	ssOut << "HTTP/1.1 200 OK" << std::endl;
        ssOut << "content-type: text/plain" << std::endl;
        ssOut << "content-length: " << s.length() << std::endl;
        ssOut << std::endl; 
        ssOut << s;

	std::string b = "base";

	h.url = "/echo";

	std::string s2 = h.get_response(s,b);
	EXPECT_EQ(ssOut.str(),s2);
}

TEST(ReplyEchoTest, GetResponse_noEcho)
{
	reply_echo h;
	std::string s = "test string";
	std::stringstream ssOut;

	std::string sHTML = "<html><body><p>To use echo, please specify http://localhost:8080/echo</p></body></html>";
         ssOut << "HTTP/1.1 404 Not Found" << std::endl;
         ssOut << "content-type: text/plain" << std::endl;
         ssOut << "content-length: " << sHTML.length() << std::endl;
         ssOut << std::endl;
         ssOut << sHTML;

	std::string b = "base";

	std::string s2 = h.get_response(s,b);
	EXPECT_EQ(ssOut.str(),s2);
}
