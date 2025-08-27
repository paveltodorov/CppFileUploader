#include "Config.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

const int DEFAULT_TOKEN_REFRESH_BUFFER_SECONDS = 30;

Config::Config() : transferApiUrl(""), tokenRefreshBufferSeconds(DEFAULT_TOKEN_REFRESH_BUFFER_SECONDS), loaded(false) {}

Config::Config(const std::string& configPath) : Config() { loadFromFile(configPath); }

bool Config::loadFromFile(const std::string& configPath) {
    if (!std::filesystem::exists(configPath)) {
        std::cout << "Config file not found: " << configPath << ". Using default configuration.\n";
        return false;
    }

    try {
        std::ifstream file(configPath);
        if (!file.is_open()) {
            std::cerr << "Failed to open config file: " << configPath << std::endl;
            return false;
        }

        json j;
        file >> j;

        if (j.contains("transferApiUrl")) {
            transferApiUrl = j["transferApiUrl"].get<std::string>();
        }
        if (j.contains("tokenRefreshBufferSeconds")) {
            tokenRefreshBufferSeconds = j["tokenRefreshBufferSeconds"].get<int>();
        }

        std::cout << "Configuration loaded from: " << configPath << std::endl;

        if (transferApiUrl.empty()) {
            std::cerr << "Error: transferApiUrl is required in configuration" << std::endl;
            return false;
        }

        loaded = true;
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Error loading config file: " << e.what() << std::endl;
        return false;
    }
}
