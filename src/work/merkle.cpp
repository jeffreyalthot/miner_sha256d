#include "miner/work.hpp"
#include "miner/hash.hpp"

namespace miner::work {
miner::Hash256 computeMerkleRoot(const miner::Hash256& coinbaseHash, const std::vector<miner::Hash256>& path) {
  miner::Hash256 h = coinbaseHash;
  for (const auto& sibling : path) {
    miner::Bytes cat;
    cat.insert(cat.end(), h.begin(), h.end());
    cat.insert(cat.end(), sibling.begin(), sibling.end());
    h = miner::hash::sha256d(cat);
  }
  return h;
}
} // namespace miner::work
