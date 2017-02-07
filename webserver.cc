#include "webserver.h"
#include <string>
#include <memory>
#include <iostream>
#include <cstdlib>
#include "config_parser.h"
#include <utility>
#include <algorithm>
//#include "handlerBase.h"

using namespace boost;
using namespace boost::system;
using namespace boost::asio;

class session;

Server::Server(const configArguments& configArgs)
: io_service()
, acceptor(io_service, ip::tcp::endpoint(ip::tcp::v4(), configArgs.port))
{
    (this->acceptor).listen();
    this->configContent = configArgs;

    doAccept();
}

void Server::doAccept()
{
    

    //std::cout << configContent.handlerType;
        std::shared_ptr<session> sesh = std::make_shared<session>(io_service, configContent.map_path_rootdir, configContent.echo_path, configContent.static_path);
        acceptor.async_accept(sesh->socket, [sesh, this](const error_code& accept_error)
        {
            if(!accept_error)
            {
                session::read_request(sesh);
            }
            doAccept();
        });

}

void Server::run()
{
    std::cout<<"Server is running!\n";
    io_service.run();
}

int Server::parseConfig(int argc, const char * argv[], configArguments& configArgs)
{	
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage ./webserver <path to config file>\n";
            return 1;
        }
        NginxConfigParser config_parser;
	//NginxConfigStatement config_blocks;
        NginxConfig config_out;

        if (config_parser.Parse(argv[1], &config_out)) 
        {
            for (int i = 0; i < config_out.statements_.size(); i++)
            {
                std::string header = config_out.statements_[i]->tokens_[0];
                if (header == "port")
                {
                    if (config_out.statements_[i]->tokens_.size() > 1 && std::all_of(config_out.statements_[i]->tokens_[1].begin(), config_out.statements_[i]->tokens_[1].end(), ::isdigit))
                    {
                        unsigned int tmpPort = std::stoi(config_out.statements_[i]->tokens_[1]);
                        if (tmpPort > 65535 || tmpPort < 0)
                        {
                            std::cerr << "The port number " << tmpPort << " in config file is invalid.\n";
                            return 2;
                        }
                        configArgs.port = (short unsigned int)tmpPort;
                    }
                    else
                    {
                        std::cerr << "Please specify a port number.\n";
                        return 3;
                    }
                }
                
                else if (header == "path")
                {
                    if (config_out.statements_[i]->tokens_.size() > 2)
                    {
                        if (config_out.statements_[i]->tokens_[1] == "/echo")
                        {
	                        configArgs.echo_path = config_out.statements_[i]->tokens_[1];
                        }
                        else if (config_out.statements_[i]->tokens_[1].find("/") == 0 && config_out.statements_[i]->tokens_[2] == "reply_static")
                        {
                            configArgs.static_path = config_out.statements_[i]->tokens_[1];
			    std::shared_ptr<NginxConfigStatement> temp_config = config_out.statements_[i];
			    if (temp_config->child_block_->statements_[0]->tokens_[0] == "root" && temp_config->child_block_->statements_[0]->tokens_.size() > 1)
                            {
                                std::string baseDirectory = temp_config->child_block_->statements_[0]->tokens_[1];
				configArgs.map_path_rootdir.insert(std::pair<std::string, std::string>(configArgs.static_path, baseDirectory));
                            }
			    else
                   	    {
                                std::cerr << "Please specify a base path for request handler.\n";
                                return 3;
                            }
                        }
                        else
                        {
                            std::cerr << "Unrecognized server type: "+ config_out.statements_[i]->tokens_[1] +".\n";
                            return 3;
                        }
                    }
                    else
                    {
                        std::cerr << "Please specify a path.\n";
                        return 3;
                    }
                }
            }
        }
        else 
        {
            std::cerr << "Could not parse config file.\n";
            return 3;
        }
    }

    catch(std::exception& error) 
    {
        std::cerr<< "Exception: "<< error.what() << "\n";
        return 4;
    }

    return 0;
}

      
