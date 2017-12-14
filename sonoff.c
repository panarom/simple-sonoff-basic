#include "power.h"
#include "button.h"
#include "wifi.h"
#include "ets_sys.h"

void ICACHE_FLASH_ATTR user_init()
{
    power_state_init();
    button_init();
    wifi_init();
}
