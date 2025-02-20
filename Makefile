CC1=gcc
CC2=winegcc
CFLAGS2=-Wall -Wextra -m64
CFLAGS1=$(CFLAGS2) -pedantic -std=c11 -fPIC
LDFLAGS=-shared -ldl
SRC1=src/core.c src/poll.c src/led.c src/log.c src/dbus.c
SRC2=src/dllmain.c src/dllmain.spec src/log.c

all:
	$(CC1) $(CFLAGS1) -o lib/mu3io_bsod.so  $(LDFLAGS) $(SRC1) -lusb-1.0 `pkg-config --libs libsystemd`
	$(CC2) $(CFLAGS2) -o lib/mu3io_bsod.dll $(LDFLAGS) $(SRC2)
	mv lib/mu3io_bsod.dll.so lib/mu3io_bsod.dll

clean:
	rm lib/mu3io_bsod.dll
	rm lib/mu3io_bsod.so

.PHONY: clean
