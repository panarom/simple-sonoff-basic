CC = xtensa-lx106-elf-gcc
CFLAGS = -I. -DICACHE_FLASH -mlongcalls -DWIFI_SSID=$(SSID) -DWIFI_PASSWORD=$(PASSWORD)
ifdef HOSTNAME
CFLAGS += -DHOSTNAME=$(HOSTNAME)
endif
LDLIBS = -nostdlib -Wl,--start-group -lmain -lnet80211 -lwpa -llwip -lpp -lphy -lc -Wl,--end-group -lgcc
LDFLAGS = -Teagle.app.v6.ld

DEPS = $(wildcard *.h)
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

sonoff-0x00000.bin: sonoff
	esptool.py elf2image $^

sonoff: $(OBJS)

%.o: %.c $(DEPS)
ifndef SSID
	$(error wifi SSID must be supplied as an environment variable)
endif
ifndef PASSWORD
	$(error wifi PASSWORD must be supplied as an environment variable)
endif

flash: sonoff-0x00000.bin
	esptool.py write_flash --flash_mode dout 0 sonoff-0x00000.bin 0x10000 sonoff-0x10000.bin

clean:
	rm -f sonoff $(OBJS) sonoff-0x00000.bin sonoff-0x10000.bin
