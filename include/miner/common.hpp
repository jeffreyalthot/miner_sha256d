#pragma once
#include <array>
#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace miner {
using Bytes = std::vector<std::uint8_t>;
using Hash256 = std::array<std::uint8_t, 32>;

std::string toHex(std::span<const std::uint8_t> bytes);
Bytes fromHex(const std::string& hex);
} // namespace miner
