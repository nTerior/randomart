CFLAGS = -Wall -Wextra
OBJS = randomart.o

all: randomart

release: CFLAGS += -O3
release: randomart

debug: CFLAGS += -g
debug: randomart

randomart: $(OBJS)
	gcc $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	gcc $(CFLAGS) -c $<

clean:
	rm -f randomart
	rm -f $(OBJS)