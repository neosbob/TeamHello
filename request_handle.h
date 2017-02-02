#ifndef ECHO_REQUEST_HANDLER_H
#define ECHO_REQUEST_HANDLER_H

#include <string>
#include "handlerBase.h"



class session : public HandlerBase
{
public:
    session(boost::asio::io_service& io_service)
        : HandlerBase(io_service)
    {
        
    }
    virtual std::string buildResponseStr(std::string body);
    
private: 

};


#endif // ECHO_REQUEST_HANDLER_H
