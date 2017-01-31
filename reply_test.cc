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

TEST(ReplyTest, ReadReqLine)
{
	http_headers h;
	std::string line = "GET /www HTTP/1.1";
	std::string url = h.read_request_line(line);
	EXPECT_EQ("/www", url);

}

TEST(ReplyTest, ContentLen)
{
	std::map<std::string, std::string> header;
	http_headers h;
	header["content-type"]="plain";
	header["content-length"]="20";
	h.setMap(header);
	int c_len = h.content_length();
	EXPECT_EQ(20,c_len);
	

}

TEST(ReplyTest, SetMap)
{
	std::map<std::string, std::string> header;
	http_headers h;
	header["content-type"]="plain";
	header["content-length"]="20";
	bool val = h.setMap(header);
	EXPECT_TRUE(val);
}
