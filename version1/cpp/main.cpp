#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<iostream>
#include<unistd.h>
#include<../include/server.h>



int main(int argc , char *argv[]){
		
		char *buffer;
		if( ( buffer = getget(NULL, 0)) == NULL)
				perror("getcwd error");
		else{
				printf("%s\n", buffer);
				free(buffer);
		}

		server::HttpServer httpServer(80);
		httpServer.run();




		return 0;
}
