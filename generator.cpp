#include <iostream>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>
#include<fstream>
#include<cstdlib>
using namespace std;


string sha512(const string str){
    unsigned char hash[SHA512_DIGEST_LENGTH];

    SHA512_CTX sha512;
    SHA512_Init(&sha512);
    SHA512_Update(&sha512, str.c_str(), str.size());
    SHA512_Final(hash, &sha512);
    
    stringstream ss;

    //ss << hex << setw(2) << setfill('0');

    for(int i = 0; i < SHA512_DIGEST_LENGTH; i++){
    ss << hex << setw(2) << setfill('0') << static_cast<int>( hash[i] );
        //ss<<setw(2)<<(int)hash[i];
    }
    return ss.str();
}

// int main() {
//     // ifstream f1;
//     // ofstream f2;
//     // string input_word,word;

//     // f1.open("eng_words.txt");
//     // f2.open("hash.txt");

//     // if(f1.is_open() and f2){
//     //     while(f1){
//     //         getline(f1,input_word);
//     //         if(input_word.length()){
//     //         word = sha512(input_word);
//     //         cout<<input_word<<":"<<word<<endl;
//     //         f2<<word<<"\n";
//     //         }
//     //     }
//     // }else{
//     //     cout<<"Error in populating db.";
//     // }

//     // f1.close();
//     // f2.close();
//     string input_word = "Terminal Root";
//     string word = sha512(input_word);
//     cout<<word<< '\n';
//     cout<<"Length:"<<word.length()<<endl;
//     return 0;
// }
