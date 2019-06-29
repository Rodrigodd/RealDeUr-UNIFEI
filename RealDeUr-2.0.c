#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#if defined(_WIN32) || defined(_WIN64) || defined(_WIN)
#include <windows.h>
#define sleep(t) Sleep(t)
#elif defined(unix) || defined(__unix__)
#include <unistd.h>
#else
#define sleep(t) printf(# t);
#endif




#define TAB_LAR 8
#define TAB_ALT 3

#define NUM_MENSAGEM 10

#define R 'R'
#define E 'E'
#define S 'S'
#define V ' '

#define JA 'X'
#define JB 'O'

typedef enum {false, true} bool;

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

typedef enum {MENU_PRINCIPAL, INSTRUCOES, JOGO, SAIR} State;
State state;

char* mensagens[NUM_MENSAGEM];
char mensagemPos = 0;
void addMensagem(const char* mensagem, ...);

void clearScreen(){
    #if defined(_WIN32) || defined(_WIN64) || defined(_WIN)
    system("cls");
    #define sleep(t) Sleep(t)
    #elif defined(unix) || defined(__unix__)
    system("clear");
    #endif
}

void desenha(){
    int i, j;

    printf("\n   ");
    for(j=0;j<TAB_LAR;j++){
        printf("  %d  ", j);
    }
    printf("\n\n");
    for(i = 0;i<TAB_ALT;i++){
        printf("%d  ", i);
        for(j = 0;j<TAB_LAR;j++){
            const char c = tab[i][j];
            if(c!=S && c!=E && c!=R) printf("["); 
            else if(c!=R) printf(" ");
            else printf("{");

            if(pecas[i][j] == 0){
                printf(" %c ",tab[i][j]);
            }
            else{
                if(pecas[i][j]!=0) printf("%c%2d",pecas[i][j]>0?'A':'B',abs(pecas[i][j]));
                else               printf("   ");
            }

            if(c!=S && c!=E && c!=R) printf("]");
            else if(c!=R) printf(" ");
            else printf("}");
        }
        printf("\n\n");
    }
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
        addMensagem("jogador %a nao pode avancar %d casas a partir daqui.", jogador, quant);
        return false;
        //pecas[4][jogador == 'A'? 0 : 2] += pecas[ci][cj];
    }
    if(pj == 5 && pi != 1){
        //printf("%d pecas do Jogador %c saem do tabuleiro.\n", abs(c),jogador);
        addMensagem("%d pecas do Jogador %c saem do tabuleiro.", abs(c),jogador);
        pecasS[jogador=='A'?0:1] += abs(c);
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
            pecas[pi][pj]+=jogador=='A'? 1 : -1;;
            pecas[ci][cj]+=jogador=='A'? -1 : 1;;
        }

        if(tab[pi][pj]==R){
            //printf("Jogador %c move casa(%d,%d) para casa(%d,%d) e atinge uma Roseta, joga novamente.\n", jogador,ci,cj,pi,pj);
            addMensagem("Jogador %c move casa(%d,%d) para casa(%d,%d) e atinge uma Roseta, joga novamente.", jogador,ci,cj,pi,pj);
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

void addMensagem(const char* mensagem, ...){


    va_list args;
    va_list args2;
    fflush(stdout);

    va_start(args,mensagem);
    va_copy(args2,args);
    size_t need = 1 + vsnprintf(NULL, 0, mensagem, args);
    va_end(args);

    fflush(stdout);

    char* s = malloc(need);
    vsprintf(s,mensagem,args2);
    va_end(args2);
    mensagens[mensagemPos] = s;
    mensagemPos = (mensagemPos+1)%NUM_MENSAGEM;
}

void apagaMensagens(){
    int i;
    for(i = 0;i<NUM_MENSAGEM;i++){
        mensagens[i] = NULL;
    }
}
void printMensagens(){
    int i;
    for(i=0;i<NUM_MENSAGEM;i++){
        char* men = mensagens[(i+mensagemPos)%NUM_MENSAGEM];
        if(men == NULL) continue;
        printf("%s\n",men);
    }
}

int jogarDados(){
    int num = rand()%2 + rand()%2 + rand()%2;

    num = (num == 0 ? 4 : num);
    return num;
}

State menuPrincipal();
State instrucoes();
State jogo();

int main()
{
    srand( time(NULL) );
    fflush(stdout);

    while(state != SAIR){
        clearScreen();
        switch (state)
        {
        case MENU_PRINCIPAL:
            state = menuPrincipal();
            break;
        case INSTRUCOES:
            state = instrucoes();
            break;
        case JOGO:
            state = jogo();
            break;
        default:
            state = SAIR;
            break;
        }
    }
    
    return 0;
}

State menuPrincipal(){
    printf("Bem vindo ao Jogo Real de Ur!!\n\n");
    
    printf("[[ Menu Principal ]] \n\n");
    printf(" 1. Jogar\n");
    printf(" 2. Instrucoes\n");
    printf(" 3. Sair\n");
    printf(" \n Digite o numero de opcao para seleciona-la: ");
    LER:;
    int opc;
    scanf("%d", &opc);
    //getchar();

    switch (opc)
    {
    case 1:
        return JOGO;
    case 2:
        return INSTRUCOES;
    case 3:
        return SAIR;
    default:
        printf("opcao invalida, digite novamente: ");
        goto LER;
        break;
    }
}

State instrucoes(){
    printf("[[ O JOGO ]]\n\n");
    printf(" O Jogo Real de Ur foi encontrado nas escavacoes feitas na antiga cidade-estado de Ur pelo\n");
    printf("arqueologo britanico Sir Leonard Wooley na decada de 1920. Um dos tabuleiros encontrados\n");
    printf("encontra-se exposto no Museu Britanico, em Londres. As suas regras originais sao desconhecidas e\n");
    printf("a maneiro como eh jogado atualmente foi baseado em documentos escritos, figuras e jogos\n");
    printf("semelhantes. A reconstituicao dessas regras diz que o Jogo Real de Ur pode ser jogado com catorze\n");
    printf("pecas sendo sete de um lado e sete de outro. As pecas movimentam-se pelo tabuleiro utilizando\n");
    printf("tres dados em forma de piramide que determinam o numero de casas que devem avancar a cada\n");
    printf("jogada. O objectivo do jogo eh fazer com que as sete pecas percorram todo o tabuleiro primeiro que\n");
    printf("o adversario.\n\n");
    printf("[[ REGRAS ]]\n\n");
    printf(" - Ha dois jogadores, Jogador A e Jogador B, cada um com 7 pecas; e tres dados tetraedricos\n");
    printf("  onde dois vertices estao marcados e os outro dois nao.\n");
    printf(" - O numero de casas que se movimentarao as pecas e obtido ao jogar os tres dados e\n");
    printf("  contar o numero de marcas. Se o numero for 0, se move 4 casas.\n");
    printf("  (Tudo isso eh automaticamente calculado pelo jogo)\n");
    printf(" - Os jogadores comecam com ambos jogando os dados. O com maior pontuacao joga primeiro.\n");
    printf(" - Os jogadores jogam em turnos, nos quais sempre jogam os dados antes de fazer um movimento\n");
    printf(" - As pecas de cada jogador entram de lados opostos, com as do Jogador A entrando por cima\n");
    printf("  e as do Jogador B por baixo.\n");
    printf(" - Em cada turno, um jogador pode decidir inserir uma peca (usando como entrada 'i'), ou escolher\n");
    printf("  mover uma peca ao entrar as coordenadas da casa onde a peca esta (usando como entrada'i j', \n");
    printf("  onde 'i' corresponde a linha da casa e 'j' a coluna\n");
    printf(" - Ao ser inserida a peca entra pela casa(0,4), caso seja o Jogador A, ou pela casa(2,4), caso\n");
    printf("  seja o jogador B, e move para a esquerda.\n");
    printf(" - As pecas ao serem movidas se movem para a esquerda caso estes estejam na linha 0 ou 2, e \n");
    printf("  se movem para direita caso estejam na linha 1. Quando atingem a parte mais a esquerda, as\n");
    printf("  pecas continuam o percurso na linha 1, e quando atingem a direita ela continua o percurso\n");
    printf("  novamente na linha 0, caso seja do Jogador A, ou linha 2, caso seja do Jogador B.\n");
    printf(" - Caso um jogador mova uma peca para uma casa com roseta (indicada entre chaves '{'), o turno\n");
    printf("  desse jogador se repete. Caso contrario o turno passa para o outro jogador.\n");
    printf(" - Caso um jogador nao tenha nenhum movimento possivel, se passa a vez para outro.\n");
    printf(" - Duas ou mais pecas podem ocupar a mesma casa, sendo todas movidas juntas ao se mover a casa.\n");
    printf(" - Caso uma peca se mova para uma casa que esteja ocupado por pecas inimigas, a pecas inimigas\n");
    printf("  sao retiradas do tabuleiro, tendo que serem posteriormente inseridos novamente no tabuleiro.\n");
    printf(" - A regra anterior nao se aplica quando a peca inimiga estiver em uma casa com roseta (indicada\n");
    printf("  por entre duas chaves '{').\n");
    printf(" - Para que um dos jogadores ganhe, ele deve fazer com que todas as suas 7 pecas chegam a casa\n");
    printf("  final, casa(0,5) ou casa(2,5).\n");
    printf(" - Para que uma peca se mova para a casa final, a peca deve ser movida no exato numero de casas\n");
    printf("  que leve a peca cair na saida, movimentos maiores nao sao legais.\n");

    printf("\n Precione qualquer tecla para voltar ao menu... ");
    getchar();
    getchar();
    return MENU_PRINCIPAL;
}

State jogo(){
    COMECA:{
        clearScreen();
        apagaMensagens();
        //desenha();

        int dA = jogarDados();
        //printf("Jogador A joga os dados, e saiu %d.\n", dA);
        {
            addMensagem("Jogador A joga os dados, e saiu %d.", dA);
        }
        

        int dB = jogarDados();
        //printf("Jogador B joga os dados, e saiu %d.\n", dB);
        {
            addMensagem("Jogador B joga os dados, e saiu %d.", dB);
        }

        if(dA == dB){
            //printf("Os dois tiveram o mesmo resultado, jogam denovo.\n\n");
            addMensagem("Os dois tiveram o mesmo resultado, jogam denovo.\n");
            goto COMECA;
        }
        jogador = (dA>dB)? 'A' : 'B';
        //printf("Jogador %c comeca, por ter tirado maior numero.\n", jogador);
        addMensagem("Jogador %c comeca, por ter tirado maior numero.", jogador);
    }

    while(true){
        char ultJog = jogador;

        clearScreen();
        desenha();
        printf("\n\n");

        int dado = jogarDados();
        //printf("Jogador %c joga os dados, e saiu %d.\n", jogador, dado);
        addMensagem("Jogador %c joga os dados, e saiu %d.", jogador, dado);

        if(!movimentoValido(dado)){
            //printf("Nao ha movimentos validos.\n");
            addMensagem("Nao ha movimentos validos.");
            jogador = (jogador== 'A' ? 'B' : 'A');
            continue;
        }

        int j = jogador=='A'?0:1;

        int ci, cj;
        JOGA:
        if(pecas[4][j*2]+pecasS[j] == 7){
            //printf("Jogador %c insere uma peca no jogo (unico movimento possivel).\n",jogador);
            addMensagem("Jogador %c insere uma peca no jogo (unico movimento possivel).",jogador);
            ci = j*2;
            cj = 4;

            printMensagens();
        }
        else{
            printMensagens();

            printf("\n\nDigite a linha e a coluna da casa que queira mover");
            if(abs(pecas[j*2][4]) > 0) printf("\n ou digite 'i' para inserir uma nova peca");
            printf(": ");
            while(!lerEntrada(&ci,&cj)){
                printf("Entrada invalida, o comando deve ser na forma ('<linha> <coluna>', ou 'i')\n  digite novamente: ");
            }
        }
        
        //scanf("%d %d", &ci, &cj);
        if(ci<0 || cj<0){
            printf("Nao existe casas com coordenada negativa.");
            goto JOGA;
        }
        if(ci>2 || cj>7){
            printf("Nao existe casas fora dos limites 'linha > 2' ou 'coluna > 7'.");
            goto JOGA;
        }

        if( !moverCasa(ci, cj, dado) ) goto JOGA;
        printf("\n");

        char ganhou = 0;
        if(pecasS[0] == 7) ganhou = 'A';
        if(pecasS[1] == 7) ganhou = 'B';

        if(ganhou){
            printf("\n\n  <<  --  JOGADOR %c GANHOU!!  --  >>\n\n", ganhou);
            printf("Quer jogar novamente? (s/n) ");
            char denovo = getchar();
            if(denovo == 's'){
                goto COMECA;
            } else {
                printf("\n\ntchau!");
                //sleep(1000);
                return MENU_PRINCIPAL;
            }
        }
        if(ultJog != jogador){
            addMensagem(" ");
        }
    }
}

