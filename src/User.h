#pragma once
#include "Token.h"
#include <string>

struct User {
    User(const std::string &username, const std::string &password);

    std::string username;
    std::string password;
    Token token;
    long homeFolderId;
};
