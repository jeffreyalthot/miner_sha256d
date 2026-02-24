#include "miner/sv2.hpp"

namespace miner::sv2 {
miner::Bytes encodeFrame(const Frame& frame) {
  miner::Bytes out;
  std::uint32_t len = static_cast<std::uint32_t>(frame.payload.size() + 2);
  out.push_back((len >> 24) & 0xff);
  out.push_back((len >> 16) & 0xff);
  out.push_back((len >> 8) & 0xff);
  out.push_back(len & 0xff);
  out.push_back((frame.type >> 8) & 0xff);
  out.push_back(frame.type & 0xff);
  out.insert(out.end(), frame.payload.begin(), frame.payload.end());
  return out;
}

std::optional<Frame> decodeFrame(miner::Bytes& buffer) {
  if (buffer.size() < 6) return std::nullopt;
  std::uint32_t len = (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];
  if (buffer.size() < len + 4) return std::nullopt;
  Frame f;
  f.type = static_cast<std::uint16_t>((buffer[4] << 8) | buffer[5]);
  f.payload.assign(buffer.begin() + 6, buffer.begin() + 4 + len);
  buffer.erase(buffer.begin(), buffer.begin() + 4 + len);
  return f;
}
} // namespace miner::sv2
