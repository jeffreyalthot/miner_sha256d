#include "miner/runtime.hpp"
#include <algorithm>

namespace miner::core {
BackoffReconnect::BackoffReconnect(int baseMs, int maxMs) : baseMs_(baseMs), maxMs_(maxMs) {}
int BackoffReconnect::nextDelayMs() {
  int delay = std::min(maxMs_, baseMs_ << std::min(attempts_, 10));
  ++attempts_;
  return delay;
}
void BackoffReconnect::reset() { attempts_ = 0; }
} // namespace miner::core
