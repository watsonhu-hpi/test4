#include "../include/auth.hpp"
#include "../include/chat_manager.hpp"
#include "../include/message_history.hpp"
#include "../include/comments.hpp"
#include "../include/file_upload.hpp"
#include "../include/database.hpp"
#include "../include/admin.hpp"
#include "../include/search.hpp"
#include "../include/profile.hpp"
#include "../include/export.hpp"
#include "../include/activity.hpp"
#include "../include/file_metadata.hpp"
#include "../include/comment_filter.hpp"
#include "../include/analytics.hpp"
#include "../include/roles.hpp"
#include "../include/activity_report.hpp"
#include "../include/file_versioning.hpp"
#include "../include/notifications.hpp"
#include "../include/system_logs.hpp"
#include "../include/real_time_notifications.hpp"
#include "../include/advanced_analytics.hpp"
#include "../include/user_dashboard.hpp"
#include "../include/system_health.hpp"
#include "../include/activity_graphs.hpp"
#include "../include/file_analytics.hpp"
#include "../include/activity_heatmap.hpp"
#include "../include/file_usage_trends.hpp"
#include "../include/detailed_activity_report.hpp"
#include "../include/system_performance_dashboard.hpp"
#include <iostream>
#include <string>

void processUserInput(const std::string& input, Auth& auth, Notifications& notifications) {
    if (input == "/register") {
        std::string username, password;
        std::cout << "Enter username: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;
        if (auth.registerUser(username, password)) {
            std::cout << "Registration successful!" << std::endl;
        } else {
            std::cout << "Registration failed!" << std::endl;
        }
    } else if (input == "/login") {
        std::string username, password;
        std::cout << "Enter username: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;
        if (auth.loginUser(username, password)) {
            std::cout << "Login successful!" << std::endl;
        } else {
            std::cout << "Login failed!" << std::endl;
        }
    } else if (input == "/send") {
        std::string recipient, message;
        std::cout << "Enter recipient: ";
        std::cin >> recipient;
        std::cout << "Enter message: ";
        std::cin.ignore();
        std::getline(std::cin, message);
        notifications.sendNotification(recipient, message);
        std::cout << "Message sent!" << std::endl;
    } else if (input == "/history") {
        std::string user1, user2;
        std::cout << "Enter your username: ";
        std::cin >> user1;
        std::cout << "Enter the other user's username: ";
        std::cin >> user2;
        auto history = messageHistory.retrieveHistory(user1, user2);
        std::cout << "Chat history:" << std::endl;
        for (const auto& message : history) {
            std::cout << message << std::endl;
        }
    } else if (input == "/logout") {
        auth.logoutUser();
        std::cout << "Logged out successfully!" << std::endl;
        std::string username;
        std::cout << "Enter username: ";
        std::cin >> username;
        detailedReport.generateDetailedReport(username);
    } else if (input == "view_performance_dashboard") {
        performanceDashboard.displayPerformanceMetrics();
    } else {
        std::cout << "Invalid command. Available commands: /register, /login, /send, /logout" << std::endl;
    }
}

int main() {
Auth auth;
     Notifications notifications;
     Database db;
     ChatManager chatManager(db, notifications);
     MessageHistory messageHistory(db);
    db.initialize();

    std::string input;
    while (true) {
        std::cout << "Enter command: ";
        std::cin >> input;
        if (input == "exit") {
            break;
        }
        processUserInput(input, auth, comments, fileUpload, admin, search, profile, exporter, activity, metadataManager, commentFilter, analytics, roles, report, versioning, notifications, logs, realTime, advancedAnalytics, dashboard, health, graphs, fileAnalytics, heatmap, trends, detailedReport, performanceDashboard);
    }

    return 0;
}