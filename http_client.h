#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <iostream>
#include <istream>
#include <ostream>
#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <string>
#include "response.h"
#include <iterator>
#include <exception>

using boost::asio::ip::tcp;

class HTTPClient {
public:
    HTTPClient();
    ~HTTPClient();
    // resolve and connect
    bool Connect(std::string host, std::string port = "80");
    bool SendRequest(std::string request);
    std::unique_ptr<Response> ObtainResponse();

private:
    boost::asio::io_service io_service_;
    tcp::socket socket_;
};

#endif
