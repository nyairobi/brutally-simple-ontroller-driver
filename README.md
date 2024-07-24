## Brutally Simple Ontroller Driver

This is a segatools io4 module for [ONTROLLER](https://www.dj-dao.com/en/ontroller) written in C with libusb. The name is supposed to scare Windows users off; you probably want to use [Mu3IO.NET](https://github.com/SirusDoma/Mu3IO.NET). If you want to play with a keyboard on Linux, you also want to use Mu3IO.NET.

### Why

Because the existing solutions didn't work on Linux in HID mode. Winusb [seems to be unimplemented](https://wiki.winehq.org/Hardware). It doesn't help that Mu3IO.NET is made specifically for .NET AOT which can't even be cross-compiled on Linux. 

### How it works

The DLL seen by segatools is a thin wrapper built with `winegcc` which redirects to a native Linux library (that uses the system's native libusb).

### Setup

By default, ONTROLLERs seem to be inaccessible in user space. Add a udev rule:

```sh
# echo 'SUBSYSTEMS=="usb", ATTRS{idVendor}=="0e8f", ATTRS{idProduct}=="1216", MODE="0666"' > /etc/udev/rules.d/10-ontroller.rules
# udevadm control --reload-rules
```

Re-plug the ONTROLLER if it was connected.

Add `mu3io_bsod.dll` and `mu3io_bsod.so` to the game directory and point at the DLL in `segatools.ini`:

```ini
[mu3io]
path=mu3io_bsod.dll
```

### Known issues
* LEDs are not implemented yet.
* Screensaver inhibition is not implemented yet.