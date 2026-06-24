#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <raylib.h>

// Certifique-se de que a estrutura Figurinha está aqui em cima
typedef struct {
    char codigo[10];
    char titulo[50];
    char secao[50];
    char grupo[50];
    char tipo[50];
} Figurinha;

// 1. O ENUM PRECISA CONTER O ESTADO_ABRIR_PACOTE
typedef enum { 
    MENU_PRINCIPAL, 
    MENU_INVENTARIO, 
    MENU_EXCLUIR, 
    MENU_MINIGAMES,
    ESTADO_ABRIR_PACOTE  // <--- Adicione esta linha aqui
} EstadoMenu;

typedef enum { MENU_FIM } Menu_FimdeJogo;

// Variáveis globais vindas da global.c
extern int pacotes_fechados;
extern int figurinha_repetida;
extern Menu_FimdeJogo estadoAtual;

// 2. ADICIONE O PROTÓTIPO DA NOVA FUNÇÃO GRÁFICA AQUI
void abrirPacoteGrafico(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album, Font fonteCopa, Color azulBrasil, Color amareloBrasil, EstadoMenu *estadoAtual);

// Outros protótipos do seu projeto (mantenha os que já existirem abaixo)
void listarFigurinhasAlbum(Figurinha *album, int total_album);
void albumGrafico(Figurinha *figurinhas, int total, Figurinha *album, int total_album);
void listarFigurinhasMochila(Figurinha *mochila, int total_mochila);
void excluirAlbum(Figurinha *figurinhas, Figurinha *album, int *total_album);
void excluirMochila(Figurinha *figurinhas, Figurinha *mochila, int *total_mochila);
void pesquisarFigurinha(Figurinha *figurinhas, int total);
void alterarFigurinha(Figurinha *figurinhas, int total);
void resetarLista(Figurinha *figurinhas, int total);
void trocarFigurinha(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int *total_mochila, int *total_album);
void jogarQuiz(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album);
void jogarGoleiro();
void jogarPenalti(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album, int *pacotes_fechados);
// adicione estas linhas dentro do seu biblioteca.h se já não estiverem lá:


// ... (outras definições e structs que você já tem)

// ==========================================================
// PROTÓTIPOS DAS FUNÇÕES PRINCIPAIS DO SISTEMA
// ==========================================================

// Certifique-se de adicionar estas duas linhas:

void carregarPacotes(void); 
void menuPrincipal(Figurinha *figurinhas, Figurinha *album, Figurinha *mochila, int total, int *total_album, int *total_mochila);

// Outros protótipos que você já possui (mantenha-os aqui)
void abrirPacoteGrafico(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album, Font fonteCopa, Color azulBrasil, Color amareloBrasil, EstadoMenu *estadoAtual);
void listarFigurinhasAlbum(Figurinha *album, int total_album);
void albumGrafico(Figurinha *figurinhas, int total, Figurinha *album, int total_album);
// Adicione esta linha dentro do seu biblioteca.h:
void salvarRepetida(void);
void salvarPacotes(void);
#endif // BIBLIOTECA_H
