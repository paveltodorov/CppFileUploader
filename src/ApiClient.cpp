#include "ApiClient.h"
#include "Token.h"
#include "User.h"

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include <chrono>
#include <filesystem>
#include <iostream>

using json = nlohmann::json;

ApiClient::ApiClient(const Config& config) : config(config) {}

bool ApiClient::login(User& user) const {
    cpr::Response r =
        cpr::Post(cpr::Url{config.getTransferApiUrl() + "/token"},
                  cpr::Payload{{"grant_type", "password"}, {"username", user.username}, {"password", user.password}});

    if (r.status_code != 200) {
        std::cerr << "[ERROR] Login failed for " << user.username << " (" << r.status_code << "): " << r.text
                  << std::endl;
        return false;
    }

    try {
        json j = json::parse(r.text);
        Token t;
        t.access_token = j["access_token"].get<std::string>();
        int expires_in = j["expires_in"].get<int>();
        t.expiry =
            std::chrono::system_clock::now() + std::chrono::seconds(expires_in - config.getTokenRefreshBufferSeconds());

        user.token = t;

        std::cout << "[INFO] Login successful for " << user.username << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Failed to parse login response: " << e.what() << std::endl;
        return false;
    }
}

int ApiClient::getFolderIdByName(const Token& token, const std::string& folderName) const {
    cpr::Response r =
        cpr::Get(cpr::Url{config.getTransferApiUrl() + "/folders"},
                 cpr::Header{{"Authorization", "Bearer " + token.access_token}, {"Accept", "application/json"}});

    if (r.status_code != 200) {
        std::cerr << "[ERROR] Failed to fetch folders: " << r.status_code << "\n";
        return -1;
    }

    try {
        json j = json::parse(r.text);
        for (json& folder : j["items"]) {
            if (folder["name"].get<std::string>() == folderName) {
                return folder["id"].get<int>();
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] JSON parse error: " << e.what() << "\n";
    }

    return -1;
}

long ApiClient::getHomeFolderId(const User& user) const { return getFolderIdByName(user.token, user.username); }

bool ApiClient::uploadFile(const User& user, const std::string& localPath, long folderId) const {
    if (!std::filesystem::exists(localPath)) {
        std::cerr << "[ERROR] File does not exist: " << localPath << std::endl;
        return false;
    }

    cpr::Response r =
        cpr::Post(cpr::Url{config.getTransferApiUrl() + "/folders/" + std::to_string(folderId) + "/files"},
                  cpr::Header{{"Authorization", "Bearer " + user.token.access_token}, {"Accept", "application/json"}},
                  cpr::Multipart{{"file", cpr::File{localPath}}});

    if (r.status_code == 201 || r.status_code == 200) {
        std::cout << "[INFO] Uploaded " << localPath << " for " << user.username << std::endl;
        return true;
    } else {
        std::cerr << "[ERROR] Upload failed for " << user.username << " (" << r.status_code << "): " << r.text
                  << std::endl;
        return false;
    }
}
