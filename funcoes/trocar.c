#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca.h"
#include "trocar.h"
#include "global.h"
#include "salvarPacotes.h"
#include "salvarRepetida.h"

void trocarFigurinha(Figurinha *mochila, int *total_mochila){

    FILE *arquivo_mochila = fopen("extras/mochila.bin", "rb");//Le o arquivo mochila.bin

    if(arquivo_mochila == NULL){
        printf("Erro ao abrir o arquivo extras/mochila.bin\n");
        return;
    }//if teste

    int totalArquivo = 0;

    //Le a quantidade de figurinhas repetidas salvas na mochila
    if(fread(&totalArquivo, sizeof(int), 1, arquivo_mochila) != 1){
        printf("Erro ao ler a quantidade de figurinhas da mochila.\n");
        fclose(arquivo_mochila);
        return;
    }//if

    fclose(arquivo_mochila);//Fecha o arquivo

    //Atualiza o total da mochila na memoria
    *total_mochila = totalArquivo;
    figurinha_repetida = totalArquivo;

    if(*total_mochila >= 14){

        int pacotesPossiveis = *total_mochila / 14;
        int pacotesDesejados = 0;
        int c;

        printf("\nVoce tem %d figurinhas repetidas na mochila.\n", *total_mochila);
        printf("Cada pacote custa 14 figurinhas repetidas.\n");
        printf("Voce pode trocar por ate %d pacote(s).\n", pacotesPossiveis);
        printf("Quantos pacotes deseja trocar? ");

        if(scanf("%d", &pacotesDesejados) != 1){
            pacotesDesejados = 0;
        }//scanf para quantidade

        while((c = getchar()) != '\n' && c != EOF);//Exclui o texto digitado anteriormente e le todos os caracteres até o final da string

        if(pacotesDesejados <= 0){
            printf("Troca cancelada.\n");
            return;
        }//if

        if(pacotesDesejados > pacotesPossiveis){
            printf("Voce nao tem figurinhas suficientes para trocar essa quantidade.\n");
            printf("Quantidade maxima possivel: %d pacote(s).\n", pacotesPossiveis);
            return;
        }//if

        int figurinhasParaRemover = pacotesDesejados * 14;//Remoção de figurinhas
        int novoTotalMochila = *total_mochila - figurinhasParaRemover;//Atualização da mochila

        //Aumenta a quantidade de pacotes fechados
        pacotes_fechados += pacotesDesejados;
        salvarPacotes();

        
        arquivo_mochila = fopen("extras/mochila.bin", "rb");//Le o arquivo mochila.bin

        if(arquivo_mochila == NULL){
            printf("Erro ao abrir o arquivo extras/mochila.bin\n");
            return;
        }//if teste

        FILE *temp = fopen("extras/temp_mochila.bin", "wb");//Abre arquivo temp em modo escrita binário

        if(temp == NULL){
            printf("Erro ao criar o arquivo temporario\n");
            fclose(arquivo_mochila);
            return;
        }//if teste

        int totalAntigo = 0;//Variável para comparação

        fread(&totalAntigo, sizeof(int), 1, arquivo_mochila);//Le o total antigo do arquivo original

        fwrite(&novoTotalMochila, sizeof(int), 1, temp);//Salva o novo total no arquivo temporario

        //Variáveis para atualizar quantidade
        Figurinha figurinhaAtual;
        int contador = 0;
        int indiceNovo = 0;

        //Le cada figurinha da mochila antiga
        while(contador < totalAntigo && fread(&figurinhaAtual, sizeof(Figurinha), 1, arquivo_mochila) == 1){

            contador++;

            //Ignora as primeiras figurinhas usadas na troca
            if(contador <= figurinhasParaRemover){
                continue;
            }//if

            //Salva no arquivo temporario as figurinhas que sobraram
            fwrite(&figurinhaAtual, sizeof(Figurinha), 1, temp);

            //Atualiza tambem o vetor mochila na memoria
            mochila[indiceNovo] = figurinhaAtual;
            indiceNovo++;

        }//while

        //Fecha arquivos
        fclose(arquivo_mochila);
        fclose(temp);

        remove("extras/mochila.bin");//Exclui a mochila antiga
        rename("extras/temp_mochila.bin", "extras/mochila.bin");//Renomea a nova para o nome anterior, "atualizando"

      
        *total_mochila = novoTotalMochila;//Atualiza o total da mochila na memoria

        //Atualiza a quantidade de repetidas
        figurinha_repetida = *total_mochila;
        salvarRepetida();

        //Texto terminal
        printf("\nTroca realizada com sucesso!\n");
        printf("Voce recebeu %d pacote(s) novo(s).\n", pacotesDesejados);
        printf("Foram removidas %d figurinhas repetidas da mochila.\n", figurinhasParaRemover);
        printf("Figurinhas restantes na mochila: %d\n", *total_mochila);

    }else{
        
        //Texto terminal
        printf("Voce nao tem figurinhas suficientes para trocar.\n");
        printf("Voce tem apenas %d figurinhas na mochila.\n", *total_mochila);
        printf("Sao necessarias 14 figurinhas repetidas para trocar por 1 pacote.\n");

    }//if

}//void