void alterarFigurinha(Figurinha *vetor, int total) {
    char codigoBusca[10];
    int encontrada = 0;

    printf("\n--- Alterar Figurinha ---\n");
    printf("Digite o codigo da figurinha que deseja alterar: ");
    scanf(" %9[^\n]", codigoBusca);

    for (int i = 0; i < total; i++) {
        if (strcmp(vetor[i].codigo, codigoBusca) == 0) {
            printf("\nFigurinha Encontrada: %s - %s\n", vetor[i].codigo, vetor[i].titulo);
            
            // Aqui você pede os novos dados. Você pode escolher quais campos podem ser alterados.
            printf("Digite o novo Titulo: ");
            scanf(" %49[^\n]", vetor[i].titulo);
            
            printf("Digite a nova Secao: ");
            scanf(" %49[^\n]", vetor[i].secao);

            printf("Digite o novo Tipo: ");
            scanf(" %49[^\n]", vetor[i].tipo);
            
            printf("\n Figurinha alterada com sucesso!\n");
            encontrada = 1;
            break;
        }
    }

    if (!encontrada) {
        printf("\n Figurinha nao encontrada para alteracao.\n");
    }
}