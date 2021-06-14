#ifndef HTTPSERVER_SOCKET_H
#define HTTPSERVER_SOCKET_H

#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<iostream>
#include<stdlib.h>

namespace sevsocket{
		class ClientSocket;

		void setReusePort(int fd);

		class ServerSocket{

				public:
						ServerSocket(int port = 8080, const char *ip= nullptr);
						~ServerSocket();

						void bind();

						void listen();

						int accept(ClientSocket &);

				public:
						sockaddr_in mSevAddr;
						int fd; 
						int mSevPort;
						const char *mSevIP;
		};

		
		class ClientSocket{

				public:
						~ClientSocket();
						socklen_t mAddrLen;
						sockaddr_in mCliAddr;
						int fd;
		};

} // sevsocket


#endif 
