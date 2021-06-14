#ifndef HTTPSERVER_SERVER_H
#define HTTPSERVER_SERVER_H

#include"socket.h"
#include"httpParse.h"
#include"httpResponse.h"

#define BUFFERSIZE 1024

namespace server{

		class HttpServer{
				public:
						explicit HttpServer(int port = 80, const char *ip = nullptr):serverSocket(port, ip){
								serverSocket.bind();
								serverSocket.listen();
						}

						void run();

				private:
						void do_request(const sevsocket:: ClientSocket&);
						void header(const http:: H
