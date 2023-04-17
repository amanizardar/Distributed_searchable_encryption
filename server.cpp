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

int STORE_IN_DB(string encrypted_word)
{
	ofstream database_file("hash.txt", ios::app);
	if (!database_file)
	{
		cout << "There was an error opening file for output" << endl;
		
		return -1;	
	}	

	database_file<<encrypted_word<<endl;
	database_file.close();
	return 1;
}


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
	int *processespoint = (int *)vargp;
    int processes = *processespoint;

    std::cout << "Compiling my_program.cpp..." << std::endl;
	int compile_status = system("mpic++ compute.cpp -o compute");
	if (compile_status != 0) {
		std::cerr << "Error: Compilation failed." << std::endl;
		return NULL ;
	}
	std::cout << "Compilation successful." << std::endl;
	
	std::cout << "Running my_program..." << std::endl;
	string running_cmd = "mpirun -np " + to_string(processes) + " ./compute";
	int run_status = system(running_cmd.c_str());
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
	cout<<"Listening..."<<endl;

	if ((client_socket1 = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    std::cout << "Client 1 connected successfully." << std::endl;
    
    



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

		cout << "The option is : "  << p.first << endl;
		cout << "The word is :  "  << p.second.first << endl;
		cout << "The num is  : " << p.second.second << endl;
		int nop = p.second.second;


		


		pthread_t t1;
		pthread_create(&t1, NULL, run_my_program, &nop);
		


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
		// function call to sha 512 encryption.
		string encrypted_word =  sha512(p.second.first);
		char* data = const_cast<char*>(encrypted_word.c_str());
		send(client_socket2, data, strlen(data), 0);

		
		
		// Receive a message from the Compute
		char buffer[1024] = {0};
		int num_bytes = recv(client_socket2, buffer, sizeof(buffer), 0);
		if (num_bytes < 0) {
			std::cerr << "Error: Failed to receive message from Compute." << std::endl;
		} else {
			std::cout << "Received message from Compute: " << buffer << std::endl;
		}



		
		// Close the client socket
		close(client_socket2);

		if(p.first=="1")
		{
			string res(buffer);
			if(res=="NO")
			{
				int res = STORE_IN_DB(encrypted_word);

				if(res==-1)
				{
					string temp = "The Updation in the DB is not possible due to an error";
					char tempbuff[1024];
					strcpy(tempbuff,temp.c_str());
					send(client_socket1, tempbuff, sizeof(tempbuff), 0);
				}
				else
				{
					string temp = "The Word has been added into the Database";
					char tempbuff[1024];
					strcpy(tempbuff,temp.c_str());
					send(client_socket1, tempbuff, sizeof(tempbuff), 0);
				}
			}
			else
			{
				send(client_socket1, buffer, sizeof(buffer), 0);
			}
		}
		


		pthread_join(t1, NULL);
    	
		

		// Send to client 1
		if(p.first!="1")
        send(client_socket1, buffer, sizeof(buffer), 0);

		

    }
    
    // Close sockets
    close(client_socket1);
    
    close(server_fd);
    
	
	// closing the listening socket
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}
