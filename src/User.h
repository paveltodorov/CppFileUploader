#pragma once
#include "Token.h"
#include <string>

struct User {
    std::string username;
    std::string password;
    Token token;
    long homeFolderId;
};
