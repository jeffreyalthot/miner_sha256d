#pragma once
#include "miner/common.hpp"
#include <span>

namespace miner::hash {

miner::Hash256 sha256(std::span<const std::uint8_t> data);
miner::Hash256 sha256d(std::span<const std::uint8_t> data);

} // namespace miner::hash
