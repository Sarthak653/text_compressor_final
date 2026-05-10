#include "authenticationManager.hpp"
#include "fileManager.hpp"
#include "hash.hpp"

#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

// --- Private helpers ---

// Generates a 32-character random hex salt using a hardware entropy source.
// A unique salt per user means identical passwords produce different hashes,
// defeating precomputed rainbow-table attacks.
std::string AuthenticationManager::generateSalt() {
    std::random_device rd;
    std::mt19937_64    rng(rd());
    std::uniform_int_distribution<uint64_t> dist;

    std::ostringstream oss;
    oss << std::hex << std::setw(16) << std::setfill('0') << dist(rng)
                    << std::setw(16) << std::setfill('0') << dist(rng);
    return oss.str();
}

// One-way password verification: SHA-256(salt + password).
// Static so it can be called without object state — pure function.
std::string AuthenticationManager::hashPassword(const std::string& salt,
                                                 const std::string& password) {
    return sha256(salt + password);
}

// --- File I/O ---
// Format: one line per user — "username:salt:hash"
// Two colons per line; usernames may not contain ':'.

void AuthenticationManager::loadUsers() {
    FileManager fm;
    if (!fm.fileExists(usersFile)) return; // silent on first run

    std::string      content = fm.readFile(usersFile);
    std::istringstream stream(content);
    std::string      line;

    while (std::getline(stream, line)) {
        if (line.empty()) continue;

        size_t first  = line.find(':');
        size_t second = (first != std::string::npos) ? line.find(':', first + 1) : std::string::npos;
        if (first == std::string::npos || second == std::string::npos) continue;

        std::string username = line.substr(0, first);
        std::string salt     = line.substr(first + 1, second - first - 1);
        std::string hash     = line.substr(second + 1);

        if (!username.empty() && !salt.empty() && !hash.empty())
            users[username] = { salt, hash };
    }
}

void AuthenticationManager::saveUsers() {
    FileManager fm;
    std::string content;
    content.reserve(users.size() * 80); // rough pre-allocation to avoid reallocations

    for (const auto& [username, cred] : users)
        content += username + ':' + cred.salt + ':' + cred.hash + '\n';

    fm.writeFile(usersFile, content);
}

// --- Public interface ---

AuthenticationManager::AuthenticationManager(const std::string& filename)
    : usersFile(filename) {
    loadUsers();
}

bool AuthenticationManager::signup(const std::string& username,
                                   const std::string& password) {
    if (username.empty()) {
        std::cerr << "Error: Username cannot be empty.\n";
        return false;
    }
    if (username.find(':') != std::string::npos) {
        std::cerr << "Error: Username may not contain ':'.\n";
        return false;
    }
    if (password.length() < 4) {
        std::cerr << "Error: Password must be at least 4 characters.\n";
        return false;
    }
    if (users.count(username)) {
        std::cerr << "Error: Username already exists.\n";
        return false;
    }

    const std::string salt = generateSalt();
    users[username] = { salt, hashPassword(salt, password) };
    saveUsers();

    currentUser = username;
    std::cout << "Signup successful! Welcome, " << username << "!\n";
    return true;
}

bool AuthenticationManager::login(const std::string& username,
                                  const std::string& password) {
    auto it = users.find(username);
    if (it == users.end()) {
        std::cerr << "Error: Username not found.\n";
        return false;
    }

    // Re-hash what the user typed with the stored salt, compare to stored hash.
    // The original password is never stored or compared directly.
    const UserCredential& cred = it->second;
    if (hashPassword(cred.salt, password) != cred.hash) {
        std::cerr << "Error: Wrong password.\n";
        return false;
    }

    currentUser = username;
    std::cout << "Login successful! Welcome back, " << username << "!\n";
    return true;
}

bool AuthenticationManager::userExists(const std::string& username) const {
    return users.count(username) > 0;
}

std::string AuthenticationManager::getCurrentUser() const {
    return currentUser.empty() ? "Guest" : currentUser;
}

void AuthenticationManager::logout() {
    currentUser.clear();
}