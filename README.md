# Full-Stack Web Application

This is a simple full-stack web application written in C++ with Crow for the backend and SQLite for the database. The frontend is built with HTML, CSS, and JavaScript.

## Features
- User registration and login
- Comment submission
- File uploads

## File Structure
```
project/
├── backend/
│   ├── main.cpp          # Backend logic (C++ code)
│   ├── database.sql      # SQLite database schema
├── frontend/
│   ├── index.html        # Frontend HTML
│   ├── style.css         # Frontend CSS
│   ├── script.js         # Frontend JavaScript
├── uploads/              # Directory for uploaded files
└── README.md             # Instructions for running the project
```

## Prerequisites
- Install Crow (a lightweight C++ web framework).
- Install SQLite3 development libraries.

## Setup
1. Clone the repository.
2. Navigate to the `backend/` directory.
3. Compile the backend:
   ```bash
   g++ main.cpp -o app -lcrow -lsqlite3
   ```
4. Run the application:
   ```bash
   ./app
   ```
5. Open `http://localhost:8080` in your browser to interact with the app.

## Notes
- Uploaded files are stored in the `uploads/` directory.
- The SQLite database file is named `database.db` and is created in the `backend/` directory.
