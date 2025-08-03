document.getElementById('registerForm').addEventListener('submit', async (e) => {
    e.preventDefault();

    const username = document.getElementById('username').value;
    const password = document.getElementById('password').value;

    const response = await fetch('/register', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ username, password }),
    });

    if (response.ok) {
        alert('Registration successful!');
    } else {
        alert('Registration failed.');
    }
});

// Login form submission
document.getElementById('loginForm').addEventListener('submit', async (e) => {
    e.preventDefault();

    const username = document.getElementById('loginUsername').value;
    const password = document.getElementById('loginPassword').value;

    const response = await fetch('/login', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ username, password }),
    });

    if (response.ok) {
        const data = await response.text();
        alert(data);
    } else {
        alert('Login failed.');
    }
});

// Comment form submission
document.getElementById('commentForm').addEventListener('submit', async (e) => {
    e.preventDefault();

    const userId = document.getElementById('userId').value;
    const commentText = document.getElementById('commentText').value;

    const response = await fetch('/comment', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ user_id: parseInt(userId), comment: commentText }),
    });

    if (response.ok) {
        alert('Comment submitted successfully!');
    } else {
        alert('Failed to submit comment.');
    }
});

// Fetch activity logs
document.getElementById('fetchLogs').addEventListener('click', async () => {
    const response = await fetch('/activity_logs', {
        method: 'GET',
    });

    if (response.ok) {
        const logs = await response.json();
        const logsList = document.getElementById('logsList');
        logsList.innerHTML = '';
        logs.forEach(log => {
            const li = document.createElement('li');
            li.textContent = `${log.timestamp}: ${log.activity}`;
            logsList.appendChild(li);
        });
    } else {
        alert('Failed to fetch activity logs.');
    }
});

// Fetch notifications
document.getElementById('fetchNotifications').addEventListener('click', async () => {
    const response = await fetch('/notifications', {
        method: 'GET',
    });

    if (response.ok) {
        const notifications = await response.json();
        const notificationsList = document.getElementById('notificationsList');
        notificationsList.innerHTML = '';
        notifications.forEach(notification => {
            const li = document.createElement('li');
            li.textContent = `${notification.timestamp}: ${notification.message}`;
            notificationsList.appendChild(li);
        });
    } else {
        alert('Failed to fetch notifications.');
    }
});

// File upload form submission
document.getElementById('uploadForm').addEventListener('submit', async (e) => {
    e.preventDefault();

    const userId = document.getElementById('uploadUserId').value;
    const filePath = document.getElementById('filePath').value;

    const response = await fetch('/upload', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ user_id: parseInt(userId), file_path: filePath }),
    });

    if (response.ok) {
        alert('File uploaded successfully!');
    } else {
        alert('Failed to upload file.');
    }
});
    e.preventDefault();

    const username = document.getElementById('username').value;
    const password = document.getElementById('password').value;

    const response = await fetch('/register', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ username, password }),
    });

    if (response.ok) {
        alert('Registration successful!');
    } else {
        alert('Registration failed.');
    }
});