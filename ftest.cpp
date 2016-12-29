#include "optimized_vector.cpp"


using namespace std;


int main() {
    optimized_vector < int > line;


    for (int i = 0; i < 20; ++i) {
        line.push_back(i + 1);
        for (int j = 0; j < (int) line.size(); ++j) {
            cout << line[j] << " ";
        }
        cout << endl;
    }

    line.reverse();
    for (int i = 0; i < (int) line.size(); ++i) {
        cout << line[i] << " ";

    }
    cout << endl;

    line.resize(30);
    for (int i = 0; i < (int) line.size(); ++i) {
        cout << line[i] << " ";
    }
    cout << endl;

    line.resize(15);

    for (int i = 0; i < (int) line.size(); ++i) {
        cout << line[i] << " ";
    }
    cout << endl;

    line.resize(20);

    for (int i = 0; i < (int) line.size(); ++i) {
        cout << line[i] << " ";
    }
    cout << endl;
    
    for (int i = 0; i < 20; ++i) {
        line.pop_back();
        for (int j = 0; j < (int) line.size(); ++j) {
            cout << line[j] << " ";
        }
        cout << endl;
    }
    return 0;
}