#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <time.h> 
#include "biblioteca.h" 

int main(){

    char linha[300];//variavel com todos os bytes de uma linha
    int opcao_inicial;//variavel para armazenar a resposta do usuário sobre a opção desejada
    int total = 0;//variavel para contabilizar a quantidade
    int total_album = 0;//variavel para contabilizar a quantidade de figurinhas no album
    int total_mochila = 0;//variavel para contabilizar a quantidade de figurinhas na mochila
    
    Figurinha *figurinhas = malloc(981 * sizeof(Figurinha));//Cria vetor dinamico para as figurinhas 

    Figurinha *album = malloc(981 * sizeof(Figurinha));//Cria vetor dinamico para o album

    Figurinha *mochila = malloc(981 * sizeof(Figurinha));//Cria vetor dinamico para a mochila
    
    if (figurinhas == NULL){//testa se o malloc funcionou 
        printf("Erro de alocacao.\n"); 
        return 1;
    }//if

    FILE *arquivo = fopen("figurinhas2026.csv","rb");//cria e abre o arquivo para leitura de figurinhas
        
    if (arquivo == NULL){//testa se o arquivo abriu 
        printf("Erro ao abrir o arquivo.\n"); 
        return 1; 
    }//if

    FILE *arquivo_album = fopen("album.csv", "r");

    if(arquivo_album != NULL){

        while(fscanf(arquivo_album," %9[^,],%49[^,],%49[^,],%49[^,],%49[^\n]", album[total_album].codigo, album[total_album].titulo, album[total_album].secao, album[total_album].grupo,album[total_album].tipo) == 5){
            //le cada linha do arquivo album.csv, armazena os campos correspondentes em cada posição do vetor album e incrementa o total de figurinhas lidas, o loop continua até ler todas as figurinhas ou atingir o limite de 981 figurinhas

            total_album++;//incrementa o total de figurinhas do album
        }

        fclose(arquivo_album);//fecha arquivo album.csv

    }

    FILE *arquivo_mochila = fopen("mochila.csv", "r");

    if(arquivo_mochila != NULL){

        while(fscanf(arquivo_mochila, " %9[^,],%49[^,],%49[^,],%49[^,],%49[^\n]", mochila[total_mochila].codigo, mochila[total_mochila].titulo, mochila[total_mochila].secao, mochila[total_mochila].grupo, mochila[total_mochila].tipo) == 5){
        //le cada linha do arquivo mochila.csv, armazena os campos correspondentes em cada posição do vetor mochila e incrementa o total de figurinhas lidas, o loop continua até ler todas as figurinhas ou atingir o limite de 981 figurinhas
        
            total_mochila++;//incrementa o total de figurinhas da mochila
        }
        
        fclose(arquivo_mochila);//fecha arquivo mochila.csv
    
    }//if teste de abertura do arquivo mochila.csv



    fgets(linha, sizeof(linha), arquivo);//ignora a primeira linha do arquivo figurinhas2026.csv
    
    while (total < 981 && fscanf(arquivo, " %9[^,],%49[^,],%49[^,],%49[^,],%49[^\n]", figurinhas[total].codigo, figurinhas[total].titulo, figurinhas[total].secao, figurinhas[total].grupo, figurinhas[total].tipo) == 5){
    
        total++;
    }//le cada linha do arquivo figurinhas2026.csv, armazena os campos correspondentes em cada posição do vetor figurinhas e incrementa o total de figurinhas lidas, o loop continua até ler todas as figurinhas ou atingir o limite de 981 figurinhas
    
    fclose(arquivo);//fecha arquivo
    
    srand(time(NULL));//libera aleatoriedade para funcao abrirPacote 

    do{
        printf("\nSelecione a opcao desejada:\n");
        printf("1 - Abrir pacote\n");
        printf("2 - Ver album\n");
        printf("3 - Ver mochila\n");
        printf("4 - Excluir figurinha do album\n");
        printf("5 - Excluir figurinha da mochila\n");
        printf("6 - Pesquisar figurinha\n");
        printf("7 - Alterar figurinha\n");
        printf("8 - Sair do programa\n");


        if(scanf("%d", &opcao_inicial) != 1){

            opcao_inicial = 0;

        }//if

        while(getchar() != '\n');

        if(opcao_inicial == 1){
            abrirPacote(figurinhas, mochila, album, total, &total_mochila, &total_album);

        }else if(opcao_inicial == 2){
            printf("Total album: %d\n", total_album);
            listarFigurinhasAlbum(album, total_album);

        }else if(opcao_inicial == 3){
            printf("Total mochila: %d\n", total_mochila);
            listarFigurinhasMochila(mochila, total_mochila);

        }else if(opcao_inicial == 4){
            excluirAlbum(figurinhas, album, &total_album);

        }else if(opcao_inicial == 5){
            excluirMochila(figurinhas, mochila, &total_mochila);

        }else if(opcao_inicial == 6){
            pesquisarFigurinha(figurinhas, total);
        
        }else if(opcao_inicial == 7){
            alterarFigurinha(figurinhas, total);
        }

        else if(opcao_inicial != 8){
            printf("Opcao invalida, por favor selecione uma opcao valida.\n");
        }

    }while(opcao_inicial != 8);//loop para manter o programa rodando até o usuário escolher sair, cada opção chama a função correspondente, caso a opção seja inválida informa o usuário e volta a solicitar uma opção válida

    
    printf("Saindo do programa...\n");

    free(figurinhas);//Libera a memória do vetor figurinhas
    free(album);//Libera a memória do vetor album
    free(mochila);//Libera a memória do vetor mochila

    return 0; 
}//main