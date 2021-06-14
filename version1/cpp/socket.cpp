#include"../include/socket.h"


void sevsocket::setReusePort(int fd){
		int opt = 1;
		setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void*) &opt, sizeof(opt));
}


sevsocket::ServerSocket::ServerSocket(int port, const char *ip): mSevPort(port), mSevIP(ip){
		bzero(&mSevAddr, sizeof(mSevAddr));
		mSevAddr.sin_family = AF_INET;
		mSevAddr.sin_port   = htons(port);
		if(ip != nullptr){
				::inet_pton(AF_INET, ip, &mSevAddr.sin_addr);
		} else{
				mSevAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		}

		fd = socket(AF_INET, SOCK_STREAM, 0);
		if(fd == -1){
				std::cout<<"create socket error in file <" <<  __FILE__  <<"> "<<  "at "  <<   __LINE__    << std::endl;
				exit(1);
		}

		setReusePort(fd);

}


void sevsocket:: ServerSocket:: bind(){

		int ret = :: bind(fd, (struct sockaddr*)&mSevAddr, sizeof(mSevAddr));
		if(ret == -1){
				std:: cout<< "bind error " << __FILE__ << " at " << __LINE__ << std::endl;
				exit(1);
		}

}

void sevsocket:: ServerSocket:: listen(){

		int ret = :: listen(fd, 128);
		if(ret == -1){
				std:: cout<< "listen error "<< __FILE__ << "at "<< __LINE__ << std::endl;
				exit(1);
		}

}


int sevsocket:: ServerSocket(sevsocket:: ClientSocket & clientsoc){

		int clientfd = :: accept(fd, (struct sockaddr*)&clientsoc.mCliAddr, mAddrLen);
		if(clientfd < 0){
				perror("accept error 1");
				exit(1);
		}

		clientsoc.fd = clientfd;
		std:: cout<< "accept a client "<< std:: endl;
		return clientfd;
}


sevsocket:: ServerSocket:: ~ServerSocket(){

		:: close(fd);
}

sevsocket:: ServerSocket:: ~ClientSOcket(){
		::close(fd);
}





