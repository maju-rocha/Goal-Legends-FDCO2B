#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

typedef enum {
    MENU_PRINCIPAL,
    MENU_INVENTARIO,
    MENU_EXCLUIR,
    MENU_MINIGAMES
} EstadoMenu;

typedef struct {

    char codigo[10];
    char titulo[50];
    char secao[50];
    char grupo[50];
    char tipo[50];

} Figurinha;//struct Figurinha

//Enum Fim de Jogo
typedef enum{
    MENU_FIM, 
    JOGAR_NOVAMENTE
}Menu_FimdeJogo;

extern int qntPacotes;
extern Menu_FimdeJogo estadoAtual;

void abrirPacote(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album);//cabeçalho da função abrirPacote

void listarFigurinhasAlbum(Figurinha *album, int total_album);//cabeçalho da função listarFigurinhas para o album

void listarFigurinhasMochila(Figurinha *mochila, int total_mochila);//cabeçalho da função listarFigurinhas para a mochila

void excluirAlbum(Figurinha *figurinhas, Figurinha *album, int *total_album);//cabeçalho da função excluir para o album

void excluirMochila(Figurinha *figurinhas, Figurinha *mochila, int *total_mochila);//cabeçalho da função excluir para a mochila

void pesquisarFigurinha(Figurinha *vetor, int total);//cabeçalho da função pesquisar figurinha

void alterarFigurinha(Figurinha *vetor, int total);//cabeçalho da função alterar figurinha

void resetarLista(Figurinha *vetor, int total);//cabeçalho da função resetar lista de figurinhas

void jogarQuiz(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album); //cabeçalho da função do quiz

void jogarGoleiro(); //cabeçalho da função do jogo do goleiro

void jogarPenalti(); //cabeçalho da função do jogo do pênalti

void menuPrincipal(void);//cabeçalho da função do menu

#endif