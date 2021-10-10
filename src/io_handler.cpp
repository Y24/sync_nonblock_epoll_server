/// Note: IOHandler wrap all the raw read and write and provide a higher-level
///
#include "io_handler.h"
IOHandler::IOHandler(int fd) : fd(fd) {}
std::pair<IOStatusCode, DemoData> IOHandler::read() {
  char nSizeBuf;
  if (int cnt = ::read(fd, &nSizeBuf, 1); cnt == -1) {
    if (errno == EWOULDBLOCK || errno == EAGAIN) {
      return std::make_pair(io_would_block, DemoData());
    }
    inPanic(fd);
    return std::make_pair(io_error, DemoData(data_invalid));
  } else if (cnt == 0) {
    fprintf(stderr, "server close.\n");
    close(fd);
    return std::make_pair(io_successed, DemoData(conn_close));
  } else {
    int nSize = factory.charTo<int>(nSizeBuf);
    char* sizeBuf = new char[nSize + 1];
    if (int cnt = ::read(fd, sizeBuf, nSize); cnt < nSize) {
      inPanic(fd);
      return std::make_pair(io_error, DemoData(data_invalid));
    }
    sizeBuf[nSize] = '\0';
    auto [status, size] = factory.stringTo<long long>(std::string(sizeBuf));
    delete[] sizeBuf;
    if (!status || size <= 0) {
      inPanic(fd);
      return std::make_pair(io_error, DemoData(data_invalid));
    }
    char* source = new char[size];
    if (long long cnt = ::read(fd, source, size); cnt < size) {
      inPanic(fd);
      delete[] source;
      return std::make_pair(io_error, DemoData(data_invalid));
    } else {
      auto src = std::string(size, '0');
      for (int i = 0; i < size; i++) {
        src[i] = source[i];
      }
      auto res = DemoData(src);
      delete[] source;
      return std::make_pair(io_successed, res);
    }
  }
}
IOStatusCode IOHandler::write(DemoData data) {
  std::string sizeStr = factory.toString<long long>(data.getSize());
  char nSize = factory.toChar<int>(sizeStr.size());
  std::string nSizeStr = std::string(1, nSize);
  if (::write(fd, nSizeStr.data(), 1) < 1) {
    if (errno == EWOULDBLOCK || errno == EAGAIN) {
      return io_would_block;
    }
    return io_error;
  }
  if (::write(fd, sizeStr.data(), sizeStr.size()) < sizeStr.size()) {
    if (errno == EWOULDBLOCK || errno == EAGAIN) {
      return io_would_block;
    }
    outPanic(fd);
    return io_error;
  }
  std::string src = data.toStr();
  int nSrc = src.size();
  if (int cnt = ::write(fd, src.data(), src.size()); cnt < src.size()) {
    if (errno == EWOULDBLOCK || errno == EAGAIN) {
      return io_would_block;
    }
    outPanic(fd);
    return io_error;
  }
  return io_successed;
}
