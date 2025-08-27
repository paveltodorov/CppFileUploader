#pragma once

#include "User.h"
#include "ApiClient.h"

#include <string>
#include <unordered_map>
#include <mutex>
#include <memory>

class UserManager {
public:
    UserManager(const ApiClient& api);
    std::shared_ptr<User> getOrCreateUser(const std::string& username, const std::string& password);
    bool ensureLoggedIn(std::shared_ptr<User> user);
    void insertUser(const std::string& username, std::shared_ptr<User> user);
    bool hasUser(const std::string& username) const;
private:
    std::unordered_map<std::string, std::shared_ptr<User>> users;
    mutable std::mutex mtx;

    const ApiClient& api;
};
