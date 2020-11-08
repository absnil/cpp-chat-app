#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include<string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/time.h>

char msg[1500]; 

sockaddr_in setupSockaddrStruct(const char* server, const char* port) {

   struct hostent* host = gethostbyname(server); 
   sockaddr_in sendSockAddr;   
   memset(&sendSockAddr, 0, sizeof(sendSockAddr)); 
   sendSockAddr.sin_family = AF_INET; 
   sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
   sendSockAddr.sin_port = htons(atoi(port));
   return sendSockAddr;
}

int createTcpClientSocket() {
    int client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0) {
        std::cout<< "Client socket creation error"<< std::endl;
    }
    return client;
}


int connectToServer(int client,sockaddr_in sendSockAddr) {
    int status = connect(client, (struct sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    if(status < 0)
    {
        std::cout<<"Error connecting to socket!"<<std::endl;    
    }
    std::cout<<"Connected to server"<< std::endl;
    return status;
}

void messaging(int client) {
    
    while(1)
    {
        char buffer[1500];
        std::string msg;
        memset(&buffer, 0, sizeof(buffer));
        std::cout<<"enter message to send\n";
        std::cin>>msg;
        strcpy(buffer, msg.c_str());
        std::cout<<"message : "<<buffer<<std::endl;
        send(client , buffer , strlen(buffer) , 0); 
        printf("Message sent\n");
        int valread = recv( client , buffer, 1500,0);
        if (valread >0) { 
        std::cout<< "Server sent reply : "<<buffer<<std::endl;
        }
    }
 }


void createClient(std::string server, std::string port) {
    sockaddr_in socketAddress = setupSockaddrStruct(server.c_str(), port.c_str());
    int clientFileDescriptor = createTcpClientSocket();
    int status = connectToServer(clientFileDescriptor, socketAddress);
    messaging(clientFileDescriptor);
}


int main(int argc, char* argv[]) {

    using namespace std;
     if(argc != 3)
    {
        cerr << "Usage: ip_address port" << endl; exit(0); 
    }

    const char *server = argv[1];
    const char *port = argv[2];
    createClient(server, port);
    

    return 0;
    
}