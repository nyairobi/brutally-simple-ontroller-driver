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