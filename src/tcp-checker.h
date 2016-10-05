class TcpChecker {
  private:
    struct linger so_linger = { 1, 0 };
    struct timeval send_tv = { 1, 0 };
    struct timeval rcv_tv = { 1, 0 };
    uv_tcp_t tcp_t;
    uv_connect_t connect_t;
    sockaddr_in dest;
    int fd = -1;
    int status = 0;
    static void on_close(uv_handle_t* handle);
    static void on_connect(uv_connect_t* req, int status);
    void done();
    void free();
    function<void(int status)> callback;
    TcpChecker *self;
  public:
    TcpChecker(char *ip, unsigned int port, function<void(int status)> cb);
};
