#include "mu3io_bsod.h"

#include <string.h>

uint8_t g_led[33];
static int g_ready = 0;

static inline void set_color(int idx, uint8_t r, uint8_t g, uint8_t b)
{
    g_led[3 * idx + 3] = r;
    g_led[3 * idx + 4] = g;
    g_led[3 * idx + 5] = b;
}

int led_init()
{
    println("led_init");

    memset(g_led, 0, 33);
    g_led[0] = 0x44;
    g_led[1] = 0x4C;
    g_led[2] = 0x01;
    g_ready = 1;

    return 0;
}

void led_set_colors(uint8_t board, uint8_t* rgb)
{
    println("led_set_colors(%d)", board);

    if(board == 1) {
        for (int i = 0; i < 6; ++i) {
            set_color(i, rgb[i * 3], rgb[i * 3 + 1], rgb[i * 3 + 2]);
        }
    } else if(board == 0) {

    }
}

void led_write(libusb_device_handle* dev_handle)
{
    if(!g_ready) {
        led_init();
    }

    if(dev_handle && g_ready) {
        libusb_bulk_transfer(dev_handle, 0x03, g_led, sizeof(g_led), NULL, 0);
    }
}