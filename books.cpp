#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ofstream file("books.csv");

    if (!file.is_open()) {
        cerr << "❌ Error: Unable to create books.csv!" << endl;
        return 1;
    }

    // Writing initial book data
    file << "C++ Programming,Bjarne Stroustrup,Addison-Wesley,2013,123456789,Available\n";
    file << "Clean Code,Robert C. Martin,Prentice Hall,2008,987654321,Available\n";
    file << "The Art of Computer Programming,Donald Knuth,Addison-Wesley,1968,192837465,Available\n";
    file << "Introduction to Algorithms,Thomas H. Cormen,MIT Press,2009,987321654,Available\n";
    file << "Artificial Intelligence: A Modern Approach,Stuart Russell,Pearson,2020,564738291,Available\n";
    file << "Deep Learning,Ian Goodfellow,MIT Press,2016,135792468,Available\n";
    file.close();
    cout << "✅ books.csv file created successfully!" << endl;

    return 0;
}
