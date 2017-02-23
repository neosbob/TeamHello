#include "gtest/gtest.h"
#include "request.h"
#include "response.h"

#include <string>

//test request with no body and headers
TEST(RequestTest, SimpleRequest)
{
	auto req = Request::Parse("GET /apple.jpg HTTP/1.1\r\n\r\n");
	EXPECT_EQ(req->raw_request(), "GET /apple.jpg HTTP/1.1\r\n\r\n"); 
	EXPECT_EQ(req->method(), "GET");
	EXPECT_EQ(req->uri(), "/apple.jpg");
	EXPECT_EQ(req->version(), "HTTP/1.1");
}

//test request with headers
TEST(RequestTest, RequestWithHeaders)
{
	auto req = Request::Parse("GET /orange.jpg HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/7.43\r\n\r\n");
	EXPECT_EQ(req->raw_request(), "GET /orange.jpg HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/7.43\r\n\r\n"); 
	EXPECT_EQ(req->method(), "GET");
	EXPECT_EQ(req->uri(), "/orange.jpg");
	EXPECT_EQ(req->version(), "HTTP/1.1");
	EXPECT_EQ(req->headers().size(), 2);
}

//test request with a body 
TEST(RequestTest, RequestWithBody)
{
	auto req = Request::Parse("GET /orange.jpg HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/7.43\r\n\r\niambody\r\n");
	EXPECT_EQ(req->raw_request(), "GET /orange.jpg HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/7.43\r\n\r\niambody\r\n"); 
	EXPECT_EQ(req->method(), "GET");
	EXPECT_EQ(req->uri(), "/orange.jpg");
	EXPECT_EQ(req->version(), "HTTP/1.1");
	EXPECT_EQ(req->headers().size(), 2);
	EXPECT_EQ(req->body(), "iambody\r\n"); 
}

TEST(RequestTest, ReadRequestLine)
{
	Request req;
	std::string line = "GET /orange.jpg HTTP/1.1";
	std::string result_uri = "";
	result_uri = req.read_request_line(line);
	EXPECT_EQ(req.method(), "GET");
	EXPECT_EQ(req.uri(), "/orange.jpg");
	EXPECT_EQ(req.version(), "HTTP/1.1");
	EXPECT_EQ(result_uri, "/orange.jpg");
	
}


TEST(RequestTest, ReadHeader)
{
	Request req;
	std::string line = "Host: localhost:8080\r\nUser-Agent: curl/7.43\r\n";
	bool result_bool = false;
	result_bool = req.read_header(line);

	std::vector<std::pair<std::string, std::string>> headers;
	std::pair<std::string, std::string> header_pair("Host", "localhost:8080");
	std::pair<std::string, std::string> header_pair2("User-Agent", "curl/7.43");
	headers.push_back(header_pair);
	headers.push_back(header_pair2);
	
	EXPECT_EQ(req.headers(), headers);
	EXPECT_TRUE(result_bool);
	
}


