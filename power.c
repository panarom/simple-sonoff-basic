#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "power.h"

static const int led = (1 << 13);
static const int relay = (1 << 12);

static int state = 0;

int get_power_state()
{
    return state;
}

void power_switch(int new_state)
{
    (new_state == 0) ?
        gpio_output_set(led, relay, led|relay, 0)
        : gpio_output_set(relay, led, led|relay, 0);

    state = new_state;
}

void ICACHE_FLASH_ATTR power_state_init()
{
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);

    //start in open-relay (power-off) state
    power_switch(0);
}
