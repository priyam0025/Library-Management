# Library Management System

## Overview

This project is a command-line library management system written in C++. It supports role-based access for students, faculty members, and librarians, along with book borrowing, returning, account tracking, and CSV-based persistence.

The project is structured as a small self-contained application rather than an assignment submission. The focus is on clean object-oriented design, simple file-based storage, and a practical terminal workflow.

## Features

- Role-based access for Student, Faculty, and Librarian users
- Borrow and return flow with availability checks
- Borrowing limits and overdue validation
- Fine handling for student accounts
- Book and user persistence through CSV files
- Transaction logging for borrow/return history

## File Structure

The repository is intentionally split into a few small files:

- [main.cpp](main.cpp) — core application logic and interactive CLI
- [books.cpp](books.cpp) — creates and seeds the initial books CSV file
- [users.cpp](users.cpp) — creates and seeds the initial users CSV file
- [borrowed_books.cpp](borrowed_books.cpp) — initializes the borrow/return log CSV file
- [instructions.md](instructions.md) — original project instructions/reference
- [README.md](README.md) — project overview and architecture documentation

## How to Run

This project uses a few small setup programs to generate the CSV files before starting the main application.

### 1. Generate the CSV files

Run these commands from the project root:

```bash
g++ books.cpp -o books
./books

g++ users.cpp -o users
./users

g++ borrowed_books.cpp -o borrowed_books
./borrowed_books
```

### 2. Build and run the main project

After the CSV files are created, compile and run the main application:

```bash
g++ main.cpp -o library_management
./library_management
```

### 3. Optional cleanup

If you want to remove the generated executables afterwards:

```bash
rm -f books users borrowed_books library_management
```

## Code Architecture

The main application lives in [main.cpp](main.cpp). It combines the domain model, persistence helpers, and the user interface in one executable.

### 1. Domain Layer

These classes represent the core library objects:

- `Book` stores book metadata and availability status
- `Account` tracks borrowed books, fines, and account state
- `User` defines the shared interface for all user roles
- `Student`, `Faculty`, and `Librarian` extend `User` with role-specific rules

### 2. Persistence Layer

CSV files are used as the storage layer:

- `BookManager` loads and saves books from `books.csv`
- `User::loadUsersFromFile()` and `User::saveUsersToFile()` handle user persistence
- `TransactionManager` appends borrow/return activity to `borrowed_books.csv`

### 3. Application Flow

The runtime flow is:

1. Load books from `books.csv`
2. Load users from `users.csv`
3. Ask for username and user ID
4. Resolve the matching user with `findUser()`
5. Launch the role-specific CLI menu

### 4. Role Behavior

- `Student` can borrow and return books, view account details, and pay fines
- `Faculty` can borrow and return books, but does not pay fines
- `Librarian` can manage books and users, but cannot borrow or return books

## Data Flow

The project follows a simple flow between memory and files:

1. CSV files are loaded into vectors at startup
2. The selected user performs actions through the CLI
3. Updates are written back to the CSV files when needed
4. Borrow/return activity is logged as text records

## Notes

- Each ISBN is treated as a unique book entry
- The reservation system is not implemented
- The project currently uses a file-based approach instead of a database

## Future Improvements

- Split the large application file into headers and implementation files
- Add a reservation workflow
- Replace CSV storage with a database backend
- Add stronger input validation and error handling

## Summary

This project demonstrates object-oriented C++ design, role-based access control, and file-backed persistence in a compact library management system.
