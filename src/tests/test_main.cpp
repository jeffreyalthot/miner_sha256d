#include "miner/hash.hpp"
#include "miner/work.hpp"
#include <cassert>
#include <iostream>

int main() {
  auto abc = miner::hash::sha256(std::span<const std::uint8_t>(reinterpret_cast<const std::uint8_t*>("abc"), 3));
  auto hex = miner::toHex(abc);
  assert(hex == "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");

  auto target = miner::work::nbitsToTarget(0x1d00ffff);
  miner::Hash256 maxHash{};
  maxHash.fill(0xff);
  assert(!miner::work::meetsTarget(maxHash, target));

  std::cout << "All tests passed\n";
  return 0;
}
