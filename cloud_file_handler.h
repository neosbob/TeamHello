#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <map>
#include "request_handler.h"
#include "not_found_handler.h"

typedef std::vector<std::pair<std::string, std::string>> Headers;

// Represents the parent of all request handlers. Implementations should expect to be long lived and created at server constrution.

//Serves files from a cloud storage device.
class CloudHandler : public RequestHandler {
 public:
    //CloudHandler(const std::string& directory): root_dir(directory) {}   
    // Initializes the handler. Returns a response code indicating success or failure condition.
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
    std::string get_cloud_object(std::string remote_dir, std::string remote_file_path);
    bool parse_remote_response(std::string remote_response);
    bool read_header(std::string headers);


  private:
      std::string root_dir_;
      std::string uri_prefix_;
      std::string host_url_;
      std::string protocol_;
      std::string path_;

      std::string response_headers, response_body, response_status, rest;
      Headers headers_;
};

REGISTER_REQUEST_HANDLER(CloudHandler);

#endif
