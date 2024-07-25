#include "mu3io_bsod.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static const int VENDOR_ID = 0x0E8F;
static const int PRODUCT_ID = 0x1216;

static libusb_device_handle* g_dev_handle = NULL;
static libusb_hotplug_callback_handle g_callback_handle = -1;

static void clean_up();
static void print_ontroller_status(int);
static int hotplug_callback(
    libusb_context*, libusb_device*,
    libusb_hotplug_event, void*
);

libusb_device_handle* get_dev_handle()
{
    return g_dev_handle;
}

int init()
{
    println("init");

    int rc = libusb_init_context(NULL, NULL, 0);
    if(rc != 0) {
        println("libusb_init: %d", rc);
        return rc;
    }

    if(libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG)) {
        rc = libusb_hotplug_register_callback(
            NULL,
            LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED
                | LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT,
            LIBUSB_HOTPLUG_ENUMERATE,
            VENDOR_ID,
            PRODUCT_ID,
            LIBUSB_HOTPLUG_MATCH_ANY,
            hotplug_callback,
            (void*)4,
            &g_callback_handle
        );
        println("hotplug: %d", rc);
    } else {
        rc = 1;
        println("hotplug unavailable");
    }

    atexit(clean_up);
    return rc;
}

static void clean_up()
{
    println("clean up");

    if(g_dev_handle) {
        libusb_close(g_dev_handle);
    }
    if(g_callback_handle >= 0) {
        libusb_hotplug_deregister_callback(NULL, g_callback_handle);
    }
    libusb_exit(NULL);
}

static int hotplug_callback(
    libusb_context* ctx,
    libusb_device* dev,
    libusb_hotplug_event event,
    void* retry_count
){
    println("hotplug");
    (void)ctx;

    struct libusb_device_descriptor desc;
    libusb_get_device_descriptor(dev, &desc);

    if(event == LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED && !g_dev_handle) {
        int rc = libusb_open(dev, &g_dev_handle);
        if(rc == LIBUSB_ERROR_ACCESS && (long)retry_count > 0) {
            // I don't know why
            // it sometimes errors out even though it should have access
            // this is yet to be investigated
            return hotplug_callback(
                NULL, dev, event,
                (void*)((long)retry_count - 1)
            );
        }
        print_ontroller_status(rc);
    } else if(event == LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT && g_dev_handle) {
        libusb_close(g_dev_handle);
        g_dev_handle = NULL;
        fprintf(stderr, "[%s] ONTROLLER disconnected\n", DRIVER_NAME);
    } else {
        println("Unhandled event %d", event);
    }

    return 0;
}

static void print_ontroller_status(int rc)
{
    if(rc == LIBUSB_ERROR_ACCESS) {
        fprintf(
            stderr,
            "[%s] access to ontroller denied; check udev rules\n",
            DRIVER_NAME
        );
    } else if(rc != 0) {
        fprintf(stderr, "[%s] libusb_open: %d\n", DRIVER_NAME, rc);
    } else {
        fprintf(stderr, "[%s] ONTROLLER connected\n", DRIVER_NAME);
    }
}