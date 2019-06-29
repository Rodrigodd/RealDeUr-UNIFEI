#ifndef __REAL_BOT_H__
#define __REAL_BOT_H__

#include <stdbool.h>

#define R 'R'
#define E 'E'
#define S 'S'
#define V ' '

#define TAB_LAR 8
#define TAB_ALT 3

#define SEARCH_DEEP 4

extern char tab[TAB_ALT][TAB_LAR];

struct State{
    int pecas[3][8];
    char jogador;
    int pontos;
    int mov;
    struct State **proximos[4];
    int proximosCount[4];
};

//typedef enum { false, true} bool;
//typedef enum { FBC, DFE} BEC;


void setColor(unsigned char color);

bool Bot_pegarComando(int *ci, int *cj, int quant);
void printStateTree(struct State* state);
void printState(struct State* state);
struct State* newState(const int _pecas[3][8], char jogador, int deep, int mov);
void iniciarTree(char primJogador, int pecas[3][8]);
void moverTree(int ci, int cj, int quant);
void destroyTree();


#endif /* __REAL_BOT_H__ */