#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

string getCurrentDateTime() {
    time_t now = time(0);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buf);
}

int main() {
    ofstream file("borrowed_books.csv", ios::app);

    if (!file.is_open()) {
        cerr << "❌ Error: Unable to create borrowed_books.csv!" << endl;
        return 1;
    }

    // Writing CSV Header if the file is empty
    if (file.tellp() == 0) {
        file << "Timestamp,Transaction\n";
    }

    file.close();
    cout << "✅ borrowed_books.csv file created successfully!" << endl;

    return 0;
}
