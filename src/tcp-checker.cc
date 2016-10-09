#include "./tcp-checker.h"

TcpChecker::TcpChecker(char *ip, unsigned int port, function<void(int status)> _callback) {

  #define CALL_UV(func, ...)                                                                                \
    status = func(__VA_ARGS__);                                                                             \
    if (status) {                                                                                           \
      cerr << "\033[31m" << uv_err_name(status) << ": " << uv_strerror(status) << "\033[0m" << endl;        \
      close();                                                                                               \
      return;                                                                                               \
    }

  callback = _callback;

  // Init Socket
  CALL_UV(uv_tcp_init_ex, uv_default_loop(), &tcp_t, AF_INET);
  tcp_t.data = (void *)this;

  // Set Optiosn
  CALL_UV(uv_fileno, (const uv_handle_s *)&tcp_t, &fd);
  setsockopt(fd, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));
  setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &send_tv, sizeof(send_tv));
  setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &rcv_tv, sizeof(rcv_tv));
  #ifdef TCP_QUICKACK
  int quick_act = 0;
  setsockopt(fd, SOL_TCP, TCP_QUICKACK, &quick_act, sizeof(quick_act));
  #endif

  // Init Connection
  CALL_UV(uv_ip4_addr, ip, port, &dest);
  CALL_UV(uv_tcp_connect, &connect_t, (uv_tcp_t *)&tcp_t, (const struct sockaddr *)&dest, on_connect);
  connect_t.data = (void *)this;

  auto name = status ? uv_err_name(status) : "OK";
  if (DEBUG) cout << "\033[32mfd: " << fd << ", status: " << name << "\033[0m" << endl;

  #undef CALL_UV

}

void TcpChecker::close() {
  if (fd != -1) {
    uv_close((uv_handle_s *)&tcp_t, on_close);
    fd = -1;
  } else {
    done();
  }
}

void TcpChecker::done() {
  auto name = status ? uv_err_name(status) : "OK";
  if (DEBUG) cout << "\033[34mfd: " << fd << ", status: " << name << "\033[0m" << endl;
  callback(status);
  delete this;
}

void TcpChecker::on_connect(uv_connect_t* req, int status) {
  auto that = (TcpChecker *)req->data;
  auto name = status ? uv_err_name(status) : "OK";
  if (DEBUG) cout << "\033[33mfd: " << that->fd << ", status: " << name << "\033[0m" << endl;
  that->status = status;
  that->close();
};

void TcpChecker::on_close(uv_handle_t* handle) {
  auto that = (TcpChecker *)handle->data;
  that->done();
};
