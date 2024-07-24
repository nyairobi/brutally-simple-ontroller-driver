#include "mu3io_bsod.h"

static uint8_t lflag = 0;
static uint8_t rflag = 0;
static uint8_t mflag = 0;
static uint16_t lever = 0;

static inline void apply_flag(uint8_t in, uint8_t bit, uint8_t target, uint8_t* output)
{
    if(in & bit) {
        *output |= target;
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
    if(rc != 0) {
        return 1;
    }

    rc = rc && data[0] == 0x44 && data[1] == 0x44 && data[2] == 0x54;

    if(rc != 0) {
        return 1;
    }

    lflag = 0;
    rflag = 0;
    mflag = 0;

    apply_flag(data[3], 0x20, 0x01, &lflag);
    apply_flag(data[3], 0x10, 0x02, &lflag);
    apply_flag(data[3], 0x08, 0x04, &lflag);

    apply_flag(data[4], 0x80, 0x08, &lflag);
    apply_flag(data[4], 0x20, 0x10, &lflag);

    apply_flag(data[3], 0x04, 0x01, &rflag);
    apply_flag(data[3], 0x02, 0x02, &rflag);
    apply_flag(data[3], 0x01, 0x04, &rflag);

    apply_flag(data[4], 0x40, 0x08, &rflag);
    apply_flag(data[4], 0x10, 0x10, &rflag);

    apply_flag(data[4], 0x08, 0x01, &mflag);
    apply_flag(data[4], 0x04, 0x02, &mflag);

    static const int min_lever = 100;
    static const int max_lever = 600;
    static const double abs_center = (double)(min_lever + max_lever) / 2.0;
    static const double range_center = (double)(min_lever - max_lever) / 2.0;

    lever = (max_lever + min_lever) - ((uint8_t)data[6] | ((uint16_t)data[5] << 8));
    double normalized = (lever - abs_center) / range_center;
    lever = (uint16_t)(SHRT_MAX * normalized);

    return 0;
}

void get_opbtns(uint8_t* opbtn) {
    *opbtn = mflag;
}

void get_gamebtns(uint8_t* left, uint8_t* right) {
    *left = lflag;
    *right = rflag;
}

void get_lever(int16_t* pos) {
    *pos = lever;
}