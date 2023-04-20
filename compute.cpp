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



// // Fast way is i know number of bytes in each line  
//     // 
//      int lineLength = 129; 
//     streampos position = (starting_line) * lineLength;

    
//     database_file.seekg(position);

//     std::string line;
//     int currentLine = starting_line;

//     // 








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

            if(my_rank==0)
            {
                database_file.clear();
                database_file.seekg(0,ios::beg);
                cout<<database_file.tellg();
            }
    

    while (getline(database_file, line) && currentLine <= starting_line + rowsperprocess[my_rank] - 1) 
    {
        // if(my_rank==0)
        // cout<<"line is ="<<line<<endl;



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


    }


    MPI_Finalize();





    

    

    







}
