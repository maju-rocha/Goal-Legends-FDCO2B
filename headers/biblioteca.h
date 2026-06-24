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

extern int pacotes_fechados;
extern int figurinha_repetida; 
extern Menu_FimdeJogo estadoAtual;

void abrirPacote(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album, int *pacotes_fechados);

void listarFigurinhasAlbum(Figurinha *album, int total_album);//cabeçalho da função listarFigurinhas para o album

void listarFigurinhasMochila(Figurinha *mochila, int total_mochila);//cabeçalho da função listarFigurinhas para a mochila

void excluirAlbum(Figurinha *figurinhas, Figurinha *album, int *total_album);//cabeçalho da função excluir para o album

void excluirMochila(Figurinha *figurinhas, Figurinha *mochila, int *total_mochila);//cabeçalho da função excluir para a mochila

void pesquisarFigurinha(Figurinha *vetor, int total);//cabeçalho da função pesquisar figurinha

void alterarFigurinha(Figurinha *vetor, int total);//cabeçalho da função alterar figurinha

void resetarLista(Figurinha *vetor, int total);//cabeçalho da função resetar lista de figurinhas

void jogarQuiz(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album);//cabeçalho do quiz

void jogarGoleiro(); //cabeçalho do goleiro

void jogarPenalti(Figurinha *figurinhas,Figurinha *mochila,Figurinha *album,int total,int *total_mochila,int *total_album,int *qtd_pacotes); //cabeçalho da função do jogo do pênalti

void menuPrincipal(Figurinha *figurinhas, Figurinha *album, Figurinha *mochila, int total, int *total_album, int *total_mochila);//cabeçalho da função do menu

void carregarPacotes();//cabeçalho da função que carrega a quantidade de pacotes

void salvarPacotes();//cabeçalho da função que salva a quantidade de pacotes

void salvarRepetida();//cabeçalho da função que salva a quantidade de repetidas

void trocarFigurinha(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int *total_mochila, int *total_album);//cabeçalho da função que troca figurinhas repetidas

void albumGrafico(Figurinha *figurinhas, int total_figurinhas, Figurinha *album, int total_album);//cabeçalho da função gráfico do album

int estaNoAlbum(Figurinha *album, int total_album, char codigo[]);//cabeçalho da função que consefere se a figurinha está no album

#endif