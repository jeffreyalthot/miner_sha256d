#pragma once
#include "miner/config.hpp"
#include "miner/logging.hpp"
#include "miner/metrics.hpp"
#include "miner/runtime.hpp"
#include "miner/work.hpp"
#include <atomic>
#include <thread>

namespace miner::mining {

struct Share {
  std::uint32_t channelId{0};
  std::uint32_t jobId{0};
  std::uint32_t nonce{0};
  miner::Hash256 hash{};
};

class Scheduler {
public:
  Scheduler(const core::PerfConfig& perf, core::MetricsRegistry& metrics, core::Logger& logger);
  ~Scheduler();

  void start(const work::Job& job);
  void stop();

private:
  void runWorker(std::size_t workerId);

  core::PerfConfig perf_;
  core::MetricsRegistry& metrics_;
  core::Logger& logger_;
  std::atomic<bool> running_{false};
  std::atomic<std::uint64_t> epoch_{0};
  work::Job currentJob_{};
  std::vector<std::thread> workers_;
  core::BoundedQueue<Share> shareQueue_;
};

} // namespace miner::mining
