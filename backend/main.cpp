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

    // Route for updating user profiles
    CROW_ROUTE(app, "/update_profile").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body["user_id"].i() || !body["username"].s() || !body["password"].s()) {
            return crow::response(400, "Invalid input");
        }

        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "UPDATE users SET username = ?, password = ? WHERE id = ?;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        sqlite3_bind_text(stmt, 1, body["username"].s().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, body["password"].s().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, body["user_id"].i());

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return crow::response(500, "Error updating profile");
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(200, "Profile updated successfully");
    });

    // Route for retrieving user profiles
    CROW_ROUTE(app, "/get_profile").methods(crow::HTTPMethod::GET)([](const crow::request& req) {
        auto query_params = crow::query_string(req.url_params);
        if (!query_params.has("user_id")) {
            return crow::response(400, "Missing user_id parameter");
        }

        int user_id = std::stoi(query_params.get("user_id"));

        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "SELECT username, password FROM users WHERE id = ?;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        sqlite3_bind_int(stmt, 1, user_id);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            crow::json::wvalue profile;
            profile["username"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            profile["password"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return crow::response(profile);
        } else {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return crow::response(404, "User not found");
        }
    });

    // Route for searching comments by keyword
    CROW_ROUTE(app, "/search_comments").methods(crow::HTTPMethod::GET)([](const crow::request& req) {
        auto query_params = crow::query_string(req.url_params);
        if (!query_params.has("keyword")) {
            return crow::response(400, "Missing keyword parameter");
        }

        std::string keyword = query_params.get("keyword");

        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "SELECT comment FROM comments WHERE comment LIKE ?;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        std::string like_keyword = "%" + keyword + "%";
        sqlite3_bind_text(stmt, 1, like_keyword.c_str(), -1, SQLITE_STATIC);

        crow::json::wvalue results = crow::json::wvalue::list();
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            results.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(results);
    });

    // Route for deleting a user
    CROW_ROUTE(app, "/delete_user").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body["user_id"].i()) {
            return crow::response(400, "Invalid input");
        }

        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "DELETE FROM users WHERE id = ?;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        sqlite3_bind_int(stmt, 1, body["user_id"].i());

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return crow::response(500, "Error deleting user");
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(200, "User deleted successfully");
    });

    // Route for retrieving all users
    CROW_ROUTE(app, "/get_all_users").methods(crow::HTTPMethod::GET)([](const crow::request& req) {
        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "SELECT id, username FROM users;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

        crow::json::wvalue users = crow::json::wvalue::list();
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            crow::json::wvalue user;
            user["id"] = sqlite3_column_int(stmt, 0);
            user["username"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            users.push_back(user);
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(users);
    });

    // Route for adding a new admin user
    CROW_ROUTE(app, "/add_admin").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
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
            return crow::response(500, "Error adding admin user");
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(200, "Admin user added successfully");
    });

    // Route for retrieving admin users
    CROW_ROUTE(app, "/get_admins").methods(crow::HTTPMethod::GET)([](const crow::request& req) {
        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "SELECT id, username FROM users WHERE username LIKE 'admin%';";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

        crow::json::wvalue admins = crow::json::wvalue::list();
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            crow::json::wvalue admin;
            admin["id"] = sqlite3_column_int(stmt, 0);
            admin["username"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            admins.push_back(admin);
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(admins);
    });

    // Route for resetting a user's password
    CROW_ROUTE(app, "/reset_password").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body["user_id"].i() || !body["new_password"].s()) {
            return crow::response(400, "Invalid input");
        }

        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "UPDATE users SET password = ? WHERE id = ?;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        sqlite3_bind_text(stmt, 1, body["new_password"].s().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, body["user_id"].i());

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return crow::response(500, "Error resetting password");
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(200, "Password reset successfully");
    });

    // Route for retrieving user statistics
    CROW_ROUTE(app, "/user_statistics").methods(crow::HTTPMethod::GET)([](const crow::request& req) {
        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "SELECT COUNT(*) AS user_count FROM users;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

        crow::json::wvalue stats;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            stats["user_count"] = sqlite3_column_int(stmt, 0);
        }

        sqlite3_finalize(stmt);

        sql = "SELECT COUNT(*) AS comment_count FROM comments;";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            stats["comment_count"] = sqlite3_column_int(stmt, 0);
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(stats);
    });

    // Route for banning a user
    CROW_ROUTE(app, "/ban_user").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body["user_id"].i()) {
            return crow::response(400, "Invalid input");
        }

        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "UPDATE users SET username = 'BANNED_' || username WHERE id = ?;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        sqlite3_bind_int(stmt, 1, body["user_id"].i());

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return crow::response(500, "Error banning user");
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(200, "User banned successfully");
    });

    // Route for retrieving banned users
    CROW_ROUTE(app, "/get_banned_users").methods(crow::HTTPMethod::GET)([](const crow::request& req) {
        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "SELECT id, username FROM users WHERE username LIKE 'BANNED_%';";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

        crow::json::wvalue banned_users = crow::json::wvalue::list();
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            crow::json::wvalue user;
            user["id"] = sqlite3_column_int(stmt, 0);
            user["username"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            banned_users.push_back(user);
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(banned_users);
    });

    // Route for user activity tracking
    CROW_ROUTE(app, "/track_activity").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body["user_id"].i() || !body["activity"].s()) {
            return crow::response(400, "Invalid input");
        }

        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "INSERT INTO activity_logs (user_id, activity) VALUES (?, ?);";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        sqlite3_bind_int(stmt, 1, body["user_id"].i());
        sqlite3_bind_text(stmt, 2, body["activity"].s().c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return crow::response(500, "Error tracking activity");
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(200, "Activity tracked successfully");
    });

    // Route for retrieving user activity logs by user ID
    CROW_ROUTE(app, "/get_user_activity").methods(crow::HTTPMethod::GET)([](const crow::request& req) {
        auto query_params = crow::query_string(req.url_params);
        if (!query_params.has("user_id")) {
            return crow::response(400, "Missing user_id parameter");
        }

        int user_id = std::stoi(query_params.get("user_id"));

        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "SELECT activity, timestamp FROM activity_logs WHERE user_id = ?;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        sqlite3_bind_int(stmt, 1, user_id);

        crow::json::wvalue activities = crow::json::wvalue::list();
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            crow::json::wvalue activity;
            activity["activity"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            activity["timestamp"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            activities.push_back(activity);
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(activities);
    });

    // Route for sending notifications to users
    CROW_ROUTE(app, "/send_notification").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body["user_id"].i() || !body["message"].s()) {
            return crow::response(400, "Invalid input");
        }

        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "INSERT INTO notifications (user_id, message) VALUES (?, ?);";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        sqlite3_bind_int(stmt, 1, body["user_id"].i());
        sqlite3_bind_text(stmt, 2, body["message"].s().c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return crow::response(500, "Error sending notification");
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(200, "Notification sent successfully");
    });

    // Route for retrieving unread notifications
    CROW_ROUTE(app, "/get_unread_notifications").methods(crow::HTTPMethod::GET)([](const crow::request& req) {
        auto query_params = crow::query_string(req.url_params);
        if (!query_params.has("user_id")) {
            return crow::response(400, "Missing user_id parameter");
        }

        int user_id = std::stoi(query_params.get("user_id"));

        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "SELECT id, message, timestamp FROM notifications WHERE user_id = ? AND is_read = 0;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        sqlite3_bind_int(stmt, 1, user_id);

        crow::json::wvalue notifications = crow::json::wvalue::list();
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            crow::json::wvalue notification;
            notification["id"] = sqlite3_column_int(stmt, 0);
            notification["message"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            notification["timestamp"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            notifications.push_back(notification);
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(notifications);
    });

    // Route for marking all notifications as read
    CROW_ROUTE(app, "/mark_all_notifications_read").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body["user_id"].i()) {
            return crow::response(400, "Invalid input");
        }

        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "UPDATE notifications SET is_read = 1 WHERE user_id = ?;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        sqlite3_bind_int(stmt, 1, body["user_id"].i());

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return crow::response(500, "Error marking notifications as read");
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(200, "All notifications marked as read");
    });

    // Route for deleting all notifications for a user
    CROW_ROUTE(app, "/delete_all_notifications").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body["user_id"].i()) {
            return crow::response(400, "Invalid input");
        }

        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "DELETE FROM notifications WHERE user_id = ?;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        sqlite3_bind_int(stmt, 1, body["user_id"].i());

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return crow::response(500, "Error deleting notifications");
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(200, "All notifications deleted successfully");
    });

    // Route for retrieving the most active users
    CROW_ROUTE(app, "/most_active_users").methods(crow::HTTPMethod::GET)([](const crow::request& req) {
        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "SELECT user_id, COUNT(*) AS activity_count FROM activity_logs GROUP BY user_id ORDER BY activity_count DESC LIMIT 5;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

        crow::json::wvalue active_users = crow::json::wvalue::list();
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            crow::json::wvalue user;
            user["user_id"] = sqlite3_column_int(stmt, 0);
            user["activity_count"] = sqlite3_column_int(stmt, 1);
            active_users.push_back(user);
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(active_users);
    });

    // Route for retrieving the most commented posts
    CROW_ROUTE(app, "/most_commented_posts").methods(crow::HTTPMethod::GET)([](const crow::request& req) {
        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "SELECT comment, COUNT(*) AS comment_count FROM comments GROUP BY comment ORDER BY comment_count DESC LIMIT 5;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

        crow::json::wvalue popular_comments = crow::json::wvalue::list();
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            crow::json::wvalue comment;
            comment["comment"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            comment["comment_count"] = sqlite3_column_int(stmt, 1);
            popular_comments.push_back(comment);
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(popular_comments);
    });

    // Route for retrieving user login history
    CROW_ROUTE(app, "/user_login_history").methods(crow::HTTPMethod::GET)([](const crow::request& req) {
        auto query_params = crow::query_string(req.url_params);
        if (!query_params.has("user_id")) {
            return crow::response(400, "Missing user_id parameter");
        }

        int user_id = std::stoi(query_params.get("user_id"));

        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "SELECT activity, timestamp FROM activity_logs WHERE user_id = ? AND activity LIKE 'User logged in%';";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        sqlite3_bind_int(stmt, 1, user_id);

        crow::json::wvalue login_history = crow::json::wvalue::list();
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            crow::json::wvalue login;
            login["activity"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            login["timestamp"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            login_history.push_back(login);
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(login_history);
    });

    // Route for retrieving user upload history
    CROW_ROUTE(app, "/user_upload_history").methods(crow::HTTPMethod::GET)([](const crow::request& req) {
        auto query_params = crow::query_string(req.url_params);
        if (!query_params.has("user_id")) {
            return crow::response(400, "Missing user_id parameter");
        }

        int user_id = std::stoi(query_params.get("user_id"));

        sqlite3* db;
        sqlite3_open("database.db", &db);

        std::string sql = "SELECT file_path, timestamp FROM uploads WHERE user_id = ?;";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
        sqlite3_bind_int(stmt, 1, user_id);

        crow::json::wvalue upload_history = crow::json::wvalue::list();
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            crow::json::wvalue upload;
            upload["file_path"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            upload["timestamp"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            upload_history.push_back(upload);
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(upload_history);
    });

    // Start the server
    app.port(8080).multithreaded().run();
}