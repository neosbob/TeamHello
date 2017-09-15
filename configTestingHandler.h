#ifndef CONFIG_TESTING_HANDLER_H
#define CONFIG_TESTING_HANDLER_H

#include <string>
#include <map>
#include "request_handler.h"

// Represents the parent of all request handlers. Implementations should expect to
// be long lived and created at server constrution.
class ConfigTestingHandler : public RequestHandler {
 public: 
  // Initializes the handler. Returns a response code indicating success or
  // failure condition.
  // uri_prefix is the value in the config file that this handler will run for.
  // config is the contents of the child block for this handler ONLY.
  virtual Status Init(const std::string& uri_prefix,
                      const NginxConfig& config);

  // Handles an HTTP request, and generates a response. Returns a response code
  // indicating success or failure condition. If ResponseCode is not OK, the
  // contents of the response object are undefined, and the server will return
  // HTTP code 500.
  virtual Status HandleRequest(const Request& request,
                               Response* response);
};

REGISTER_REQUEST_HANDLER(ConfigTestingHandler);

#endif // CONFIG_TESTING_HANDLER_H
