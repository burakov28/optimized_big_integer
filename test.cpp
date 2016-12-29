#include "big_integer.h"
#include <iostream>
#include <cstdio>
#include <ctime>


using namespace std;


int main() {
    freopen("inp", "r", stdin);
    freopen("out1", "w", stdout);
    
    //string s;
    //cin >> s;


    
    big_integer a( "184467440737095516161844674407370955161618446744073709551616");
    big_integer b("-184467440737095516161844674407370955161618446744073709551616");
    big_integer c( "368934881474191032323689348814741910323236893488147419103232");

    cout << "lol" << endl;
    cout << a << endl;
    cout << (a + a) << endl;
    cout << (b + c) << endl;
    cout << (c + b) << endl;
    cout << (b + a) << endl;
    

    /*
    string s, d;
    cin >> s >> d;
    //cout << s << endl;
    big_integer a (s), b (d);
    big_integer tmp = a;
    cout << a << endl;
    cout << b << endl;
    a *= b;
    cout << a << endl;
    cout << b << endl;

    big_integer c = tmp * b;
    if (c == a && a == c) {
        cout << "true" << endl;
    }
    else {
        cout << "false" << endl;
    }
    cout << c << endl;
    */
    //cout << (a >> b) << endl;
    //cout << (a ^ b) << endl;
    return 0;
}