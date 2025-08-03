#include "crow_all.h"
#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include <string>

// Function to initialize the SQLite database
void initializeDatabase() {
    sqlite3* db;
    char* errMsg = 0;
    int rc = sqlite3_open("database.db", &db);

    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            password TEXT NOT NULL
        );

        CREATE TABLE IF NOT EXISTS comments (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER,
            comment TEXT NOT NULL,
            FOREIGN KEY(user_id) REFERENCES users(id)
        );

        CREATE TABLE IF NOT EXISTS uploads (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER,
            file_path TEXT NOT NULL,
            FOREIGN KEY(user_id) REFERENCES users(id)
        );
    )";

    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    sqlite3_close(db);
}

int main() {
    crow::SimpleApp app;

    // Initialize the database
    initializeDatabase();

    // Route for user registration
    CROW_ROUTE(app, "/register").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body["username"].s() || !body["password"].s()) {
            return crow::response(400, "Invalid input");
        }

        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "INSERT INTO users (username, password) VALUES (?, ?);";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        sqlite3_bind_text(stmt, 1, body["username"].s().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, body["password"].s().c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return crow::response(500, "Error registering user");
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(200, "User registered successfully");
    });

        // Route for user login
    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body["username"].s() || !body["password"].s()) {
            return crow::response(400, "Invalid input");
        }

        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "SELECT id FROM users WHERE username = ? AND password = ?;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        sqlite3_bind_text(stmt, 1, body["username"].s().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, body["password"].s().c_str(), -1, SQLITE_STATIC);

        int rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            int userId = sqlite3_column_int(stmt, 0);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return crow::response(200, "Login successful. User ID: " + std::to_string(userId));
        } else {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return crow::response(401, "Invalid username or password");
        }
    });

    // Route for comment submission
    CROW_ROUTE(app, "/comment").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body["user_id"].i() || !body["comment"].s()) {
            return crow::response(400, "Invalid input");
        }

        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "INSERT INTO comments (user_id, comment) VALUES (?, ?);";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        sqlite3_bind_int(stmt, 1, body["user_id"].i());
        sqlite3_bind_text(stmt, 2, body["comment"].s().c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return crow::response(500, "Error submitting comment");
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(200, "Comment submitted successfully");
    });

    // Route for file upload
    CROW_ROUTE(app, "/upload").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body["user_id"].i() || !body["file_path"].s()) {
            return crow::response(400, "Invalid input");
        }

        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "INSERT INTO uploads (user_id, file_path) VALUES (?, ?);";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        sqlite3_bind_int(stmt, 1, body["user_id"].i());
        sqlite3_bind_text(stmt, 2, body["file_path"].s().c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return crow::response(500, "Error uploading file");
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(200, "File uploaded successfully");
    });

    // Route for retrieving user activity logs
    CROW_ROUTE(app, "/activity_logs").methods(crow::HTTPMethod::GET)([](const crow::request& req) {
        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "SELECT * FROM activity_logs;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

        crow::json::wvalue logs = crow::json::wvalue::list();
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            crow::json::wvalue log;
            log["id"] = sqlite3_column_int(stmt, 0);
            log["user_id"] = sqlite3_column_int(stmt, 1);
            log["activity"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            log["timestamp"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            logs.push_back(log);
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(logs);
    });

    // Route for marking notifications as read
    CROW_ROUTE(app, "/mark_notification").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body["notification_id"].i()) {
            return crow::response(400, "Invalid input");
        }

        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "UPDATE notifications SET is_read = 1 WHERE id = ?;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        sqlite3_bind_int(stmt, 1, body["notification_id"].i());

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return crow::response(500, "Error marking notification as read");
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(200, "Notification marked as read");
    });

    // Start the server
    app.port(8080).multithreaded().run();
}