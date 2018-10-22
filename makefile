threads: threads.o
	g++ -o threads threads.o -lpthread

threads.o: threads.cpp
	g++ -c threads.cpp

run:
	./threads

clean:
	rm *.o threads
