#include "tcp_client.hpp"

#include <pico/time.h>
#include <string.h>
#include <time.h>

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"

#define TEST_TCP_SERVER_IP "192.168.8.180"

#define CLIENT_TCP_PORT 8080  // Standard HTTP port
#define printf printf
#define BUF_SIZE 2048

#define TEST_ITERATIONS 10
#define POLL_TIME_S 5

extern char ssid[64];
extern char passwd[64];

typedef struct TCP_CLIENT_T_ {
    struct tcp_pcb *tcp_pcb;
    ip_addr_t remote_addr;
    uint8_t buffer[BUF_SIZE];
    int buffer_len;
    int sent_len;
    bool complete;
    int run_count;
    bool connected;
} TCP_CLIENT_T;

static err_t tcp_client_close(void *arg) {
    TCP_CLIENT_T *state = (TCP_CLIENT_T*)arg;
    err_t err = ERR_OK;
    if (state->tcp_pcb != NULL) {
        tcp_arg(state->tcp_pcb, NULL);
        tcp_poll(state->tcp_pcb, NULL, 0);
        tcp_sent(state->tcp_pcb, NULL);
        tcp_recv(state->tcp_pcb, NULL);
        tcp_err(state->tcp_pcb, NULL);
        err = tcp_close(state->tcp_pcb);
        if (err != ERR_OK) {
            printf("close failed %d, calling abort\n", err);
            tcp_abort(state->tcp_pcb);
            err = ERR_ABRT;
        }
        state->tcp_pcb = NULL;
    }
    return err;
}

// Called with results of operation
static err_t tcp_result(void *arg, int status) {
    TCP_CLIENT_T *state = (TCP_CLIENT_T*)arg;
    if (status == 0) {
        printf("test success\n");
    } else {
        printf("test failed %d\n", status);
    }
    state->complete = true;
    return tcp_client_close(arg);
}

// Handle TCP data sent event
static err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    TCP_CLIENT_T *state = (TCP_CLIENT_T*)arg;
    printf("tcp_client_sent %u\n", len);
    state->sent_len += len;

    // Check if all data has been sent
    if (state->sent_len >= state->buffer_len) {
        state->run_count++;
        tcp_result(arg, 0);  // Close connection after data is sent
        return ERR_OK;
    }

    return ERR_OK;
}

// Handle TCP connection established
static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err) {
    TCP_CLIENT_T *state = (TCP_CLIENT_T*)arg;
    if (err != ERR_OK) {
        printf("connect failed %d\n", err);
        return tcp_result(arg, err);
    }
    state->connected = true;
    printf("Connected to server, sending request\n");

    // Initialize random seed
    srand(time(NULL));

    // Generate a random integer value for the Temperature header
    int random_temperature = rand() % 100;  // Random temperature value between 0 and 99

    // Prepare HTTP GET request with Temperature header
    const char *request_template = "GET /test HTTP/1.1\r\n"
                                   "Host: " TEST_TCP_SERVER_IP "\r\n"
                                   "Temperature: %d\r\n"
                                   "Connection: close\r\n"
                                   "\r\n";
    
    // Format the request with the random temperature
    snprintf((char*)state->buffer, sizeof(state->buffer), request_template, random_temperature);
    state->buffer_len = strlen((char*)state->buffer);

    // Send HTTP GET request to the server
    err_t write_err = tcp_write(tpcb, state->buffer, state->buffer_len, TCP_WRITE_FLAG_COPY);
    if (write_err != ERR_OK) {
        printf("Failed to write data %d\n", write_err);
        return tcp_result(arg, -1);
    }
    return ERR_OK;
}

// Handle TCP data reception
err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    TCP_CLIENT_T *state = (TCP_CLIENT_T*)arg;
    if (!p) {
        return tcp_result(arg, -1);
    }
    cyw43_arch_lwip_check();
    if (p->tot_len > 0) {
        printf("recv %d err %d\n", p->tot_len, err);
        for (struct pbuf *q = p; q != NULL; q = q->next) {
            printf("%.*s", q->len, (char*)q->payload);  // Print received data as string
        }
        tcp_recved(tpcb, p->tot_len);
    }
    pbuf_free(p);
    return ERR_OK;
}

// Handle TCP connection polling
static err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb) {
    printf("tcp_client_poll\n");
    return tcp_result(arg, -1); // no response is an error?
}

// Handle TCP connection error
static void tcp_client_err(void *arg, err_t err) {
    if (err != ERR_ABRT) {
        printf("tcp_client_err %d\n", err);
        tcp_result(arg, err);
    }
}

// Perform initialisation
static TCP_CLIENT_T* tcp_client_init(void) {
    TCP_CLIENT_T *state = (TCP_CLIENT_T*)calloc(1, sizeof(TCP_CLIENT_T));
    if (!state) {
        printf("failed to allocate state\n");
        return NULL;
    }
    ip4addr_aton(TEST_TCP_SERVER_IP, &state->remote_addr);
    return state;
}

// Open a TCP connection to the server
static bool tcp_client_open(void *arg) {
    TCP_CLIENT_T *state = (TCP_CLIENT_T*)arg;
    printf("Connecting to %s port %u\n", ip4addr_ntoa(&state->remote_addr), CLIENT_TCP_PORT);
    state->tcp_pcb = tcp_new_ip_type(IP_GET_TYPE(&state->remote_addr));
    if (!state->tcp_pcb) {
        printf("failed to create pcb\n");
        return false;
    }

    tcp_arg(state->tcp_pcb, state);
    tcp_poll(state->tcp_pcb, tcp_client_poll, POLL_TIME_S * 2);
    tcp_sent(state->tcp_pcb, tcp_client_sent);
    tcp_recv(state->tcp_pcb, tcp_client_recv);
    tcp_err(state->tcp_pcb, tcp_client_err);

    state->buffer_len = 0;

    cyw43_arch_lwip_begin();
    err_t err = tcp_connect(state->tcp_pcb, &state->remote_addr, CLIENT_TCP_PORT, tcp_client_connected);
    cyw43_arch_lwip_end();

    return err == ERR_OK;
}

// Run the TCP client test
void run_tcp_client_test(void) {
    TCP_CLIENT_T *state = tcp_client_init();
    if (!state) {
        return;
    }
    if (!tcp_client_open(state)) {
        tcp_result(state, -1);
        return;
    }
    while(!state->complete) {
        sleep_ms(1000);
    }
    free(state);
}


TcpClient::TcpClient() {

}

TcpClient::~TcpClient() {

}

InitTcpClientStatus TcpClient::init() {
    if (cyw43_arch_init_with_country(CYW43_COUNTRY_POLAND)) {
        printf("failed to initialise\n");
        return InitTcpClientStatus::ERROR;
    }
    cyw43_arch_enable_sta_mode();

    printf("WIFI: %s\n", ssid);
    printf("Connecting to Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(ssid, passwd, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
        return InitTcpClientStatus::ERROR;
    } else {
        printf("Connected.\n");
    }
    printf("Starting test\n");
    run_tcp_client_test();
    printf("Test finished\n");

    return InitTcpClientStatus::SUCCESS;
}
