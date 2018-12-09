all: main.o fileReader.o stack.o print.o
	gcc main.o fileReader.o stack.o print.o -pedantic -Wall -o analyser

main.o: src/main.c src/fileReader.h src/stack.h src/printUtils.h
	gcc src/main.c -c -pedantic -Wall -o main.o

fileReader.o: src/fileReader.c src/fileReader.h
	gcc src/fileReader.c -c -pedantic -Wall -o fileReader.o

stack.o: src/stack.c src/stack.h
	gcc src/stack.c -c -pedantic -Wall -o stack.o

print.o: src/printUtils.c src/printUtils.h
	gcc src/printUtils.c -c -pedantic -Wall -o print.o

clean: all
	rm -f *.o
	
test: clean
	./analyser src/main.c src/fileReader.c src/fileReader.h src/stack.c src/stack.h src/printUtils.c src/printUtils.h > test/testOutput.txt