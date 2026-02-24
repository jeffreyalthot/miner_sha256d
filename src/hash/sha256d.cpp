#include "miner/hash.hpp"

namespace miner::hash {
miner::Hash256 sha256d(std::span<const std::uint8_t> data) {
  auto first = sha256(data);
  return sha256(first);
}
} // namespace miner::hash
