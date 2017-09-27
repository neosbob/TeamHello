//Inspired by minimal server design by phillipvoyle
//https://www.codeproject.com/Articles/1007262/A-minimal-http-web-server-using-boost-asio#_comments

#include <boost/asio.hpp>
#include <string>
#include <algorithm>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <utility>
#include "file_handler.h"
#include "not_found_handler.h"
#include "cpp-markdown/markdown.h"
#include <boost/algorithm/string.hpp>

using namespace boost;
using namespace boost::system;
using namespace boost::asio;

class mime_types;

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}

std::string parse_Qstring(std::string query_string) {
    std::string s = query_string;
    std::replace( s.begin(), s.end(), '&', ' '); // replace all '&' to ' '
    size_t pos = s.find("comment=", 0) + 8;
    std::string o_string = s.substr(0, pos);
    
    std::string fix_comment;
    if (pos != std::string::npos)
    {
        std::string comment = s.substr(pos);
        fix_comment = "\"" + comment + "\"";
    }

    std::string final_string = o_string + fix_comment;

    return final_string;
}

RequestHandler::Status StaticHandler::Init(const std::string& uri_prefix, const NginxConfig& config)
{   
    if (config.statements_[0]->tokens_[0] == "root" && config.statements_[0]->tokens_.size() == 2)
    {
        this->root_dir = config.statements_[0]->tokens_[1];
        this->prefix = uri_prefix;
        return RequestHandler::Status::OK;
    }
    return RequestHandler::Status::FAILED;
}

RequestHandler::Status StaticHandler::HandleRequest(const Request& request, Response* response)
{
    std::string uri_path = request.uri();
    std::string request_path, static_path, sfile_dir;
    std::string full_path;
    std::size_t url_found = uri_path.find(this->prefix, 0);
    
    if (url_found != std::string::npos)
    {
        request_path = uri_path.erase(url_found, this->prefix.length());     
    }

    if (request_path != "/" && request_path != "") 
    {
        // Determine the file extension.
        std::size_t last_slash_pos = request_path.find_last_of("/");
        std::size_t last_dot_pos = request_path.find_last_of(".");
	std::size_t form_input_begin = request_path.find_last_of("?");
        std::string extension;
	std::string form_input;
        
	if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
        {
            extension = request_path.substr(last_dot_pos + 1);
	    if(form_input_begin != std::string::npos)
	    {
                form_input = request_path.substr(form_input_begin + 1);
		form_input = parse_Qstring(form_input);               
		request_path = request_path.substr(0, form_input_begin);
                std::size_t last_dot_pos_FI = request_path.find_last_of(".");
		extension = request_path.substr(last_dot_pos_FI + 1);
            }
            
        }
        std::cout << "resource path: " << request_path << std::endl;
	std::cout << "form input: " << form_input << std::endl;
        
        // Open the file to send back.
        if(request_path.substr(0,1) != "/")
            request_path = "/" + request_path;
        full_path = this->root_dir + request_path;
        std::cout<< full_path << std::endl;;
        std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
        if (!is)
        {
            return RequestHandler::Status::FILE_NOT_FOUND; 
        }
        
	// Fill out the reply to be sent to the client.
        std::string content = "";
        mime_types mime;
        char buf[512];
        while (is.read(buf, sizeof(buf)).gcount() > 0)
            content.append(buf, is.gcount());

	//php handling
	if(mime.extension_to_type(extension) == "text/html; charset=UTF-8")
	{
	    std::cout<< exec("which make")<< std::endl;
            std::string form_input_command;
	    if(form_input != "")
            {
                form_input_command = " " + form_input;
            }
	    std::string php_command = "php-cgi -f " + full_path + form_input_command;
            std::cout << php_command << std::endl;
	    content = exec(php_command.c_str());
	    std::cout<< content << "BOB"<< std::endl;
	}	

        response->SetStatus(Response::ok);
        //get the content type
        std::string extension_type = mime.extension_to_type(extension);
        //check if the content type is markdown. set to html
        if (extension_type == "text/markdown")
        {
        response->AddHeader("content-type", "text/html");
        }
        else
        {
            response->AddHeader("content-type", extension_type);
            response->AddHeader("content-length", std::to_string(content.length()));
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
	    //std::cout<< "BOB" << content << std::endl;
        }
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


