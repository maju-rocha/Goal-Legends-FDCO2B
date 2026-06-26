#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include "biblioteca.h"
#include "global.h"
#include "albumgrafico.h"

//Paleta de cores usada na tela do album grafico
#define COPA_AZUL_ESCURO (Color){ 8, 12, 24, 255 }
#define COPA_AZUL_MEDIO  (Color){ 20, 28, 48, 255 }
#define COPA_VERDE_NEON  (Color){ 0, 230, 150, 255 }
#define COPA_VERMELHO    (Color){ 210, 20, 52, 255 }
#define COPA_OURO_PURO   (Color){ 255, 190, 0, 255 }
#define COPA_OURO_BRILHO (Color){ 255, 230, 120, 255 }
#define COPA_CINZA_CARD  (Color){ 28, 36, 56, 255 }

#define quantidadeMaximaParticulas 50 // Define o limite máximo de partículas flutuando ao mesmo tempo no fundo da tela

//Estrutura usada para criar as particulas do fundo
typedef struct{

    Vector2 posicao; // Coordenadas X e Y da partícula na tela
    Vector2 velocidade; // Direção e velocidade de movimento da partícula
    float tamanhoBase; // Tamanho geométrico inicial da partícula
    float randomizadorPosicao; // Valor usado para aplicar variação matemática (senoide) no movimento
    float transparenciaParticula; // Opacidade atual da partícula
    float velocidadetransparenciaParticula; // Ritmo com que a partícula desaparece

}ParticulaCopa;

//Funcoes para montar o caminho da imagem tentando varios nomes de pasta e codigo
static void removerEspacosInternosImagem(char *texto) {
    int j = 0;

    for (int i = 0; texto[i] != '\0'; i++) {
        if (texto[i] != ' ' && texto[i] != '\t') { // Se o caractere atual não for um espaço ou tabulação, ele é reposicionado na string
            texto[j] = texto[i];
            j++;
        }
    }

    texto[j] = '\0'; // Finaliza a string tratada
}

// Função estática para substituir caracteres de espaço em branco por underlines ('_')
static void trocarEspacoPorUnderlineImagem(char *texto) {
    for (int i = 0; texto[i] != '\0'; i++) {
        if (texto[i] == ' ') {
            texto[i] = '_';
        }
    }
}

// Função estática que copia uma string de origem para o destino convertendo letras minúsculas em maiúsculas
static void copiarMaiusculoImagem(char *destino, const char *origem) {
    int i = 0;

    while (origem[i] != '\0') {
        char c = origem[i];

        if (c >= 'a' && c <= 'z') {
            c = c - 32; //  subtrair 32 transforma uma letra minúscula em sua equivalente maiúscula
        }

        destino[i] = c;
        i++;
    }

    destino[i] = '\0'; // Finaliza a string de destino
}

// Função estática que copia uma string de origem para o destino convertendo letras maiúsculas em minúsculas
static void copiarMinusculoImagem(char *destino, const char *origem) {
    int i = 0;

    while (origem[i] != '\0') {
        char c = origem[i];

        if (c >= 'A' && c <= 'Z') {
            c = c + 32; // somar 32 transforma uma letra maiúscula em sua equivalente minúscula
        }

        destino[i] = c;
        i++;
    }

    destino[i] = '\0'; // Finaliza a string de destino
}

// Função que verifica se uma string de texto começa exatamente com um determinado prefixo
static int comecaComImagem(const char *texto, const char *prefixo) {
    int i = 0;
    while (prefixo[i] != '\0') {
        // Se houver qualquer divergência de caracteres, retorna falso (0)
        if (texto[i] != prefixo[i]) {
            return 0;
        }
        i++;
    }
    return 1; // Retorna verdadeiro (1) caso o prefixo coincida perfeitamente
}

// Função que extrai apenas a parte numérica existente no final do código de uma figurinha
static void pegarSufixoNumericoImagem(const char *codigo, char *sufixo) {
    int i = 0;
    // Avança o índice enquanto encontrar caracteres de texto (letras)
    while (codigo[i] != '\0' && (codigo[i] < '0' || codigo[i] > '9')) {
        i++;
    }
    // Copia o restante da string (que são os números) para o sufixo resultante
    strcpy(sufixo, codigo + i);
}

// Junta um prefixo textual com um sufixo numérico gerando uma nova string de código
static void montarCodigoComPrefixoImagem(char *destino, const char *prefixo, const char *sufixo) {
    strcpy(destino, prefixo);
    strcat(destino, sufixo);
}

// Adiciona um nome de pasta em uma lista de pastas a testar, evitando duplicatas e estouro de memória
static void adicionarPastaImagem(char pastas[][100], int *totalPastas, const char *pasta) {
    if (pasta[0] == '\0') {
        return; // Ignora se a string estiver vazia
    }
    // Varre a lista existente para checar se a pasta já foi inserida anteriormente
    for (int i = 0; i < *totalPastas; i++) {
        if (strcmp(pastas[i], pasta) == 0) {
            return; // Encontrou duplicada, encerra sem adicionar
        }
    }
    // Caso não seja duplicada e haja espaço na matriz (limite de 40), realiza a inserção
    if (*totalPastas < 40) {
        strcpy(pastas[*totalPastas], pasta);
        (*totalPastas)++;
    }
}

// Adiciona um código tratado em uma lista de códigos a testar, evitando duplicatas
static void adicionarCodigoImagem(char codigos[][30], int *totalCodigos, const char *codigo) {
    if (codigo[0] == '\0') {
        return;
    }
    for (int i = 0; i < *totalCodigos; i++) {
        if (strcmp(codigos[i], codigo) == 0) {
            return;
        }
    }
    if (*totalCodigos < 40) {
        strcpy(codigos[*totalCodigos], codigo);
        (*totalCodigos)++;
    }
}

// Tenta estruturar um caminho de arquivo padrão do tipo "imagens/imagens_figurinhas/PASTA/CODIGO.png"
static int tentarCaminhoImagemFigurinha(char *caminho, const char *pasta, const char *codigo) {
    strcpy(caminho, "imagens/imagens_figurinhas/");
    strcat(caminho, pasta);
    strcat(caminho, "/");
    strcat(caminho, codigo);
    strcat(caminho, ".png");

    // Utiliza uma função da Raylib para checar fisicamente se o arquivo existe no armazenamento local
    if (FileExists(caminho)) {
        return 1; // Retorna verdadeiro se o arquivo PNG existe
    }
    return 0; // Retorna falso se não foi localizado
}

// Função robusta encarregada de mapear o caminho correto de uma imagem testando variações de nomes e exceções de países
int montarCaminhoImagemFigurinha(char *caminho, char *secao, char *codigo) {
    char pastaOriginal[100];
    char pastaUnderline[100];
    char pastaMaiuscula[100];
    char codigoOriginal[30];
    char codigoSemEspaco[30];
    char pastas[40][100];
    char codigos[40][30];
    int totalPastas = 0;
    int totalCodigos = 0;

    // Faz cópias locais para poder manipular e tratar as strings com segurança
    strcpy(pastaOriginal, secao);
    strcpy(codigoOriginal, codigo);

    // Remove espaços sobressalentes nas extremidades utilizando uma função externa limpadora
    limparEspacos(pastaOriginal);
    limparEspacos(codigoOriginal);

    // Cria as variações textuais de buscas para as pastas (com underline e tudo em maiúsculo)
    strcpy(pastaUnderline, pastaOriginal);
    trocarEspacoPorUnderlineImagem(pastaUnderline);
    copiarMaiusculoImagem(pastaMaiuscula, pastaUnderline);

    // Cria a variação sem espaços internos para o código do cromo
    strcpy(codigoSemEspaco, codigoOriginal);
    removerEspacosInternosImagem(codigoSemEspaco);

    // Alimenta a matriz de testes com as opções geradas de códigos e pastas
    adicionarCodigoImagem(codigos, &totalCodigos, codigoOriginal);
    adicionarCodigoImagem(codigos, &totalCodigos, codigoSemEspaco);
    adicionarPastaImagem(pastas, &totalPastas, pastaOriginal);
    adicionarPastaImagem(pastas, &totalPastas, pastaUnderline);

    // Variáveis de controle para contornar problemas de compatibilidade e acentuação de pastas específicas
    int ehCongo = 0;
    int ehEstadosUnidos = 0;
    int ehQatar = 0;
    int ehSuica = 0;
    int ehFifa = 0;

    // Comparações de String (strcmp) para mapear exceções geográficas conhecidas do banco de dados do álbum
    if(strcmp(pastaUnderline, "Congo_DR") == 0 ){
        ehCongo = 1;
    }
    if(strcmp(pastaUnderline, "Estados_Unidos") == 0){
        ehEstadosUnidos = 1;
    }
    if( strcmp(pastaUnderline, "Catar") == 0 ){
        ehQatar = 1;
    }
    if(strcmp(pastaUnderline, "Suíça") == 0 ){
        ehSuica = 1;
    }
    if(strcmp(pastaMaiuscula, "FIFA_WORLD_CUP_2026") == 0){
        ehFifa = 1;
    }

   // Tratamento de erro: busca caminhos alternativos para carregar as fotos caso a pasta padrão falhe
    if(ehCongo){
        adicionarPastaImagem(pastas, &totalPastas, "Congo_DR");
    }
    if(ehEstadosUnidos){
        adicionarPastaImagem(pastas, &totalPastas, "EUA");
    }
    if(ehQatar){
        adicionarPastaImagem(pastas, &totalPastas, "Catar");
    }
    if(ehSuica){
        adicionarPastaImagem(pastas, &totalPastas, "Suiça");
    }
    if(ehFifa){
        adicionarPastaImagem(pastas, &totalPastas, "FIFA_World_Cup_2026");
    }

    // Estrutura de repetição aninhada (for dentro de for) que testa todas as combinações de pastas e códigos
    for (int i = 0; i < totalPastas; i++) {
        for (int j = 0; j < totalCodigos; j++) {
            // Se encontrar qualquer caminho válido ativo na máquina, encerra a busca retornando sucesso
            if (tentarCaminhoImagemFigurinha(caminho, pastas[i], codigos[j])) {
                return 1;
            }
        }
    }

    return 0; // Retorna 0 se nenhuma variação de arquivo físico PNG foi encontrada
}

// Verifica se uma determinada figurinha está colada no álbum, ignorando variações de espaços internos
int estaNoAlbumFlexivel(Figurinha *album, int total_album, char *codigo){
    char codigoBusca[30];
    char codigoAlbum[30];

    // Trata e padroniza o código que está sendo procurado
    strcpy(codigoBusca, codigo);
    limparEspacos(codigoBusca);
    removerEspacosInternosImagem(codigoBusca);

    // Percorre toda a estrutura de figurinhas salvas no álbum do usuário
    for(int i = 0; i < total_album; i++){
        // Trata e padroniza o código guardado em cada posição do álbum para comparação justa
        strcpy(codigoAlbum, album[i].codigo);
        limparEspacos(codigoAlbum);
        removerEspacosInternosImagem(codigoAlbum);

        // Se ambos os códigos tratados forem idênticos, confirma que a figurinha já está colada
        if(strcmp(codigoAlbum, codigoBusca) == 0){
            return 1;
        }//if
    }//for

    return 0; // Retorna 0 caso a figurinha não esteja registrada no álbum
}

// Renderiza a imagem de plano de fundo criando um efeito visual dinâmico (parallax) controlado pelo mouse
void DesenharFundoParallax(Texture2D textura, Vector2 posicaoMouse, float transparenciaParticula){
    if(textura.id == 0){
        return; // Evita erros se a textura não tiver sido carregada corretamente
    }//if

    // Calcula pequenas compensações de movimento inverso baseado nas coordenadas atuais do mouse
    float deslocamentoX = (posicaoMouse.x - 500.0f) * -0.02f;
    float deslocamentoY = (posicaoMouse.y - 400.0f) * -0.02f;

    // Define qual fatia da imagem original será usada (neste caso, a imagem inteira)
    Rectangle origem = { 0.0f, 0.0f, (float)textura.width, (float)textura.height };
    // Define a área de destino na janela com uma margem de segurança ligeiramente maior para o deslocamento
    Rectangle destino = { deslocamentoX - 20.0f, deslocamentoY - 20.0f, 1040.0f, 840.0f };
    Vector2 centro = { 0.0f, 0.0f };

    // Desenha a textura aplicando as proporções e aplicando um nível de transparência específico
    DrawTexturePro(textura, origem, destino, centro, 0.0f, Fade(WHITE, transparenciaParticula));
}//void

// Função principal responsável por gerenciar toda a interface visual e lógica interna do álbum gráfico
void albumGrafico(Figurinha *figurinhas, int total_figurinhas, Figurinha *album, int total_album){
    // Alocação dinâmica/estática de vetores locais para armazenar o estado visual exibido em tela
    Texture2D imagens[total_figurinhas];
    int posicoesX[total_figurinhas];
    int posicoesY[total_figurinhas];
    int temImagem[total_figurinhas];
    int paginaDaFigurinha[total_figurinhas];
    char codigosFigurinha[total_figurinhas][20];
    char tiposFigurinha[total_figurinhas][30];

    char secoes[100][50];
    int totalSecoes = 0;
    int totalMostradas = 0;
    int contadorFigurinhasColadas = 0;
    char caminho[256];

    // Laço para resetar/limpar strings e lixos de memória das structs do álbum
    for(int j = 0; j < total_album; j++){
        limparFigurinha(&album[j]);
    }//for

    // Laço para resetar/limpar strings e lixos de memória da coleção mestre de figurinhas
    for(int i = 0; i < total_figurinhas; i++){
        limparFigurinha(&figurinhas[i]);
    }//for

    // Filtra e agrupa as seções exclusivas (países/páginas) contidas na lista geral de figurinhas
    for(int i = 0; i < total_figurinhas; i++){
        int existe = 0;
        // Percorre as seções já mapeadas para evitar duplicidade de páginas
        for(int j = 0; j < totalSecoes; j++){
            if(strcmp(secoes[j], figurinhas[i].secao) == 0){
                existe = 1;
                break;
            }//if
        }//for

        // Se encontrou uma nova seção inédita, adiciona à lista para criar uma página nova
        if(!existe && totalSecoes < 100){
            strcpy(secoes[totalSecoes], figurinhas[i].secao);
            totalSecoes++;
        }//if
    }//for

    // Configurações iniciais da janela gráfica com Raylib
    SetConfigFlags(FLAG_MSAA_4X_HINT); // Ativa suavização de bordas (Anti-aliasing 4x)
    InitWindow(1000, 800, "FIFA WORLD CUP 2026 - ALBUM"); // Inicia janela 1000x800 px
    SetExitKey(KEY_NULL); // Desativa a tecla ESC para não fechar o programa abruptamente
    SetTargetFPS(60); // Crava o ciclo de atualização do jogo em 60 frames por segundo

    // Cria e inicializa um array para armazenar os três arquivos de planos de fundo temáticos
    Texture2D fundosCopa[3];

    fundosCopa[0] = LoadTexture("imagens/FIFA_2026_Verde.png");
    SetTextureFilter(fundosCopa[0], TEXTURE_FILTER_BILINEAR); // Aplica filtro de textura bilinear para evitar pixelização

    fundosCopa[1] = LoadTexture("imagens/FIFA_2026_Vermelho.png");
    SetTextureFilter(fundosCopa[1], TEXTURE_FILTER_BILINEAR);

    fundosCopa[2] = LoadTexture("imagens/FIFA_2026_Azul.png");
    SetTextureFilter(fundosCopa[2], TEXTURE_FILTER_BILINEAR);

    // Constantes de layout para organizar os cromos em um grid simétrico em tela
    int larguraFigurinha = 95;
    int alturaFigurinha = 130;
    int espacoX = 145; // Distância horizontal entre os cromos
    int espacoY = 150; // Distância vertical entre as fileiras de cromos
    int inicioX = 135; // Margem inicial à esquerda da tela
    int inicioY = 110; // Margem inicial no topo da tela

    // Laço que faz a paginação, posicionamento de grid e carregamento prévio das figurinhas coladas
    for(int pagina = 0; pagina < totalSecoes; pagina++){
        int coluna = 0;
        int linha = 0;

        for(int i = 0; i < total_figurinhas; i++){
            // Vincula e agrupa as figurinhas correspondentes à página/seção atual
            if(strcmp(figurinhas[i].secao, secoes[pagina]) == 0){
                // Calcula as coordenadas exatas cartesianas baseadas no índice de linhas e colunas atuais
                posicoesX[totalMostradas] = inicioX + coluna * espacoX;
                posicoesY[totalMostradas] = inicioY + linha * espacoY;
                paginaDaFigurinha[totalMostradas] = pagina;
                temImagem[totalMostradas] = 0; // Padrão: assumir descolada (sem imagem)

                strcpy(codigosFigurinha[totalMostradas], figurinhas[i].codigo);
                strcpy(tiposFigurinha[totalMostradas], figurinhas[i].tipo);

                // Se a figurinha constar no registro do álbum do usuário (colada)
                if(estaNoAlbum(album, total_album, figurinhas[i].codigo) || estaNoAlbumFlexivel(album, total_album, figurinhas[i].codigo)){
                    // Se o arquivo gráfico for localizado com sucesso no disco
                    if(montarCaminhoImagemFigurinha(caminho, figurinhas[i].secao, figurinhas[i].codigo)){
                        imagens[totalMostradas] = LoadTexture(caminho); // Carrega textura na GPU

                        if(imagens[totalMostradas].id != 0){
                            SetTextureFilter(imagens[totalMostradas], TEXTURE_FILTER_BILINEAR);
                            temImagem[totalMostradas] = 1; // Sinaliza que o cromo possui imagem carregada
                            contadorFigurinhasColadas++; // Incrementa o somador de progresso geral
                        }//if
                    }else{
                        // Registra uma mensagem de aviso no console de depuração caso a imagem falhe ao carregar
                        TraceLog(LOG_WARNING, "IMAGEM NAO ENCONTRADA: secao=%s codigo=%s", figurinhas[i].secao, figurinhas[i].codigo);
                    }//if
                }//if

                totalMostradas++;
                coluna++;

                // Se atingir 5 figurinhas na horizontal, quebra para a próxima linha e reseta a coluna
                if(coluna == 5){
                    coluna = 0;
                    linha++;
                }//if
            }//if
        }//for
    }//for

    // Inicialização pseudo-aleatória das propriedades de cada uma das 50 partículas decorativas de fundo
    ParticulaCopa particulas[quantidadeMaximaParticulas];

    for(int i = 0; i < quantidadeMaximaParticulas; i++){
        particulas[i].posicao = (Vector2){ (float)GetRandomValue(-20, 1020), (float)GetRandomValue(0, 800) };
        // Define velocidades levemente discrepantes fazendo as partículas subirem de forma flutuante
        particulas[i].velocidade = (Vector2){ (float)GetRandomValue(-8, 8) / 15.0f, (float)GetRandomValue(-15, -4) / 10.0f };
        particulas[i].tamanhoBase = (float)GetRandomValue(2, 4);
        particulas[i].randomizadorPosicao = (float)GetRandomValue(0, 360);
        particulas[i].transparenciaParticula = (float)GetRandomValue(20, 80) / 100.0f;
        particulas[i].velocidadetransparenciaParticula = (float)GetRandomValue(5, 15) / 1000.0f;
    }//for

    // Variáveis de controle de navegação de páginas e gatilhos da animação de transição fluida (Page Flip)
    int paginaAtual = 0;
    int proximaPagina = 0;
    float tempoGlobal = 0.0f;
    float viradaProgresso = 1.0f;          // Controla o fator de escala horizontal na interpolação
    float velocidadeVirada = 0.07f;         // Ritmo de velocidade do fechamento/abertura da página
    int estadoTransicao = 0;               // Estado: 0 = Parado, 1 = Encolhendo, 2 = Expandindo
    int direcaoTransicao = 1;              // Controla o sentido visual (1 = Avançar, -1 = Voltar)

    // Game Loop / Laço principal executado continuamente enquanto a janela gráfica estiver ativa
    while(!WindowShouldClose()){

        // Permite sair da tela do álbum e retornar ao menu console normal pressionando a tecla ESC
        if(IsKeyPressed(KEY_ESCAPE)){
            break;
        }//if

        tempoGlobal += GetFrameTime(); // Acumulador de tempo flutuante baseado nos ciclos de processamento (delta time)

        Vector2 posicaoMouse = GetMousePosition(); // Obtém instantaneamente a posição X/Y do cursor do mouse

        // Ciclo de atualização física do movimento e propriedades das partículas do plano de fundo
        for(int i = 0; i < quantidadeMaximaParticulas; i++){
            // Aplica movimento horizontal suavizado com base no seno matemático do tempo (efeito ondulação)
            particulas[i].posicao.x += particulas[i].velocidade.x + sinf(tempoGlobal + particulas[i].randomizadorPosicao) * 0.2f;
            particulas[i].posicao.y += particulas[i].velocidade.y; // Desloca a partícula para cima
            particulas[i].transparenciaParticula -= particulas[i].velocidadetransparenciaParticula; // Reduz a opacidade

            // Efeito físico interativo: repele partículas para longe caso o ponteiro do mouse chegue muito perto
            float distanciaMouse = Vector2Distance(posicaoMouse, particulas[i].posicao);
            if(distanciaMouse < 80.0f){
                particulas[i].posicao.x += (particulas[i].posicao.x - posicaoMouse.x) * 0.03f;
            }//if

            // Condição para resetar partículas mortas (invisíveis ou que saíram do topo da tela de exibição)
            if(particulas[i].transparenciaParticula <= 0.0f || particulas[i].posicao.y < 70.0f){
                particulas[i].posicao = (Vector2){ (float)GetRandomValue(0, 1000), 760.0f }; // Renasce na parte inferior
                particulas[i].transparenciaParticula = (float)GetRandomValue(60, 100) / 100.0f; // Reseta a opacidade máxima
            }//if
        }//for

        // Máquina de estados finita que calcula a animação de encolhimento e expansão horizontal dos elementos
        if(estadoTransicao == 1){
            viradaProgresso -= velocidadeVirada; // Encolhe até zero (página fecha no meio)
            if(viradaProgresso <= 0.0f){
                viradaProgresso = 0.0f;
                paginaAtual = proximaPagina;     // Altera de fato o índice da página em exibição
                estadoTransicao = 2;             // Passa para o estado de reabertura da página
            }//if
        }else if(estadoTransicao == 2){
            viradaProgresso += velocidadeVirada; // Expande de volta até recuperar o tamanho proporcional normal (1.0)
            if(viradaProgresso >= 1.0f){
                viradaProgresso = 1.0f;
                estadoTransicao = 0;             // Finaliza o estado e devolve o controle ao usuário
            }//if
        }//if

        // Captura de eventos do teclado e mouses para trocar de páginas, operando apenas se não houver transição ativa
        if(estadoTransicao == 0){
            // Avança página com Seta Direita ou D
            if(IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)){
                proximaPagina = (paginaAtual + 1) % totalSecoes; // Uso do operador resto (%) para rotacionar ao início se estourar
                direcaoTransicao = 1;
                estadoTransicao = 1;
            }//if

            // Retorna página com Seta Esquerda ou A
            if(IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)){
                proximaPagina = (paginaAtual - 1 + totalSecoes) % totalSecoes;
                direcaoTransicao = -1;
                estadoTransicao = 1;
            }//if

            // Caixas de colisão invisíveis correspondentes aos botões geométricos de setas na tela
            Rectangle botaoVoltar = { 5, 370, 50, 60 };
            Rectangle botaoAvancar = { 945, 370, 50, 60 };

            // Trata eventos de cliques físicos do botão esquerdo do mouse sobre as setas
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                if(CheckCollisionPointRec(posicaoMouse, botaoVoltar)){
                    proximaPagina = (paginaAtual - 1 + totalSecoes) % totalSecoes;
                    direcaoTransicao = -1;
                    estadoTransicao = 1;
                }//if

                if(CheckCollisionPointRec(posicaoMouse, botaoAvancar)){
                    proximaPagina = (paginaAtual + 1) % totalSecoes;
                    direcaoTransicao = 1;
                    estadoTransicao = 1;
                }//if
            }//if
        }//if

        // ETAPA DE RENDERIZAÇÃO: Início do desenho dos frames gráficos na tela
        BeginDrawing();
        ClearBackground(COPA_AZUL_ESCURO); // Limpa e preenche o fundo com a cor sólida padrão

        int indiceFundo = paginaAtual % 3; // Rotaciona ciclicamente entre os 3 planos de fundo disponíveis

        // Desenha o wallpaper correspondente da Copa com deslocamento Parallax baseado no mouse
        DesenharFundoParallax(fundosCopa[indiceFundo], posicaoMouse, 1.0f);

        // Laço de desenho de círculos brilhantes simulando as partículas em suspensão
        for(int i = 0; i < quantidadeMaximaParticulas; i++){
            // Modula levemente o raio da partícula usando a função matemática seno para criar efeito de pulsação orgânica
            float tamanhoPulso = particulas[i].tamanhoBase * (1.0f + sinf(tempoGlobal * 3.0f + particulas[i].randomizadorPosicao) * 0.2f);
            DrawCircleV(particulas[i].posicao, tamanhoPulso, Fade(COPA_OURO_PURO, particulas[i].transparenciaParticula * 0.5f));
        }//for

        // Interpolação matemática suave (Smoothstep simplificado) aplicada ao fator de animação
        float s = viradaProgresso;
        float escalaHorizontal = (s * s * (3.0f - 2.0f * s));

        // Laço que varre todas as figurinhas indexadas e renderiza apenas aquelas que pertencem à página atual
        for(int i = 0; i < totalMostradas; i++){
            if(paginaDaFigurinha[i] == paginaAtual){

                float originalX = (float)posicoesX[i];
                float x = originalX;
                float y = (float)posicoesY[i];

                // Ajusta matematicamente o ponto central X de renderização para simular o efeito tridimensional de folheada
                if(estadoTransicao != 0){
                    if(direcaoTransicao == 1){
                        x = 500.0f - (500.0f - originalX) * escalaHorizontal;
                    }else{
                        x = 500.0f + (originalX - 500.0f) * escalaHorizontal;
                    }//if
                }//if

                // Cria o retângulo delimitador da figurinha, redimensionando a largura com base no progresso da transição
                Rectangle retanguloFigurinha = { x, y, (float)larguraFigurinha * escalaHorizontal, (float)alturaFigurinha };
                // Checa se o mouse está sobre a figurinha (apenas se não estiver acontecendo transição de página)
                bool mouseEmCima = (estadoTransicao == 0) && CheckCollisionPointRec(posicaoMouse, retanguloFigurinha);

                // Efeito visual de foco: Se o mouse estiver por cima, expande ligeiramente o tamanho do cromo (Zoom / Elevação)
                if(mouseEmCima){
                    retanguloFigurinha.y -= 6.0f;
                    retanguloFigurinha.x -= 3.0f;
                    retanguloFigurinha.width += 6.0f;
                    retanguloFigurinha.height += 6.0f;
                }//if

                // Avalia se o tipo textual da figurinha contém a propriedade de cromo "Especial" (Brilhante)
                bool ehEspecial = (strstr(tiposFigurinha[i], "Especial") != NULL);

                // CASO 1: A figurinha está colada (tem imagem)
                if(temImagem[i]){
                    Rectangle origemImagem = { 0.0f, 0.0f, (float)imagens[i].width, (float)imagens[i].height };
                    // Projeta a textura do jogador exatamente dentro dos limites geométricos calculados para o cromo
                    DrawTexturePro(imagens[i], origemImagem, retanguloFigurinha, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);

                    // Se for do tipo especial, cria um efeito visual personalizado de contorno dourado oscilante
                    if(ehEspecial){
                        float pulso = (sinf(tempoGlobal * 5.0f) + 1.0f) / 2.0f; // Oscila de 0 a 1 em ritmo acelerado
                        Color corBorda = Fade(COPA_OURO_PURO, 0.5f + (pulso * 0.5f));

                        // Renderiza duas molduras concêntricas para compor a estética brilhante da borda dourada
                        DrawRectangleLinesEx((Rectangle){retanguloFigurinha.x - 1.0f, retanguloFigurinha.y - 1.0f, retanguloFigurinha.width + 2.0f, retanguloFigurinha.height + 2.0f}, 2.0f, corBorda);
                        DrawRectangleLinesEx((Rectangle){retanguloFigurinha.x - 3.0f, retanguloFigurinha.y - 3.0f, retanguloFigurinha.width + 6.0f, retanguloFigurinha.height + 6.0f}, 1.0f, Fade(COPA_OURO_BRILHO, pulso * 0.3f));
                    }else{
                        // Caso seja comum, aplica uma sutil linha de borda preta ou verde (se o mouse estiver sob o foco)
                        Color corLinha = Fade(BLACK, 0.5f);
                        if(mouseEmCima){
                            corLinha = COPA_VERDE_NEON;
                        }//if
                        DrawRectangleLinesEx(retanguloFigurinha, 1.5f, corLinha);
                    }//if

                    // Adiciona um feixe luminoso linear oblíquo que corre a face do cromo ao passar o mouse (efeito Foil)
                    if(mouseEmCima){
                        float reflexoX = retanguloFigurinha.x + ((float)sinf(tempoGlobal * 2.5f) + 1.0f) * 0.5f * retanguloFigurinha.width;
                        DrawLineEx((Vector2){reflexoX, retanguloFigurinha.y}, (Vector2){reflexoX + 15.0f, retanguloFigurinha.y + retanguloFigurinha.height}, 3.0f, Fade(WHITE, 0.25f));
                    }//if

                }else{
                    // CASO 2: A figurinha não está colada (vazia)
                    // Preenche a silhueta da vaga vazia com uma cor cinza escura opaca
                    DrawRectangleRec(retanguloFigurinha, COPA_CINZA_CARD);

                    Color corLinha = Fade(WHITE, 0.08f);
                    if(mouseEmCima){
                        corLinha = COPA_VERDE_NEON; // Muda cor da borda para neon indicando seleção
                    }//if
                    DrawRectangleLinesEx(retanguloFigurinha, 1.5f, corLinha);

                    if(mouseEmCima){
                        // Cria um sombreamento interno azul translúcido se passar o cursor sobre o card vazio
                        DrawRectangleRec(retanguloFigurinha, Fade(COPA_AZUL_MEDIO, 0.4f));
                    }//if

                    // Garante que o código de identificação do cromo só apareça se houver largura legível suficiente na animação
                    if(escalaHorizontal > 0.25f){
                        // Desenha linhas cruzadas em marca d'água no fundo do card
                        DrawLineEx((Vector2){retanguloFigurinha.x + 4.0f, retanguloFigurinha.y + 4.0f}, (Vector2){retanguloFigurinha.x + retanguloFigurinha.width - 4.0f, retanguloFigurinha.y + retanguloFigurinha.height - 4.0f}, 1.0f, Fade(WHITE, 0.02f));

                        // Calcula as dimensões em pixels do texto do código para fazer a centralização matemática perfeita
                        int larguraCodigo = MeasureText(codigosFigurinha[i], 16);
                        float posicaoCodigoX = retanguloFigurinha.x + (retanguloFigurinha.width - (larguraCodigo * escalaHorizontal)) / 2.0f;
                        Color corCodigo = Fade(WHITE, 0.35f);

                        if(mouseEmCima){
                            corCodigo = COPA_VERDE_NEON;
                        }//if

                        // Desenha primeiro o texto deslocado em preto para criar uma sombra projetada (Drop Shadow), melhorando o contraste
                        DrawText(codigosFigurinha[i], (int)posicaoCodigoX + 1, (int)(retanguloFigurinha.y + (retanguloFigurinha.height / 2.0f) - 7.0f), 16, Fade(BLACK, 0.6f));
                        // Desenha o código original por cima
                        DrawText(codigosFigurinha[i], (int)posicaoCodigoX, (int)(retanguloFigurinha.y + (retanguloFigurinha.height / 2.0f) - 8.0f), 16, corCodigo);
                    }//if
                }//if
            }//if
        }//for

        // INTERFACE DO MENU SUPERIOR (HUD / Cabeçalho)
        DrawRectangle(0, 0, 1000, 85, Fade(COPA_AZUL_MEDIO, 0.85f)); // Retângulo de fundo do cabeçalho
        DrawRectangle(0, 83, 1000, 2, COPA_OURO_PURO); // Listra horizontal dourada divisória

        // Renderiza o título textual da Seção/País da página atual com efeito de sombra escura por baixo
        DrawText(secoes[paginaAtual], 32, 27, 32, Fade(BLACK, 0.4f));
        DrawText(secoes[paginaAtual], 30, 25, 32, WHITE);

        // Seção matemática de cálculo do percentual global de preenchimento do álbum
        float porcentagemConclusao = 0.0f;
        if(total_figurinhas > 0){
            porcentagemConclusao = ((float)contadorFigurinhasColadas / total_figurinhas) * 100.0f;
        }//if

        // Desenha fisicamente os componentes da barra de carregamento do progresso geral (Barra de fundo e indicador verde)
        DrawRectangle(650, 47, 320, 10, Fade(BLACK, 0.5f));
        DrawRectangle(651, 48, (int)(318 * (porcentagemConclusao / 100.0f)), 8, COPA_VERDE_NEON);
        // Exibe o texto de progresso formatado dinamicamente com variáveis (ex: "75.5% (150/200)")
        DrawText(TextFormat("ALBUM PROGRESSO: %.1f%% (%d/%d)", porcentagemConclusao, contadorFigurinhasColadas, total_figurinhas), 650, 24, 13, Fade(WHITE, 0.7f));

        // Texto informativo central indicando a numeração absoluta da página aberta
        DrawText(TextFormat("PAGINA %02d / %02d", paginaAtual + 1, totalSecoes), 455, 58, 15, COPA_OURO_PURO);

        // Áreas lógicas duplicadas para o efeito estético Hover (Mouse posicionado em cima) dos botões laterais de setas
        Rectangle botaoVoltarReal = { 5, 370, 50, 60 };
        Rectangle botaoAvancarReal = { 945, 370, 50, 60 };

        bool emCimaVoltar = CheckCollisionPointRec(posicaoMouse, botaoVoltarReal) && (estadoTransicao == 0);
        bool emCimaAvancar = CheckCollisionPointRec(posicaoMouse, botaoAvancarReal) && (estadoTransicao == 0);

        // Configuração visual padrão da seta esquerda
        float tamanhoBotaoVoltar = 22.0f;
        Color corBotaoVoltar = Fade(WHITE, 0.1f);
        Color corTextoVoltar = WHITE;

        // Se o mouse focar no botão Voltar, altera o tamanho do raio e pinta com verde neon
        if(emCimaVoltar){
            tamanhoBotaoVoltar = 26.0f;
            corBotaoVoltar = COPA_VERDE_NEON;
            corTextoVoltar = COPA_VERDE_NEON;
        }//if

        // Desenha os círculos concêntricos e o símbolo "<" gerando o botão gráfico de retroceder página
        DrawCircleV((Vector2){30, 400}, tamanhoBotaoVoltar, corBotaoVoltar);
        DrawCircleV((Vector2){30, 400}, 20.0f, COPA_AZUL_MEDIO);
        DrawText("<", 24, 387, 24, corTextoVoltar);

        // Configuração visual padrão da seta direita
        float tamanhoBotaoAvancar = 22.0f;
        Color corBotaoAvancar = Fade(WHITE, 0.1f);
        Color corTextoAvancar = WHITE;

        // Se o mouse focar no botão Avançar, expande o raio e pinta com verde neon
        if(emCimaAvancar){
            tamanhoBotaoAvancar = 26.0f;
            corBotaoAvancar = COPA_VERDE_NEON;
            corTextoAvancar = COPA_VERDE_NEON;
        }//if

        // Desenha os círculos concêntricos e o símbolo ">" gerando o botão gráfico de avançar página
        DrawCircleV((Vector2){970, 400}, tamanhoBotaoAvancar, corBotaoAvancar);
        DrawCircleV((Vector2){970, 400}, 20.0f, COPA_AZUL_MEDIO);
        DrawText(">", 964, 387, 24, corTextoAvancar);

        // INTERFACE DO RODAPÉ INFORMATIVO
        DrawRectangle(0, 765, 1000, 35, COPA_AZUL_MEDIO);
        DrawLine(0, 765, 1000, 765, Fade(WHITE, 0.08f)); // Sutil linha divisória cinza claro

        // Mede a largura das instruções textuais para que fiquem centralizadas na base da tela de exibição
        int larguraInstrucao = MeasureText("Seta Direita / Esquerda ou Cliques para mudar de pagina", 14);
        DrawText("Seta Direita / Esquerda ou Cliques para mudar de pagina", (1000 - larguraInstrucao) / 2, 775, 14, Fade(WHITE, 0.5f));

        EndDrawing(); // Finaliza a construção do frame atual e atualiza a janela de exibição na tela
    }//while

    // ETAPA DE DESALOCAÇÃO E LIMPEZA DA MEMÓRIA RAM / VRAM (Evita estouro de memória no computador)
    // Libera a memória de vídeo de todas as texturas de jogadores que foram carregadas durante o álbum
    for(int i = 0; i < totalMostradas; i++){
        if(temImagem[i]){
            UnloadTexture(imagens[i]);
        }//if
    }//for

    // Libera a memória de vídeo associada às 3 texturas dos planos de fundo da Copa
    for(int i = 0; i < 3; i++){
        UnloadTexture(fundosCopa[i]);
    }//for

    CloseWindow(); // Finaliza o contexto do driver gráfico e destrói o ciclo da janela da Raylib
}//void