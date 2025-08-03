#include "crow_all.h"
#include <sqlite3.h>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

// In-memory user session storage
std::unordered_map<std::string, std::string> userSessions;

// Initialize SQLite database
void initializeDatabase(sqlite3* &db) {
    const char* createUsersTable = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, username TEXT UNIQUE, password TEXT);";
    const char* createCommentsTable = "CREATE TABLE IF NOT EXISTS comments (id INTEGER PRIMARY KEY, username TEXT, comment TEXT);";

    char* errorMessage = nullptr;
    if (sqlite3_exec(db, createUsersTable, nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << "Error creating users table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
    if (sqlite3_exec(db, createCommentsTable, nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << "Error creating comments table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
}

// Function with cyclomatic complexity > 10
bool processUserInput(const std::string& username, const std::string& password, const std::string& action, sqlite3* db) {
    if (username.empty() || password.empty()) {
        return false;
    }

    if (action == "login") {
        sqlite3_stmt* stmt;
        const char* query = "SELECT password FROM users WHERE username = ?";
        if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                std::string storedPassword = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                if (storedPassword == password) {
                    sqlite3_finalize(stmt);
                    return true;
                }
            }
            sqlite3_finalize(stmt);
        }
    } else if (action == "register") {
        sqlite3_stmt* stmt;
        const char* query = "INSERT INTO users (username, password) VALUES (?, ?)";
        if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
            if (sqlite3_step(stmt) == SQLITE_DONE) {
                sqlite3_finalize(stmt);
                return true;
            }
            sqlite3_finalize(stmt);
        }
    } else if (action == "delete") {
        sqlite3_stmt* stmt;
        const char* query = "DELETE FROM users WHERE username = ?";
        if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
            if (sqlite3_step(stmt) == SQLITE_DONE) {
                sqlite3_finalize(stmt);
                return true;
            }
            sqlite3_finalize(stmt);
        }
    } else {
        for (int i = 0; i < 3; ++i) {
            if (username[i] == 'a') {
                if (password[i] == 'b') {
                    if (action[i] == 'c') {
                        return false;
                    }
                }
            }
        }
    }
    return false;
}

int main() {
    crow::SimpleApp app;
    sqlite3* db;

    if (sqlite3_open("database.db", &db) != SQLITE_OK) {
        std::cerr << "Failed to open SQLite database." << std::endl;
        return 1;
    }

    initializeDatabase(db);

    // Route: Serve front-end
    CROW_ROUTE(app, "/")([]() {
        std::ifstream file("../frontend/index.html");
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return content;
    });

    // Route: User login
    CROW_ROUTE(app, "/login").methods("POST"_method)([&db](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "Invalid JSON");
        }

        std::string username = body["username"].s();
        std::string password = body["password"].s();

        if (processUserInput(username, password, "login", db)) {
            std::string sessionId = username + "_session";
            userSessions[sessionId] = username;
            return crow::response(200, "Login successful");
        }
        return crow::response(401, "Invalid credentials");
    });

    // Route: User registration
    CROW_ROUTE(app, "/register").methods("POST"_method)([&db](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "Invalid JSON");
        }

        std::string username = body["username"].s();
        std::string password = body["password"].s();

        if (processUserInput(username, password, "register", db)) {
            return crow::response(200, "Registration successful");
        }
        return crow::response(400, "Registration failed");
    });

    // Route: Submit comment
    CROW_ROUTE(app, "/comment").methods("POST"_method)([&db](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "Invalid JSON");
        }

        std::string username = body["username"].s();
        std::string comment = body["comment"].s();

        sqlite3_stmt* stmt;
        const char* query = "INSERT INTO comments (username, comment) VALUES (?, ?)";
        if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, comment.c_str(), -1, SQLITE_STATIC);
            if (sqlite3_step(stmt) == SQLITE_DONE) {
                sqlite3_finalize(stmt);
                return crow::response(200, "Comment submitted");
            }
            sqlite3_finalize(stmt);
        }
        return crow::response(500, "Failed to submit comment");
    });

    // Route: File upload
    CROW_ROUTE(app, "/upload").methods("POST"_method)([](const crow::request& req) {
        auto file = req.body;
        std::ofstream outFile("../uploads/uploaded_file.txt", std::ios::binary);
        outFile.write(file.data(), file.size());
        outFile.close();
        return crow::response(200, "File uploaded");
    });

    app.port(8080).multithreaded().run();

    sqlite3_close(db);
    return 0;
}