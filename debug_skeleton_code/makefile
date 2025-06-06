AS_SRCS = $(shell find src/ -name '*.S')
C_SRCS = $(shell find src/ -name '*.c')
AS_OBJS = $(AS_SRCS:.S=.o)
C_OBJS = $(C_SRCS:.c=.o)

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

lib/libdebug.o: $(AS_OBJS) $(C_OBJS)
	ld -r -o $@ $^

test/main: test/main.c lib/libdebug.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(AS_OBJS) $(C_OBJS) lib/libdebug.o test/main
