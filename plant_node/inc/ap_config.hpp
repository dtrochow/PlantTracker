
#define TCP_PORT 80
#define DEBUG_printf printf
#define POLL_TIME_S 5
#define HTTP_GET "GET"
#define LED_TEST_BODY "<html><body><h1>Hello from Pico W.</h1><p>Led is %s</p><p><a href=\"?led=%d\">Turn led %s</a></body></html>"
#define LED_PARAM "led=%d"
#define LED_TEST "/ledtest"
#define LED_GPIO 0
#define HTTP_RESPONSE_REDIRECT "HTTP/1.1 302 Redirect\nLocation: http://%s" LED_TEST "\n\n"

#define WIFI_CONFIG_BODY "<html><body><h1>Wi-Fi Configuration</h1>" \
                         "<form action=\"/config\" method=\"get\">" \
                         "SSID: <input type=\"text\" name=\"ssid\"><br>" \
                         "Password: <input type=\"password\" name=\"password\"><br>" \
                         "<input type=\"submit\" value=\"Submit\">" \
                         "</form></body></html>"

#define HTTP_RESPONSE_HEADERS "HTTP/1.1 %d OK\nContent-Length: %d\nContent-Type: text/html; charset=utf-8\nConnection: close\n\n"
