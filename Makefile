main: main.o figurinhas.o
	@echo "Gerando executavel..." 
	gcc main.o figurinhas.o -o main

main.o: main.c figurinhas.h 
	gcc -c main.c

figurinhas.o: figurinhas.c figurinhas.h 
	gcc -c figurinhas.c

clean: 
	rm -f *.o main
	 
run: main 
	./main