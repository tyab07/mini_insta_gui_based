#pragma once
#include <sqlite3.h>
#include <string>
#include <vector>

struct User {
  int id;
  std::string username;
  std::string password;
  std::string city;
};

struct Post {
  int id;
  int user_id;
  std::string content;
  std::string timestamp;
};

struct Message {
  int id;
  int sender_id;
  int receiver_id;
  std::string content;
  std::string timestamp;
};

class Database {
public:
  static Database &GetInstance();

  bool Initialize(const std::string &db_path);
  void Close();

  // User Operations
  bool CreateUser(const std::string &username, const std::string &password,
                  const std::string &city);
  bool VerifyUser(const std::string &username, const std::string &password,
                  User &out_user);
  bool GetUserById(int id, User &out_user);
  bool GetUserByName(const std::string &username, User &out_user);
  std::vector<User> SearchUsers(const std::string &query);

  // Friends Operations (Replacing the Request Queue and BST)
  bool SendFriendRequest(int sender_id, int receiver_id);
  bool AcceptFriendRequest(int sender_id, int receiver_id);
  bool DeleteFriendRequest(int sender_id, int receiver_id);
  std::vector<User> GetFriendRequests(int user_id);
  std::vector<User> GetFriends(int user_id);

  // Posts Operations (Replacing the Stack)
  bool CreatePost(int user_id, const std::string &content,
                  const std::string &timestamp);
  std::vector<Post> GetUserPosts(int user_id);
  std::vector<Post> GetFeedPosts(int user_id); // Gets posts from friends

  // Messaging Operations (Replacing the Queue_m)
  bool SendMessage(int sender_id, int receiver_id, const std::string &content,
                   const std::string &timestamp);
  std::vector<Message> GetConversation(int user1_id, int user2_id);

private:
  Database() = default;
  ~Database();

  // Delete copy/move constructors for singleton
  Database(const Database &) = delete;
  Database &operator=(const Database &) = delete;

  bool ExecuteQuery(const std::string &query);
  bool CreateTables();

  sqlite3 *db = nullptr;
};
