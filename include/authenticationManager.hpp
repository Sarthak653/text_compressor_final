#pragma once
// authenticationManager.hpp
// Passwords are NEVER stored in plaintext.
// File format (users.txt): "username:salt:sha256hash"

#include <map>
#include <string>

struct UserCredential {
    std::string salt;
    std::string hash; // SHA-256(salt + password)
};

class AuthenticationManager {
private:
    std::string                           usersFile;
    std::map<std::string, UserCredential> users;
    std::string                           currentUser;

    void loadUsers();
    void saveUsers();

    static std::string generateSalt();
    static std::string hashPassword(const std::string& salt, const std::string& password);

public:
    explicit AuthenticationManager(const std::string& filename = "users.txt");

    bool        signup(const std::string& username, const std::string& password);
    bool        login(const std::string& username, const std::string& password);
    bool        userExists(const std::string& username) const;
    std::string getCurrentUser() const; // returns "Guest" when no user is logged in
    void        logout();
};