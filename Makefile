OBJS=graph.o SA.o main.o
CC=g++
CFLAGS=-c -std=c++11 -O3

main:$(OBJS)
	$(CC) $^ -o main -O3
%.o:src/%.cpp
	$(CC) $^ $(CFLAGS)

clean:
	$(RM) *.o main