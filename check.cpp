#include <iostream>
#include "optimized_vector.cpp"


using namespace std;

int main() {
    optimized_vector < int > line;
    


    for (int i = 0; i < 30; ++i) {
        line.push_back(i + 1);

    }

    
    //cout << "lol" << endl;
    optimized_vector < int > line2;
    for (int i = 0; i < 20; ++i) {
        line2.push_back(i - 1);
    }

    line2 = line;
    cout << line2.size() << endl;
    for (int i = 0; i < (int) line2.size(); ++i) {
        cout << line2[i] << " ";
    }
    cout << endl;
    return 0;
}