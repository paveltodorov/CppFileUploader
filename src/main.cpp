#include "ApiClient.h"
#include "Config.h"
#include "Queue.h"
#include "UserManager.h"

#include <atomic>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

std::atomic<bool> shutdown_flag{false};

void worker(RequestQueue& queue, const ApiClient& api, UserManager& userManager) {
    while (!shutdown_flag) {
        try {
            std::optional<Request> req = queue.pop();
            if (!req) {
                std::cerr << "Failed to obtain request from queue" << std::endl;
                continue;
            }

            std::shared_ptr<User> user = userManager.getOrCreateUser(req->username, req->password);
            if (!user) {
                std::cerr << "Failed to create user: " << req->username << std::endl;
                continue;
            }

            if (!userManager.ensureLoggedIn(user)) {
                std::cerr << "Failed to login user: " << req->username << std::endl;
                continue;
            }

            if (!api.uploadFile(*user, req->localFile, user->homeFolderId)) {
                std::cerr << "Failed to upload file: " << req->localFile << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Worker error: " << e.what() << std::endl;
        }
    }
}

void printHelp() {
    std::cout << "File Uploader CLI - Commands:\n"
              << "  upload <username> <password> <file_path>  - Upload a single file\n"
              << "  status - Show upload queue status\n"
              << "  help - Show this help message\n"
              << "  quit/exit - Exit the application\n\n"
              << "Examples:\n"
              << "  upload john mypass /path/to/file.txt\n"
              << "  status\n";
}

void printStatus(const RequestQueue& queue) {
    std::cout << "Queue status: " << queue.size() << " files pending upload\n";
    std::cout << "Worker thread: Active\n";
}

int main() {
    Config config("config.json");

    if (!config.isLoaded()) {
        std::cerr << "Error: Failed to load configuration. Please check config.json file." << std::endl;
        return 1;
    }

    RequestQueue queue;
    ApiClient api(config);
    UserManager userManager(api);

    std::thread workerThread(worker, std::ref(queue), std::ref(api), std::ref(userManager));

    std::cout << "File Uploader CLI\n";
    std::cout << "Type 'help' for commands, 'quit' to exit\n\n";

    std::string command;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);

        if (command.empty()) continue;

        std::istringstream iss(command);
        std::string cmd;
        iss >> cmd;

        if (cmd == "quit" || cmd == "exit") {
            break;
        } else if (cmd == "help") {
            printHelp();
        } else if (cmd == "status") {
            printStatus(queue);
        } else if (cmd == "upload") {
            std::string username, password, filepath;
            if (iss >> username >> password >> filepath) {
                if (username.empty() || password.empty() || filepath.empty()) {
                    std::cerr << "Error: Username, password, and file path cannot be empty\n";
                    continue;
                }

                if (!std::filesystem::exists(filepath)) {
                    std::cerr << "Error: File does not exist: " << filepath << std::endl;
                    continue;
                }

                Request req{username, password, filepath};
                queue.push(req);
                std::cout << "Queued file for upload: " << filepath << std::endl;
            } else {
                std::cerr << "Error: upload command requires username, password, and file path\n";
                std::cerr << "Usage: upload <username> <password> <file_path>\n";
            }
        } else {
            std::cerr << "Unknown command: " << cmd << ". Type 'help' for available commands.\n";
        }
    }

    queue.shutdown();
    shutdown_flag = true;
    workerThread.join();
    std::cout << "Exiting...\n";
    return 0;
}
