#include <string.h>
#include "../headers/biblioteca.h"

// Função para comparar se uma figurinha específica já está no álbum
int estaNoAlbum(Figurinha *album, int total_album, char codigo[]) {
    for(int i = 0; i < total_album; i++) {
        // Compara o código da figurinha no álbum com o código sorteado
        if(strcmp(album[i].codigo, codigo) == 0) {
            return 1; // Encontrou: Retorna verdadeiro (1)
        }
    }
    return 0; // Não encontrou: Retorna falso (0)
}