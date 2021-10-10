#ifndef _SOCKET_FACTORY_H
#define _SOCKET_FACTORY_H 1
#include <fcntl.h>
class SocketFactory {
 public:
  SocketFactory() = default;
  ~SocketFactory() = default;
  static bool setNonBlocked(int sockFd) {
    int flags = fcntl(sockFd, F_GETFL, 0);
    if (flags < 0) {
      return false;
    }
    if (fcntl(sockFd, F_SETFL, flags | O_NONBLOCK) < 0) {
      return false;
    }
    return true;
  }
};
#endif  // socket_factory.h
