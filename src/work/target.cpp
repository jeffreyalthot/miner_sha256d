#include "miner/work.hpp"

namespace miner::work {
std::array<std::uint8_t, 32> nbitsToTarget(std::uint32_t nbits) {
  std::array<std::uint8_t, 32> target{};
  std::uint32_t exp = (nbits >> 24) & 0xff;
  std::uint32_t mant = nbits & 0x007fffff;
  int idx = static_cast<int>(32 - exp);
  if (idx >= 0 && idx + 2 < 32) {
    target[idx] = (mant >> 16) & 0xff;
    target[idx + 1] = (mant >> 8) & 0xff;
    target[idx + 2] = mant & 0xff;
  }
  return target;
}

bool meetsTarget(const miner::Hash256& hash, const std::array<std::uint8_t, 32>& target) {
  for (std::size_t i = 0; i < 32; ++i) {
    if (hash[i] < target[i]) return true;
    if (hash[i] > target[i]) return false;
  }
  return true;
}
} // namespace miner::work
