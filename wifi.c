#include "wifi.h"
#include "mdns.h"
#include "http.h"
#include "user_interface.h"
#include "osapi.h"

//these are to pass in strings from the compilation command line for wifi configuration; to avoid hard-coding credentials in source
#define STRNAME1(n) #n
#define STRNAME(n) STRNAME1(n)

void ICACHE_FLASH_ATTR wifi_event_handler(System_Event_t *evt)
{
    if(evt->event != EVENT_STAMODE_GOT_IP) return;

    configure_mdns();
    start_http_server();
}

void ICACHE_FLASH_ATTR configure_wifi(struct station_config *wifi)
{
    wifi_set_opmode(STATION_MODE);

    wifi->bssid_set = 0;
    os_memcpy(wifi->ssid, STRNAME(WIFI_SSID), os_strlen(STRNAME(WIFI_SSID)));
    os_memcpy(wifi->password, STRNAME(WIFI_PASSWORD), os_strlen(STRNAME(WIFI_PASSWORD)));

    wifi_station_set_config(wifi);
    system_restart();
}

void ICACHE_FLASH_ATTR wifi_init()
{
#ifdef HOSTNAME
    wifi_station_set_hostname(STRNAME(HOSTNAME));

#endif
    struct station_config wifi_config[5]; //esp8266 remembers 5 APs
    //if no APs stored, connect to wifi & restart; otherwise handle ip address assignment
    if(wifi_station_get_ap_info(wifi_config) == 0) configure_wifi(wifi_config);
    else wifi_set_event_handler_cb(&wifi_event_handler);
}
