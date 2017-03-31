CC = cc
OBJS := tcp_server.o tcp_client.o
CFLAGS := -g -Wall -O

all: tcp_server tcp_client
tcp_server: tcp_server.o
	$(CC)  $<  -o $@  $(CFLAGS)

tcp_client: tcp_client.o
	$(CC)  $<  -o $@  $(CFLAGS)
$(OBJS):%.o:%.c

.PHONY:clean
clean:
	@rm -rf *.o tcp_server tcp_client








