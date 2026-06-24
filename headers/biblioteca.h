#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <raylib.h>

// Estrutura das Figurinhas
typedef struct {
    char codigo[10];
    char titulo[50];
    char secao[50];
    char grupo[50];
    char tipo[50];
} Figurinha;

// Enumeradores de Estado
typedef enum { 
    MENU_PRINCIPAL, 
    MENU_INVENTARIO, 
    MENU_EXCLUIR, 
    MENU_MINIGAMES,
    ESTADO_ABRIR_PACOTE
} EstadoMenu;

typedef enum { MENU_FIM } Menu_FimdeJogo;

//Função para limpar espacos antes e depois do texto
void limparEspacos(char *texto);

//Função para limpar todos os campos da figurinha
void limparFigurinha(Figurinha *f);

//Função para trocar espacos por underline
void trocarEspacoPorUnderline(char *texto);

//Variáveis Globais
extern int pacotes_fechados;
extern int figurinha_repetida;
extern Menu_FimdeJogo estadoAtual;

//Menu e necessários
void menuPrincipal(Figurinha *figurinhas, Figurinha *album, Figurinha *mochila, int total, int *total_album, int *total_mochila);
void abrirPacote(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album, Font fonteCopa, Color azulBrasil, Color amareloBrasil, EstadoMenu *estadoAtual);
int estaNoAlbum(Figurinha *album, int total_album, char codigo[]);

//Funções de Listagem e Visuais
void listarFigurinhasAlbum(Figurinha *album, int total_album);
void listarFigurinhasMochila(Figurinha *mochila, int total_mochila);
void albumGrafico(Figurinha *figurinhas, int total, Figurinha *album, int total_album);

//Funções excluir/pesquisar/alterar/resetar/trocar
void excluirAlbum(Figurinha *figurinhas, Figurinha *album, int *total_album);
void excluirMochila(Figurinha *figurinhas, Figurinha *mochila, int *total_mochila);
void pesquisarFigurinha(Figurinha *figurinhas, int total);
void alterarFigurinha(Figurinha *figurinhas, int total);
void resetarLista(Figurinha *figurinhas, int total);
void trocarFigurinha(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int *total_mochila, int *total_album);

//Minigames
void jogarQuiz(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album);
void jogarGoleiro();
void jogarPenalti(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album, int *pacotes_fechados);

//Salvamento de Arquivos
void carregarPacotes(void); 
void salvarPacotes(void);
void salvarRepetida(void);

#endif // BIBLIOTECA_H