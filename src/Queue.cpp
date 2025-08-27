#include "Queue.h"

void RequestQueue::push(const Request& req) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        q.push(req);
    }
    cv.notify_one();
}

std::optional<Request> RequestQueue::pop() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this]{ return !q.empty() || shutdown_flag; });

    if (q.empty() && shutdown_flag) {
        return std::nullopt;
    }

    Request req = q.front();
    q.pop();
    return req;
}

size_t RequestQueue::size() const {
    std::lock_guard<std::mutex> lock(mtx);
    return q.size();
}

void RequestQueue::shutdown() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        shutdown_flag = true;
    }
    cv.notify_all();
}
