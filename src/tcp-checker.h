class TcpChecker {

  private:

    // Enable linger and set linger_time to 0
    // socket will skip TIME_WAIT state, and send a RST to replace FIN
    struct linger so_linger = { 1, 0 };

    // I want to set handshaking timeout with 1 second, but :joy:
    struct timeval send_tv = { 1, 0 };
    struct timeval rcv_tv = { 1, 0 };

    // Some data structs
    uv_tcp_t tcp_t;
    uv_connect_t connect_t;
    sockaddr_in dest;

    // Event callbacks
    static void on_close(uv_handle_t *handle);
    static void on_connect(uv_connect_t *req, int status);

    // Methods
    void done();
    void close();

    // Final callback
    function<void(int status)> callback;

  public:

    TcpChecker(char *ip, unsigned int port, function<void(int status)> cb);

    int fd = -1;
    int status = 0;

};
