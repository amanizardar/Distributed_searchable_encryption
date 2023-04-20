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


// Color codes
const std::string red("\033[0;31m");
const std::string green("\033[1;32m");
const std::string yellow("\033[1;33m");
const std::string cyan("\033[0;36m");
const std::string magenta("\033[0;35m");
const std::string reset("\033[0m");
// End of color codes

using namespace std;

#define PORT 8080

int STORE_IN_DB(string encrypted_word)
{
	ofstream database_file("hash.txt", ios::app);
	if (!database_file)
	{
		cout <<red<< "There was an error opening file for output" << cyan<<endl;
		
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







void* run_my_program(void *vargp) 
{
	int *processespoint = (int *)vargp;
    int processes = *processespoint;

    std::cout <<"==> Distributing work to different processes" << endl;
	int compile_status = system("mpic++ compute.cpp -o compute");
	if (compile_status != 0) {
		std::cerr <<red<< "Error: Something went wrong " << cyan<<endl;
		return NULL ;
	}
	
	string running_cmd = "mpirun -np " + to_string(processes) + " ./compute";
	int run_status = system(running_cmd.c_str());
	return NULL;
}



int main()
{
	 system("clear");
	cout <<green<<R"(
SSSSS  EEEEE   AAA   RRRR   CCCCC  H   H    A    BBBBB  L     EEEEE
S      E      A   A  R  R  C       H   H   A A   B   B  L     E
SSSSS  EEEEE  AAAAA  R R   C       HHHHH  A   A  BBBBB  L     EEEEE
    S  E      A   A  R  R  C       H   H  AAAAA  B   B  L     E
SSSSS  EEEEE  A   A  R   R  CCCCC  H   H  A   A  BBBBB  LLLLL EEEEE

    )"<<reset;



	cout <<green<<R"(
EEEE  N    N   CCCC  RRRR  Y   Y  PPPPP  TTTTT  IIII  OOOOO  N    N
E     N N  N  C      R  R   Y Y   P   P    T     II   O   O  N N  N
EEEE  N  N N  C      R R     Y    PPPPP    T     II   O   O  N  N N
E     N   NN  C      R  R    Y    P        T     II   O   O  N   NN
EEEE  N    N   CCCC  R   R   Y    P        T    IIII  OOOOO  N    N

    )"<<reset;


cout<<"\n\n\n\n\n";
cout<<red<<"##############################################################################\n\n"<<reset;
cout<<yellow<<"                         WElCOME TO THE SEARCHABLE ENCRYPTION\n";
cout<<"                             Server Starting...\n\n";


	



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
	if (listen(server_fd, 20) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	cout<<"Listening..."<<endl<<reset<<cyan;

	if ((client_socket1 = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    std::cout << "==> Client connected successfully." << std::endl;
    
    



	char buffer[1024] = {0};

	


    while (true) 
	{
        // Receive from client.cpp


        memset(buffer, 0, sizeof(buffer));

        if (recv(client_socket1, buffer, sizeof(buffer), 0) <= 0) {
            	cout << "==> Unable to recieve data from the Client." << endl;
            break;
        }

		pair<string, pair<string,int>> p = split(buffer); //option,word,num_nodes

		string option = p.first;
		string encrypted_word =  p.second.first;
		int no_of_processes = p.second.second;

		pthread_t t1;
		pthread_create(&t1, NULL, run_my_program, &no_of_processes);
		


		std::cout << "==> Listening for incoming connections..." << std::endl;
		sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
		
			// Accept incoming connections
		int client_socket2 = accept(server_fd, (sockaddr*)&client_addr, &client_addr_len);
		if (client_socket2 < 0) {
			std::cerr << "Error: Failed to accept incoming connection." << std::endl;
			continue;
		}


		char charArray[129];
        strcpy(charArray, encrypted_word.c_str());  
        send(client_socket2, charArray, sizeof(charArray), 0);

		
		// Receive a message from the Compute
		char buffer[1024] = {0};
		int num_bytes = recv(client_socket2, buffer, sizeof(buffer), 0);  // Buffer will get Either Yes or NO.
		if (num_bytes < 0) {
			cerr << "Error: Failed to receive message from Compute." << endl;
		} else {
			cout << "==> Received message from Compute: " << buffer << endl;
		}



		pthread_join(t1, NULL);

		
		// Close the client socket
		close(client_socket2);

		if(option=="1")
		{
			string res(buffer);
			if(res=="NO")   //means Word is not present in DB, So we need to store it.
			{
				int res_of_storing = STORE_IN_DB(encrypted_word);

				if(res_of_storing==-1) 
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
			else  //Means Word is already there in the DB. (Means Sending Yes to the Client)
			{
				send(client_socket1, buffer, sizeof(buffer), 0);
			}
		}
		
	
		if(option=="2")  // Searching result will be sent to the client.
        send(client_socket1, buffer, sizeof(buffer), 0);
	
    }
    
    // Close sockets
    close(client_socket1);
    
    close(server_fd);

	// closing the listening socket
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}
