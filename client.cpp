#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include "generator.cpp"

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

int main(int argc, char const* argv[])
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
cout<<"                             Client Starting...\n\n";






	int status, valread, client_fd;
	struct sockaddr_in serv_addr;
	
	char buffer[1024] = { 0 };

    
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
		<= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}

	if ((status
		= connect(client_fd, (struct sockaddr*)&serv_addr,
				sizeof(serv_addr)))
		< 0) {
		printf("\nConnection Failed \n");
		return -1;
	}

    while(true){

        string word="",option="2";
        string number_of_nodes="0",res="";
		string encrypted_word ="";
		string data_to_send="";

		// cout<<"###########################################################"<<endl;

		cout<<cyan<<"==> Enter the word:  "<<reset;
        cin >> word;
		cout<<cyan;

		encrypted_word =  sha512(word);

	    
		while(word.length()==0){
			cout<<red<<"Invalid. Try again.\n";	
			cout<<cyan<<"==> Enter the word:  "<<reset;
			cin >> word;
			cout<<cyan;
			encrypted_word =  sha512(word);
		}
	    
        // printf("==> Press 1 to store or 2 to search :  ");
		cout<<"==> Press 1 to store or 2 to search :  "<<reset;
        cin>>option;
		cout<<cyan;
        
        data_to_send=option+"_"+encrypted_word;


		cout<<"==> Enter the number of nodes you want :  "<<reset;
		cin >> number_of_nodes;
		cout<<cyan;

        data_to_send+="_"+number_of_nodes;


        char* data = const_cast<char*>(data_to_send.c_str());
        send(client_fd, data, strlen(data), 0);

		cout<<green<<"$: Performing Encryption and Sending data to the server\n";

		memset(buffer,0,sizeof(buffer));
        valread = read(client_fd, buffer, sizeof(buffer));
		string result(buffer);

		if(option=="1")
		{
			if(result=="YES")
			{
				cout<<"$: The word is already present in the Database"<<endl;
			}
			else
			{
				cout<<"$: "<<result<<endl;
			}
		}
		else
		{
			if(result=="YES")
			{
				cout<<"$: The Word is found in the Database"<<endl;
			}
			else
			{
				cout<<red<<"$: No such word exists in the Database"<<cyan<<endl;
			}
		}

        // printf("\n==> Press any key to continue. Press q to quit : ");
		cout<<yellow<<"\n==> Press any key to continue. Press q to quit : "<<reset;
        char ch;
        cin >> ch;
		cout<<cyan;
        if( ch == 'q' or ch=='Q'){
            break;
        }



    }

    close(client_fd);
	return 0;
}
