#include "gtest/gtest.h"
#include "file_handler.h"
#include "request.h"
#include "response.h"
#include "request_handler.h"


#include <string>


//test with an empty file
TEST(FileHandlerTest, EmptyFile)
{
	std::string raw_request = "GET /static/ HTTP/1.1\r\n\r\n";
	auto req = Request::Parse(raw_request.c_str());

	StaticHandler file_handler;
	NginxConfig config;

	NginxConfigParser parser;
	std::stringstream config_stream("root ./test_folder;");
	parser.Parse(&config_stream, &config);

	EXPECT_EQ(config.statements_[0]->tokens_[1],"./test_folder");
	EXPECT_EQ(config.statements_[0]->tokens_[0],"root");

	file_handler.Init("/static", config);
	Response resp;
	RequestHandler::Status response_status = file_handler.HandleRequest(*req, &resp);
	EXPECT_EQ(response_status, RequestHandler::Status::FILE_NOT_FOUND);
}

//test with JPG file 
TEST(FileHandlerTest, JPGFile)
{
	std::string raw_request = "GET /static/apple.jpg HTTP/1.1\r\n\r\n";
	auto req = Request::Parse(raw_request.c_str());

	StaticHandler file_handler;
	NginxConfig config;

	NginxConfigParser parser;
	std::stringstream config_stream("root ./test_folder;");
	parser.Parse(&config_stream, &config);

	EXPECT_EQ(config.statements_[0]->tokens_[1],"./test_folder");
	EXPECT_EQ(config.statements_[0]->tokens_[0],"root");

	file_handler.Init("/static", config);

	Response resp;
	file_handler.HandleRequest(*req, &resp);
	std::string resp_str = resp.ToString();
	
	int RN = resp_str.find("\r\n");
	EXPECT_EQ(resp_str.substr(0,RN), "HTTP/1.1 200 OK");

}

//test with JPG file from another path
TEST(FileHandlerTest, JPGFile_anotherPath)
{
	std::string raw_request = "GET /static2/orange.jpg HTTP/1.1\r\n\r\n";
	auto req = Request::Parse(raw_request.c_str());

	StaticHandler file_handler;
	NginxConfig config;

	NginxConfigParser parser;
	std::stringstream config_stream("root ./test_folder2;");
	parser.Parse(&config_stream, &config);

	EXPECT_EQ(config.statements_[0]->tokens_[1],"./test_folder2");
	EXPECT_EQ(config.statements_[0]->tokens_[0],"root");

	file_handler.Init("/static2", config);

	Response resp;
	file_handler.HandleRequest(*req, &resp);
	std::string resp_str = resp.ToString();
	
	int RN = resp_str.find("\r\n");
	EXPECT_EQ(resp_str.substr(0,RN), "HTTP/1.1 200 OK");

}

//test with JPG file that does not exist
TEST(FileHandlerTest, JPGFile_notexist)
{
	std::string raw_request = "GET /static/orange.jpg HTTP/1.1\r\n\r\n";
	auto req = Request::Parse(raw_request.c_str());

	StaticHandler file_handler;
	NginxConfig config;

	NginxConfigParser parser;
	std::stringstream config_stream("root ./test_folder;");
	parser.Parse(&config_stream, &config);

	file_handler.Init("/static", config);
	Response resp;
	RequestHandler::Status response_status = file_handler.HandleRequest(*req, &resp);
	EXPECT_EQ(response_status, RequestHandler::Status::FILE_NOT_FOUND);
}

//test with HTML file 
TEST(FileHandlerTest, HTMLfile)
{
	std::string raw_request = "GET /static/sample.html HTTP/1.1\r\n\r\n";
	auto req = Request::Parse(raw_request.c_str());

	StaticHandler file_handler;
	NginxConfig config;

	NginxConfigParser parser;
	std::stringstream config_stream("root ./test_folder;");
	parser.Parse(&config_stream, &config);

	file_handler.Init("/static", config);

	Response resp;
	file_handler.HandleRequest(*req, &resp);
	std::string resp_str = resp.ToString();
	
	int RN = resp_str.find("\r\n");
	EXPECT_EQ(resp_str.substr(0,RN), "HTTP/1.1 200 OK");
}

