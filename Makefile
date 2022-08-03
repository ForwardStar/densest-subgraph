OBJS=heap.o graph.o SA.o main.o
TESTHEAP=heap.o heap_test.o
TESTGRAPH=heap.o graph.o graph_test.o
CC=g++
CFLAGS=-c -std=c++11 -O3

test:$(TESTHEAP) $(TESTGRAPH)
	$(CC) $(TESTHEAP) -o heap_test -O3
	$(CC) $(TESTGRAPH) -o graph_test -O3
	./heap_test
	./graph_test
build:$(OBJS)
	$(CC) $^ -o main -O3 -lpthread
%.o:src/%.cpp
	$(CC) $^ $(CFLAGS)

clean:
	$(RM) *.o main *_test