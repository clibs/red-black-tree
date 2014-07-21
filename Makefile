AR ?= ar
CC ?= gcc
CFLAGS = -Ideps -pedantic -std=c99 -v -Wall -Wextra

ifeq ($(APP_DEBUG),true)
	CFLAGS += -g -O0
else
	CFLAGS += -O2
endif

PREFIX ?= /usr/local

SRCS += $(wildcard src/*.c)

OBJS += $(SRCS:.c=.o)

all: build

%.o: %.c
	$(CC) $< $(CFLAGS) -c -o $@

build: build/lib/libjsw_rbtree.a
	mkdir -p build/include/jsw_rbtree
	cp -f src/jsw_rbtree.h build/include/jsw_rbtree/jsw_rbtree.h

build/lib/libjsw_rbtree.a: $(OBJS)
	mkdir -p build/lib
	$(AR) -crs $@ $^

clean:
	rm -fr *.o build example example.dSYM src/*.o

example: build
	$(CC) $(CFLAGS) -Ibuild/include -o example example.c -Lbuild/lib -ljsw_rbtree

install: all
	mkdir -p $(PREFIX)/include/jsw_rbtree
	mkdir -p $(PREFIX)/lib
	cp -f src/jsw_rbtree.h $(PREFIX)/include/jsw_rbtree/jsw_rbtree.h
	cp -f build/lib/libjsw_rbtree.a $(PREFIX)/lib/libjsw_rbtree.a

uninstall:
	rm -fr $(PREFIX)/include/jsw_rbtree/jsw_rbtree.h
	rm -fr $(PREFIX)/lib/libjsw_rbtree.a

.PHONY: build clean example install uninstall
