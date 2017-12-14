#include "service.h"
#include "mdns.h"
#include "user_interface.h"
#include "espconn.h"

void configure_mdns()
{
    struct ip_info ipconfig;
    struct mdns_info mdns;

    wifi_get_ip_info(STATION_IF, &ipconfig);

    mdns.host_name = wifi_station_get_hostname();
    mdns.ipAddr = ipconfig.ip.addr; //ESP8266 Station IP
    mdns.server_name = "powerswitch";
    mdns.server_port = API_PORT_NUMBER;
    espconn_mdns_init(&mdns);
}
