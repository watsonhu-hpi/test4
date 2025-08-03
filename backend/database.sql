-- SQLite database schema

CREATE TABLE IF NOT EXISTS activity_logs (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER,
    activity TEXT NOT NULL,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(user_id) REFERENCES users(id)
);

CREATE TABLE IF NOT EXISTS notifications (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER,
    message TEXT NOT NULL,
    is_read BOOLEAN DEFAULT 0,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(user_id) REFERENCES users(id)
);

-- Sample data
INSERT INTO activity_logs (user_id, activity) VALUES (1, 'User updated profile.');
INSERT INTO activity_logs (user_id, activity) VALUES (1, 'User uploaded a file.');
INSERT INTO notifications (user_id, message) VALUES (1, 'Your profile has been updated.');
INSERT INTO notifications (user_id, message) VALUES (1, 'Your file upload was successful.');
INSERT INTO users (username, password) VALUES ('testuser', 'password123');
INSERT INTO comments (user_id, comment) VALUES (1, 'This is a test comment.');
INSERT INTO uploads (user_id, file_path) VALUES (1, '/uploads/testfile.txt');
INSERT INTO activity_logs (user_id, activity) VALUES (1, 'User logged in.');
INSERT INTO notifications (user_id, message) VALUES (1, 'Welcome to the platform!');

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