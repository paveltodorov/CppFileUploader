#include "Token.h"
#include <chrono>

bool Token::isExpired() const {
    return std::chrono::system_clock::now() > expiry;
}

bool Token::isValid() const {
    return !access_token.empty() && !isExpired();
}
