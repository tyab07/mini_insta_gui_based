#include "Database.h"
#include <iostream>

Database &Database::GetInstance() {
  static Database instance;
  return instance;
}

Database::~Database() { Close(); }

bool Database::Initialize(const std::string &db_path) {
  int rc = sqlite3_open(db_path.c_str(), &db);
  if (rc != SQLITE_OK) {
    std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
    return false;
  }
  return CreateTables();
}

void Database::Close() {
  if (db) {
    sqlite3_close(db);
    db = nullptr;
  }
}

bool Database::ExecuteQuery(const std::string &query) {
  char *error_message = nullptr;
  int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &error_message);
  if (rc != SQLITE_OK) {
    std::cerr << "SQL error: " << error_message << std::endl;
    sqlite3_free(error_message);
    return false;
  }
  return true;
}

bool Database::CreateTables() {
  const std::string user_table = "CREATE TABLE IF NOT EXISTS Users ("
                                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                 "username TEXT UNIQUE NOT NULL,"
                                 "password TEXT NOT NULL,"
                                 "city TEXT NOT NULL"
                                 ");";

  const std::string friend_request_table =
      "CREATE TABLE IF NOT EXISTS FriendRequests ("
      "sender_id INTEGER NOT NULL,"
      "receiver_id INTEGER NOT NULL,"
      "status TEXT DEFAULT 'pending', " // 'pending' or 'accepted'
      "PRIMARY KEY (sender_id, receiver_id),"
      "FOREIGN KEY (sender_id) REFERENCES Users(id),"
      "FOREIGN KEY (receiver_id) REFERENCES Users(id)"
      ");";

  const std::string post_table = "CREATE TABLE IF NOT EXISTS Posts ("
                                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                 "user_id INTEGER NOT NULL,"
                                 "content TEXT NOT NULL,"
                                 "timestamp TEXT NOT NULL,"
                                 "FOREIGN KEY (user_id) REFERENCES Users(id)"
                                 ");";

  const std::string message_table =
      "CREATE TABLE IF NOT EXISTS Messages ("
      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
      "sender_id INTEGER NOT NULL,"
      "receiver_id INTEGER NOT NULL,"
      "content TEXT NOT NULL,"
      "timestamp TEXT NOT NULL,"
      "FOREIGN KEY (sender_id) REFERENCES Users(id),"
      "FOREIGN KEY (receiver_id) REFERENCES Users(id)"
      ");";

  bool success = ExecuteQuery(user_table);
  success &= ExecuteQuery(friend_request_table);
  success &= ExecuteQuery(post_table);
  success &= ExecuteQuery(message_table);
  return success;
}

// User Operations
bool Database::CreateUser(const std::string &username,
                          const std::string &password,
                          const std::string &city) {
  std::string sql = "INSERT INTO Users (username, password, city) VALUES ('" +
                    username + "', '" + password + "', '" + city + "');";
  return ExecuteQuery(sql);
}

bool Database::VerifyUser(const std::string &username,
                          const std::string &password, User &out_user) {
  std::string sql =
      "SELECT id, username, password, city FROM Users WHERE username = '" +
      username + "' AND password = '" + password + "';";
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    return false;
  }

  bool found = false;
  if (sqlite3_step(stmt) == SQLITE_ROW) {
    out_user.id = sqlite3_column_int(stmt, 0);
    out_user.username = std::string(
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
    out_user.password = std::string(
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)));
    out_user.city = std::string(
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3)));
    found = true;
  }
  sqlite3_finalize(stmt);
  return found;
}

bool Database::CreatePost(int user_id, const std::string &content,
                          const std::string &timestamp) {
  std::string sql = "INSERT INTO Posts (user_id, content, timestamp) VALUES (" +
                    std::to_string(user_id) + ", '" + content + "', '" +
                    timestamp + "');";
  return ExecuteQuery(sql);
}

std::vector<Post> Database::GetFeedPosts(int user_id) {
  std::vector<Post> posts;
  // For now, load ALL posts in descending order -> later we filter by friends
  // only.
  std::string sql = "SELECT id, user_id, content, timestamp FROM Posts ORDER "
                    "BY id DESC LIMIT 50;";
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      Post p;
      p.id = sqlite3_column_int(stmt, 0);
      p.user_id = sqlite3_column_int(stmt, 1);
      p.content = std::string(
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)));
      p.timestamp = std::string(
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3)));
      posts.push_back(p);
    }
  }
  sqlite3_finalize(stmt);
  return posts;
}

bool Database::GetUserById(int id, User &out_user) {
  std::string sql =
      "SELECT id, username, password, city FROM Users WHERE id = " +
      std::to_string(id) + ";";
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    return false;

  bool found = false;
  if (sqlite3_step(stmt) == SQLITE_ROW) {
    out_user.id = sqlite3_column_int(stmt, 0);
    out_user.username = std::string(
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
    out_user.password = std::string(
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)));
    out_user.city = std::string(
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3)));
    found = true;
  }
  sqlite3_finalize(stmt);
  return found;
}

std::vector<Post> Database::GetUserPosts(int user_id) {
  std::vector<Post> posts;
  std::string sql =
      "SELECT id, user_id, content, timestamp FROM Posts WHERE user_id = " +
      std::to_string(user_id) + " ORDER BY id DESC;";
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      Post p;
      p.id = sqlite3_column_int(stmt, 0);
      p.user_id = sqlite3_column_int(stmt, 1);
      p.content = std::string(
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)));
      p.timestamp = std::string(
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3)));
      posts.push_back(p);
    }
  }
  sqlite3_finalize(stmt);
  return posts;
}

std::vector<User> Database::GetFriends(int user_id) {
  std::vector<User> friends;
  std::string sql = "SELECT u.id, u.username, u.password, u.city FROM Users u "
                    "INNER JOIN FriendRequests fr ON "
                    "((fr.sender_id = " +
                    std::to_string(user_id) +
                    " AND fr.receiver_id = u.id) "
                    "OR (fr.receiver_id = " +
                    std::to_string(user_id) +
                    " AND fr.sender_id = u.id)) "
                    "WHERE fr.status = 'accepted';";
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      User u;
      u.id = sqlite3_column_int(stmt, 0);
      u.username = std::string(
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
      u.password = std::string(
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)));
      u.city = std::string(
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3)));
      friends.push_back(u);
    }
  }
  sqlite3_finalize(stmt);
  return friends;
}

std::vector<User> Database::SearchUsers(const std::string &query) {
  std::vector<User> users;
  std::string sql =
      "SELECT id, username, password, city FROM Users WHERE username LIKE '%" +
      query + "%' LIMIT 20;";
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      User u;
      u.id = sqlite3_column_int(stmt, 0);
      u.username = std::string(
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
      u.password = std::string(
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)));
      u.city = std::string(
          reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3)));
      users.push_back(u);
    }
  }
  sqlite3_finalize(stmt);
  return users;
}
