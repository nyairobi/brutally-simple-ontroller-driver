#pragma once

#include <libusb-1.0/libusb.h>
#include <stdint.h>

int init();
int poll();
void get_opbtns(uint8_t*);
void get_gamebtns(uint8_t*, uint8_t*);
void get_lever(int16_t*);
void led_init();

libusb_device_handle* get_dev_handle();

#define DRIVER_NAME "mu3io-bsod"

#ifdef DEBUG
#   define println(...)                        \
        fprintf(stderr, "[%s] ", DRIVER_NAME), \
        fprintf(stderr, __VA_ARGS__),          \
        fprintf(stderr, "\n")
#else
#   define println(...)
#endif

#if LIBUSB_API_VERSION < 0x0100010A
#   define libusb_init_context(a, b, c) libusb_init(a)
#endif