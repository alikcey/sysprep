#include <iostream>
#include <limits>

using namespace std;

int main(void){
        int n;
        cout << "Enter new hostname: " << flush;
        string line;
        cin >> line;
        //cin.ignore(numeric_limits <streamsize> ::max(), '\n');
        getline(cin, line);
        cout << "New host name is: " << line << endl;

        return 0;
        }
