#include <iostream>
#include <mpi.h>
#include <math.h>
#include <bits/stdc++.h>
#include <fstream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>

using namespace std;

int main(int argc,char** argv) {

    #ifndef ONLINE_JUDGE
 
        // For getting input from input.txt file
        freopen("input.txt", "r", stdin);
    
        // Printing the Output to output.txt file
        freopen("output.txt", "w", stdout);
 
    #endif



    string search_string = argv[1];
    string output_file_name = argv[2];


    MPI_Init(&argc,&argv);
    int my_rank;
    int total_process;

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &total_process);

    // char charArray[129];
    // int c_socket = 0;


    // if(my_rank==0)
    // {
    //     string tracker_port ="8080";
    //     string tracker_ip ="127.0.0.1";

    //     if ((c_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    //     {
    //         cout <<"Error while creating client socket\n";
    //     }
        
    //     struct sockaddr_in server_address;
    //     server_address.sin_family = AF_INET;
    //     // server_address.sin_addr.s_addr=INADDR_ANY;
    //     server_address.sin_port = htons(stoi(tracker_port));

    //     if (inet_pton(AF_INET, tracker_ip.c_str(), &server_address.sin_addr) <= 0)
    //     {
    //         printf("\nInvalid address/ Address not supported \n");
    //         exit(EXIT_FAILURE);
    //     }

    //     int connection_status = connect(c_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    //     if (connection_status == -1)
    //     {
    //         cout << "Error in connection from client side.\n";
    //         exit(EXIT_FAILURE);
    //     }
    //     // else
    //     // {
    //     //     cout <<"Congratulations!! Connection Established with the Tracker." << endl;
    //     //     cout<<"Tracker ip is "<<tracker_ip<<" and Port is "<<tracker_port<<".\n";
    //     // }

        
    //     recv(c_socket, charArray, sizeof(charArray), 0);

        
        
    // }


    // MPI_Bcast(&charArray,129,MPI_CHAR,0,MPI_COMM_WORLD);

    // string s(charArray);
    // search_string=s;

    // if(my_rank==1)
    // cout<<search_string;   

    ifstream database_file; 
    database_file.open("hash.txt");

   


    int total_lines_in_db=0;

    if(my_rank==0)
    {
        string line;
        while (getline(database_file, line))
        {
            total_lines_in_db++;
            // cout<<line<<endl;
        }

    
        // cout<<total_lines_in_db;
    }

    MPI_Bcast(&total_lines_in_db,1,MPI_INT,0,MPI_COMM_WORLD);



    // if(my_rank==1)
    // cout<<"hi"<<total_lines_in_db;




    int rowsperprocess[total_process];
    int pastrows[total_process];
    int rem = (total_lines_in_db)%total_process;
    int sum=0;


    for(int i=0;i<total_process;i++)
    {
        rowsperprocess[i]=(total_lines_in_db)/total_process;
        if(rem>0)
        {
            rowsperprocess[i]++;
            rem--;

        }
        pastrows[i]=sum;
        sum+=rowsperprocess[i];
    }

    // if(my_rank==2)
    // {
    //     for(int i=0;i<total_process;i++)
    //     {
    //         cout<<rowsperprocess[i]<<" "<<pastrows[i]<<endl;
    //     }
    // }
    

    int starting_line = pastrows[my_rank];


    int isfound=0;



// Fast way is i know number of bytes in each line  
    // // 
    //  int lineLength = 128; 
    // streampos position = (starting_line - 1) * lineLength;

    
    // database_file.seekg(position);

    // std::string line;
    // int currentLine = starting_line;

    // // 
// 







    // IF fast way remove below lines
    string line;
    int currentLine = 0;

    while (currentLine < starting_line && getline(database_file, line)) 
    {
        currentLine++;
    }


    // IF fast way remove above lines
    if(my_rank==0)
        cout<<"search_ string is ="<<search_string<<endl<<"curr_line = "<<currentLine<<endl;

    while (getline(database_file, line) && currentLine <= starting_line + rowsperprocess[my_rank] - 1) 
    {
        if(my_rank==0)
        cout<<"line is ="<<line<<endl;



        if (line == search_string) {
            isfound=1;
            break;
        }

        currentLine++;
    }

    // if(my_rank==1)
    // cout<<isfound<<endl;


    int is_found_array[total_process];
    memset(is_found_array,0,sizeof(int)*total_process);


    


    MPI_Gather(&isfound,1,MPI_INT,is_found_array,1,MPI_INT,0,MPI_COMM_WORLD);



    if(my_rank==0)
    {
        
        int found_or_not=0;

        for(int i=0;i<total_process;i++)
        {
            if(is_found_array[i]==1)
                {
                    found_or_not=1;
                    break;
                }
        }


        // for(int i=0;i<total_process;i++)
        // {
        //     cout<<is_found_array[i]<<" ";
        // }

        cout<<found_or_not;

        ofstream output_file(output_file_name, ios::out);
        if (!output_file)
        {
            cout << "There was an error opening file for output: compute.cpp" <<endl;
            return -1;	
        }



        // Write YES/NO to the File.
        if(found_or_not==1)
        {
            output_file<<"YES";   
        }
        else
        {
            output_file<<"NO"; 
        }

        output_file.close();




            
        

        // SEND found_or_not to server
        // if(found_or_not==1)
        // {
        //     char buf[1024] = "YES";
        //     send(c_socket, buf, sizeof(buf), 0);
        // }
        // else
        // {
        //     char buf[1024] = "NO";
        //     send(c_socket, buf, sizeof(buf), 0);
        // }

        // close(c_socket);

    }


    MPI_Finalize();





    

    

    







}
