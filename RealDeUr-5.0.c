#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#if defined(_WIN32)

#include <windows.h>
#include <conio.h>

#define usleep(t) Sleep(t)

#elif defined(unix) || defined(__unix__)

#include <unistd.h>
#include <curses.h>

#else
#define usleep(t) printf(# t);
#endif


#include "RealBot.h"


#define NUM_MENSAGEM 100

#define MIN_CONSOLE_WIDTH 120
#define MIN_CONSOLE_HEIGHT 37

char tab[TAB_ALT][TAB_LAR] = {
            {R, V, V, V, E, S, R, V},
            {V, V, V, R, V, V, V, V},
            {R, V, V, V, E, S, R, V}
        };

int pecas[TAB_ALT][TAB_LAR] = {
        {  0,  0,  0,  0, 7,  0,  0,  0},
        {  0,  0,  0,  0,  0,  0,  0,  0},
        {  0,  0,  0,  0,-7,  0,  0,  0}
    };

int pecasS[2] = {0,0};
char jogador;
bool isBot[2] = {false,false};

#if defined(_WIN32)
HANDLE hConsole;
#endif
short consoleWidth = -1;
short consoleHeight = -1;

typedef enum GameState {MENU_PRINCIPAL, INSTRUCOES, JOGO, SAIR, MENU_JOGADORES} GameState;
GameState gameState;
typedef enum Key {NONE, ENTER, UP_ARROW, DOWN_ARROW, LEFT_ARROW, RIGHT_ARROW, ESCAPE} Key;

char* mensagens[NUM_MENSAGEM];
char mensagemPos = 0;

void addMensagem(const char* mensagem, ...);

void setColor(unsigned char color);
void setCursorPos(short int x, short int y);
Key getKeyPressed();
short getConsoleWidth();
void updateConsole();
void clearConsole();

void iniciarPecas(){
    int i, j;
    for(i=0;i<TAB_ALT;i++){
        for(j=0;j<TAB_LAR;j++){
            pecas[i][j] = 0;
        }
    }
    pecas[0][4] = 7;
    pecas[2][4] = -7;
}

void desenha(int ci, int cj){
    int i, j;
    setColor(0x0f);
    printf("\n    ");
    for(j=0;j<TAB_LAR;j++){
        printf("  %d  ", j);
    }
    printf("\n\n");
    for(i = 0;i<TAB_ALT;i++){
        setColor(0x07);
        printf(" %d  ", i);
        for(j = 0;j<TAB_LAR;j++){
            const char c = tab[i][j];
            //if(c!=S && c!=E && c!=R) printf("["); 
            //else if(c!=R) printf(" ");
            //else printf("{");

            if(c == R) setColor(0xce); //vermelho-amarelo
            else if(c == E) setColor(0x0b);
            else if(c == S) setColor(0x0a);
            else if(c == V){
                if((i+j)%2==0)
                    setColor(0x70);
                else
                    setColor(0x80);
                
            }
            if(!(i == ci && j == cj)){
                if(pecas[i][j] == 0){
                    printf("  %c  ",tab[i][j]);
                }
                else{
                    if(pecas[i][j]!=0) printf(" %c%2d ",pecas[i][j]>0?'A':'B',abs(pecas[i][j]));
                    else               printf("   ");
                }
            }
            else{
                if(pecas[i][j] == 0){
                    printf("[ %c ]",tab[i][j]);
                }
                else{
                    if(pecas[i][j]!=0) printf("[%c%2d]",pecas[i][j]>0?'A':'B',abs(pecas[i][j]));
                    else               printf("[ ]");
                }
            }

            //if(c!=S && c!=E && c!=R) printf("]");
            //else if(c!=R) printf(" ");
            //else printf("}");
        }
        printf("\n");
    }
    setColor(0x0f);
}

bool moverCasa(int ci, int cj, int quant){
    if(tab[ci][cj] == S){
        addMensagem("A casa(%d, %d) nunca pode ser movida.", ci, cj);
        return false;
    }
    int c = pecas[ci][cj];
    if(c==0){
        if(ci==(jogador=='A'?0:2) && cj==4){
            //printf("Nao existem pecas para ser inseridas.\n");
            addMensagem("Nao existem pecas para ser inseridas.");
        }
        else{
            //printf("Nao existem pecas na casa (%d,%d).\n", ci ,cj);
            addMensagem("Nao existem pecas na casa (%d,%d).", ci ,cj);
        }
        return false;
    }
    
    switch (jogador)
    {
        case 'A':
            if(c<0){
                //printf("Essas pecas sao do Jogador B.\n");
                addMensagem("Essas pecas sao do Jogador B.");
                return false;
            }
            break;

        case 'B':
            if(c>0){
                //printf("Essas pecas sao do Jogador A.\n");
                addMensagem("Essas pecas sao do Jogador A.");
                return false;
            }
            break;
    }

    int pi, pj;
    if(ci != 1){
        pi = ci;
        pj = cj - quant;
        if(pj < 0){
            pi = 1;
            pj = -pj - 1;
        }
    }
    else{
        pi = ci;
        pj = cj + quant;
        if(pj >= 8){
            pi = jogador == 'A'? 0 : 2;
            pj = 7 - (pj - 8);
        }
    }
    int p = pecas[pi][pj];

    if(cj>5 && pj<5){
        //printf("jogador %a nao pode avancar %d casas a partir daqui.\n", jogador, quant);
        addMensagem("jogador %c nao pode avancar %d casas a partir daqui.", jogador, quant);
        return false;
        //pecas[4][jogador == 'A'? 0 : 2] += pecas[ci][cj];
    }
    if(pj == 5 && pi != 1){
        //printf("%d pecas do Jogador %c saem do tabuleiro.\n", abs(c),jogador);
        addMensagem("%d pecas do Jogador %c saem do tabuleiro.", abs(c),jogador);
        int j = jogador=='A'?0:1;
        pecasS[j] += abs(c);
        pecas[j*2][5] += c;
        pecas[ci][cj] = 0;
    }

    if(p!=0 && (c>0)!=(p>0)){
        //printf("c=%d p=%d",c,p);
        if(tab[pi][pj] == R){
            //printf("A peca do Jogador %c esta protegida pela casa com Roseta.\n", (jogador=='A')?'B':'A');
            addMensagem("A peca do Jogador %c esta protegida pela casa com Roseta.", (jogador=='A')?'B':'A');
            return false;
        }
        //printf("A peca do Jogador %c ataca as pecas do Jogador %c!\n", jogador, (jogador=='A')?'B':'A');
        addMensagem("A peca do Jogador %c ataca as pecas do Jogador %c!", jogador, (jogador=='A')?'B':'A');
        if(jogador == 'A'){
            pecas[2][4] += pecas[pi][pj];
        } else{
            pecas[0][4] += pecas[pi][pj];
        }
        
        if(tab[ci][cj]!=E){ //ci != 0 && cj != 4){
            pecas[pi][pj] = pecas[ci][cj];
            pecas[ci][cj] = 0;
        }
        else{
            pecas[pi][pj]  = jogador=='A'? 1 : -1;
            pecas[ci][cj] += jogador=='A'? -1 : 1;;
        }
    }
    else{
        if(tab[ci][cj]!=E){ //ci != 0 && cj != 4){
            pecas[pi][pj] += pecas[ci][cj];
            pecas[ci][cj] = 0;
        }
        else{
            pecas[pi][pj]+= jogador=='A'? 1 : -1;
            pecas[ci][cj]+=jogador=='A'? -1 : 1;
        }

        if(tab[pi][pj]==R){
            //printf("Jogador %c move casa(%d,%d) para casa(%d,%d) e atinge uma Roseta, joga novamente.\n", jogador,ci,cj,pi,pj);
            addMensagem("Jogador %c move casa(%d,%d) para casa(%d,%d) e atinge uma Roseta.", jogador,ci,cj,pi,pj);
            addMensagem("Jogador %c joga novamente.",jogador);
            return true;
        }
        else{
            addMensagem("Jogador %c move casa(%d,%d) para casa(%d,%d)", jogador, ci, cj, pi, pj);
        }
    }
    jogador = jogador=='A'? 'B' : 'A';
    return true;
}

bool testarCasa(int ci, int cj, int quant){
    if(tab[ci][cj] == S){
        //addMensagem("A casa(%d, %d) nunca pode ser movida.", ci, cj);
        return false;
    }
    int c = pecas[ci][cj];
    if(c==0){
        //printf("Nao existem pecas na casa (%d,%d).\n", ci ,cj);
        return false;
    }
    
    switch (jogador)
    {
        case 'A':
            if(c<0){
                //printf("Essas pecas sao do Jogador B.\n");
                return false;
            }
            break;

        case 'B':
            if(c>0){
                //printf("Essas pecas sao do Jogador A.\n");
                return false;
            }
            break;
    }

    int pi, pj;
    if(ci != 1){
        pi = ci;
        pj = cj - quant;
        if(pj < 0){
            pi = 1;
            pj = -pj - 1;
        }
    }
    else{
        pi = ci;
        pj = cj + quant;
        if(pj >= 8){
            pi = jogador == 'A'? 0 : 2;
            pj = 7 - (pj - 8);
        }
    }

    int p = pecas[pi][pj];

    if(cj>5 && pj<5){
        //printf("jogador %a nao pode avancar %d casas a partir daqui.\n", jogador, quant);
        return false;
    }
    if(pj == 5 && pi != 1){
        //printf("%d pecas do Jogador %c saem do tabuleiro.\n", c,jogador);
        return true;
    }

    if(p!=0 && (c>0)!=(p>0)){
        //printf("c=%d p=%d",c,p);
        if(tab[pi][pj] == R){
            //printf("A peca do Jogador %c esta protegida pela casa com Roseta.\n", (jogador=='A')?'B':'A');
            return false;
        }
        //printf("A peca do Jogador %c ataca as pecas do Jogador %c!\n", jogador, (jogador=='A')?'B':'A');
        return true;
    }
    else{
        //if(tab[pi][pj]==R){
            //printf("Jogador %c atinge uma casa com Roseta, joga novamente.\n", jogador);
        //}
        //printf("Jogador %c joga normal.\n");
        return true;
    }

}

bool movimentoValido(int quant){
    int i, j;
    int ji = jogador=='A'?0:1;
    for(i =ji; i<=ji+1; i++){
        for(j = 0;j<TAB_LAR;j++){
            if(testarCasa(i,j,quant)){
                return true;
            }
        }
    }
    return false;
}

bool lerEntrada(int *ci,int *cj){
    char entrada[15];
    fgets(entrada,15,stdin);
    size_t l = strlen(entrada);
    if(l == 0){
        //printf("nada foi inserido ci = %d, cj = %d, +=%d.\n");
        return false;
    }
    if(entrada[0] == 'i'){
        *ci = jogador=='A'?0:2;
        *cj = 4;
        return true;
    }
    
    char *start = entrada;
    char *end;
    *ci = *cj = -1;
    for(long i = strtol(start,&end, 10); start != end; i = strtol(start,&end,10)){
        //printf(" '%.*s' -> ", (int)(end-start),start);
        //printf("%d\n",i);
        start = end;
        if(*ci<0) *ci = (int)i;
        else if(*cj<0) *cj = (int)i;
        else {
            //printf("mais de um valor foi inserido. ci = %d, cj = %d, +=%d.\n",*ci,*cj,i);
            return false;
        }
        //printf("%ld\n",i);
    }
    if(*cj < 0){
        //printf("apenas ci foi inserido. ci = %d, cj = %d\n", *ci, *cj);
        return false;
    }
    return true;
}

// void addMensagem(char* mensagem){
//     mensagens[mensagemPos] = mensagem;
//     mensagemPos = (mensagemPos+1)%NUM_MENSAGEM;
// }


#if defined(_WIN32) || defined(_WIN64) || defined(_WIN)

void setColor(unsigned char color){
    SetConsoleTextAttribute(hConsole, color);
}

void setCursorPos(short int x, short int y){
    COORD pos = {x,y};
    SetConsoleCursorPosition(hConsole, pos);
}

Key getKeyPressed(){
    
    while(kbhit()){
        int c = getch();
        if(c == 0 || c == 0xE0){
            c = getch();
            switch (c){
            case 0x48:
                return UP_ARROW;
            case 0x50:
                return DOWN_ARROW;
            case 0x4d:
                return RIGHT_ARROW;
            case 0x4b:
                return LEFT_ARROW;
            }
        }
        switch(c){
            case 0xd:
                return ENTER;
                break;
            case 0x1b:
                return ESCAPE;
                break;
        }
    }

    return NONE;
}

void updateConsole(){
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole,&csbi);
    short width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    short height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    if(width != consoleWidth || height != consoleHeight){
        COORD bufferDimen = {max(width,MIN_CONSOLE_WIDTH), MIN_CONSOLE_HEIGHT};
        int r = SetConsoleScreenBufferSize(hConsole, bufferDimen);
        
        if(r==0) {
            setCursorPos(0,0);
            setColor(0x0e);
            printf("SetConsoleScreenBufferSize error: %d\n", GetLastError());
            system("pause");
        }

        r = GetConsoleScreenBufferInfo(hConsole,&csbi);

        if(r==0) {
            setCursorPos(0,0);
            setColor(0x0e);
            printf("GetConsoleScreenBufferInfo error: %d\n", GetLastError());
            system("pause");
        }

        SMALL_RECT newSize = {0,0,bufferDimen.X-1,bufferDimen.Y-1};
        r = SetConsoleWindowInfo(hConsole, TRUE, &newSize);
        if(r==0){
            setCursorPos(0,0);
            setColor(0x0e);
            printf("SetConsoleWindowInfo error: %d\n", GetLastError());
            system("pause");
        }
        
        CONSOLE_CURSOR_INFO cci;
        cci.bVisible = false;
        cci.dwSize = 100;
        SetConsoleCursorInfo(hConsole,&cci);
        consoleWidth = width;
        consoleHeight = height;
        clearConsole();
        //TODO: fazer com que o resize funcione!!
        // setCursorPos(0,0);
        // printf("updated!!!!");
        // system("pause");
    }
}

#define PERR(bSuccess, api){if(!(bSuccess)) {setCursorPos(0,0); printf("%s:Error %d from %s  on line %d\n", __FILE__, GetLastError(), api, __LINE__); system("pause");};}

void clearConsole(){
    COORD pos = {0,0};
    DWORD _;
    BOOL sucess = FillConsoleOutputCharacter(hConsole, ' ', consoleWidth*consoleHeight,pos, &_);
    PERR(sucess, "FillConsoleOutputCharacter");
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    sucess = FillConsoleOutputAttribute(hConsole, 0x07, consoleWidth*consoleHeight,pos, &_);
    PERR(sucess, "FillConsoleOutputCharacter");
}

#elif defined(unix) || defined(__unix__)

void setColor(unsigned char color){
    if(color == 0x0f || color == 0x0f){
        attroff(COLOR_PAIR(2));
    }
    else{
        attron(COLOR_PAIR(2));
    }
}

void setCursorPos(short int x, short int y){
    //printf("\033[%d;%dH", (y), (x));
    move(y,x);
    ////refresh();    
}

Key getKeyPressed(){
    
    while(true){
        int c = getch();
        setCursorPos(0,15);
        printf("%10c",' ');
        setCursorPos(0,15);
        printf("%x",c);
        if(c == EOF){
            break;
        }
        if(c == '\033'){
            getch();
            c = getch();
            printf(" %x",c);
            switch (c){
            case 'A':
                return UP_ARROW;
            case 'B':
                return DOWN_ARROW;
            case 'C':
                return RIGHT_ARROW;
            case 'D':
                return LEFT_ARROW;
            }
        }
        switch(c){
            case 0xa:
                return ENTER;
                break;
            case 0x1b:
                return ESCAPE;
                break;
        }
    }

    return NONE;
}

void updateConsole(){
    
}

#define PERR(bSuccess, api){if(!(bSuccess)) {setCursorPos(0,0); printf("%s:Error %d from %s  on line %d\n", __FILE__, GetLastError(), api, __LINE__); system("pause");};}

void clearConsole(){
    //printf("\033c");
    clear();
    //refresh();
}

#endif


void addMensagem(const char* mensagem, ...){
    va_list args;
    va_list argsc;
    va_start(args,mensagem);
    va_copy(argsc, args);
    size_t need = 1 + vsnprintf(NULL, 0, mensagem, args);
    va_end(args);

    va_start (argsc,mensagem);
    char* s = malloc(need);
    vsprintf(s,mensagem,args);
    va_end(argsc);

    mensagens[mensagemPos] = s;
    mensagemPos = (mensagemPos+1)%NUM_MENSAGEM;
}

void apagaMensagens(){
    int i;
    for(i = 0;i<NUM_MENSAGEM;i++){
        mensagens[i] = NULL;
    }
}
void printMensagens(int x, int y, int w, int h){
    int i;

    setCursorPos(x,y);
    setColor(0x70);
    printf("%c",201);
    for(i = 0; i< w-2;i++)
        printf("%c",205);
    printf("%c\n",187);

    for(i=1;i<h;i++){
        setCursorPos(x,y+i);
        printf("%c",186);
        setCursorPos(x+w-1,y+i);        
        printf("%c",186);
    }

    for(i=0;i<h-1;i++){
        int m = (( NUM_MENSAGEM - i)+mensagemPos-1)%NUM_MENSAGEM;
        char* men = mensagens[m];
        setCursorPos(x+1,y + h-1-i);
        if(men != NULL && i<NUM_MENSAGEM){
            int color = 0;
            color = men[8] == 'A'? 0x70 : men[8] == 'B'? 0xf0 : 0xe0;
            color += strlen(men)>50? 0x0c : 0;
            setColor(color);
            
            if(strlen(men) >= w-2) men[w-2] = '\0';
            printf("%-*s", w-2, men);
        }
        else{
            setColor(0x70);
            printf("%*c", w-2, ' ');
        }
    }

    setColor(0x70);
    setCursorPos(x,y+h);
    printf("%c",200);
    for(i = 0; i< w-2;i++)
        printf("%c",205);
    printf("%c",188);

    setColor(0x0f);
}


void drawTextCenter(short y, char color, char* text, ...){
    

    va_list args;
    va_start(args,text);
    size_t length =  vsnprintf(NULL, 0, text, args);

    setCursorPos(consoleWidth/2 - length/2,y);
    setColor(color);
    vprintf(text, args);

    va_end(args);
}

int jogarDados(){
    int num = rand()%2 + rand()%2 + rand()%2;
    num = (num == 0 ? 4 : num);
    return num;
}

GameState menuPrincipal();
GameState instrucoes();
GameState jogo();
GameState menuJogadores();

int main(int argc, char argv[]) {
    srand( time(NULL) );

    #if defined(_WIN32)
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    #elif defined(unix)
    initscr();
    start_color();

    init_pair(1,COLOR_WHITE, COLOR_BLACK);
    init_pair(2,COLOR_BLACK, COLOR_WHITE);

    bkgd(COLOR_PAIR(1));
    #endif
    
    updateConsole();

    isBot[0] = true;
    isBot[1] = true;

    while(gameState != SAIR){
        setColor(0x0f);
        clearConsole();
        switch (gameState)
        {
        case MENU_PRINCIPAL:
            gameState = menuPrincipal();
            break;
        case INSTRUCOES:
            gameState = instrucoes();
            break;
        case JOGO:
            gameState = jogo();
            break;
        case MENU_JOGADORES:
            gameState = menuJogadores();
            break;
        default:
            gameState = SAIR;
            break;
        }
    }
    
    return 0;
}

GameState menuPrincipal(){
    //> Menu Principal

    int opc = 1;
    int i = 0;
    while(true){
        Key key = getKeyPressed();

        if(key == DOWN_ARROW){
            opc++;
            opc=(opc-1)%3 + 1;
        }
        if(key == UP_ARROW){
            opc--;
            opc=(opc+2)%3 + 1;
        }
        if(key == ENTER){
            switch (opc)
            {
            case 1:
                return MENU_JOGADORES;
            case 2:
                return INSTRUCOES;
            case 3:
                return SAIR;
            }
        }

        updateConsole();


        setCursorPos(0,0);
        i++;
        printf("%d",i);

        //printf("Bem vindo ao Jogo Real de Ur!!");
        drawTextCenter(2, 0x0f, "JOGO REAL DE UR");
        //drawTextCenter(2, 0xf0, "-- Menu Principal --");

        drawTextCenter(5, opc==1? 0xf0 : 0x0f, "Jogar");
        drawTextCenter(7, opc==2? 0xf0 : 0x0f, "Instrucoes");
        drawTextCenter(9, opc==3? 0xf0 : 0x0f, "Sair");
        setColor(0x0f);
        //refresh();
        usleep(16);
    }
}

GameState instrucoes(){
    clearConsole();
    setCursorPos(0,0);
    // printf("  [[ O JOGO ]]");
    // setCursorPos(0,1);
    // printf(" O Jogo Real de Ur foi encontrado nas escavacoes feitas na antiga cidade-estado de Ur pelo");
    // printf(" arqueologo britanico Sir Leonard Wooley na decada de 1920. Um dos tabuleiros encontrados");
    // printf(" encontra-se exposto no Museu Britanico, em Londres. As suas regras originais sao desconhecidas e");
    // printf(" a maneiro como eh jogado atualmente foi baseado em documentos escritos, figuras e jogos");
    // printf(" semelhantes. A reconstituicao dessas regras diz que o Jogo Real de Ur pode ser jogado com catorze");
    // printf(" pecas sendo sete de um lado e sete de outro. As pecas movimentam-se pelo tabuleiro utilizando");
    // printf(" tres dados em forma de piramide que determinam o numero de casas que devem avancar a cada");
    // printf(" jogada. O objectivo do jogo eh fazer com que as sete pecas percorram todo o tabuleiro primeiro que");
    // printf(" o adversario.\n\n");
    printf("  [[ REGRAS ]]\n");
    printf(" - Ha dois jogadores, Jogador A e Jogador B, cada um com 7 pecas; e tres dados tetraedricos");
    printf(" onde dois vertices estao marcados e os outro dois nao.\n");
    printf(" - O numero de casas que se movimentarao as pecas e obtido ao jogar os tres dados e");
    printf(" contar o numero de marcas. Se o numero for 0, se move 4 casas.\n");
    printf(" (Tudo isso eh automaticamente calculado pelo jogo)\n");
    printf(" - Os jogadores comecam com ambos jogando os dados. O com maior pontuacao joga primeiro.");
    printf(" - Os jogadores jogam em turnos, nos quais sempre jogam os dados antes de fazer um movimento");
    printf(" - As pecas de cada jogador entram de lados opostos, com as do Jogador A entrando por cima");
    printf(" e as do Jogador B por baixo.\n");
    printf(" - Em cada turno, um jogador pode decidir inserir uma peca (usando como entrada 'i'), ou escolher");
    printf(" mover uma peca ao entrar as coordenadas da casa onde a peca esta (usando como entrada'i j', \n");
    printf(" onde 'i' corresponde a linha da casa e 'j' a coluna\n");
    printf(" - Ao ser inserida a peca entra pela casa(0,4), caso seja o Jogador A, ou pela casa(2,4), caso");
    printf(" seja o jogador B, e move para a esquerda.\n");
    printf(" - As pecas ao serem movidas se movem para a esquerda caso estes estejam na linha 0 ou 2, e ");
    printf(" se movem para direita caso estejam na linha 1. Quando atingem a parte mais a esquerda, as\n");
    printf(" pecas continuam o percurso na linha 1, e quando atingem a direita ela continua o percurso\n");
    printf(" novamente na linha 0, caso seja do Jogador A, ou linha 2, caso seja do Jogador B.\n");
    printf(" - Caso um jogador mova uma peca para uma casa com roseta (indicada entre chaves '{'), o turno");
    printf(" desse jogador se repete. Caso contrario o turno passa para o outro jogador.\n");
    printf(" - Caso um jogador nao tenha nenhum movimento possivel, se passa a vez para outro.");
    printf(" - Duas ou mais pecas podem ocupar a mesma casa, sendo todas movidas juntas ao se mover a casa.");
    printf(" - Caso uma peca se mova para uma casa que esteja ocupado por pecas inimigas, a pecas inimigas");
    printf(" sao retiradas do tabuleiro, tendo que serem posteriormente inseridos novamente no tabuleiro.\n");
    printf(" - A regra anterior nao se aplica quando a peca inimiga estiver em uma casa com roseta (indicada");
    printf(" pela cor vermelha).\n");
    printf(" - Para que um dos jogadores ganhe, ele deve fazer com que todas as suas 7 pecas chegam a casa");
    printf(" final, casa(0,5) ou casa(2,5).\n");
    printf(" - Para que uma peca se mova para a casa final, a peca deve ser movida no exato numero de casas");
    printf(" que leve a peca cair na saida, movimentos maiores nao sao legais.\n");

    printf("\n Precione ENTER para voltar.");
    while(getKeyPressed() != ENTER) { usleep(100); }
    return MENU_PRINCIPAL;
}

GameState menuJogadores(){
    int opc = 1;
    while(true){
        Key key = getKeyPressed();

        if(key == DOWN_ARROW){
            opc++;
            opc=(opc-1)%4 + 1;
        }
        if(key == UP_ARROW){
            opc--;
            opc=(opc+3)%4 + 1;
        }
        if(key == ENTER){
            if(opc == 4) return MENU_PRINCIPAL;
            isBot[0] = opc == 3;
            isBot[1] = opc >= 2;
            return JOGO;
        }

        updateConsole();

        drawTextCenter(2, 0x0f, "Selecione os Jogadores");

        drawTextCenter(4, opc==1? 0xf0 : 0x0f, "Humano vs Humano");
        drawTextCenter(5, opc==2? 0xf0 : 0x0f, "Humano vs Bot   ");
        drawTextCenter(6, opc==3? 0xf0 : 0x0f, "   Bot vs Bot   ");

        drawTextCenter(8, opc==4? 0xf0 : 0x0f, "   Voltar   ");
        usleep(16);
    }   
}

GameState jogo(){

    clearConsole();
    apagaMensagens();
    iniciarPecas();
    pecasS[0] = pecasS[1] = 0;

    while(true){
        int dA = jogarDados();
        addMensagem("Jogador A joga os dados, e saiu %d.", dA);
        

        int dB = jogarDados();
        addMensagem("Jogador B joga os dados, e saiu %d.", dB);

        if(dA == dB){
            addMensagem("Os dois tiveram o mesmo resultado, jogam denovo.");
            continue;
        }
        jogador = (dA>dB)? 'A' : 'B';
        addMensagem("Jogador %c comeca, por ter tirado maior numero.", jogador);

        iniciarTree(jogador, pecas);
        break;
    }

    char ultJog = jogador;

    int ci;
    int cj;
    int dado;
    bool mover = false;
    int j = jogador=='A'? 0 : 1;

    typedef enum JogoState {NOVO_TURNO, SELECIONANDO_PECA, MENU, GANHOU} JogoState;

    JogoState jogoState = NOVO_TURNO;

    char ganhou = 0;

    while(true){
        switch(jogoState){
            case NOVO_TURNO:{
                dado = jogarDados();
                addMensagem("Jogador %c joga os dados e saiu %d.", jogador, dado);

                if(!movimentoValido(dado)){
                    //printf("Nao ha movimentos validos.\n");
                    addMensagem("Jogador %c nao tem movimentos validos.", jogador);
                    jogador = (jogador== 'A' ? 'B' : 'A');
                    moverTree(0,-1,dado);
                    jogoState = NOVO_TURNO;
                }
                else{
                    jogoState = SELECIONANDO_PECA;
                }
                j = jogador == 'A'? 0 : 1;
                ci = j*2;
                cj = 4;
                if(getKeyPressed() == ESCAPE){
                    jogoState = MENU;
                }
                mover = false;
            } break;
            case SELECIONANDO_PECA:{

                if(isBot[j]){
                    Bot_pegarComando(&ci,&cj,dado);
                    //usleep(200);
                    mover = true;
                }
                else{
                    Key key = getKeyPressed();
                    switch(key){
                        case LEFT_ARROW:{
                            int nj;
                            for(nj = cj-1; nj>=0; nj--){
                                if(pecas[ci][nj] != 0 && ( (pecas[ci][nj]<0) == j ) ){
                                    cj = nj;
                                    break;
                                }
                            }
                        } break;
                        case RIGHT_ARROW: {
                            int nj;
                            for(nj = cj+1; nj<TAB_LAR; nj++){
                                if(pecas[ci][nj] != 0 && ( (pecas[ci][nj]<0) == j ) ){
                                    cj = nj;
                                    break;
                                }
                            }
                        } break;
                        case DOWN_ARROW: {
                            int ni = ci;
                            if(ni+1<=j+1){
                                ni+=1;
                                int i;
                                char find = 0b11;
                                for(i = 0; i<TAB_LAR+1;i++){
                                    if( (find & 0b01) && ((cj + i) < TAB_LAR)){
                                        if(pecas[ni][cj + i] != 0 && ( (pecas[ni][cj + i]<0) == j ) ){
                                            cj = cj + i;
                                            break;
                                        }
                                    }else{
                                        find = find & (~0b01);
                                    }
                                    if( (find & 0b10) && ((cj - i) >= 0)){
                                        if(pecas[ni][cj - i] != 0 && ( (pecas[ni][cj - i]<0) == j ) ){
                                            cj = cj - i;
                                            break;
                                        }
                                    }
                                    else{
                                        find = find & (~0b10);                               
                                    }
                                    if(find == 0b00) break;
                                }
                                if(find == 0b00) continue;
                                else{
                                    ci = ni;
                                    break;
                                }
                            }
                        } break;
                        case UP_ARROW: {
                            int ni = ci;
                            if(ni-1>=j){
                                ni-=1;
                                int i;
                                char find = 0b11;
                                for(i = 0; i<TAB_LAR+1;i++){
                                    if( (find & 0b01) && cj + i < TAB_LAR){
                                        if(pecas[ni][cj + i] != 0 && ( (pecas[ni][cj + i]<0) == j ) ){
                                            cj = cj + i;
                                            break;
                                        }
                                    }else{
                                        find = find & (~0b01);
                                    }
                                    if( (find & 0b10) && cj - i >= 0){
                                        if(pecas[ni][cj - i] != 0 && ( (pecas[ni][cj - i]<0) == j ) ){
                                            cj = cj - i;
                                            break;
                                        }
                                    }
                                    else{
                                        find = find & (~0b10);
                                    }
                                    if(find == 0b00) break;
                                }
                                if(find == 0b00) continue;
                                else{
                                    ci = ni;
                                    break;
                                }
                            }
                        } break;
                        case ENTER: {
                            mover = true;
                        }  break;
                        case ESCAPE: {
                            ci = 0;
                            jogoState = MENU;
                        } break;
                    }
                }

                if(mover){
                    mover = false;
                    if( !moverCasa(ci, cj, dado) ){
                        //addMensagem("Algo muito errado aconteceu...");
                        //addMensagem("você foi capaz de selecionar um movimento invalido!");
                        continue;
                    }
                    else{
                        moverTree(ci,cj,dado);

                        if(pecasS[0] == 7) ganhou = 'A';
                        if(pecasS[1] == 7) ganhou = 'B';

                        jogoState = NOVO_TURNO;
                    }
                }

                if(ganhou){
                    jogoState = GANHOU;
                }
            } break;
            case MENU:{
                Key key = getKeyPressed(); //TODO: terminar esse negocio aqui
                ci %= 2;
                cj = -1;
                switch (key)
                {
                case ENTER:
                    if(ci == 1){
                        destroyTree();
                        return MENU_PRINCIPAL;
                    }
                    //else { faça o mesmo que a tecla ESCAPE }
                case ESCAPE:
                    ci = j*2;
                    cj = 4;
                    jogoState = SELECIONANDO_PECA;
                    clearConsole();
                    goto SWITCH_JOGOSTATE_EXIT;
                    break;
                case UP_ARROW:
                    ci--;
                    ci = (ci+2)%2;
                    break;
                case DOWN_ARROW:
                    ci++;
                    ci = ci%2;
                    break;
                }

                setCursorPos(25 - 9,9);
                setColor(ci==0? 0xf0 : 0x0f);
                printf("Continuar Jogando");

                setCursorPos(25 - 7,11);
                setColor(ci==1? 0xf0 : 0x0f);
                printf("Voltar ao Menu");
            } break;
            case GANHOU:{

                Key key = getKeyPressed(); //TODO: terminar esse negocio aqui
                ci %= 2;
                cj = -1;
                switch (key)
                {
                case ENTER:
                    if(ci == 0){
                        destroyTree();
                        return JOGO;
                    }
                    else{ 
                        return MENU_PRINCIPAL;
                    }
                    break;
                case UP_ARROW:
                    ci--;
                    ci = (ci+2)%2;
                    break;
                case DOWN_ARROW:
                    ci++;
                    ci = ci%2;
                    break;
                }

                setCursorPos(0,8);
                setColor(0x0f);
                printf("  <<       --  JOGADOR %c GANHOU!!  --       >>", ganhou);

                setCursorPos(1,10);
                printf("Quer jogar novamente? ");

                setCursorPos(2,12);
                setColor(ci==0? 0xf0 : 0x0f);
                printf("Jogar Novamente");

                setCursorPos(2,14);
                setColor(ci==1? 0xf0 : 0x0f);
                printf("Voltar ao Menu");
            } break;
        }
        SWITCH_JOGOSTATE_EXIT: ;


        //setCursorPos(0,17);
        //printf("%d %d", ci, cj);
        
        setCursorPos(0,0);
        desenha(ci, cj);
        printMensagens(50, 0, consoleWidth-51,consoleHeight-1);
        usleep(16);
    }
}

