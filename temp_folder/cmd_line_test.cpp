#include <iostream>
#include <mpi.h>
#include <math.h>
#include <bits/stdc++.h>

using namespace std;


int main(int argc, char **argv)
{

    MPI_Init(&argc, &argv);
    int my_rank;
    int total_process;

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &total_process);

    if(my_rank==1)
    cout<<argc<<" "<<argv[0]<<" "<<argv[1]<<" "<<argv[2]<<endl;

    MPI_Finalize();
}
