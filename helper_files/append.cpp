
#include<bits/stdc++.h>
using namespace std;


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

int main()
{
    STORE_IN_DB("heelloo");
    return 0;
}

