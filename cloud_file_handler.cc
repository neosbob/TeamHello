//Inspired by minimal server design by phillipvoyle
//https://www.codeproject.com/Articles/1007262/A-minimal-http-web-server-using-boost-asio#_comments

#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <utility>
#include "cloud_file_handler.h"
#include "not_found_handler.h"
#include "cpp-markdown/markdown.h"

using namespace boost;
using namespace boost::system;
using namespace boost::asio;
using boost::asio::ip::tcp;

class mime_types;

RequestHandler::Status CloudHandler::Init(const std::string& uri_prefix, const NginxConfig& config)
{   
    bool remote_host_found = false;

    if (config.statements_[0]->tokens_[0] == "root" && config.statements_[0]->tokens_.size() == 2)
    {
        root_dir_ = config.statements_[0]->tokens_[1];
        uri_prefix_ = uri_prefix; //in this case "/cloud"
	//remote_host_found = parse_remote_url(root_dir_);         
        return RequestHandler::Status::OK;
    }
    return RequestHandler::Status::FAILED;
}

RequestHandler::Status CloudHandler::HandleRequest(const Request& request, Response* response)
{
    std::string uri_path = request.uri();
    std::string request_path, static_path, sfile_dir;
    std::string full_path;
    std::size_t url_found = uri_path.find(this->uri_prefix_, 0);
    if (url_found != std::string::npos)
    {
        request_path = uri_path.erase(url_found, this->uri_prefix_.length());     
    }
    if (request_path != "/" && request_path != "") 
    {
        // Determine the file extension.
        std::size_t last_slash_pos = request_path.find_last_of("/");
        std::size_t last_dot_pos = request_path.find_last_of(".");
        std::string extension;
        if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
        {
            extension = request_path.substr(last_dot_pos + 1);
        }
        std::cout << "resource path: " << request_path << std::endl;
        // Open the file to send back.
        if(request_path.substr(0,1) != "/")
            request_path = "/" + request_path;

	//full_path will be the path of the object from the cloud storage.
        full_path = this->root_dir_ + request_path;
        std::cout<< full_path;

	std::string content = "";
        // Fill out the reply to be sent to the client.
	content = get_cloud_object(root_dir_, request_path);
    
	//determine reponse_status, response_headers.
        mime_types mime;
        if(response_status == "HTTP/1.1 200 OK\r\n")
            response->SetStatus(Response::ok);
        else if (response_status == "HTTP/1.1 404 Not Found\r\n")
	     response->SetStatus(Response::not_found);
        //get the content type
        std::string extension_type = mime.extension_to_type(extension);
        //check if the content type is markdown. set to html
        if (extension_type == "text/markdown")
        {
        response->AddHeader("content-type", "text/html");
        }
        else
        {
            for (std::vector<std::pair<std::string, std::string>>::const_iterator it = headers_.begin(); it != headers_.end(); it++)
            {
                response->AddHeader(it->first, it->second);
            };
        }

        //set response body
        if (extension_type == "text/markdown")
        {
            markdown::Document doc;
            doc.read(content);
            std::ostringstream stream;
            doc.write(stream);   
            std::string markdown = stream.str();
            response->AddHeader("content-length",std::to_string(markdown.length()));
            response->SetBody(markdown);
        }
        else
        {
            response->SetBody(content);
        }
	headers_.clear();
        return RequestHandler::Status::OK;
    }
    else if (request_path == "/")
    {
        return RequestHandler::Status::FILE_NOT_FOUND;     
    }
    else 
    {
        return RequestHandler::Status::FILE_NOT_FOUND;
    }
}

std::string CloudHandler::get_cloud_object(std::string remote_dir, std::string remote_file_path)
{
    std::string host_url = remote_dir;
    std::string path = remote_file_path;

    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    std::cout<<host_url<<std::endl;
    tcp::resolver::query query(host_url, "80");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::resolver::iterator iter = endpoint_iterator;
    tcp::resolver::iterator end;

    while (iter != end)
    {
      tcp::endpoint endpoint = *iter++;
      std::cout << endpoint << std::endl;
    }

    tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);

    std::cout << "Successfully connected!\n";

    
    std::string request_string = std::string("GET ") + path + " HTTP/1.1\r\n" + "Host: " + host_url + ":" + "80";

    request_string += std::string("\r\nConnection: keep-alive\r\n") + "Accept: text/html\r\n" + "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n" + "Accept-Encoding: gzip, deflate, sdch\r\n" + "Accept-Language: en-US,en;q=0.8\r\n" + "\r\n";
                                     
    std::cout << request_string<<std::endl;

    // Write request to remote server
    socket.write_some(boost::asio::buffer(request_string, request_string.size()));

    std::string remote_response;

    while(1) {
      
        boost::array<char, 4096> buf;
        boost::system::error_code error;

	//read the response from remote server
        size_t len = socket.read_some(boost::asio::buffer(buf), error);

        if (error == boost::asio::error::eof) {
            std::cout <<"Exited cleanly\n";
            break; // Connection closed cleanly by peer.
        }
        else if (error)
            throw boost::system::system_error(error); // Some other error.

	
        //std::cout.write(buf.data(), len);
        remote_response += std::string(buf.data(), len);

        
    }

    std::cout<<remote_response<<std::endl;

    std::string parsed_response;
    bool parsed_response_status;

    parsed_response_status = parse_remote_response(remote_response);
    if(parsed_response_status  == false)
        return "";
    parsed_response = response_body;

    return parsed_response;

}

bool CloudHandler::parse_remote_response(std::string remote_response)
{
    std::size_t status_found = remote_response.find("\r\n");
    if(status_found == std::string::npos)
	return false;
    response_status = remote_response.substr(0, status_found + 2);
    rest = remote_response.substr(status_found + 2);        

    std::size_t header_found = rest.find("\r\n\r\n");
    if(header_found == std::string::npos)
	return false;
    response_headers = rest.substr(0, header_found + 3);
    response_body = rest.substr(header_found + 4);

    read_header(response_headers);  

    return true;
    	
}

bool CloudHandler::read_header(std::string headers)
{
  std::string temp_headers = headers;
  std::string line;
  while(temp_headers.size() > 0 && temp_headers != "\r" && temp_headers != "\n")
  {
    std::size_t header_found = temp_headers.find("\r\n", 0);
    if (header_found != std::string::npos){
        line = temp_headers.substr(0, header_found+1);
	temp_headers = temp_headers.substr(header_found+2);
    }
    std::stringstream ssHeader(line);

    std::string headerName;
    std::getline(ssHeader, headerName, ':');

    ssHeader >> std::ws; 
    std::string value;
    std::getline(ssHeader, value, '\r');
    
    std::pair<std::string, std::string> header_pair(headerName, value);
    this->headers_.push_back(header_pair);
  }
  return 1;
	
}


