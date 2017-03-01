#include "http_client.h"

HTTPClient::HTTPClient(): io_service_(), socket_(io_service_)
{ 
}

HTTPClient::~HTTPClient() {
    socket_.shutdown(tcp::socket::shutdown_both);
}

bool HTTPClient::Connect(std::string host, std::string port) {
    tcp::resolver resolver(io_service_);
    tcp::resolver::query query(host, port);
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    boost::system::error_code error;
    boost::asio::connect(socket_, endpoint_iterator, error);

    // check error
    if(error){
        std::cout<<"Error: "<<error.message()<<std::endl;
        return false;
    }
    else{
        return true;
    }
}

bool HTTPClient::SendRequest(std::string request) {
    boost::asio::streambuf requestbuf;
    std::ostream request_stream(&requestbuf);
    request_stream << request;
    if(boost::asio::write(socket_, requestbuf) == request.length()){
        return true;
    }
    else{
        return false;
    }
}

std::unique_ptr<Response> HTTPClient::ObtainResponse() {
    boost::asio::streambuf resp;
    std::unique_ptr<Response> response;
    boost::system::error_code error;
    boost::asio::read_until(socket_, resp, "\r\n\r\n");
    std::string str( (std::istreambuf_iterator<char>(&resp)), std::istreambuf_iterator<char>() );
    response = Response::ParseHeader(str);
    while (boost::asio::read(socket_, resp, boost::asio::transfer_at_least(1), error)) {
        if (error) break;
    }
    std::string body( (std::istreambuf_iterator<char>(&resp)), std::istreambuf_iterator<char>() );
    std::string whole = str+body;
    body = whole.substr(whole.find("\r\n\r\n")+4);
    response->SetBody(body);

    return response;
}
