#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <map>
#include "request.h"
#include "response.h"
#include "mime_types.h"
#include "config_parser.h"

// Represents the parent of all request handlers. Implementations should expect to
// be long lived and created at server constrution.
class RequestHandler {
 public:
  enum Status {
    OK = 0,
    FAILED = 1
    // Define your status codes here.
  };
  
  static RequestHandler* CreateByName(const char* type);

  // Initializes the handler. Returns a response code indicating success or
  // failure condition.
  // uri_prefix is the value in the config file that this handler will run for.
  // config is the contents of the child block for this handler ONLY.
  virtual Status Init(const std::string& uri_prefix,
                      const NginxConfig& config) = 0;

  // Handles an HTTP request, and generates a response. Returns a response code
  // indicating success or failure condition. If ResponseCode is not OK, the
  // contents of the response object are undefined, and the server will return
  // HTTP code 500.
  virtual Status HandleRequest(const Request& request,
                               Response* response) = 0;
};

typedef std::map<const std::string, RequestHandler*> HandlerContainer;

extern std::map<std::string, RequestHandler* (*)(void)>* request_handler_builders;
template<typename T>
class RequestHandlerRegisterer {
  public:
    RequestHandlerRegisterer(const std::string& type) {
      if (request_handler_builders == nullptr) {
          request_handler_builders = new std::map<std::string, RequestHandler* (*)(void)>;
      }
      (*request_handler_builders)[type] = RequestHandlerRegisterer::Create;
    }
    static RequestHandler* Create() {
        return new T;
    }
};
#define REGISTER_REQUEST_HANDLER(ClassName) \
  static RequestHandlerRegisterer<ClassName> ClassName##__registerer(#ClassName)

#endif
