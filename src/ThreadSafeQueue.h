#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

namespace libhid {

template <typename T>
class ThreadSafeQueue {
protected:
    std::queue<T> m_queue;
    mutable std::mutex m_mutex;
    std::condition_variable m_cv;

public:
    bool empty() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.size();
    }

    void clear() {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.swap(std::queue<T>());
    }

    void push(const T & item) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(item);
        m_cv.notify_one();
    }

    void push(T && item) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(std::move(item));
        m_cv.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> ulock(m_mutex);
        m_cv.wait(ulock, [this](){ return !m_queue.empty(); });
        T value = m_queue.front();
        m_queue.pop();
        return value;
    }

    bool tryPop(T & value, double timeout_in_seconds = 0) {
        std::unique_lock<std::mutex> ulock(m_mutex);
        auto timeout = std::chrono::milliseconds(int64_t(timeout_in_seconds * 1000));
        if(m_cv.wait_for(ulock, timeout, [this](){ return !m_queue.empty(); })) {
            value = m_queue.front();
            m_queue.pop();
            return true;
        } else {
            return false;
        }
    }

};

}

#endif
