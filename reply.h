#ifndef REPLY_H
#define REPLY_H

#include <string>
#include <map>


class http_headers
{
public:
    std::string parse_request_line(std::string line);
    
    std::string parse_header(std::string line);

    bool setMap(std::map<std::string, std::string> m);
    
    int content_length();
   
private:
    std::string method;
    std::string url;
    std::string version;
    std::map<std::string, std::string> headers;
};

#endif // REPLY_H
