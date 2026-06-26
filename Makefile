<<<<<<< HEAD
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
=======
# ==========================================
# 1. DETECÇÃO AUTOMÁTICA DE SISTEMA
# ==========================================
ifeq ($(OS),Windows_NT)
    TARGET = main.exe
    LIBS = -L./extras/libwin -LC:/w64devkit/w64devkit/x86_64-w64-mingw32/lib \
           -LC:/w64devkit/w64devkit/lib -Lraylib/src -Lraylib/lib \
           -lraylib -lopengl32 -lgdi32 -lwinmm
    RM = rm -f
else
    TARGET = main
    LIBS = -lraylib -lX11 -lGL -lm -lpthread -ldl -lrt
    RM = rm -f
endif

# ==========================================
# 2. CONFIGURAÇÕES DE COMPILAÇÃO
# ==========================================
>>>>>>> 16c582dd713c6b6c580db3e690781347b7bb83d1
CC = gcc
CFLAGS = -Iheaders -Iraylib/src

OBJS = main.o \
       menu.o \
       abrirPacote.o \
       listarAlbum.o \
       listarMochila.o \
       excluirAlbum.o \
       excluirMochila.o \
       pesquisar.o \
       alterar.o \
       resetarLista.o \
       quiz.o \
       goleiro.o \
       penalti.o \
	   global.o \
       salvarPacotes.o \
       carregarPacotes.o \
       salvarRepetida.o \
       trocar.o \
       albumgrafico.o \
       salvarAlbum.o \
       salvarMochila.o \
       carregarAlbum.o \
       carregarMochila.o \
       carregarRepetida.o \
       inserir.o 


# ==========================================
# 3. REGRAS DE COMPILAÇÃO
# ==========================================
all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "---------------------------------------------"
	@echo "Gerando executavel final: $(TARGET)"
	@echo "---------------------------------------------"
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

# Compila arquivos da pasta funcoes
%.o: funcoes/%.c
	@echo "Compilando: $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Compila o novo main.c (o da raiz)
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

# ==========================================
# 4. LIMPEZA
# ==========================================
clean:
<<<<<<< HEAD
	rm -f *.o main
>>>>>>> 09d87b5cd5bc06cc3679a6acb1f17c3b1f916654
=======
	@echo "Limpando arquivos temporarios..."
	$(RM) *.o $(TARGET)

run: $(TARGET)
	./$(TARGET)
>>>>>>> 16c582dd713c6b6c580db3e690781347b7bb83d1
