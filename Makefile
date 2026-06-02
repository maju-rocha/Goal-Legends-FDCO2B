main: main.o abrirPacote.o listarAlbum.o listarMochila.o excluirAlbum.o excluirMochila.o pesquisar.o
	@echo "Gerando executavel..." 
	gcc main.o abrirPacote.o listarAlbum.o listarMochila.o excluirAlbum.o excluirMochila.o pesquisar.o -o main

main.o: main.c biblioteca.h 
	gcc -c main.c

abrirPacote.o: abrirPacote.c biblioteca.h 
	gcc -c abrirPacote.c

listarAlbum.o: listarAlbum.c biblioteca.h 
	gcc -c listarAlbum.c

listarMochila.o: listarMochila.c biblioteca.h 
	gcc -c listarMochila.c

excluirAlbum.o: excluirAlbum.c biblioteca.h 
	gcc -c excluirAlbum.c

excluirMochila.o: excluirMochila.c biblioteca.h
	gcc -c excluirMochila.c

pesquisar.o: pesquisar.c biblioteca.h
	gcc -c pesquisar.c

clean: 
	rm -f *.o main
	 
run: main 
	./main