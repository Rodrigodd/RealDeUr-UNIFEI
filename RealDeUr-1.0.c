#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
// #if defined(_WIN32) || defined(_WIN64) || defined(_WIN)
// #include <windows.h>
// #define sleep(t) Sleep(t)
// #elif defined(unix) || defined(__unix__)
// #include <unistd.h>
// #else
// #define sleep(t) printf(# t);
// #endif




#define TAB_LAR 8
#define TAB_ALT 3

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
        if(ci==(jogador=='A'?0:2) && cj==4)
            printf("Nao existem pecas para ser inseridas.\n", ci ,cj);
        else
            printf("Nao existem pecas na casa (%d,%d).\n", ci ,cj);
        return false;
    }
    
    switch (jogador)
    {
        case 'A':
            if(c<0){
                printf("Essas pecas sao do Jogador B.\n");
                return false;
            }
            break;

        case 'B':
            if(c>0){
                printf("Essas pecas sao do Jogador A.\n");
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
        printf("jogador %a nao pode avancar %d casas a partir daqui.\n", jogador, quant);
        return false;
        //pecas[4][jogador == 'A'? 0 : 2] += pecas[ci][cj];
    }
    if(pj == 5 && pi != 1){
        printf("%d pecas do Jogador %c saem do tabuleiro.\n", abs(c),jogador);
        pecasS[jogador=='A'?0:1] += abs(c);
        pecas[ci][cj] = 0;
    }

    if(p!=0 && (c>0)!=(p>0)){
        //printf("c=%d p=%d",c,p);
        if(tab[pi][pj] == R){
            printf("A peca do Jogador %c esta protegida pela casa com Roseta.\n", (jogador=='A')?'B':'A');
            return false;
        }
        printf("A peca do Jogador %c ataca as pecas do Jogador %c!\n", jogador, (jogador=='A')?'B':'A');
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
            printf("Jogador %c atinge uma casa com Roseta, joga novamente.\n", jogador);
            return true;
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

int jogarDados(){
    int num = rand()%2 + rand()%2 + rand()%2;
    num = (num == 0 ? 4 : num);
    return num;
}

int main()
{
    srand( time(NULL) );

    COMECA:{
        //desenha();

        int dA = jogarDados();
        printf("Jogador A joga os dados, e saiu %d.\n", dA);

        int dB = jogarDados();
        printf("Jogador B joga os dados, e saiu %d.\n", dB);

        if(dA == dB){
            printf("Os dois tiveram o mesmo resultado, jogam denovo.\n\n");
            goto COMECA;
        }
        jogador = (dA>dB)? 'A' : 'B';
        printf("Jogador %c comeca, por ter tirado maior numero.\n", jogador);
    }

    while(true){
        //system("cls");
        desenha();

        int dado = jogarDados();
        printf("Jogador %c joga os dados, e saiu %d.\n", jogador, dado);

        if(!movimentoValido(dado)){
            printf("Nao ha movimentos validos.\n");
            jogador = (jogador== 'A' ? 'B' : 'A');
            continue;
        }

        int j = jogador=='A'?0:1;

        int ci, cj;
        JOGA:
        if(pecas[4][j*2]+pecasS[j] == 7){
            printf("Jogador %c insere uma peca no jogo (unico movimento possivel).\n",jogador);
            ci = j*2;
            cj = 4;
        }
        else{
            printf("Digite a linha e a coluna da casa que queira mover");
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
                break;
            }
        }
    }
    return 0;
}