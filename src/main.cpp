#include "../include/auth.hpp"
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

void processUserInput(const std::string& input, Auth& auth, Comments& comments, FileUpload& fileUpload, Admin& admin, Search& search, Profile& profile, Export& exporter, Activity& activity, FileMetadataManager& metadataManager, CommentFilter& commentFilter, Analytics& analytics, Roles& roles, ActivityReport& report, FileVersioning& versioning, Notifications& notifications, SystemLogs& logs, RealTimeNotifications& realTime, AdvancedAnalytics& advancedAnalytics, UserDashboard& dashboard, SystemHealth& health, ActivityGraphs& graphs, FileAnalytics& fileAnalytics, ActivityHeatmap& heatmap, FileUsageTrends& trends, DetailedActivityReport& detailedReport, SystemPerformanceDashboard& performanceDashboard) {
    if (input == "generate_detailed_report") {
        std::string username;
        std::cout << "Enter username: ";
        std::cin >> username;
        detailedReport.generateDetailedReport(username);
    } else if (input == "view_performance_dashboard") {
        performanceDashboard.displayPerformanceMetrics();
    } else {
        std::cout << "Invalid command." << std::endl;
    }
}

int main() {
    Auth auth;
    Comments comments;
    FileUpload fileUpload;
    Database db;
    Admin admin;
    Search search;
    Profile profile;
    Export exporter;
    Activity activity;
    FileMetadataManager metadataManager;
    CommentFilter commentFilter;
    Analytics analytics;
    Roles roles;
    ActivityReport report;
    FileVersioning versioning;
    Notifications notifications;
    SystemLogs logs;
    RealTimeNotifications realTime;
    AdvancedAnalytics advancedAnalytics;
    UserDashboard dashboard;
    SystemHealth health;
    ActivityGraphs graphs;
    FileAnalytics fileAnalytics;
    ActivityHeatmap heatmap;
    FileUsageTrends trends;
    DetailedActivityReport detailedReport;
    SystemPerformanceDashboard performanceDashboard;

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