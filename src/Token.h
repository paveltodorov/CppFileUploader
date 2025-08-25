#pragma once
#include <string>
#include <chrono>

struct Token {
    std::string access_token;
    std::chrono::system_clock::time_point expiry;

    bool isExpired() const;
    bool isValid() const;
};
