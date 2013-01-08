TARGET=main
CC=gcc
CFLAGS= -O3 -Wall -I../utils -I../ -std=gnu99
HEADERS = $(wildcard *.h)
SOURCES= main.c
OBJS= $(SOURCES:%.c=%.o)
LDFLAGS= -lm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%o: %c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean: 
	$(RM) $(OBJS) $(TARGET)
