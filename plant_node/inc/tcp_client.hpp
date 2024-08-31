
enum  InitTcpClientStatus{
    SUCCESS,
    ERROR
};

class TcpClient {
public:
    TcpClient();
    ~TcpClient();

    InitTcpClientStatus init();
public:

private:
};
