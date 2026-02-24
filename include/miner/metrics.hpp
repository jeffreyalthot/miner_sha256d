#pragma once
#include <atomic>
#include <cstdint>
#include <string>

namespace miner::core {

struct MetricsSnapshot {
  double instantHashrate{0};
  double averageHashrate{0};
  std::uint64_t accepted{0};
  std::uint64_t rejected{0};
  std::uint64_t stale{0};
  std::uint64_t latencyMs{0};
};

class MetricsRegistry {
public:
  void addHashes(std::uint64_t count);
  void markAccepted();
  void markRejected();
  void markStale();
  void setLatency(std::uint64_t ms);
  MetricsSnapshot snapshot() const;
  std::string toPrometheus() const;

private:
  std::atomic<std::uint64_t> hashes_{0};
  std::atomic<std::uint64_t> accepted_{0};
  std::atomic<std::uint64_t> rejected_{0};
  std::atomic<std::uint64_t> stale_{0};
  std::atomic<std::uint64_t> latencyMs_{0};
};

} // namespace miner::core
