<<<<<<< HEAD
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
=======
CC = gcc

CFLAGS = -Iheaders -Iraylib/src
LDFLAGS = -Lraylib/src -lraylib -lm -lpthread -ldl -lrt -lX11

OBJ = main.o abrirPacote.o listarAlbum.o listarMochila.o \
    excluirAlbum.o excluirMochila.o pesquisar.o \
    alterar.o resetarLista.o quiz.o goleiro.o penalti.o

main: $(OBJ)
	@echo "Gerando executavel..."
	$(CC) $(OBJ) -o main $(LDFLAGS)

main.o: funcoes/main.c headers/biblioteca.h
	$(CC) $(CFLAGS) -c funcoes/main.c

abrirPacote.o: funcoes/abrirPacote.c headers/biblioteca.h
	$(CC) $(CFLAGS) -c funcoes/abrirPacote.c

listarAlbum.o: funcoes/listarAlbum.c headers/biblioteca.h
	$(CC) $(CFLAGS) -c funcoes/listarAlbum.c

listarMochila.o: funcoes/listarMochila.c headers/biblioteca.h
	$(CC) $(CFLAGS) -c funcoes/listarMochila.c

excluirAlbum.o: funcoes/excluirAlbum.c headers/biblioteca.h
	$(CC) $(CFLAGS) -c funcoes/excluirAlbum.c

excluirMochila.o: funcoes/excluirMochila.c headers/biblioteca.h
	$(CC) $(CFLAGS) -c funcoes/excluirMochila.c

pesquisar.o: funcoes/pesquisar.c headers/biblioteca.h
	$(CC) $(CFLAGS) -c funcoes/pesquisar.c

alterar.o: funcoes/alterar.c headers/biblioteca.h
	$(CC) $(CFLAGS) -c funcoes/alterar.c

resetarLista.o: funcoes/resetarLista.c headers/biblioteca.h
	$(CC) $(CFLAGS) -c funcoes/resetarLista.c

quiz.o: funcoes/quiz.c headers/biblioteca.h
	$(CC) $(CFLAGS) -c funcoes/quiz.c

goleiro.o: funcoes/goleiro.c headers/biblioteca.h
	$(CC) $(CFLAGS) -c funcoes/goleiro.c

penalti.o: funcoes/penalti.c headers/biblioteca.h
	$(CC) $(CFLAGS) -c funcoes/penalti.c


run: main
	./main

clean:
	rm -f *.o main
>>>>>>> 09d87b5cd5bc06cc3679a6acb1f17c3b1f916654
