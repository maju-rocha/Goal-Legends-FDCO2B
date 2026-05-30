main: main.o figurinhas.o
	@echo "Gerando arquivo executável..."
	gcc main.o figurinhas.o -o main

main.o: main.c figurinhas.h
	@echo "Compilando main.c..."
	gcc -c main.c

figurinhas.o: figurinhas.c figurinhas.h
	@echo "Compilando figurinhas.c..."
	gcc -c figurinhas.c

clean:
	@echo "Apagando objetos e executável..."
	rm -f *.o main

run: main
	./main