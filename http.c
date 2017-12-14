#include "http.h"
#include "service.h"
#include "power.h"
#include "c_types.h"
#include "ip_addr.h"
#include "espconn.h"
#include "osapi.h"

static struct espconn server_conn;
static esp_tcp server_tcp_settings;
#define SOCKET_TIMEOUT 150//timeout in seconds

static struct response_message {
    uint16 code;
    char *message;
} RESPONSE_MESSAGES[2] = {
    {200, "OK"},
    {404, "Not Found"}
};

#define JSON_BODY_SIZE 256
#define HTTP_BODY_SIZE 256
#define RESPONSE_BODY_SIZE (HTTP_BODY_SIZE + JSON_BODY_SIZE)

static const char *JSON_TEMPLATE = "{\"state\": %d}";
static const char *HTTP_RESPONSE_TEMPLATE =
    "HTTP/1.1 %u %s\r\n"
    "Content-Type: application/json; charset=UTF-8\r\n"
    "Content-Length: %d\r\n"
    "\r\n"
    "%s";
static char response_body[RESPONSE_BODY_SIZE];

static void build_response(uint16 code, char *message, int state)
{
    static char json[JSON_BODY_SIZE];

    os_sprintf(json, JSON_TEMPLATE, state);

    os_sprintf(response_body, HTTP_RESPONSE_TEMPLATE, code, message, os_strlen(json), json);
}

static void respond(void *arg, char *pdata, unsigned short len)
{
    uint8 rsp_code_idx = 1;

    if(os_strstr(pdata, "power/on")) {
        power_switch(1);
        rsp_code_idx = 0;
    }
    if(os_strstr(pdata, "power/off")) {
        power_switch(0);
        rsp_code_idx = 0;
    }

    if(os_strstr(pdata, "power/state")) rsp_code_idx = 0;

    struct response_message rsp = RESPONSE_MESSAGES[rsp_code_idx];
    build_response(rsp.code, rsp.message, get_power_state());
    espconn_send(arg, response_body, os_strlen(response_body));
}

static void connection_opened(void *arg)
{
    struct espconn *client_conn = (struct espconn *)arg;

    espconn_regist_recvcb(client_conn, respond);
}

void ICACHE_FLASH_ATTR start_http_server()
{
 
    server_tcp_settings.local_port = API_PORT_NUMBER;

    server_conn.type = ESPCONN_TCP;
    server_conn.state = ESPCONN_NONE;
    server_conn.proto.tcp = &server_tcp_settings;

    espconn_regist_connectcb(&server_conn, connection_opened);
    espconn_accept(&server_conn);
    espconn_regist_time(&server_conn, SOCKET_TIMEOUT, 0);
}
