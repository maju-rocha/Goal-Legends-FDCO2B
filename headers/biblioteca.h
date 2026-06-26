#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

//Struct Figurinha
typedef struct{
    char codigo[10];
    char titulo[50];
    char secao[50];
    char grupo[50];
    char tipo[50];
}Figurinha;

//Enum estados
typedef enum{
    MENU_PRINCIPAL,
    MENU_INVENTARIO,
    MENU_EXCLUIR,
    MENU_MINIGAMES,
    ESTADO_ABRIR_PACOTE
}EstadoMenu;

//Enumerador estados fim de jogo
typedef enum{
    MENU_FIM
}Menu_FimdeJogo;



#endif