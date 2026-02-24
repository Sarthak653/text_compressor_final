#include "authenticationManager.hpp"
#include "fileManager.hpp"
#include <iostream>
#include <sstream>

AuthenticationManager::AuthenticationManager(const std::string& filename) 
    : usersFile(filename), currentUser("") {
    loadUsers();
}

void AuthenticationManager::loadUsers() {
    FileManager fm;
    
    // Check if file exists FIRST - don't print error if it doesn't
    if (!fm.fileExists(usersFile)) {
        return;  // Silently exit if file doesn't exist (first run)
    }
    
    std::string content = fm.readFile(usersFile);
    
    if (content.empty()) {
        return;
    }
    
    std::istringstream stream(content);
    std::string line;
    
    while (std::getline(stream, line)) {
        if (line.empty()) continue;
        
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
            std::string username = line.substr(0, colonPos);
            std::string password = line.substr(colonPos + 1);
            users[username] = password;
        }
    }
}
void AuthenticationManager::saveUsers() {
    FileManager fm;
    std::string content = "";
    
    for (auto& pair : users) {
        content += pair.first + ":" + pair.second + "\n";
    }
    
    fm.writeFile(usersFile, content);
}

bool AuthenticationManager::signup(const std::string& username, const std::string& password) {
    if (users.find(username) != users.end()) {
        std::cerr << "Error: Username already exists!" << std::endl;
        return false;
    }
    
    if (password.length() < 4) {
        std::cerr << "Error: Password must be at least 4 characters!" << std::endl;
        return false;
    }
    
    if (username.empty()) {
        std::cerr << "Error: Username cannot be empty!" << std::endl;
        return false;
    }
    
    users[username] = password;
    saveUsers();
    currentUser = username;
    
    std::cout << "Signup successful! Welcome, " << username << "!" << std::endl;
    return true;
}

bool AuthenticationManager::login(const std::string& username, const std::string& password) {
    if (users.find(username) == users.end()) {
        std::cerr << "Error: Username not found!" << std::endl;
        return false;
    }
    
    if (users[username] != password) {
        std::cerr << "Error: Wrong password!" << std::endl;
        return false;
    }
    
    currentUser = username;
    std::cout << "Login successful! Welcome back, " << username << "!" << std::endl;
    return true;
}

bool AuthenticationManager::userExists(const std::string& username) {
    return users.find(username) != users.end();
}

std::string AuthenticationManager::getCurrentUser() {
    return currentUser.empty() ? "Guest" : currentUser;
}

void AuthenticationManager::logout() {
    currentUser = "";
}
