#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/biblioteca.h"

void abrirPacote(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album, int *pacotes_fechados){

    // 1. Verifica se o jogador tem pacotes para abrir
    if (*pacotes_fechados <= 0) {
        printf("OPSSS! Voce nao tem pacotes fechados no inventario!\n");
        printf("Jogue os Minigames da Copa para conquistar recompensas.\n");
        return;
    }

    int qtd_abrir = 0;
    char buffer[100];

    // 2. Pergunta a quantidade
    printf("Voce tem %d pacote(s) disponivel(is).\n", *pacotes_fechados);
    printf("Quantos pacotes deseja abrir de uma vez? ");
    
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &qtd_abrir); // Converte o texto digitado para número

    // 3. Validações de segurança
    if (qtd_abrir <= 0) {
        printf("\nQuantidade invalida. Operacao cancelada.\n");
        return;
    }

    if (qtd_abrir > *pacotes_fechados) {
        printf("\nVoce nao tem pacotes suficientes! Tentou abrir %d, mas so tem %d.\n", qtd_abrir, *pacotes_fechados);
        return;
    }

    // 4. Desconta os pacotes do saldo e calcula as figurinhas
    *pacotes_fechados -= qtd_abrir;
    salvarPacotes();
    int total_figurinhas = qtd_abrir * 7;

    FILE *arquivoalbum = fopen("extras/album.csv", "a");
    if(arquivoalbum == NULL){
        printf("\nErro ao abrir album.csv\n");
        return;
    }

    FILE *arquivomochila = fopen("extras/mochila.csv", "a");
    if(arquivomochila == NULL){
        printf("\nErro ao abrir mochila.csv\n");
        fclose(arquivoalbum);
        return;
    }

    printf("\n>>> Abrindo %d pacote(s)... Voce tirou %d figurinhas!\n\n", qtd_abrir, total_figurinhas);

    char opcao[10];
    
    // 5. Loop para sortear todas as figurinhas escolhidas
    for(int i = 0; i < total_figurinhas; i++){
        int sorteada = rand() % total;

        printf("--- Figurinha %d de %d ---\n", i + 1, total_figurinhas);
        printf("Codigo: %s\n", figurinhas[sorteada].codigo);
        printf("Titulo: %s\n", figurinhas[sorteada].titulo);
        printf("Secao : %s\n", figurinhas[sorteada].secao);
        printf("Grupo : %s\n", figurinhas[sorteada].grupo);
        printf("Tipo  : %s\n\n", figurinhas[sorteada].tipo);

        printf("Guardar no album? (sim/nao): ");
        fgets(opcao, sizeof(opcao), stdin);
        opcao[strcspn(opcao, "\n")] = '\0';

        if(strcmp(opcao, "sim") == 0){
            album[*total_album] = figurinhas[sorteada];
            (*total_album)++;
            fprintf(arquivoalbum,"%s,%s,%s,%s,%s\n",figurinhas[sorteada].codigo, figurinhas[sorteada].titulo, figurinhas[sorteada].secao, figurinhas[sorteada].grupo, figurinhas[sorteada].tipo);
            printf("=> Figurinha salva no album.\n\n");
        }else{
            printf("Guardar na mochila? (sim/nao): ");
            fgets(opcao, sizeof(opcao), stdin);
            opcao[strcspn(opcao, "\n")] = '\0';

            if(strcmp(opcao, "sim") == 0){
                mochila[*total_mochila] = figurinhas[sorteada];
                (*total_mochila)++;
                fprintf(arquivomochila,"%s,%s,%s,%s,%s\n", figurinhas[sorteada].codigo, figurinhas[sorteada].titulo, figurinhas[sorteada].secao, figurinhas[sorteada].grupo, figurinhas[sorteada].tipo);
                printf("=> Figurinha salva na mochila.\n\n");
            }else{
                printf("=> Figurinha descartada.\n\n");
            }
        }
    }

    fclose(arquivoalbum);
    fclose(arquivomochila);
}