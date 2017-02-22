#include "gtest/gtest.h"
#include "file_handler.h"
#include "request.h"
#include "response.h"

#include <string>

//test with an empty file
TEST(FileHandlerTest, JPGFile_notexist)
{
	std::string raw_request = "GET /static/ HTTP/1.1\r\n\r\n";
	auto req = Request::Parse(raw_request.c_str());

	FileHandler file_handler("static/");
	Response resp;
	file_handler.HandleRequest(*req, &resp);
	std::string resp_str = resp.ToString();
	
	int RN = resp.find("\r\n");
	EXPECT_EQ(resp_str.substr(0,RN), "HTTP/1.1 404 Not Found");
}

//test with file with no keyword
TEST(FileHandlerTest, JPGFile_notexist)
{
	std::string raw_request = "GET /static/orange.jpg HTTP/1.1\r\n\r\n";
	auto req = Request::Parse(raw_request.c_str());

	FileHandler file_handler("static/");
	Response resp;
	file_handler.HandleRequest(*req, &resp);
	std::string resp_str = resp.ToString();
	
	int RN = resp.find("\r\n");
	EXPECT_EQ(resp_str.substr(0,RN), "HTTP/1.1 404 Not Found");
}

//test with JPG file 
TEST(FileHandlerTest, JPGFile)
{
	std::string raw_request = "GET /static/apple.jpg HTTP/1.1\r\n\r\n";
	auto req = Request::Parse(raw_request.c_str());

	FileHandler file_handler("static/");
	Response resp;
	file_handler.HandleRequest(*req, &resp);
	std::string resp_str = resp.ToString();
	
	int RN = resp.find("\r\n");
	EXPECT_EQ(resp_str.substr(0,RN), "HTTP/1.1 200 OK");

}

//test with JPG file from another path
TEST(FileHandlerTest, JPGFile_anotherPath)
{
	std::string raw_request = "GET /static2/orange.jpg HTTP/1.1\r\n\r\n";
	auto req = Request::Parse(raw_request.c_str());

	FileHandler file_handler("static2/");
	Response resp;
	file_handler.HandleRequest(*req, &resp);
	std::string resp_str = resp.ToString();
	
	int RN = resp.find("\r\n");
	EXPECT_EQ(resp_str.substr(0,RN), "HTTP/1.1 200 OK");

}

//test with JPG file that does not exist
TEST(FileHandlerTest, JPGFile_notexist)
{
	std::string raw_request = "GET /static/orange.jpg HTTP/1.1\r\n\r\n";
	auto req = Request::Parse(raw_request.c_str());

	FileHandler file_handler("static/");
	Response resp;
	file_handler.HandleRequest(*req, &resp);
	std::string resp_str = resp.ToString();
	
	int RN = resp.find("\r\n");
	EXPECT_EQ(resp_str.substr(0,RN), "HTTP/1.1 404 Not Found");
}

//test with HTML file 
TEST(FileHandlerTest, HTMLfile)
{
	std::string raw_request = "GET /static/sample.html HTTP/1.1\r\n\r\n";
	auto req = Request::Parse(raw_request.c_str());

	FileHandler file_handler("static/");
	Response resp;
	file_handler.HandleRequest(*req, &resp);
	std::string resp_str = resp.ToString();
	
	int RN = resp.find("\r\n");
	EXPECT_EQ(resp_str.substr(0,RN), "HTTP/1.1 200 OK");
}
