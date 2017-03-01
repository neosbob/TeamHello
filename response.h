#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <memory>

// Represents an HTTP response.
//
// Usage:
//   Response r;
//   r.SetStatus(RESPONSE_200);
//   r.SetBody(...);
//   return r.ToString();
//
// Constructed by the RequestHandler, after which the server should call ToString
// to serialize.
class Response {
public:
    enum ResponseCode {
          // Define your HTTP response codes here.
      ok = 200,
      created = 201,
      accepted = 202,
      no_content = 204,
      multiple_choices = 300,
      moved_permanently = 301,
      moved_temporarily = 302,
      not_modified = 304,
      bad_request = 400,
      unauthorized = 401,
      forbidden = 403,
      not_found = 404,
      internal_server_error = 500,
      not_implemented = 501,
      bad_gateway = 502,
      service_unavailable = 503
  } response_code;
  
  void SetStatus(const ResponseCode response_code);
  void AddHeader(const std::string& header_name, const std::string& header_value);
  void SetBody(const std::string& body);
  
  std::string ToString();
  std::vector<std::pair<std::string, std::string>> GetHeaders();
  ResponseCode GetResponseCode();
  std::string GetBody();
  static std::unique_ptr<Response> ParseHeader(std::string header);

private:
	std::vector<std::pair<std::string, std::string>> response_header;
	std::string content;
    std::string response_status_string;
    ResponseCode  response_code_;   
    static char space ;
    static std::string header_separator;
	
};

#endif  