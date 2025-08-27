#pragma once
#include "Request.h"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

class RequestQueue {
public:
    void push(const Request& req);
    std::optional<Request> pop();
    size_t size() const;
    void shutdown();
private:
    std::queue<Request> q;
    mutable std::mutex mtx;
    std::condition_variable cv;
    bool shutdown_flag = false;
};
