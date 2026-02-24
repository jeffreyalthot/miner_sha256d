#pragma once
#include <atomic>
#include <optional>
#include <thread>
#include <vector>

namespace miner::core {

template <typename T>
class BoundedQueue {
public:
  explicit BoundedQueue(std::size_t capacity) : storage_(capacity), capacity_(capacity) {}

  bool push(const T& value) {
    auto head = head_.load(std::memory_order_relaxed);
    auto next = (head + 1) % capacity_;
    if (next == tail_.load(std::memory_order_acquire)) {
      return false;
    }
    storage_[head] = value;
    head_.store(next, std::memory_order_release);
    return true;
  }

  std::optional<T> pop() {
    auto tail = tail_.load(std::memory_order_relaxed);
    if (tail == head_.load(std::memory_order_acquire)) {
      return std::nullopt;
    }
    T value = storage_[tail];
    tail_.store((tail + 1) % capacity_, std::memory_order_release);
    return value;
  }

private:
  std::vector<T> storage_;
  std::size_t capacity_;
  std::atomic<std::size_t> head_{0};
  std::atomic<std::size_t> tail_{0};
};

class BackoffReconnect {
public:
  explicit BackoffReconnect(int baseMs = 500, int maxMs = 30000);
  int nextDelayMs();
  void reset();

private:
  int baseMs_;
  int maxMs_;
  int attempts_{0};
};

} // namespace miner::core
