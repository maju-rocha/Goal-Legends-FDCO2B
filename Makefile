CFLAGS = -Iheaders #Opção para incluir o diretório de cabeçalhos

main: main.o abrirPacote.o listarAlbum.o listarMochila.o excluirAlbum.o excluirMochila.o pesquisar.o alterar.o resetarLista.o
	@echo "Gerando executavel..."
	gcc main.o abrirPacote.o listarAlbum.o listarMochila.o excluirAlbum.o excluirMochila.o pesquisar.o alterar.o resetarLista.o -o main

main.o: funcoes/main.c headers/biblioteca.h
	gcc $(CFLAGS) -c funcoes/main.c

abrirPacote.o: funcoes/abrirPacote.c headers/biblioteca.h
	gcc $(CFLAGS) -c funcoes/abrirPacote.c

listarAlbum.o: funcoes/listarAlbum.c headers/biblioteca.h
	gcc $(CFLAGS) -c funcoes/listarAlbum.c

listarMochila.o: funcoes/listarMochila.c headers/biblioteca.h
	gcc $(CFLAGS) -c funcoes/listarMochila.c

excluirAlbum.o: funcoes/excluirAlbum.c headers/biblioteca.h
	gcc $(CFLAGS) -c funcoes/excluirAlbum.c

excluirMochila.o: funcoes/excluirMochila.c headers/biblioteca.h
	gcc $(CFLAGS) -c funcoes/excluirMochila.c

pesquisar.o: funcoes/pesquisar.c headers/biblioteca.h
	gcc $(CFLAGS) -c funcoes/pesquisar.c

alterar.o: funcoes/alterar.c headers/biblioteca.h
	gcc $(CFLAGS) -c funcoes/alterar.c

resetarLista.o: funcoes/resetarLista.c headers/biblioteca.h
	gcc $(CFLAGS) -c funcoes/resetarLista.c

clean:
	rm -f *.o main

run: main
	./main