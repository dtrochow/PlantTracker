/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"

#include "ap_config.hpp"
#include "access_point.hpp"
#include "tcp_client.hpp"


int main() {
    stdio_init_all();

    AccessPoint ap;
    ap.init();

    ap.state->complete = false;
    while(!ap.state->complete) {
        printf("Complete: %d\n", ap.state->complete);
        sleep_ms(2000);
    }

    ap.deinit();
    TcpClient tcp_cli;
    tcp_cli.init();

    while(true) {
        sleep_ms(1000);
    }

    return 0;
}
