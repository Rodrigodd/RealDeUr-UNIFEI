#include "RealBot.h"

#include <stdlib.h>
#include <stdio.h>


struct State *tabTree = NULL;

bool moverCasaBot(int _pecas[3][8], char* _jogador, int ci, int cj, int quant){
    if(tab[ci][cj] == S) return false;
    int c = _pecas[ci][cj];
    if(c==0){
        //printf("Nao existem pecas na casa (%d,%d).\n", ci ,cj);
        return false;
    }
    
    switch (*_jogador)
    {
        case 'A':
            if(c<0){
                return false; //printf("Essas pecas sao do Jogador B.\n");
            }
            break;
        case 'B':
            if(c>0){
                return false; //printf("Essas pecas sao do Jogador A.\n");
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
            pi = *_jogador == 'A'? 0 : 2;
            pj = 7 - (pj - 8);
        }
    }
    int p = _pecas[pi][pj];

    if(cj>5 && pj<5){
        //printf("jogador %a nao pode avancar %d casas a partir daqui.\n", jogador, quant);
        return false;
        //pecas[4][jogador == 'A'? 0 : 2] += pecas[ci][cj];
    }
    if(pj == 5 && pi != 1){
        //printf("%d pecas do Jogador %c saem do tabuleiro.\n", abs(c),jogador);
        int j = *_jogador=='A'?0:1;
        //_pecasS[j] += abs(c);
        _pecas[j*2][5] += c;
        _pecas[ci][cj] = 0;
    }

    if(p!=0 && (c>0)!=(p>0)){
        if(tab[pi][pj] == R){
            //addMensagem("A peca do Jogador %c esta protegida pela casa com Roseta.", (jogador=='A')?'B':'A');
            return false;
        }
        //addMensagem("A peca do Jogador %c ataca as pecas do Jogador %c!", jogador, (jogador=='A')?'B':'A');
        if(*_jogador == 'A'){
            _pecas[2][4] += _pecas[pi][pj];
        } else{
            _pecas[0][4] += _pecas[pi][pj];
        }
        
        if(tab[ci][cj]!=E){ //ci != 0 && cj != 4){
            _pecas[pi][pj] = _pecas[ci][cj];
            _pecas[ci][cj] = 0;
        }
        else{
            _pecas[pi][pj]  = *_jogador=='A'? 1 : -1;
            _pecas[ci][cj] += *_jogador=='A'? -1 : 1;;
        }
    }
    else{
        if(tab[ci][cj]!=E){ //ci != 0 && cj != 4){
            _pecas[pi][pj] += _pecas[ci][cj];
            _pecas[ci][cj] = 0;
        }
        else{
            _pecas[pi][pj]+=*_jogador=='A'? 1 : -1;;
            _pecas[ci][cj]+=*_jogador=='A'? -1 : 1;;
        }

        if(tab[pi][pj]==R){
            //addMensagem("Jogador %c move casa(%d,%d) para casa(%d,%d) e atinge uma Roseta.", jogador,ci,cj,pi,pj);
            //addMensagem("Jogador %c joga novamente.",jogador);
            return true;
        }
        else{
            //addMensagem("Jogador %c move casa(%d,%d) para casa(%d,%d)", jogador, ci, cj, pi, pj);
        }
    }
    *_jogador = *_jogador=='A'? 'B' : 'A';
    return true;
}

bool testarCasaBot(const int _pecas[3][8], const char* _jogador, int ci, int cj, int quant){
    if(tab[ci][cj] == S) return false;
    int c = _pecas[ci][cj];
    if(c==0){
        //printf("Nao existem pecas na casa (%d,%d).\n", ci ,cj);
        return false;
    }
    
    switch (*_jogador)
    {
        case 'A':
            if(c<0){
                return false; //printf("Essas pecas sao do Jogador B.\n");
            }
            break;
        case 'B':
            if(c>0){
                return false; //printf("Essas pecas sao do Jogador A.\n");
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
            pi = *_jogador == 'A'? 0 : 2;
            pj = 7 - (pj - 8);
        }
    }
    int p = _pecas[pi][pj];

    if(cj>5 && pj<5){
        //printf("jogador %a nao pode avancar %d casas a partir daqui.\n", jogador, quant);
        return false;
        //pecas[4][jogador == 'A'? 0 : 2] += pecas[ci][cj];
    }
    if(pj == 5 && pi != 1){
        //printf("%d pecas do Jogador %c saem do tabuleiro.\n", abs(c),jogador);
        //int j = *_jogador=='A'?0:1;
        //_pecasS[j] += abs(c);
        //_pecas[j*2][5] += c;
        //_pecas[ci][cj] = 0;
    }

    if(p!=0 && (c>0)!=(p>0)){
        if(tab[pi][pj] == R){
            //addMensagem("A peca do Jogador %c esta protegida pela casa com Roseta.", (jogador=='A')?'B':'A');
            return false;
        }
        //addMensagem("A peca do Jogador %c ataca as pecas do Jogador %c!", jogador, (jogador=='A')?'B':'A');
        if(*_jogador == 'A'){
            //_pecas[2][4] += _pecas[pi][pj];
        } else{
            //_pecas[0][4] += _pecas[pi][pj];
        }
        
        if(tab[ci][cj]!=E){ //ci != 0 && cj != 4){
            //_pecas[pi][pj] = _pecas[ci][cj];
            //_pecas[ci][cj] = 0;
        }
        else{
            //_pecas[pi][pj]  = *_jogador=='A'? 1 : -1;
            //_pecas[ci][cj] += *_jogador=='A'? -1 : 1;;
        }
    }
    else{
        if(tab[ci][cj]!=E){ //ci != 0 && cj != 4){
            //_pecas[pi][pj] += _pecas[ci][cj];
            //_pecas[ci][cj] = 0;
        }
        else{
            //_pecas[pi][pj]+=*_jogador=='A'? 1 : -1;;
            //_pecas[ci][cj]+=*_jogador=='A'? -1 : 1;;
        }

        if(tab[pi][pj]==R){
            //addMensagem("Jogador %c move casa(%d,%d) para casa(%d,%d) e atinge uma Roseta.", jogador,ci,cj,pi,pj);
            //addMensagem("Jogador %c joga novamente.",jogador);
            return true;
        }
        else{
            //addMensagem("Jogador %c move casa(%d,%d) para casa(%d,%d)", jogador, ci, cj, pi, pj);
        }
    }
    //*_jogador = *_jogador=='A'? 'B' : 'A';
    return true;
}

int contarPontos(const int _tab[3][8]){
    int i,j;
    int pontos = 0;

    for(i = 0;i<3;i++){
        for(j = 0;j<8;j++){
            if(i!=1 && (j==4 || j==5)) continue;
            if(_tab[i][j] != 0){
                pontos += _tab[i][j] * (i!=1? (j>4?20-j:4-j) : j+5);
            }
        }
    }
    pontos += 30*_tab[0][5];
    pontos += 30*_tab[2][5];

    return pontos;
}

void copyPecas(const int _pecas[3][8], int copy[3][8]){
    int i, j;
    for(i = 0;i<3;i++){
        for(j = 0;j<8;j++){
            copy[i][j] = _pecas[i][j];
        }
    }
}

struct State* newState(const int _pecas[3][8], char jogador, int deep, int mov){
    struct State *state = (struct State*)malloc(sizeof(struct State));
    
    copyPecas(_pecas,state->pecas);
    
    state->jogador = jogador;
    state->pontos = contarPontos(state->pecas);
    state->mov = mov;
    if(deep>0){
        int i, j, quant;
        int ji = jogador=='A'?0:1;
        for(quant = 1; quant<=4;quant++){
            int count = 0;
            for(i =ji; i<=ji+1; i++){
                for(j = 0;j<TAB_LAR;j++){
                    if(testarCasaBot(_pecas,&jogador,i,j,quant)){
                        count++;
                    }
                }
            }

            if(count == 0){
                state->proximos[quant-1] = (struct State**)malloc( 1 * sizeof(struct State*));
                state->proximosCount[quant-1] = 1;
                state->proximos[quant-1][0] = newState(_pecas, jogador=='A'?'B':'A',deep-1,-1);
            }
            else{
                int pPecas[3][8];
                char pJogador = jogador;
                copyPecas(_pecas,pPecas);
                state->proximos[quant-1] = (struct State**)malloc( count * sizeof(struct State*));
                state->proximosCount[quant-1] = 0;//count;
                int c = 0;
                for(i =ji; i<=ji+1; i++){
                    for(j = 0;j<TAB_LAR;j++){
                        if(moverCasaBot(pPecas,&pJogador,i,j,quant)){
                            state->proximos[quant-1][c] = newState(pPecas,pJogador,deep-1, i*8 + j);
                            state->proximosCount[quant-1]++;
                            c++;
                            copyPecas(_pecas,pPecas);
                            pJogador = jogador;
                        }
                    }
                }
                if(count != state->proximosCount[quant-1]){
                    printf("ue?");
                    int c1 = 0;
                    for(i = ji; i<=ji+1; i++){
                        for(j = 0;j<TAB_LAR;j++){
                            if(testarCasaBot(_pecas,&jogador,i,j,quant)){
                                c1++;
                            }
                        }
                    }
                    int c2 = 0;
                    for(i = ji; i<=ji+1; i++){
                        for(j = 0;j<TAB_LAR;j++){
                            if(moverCasaBot(pPecas,&pJogador,i,j,quant)){
                                //state->proximos[quant-1][c] = newState(pPecas,pJogador,deep-1, i*8 + j);
                                //state->proximosCount[quant-1]++;
                                c2++;
                                copyPecas(_pecas,pPecas);
                                pJogador = jogador;
                            }
                        }
                    }
                }
            }
        }
    }
    else{
        state->proximosCount[0] = 0;
        state->proximosCount[1] = 0;
        state->proximosCount[2] = 0;
        state->proximosCount[3] = 0;
        state->proximos[0] = NULL;
        state->proximos[1] = NULL;
        state->proximos[2] = NULL;
        state->proximos[3] = NULL;
    }
    return state;
}

void destroyState(struct State* state){
    int i, j;
    for(i = 0;i<4;i++){
        if(state->proximos[i] != NULL){
            for(j = 0;j<state->proximosCount[i];j++){
                if(state->proximos[i][j]!=NULL){
                    destroyState(state->proximos[i][j]);
                }
            }
            free(state->proximos[i]);
        }
    }

    free(state);
}

void setStateDeep(struct State** state, int deep){
    if(deep>0){
        int i, j;
        bool empty = true;
        for(i = 0;i<4;i++){
            if((*state)->proximosCount[i] != 0){
                empty = false;
                break;
            }
        }

        if(empty){
            struct State copy = **state;
            free(*state);
            *state = newState((copy).pecas,(copy).jogador,deep, (copy).mov);
        }
        else{
            for(i = 0;i<4;i++){
                for(j = 0; j<(*state)->proximosCount[i] ;j++){
                    setStateDeep(&(*state)->proximos[i][j], deep-1);
                }
            }
        }
    }
}

void _printStateTree(FILE*file, struct State* state, int deep){
    
    //fprintf(file,
    int i, j;

    
    fprintf(file,"%*c",deep*2,' ');
    fprintf(file,"state deep %d\n\n",deep);
    //printf("state deep %d\n\n",deep);

    for(i=0;i<3;i++){
        fprintf(file,"%*c",deep*2,' ');
        for(j=0;j<8;j++){
            fprintf(file,"%3d",state->pecas[i][j]);
            //printf("%2d",state->pecas[i][j]);
        }
        fprintf(file,"\n");
        //printf("\n");
    }
    fprintf(file,"\n");

    for(i = 0;i<4;i++){
        for(j = 0; j < state->proximosCount[i]; j++){
            _printStateTree(file,state->proximos[i][j],deep+1);
        }
    }

    //printf("\n");
}

void printStateTree(struct State* state){
    FILE *file= fopen(".\\stateTree.txt","w");
    _printStateTree(file,state,0);
    fclose(file);
}

void printState(struct State* state){
    if(state == NULL)
        state = tabTree;
    int i, j;
    printf("mov: %d\n", state->mov);
    printf("jog: %c\n", state->jogador);
    for(i = 0;i<4;i++){
        printf("p %d: %d | ", i, state->proximosCount[0]);
        for(j = 0;j<state->proximosCount[i];j++){
            int mov = state->proximos[i][j]->mov;
            printf("%d %d, ", mov/8, mov%8);
        }
        printf("\n");
    }
    for(i=0;i<3;i++){
        for(j=0;j<8;j++){
            printf("%3d",state->pecas[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void iniciarTree(char primJogador, int pecas[3][8]){
    if(tabTree != NULL)
        destroyState(tabTree);
    tabTree = newState(pecas,primJogador,SEARCH_DEEP, -1);
}

void moverTree(int ci, int cj, int quant){
    int ind = -1;
    int i;
    int mov = ci*TAB_LAR + cj;
    for(i = 0;i<tabTree->proximosCount[quant-1];i++){
        if(tabTree->proximos[quant-1][i]->mov == mov){
            ind = i;
        }
    }
    if(ind<0){
        setColor(0x0c);
        printf("\n\nNao foi encontrado o state definido por esse movimento\n"
               " movimento(%d,%d), quant(%d)!!",ci,cj,quant);
        system("pause");
        exit(EXIT_FAILURE);
    }

    struct State* nextTabTree = tabTree->proximos[quant-1][ind];
    tabTree->proximos[quant-1][ind] = NULL;
    destroyState(tabTree);
    tabTree = nextTabTree;
    setStateDeep(&tabTree,1);
}

void destroyTree(){
    destroyState(tabTree);
    tabTree = NULL;
}

float getFinalPoints(struct State* state){
    
    int i;
    bool empty = true;
    for(i = 0;i<4;i++){
        if(state->proximosCount[i] != 0){
            empty = false;
            break;
        }
    }

    if(empty){
        return state->pontos;
    }

    int j;
    float fp = 0.f;
    int dir = (state->jogador == 'A'? 1 : -1);

    for(int i = 0;i<4;i++){
        if(state->proximosCount[i] == 0) continue;
        float mPont = getFinalPoints(state->proximos[i][0]);

        for(j = 1;j<state->proximosCount[i];j++){
            float p = getFinalPoints(state->proximos[i][j]);
            if(mPont * dir < p * dir ){
                mPont = p;
            }
        }
        fp += mPont * (i<=1? 0.375 : 0.125);
    }

    return fp;
}

bool Bot_pegarComando(int *ci, int *cj, int quant){
    int i;

    setStateDeep(&tabTree, SEARCH_DEEP);

    int mov = tabTree->proximos[quant-1][0]->mov;
    if(tabTree->proximosCount[quant-1] > 1){
        int dir = (tabTree->jogador == 'A'? 1 : -1);
        float mPont = getFinalPoints(tabTree->proximos[quant-1][0]);

        for(i = 1;i<tabTree->proximosCount[quant-1];i++){
            float p = getFinalPoints(tabTree->proximos[quant-1][i]);
            if(mPont*dir < p*dir ){
                mov = tabTree->proximos[quant-1][i]->mov;
                mPont = p;
            }
        }
    }
    *ci = mov / TAB_LAR;
    *cj = mov % TAB_LAR;

    if(*ci <0  || *cj <0 || *ci >=3 || *cj >=8){
        setColor(0x0c);
        printf("\n\nEsse movimento nao existe!!! (%d, %d, %d)\n",*ci,*cj,quant);
        system("pause");
        exit(EXIT_FAILURE);
    }

    return false;
}