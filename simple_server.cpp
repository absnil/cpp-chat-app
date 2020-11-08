#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include<string.h>
#include <sys/time.h>

sockaddr_in setupSockaddrStruct(int port) {
    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress)); 
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(port); 
    return serverAddress;
}

int createTcpSocket() {
    int server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) {
        std::cout<< "Server socket creation error"<< std::endl;
    }
    return server;
}

int bindSocket(int socketfd, sockaddr_in sockAddr) {
    int status = bind(socketfd, (struct sockaddr*) &sockAddr, sizeof(sockAddr));
    if (status < 0) {
        std::cout << "Socket not binding to address properly" << std::endl;
    }
    return status;
}
int createServer (int port) {

    sockaddr_in socketAddress = setupSockaddrStruct(port);
    int serverFileDescriptor = createTcpSocket();
    int bindStat = bindSocket(serverFileDescriptor, socketAddress);
    return serverFileDescriptor;
}
int listenAndAcceptConns(int server) {

    listen(server, 4);
    sockaddr_in clientSockAddress;
    socklen_t clientSockAddrSize = sizeof(clientSockAddress);
    int clientFileDescriptor = accept(server, (sockaddr *)&clientSockAddress, &clientSockAddrSize);
    std::cout << "Connected with client!" << std::endl;
    return clientFileDescriptor;
}

void messaging(int client) {
    while(1)
    {
    char buffer[1500];
    memset(&buffer, 0, sizeof(buffer));
       int valread = recv( client , buffer, 1500, 0);
       if (valread >0) { 
        std::cout<< "Client: "<<buffer<<std::endl; 
        send(client , buffer , strlen(buffer) , 0 ); 
        printf("Message sent\n");
       }
    }
}

void closeSockets(int server, int client) {
    
    close(server);
    close(client);
    std::cout << "Connection closed..." << std::endl;

}


void doServerThings(int server) {
    std::cout<< "Listening for Clients"<<std::endl;
    int client = listenAndAcceptConns(server);

    messaging(client);
    closeSockets(server, client);
}

int main(int argc, char *argv[]) {

     using namespace std;
    if(argc != 2)
    {
        cerr << "Usage: port" << endl;
        exit(0);
    }
    cout << "Welcome to the server!!!" << endl;
   
    int port = atoi(argv[1]);
    int server = createServer(port);
    doServerThings(server);
    return 0;
}


