#include "UserManager.h"
#include "ApiClient.h"

#include <iostream>

UserManager::UserManager(const ApiClient &api) : api(api) {}

std::shared_ptr<User> UserManager::getOrCreateUser(const std::string &username, const std::string &password) {
    std::lock_guard<std::mutex> lock(mtx);

    std::unordered_map<std::string, std::shared_ptr<User>>::iterator it = users.find(username);
    if (it != users.end()) {
        return it->second;
    }

    auto user = std::make_shared<User>(username, password);
    users[username] = user;
    return user;
}

bool UserManager::ensureLoggedIn(std::shared_ptr<User> user) {
    std::lock_guard<std::mutex> lock(mtx);
    if (!user) {
        return false;
    }

    if (user->token.isValid()) {
        return true;
    }

    bool isLoggedIn = api.login(*user);
    if (!isLoggedIn) {
        return false;
    }

    if (user->homeFolderId < 0) {
        long homeId = api.getHomeFolderId(*user);
        user->homeFolderId = homeId;
    }

    return true;
}

void UserManager::insertUser(const std::string &username, std::shared_ptr<User> user) {
    std::lock_guard<std::mutex> lock(mtx);
    users[username] = user;
}

bool UserManager::hasUser(const std::string &username) const {
    std::lock_guard<std::mutex> lock(mtx);
    return users.find(username) != users.end();
}
