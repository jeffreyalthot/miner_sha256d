#include "miner/metrics.hpp"
#include <sstream>

namespace miner::core {
void MetricsRegistry::addHashes(std::uint64_t count) { hashes_.fetch_add(count); }
void MetricsRegistry::markAccepted() { accepted_.fetch_add(1); }
void MetricsRegistry::markRejected() { rejected_.fetch_add(1); }
void MetricsRegistry::markStale() { stale_.fetch_add(1); }
void MetricsRegistry::setLatency(std::uint64_t ms) { latencyMs_.store(ms); }

MetricsSnapshot MetricsRegistry::snapshot() const {
  MetricsSnapshot s;
  s.instantHashrate = static_cast<double>(hashes_.load());
  s.averageHashrate = s.instantHashrate;
  s.accepted = accepted_.load();
  s.rejected = rejected_.load();
  s.stale = stale_.load();
  s.latencyMs = latencyMs_.load();
  return s;
}

std::string MetricsRegistry::toPrometheus() const {
  std::ostringstream os;
  os << "miner_hashes_total " << hashes_.load() << "\n";
  os << "miner_shares_accepted_total " << accepted_.load() << "\n";
  os << "miner_shares_rejected_total " << rejected_.load() << "\n";
  os << "miner_shares_stale_total " << stale_.load() << "\n";
  return os.str();
}
} // namespace miner::core
