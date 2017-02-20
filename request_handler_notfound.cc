#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <utility>
#include "request_handler.h"
#include "request_handler_notfound.h"

using namespace boost;
using namespace boost::system;
using namespace boost::asio;

class mime_types;   

std::string reply_notfound::get_response(std::string echoback, std::string base_path)
{
	std::stringstream ssOut;
	std::string sHTML = "<html><body><p>404 NOT FOUND. No Handler Match!</p></body></html>";
        ssOut << "HTTP/1.1 404 Not Found" << std::endl;
        ssOut << "content-type: text/html" << std::endl;
        ssOut << "content-length: " << sHTML.length() << std::endl;
        ssOut << std::endl;
        ssOut << sHTML;

	return ssOut.str();
}
