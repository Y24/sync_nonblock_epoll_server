#ifndef _SERVER_EPOLL_MANAGER_H
#define _SERVER_EPOLL_MANAGER_H 1
#include <sys/epoll.h>
#include <unistd.h>

#include <unordered_map>

#include "config.h"
#include "server_event_handler.h"
class EpollManager {
 private:
  int listenFd;
  int size;
  int epollFd;
  epoll_event* events;
  int nEvents;
  std::unordered_map<int, DemoData> data;

  EventHandler handler;

 public:
  EpollManager(int listenFd, int size, int nEvents);
  EpollManager(EpollManager& other);
  // Wait for {timeout} ms per loop.
  void work(int timeout = EPOLL_TIMEOUT);
  ~EpollManager();
};
#endif  // server_epoll_manager.h