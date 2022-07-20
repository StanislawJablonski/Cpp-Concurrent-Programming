#include<iostream>
#include<fstream>

using namespace std;

int inputTimes2(int input){
    return input*2;
}

int InputLoop(){
    int i = 1 ;
    while ( i ){
         ifstream f("data.txt");
         if ( f.is_open() && f.peek() != EOF ){
             int dane;
             f >> dane;
             f.close();
             remove("data.txt");
             dane = inputTimes2(dane);
            ofstream f("output.txt");
            f << dane;
            i = 0;
         }
    }
    return 0;
}

int main(){

    while(true){
        InputLoop();
    }
    return 0;
}