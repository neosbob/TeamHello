//Inspired by minimal server design by phillipvoyle
//https://www.codeproject.com/Articles/1007262/A-minimal-http-web-server-using-boost-asio#_comments

#include <iostream>
#include "reply.h"
#include <sstream>

   
int http_headers::content_length()
{
    auto request = headers.find("content-length");
    //This only applies if a header of "content-length" is specified
    //If found, don't enter if statement.
    if(request == headers.end())
        return 0;
    else if(request != headers.end())
    {
        std::stringstream ssLength(request->second);
        int content_length;
        ssLength >> content_length;
        return content_length;
    }
}
   
std::string http_headers::parse_request_line(std::string line)
{
    //This is a check for what url the request wants.

    std::stringstream ssRequestLine(line);
    ssRequestLine >> method;
    ssRequestLine >> url;
    ssRequestLine >> version;
  
    std::cout << "request for resource: " << url << std::endl;
    return url;
}
 
std::string http_headers::parse_header(std::string line)
{  
    //only applies for lines after the first line
    //Maps specific headers to a certain value
    //Ex. Host: localhost. Host maps to localhost.

    std::stringstream ssHeader(line);
    std::string headerName;
    std::getline(ssHeader, headerName, ':');
  
    std::string value;
    std::getline(ssHeader, value);
    headers[headerName] = value;

    return headerName+":"+value;	
}

bool http_headers::setMap(std::map<std::string, std::string> m)
{
	headers=m;
	if (headers.empty()) 
		return false;
	else
		return true;
}
