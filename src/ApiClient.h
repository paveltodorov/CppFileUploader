#pragma once
#include <string>
#include "User.h"
#include "Token.h"

class ApiClient {
public:
    ApiClient(const std::string& baseUrl);

    bool login(User& user) const;
    int getFolderIdByName(const Token& token, const std::string& folderName) const;
    long getHomeFolderId(const User& user) const;
    bool uploadFile(const User& user, const std::string& localPath, long folderId) const;

private:
    const std::string& baseUrl;
};
