#include "dbus.h"

#include <stdio.h>
#include <systemd/sd-bus.h>

static sd_bus* bus = NULL;

int dbus_init()
{
    return sd_bus_open_user(&bus);
}

int dbus_inhibit()
{
    return bus == NULL ? -1 : sd_bus_call_method(
        bus,
        "org.freedesktop.ScreenSaver",
        "/org/freedesktop/ScreenSaver",
        "org.freedesktop.ScreenSaver",
        "SimulateUserActivity",
        NULL,
        NULL,
        ""
    );
}

void dbus_exit()
{
    if(bus != NULL) {
        sd_bus_unrefp(&bus);
    }
}