#include "miner/mining.hpp"
#include "miner/hash.hpp"

namespace miner::mining {
Scheduler::Scheduler(const core::PerfConfig& perf, core::MetricsRegistry& metrics, core::Logger& logger)
    : perf_(perf), metrics_(metrics), logger_(logger), shareQueue_(perf.queueDepth) {}

Scheduler::~Scheduler() { stop(); }

void Scheduler::start(const work::Job& job) {
  stop();
  currentJob_ = job;
  running_.store(true);
  epoch_.fetch_add(1);
  for (std::size_t i = 0; i < perf_.cpuThreads; ++i) {
    workers_.emplace_back(&Scheduler::runWorker, this, i);
  }
}

void Scheduler::stop() {
  running_.store(false);
  for (auto& t : workers_) if (t.joinable()) t.join();
  workers_.clear();
}

void Scheduler::runWorker(std::size_t workerId) {
  auto target = work::nbitsToTarget(currentJob_.header.nbits);
  std::uint32_t nonce = static_cast<std::uint32_t>(workerId);
  while (running_.load()) {
    auto hdr = currentJob_.header;
    hdr.nonce = nonce;
    auto serialized = work::serializeHeader(hdr);
    auto digest = hash::sha256d(serialized);
    metrics_.addHashes(1);
    if (work::meetsTarget(digest, target)) {
      shareQueue_.push(Share{currentJob_.channelId, currentJob_.jobId, nonce, digest});
      logger_.log(core::LogLevel::Info, "Share candidate nonce=" + std::to_string(nonce), "worker");
    }
    nonce += static_cast<std::uint32_t>(perf_.cpuThreads);
  }
}
} // namespace miner::mining
