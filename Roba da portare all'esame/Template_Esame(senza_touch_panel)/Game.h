#ifndef __GAME_H 
#define __GAME_H

#include "LPC17xx.h"
#define QUADRATO 30
#define MURO 5


typedef struct {
	int x, y; 																		//struct che racchiude informazioni sulla posizione di una pedina o muro (in coordinate)
	uint16_t color;
	int v, n;
} pos;

typedef struct {
	int i[13][13];															//matrice che rappresenta la scacchiera 7x7 contando anche gli spazi
	pos g1, g2, muro_sospeso, mossa_sospesa;		//posizioni del g1 e del g2, strutture per gestione muro e mossa
	int turno;																	//turno = 1 tocca al g1, turno = 2 al g2
	int game_mode;															// 0 il giocatore si muove, 1 si posiziona il muro
	int secondi[2];
	unsigned int mossa_32bit;
} G;

void Matrix_Initialization( G* gioco );
void Positionate_player( int n, G* gioco );
void SetUp_turn( G* gioco );
void SetUp_HMoves( G* gioco, uint16_t color );
void SetUp_display( G* gioco );
void Search_free_place(pos* wall, int i[13][13]);
void Positionate_wall ( G* gioco ); 
int Check_down ( int a, int b, G* gioco );
int Check_high ( int a, int b, G* gioco );
int Check_left ( int a, int b, G* gioco );
int Check_right ( int a, int b, G* gioco );
void debug_function (G* gioco);
void Erase_old_Hmove ( pos* mossa );
void Make_new_Hmove ( pos* mossa );
void Confirmed_move ( G* gioco );
void Check_win( G* gioco );
int Check_not_trapW1(G* gioco);
int Check_not_trapW2(int i[13][13], int x, int y, int a);
void Check_not_trapR1(int i[13][13], int x, int y, int a, int value, int* flag);
extern unsigned int MoveOn32Bit (int player_id, int move_wall, int v_h, int y, int x);
int anyMoves(G* gioco);


#endif
