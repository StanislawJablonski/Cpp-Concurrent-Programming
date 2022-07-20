#include<iostream>
#include<fstream>

using namespace std;

int input() {
    string input;
    ofstream f;

    f.open("data.txt");
    
    cout << "Podaj liczbe: " << endl;
    cin >> input;
    f << input;
    f.close();

    return 0;
}

int output(){
    cout << "Oczekiwanie na odpowiedz" << endl;
    while (1){
        ifstream f2("output.txt");

        if ( f2.is_open() && f2.peek() != EOF ){
            int result;
            f2 >> result ;
            cout << "Wynik: " << result << endl ;
            f2.close();
            remove("output.txt");
            break;
        } 
    }
    return 0;
}

int main(){

    input();
    output();

    return 0;
}
