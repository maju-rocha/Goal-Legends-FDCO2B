# ==========================================
# 1. DETECÇÃO AUTOMÁTICA DE SISTEMA
# ==========================================
ifeq ($(OS),Windows_NT)
    # Configurações para Windows (w64devkit buscando no sistema ou no projeto)
    TARGET = main.exe
    LIBS = -L./extras/libwin -LC:/w64devkit/w64devkit/x86_64-w64-mingw32/lib -LC:/w64devkit/w64devkit/lib -Lraylib/src -Lraylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm
    RM = rm -f
else
    # Configurações para Linux
    TARGET = main
    LIBS = -lraylib -lX11 -lGL -lm -lpthread -ldl -lrt
    RM = rm -f
endif

# ==========================================
# 2. CONFIGURAÇÕES DE COMPILAÇÃO
# ==========================================
CC = gcc
CFLAGS = -Iheaders -Iraylib/src

# Lista de todos os arquivos objeto (.o) necessários
OBJS = main.o abrirPacote.o listarAlbum.o listarMochila.o excluirAlbum.o excluirMochila.o pesquisar.o alterar.o resetarLista.o quiz.o goleiro.o penalti.o

# ==========================================
# 3. REGRAS DE COMPILAÇÃO
# ==========================================

# A regra 'all' é a padrão que roda ao digitar apenas 'make'
all: $(TARGET)

# Linkagem final: Junta todos os .o e as bibliotecas para gerar o executável
$(TARGET): $(OBJS)
	@echo "---------------------------------------------"
	@echo "Gerando executavel final: $(TARGET)"
	@echo "---------------------------------------------"
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

# Regra genérica para compilar qualquer arquivo .c que esteja na pasta 'funcoes'
%.o: funcoes/%.c
	@echo "Compilando: $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Regra específica para o main.o caso ele esteja na raiz ou na pasta funcoes
main.o: funcoes/main.c
	$(CC) $(CFLAGS) -c funcoes/main.c -o main.o

# ==========================================
# 4. LIMPEZA ARQUIVOS TEMPORÁRIOS
# ==========================================
clean:
	@echo "Limpando arquivos temporarios..."
	$(RM) *.o $(TARGET)

run:
	./main