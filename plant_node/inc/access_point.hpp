#include "pico/async_context.h"
#include "pico/cyw43_arch.h"
#include "dhcpserver.h"
#include "dnsserver.h"
#include "lwip/tcp.h"
#include "ap_config.hpp"

typedef struct TCP_SERVER_T_ {
    struct tcp_pcb *server_pcb;
    bool complete;
    ip_addr_t gw;
    async_context_t *context;
} TCP_SERVER_T;

typedef struct TCP_CONNECT_STATE_T_ {
    struct tcp_pcb *pcb;
    int sent_len;
    char headers[128];
    char result[256];
    int header_len;
    int result_len;
    ip_addr_t *gw;
    bool *complete;
} TCP_CONNECT_STATE_T;

enum InitStatus{
    AP_SUCCESS,
    AP_ERROR
};

class AccessPoint {
public:
    AccessPoint();
    ~AccessPoint();

    InitStatus init();
    void deinit();
public:
    TCP_SERVER_T *state;
    dhcp_server_t dhcp_server;
    dns_server_t dns_server;

    char ssid[64];
    char passwd[64];
private:
    bool tcp_server_open(void *arg, const char *ap_name);
    void tcp_server_close(TCP_SERVER_T *state);
    int test_server_content(const char *request, const char *params, char *result, size_t max_result_len);
    void tcp_server_err(void *arg, err_t err);
    void decode_url(char *src);
};
