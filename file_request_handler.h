#ifndef FILE_REQUEST_HANDLER_H
#define FILE_REQUEST_HANDLER_H

#include <string>
#include "handlerBase.h"



class FileRequestHandler : public HandlerBase
{
public:
    FileRequestHandler(boost::asio::io_service& io_service)
        : HandlerBase(io_service)
    {
        
    }
    virtual std::string buildResponseStr(const std::string& body);
    
private: 
    // Use this function to get the content of file. And then
    // you can append the content to the reply message in 
    // other functions
    std::string getFileContent(const std::string& fileName); 
};


#endif // FILE_REQUEST_HANDLER_H
