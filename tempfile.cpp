#include <iostream>
#include "generator.cpp"

using namespace std;



int main() {
    
    string input_word = "Terminal Root";
    string word = sha512(input_word);
    cout<<word<< '\n';
    cout<<"Length:"<<word.length()<<endl;
    return 0;
}
