#include "miner/work.hpp"

namespace miner::work {
namespace {
void pushLe32(miner::Bytes& out, std::uint32_t v) {
  out.push_back(v & 0xff);
  out.push_back((v >> 8) & 0xff);
  out.push_back((v >> 16) & 0xff);
  out.push_back((v >> 24) & 0xff);
}
} // namespace

miner::Bytes serializeHeader(const BitcoinHeader& h) {
  miner::Bytes out;
  out.reserve(80);
  pushLe32(out, h.version);
  out.insert(out.end(), h.prevHash.begin(), h.prevHash.end());
  out.insert(out.end(), h.merkleRoot.begin(), h.merkleRoot.end());
  pushLe32(out, h.ntime);
  pushLe32(out, h.nbits);
  pushLe32(out, h.nonce);
  return out;
}

} // namespace miner::work
