#pragma once
#include "Config.h"
#include "Token.h"
#include "User.h"
#include <string>

class ApiClient {
public:
    ApiClient(const Config &config);

    bool login(User &user) const;
    int getFolderIdByName(const Token &token, const std::string &folderName) const;
    long getHomeFolderId(const User &user) const;
    bool uploadFile(const User &user, const std::string &localPath, long folderId) const;
private:
    const Config &config;
};
