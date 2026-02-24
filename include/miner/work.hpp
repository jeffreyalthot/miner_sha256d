#pragma once
#include "miner/common.hpp"
#include <array>
#include <cstdint>
#include <optional>
#include <vector>

namespace miner::work {

struct BitcoinHeader {
  std::uint32_t version;
  miner::Hash256 prevHash;
  miner::Hash256 merkleRoot;
  std::uint32_t ntime;
  std::uint32_t nbits;
  std::uint32_t nonce;
};

struct Job {
  std::uint32_t channelId{0};
  std::uint32_t jobId{0};
  std::uint64_t epoch{0};
  BitcoinHeader header{};
  std::vector<miner::Hash256> merklePath;
};

miner::Bytes serializeHeader(const BitcoinHeader& header);
miner::Hash256 computeMerkleRoot(const miner::Hash256& coinbaseHash, const std::vector<miner::Hash256>& path);
std::array<std::uint8_t, 32> nbitsToTarget(std::uint32_t nbits);
bool meetsTarget(const miner::Hash256& hash, const std::array<std::uint8_t, 32>& target);

} // namespace miner::work
