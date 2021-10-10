/// Note: IOHandler wrap all the raw read and write and provide a higher-level
/// IO-fn
#ifndef _IO_HANDLER_H
#define _IO_HANDLER_H 1
#include <unistd.h>

#include "demo_data.h"
enum IOStatusCode {
  io_would_block,
  io_successed,
  io_error,
};
class IOHandler {
 private:
  int fd;
  static const DataFactory factory;
  static void inPanic(int fd) {
    // perror("read error:");
    close(fd);
  }
  static void outPanic(int fd) {
    perror("write error:");
    close(fd);
  }

 public:
  IOHandler(int fd);
  std::pair<IOStatusCode, DemoData> read();
  IOStatusCode write(DemoData data);
  ~IOHandler() = default;
};

#endif  // io_handler.h