#include "gtest/gtest.h"
#include "request.h"
#include "response.h"

#include <string>

//test request with no body and headers
TEST(RequestTest, SimpleRequest)
{
	auto req = Request::Parse("GET /apple.jpg HTTP/1.1\r\n\r\n");
	EXPECTED_EQ(req->raw_request("GET /apple.jpg HTTP/1.1\r\n\r\n"); 
	EXPECTED_EQ(req->method(), "GET");
	EXPECTED_EQ(req->uri(), "/apple.jpg");
	EXPECTED_EQ(req->version(), "HTTP/1.1");
}

//test request with headers
TEST(RequestTest, RequestWithHeaders)
{
	auto req = Request::Parse("GET /orange.jpg HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/7.43\r\n\r\n");
	EXPECTED_EQ(req->raw_request("GET /orange.jpg HTTP/1.1\r\n\r\n"); 
	EXPECTED_EQ(req->method(), "GET");
	EXPECTED_EQ(req->uri(), "/orange.jpg");
	EXPECTED_EQ(req->version(), "HTTP/1.1");
	EXPECTED_EQ(req->headers().size(), 2);
}

//test request with a body 
TEST(RequestTest, RequestWithBody)
{
	auto req = Request::Parse("GET /orange.jpg HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/7.43\r\n\r\niambody\r\n");
	EXPECTED_EQ(req->raw_request("GET /orange.jpg HTTP/1.1\r\n\r\n"); 
	EXPECTED_EQ(req->method(), "GET");
	EXPECTED_EQ(req->uri(), "/orange.jpg");
	EXPECTED_EQ(req->version(), "HTTP/1.1");
	EXPECTED_EQ(req->headers().size(), 2);
	EXPECTED_EQ(req->body(), "iambody");
}

TEST(RequestTest, ReadRequestLine)
{
	Request req;
	std::string line = "GET /orange.jpg HTTP/1.1";
	std::string result_uri = "";
	result_uri = req.read_request_line(line);
	EXPECTED_EQ(req.method(), "GET");
	EXPECTED_EQ(req.uri(), "/orange.jpg");
	EXPECTED_EQ(req.version(), "HTTP/1.1");
	EXPECTED_EQ(result_uri, "/orange.jpg");
	
}

//test read_header function
TEST(RequestTest, ReadHeader)
{
	Request req;
	std::string line = "Host: localhost:8080\r\nUser-Agent: curl/7.43";
	std::string result_bool = "";
	result_bool = req.read_header(line);

	std::vector<std::pair<std::string, std::string>> headers;
	std::pair<std::string, std::string> header_pair("Host", "localhost:8080");
	std::pair<std::string, std::string> header_pair2("User-Agent", "curl/7.43")
	headers.pushback(header_pair);
	headers.pushback(header_pair2);
	
	EXPECTED_EQ(req.headers(), headers);
	EXPECTED_TRUE(result_bool);
	
}

