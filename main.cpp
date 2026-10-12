#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;
string getCurrentDateTime() {
    time_t now = time(0);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buffer); 
}
// 📌 Book Class
class Book {
private:
    string title, author, publisher, isbn, status;
    int year;
public:
    Book(string t, string a, string p, int y, string i, string s = "Available") 
        : title(t), author(a), publisher(p), year(y), isbn(i), status(s) {}

    string getISBN() const { return isbn; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getPublisher() const { return publisher; }
    int getYear() const { return year; }
    string getStatus() const { return status; }

    void borrowBook() { status = "Borrowed"; }
    void returnBook() { status = "Available"; }

    void displayInfo() const {
        cout << "ISBN: " << isbn << ", Title: " << title 
             << ", Author: " << author << ", Year: " << year 
             << ", Status: " << status << endl;
    }

    // Convert Book data to CSV format
    string toCSV() const {
        return title + "," + author + "," + publisher + "," + to_string(year) + "," + isbn + "," + status;
    }

    // Create Book object from CSV line
    static Book fromCSV(const string& line) {
        stringstream ss(line);
        string t, a, p, i, s;
        int y;
        string temp;

        getline(ss, t, ',');
        getline(ss, a, ',');
        getline(ss, p, ',');
        getline(ss, temp, ',');
        y = stoi(temp);
        getline(ss, i, ',');
        getline(ss, s, ',');

        return Book(t, a, p, y, i, s);
    }
};

// 📌 BookManager Class for CSV Operations
class BookManager {
private:
    string filename;

public:
    BookManager(string fname) : filename(fname) {}

    void saveBooksToCSV(const vector<Book>& books) {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file!" << endl;
            return;
        }

        for (const Book& book : books) {
            file << book.toCSV() << endl;
        }
        file.close();
        cout << "✅ Books saved to " << filename << endl;
    }

    vector<Book> loadBooksFromCSV() {
        vector<Book> books;
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file!" << endl;
            return books;
        }

        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                books.push_back(Book::fromCSV(line));
            }
        }
        file.close();
        cout << "📚 Books loaded from " << filename << endl;
        return books;
    }
};
// 📌 TransactionManager (Handles Borrow History Logging)
class TransactionManager {
    private:
        string filename;
    public:
        TransactionManager(string fname) : filename(fname) {}
    
        void logTransaction(const string& message) {
            ofstream file(filename, ios::app);
            if (!file.is_open()) {
                cerr << "❌ Error opening transaction file!" << endl;
                return;
            }
            file << message << endl;
            file.close();
        }
   };
// Account Class
class Account {
    private:
        vector<pair<string, time_t> > borrowedBooks;
        int fineAmount;
        TransactionManager transactionManager; 
    public:
        Account() : fineAmount(0),transactionManager("borrowed_books.csv") {}
    
        void borrowBook(string isbn,string name) { borrowedBooks.push_back(make_pair(isbn, time(0))); 
            string log = "Book " + isbn + " was borrowed on " + getCurrentDateTime() + " by " + name;
            transactionManager.logTransaction(log);
        }
    
        void returnBook(string isbn,string name) {
            borrowedBooks.erase(remove_if(borrowedBooks.begin(), borrowedBooks.end(), [&](const pair<string, time_t>& book) {
                return book.first == isbn;
            }), borrowedBooks.end());
        string log = "Book " + isbn + " was returned on " + getCurrentDateTime() + " by " + name;
        transactionManager.logTransaction(log);
        }
    
        vector<pair<string, time_t> > getBorrowedBooks() const { return borrowedBooks; }
    
        int calculateFine(int daysOverdue) { return fineAmount += daysOverdue * 10; }
    
        void makePayment() { fineAmount = 0; }
    
        void displayAccount() const {
            cout << "Fine Amount: " << fineAmount << " INR" << endl;
        }
    
        bool hasUnpaidFine() const { return fineAmount > 0; }
    
        bool hasOverdueBook(int maxDays) const {
            time_t now = time(0);
            for (const auto& book : borrowedBooks) {
                double daysBorrowed = difftime(now, book.second) / (60 * 60 * 24);
                if (daysBorrowed > maxDays) return true;
            }
            return false;
        }
    
        size_t getBorrowedBookCount() const { return borrowedBooks.size(); }
    
        void displayBorrowedBooks() const {
            if (borrowedBooks.empty()) {
                cout << "No borrowed books." << endl;
            } else {
                cout << "Borrowed Books:" << endl;
                for (const auto& book : borrowedBooks) {
                    cout << "  - ISBN: " << book.first << " | Borrowed Time: " << ctime(&book.second);
                }
            }
        }
    };
class User {
    protected:
        string name;
        int userId;
        string userType;  // Student, Faculty, Librarian
        Account account;
    
    public:
        User(string n, int id, string type) : name(n), userId(id), userType(type) {}
    
        virtual void borrowBook(string isbn, vector<Book>& books) = 0;
        virtual void returnBook(string isbn, vector<Book>& books) = 0;
    
        void viewAccount() const { account.displayAccount(); }
    
        virtual void displayUserDetails() = 0;
        virtual void displayBorrowedBooks() = 0;
        virtual  void makePayment()=0;
        virtual void addBook(vector<Book>& books, BookManager& bookManager, const Book& newBook)=0;
        virtual void removeBook(vector<Book>& books, BookManager& bookManager, const string& isbn)=0;
        virtual void addUser(vector<User*>& users, const string& name, int id, const string& type)=0;
        virtual void removeUser(vector<User*>& users, int id)=0;
        // Getter functions
        int getUserId() const { return userId; }
        string getUserName() const { return name; }
        string getUserType() const { return userType; }
        static User* fromCSV(string line);
        static vector<User*> loadUsersFromFile(const string& filename);
        virtual string toCSV() {
            return getUserName() + "," + to_string(getUserId()) + "," + getUserType();
        }
        static void saveUsersToFile(const string& filename, const vector<User*>& users);
    };
// Derived Student Class
class Student : public User {
    private:
        static const int borrowLimit = 3;
    public:
        Student(string n, int id) : User(n, id,"Student") {}
    
        void borrowBook(string isbn, vector<Book>& books) override {
            if (account.hasUnpaidFine()) {
                cout << "Clear your fine first!" << endl;
                return;
            }
            if (account.getBorrowedBookCount() >= borrowLimit) {
                cout << "Borrow limit reached!" << endl;
                return;
            }
            if (account.hasOverdueBook(15)) {
                cout << "You have an overdue book! Return it before borrowing another." << endl;
                return;
            }
        
            // Find the book with the given ISBN
            auto it = find_if(books.begin(), books.end(), [&](Book& b) { return b.getISBN() == isbn; });
        
            if (it == books.end()) {
                cout << "Book not found!" << endl;
                return;
            }
        
            if (it->getStatus() != "Available") {
                cout << "Book is not available!" << endl;
                return;
            }
        
            account.borrowBook(isbn, name);
            it->borrowBook();
        }
        
    
        void returnBook(string isbn, vector<Book>& books) override {
            // Find the book with the given ISBN
            auto it = find_if(books.begin(), books.end(), [&](Book& b) { return b.getISBN() == isbn; });
        
            if (it == books.end()) {
                cout << "Book not found!" << endl;
                return;
            }
            account.returnBook(isbn,name);
            it->returnBook();
        }
        void displayBorrowedBooks() override{
            account.displayBorrowedBooks();
        }
        void makePayment() override{
            account.makePayment();
            cout<<"Your Fine has been Cleared !"<<endl;
        }
        void addBook(vector<Book>& books, BookManager& bookManager, const Book& newBook) override{
            cout<<"Students can not add books !"<<endl;
        }
        void removeBook(vector<Book>& books, BookManager& bookManager, const string& isbn) override{
            cout<<"Students can not remove books !"<<endl;
        }
        void addUser(vector<User*>& users, const string& name, int id, const string& type) override{
            cout<<"Students can not add Users !"<<endl;
        }
        void removeUser(vector<User*>& users, int id)override{
            cout<<"Students can not Remove Users !"<<endl;
        }

        void displayUserDetails() override {
            cout << "\nStudent Name: " << name << ", User ID: " << userId << endl;
            account.displayAccount();
            account.displayBorrowedBooks();
        }
    };
    
    // Derived Faculty Class
    class Faculty : public User {
    private:
        static const int borrowLimit = 5;
    public:
        Faculty(string n, int id) : User(n, id,"Faculty") {}
    
        void borrowBook(string isbn, vector<Book>& books) override {
            if (account.getBorrowedBookCount() >= borrowLimit) {
                cout << "Borrow limit reached!" << endl;
                return;
            }
            if (account.hasOverdueBook(30)) {
                cout << "You have an overdue book! Return it before borrowing another." << endl;
                return;
            }
                  // Find the book with the given ISBN
                  auto it = find_if(books.begin(), books.end(), [&](Book& b) { return b.getISBN() == isbn; });
           
                  if (it == books.end()) {
                    cout << "Book not found!" << endl;
                    return;
                }
            
                if (it->getStatus() != "Available") {
                    cout << "Book is not available!" << endl;
                    return;
                }
            account.borrowBook(isbn,name);
            it->borrowBook();
        }
    
        void returnBook(string isbn, vector<Book>& books) override {
            // Find the book with the given ISBN
            auto it = find_if(books.begin(), books.end(), [&](Book& b) { return b.getISBN() == isbn; });
        
            if (it == books.end()) {
                cout << "Book not found!" << endl;
                return;
            }
            account.returnBook(isbn,name);
            it->returnBook();
        }
        void displayBorrowedBooks() override{
            account.displayBorrowedBooks();
        }
        void makePayment() override{
            cout<<"No fine for Faculty !"<<endl;
        }
        void addBook(vector<Book>& books, BookManager& bookManager, const Book& newBook) override{
            cout<<"Faculty Members can not add books !"<<endl;
        }
        void removeBook(vector<Book>& books, BookManager& bookManager, const string& isbn) override{
            cout<<"Faculty Members can not remove books !"<<endl;
        }
        void addUser(vector<User*>& users, const string& name, int id, const string& type) override{
            cout<<"Faculty Members can not add Users !"<<endl;
        }
        void removeUser(vector<User*>& users, int id)override{
            cout<<"Faculty Members can not Remove Users !"<<endl;
        }
        void displayUserDetails() override {
            cout << "\nFaculty Name: " << name << ", User ID: " << userId << endl;
            account.displayAccount();
            account.displayBorrowedBooks();
        }
    };
// 📌 Librarian Class (Handles Books)
class Librarian : public User {
public:
    Librarian(string n, int id) : User(n, id, "Librarian") {}

    // Override borrowBook and returnBook (even if not needed)
    void borrowBook(string isbn, vector<Book>& book) override {
        cout << "📌 Librarians do not borrow books!" << endl;
    }

    void returnBook(string isbn,  vector<Book>& book) override {
        cout << "📌 Librarians do not return books!" << endl;
    }
    void addBook(vector<Book>& books, BookManager& bookManager, const Book& newBook) override {
        for (const Book& book : books) {
            if (book.getISBN() == newBook.getISBN()) {
                cout << "⚠️ Book with ISBN " << newBook.getISBN() << " already exists!" << endl;
                return;
            }
        }
        books.push_back(newBook);
        bookManager.saveBooksToCSV(books);
        cout << "✅ Book added successfully!\n";
    }

    void removeBook(vector<Book>& books, BookManager& bookManager, const string& isbn) override {
        auto it = remove_if(books.begin(), books.end(), [&](const Book& book) {
            return book.getISBN() == isbn;
        });

        if (it != books.end()) {
            books.erase(it, books.end());
            bookManager.saveBooksToCSV(books);
            cout << "✅ Book with ISBN " << isbn << " removed successfully!\n";
        } else {
            cout << "⚠️ Book with ISBN " << isbn << " not found!\n";
        }
    }
    void displayBorrowedBooks() override{
        cout<<"📌 Librarians do not borrow books!"<<endl;
    }
    void makePayment() override{
        cout<<"Librarians Don't Pay fine "<<endl;
    }
    // Declare friend functions
    void addUser(vector<User*>& users, const string& name, int id, const string& type) override;
    void removeUser(vector<User*>& users, int id) override;
    void displayUserDetails() override {
        cout << "👨‍🏫 Librarian: " << name << ", User ID: " << userId << endl;
    }
};
User* User::fromCSV(string line) {
    stringstream ss(line);
    string name, type, idStr;
    int id;

    getline(ss, name, ',');
    getline(ss, idStr, ',');
    getline(ss, type, ',');

    id = stoi(idStr);

    if (type == "Student") return new Student(name, id);
    if (type == "Faculty") return new Faculty(name, id);
    if (type == "Librarian") return new Librarian(name, id);

    return nullptr;
}
// Implement User::loadUsersFromFile()
vector<User*> User::loadUsersFromFile(const string& filename) {
    vector<User*> users;
    ifstream file(filename);
    string line;

    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        return users;
    }

    while (getline(file, line)) {
        User* user = User::fromCSV(line);
        if (user) users.push_back(user);
    }

    file.close();
    return users;
}
// Implement User::saveUsersToFile()
void User::saveUsersToFile(const string& filename, const vector<User*>& users) {
    ofstream file(filename);
    
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    for (User* user : users) {
        file << user->toCSV() << endl;
    }

    file.close();
}
void Librarian::addUser(vector<User*>& users, const string& name, int id, const string& type) {
    for (User* user : users) {
        if (user->toCSV().find(to_string(id) + ",") != string::npos) {
            cout << "User with ID " << id << " already exists.\n";
            return;
        }
    }

    User* newUser = nullptr;
    if (type == "Student") newUser = new Student(name, id);
    else if (type == "Faculty") newUser = new Faculty(name, id);
    else if (type == "Librarian") newUser = new Librarian(name, id);
    else {
        cout << "Invalid user type!\n";
        return;
    }

    users.push_back(newUser);
    User::saveUsersToFile("users.csv", users);
    cout << "User added successfully!\n";
}
void Librarian::removeUser(vector<User*>& users, int id) {
    for (auto it = users.begin(); it != users.end(); ++it) {
        if ((*it)->toCSV().find(to_string(id) + ",") != string::npos) {
            users.erase(it);
            User::saveUsersToFile("users.csv", users);
            cout << "User removed successfully!\n";
            return;
        }
    }

    cout << "User with ID " << id << " not found.\n";
}
User* findUser(vector<User*>& users, const string& name, const string& id) {
    for (auto& user : users) {
        if (user->getUserName() == name && to_string(user->getUserId()) == id) {
            return user;
        }
    }
    return nullptr;
}
void displaystudent(){
    cout<<"Enter a Number for following operation "<<endl;
    cout << "1 for Borrow a book\n";
    cout << "2 for Return a book\n";
    cout << "3 for View borrowed books\n";
    cout << "4 for View account\n";
    cout << "5 for Make Fine Payment \n";
    cout << "6 for Exit \n";
}
void displayFaculty(){
    cout<<"Enter a Number for following operation "<<endl;
    cout << "1 for Borrow a book\n";
    cout << "2 for Return a book\n";
    cout << "3 for View borrowed books\n";
    cout << "4 for View account\n";
    cout << "5 for Exit \n";
}
void displayLibrarian(){
    cout<<"Enter a Number for following operation "<<endl;
    cout << "1 for To Add a book\n";
    cout << "2 for To Remove a book\n";
    cout << "3 for To Add a user\n";
    cout << "4 for To Remove a user\n";
    cout << "5 for Exit \n";
}
void CLI_student(User*user,vector<Book>& books){
   while(true){ 
    int t;
    displaystudent();
    cin>>t;
    if(t==1)
       { cout<<"Enter ISBN of Book to borrow "<<endl;
        string isbn;
        cin>>isbn;
        user->borrowBook(isbn,books);
    }
    else if(t==2)
    { cout<<"Enter ISBN of Book to return "<<endl;
     string isbn;
     cin>>isbn;
     user->returnBook(isbn,books);
    }
    else if(t==3)
    {
     user->displayBorrowedBooks();
    }
    else if(t==4)
    {
     user->displayUserDetails();
    }
    else if(t==5)
    {
     user->makePayment();
    }
    else
        break;}
}
void CLI_faculty(User*user,vector<Book>& books){
    while(true){ 
     int t;
     displayFaculty();
     cin>>t;
     if(t==1)
        { cout<<"Enter ISBN of Book to borrow "<<endl;
         string isbn;
         cin>>isbn;
         user->borrowBook(isbn,books);
     }
     else if(t==2)
     { cout<<"Enter ISBN of Book to return "<<endl;
      string isbn;
      cin>>isbn;
      user->returnBook(isbn,books);
     }
     else if(t==3)
     {
      user->displayBorrowedBooks();
     }
     else if(t==4)
     {
      user->displayUserDetails();
     }
     else
         break;}
 }
void CLI_Libraian(User*user,vector<User*> users,vector<Book>& books,BookManager& bookManager){
while(true){
    displayLibrarian();
    int t;
    cin>>t;
    if(t==1){
        cout<<"To add a book give corresponding informations of book"<<endl;
        string title, author, publisher, isbn;
    int year;

    // Taking user input
    cout << "Enter book title: ";
    cin>>title;
    cout << "Enter author: ";
    cin>>author;
    cout << "Enter publisher: ";
    cin>>publisher;
    cout << "Enter publication year: ";
    cin >> year; // To clear the newline character from the buffer
    cout << "Enter ISBN: ";
    cin>>isbn;

    // Creating Book object
    Book newBook(title, author, publisher, year, isbn);
    user->addBook(books,bookManager,newBook);
    }
    else if(t==2){
        string isbn;
        cout << "Enter ISBN of book which you want to delete: ";
        cin>>isbn;
        user->removeBook(books, bookManager,isbn);
    }
    else if(t==3){
        string name,type;
        int id;
        cout << "Enter Name of User: ";
        cin>>name;
        cout << "Enter ID of User: ";
        cin>>id;
        cout << "Enter Type of User(i.e. Student,Faculty,Librarian): ";
        cin>>type;
        user->addUser(users,name ,id ,type);
    }
    else if(t==4){
        int id;
        cout << "Enter ID of User you want to remove: ";
        cin>>id;
        user->removeUser(users,id);
    }
    else break;

}
}
 int main() {
    BookManager bookManager("books.csv");
    vector<Book> books = bookManager.loadBooksFromCSV();
    vector<User*> users = User::loadUsersFromFile("users.csv");
    // cout << "\n📖 Initial Book List:\n";
    // for (const Book& book : books) {
    //     book.displayInfo();
    // }
    // cout << "Loaded Users:\n";
    // for (User* user : users) {
    //     cout << user->toCSV() << endl;
    // }
    string name, id;
    cout << "Enter your username: ";
    cin >> name;
    cout << "Enter your ID: ";
    cin >> id;
    User* user = findUser(users, name, id);
    if (user) {
            string type=user->getUserType();
            cout<<"Welcome "<<name<<endl;
            if(type=="Student"){
                CLI_student(user,books);
            }
            else if(type=="Faculty"){
                CLI_faculty(user,books);
            }
            else{
                CLI_Libraian(user,users,books,bookManager);
            }
            cout<<"Programmend runned Successfully"<<endl;
        
    } else {
        cout << "Invalid username or ID. Please try again.\n";
    }
    return 0;
}
