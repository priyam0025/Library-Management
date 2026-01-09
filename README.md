# Library Management System  

## Overview  
This Library Management System is implemented with all the required functions as per the assignment. All the implemented functions have been thoroughly tested and work correctly. The command-line interface is simple and intuitive—you can understand how it works when you run the code.  

## Running the Program  
To run the main C++ file, you must first compile and execute the following files:  
- `users.cpp`  
- `books.cpp`  
- `borrow_books.cpp`  

These files are included in the attached folder.  

## File Descriptions  
- **`users.cpp`**: Stores and manages user data.  
- **`books.cpp`**: Stores and manages book data.  
- **`borrow_books.cpp`**: Stores and manages transaction records.  

## Code Structure  
The system is designed using object-oriented principles with separate classes for different functionalities:  
- **`Book`**: Represents book-related information.  
- **`Account`**: Manages account details.  
- **`User`**: A base class for users, with derived classes:  
  - `Student`  
  - `Faculty`  
  - `Librarian`  
- **`BookManager`**: Handles loading and saving book data.  
- **`UserManager`**: Manages user information.  
- **`TransactionManager`**: Stores and processes borrowing transactions.  
- **`findUser()` Function**: Checks whether a user exists in the system.  

## Assumptions  
- Copies of the same book have different ISBNs. Each ISBN is treated as a unique book entry in the system.  

## Limitations  
- The concept of reserved books was not implemented due to complexity and time constraints.  

## Future Improvements  
- Enhance the reservation system for books.  
- Improve code modularity by further separating files.  

Thank you for reviewing my project!
