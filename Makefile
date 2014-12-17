CC = gcc
CFLAGS = -Werror
LDFLAGS = -lcrypto
TARGET = cmd5_lib cmd5

all:$(TARGET)

cmd5_lib:
	$(CC) libcmd5.c $(CFLAGS) $(LDFLAGS) -fPIC -shared -o cmd5.so

cmd5:
	$(CC) cmd5.c $(CFLAGS) $(LDFLAGS) -o $@

install:
	sudo cp cmd5.so /usr/lib/lua

clean:
	rm cmd5.so
	rm cmd5
