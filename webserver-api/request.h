#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <vector>
#include <memory>

typedef std::vector<std::pair<std::string, std::string>> Headers;

// Represents an HTTP Request.
//
// Usage:
//   auto request = Request::Parse(raw_request);
class Request {
    public:
        static std::unique_ptr<Request> Parse(const std::string& raw_request);
	std::string read_request_line(std::string line);
	bool read_header(std::string headers);

        std::string raw_request() const;
        std::string method() const;
        std::string uri() const;
        std::string version() const;

        Headers headers() const;
        std::string body() const;

    private:
	//member variables
        std::string raw_request_;
        std::string method_;
        std::string uri_;
        std::string version_;
        std::string body_;
	Headers headers_;
};

#endif
