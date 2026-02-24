#pragma once

#include <string>
#include <map>

class AuthenticationManager {
private:
    std::string usersFile;
    std::map<std::string, std::string> users;
    std::string currentUser;
    
    void loadUsers();
    void saveUsers();

public:
    AuthenticationManager(const std::string& filename = "users.txt");
    
    bool signup(const std::string& username, const std::string& password);
    bool login(const std::string& username, const std::string& password);
    bool userExists(const std::string& username);
    std::string getCurrentUser();
    void logout();
};
