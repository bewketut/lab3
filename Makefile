CC= g++
all:
	$(CC) -c *.cc *.cpp
	$(CC) -o main *.o 
.PHONY: clean
clean:	
	rm -rf *.o main


