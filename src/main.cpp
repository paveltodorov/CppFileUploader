#include "ApiClient.h"
#include "User.h"

#include <iostream>
#include <string>
#include <filesystem>

int main(int argc, char* argv[]) {
    const std::string defaultBaseUrl = "https://testserver.moveitcloud.com/api/v1";

    if (argc < 4 || argc > 6) {
        std::cerr << "Error: Wrong number of arguments\n";
        std::cerr << "Usage: " << argv[0] << " <username> <password> <file_path> [base_url]\n";
        return 1;
    }

    std::string username = argv[1];
    std::string password = argv[2];
    std::string filePath = argv[3];
    std::string baseUrl = (argc >= 5) ? argv[4] : defaultBaseUrl;

    if (username.empty() || password.empty()) {
        std::cerr << "Error: Username and password cannot be empty\n";
        return 1;
    }

    if (!std::filesystem::exists(filePath)) {
        std::cerr << "Error: File does not exist: " << filePath << "\n";
        return 1;
    }

    try {
        User user{username, password};
        ApiClient client(baseUrl);

        std::cout << "Logging in as " << username << "...\n";

        if (!client.login(user)) {
            std::cerr << "Error: Login failed\n";
            return 1;
        }

        std::cout << "Login successful!\n";

        std::cout << "Getting home folder ID...\n";
        long folderId = client.getHomeFolderId(user);

        if (folderId == -1) {
            std::cerr << "Error: Could not find home folder for user " << username << "\n";
            return 1;
        }

        std::cout << "Found home folder ID: " << folderId << "\n";

        std::cout << "Uploading file: " << filePath << "\n";
        if (client.uploadFile(user, filePath, folderId)) {
            std::cout << "File uploaded successfully!\n";
        } else {
            std::cerr << "Error: File upload failed\n";
            return 1;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
