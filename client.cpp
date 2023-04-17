#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <bits/stdc++.h>

using namespace std;

#define PORT 8080

int main(int argc, char const* argv[])
{
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
		cout<<"###########################################################"<<endl;

        printf("==> Enter the word:  ");
        cin >> word;
	    
	while(word.length()==0){
		cout<<"Invalid. Try again.\n";	
		printf("==> Enter the word:  ");
        	cin >> word;
	}
	    
        printf("==> Press 1 to store or 2 to search :  ");
        cin>>option;

        
        word=option+"_"+word;

		printf("==> Enter the number of nodes you want :  ");
		cin >> number_of_nodes;
            

        word+="_"+number_of_nodes;

        // cout<<word<<endl;

        char* data = const_cast<char*>(word.c_str());
        send(client_fd, data, strlen(data), 0);

        printf("$: Sent info to the server\n");

		memset(buffer,0,sizeof(buffer));

        valread = read(client_fd, buffer, sizeof(buffer));
	    // printf("%s\n", buffer);

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
				cout<<"$: No such word exists in the Database"<<endl;
			}
		}

        printf("\n==> Press any key to continue. Press q to quit : ");
        char ch;
        cin >> ch;
        if( ch == 'q'){
            break;
        }



    }

    close(client_fd);
	return 0;
}
