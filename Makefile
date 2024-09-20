CC1=gcc
CC2=winegcc
CFLAGS=-Wall -Wextra -O3 -std=c11 -pedantic -DDEBUG
LDFLAGS=-shared -ldl
SRC1=src/core.c src/poll.c src/led.c src/log.c
SRC2=src/dllmain.c src/dllmain.spec src/log.c

all:
	$(CC1) $(CFLAGS) -fPIC -o lib/mu3io_bsod.so $(LDFLAGS) -lusb-1.0 $(SRC1)
	$(CC2) -Wall -Wextra -m64 -DDEBUG -o lib/mu3io_bsod.dll $(LDFLAGS) $(SRC2)
	mv lib/mu3io_bsod.dll.so lib/mu3io_bsod.dll

clean:
	rm lib/mu3io_bsod.dll
	rm lib/mu3io_bsod.so

.PHONY: clean