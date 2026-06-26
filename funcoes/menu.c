#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include "biblioteca.h"
#include "global.h"
#include "menu.h"
#include "abrirPacote.h"
#include "listarAlbum.h"
#include "listarMochila.h"
#include "albumgrafico.h"
#include "excluirAlbum.h"
#include "excluirMochila.h"
#include "pesquisar.h"
#include "alterar.h"
#include "resetarLista.h"
#include "trocar.h"
#include "quiz.h"
#include "goleiro.h"
#include "penalti.h"

void menuPrincipal(Figurinha *figurinhas, Figurinha *album, Figurinha *mochila, int total, int *total_album, int *total_mochila){// Função principal do menu do jogo
    
    //==========================================//
    //============ Interface Gráfica ===========//
    //==========================================//

    InitWindow(1000, 800, "Album de Figurinhas - Copa do Mundo");

    
    if (!IsAudioDeviceReady()){
        InitAudioDevice();
    }

    //Váriaveis de audio
    Music somMenu = LoadMusicStream("audio/som_menu.mp3");
    SetMusicVolume(somMenu, 0.08f);

    Font fonteCopa = LoadFont("extras/PressStart2P-Regular.ttf");
    
    //Configuração do Cursor
    HideCursor();
    Image imagemBola = LoadImage("imagens/bola_cursor.png");
    ImageResize(&imagemBola, 40, 40);
    Texture2D cursorBola = LoadTextureFromImage(imagemBola);
    UnloadImage(imagemBola);

    SetTargetFPS(60);

    //EstadoAtual
    EstadoMenu telaAtual = MENU_PRINCIPAL;

    //Textos do Menu Principal
    const char *textosPrincipal[] = {
        "1. Abrir Pacote",
        "2. Ver Inventario",
        "3. Excluir do Inventario",
        "4. Pesquisar Figurinha",
        "5. Alterar Figurinha",
        "6. Trocar Figurinhas",
        "7. Area de Minigames"
    };

    Rectangle botoesPrincipal[7];// Define os retângulos para os botões do menu principal

    for(int i = 0; i < 7; i++){// Define a posição e tamanho de cada botão
        botoesPrincipal[i] = (Rectangle){250, 220 + (i * 70), 500, 50};
    }

    //Textos dos Submenus
    const char *textosInventario[] = {"1. Ver Album", "2. Ver Mochila", "3. Voltar ao Menu"};
    const char *textosExcluir[] = {"1. Excluir Album", "2. Excluir Mochila", "3. Voltar ao Menu"};
    const char *textosMinigames[] = {"1. Quiz da Copa", "2. Jogo do Goleiro", "3. Penalti", "4. Voltar ao Menu"};

    Rectangle botoesSubmenu[4];// Define os retângulos para os botões dos submenus

    for(int i = 0; i < 4; i++){// Define a posição e tamanho de cada botão dos submenus
        botoesSubmenu[i] = (Rectangle){250, 300 + (i * 80), 500, 55};
    }

    Color verdeCampo = (Color){34, 139, 34, 255};
    Color amareloBrasil = (Color){255, 215, 0, 255};
    Color azulBrasil = (Color){25, 35, 66, 255};
    Color COPA_AZUL_MEDIO = (Color){14, 20, 38, 255};
    Color COPA_VERDE_NEON = (Color){0, 255, 140, 255};
    Color COPA_OURO_PURO = (Color){255, 205, 66, 255};
    Color bordabotao = (Color){128, 128, 128, 255};

    while(!WindowShouldClose()){// Loop principal do menu, continua até que a janela seja fechada
        
        //Mantém a música tocando em todas as telas de menu
        if (!IsMusicStreamPlaying(somMenu)) {
            PlayMusicStream(somMenu); 
        }

        UpdateMusicStream(somMenu); // Atualiza o stream de música para manter a reprodução contínua

        Vector2 mousePoint = GetMousePosition(); // Obtém a posição atual do mouse

        int acaoTerminal = 0; // Variável para armazenar a ação selecionada no terminal, inicializada como 0 (nenhuma ação)

        //====================================================//
        //================== Cliques/Tela ====================//
        //====================================================//

        if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){ // Verifica se o botão esquerdo do mouse foi liberado
            if(telaAtual == MENU_PRINCIPAL){ // Se estiver no menu principal, verifica os cliques nos botões principais
                for(int i = 0; i < 7; i++){ // Itera sobre os 7 botões do menu principal
                    if(CheckCollisionPointRec(mousePoint, botoesPrincipal[i])){
                        if(i == 0){
                            telaAtual = ESTADO_ABRIR_PACOTE; //Ativa a tela gráfica nova
                        }else if(i == 1){
                            telaAtual = MENU_INVENTARIO;
                        }else if(i == 2){
                            telaAtual = MENU_EXCLUIR;
                        }else if(i == 3){
                            acaoTerminal = 6;  //Pesquisar
                        }else if(i == 4){
                            acaoTerminal = 7;  //Alterar
                        }else if(i == 5){
                            acaoTerminal = 11; //Trocar
                        }else if(i == 6){
                            telaAtual = MENU_MINIGAMES;
                        }
                    }
                }
            }else if(telaAtual == MENU_INVENTARIO){ // Se estiver no menu de inventário, verifica os cliques nos botões do submenu
                for(int i = 0; i < 3; i++){
                    if(CheckCollisionPointRec(mousePoint, botoesSubmenu[i])){ // Verifica se o mouse está sobre algum dos botões do submenu
                        if(i == 0){
                            acaoTerminal = 2; //Ver Album
                        }else if(i == 1){
                            acaoTerminal = 3; //Ver Mochila
                        }else if(i == 2){
                            telaAtual = MENU_PRINCIPAL;
                        }
                    }
                }
            }else if(telaAtual == MENU_EXCLUIR){
                for(int i = 0; i < 3; i++){
                    if(CheckCollisionPointRec(mousePoint, botoesSubmenu[i])){
                        if(i == 0){
                            acaoTerminal = 4; //Excluir Album
                        }else if(i == 1){
                            acaoTerminal = 5; //Excluir Mochila
                        }else if(i == 2){
                            telaAtual = MENU_PRINCIPAL;
                        }
                    }
                }
            }else if(telaAtual == MENU_MINIGAMES){
                for(int i = 0; i < 4; i++){
                    if(CheckCollisionPointRec(mousePoint, botoesSubmenu[i])){
                        if(i == 0){
                            acaoTerminal = 8;  //Quiz
                        }else if(i == 1){
                            acaoTerminal = 9;  //Goleiro
                        }else if(i == 2){
                            acaoTerminal = 10; //Penalti
                        }else if(i == 3){
                            telaAtual = MENU_PRINCIPAL;
                        }
                    }
                }
            }
        }

        //==========================================//
        //================= Desenho ================//
        //==========================================//
        BeginDrawing();
        ClearBackground(COPA_AZUL_MEDIO);

        //Se estiver no estado_abrir_pacote chama a funçao
        if(telaAtual == ESTADO_ABRIR_PACOTE){
            abrirPacote(figurinhas, mochila, album, total, total_mochila, total_album, fonteCopa, azulBrasil, amareloBrasil, &telaAtual);
        }else if(telaAtual == MENU_PRINCIPAL){ // Se estiver no menu principal, desenha os elementos do menu principal
            const char *titulo = "ALBUM DE FIGURINHAS DA COPA";
            int tituloTamanho = MeasureTextEx(fonteCopa, titulo, 28, 2).x;

            DrawTextEx(fonteCopa, titulo, (Vector2){500 - (tituloTamanho / 2) + 3, 60 + 3}, 28, 2, BLACK);
            DrawTextEx(fonteCopa, titulo, (Vector2){500 - (tituloTamanho / 2), 60}, 28, 2, WHITE);

            const char *subtitulo = "Selecione uma opcao abaixo:";
            int subtituloTamanho = MeasureTextEx(fonteCopa, subtitulo, 16, 2).x;

            DrawTextEx(fonteCopa, subtitulo, (Vector2){500 - (subtituloTamanho / 2) + 2, 120 + 2}, 16, 2, BLACK);
            DrawTextEx(fonteCopa, subtitulo, (Vector2){500 - (subtituloTamanho / 2), 120}, 16, 2, COPA_VERDE_NEON);

            for(int i = 0; i < 7; i++){ // Itera sobre os 7 botões do menu principal para desenhá-los
                bool mouseEmCima = CheckCollisionPointRec(mousePoint, botoesPrincipal[i]);
                int offsetAnimacao = 0;

                if(mouseEmCima){ // Se o mouse estiver sobre o botão, aplica um efeito de animação e muda a cor do botão
                    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
                        offsetAnimacao = 2;
                    }else{ // Se o mouse estiver sobre o botão, mas não estiver clicando, aplica um efeito de animação diferente
                        offsetAnimacao = -6;
                        DrawRectangle(botoesPrincipal[i].x + 4, botoesPrincipal[i].y + 4, botoesPrincipal[i].width, botoesPrincipal[i].height, Fade(BLACK, 0.4f));
                    }
                }

                // Define o retângulo animado do botão com base na posição original e no deslocamento da animação
                Rectangle btnAnimado = {botoesPrincipal[i].x,botoesPrincipal[i].y + offsetAnimacao,botoesPrincipal[i].width,botoesPrincipal[i].height};

                Color corBotao;
                Color corBorda;

                if(mouseEmCima){// Se o mouse estiver sobre o botão, muda a cor do botão e da borda para indicar que está selecionado
                    corBotao = LIGHTGRAY;
                    corBorda = azulBrasil;
                }else{// Se o mouse não estiver sobre o botão, mantém a cor padrão do botão e da borda
                    corBotao = azulBrasil;
                    corBorda = bordabotao;
                }

                DrawRectangleRec(btnAnimado, corBotao);
                DrawRectangleLinesEx(btnAnimado, 2.0f, corBorda);
                
                char textoBotao[100];

                if(i == 0){// Se for o primeiro botão (Abrir Pacote), exibe a quantidade de pacotes fechados disponíveis
                    sprintf(textoBotao, "1. Abrir Pacote (%d disp.)", pacotes_fechados);
                }else{// Para os outros botões, exibe o texto padrão do menu principal
                    strcpy(textoBotao, textosPrincipal[i]);
                }

                int textoTamanho = MeasureTextEx(fonteCopa, textoBotao, 14, 2).x;

                DrawTextEx(fonteCopa, textoBotao, (Vector2){btnAnimado.x + (btnAnimado.width / 2) - (textoTamanho / 2), btnAnimado.y + 18}, 14, 2, WHITE);
            }
        }else{
            //Submenus (Inventário, Exclusão, Minigames)
            const char *titulo;
            const char *subtitulo;
            const char **textosBotoes;
            int numBotoes = 3;

            if(telaAtual == MENU_INVENTARIO){// Se estiver no menu de inventário, define os textos e títulos correspondentes
                titulo = "MEU INVENTARIO";
                subtitulo = "Qual colecao voce quer visualizar?";
                textosBotoes = textosInventario;
            }else if(telaAtual == MENU_EXCLUIR){// Se estiver no menu de exclusão, define os textos e títulos correspondentes
                titulo = "EXCLUIR FIGURINHAS";
                subtitulo = "De onde voce quer remover figurinhas?";
                textosBotoes = textosExcluir;
            }else{// Se estiver no menu de minigames, define os textos e títulos correspondentes
                titulo = "AREA DE MINIGAMES";
                subtitulo = "Escolha seu desafio:";
                textosBotoes = textosMinigames;
                numBotoes = 4;
            }

            int tituloTamanho = MeasureTextEx(fonteCopa, titulo, 28, 2).x;// Calcula a largura do título para centralizá-lo na tela

            DrawTextEx(fonteCopa, titulo, (Vector2){500 - (tituloTamanho / 2) + 3, 100 + 3}, 28, 2, BLACK);
            DrawTextEx(fonteCopa, titulo, (Vector2){500 - (tituloTamanho / 2), 100}, 28, 2, WHITE);
            
            int subtituloTamanho = MeasureTextEx(fonteCopa, subtitulo, 16, 2).x;// Calcula a largura do subtítulo para centralizá-lo na tela

            DrawTextEx(fonteCopa, subtitulo, (Vector2){500 - (subtituloTamanho / 2) + 2, 180 + 2}, 16, 2, BLACK);
            DrawTextEx(fonteCopa, subtitulo, (Vector2){500 - (subtituloTamanho / 2), 180}, 16, 2, COPA_VERDE_NEON);

            for(int i = 0; i < numBotoes; i++){// Itera sobre os botões do submenu para desenhá-los
                bool mouseEmCima = CheckCollisionPointRec(mousePoint, botoesSubmenu[i]);
                int offsetAnimacao = 0;

                if(mouseEmCima){// Se o mouse estiver sobre o botão, aplica um efeito de animação e muda a cor do botão
                    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
                        offsetAnimacao = 2;
                    }else{
                        offsetAnimacao = -6;
                    }
                }else{
                    offsetAnimacao = 0;
                }

                if(mouseEmCima && !IsMouseButtonDown(MOUSE_LEFT_BUTTON)){// Se o mouse estiver sobre o botão, mas não estiver clicando, desenha uma sombra para indicar que está selecionado
                    DrawRectangle(botoesSubmenu[i].x + 4, botoesSubmenu[i].y + 4, botoesSubmenu[i].width, botoesSubmenu[i].height, Fade(BLACK, 0.4f));
                }

                Rectangle btnAnimado = {botoesSubmenu[i].x,botoesSubmenu[i].y + offsetAnimacao,botoesSubmenu[i].width,botoesSubmenu[i].height};

                Color corBotao;
                Color corBorda;

                if(mouseEmCima){
                    corBotao = LIGHTGRAY;
                    corBorda = azulBrasil;
                }else{
                    corBotao = azulBrasil;
                    corBorda = DARKGRAY;
                }

                DrawRectangleRec(btnAnimado, corBotao);
                DrawRectangleLinesEx(btnAnimado, 2, corBorda);
                
                int textoTamanho = MeasureTextEx(fonteCopa, textosBotoes[i], 16, 2).x;

                DrawTextEx(fonteCopa, textosBotoes[i], (Vector2){btnAnimado.x + (btnAnimado.width / 2) - (textoTamanho / 2), btnAnimado.y + 20}, 16, 2, WHITE);
            }
        }

        //Texto Fechamento
        const char *base = "Feche a janela (X) ou aperte ESC para sair";
        int baseTamanho = MeasureTextEx(fonteCopa, base, 12, 2).x;

        DrawTextEx(fonteCopa, base, (Vector2){500 - (baseTamanho / 2) + 2, 740 + 2}, 12, 2, BLACK);
        DrawTextEx(fonteCopa, base, (Vector2){500 - (baseTamanho / 2), 740}, 12, 2, LIGHTGRAY);

        //Cursor
        DrawTexture(cursorBola, (int)mousePoint.x - cursorBola.width / 2, (int)mousePoint.y - cursorBola.height / 2, WHITE);

        EndDrawing();

        //==========================================//
        //============= Reaparecer Menu ============//
        //==========================================//
        if(acaoTerminal != 0){// Se uma ação foi selecionada no terminal, processa a ação correspondente
            //Desliga a música
            
            if(IsMusicStreamPlaying(somMenu)){
                StopMusicStream(somMenu);
            }

            UnloadTexture(cursorBola);
            UnloadFont(fonteCopa);
            CloseWindow();

            bool voltarDiretoMenuGrafico = false; // Variável para controlar se deve voltar diretamente para o menu gráfico após a ação do terminal
            
            if(acaoTerminal == 2){ //Ver Album
                voltarDiretoMenuGrafico = true;
                printf("\n=========================================\nTotal album: %d\n", *total_album);
                listarFigurinhasAlbum(album, *total_album);
                albumGrafico(figurinhas, total, album, *total_album);
            }else if(acaoTerminal == 3){ //Ver Mochila
                printf("\n=========================================\nTotal mochila: %d\n", *total_mochila);
                listarFigurinhasMochila(mochila, *total_mochila);

            }else if(acaoTerminal == 4){ //Excluir Album
                excluirAlbum(figurinhas, album, total_album);
                
            }else if(acaoTerminal == 5){ //Excluir Mochila
                excluirMochila(figurinhas, mochila, total_mochila);
                
            }else if(acaoTerminal == 6){ //Pesquisar
                pesquisarFigurinha(figurinhas, total);

            }else if(acaoTerminal == 7){ //Alterar
                int opcao_alterar;

                do{ // Loop para o menu de alteração, continua até que o usuário escolha voltar ao menu principal
                    printf("\n--- MENU DE ALTERACAO ---\n1 - Alterar figurinha do album\n2 - Resetar a lista de figurinhas\n3 - Voltar ao menu principal\nEscolha: ");

                    if(scanf("%d", &opcao_alterar) != 1){ // Se a entrada não for um número válido, limpa o buffer de entrada e solicita novamente
                        opcao_alterar = 0;
                    }

                    while(getchar() != '\n');

                    if(opcao_alterar == 1){ // Se o usuário escolher alterar uma figurinha, chama a função correspondente
                        alterarFigurinha(figurinhas, total);
                    }else if(opcao_alterar == 2){ // Se o usuário escolher resetar a lista de figurinhas, chama a função correspondente
                        resetarLista(figurinhas, total);
                    }

                }while(opcao_alterar != 3);

            }else if(acaoTerminal == 11){ //Trocar
                trocarFigurinha(figurinhas, mochila, album, total_mochila, total_album);
            }else if(acaoTerminal == 8){ //Quiz
                voltarDiretoMenuGrafico = true;
                jogarQuiz(figurinhas, mochila, album, total, total_mochila, total_album);
            }else if(acaoTerminal == 9){ //Goleiro
                voltarDiretoMenuGrafico = true;
                jogarGoleiro();
            }else if(acaoTerminal == 10){ //Penalti
                voltarDiretoMenuGrafico = true;
                jogarPenalti(figurinhas, mochila, album, total, total_mochila, total_album, &pacotes_fechados);
            }

            //Menu de travamento pós terminal
            if(!voltarDiretoMenuGrafico){
                printf("\n=========================================\nPressione ENTER para voltar ao menu grafico...");

            int c; // Limpa o buffer de entrada para evitar que caracteres indesejados interfiram na leitura do ENTER

                while((c = getchar()) != '\n' && c != EOF);
                getchar();
            }//if

            telaAtual = MENU_PRINCIPAL;
            
            //Reinicializa os componentes gráficos da tela
            InitWindow(1000, 800, "Album de Figurinhas - Copa do Mundo");
            fonteCopa = LoadFont("extras/PressStart2P-Regular.ttf");
            HideCursor();

            Image imgBolaTemp = LoadImage("imagens/bola_cursor.png");
            ImageResize(&imgBolaTemp, 40, 40);
            cursorBola = LoadTextureFromImage(imgBolaTemp);
            UnloadImage(imgBolaTemp);

            SetTargetFPS(60);
        }
    }

    //Libera Memória
    UnloadMusicStream(somMenu);
    CloseAudioDevice();
    
    UnloadTexture(cursorBola);
    UnloadFont(fonteCopa);
    CloseWindow();

    free(figurinhas);
    free(album);
    free(mochila);
}