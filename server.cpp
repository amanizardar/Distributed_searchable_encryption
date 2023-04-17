#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include<bits/stdc++.h>
#include <cstdlib> 
#include <pthread.h>
#include <cstring>
#include "generator.cpp"

using namespace std;

#define PORT 8080


pair<string, pair<string,int>> split(string str){ 
	
	string option;
	string word;
	string num;
	int number_of_nodes;
	int i = 0;

	while(str[i] != '_'){
		option += str[i];
		i++;
	}
	i++;

	while(str[i] != '_'){
		word += str[i];
		i++;
	}
	i++;
	
	while(i < str.size()){
		num += str[i];
		i++;
	}

	number_of_nodes = stoi(num);

	pair<string, pair<string,int>> p = make_pair(option, make_pair(word,number_of_nodes));
	
	return p;

}







void* run_my_program(void *vargp) {

    std::cout << "Compiling my_program.cpp..." << std::endl;
	int compile_status = system("g++ compute.cpp -o compute");
	if (compile_status != 0) {
		std::cerr << "Error: Compilation failed." << std::endl;
		return NULL ;
	}
	std::cout << "Compilation successful." << std::endl;
	
	std::cout << "Running my_program..." << std::endl;
	// string command = ; // the command you want to send

	// std::cout << "Running my_program with command: " << command << std::endl;
	int run_status = system("./compute");
	return NULL;
}



int main()
{
	int server_fd, client_socket1, client_socket2,  valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	// char buffer[1024] = { 0 };
	

	// Creating socket file descriptor

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}


	if (setsockopt(server_fd, SOL_SOCKET,
				SO_REUSEADDR | SO_REUSEPORT, &opt,
				sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr*)&address,
			sizeof(address))
		< 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((client_socket1 = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    std::cout << "Client 1 connected successfully." << std::endl;
    
    
	// if ((new_socket
	// 	= accept(server_fd, (struct sockaddr*)&address,
	// 			(socklen_t*)&addrlen))
	// 	< 0) {
	// 	perror("accept");
	// 	exit(EXIT_FAILURE);
	// }




	char buffer[1024] = {0};

	


    while (true) {
        // Receive from client.cpp


        memset(buffer, 0, sizeof(buffer));

        if (recv(client_socket1, buffer, sizeof(buffer), 0) <= 0) {
            std::cout << "Client 1 disconnected." << std::endl;
            break;
        }
        std::cout << "THe word along with n.of nodes given by the Client 1: " << buffer << std::endl;

		// splitting the word and number of nodes

		pair<string, pair<string,int>> p = split(buffer); //option,word,num_nodes

		cout << "the option is :  "  << p.first << endl;
		cout << "the word is :  "  << p.second.first << endl;
		cout << " the num is  : " << p.second.second << endl;



		// function call to sha 512 encryption.

		string hash512 =  sha512(p.second.first);



		/////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////

		// compile and run the compute.cpp program

		
    
    	// Create a thread for running my_program

    	// thread t1(run_my_program);
		pthread_t t1;
		pthread_create(&t1, NULL, run_my_program, NULL);
		


		std::cout << "Listening for incoming connections..." << std::endl;
		sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
		
			// Accept incoming connections
		int client_socket2 = accept(server_fd, (sockaddr*)&client_addr, &client_addr_len);
		if (client_socket2 < 0) {
			std::cerr << "Error: Failed to accept incoming connection." << std::endl;
			continue;
		}
		// std::cout << "Accepted incoming connection from " << inet_ntoa(client_addr.sin_addr) << std::endl;
		
		// Send a message to the client

		string encrypted_word = hash512;
		char* data = const_cast<char*>(encrypted_word.c_str());
		send(client_socket2, data, strlen(data), 0);

		
		
		// Receive a message from the client
		char buffer[1024] = {0};
		int num_bytes = recv(client_socket2, buffer, sizeof(buffer), 0);
		if (num_bytes < 0) {
			std::cerr << "Error: Failed to receive message from client." << std::endl;
		} else {
			std::cout << "Received message from client: " << buffer << std::endl;
		}
		
		// Close the client socket
		close(client_socket2);
		


		pthread_join(t1, NULL);
    	
		

		// Send to client 1
        send(client_socket1, buffer, strlen(buffer), 0);

    }
    
    // Close sockets
    close(client_socket1);
    
    close(server_fd);
    
	
	// closing the listening socket
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}
