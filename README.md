# cpp-chat-app

This is a simple TCP client-server project using Sockets in C++.
The server is a repeating server. It recieves messages from the client and sends back the same message.
To run:
Clone the repository

Run
```
    g++ -o server simple_server.cpp
    g++ -o client simple_client.cpp
```
To compile the server and client

Use 

```
./server *port no*
```
to start the server

Use
```
./client *ip address of server* *port no of server*

```
to start the client