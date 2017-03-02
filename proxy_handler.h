#ifndef PROXY_HANDLER_H
#define PROXY_HANDLER_H

#include "uri_parser.hpp"
#include "request_handler.h"
#include "http_client.h"

class ProxyHandler : public RequestHandler {
public:

    // Initializes the handler. Returns a response code indicating success or
    // failure condition.
    virtual Status Init(const std::string& uri_prefix,
                  const NginxConfig& config);

    // Handles an HTTP request, and generates a response. Returns a response code
    // indicating success or failure condition. If ResponseCode is not OK, the
    // contents of the response object are undefined, and the server will return
    // HTTP code 200.
    virtual Status HandleRequest(const Request& request,
                           Response* response);
private:
    std::string host_;
    std::string port_ = "80";
};

REGISTER_REQUEST_HANDLER(ProxyHandler);

#endif
