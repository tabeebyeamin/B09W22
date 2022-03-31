CC = gcc
PORT=30001
CFLAGS = -DPORT=$(PORT) -g -Wall

all: countdown client

countdown: countdown.o wrapsock.o
	${CC} ${CFLAGS} -o $@ countdown.o  wrapsock.o

client: client.o wrapsock.o
	${CC} ${CFLAGS} -o $@ client.o wrapsock.o

.c.o:
	${CC} ${CFLAGS}  -c $<

clean:
	rm *.o 

realclean:
	rm countdown client

