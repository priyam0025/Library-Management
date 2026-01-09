#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ofstream file("users.csv");

    if (!file.is_open()) {
        cerr << "❌ Error: Unable to create users.csv!" << endl;
        return 1;
    }

    // Writing initial user data
    file << "Alice,101,Librarian\n";
    file << "Bob,102,Student\n";
    file << "David,103,Student\n";
    file << "Rahul,104,Student\n";
    file << "Krishna,105,Student\n";
    file << "Ram,106,Student\n";
    file << "Charlie,107,Faculty\n";
    file << "Adam,108,Faculty\n";
    file << "Eve,109,Faculty\n";

    file.close();
    cout << "✅ users.csv file created successfully!" << endl;

    return 0;
}
