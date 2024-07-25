#include "mu3io_bsod.h"

#include <string.h>

static const int MIN_LEVER = 100;
static const int MAX_LEVER = 600;

static uint8_t g_lflag = 0;
static uint8_t g_rflag = 0;
static uint8_t g_mflag = 0;
static uint16_t g_lever = 0;

static inline void apply_flag(uint8_t in, uint8_t bit, uint8_t target, uint8_t* out)
{
    if(in & bit) {
        *out |= target;
    }
}

int poll()
{
    libusb_handle_events_completed(NULL, NULL);

    libusb_device_handle* dev_handle = get_dev_handle();
    if(!dev_handle) {
        return 1;
    }

    uint8_t data[8];
    memset(data, 0, sizeof(data));

    int rc = libusb_bulk_transfer(dev_handle, 0x84, data, sizeof(data), NULL, 0);
    if(rc) {
        return rc;
    }

    rc = data[0] != 0x44 || data[1] != 0x44 || data[2] != 0x54;
    if(rc) {
        return 1;
    }

    g_lflag = 0;
    g_rflag = 0;
    g_mflag = 0;

    apply_flag(data[3], 0x20, 0x01, &g_lflag);
    apply_flag(data[3], 0x10, 0x02, &g_lflag);
    apply_flag(data[3], 0x08, 0x04, &g_lflag);

    apply_flag(data[4], 0x80, 0x08, &g_lflag);
    apply_flag(data[4], 0x20, 0x10, &g_lflag);

    apply_flag(data[3], 0x04, 0x01, &g_rflag);
    apply_flag(data[3], 0x02, 0x02, &g_rflag);
    apply_flag(data[3], 0x01, 0x04, &g_rflag);

    apply_flag(data[4], 0x40, 0x08, &g_rflag);
    apply_flag(data[4], 0x10, 0x10, &g_rflag);

    apply_flag(data[4], 0x08, 0x01, &g_mflag);
    apply_flag(data[4], 0x04, 0x02, &g_mflag);

    static const double abs_center = (double)(MIN_LEVER + MAX_LEVER) / 2.0;
    static const double range_center = (double)(MIN_LEVER - MAX_LEVER) / 2.0;

    g_lever = (MAX_LEVER + MIN_LEVER) - ((uint8_t)data[6] | ((uint16_t)data[5] << 8));
    double normalized = (g_lever - abs_center) / range_center;
    g_lever = (uint16_t)(SHRT_MAX * normalized);

    return 0;
}

void get_opbtns(uint8_t* opbtn)
{
    *opbtn = g_mflag;
}

void get_gamebtns(uint8_t* left, uint8_t* right)
{
    *left = g_lflag;
    *right = g_rflag;
}

void get_lever(int16_t* pos)
{
    *pos = g_lever;
}