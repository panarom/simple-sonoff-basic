#include "button.h"
#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "power.h"

static const int button = (1 << 0);

static uint32 bounce = 0;
#define BOUNCE_MICROSECONDS 2000000

static void button_push_handler(int8_t key)
{
    uint32 now = system_get_time();
    if(now - bounce > BOUNCE_MICROSECONDS) {
        power_switch(!get_power_state());

        bounce = now;
    }

    //clear interrupt status
    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS,
      (GPIO_REG_READ(GPIO_STATUS_ADDRESS)) & BIT(0));
}

void ICACHE_FLASH_ATTR button_init()
{
    ETS_GPIO_INTR_DISABLE();
    gpio_init();
    // GPIO0 interrupt handler
    ETS_GPIO_INTR_ATTACH(button_push_handler, 0);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);
    gpio_output_set(0, 0, 0, button);
    // Clear GPIO0 status
    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(0));
    gpio_pin_intr_state_set(GPIO_ID_PIN(0), GPIO_PIN_INTR_LOLEVEL);
    ETS_GPIO_INTR_ENABLE();
}
