#include <iostream>
#include <mpi.h>
#include <math.h>
#include <bits/stdc++.h>
#include <fstream>
#include <sys/socket.h>
#include <unistd.h>;
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

    // socket_client_code_here()



    // search_string = ""

    
    MPI_Init(&argc,&argv);
    int my_rank;
    int total_process;

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &total_process);

    string search_string ;
    char charArray[128]; // Allocate memory for char array
    
    if(my_rank==0)
    {
        search_string="9b71d224bd62f3785d96d46ad3ea3d73319bfbc2890caadae2dff72519673ca72323c3d99ba5c11d7c7acc6e14b8c5da0c4663475c2e5c3adef46f73bcdec043";

        strcpy(charArray, search_string.c_str());
    }


    MPI_Bcast(&charArray,128,MPI_CHAR,0,MPI_COMM_WORLD);

    string s(charArray);
    search_string=s;

    if(my_rank==0)
    cout<<search_string;

    MPI_Finalize();





    

    

    







}
