#include "miner/sv2.hpp"

#include <cerrno>
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace miner::sv2 {
namespace {
int openTcpSocket(const core::PoolProfile& pool) {
  addrinfo hints{};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  addrinfo* result = nullptr;
  auto port = std::to_string(pool.port);
  if (getaddrinfo(pool.host.c_str(), port.c_str(), &hints, &result) != 0) return -1;

  int fd = -1;
  for (addrinfo* rp = result; rp != nullptr; rp = rp->ai_next) {
    fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (fd < 0) continue;
    if (connect(fd, rp->ai_addr, rp->ai_addrlen) == 0) break;
    close(fd);
    fd = -1;
  }

  freeaddrinfo(result);
  return fd;
}
} // namespace

bool connectAndSetup(const core::PoolProfile& pool, const core::SV2Config& sv2Cfg, core::Logger& logger) {
  if (pool.tls) {
    logger.log(core::LogLevel::Warn, "TLS demandé mais non implémenté dans cette base (utiliser tls=false)", "sv2");
    return false;
  }

  auto fd = openTcpSocket(pool);
  if (fd < 0) {
    logger.log(core::LogLevel::Warn,
               "Connexion pool impossible " + pool.host + ":" + std::to_string(pool.port) + " err=" + std::strerror(errno),
               "sv2");
    return false;
  }

  SetupConnection setup;
  setup.deviceId = sv2Cfg.deviceId;
  auto setupPayload = encodeSetupConnection(setup);
  auto frame = encodeFrame(Frame{0x0001, setupPayload});

  auto written = send(fd, frame.data(), frame.size(), 0);
  if (written < 0) {
    logger.log(core::LogLevel::Warn, "Envoi SetupConnection échoué", "sv2");
    close(fd);
    return false;
  }

  logger.log(core::LogLevel::Info,
             "Pool connecté " + pool.name + " (" + pool.host + ":" + std::to_string(pool.port) + "), setup envoyé pour user=" + pool.user,
             "sv2");
  close(fd);
  return true;
}

} // namespace miner::sv2
