#pragma once
#include <string>
#include <filesystem>

class Config {
public:
    Config();
    Config(const std::string& configPath);

    bool loadFromFile(const std::string& configPath);
    bool isLoaded() const { return loaded; }
    const std::string& getTransferApiUrl() const { return transferApiUrl; }
    int getTokenRefreshBufferSeconds() const { return tokenRefreshBufferSeconds; }

private:
    std::string transferApiUrl;
    int tokenRefreshBufferSeconds;
    bool loaded;
};
