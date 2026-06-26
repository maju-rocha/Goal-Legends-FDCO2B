#==========================================
# DETECÇÃO AUTOMÁTICA DE SISTEMA
#==========================================

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

#==========================================
# CONFIGURAÇÕES DE COMPILAÇÃO
#==========================================

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

#==========================================
# REGRAS DE COMPILAÇÃO
#==========================================

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

# Compila o main.c da raiz
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

#==========================================
# LIMPEZA
#==========================================

clean:
	@echo "Limpando arquivos temporarios..."
	$(RM) *.o $(TARGET)

run: $(TARGET)
	./$(TARGET)