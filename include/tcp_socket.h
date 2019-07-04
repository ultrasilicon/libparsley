#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "stream.h"
#include "abstract_socket.h"
#include "address.h"

P_NS_BEGIN

class TcpSocket
    : public Stream
    , public AbstractSocket
    , public PUvObject<uv_tcp_t, TcpSocket>
{
  typedef struct {
    uv_write_t req;
    uv_buf_t buf;
  } write_req_t;

  static void receiveCb(uv_stream_t *handle, ssize_t nread, const uv_buf_t *buf);
  static void writeCb(uv_write_t *handle, int status);
  static void connectCb(uv_connect_t *handle, int status);
  static void freeWriteReq(uv_write_t *handle);

public:
  CallbackHandler<void()> onConnected;
  CallbackHandler<void(const Buffer*, TcpSocket*)> onReadyRead;
  CallbackHandler<void(const SocketDescriptor&)> onWritten;

  TcpSocket(Loop *l);
  ~TcpSocket();

  int start();
  void stop();
  void close();
  int connect(const char* addr, const int &port);
  int write(char* data);
  int write(const std::string &data);
  void setKeepAlive(const bool &enabled, const int &delay);
  const IPAddress *peerAddress();
  const IPAddress *retrievePeerAddress();

protected:
  IPAddress *peer_address_ = nullptr;

};


P_NS_END
#endif // TCPSOCKET_H
