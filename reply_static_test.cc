#include "gtest/gtest.h"
#include "reply_static.h"

TEST(ReplyStaticTest, GetResponse_notStatic)
{
	reply_static r;
	std::string s = "test string";
	std::stringstream ssOut;

	std::string sHTML = "<html><body><p>URL does not start with \"/static/\" </p></body></html>";
        ssOut << "HTTP/1.1 404 Not Found" << std::endl;
        ssOut << "content-type: text/html" << std::endl;
        ssOut << "content-length: " << sHTML.length() << std::endl;
        ssOut << std::endl;
        ssOut << sHTML;

	std::string b = "base";

	r.url = "/echo";

	std::string s2 = r.get_response(s,b);
	EXPECT_EQ(ssOut.str(),s2);
}
