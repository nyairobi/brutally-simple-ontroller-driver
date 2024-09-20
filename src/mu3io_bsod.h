#pragma once

#include <libusb-1.0/libusb.h>
#include <stdint.h>

#include "log.h"

int init();
int poll();
void get_opbtns(uint8_t*);
void get_gamebtns(uint8_t*, uint8_t*);
void get_lever(int16_t*);
int led_init();
void led_set_colors(uint8_t, uint8_t*);
void led_write(libusb_device_handle*);

libusb_device_handle* get_dev_handle();

#if LIBUSB_API_VERSION < 0x0100010A
#   define libusb_init_context(a, b, c) libusb_init(a)
#endif