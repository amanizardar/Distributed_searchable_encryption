#include <iostream>
#include <math.h>
#include <bits/stdc++.h>

using namespace std;


int main(int argc, char **argv)
{
    string cmd1 = "mpic++ cmd_line_test.cpp";
    string cmd2= "mpirun -np 3 ./a.out firsttt second";
   system(cmd1.c_str());
   system(cmd2.c_str());
   return 0;
}
