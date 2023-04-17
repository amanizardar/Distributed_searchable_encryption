#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <bits/stdc++.h>


#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

using namespace std;

int main(int argc, char const* argv[])
{
    // int server_fd, new_socket, valread;
    // struct sockaddr_in address;
    // int opt = 1;
    // int addrlen = sizeof(address);
    // char buffer[1024] = { 0 };
    // char* hello = "Hello from server";
  
    // // Creating socket file descriptor
    // if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    //     perror("socket failed");
    //     exit(EXIT_FAILURE);
    // }
  
    // // Forcefully attaching socket to the port 8080
    // if (setsockopt(server_fd, SOL_SOCKET,
    //                SO_REUSEADDR | SO_REUSEPORT, &opt,
    //                sizeof(opt))) {
    //     perror("setsockopt");
    //     exit(EXIT_FAILURE);
    // }
    // address.sin_family = AF_INET;
    // address.sin_addr.s_addr = INADDR_ANY;
    // address.sin_port = htons(PORT);
  
    // // Forcefully attaching socket to the port 8080
    // if (bind(server_fd, (struct sockaddr*)&address,
    //          sizeof(address))
    //     < 0) {
    //     perror("bind failed");
    //     exit(EXIT_FAILURE);
    // }
    // if (listen(server_fd, 3) < 0) {
    //     perror("listen");
    //     exit(EXIT_FAILURE);
    // }
    // if ((new_socket
    //      = accept(server_fd, (struct sockaddr*)&address,
    //               (socklen_t*)&addrlen))
    //     < 0) {
    //     perror("accept");
    //     exit(EXIT_FAILURE);
    // }
    // valread = read(new_socket, buffer, 1024);
    // printf("%s\n", buffer);
    // send(new_socket, hello, strlen(hello), 0);
    // printf("Hello message sent\n");
  
    // // closing the connected socket
    // close(new_socket);
    // // closing the listening socket
    // shutdown(server_fd, SHUT_RDWR);


    int s_socket;
    string ip="127.0.0.1";
    string port="8080";
    s_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (s_socket == -1)
    {
        cout << "Error in connection from Server side.\n";
        exit(EXIT_FAILURE);
    }
    int opt = 1;
    if (setsockopt(s_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip.c_str());
    // inet_aton(ip, &server_address.sin_addr);
    server_address.sin_port = htons(stoi(port));

    if (bind(s_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        cout << "Binding Error" << endl;
        exit(EXIT_FAILURE);
    }

    if (listen(s_socket, 3) < 0)
    {
        cout << "Listening Error\n";
        exit(EXIT_FAILURE);
    }

  

  
        
        cout <<setw(50)<< "\nListening for peers on ip " << ip << " "<< "and Port " << port <<"..."<<endl;

        int client_socket;
        int addrlen;
        if ((client_socket = accept(s_socket, (struct sockaddr *)&server_address, (socklen_t *)&addrlen)) < 0)
        {
            cout << "Accept Error\n";
        }
        else
        {
            cout << "Connection with peer Established.\n" << endl;
        }

        string str = "9b71d224bd62f3785d96d46ad3ea3d73319bfbc2890caadae2dff72519673ca72323c3d99ba5c11d7c7acc6e14b8c5da0c4663475c2e5c3adef46f73bcdec043";
        char charArray[129];
        strcpy(charArray, str.c_str());
        // cout<<charArray;
        
        send(client_socket, charArray, sizeof(charArray), 0);

        char recvans[129];
        recv(client_socket, recvans, sizeof(recvans), 0);
        string ans(recvans);
        cout<<recvans;

       

    
 

    close(s_socket);


    return 0;
}