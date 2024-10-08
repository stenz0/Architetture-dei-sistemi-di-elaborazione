#include "./Game.h"
#include "GLCD/GLCD.h"
#include "timer/timer.h"
#include "lpc17xx.h"

uint16_t game_color = 0xCC89;									
uint16_t background_color = 0x59A0;						
uint16_t Hmoves_color = 0xDCC9;								
uint16_t positioned_wall_color = 0xFE00;	
uint16_t error_color = Red;

// funzione che inizializza la matrice di gioco (e tutti gli altri parametri)

void Matrix_Initialization(G* gioco){
	int a, b;
	for ( a = 0; a < 13; a++ ){
		for ( b = 0; b < 13; b++ ){
			gioco->i[a][b] = 0;
		}																									//La tastiera è inizializzata a zero
	}
	gioco->g1.x = 6;
	gioco->g1.y = 0;
	gioco->g2.x = 6; 
	gioco->g2.y = 12;
	gioco->g1.color = Red;
	gioco->g2.color = White;
	gioco->g1.n = 8;
	gioco->g2.n = 8;
	gioco->muro_sospeso.x = -1;
	gioco->muro_sospeso.y = -1;		
	gioco->muro_sospeso.v = 1;
	gioco->muro_sospeso.color = Yellow;
	gioco->i[gioco->g1.x][gioco->g1.y] = 2;
	gioco->i[gioco->g2.x][gioco->g2.y] = 2;							//Le pedine iniziano al centro opposte nella scacchiera e sono identificate con un 1
	gioco->turno = 0;	
	gioco->game_mode = 0;
	gioco->secondi[1] = 0;
	gioco->secondi[0] = 2;
	
}

//funzione che posiziona le pedine all'inizio

void Positionate_player(int n, G* gioco){
	if (n == 1){
		LCD_Draw_Pawn(gioco->g1.x, gioco->g1.y, gioco->g1.color);
	}
	else {
		LCD_Draw_Pawn(gioco->g2.x, gioco->g2.y, gioco->g2.color);
	}
}

// funzione che organizza il cambio di turno

void SetUp_turn( G* gioco ){
	if (gioco->game_mode != 2){
		gioco->game_mode = 0;
		if (gioco->turno == 1) gioco->turno = 2, gioco->mossa_sospesa.color = gioco->g2.color;
		else gioco->turno = 1, gioco->mossa_sospesa.color = gioco->g1.color;
		SetUp_display( gioco );
		SetUp_HMoves( gioco, Hmoves_color );		//set up hypotetical moves; funzione che calcola le mosse possibili per il giocatore di turno
		gioco->secondi[1] = 0;
		gioco->secondi[0] = 2;								//1 sec
		enable_timer(0);
	}
}

//funzione che calcola le Hmoves, le "mosse ipotetiche"

void SetUp_HMoves( G* gioco, uint16_t color ){
	int a, b, c;
	if (gioco->turno == 1){
		a = gioco->g1.x;
		b = gioco->g1.y;
	}
	else if (gioco->turno == 2){
		a = gioco->g2.x;
		b = gioco->g2.y;
	}
	if ((c = Check_down(a, b, gioco ))) LCD_Draw_Pawn (a, b + c, color );
	if ((c = Check_high(a, b, gioco ))) LCD_Draw_Pawn (a, b - c, color );
	if ((c = Check_left(a, b, gioco ))) LCD_Draw_Pawn (a - c, b, color );
	if ((c = Check_right(a, b, gioco ))) LCD_Draw_Pawn (a + c, b, color );
	
	gioco->mossa_sospesa.x = a;
	gioco->mossa_sospesa.y = b;
	gioco->mossa_sospesa.v = -1;
}

// sottolinea il turno di un giocatore circondando il suo nome con un quadrato del suo colore nel suo turno

void SetUp_display( G* gioco ){
	if (gioco->turno == 1){
		LCD_DrawRectangle( 154, 264, 72, 42, background_color );
		LCD_DrawRectangle( 14, 264, 72, 42, gioco->g1.color );
	}
	else {
		LCD_DrawRectangle( 14, 264, 72, 42, background_color );
		LCD_DrawRectangle( 154, 264, 72, 42, gioco->g2.color );
	}
}

//funzione che cerca un posto libero per un muro

void Search_free_place(pos* wall, int i[13][13]){
	if (i[5][5] == 0 && i[5][4] == 0 && i[5][6] == 0) wall->x = 5, wall->y = 5, wall->v = 1;
	else{
		int a, b;
		for (a = 1; a < 13; a += 2 ){
			for (b = 1; b < 13; b+= 2 ){
				if (i[a][b] == 0 && i[a][b+1] == 0 && i[a][b-1] == 0){
					wall->x = a;
					wall->y = b;
					wall->v = 1;
				}
				else if (i[a][b] == 0 && i[a-1][b] == 0 && i[a+1][b] == 0){
					wall->x = a;
					wall->y = b;
					wall->v = 0;
				}
			}
		}
	}
}

// funziona che conferma un muro. Muro_sospeso viene riportato ai valori iniziali e vengono registrati tre 1 nella matrice i[13][13]

void Positionate_wall ( G* gioco ) {
	pos wall = gioco->muro_sospeso;
	if (Check_not_trapW1(gioco)){
		LCD_Draw_Full_Rectangle( wall, positioned_wall_color ) ;
		if (gioco->turno == 1){
						gioco->g1.n -= 1;
						PutChar( 77, 289, gioco->g1.n + 48, game_color, background_color );
						gioco->mossa_32bit = MoveOn32Bit(1, 1, gioco->muro_sospeso.v - 1, gioco->muro_sospeso.y, gioco->muro_sospeso.x);
					}
					else{
						gioco->g2.n -= 1;
						PutChar( 217, 289, gioco->g2.n + 48, game_color, background_color );
						gioco->mossa_32bit = MoveOn32Bit(2, 1, gioco->muro_sospeso.v - 1, gioco->muro_sospeso.y, gioco->muro_sospeso.x);
					}
		gioco->muro_sospeso.x = -1;
		gioco->muro_sospeso.y = -1;
		SetUp_turn( gioco );
	}
	else{
		LCD_Draw_Full_Rectangle( wall, error_color ) ;
		LCD_Draw_Full_Rectangle( wall, wall.color ) ;
	}
}

// funzioni che controllano se si possa andare in quella direzione. Se ritornano 2 è di unaposizione, se 4 di due, se 0 non si può

int Check_down ( int a, int b, G* gioco ){					
	if ( b <= 11 && gioco->i[a][b + 1] == 0 && gioco->i[a][b + 2] == 0) return 2;
	else if ( b < 9 && gioco->i[a][b + 2] == 2 && gioco->i[a][b + 1] == 0 && gioco->i[a][b + 3] == 0) return 4;
	else return 0;
}


int Check_high ( int a, int b, G* gioco ){
	if ( b >= 1 && gioco->i[a][b - 1] == 0 && gioco->i[a][b - 2] == 0) return 2;
	else if ( b > 3 && gioco->i[a][b - 2] == 2 && gioco->i[a][b - 1] == 0 && gioco->i[a][b - 3] == 0) return 4;
	else return 0;
}


int Check_left ( int a, int b, G* gioco ){
	if ( a >= 1 && gioco->i[a - 1][b] == 0 && gioco->i[a - 2][b] == 0) return 2;
	else if ( a > 3 && gioco->i[a - 2][b] == 2 && gioco->i[a - 1][b] == 0 && gioco->i[a - 3][b] == 0) return 4;
	else return 0;
}


int Check_right ( int a, int b, G* gioco ){
	if ( a <= 11 && gioco->i[a + 1][b] == 0 && gioco->i[a + 2][b] == 0 ) return 2;
	else if ( a < 9 && gioco->i[a + 2][b] == 2 && gioco->i[a + 1][b] == 0 && gioco->i[a + 3][b] == 0) return 4;
	else return 0;
}

void debug_function ( G* gioco){			//funzione di debug che ho usato. Nel gioco finito non fa assolutamente nulla
int a, b;
	for (a = 0; a < 13; a += 2){
		for (b = 0; b < 13; b += 2){
			if (gioco->i[a][b] == 1){
				LCD_Draw_Full_Square(a, b, White);
			}
			else {
				LCD_Draw_Full_Square(a, b, Red);
			}
		}
	}
}

void Erase_old_Hmove (pos* mossa){				// cancella la vecchia Hmove, utile se il giocatore ha cambiato idea e ne ha selezionata un'altra
	if (mossa->v != -1){										// oppure ha deciso di piazzare un muro
		int a, b;
		a = mossa->x;
		b = mossa->y;
		if (mossa->v % 4 == 0){
			while (mossa->v > -1){
				mossa->v -= 4;
				b -= 2;
			}
		}
		else if (mossa->v % 4 == 1){
			while (mossa->v > -1){
				mossa->v -= 4;
				b += 2;
			}
		}
		else if (mossa->v % 4 == 2){
			while (mossa->v > -1){
				mossa->v -= 4;
				a -= 2;
			}
		}
		else if (mossa->v % 4 == 3){
			while (mossa->v > -1){
				mossa->v -= 4;
				a += 2;
			}
		}
		a = a/2*QUADRATO + a/2*MURO;
		b = b/2*QUADRATO + b/2*MURO;
		LCD_DrawSquare (a, b, QUADRATO, game_color);
		mossa->v = -1;
	}
}
void Make_new_Hmove ( pos* mossa ){				// si fa una "mossa ipotetica" e lo si fa vedere colorando il bordo del quadrato
	int a, b, v;														// ci si salva il verso della mossa ipotetica in mossa_sospesa
	a = mossa->x;
	b = mossa->y;
	v = mossa->v;
	
	if (mossa->v % 4 == 0){
		while (mossa->v > -1){
			mossa->v -= 4;
			b -= 2;
		}
	}
	else if (mossa->v % 4 == 1){
		while (mossa->v > -1){
			mossa->v -= 4;
			b += 2;
		}
	}
	else if (mossa->v % 4 == 2){
		while (mossa->v > -1){
			mossa->v -= 4;
			a -= 2;
		}
	}
	else if (mossa->v % 4 == 3){
		while (mossa->v > -1){
			mossa->v -= 4;
			a += 2;
		}
	}
	a = a/2*QUADRATO + a/2*MURO;
	b = b/2*QUADRATO + b/2*MURO;
	LCD_DrawSquare (a, b, QUADRATO, mossa->color);
	mossa->v = v;
}

void Confirmed_move (G* gioco){							//mossa confermata, si muove la pedina in base al verso v contenuto in mossa_sospesa, si muove
	pos* mossa = &(gioco->mossa_sospesa);			//la pedina e si riazzera la variabile mossa_sospesa
	int a = mossa->x;
	int b = mossa->y;
	
	if (mossa->v % 4 == 0){
		while (mossa->v > -1){
			mossa->v -= 4;
			b -= 2;
		}
	}
	else if (mossa->v % 4 == 1){
		while (mossa->v > -1){
			mossa->v -= 4;
			b += 2;
		}
	}
	else if (mossa->v % 4 == 2){
		while (mossa->v > -1){
			mossa->v -= 4;
			a -= 2;
		}
	}
	else if (mossa->v % 4 == 3){
		while (mossa->v > -1){
			mossa->v -= 4;
			a += 2;
		}
	}
	LCD_DrawSquare (a/2*QUADRATO + a/2*MURO, b/2*QUADRATO + b/2*MURO, QUADRATO, game_color);
	SetUp_HMoves( gioco,  background_color );
	
	if (gioco->turno == 1){
		LCD_Draw_Full_Square(gioco->g1.x, gioco->g1.y, background_color);
		gioco->i[gioco->g1.x][gioco->g1.y] = 0;
		gioco->g1.x = a;
		gioco->g1.y = b;
		gioco->i[gioco->g1.x][gioco->g1.y] = 2;
		LCD_Draw_Pawn(gioco->g1.x, gioco->g1.y, gioco->g1.color);
		gioco->mossa_32bit = MoveOn32Bit(1, 0, 0, gioco->g1.y, gioco->g1.x);
	}
	
	else {
		LCD_Draw_Full_Square(gioco->g2.x, gioco->g2.y, background_color);
		gioco->i[gioco->g2.x][gioco->g2.y] = 0;
		gioco->g2.x = a;
		gioco->g2.y = b;
		gioco->i[gioco->g2.x][gioco->g2.y] = 2;
		LCD_Draw_Pawn(gioco->g2.x, gioco->g2.y, gioco->g2.color);
		gioco->mossa_32bit = MoveOn32Bit(2, 0, 0, gioco->g2.y, gioco->g2.x);
		}
	Check_win( gioco );
	SetUp_turn( gioco );
}

// controlla se qualcuno ha vinto

void Check_win( G* gioco ){
	if (gioco->turno == 1){
		if (gioco->g1.y == 12){
			gioco->game_mode = 2;
			NVIC_DisableIRQ(EINT1_IRQn);
			NVIC_DisableIRQ(EINT2_IRQn);
			GUI_Text(0, 241, (uint8_t *) "Congratulazioni! G1 ha vinto!  Premere Int0 per rigiocare   ", background_color , gioco->g1.color);
		}
	}
	else{
		if (gioco->g2.y == 0){
			gioco->game_mode = 2;
			NVIC_DisableIRQ(EINT1_IRQn);
			NVIC_DisableIRQ(EINT2_IRQn);
			GUI_Text(0, 241, (uint8_t *) "Congratulazioni! G2 ha vinto!  Premere Int0 per rigiocare   ", background_color , gioco->g2.color);
		}
	}
}
	
int Check_not_trapW1(G* gioco){
	pos wall = gioco->muro_sospeso;
	gioco->i[wall.x][wall.y] = 1;
	if (wall.v == 1) gioco->i[wall.x][wall.y-1] = 1, gioco->i[wall.x][wall.y+1] = 1;
	else if (wall.v == 0) gioco->i[wall.x-1][wall.y] = 1, gioco->i[wall.x+1][wall.y] = 1;
	if (Check_not_trapW2(gioco->i, gioco->g2.x, gioco->g2.y, 0) && (Check_not_trapW2(gioco->i, gioco->g1.x, gioco->g1.y, 12))){
		return 1;
	}
	else {
		if (wall.v == 1) gioco->i[wall.x][wall.y-1] = 0, gioco->i[wall.x][wall.y+1] = 0;
		else if (wall.v == 0) gioco->i[wall.x-1][wall.y] = 0, gioco->i[wall.x+1][wall.y] = 0;
		gioco->i[wall.x][wall.y] = 0;
		return 0;
	}
}

int Check_not_trapW2(int i[13][13], int x, int y, int a){
	int b, c, d = 0;
	Check_not_trapR1(i, x, y, a, 3, &d);
	for (b = 0; b < 13; b++){
		for (c = 0; c < 13; c++){
			if (i[b][c] >= 3) i[b][c] -= 3;
		}
	}			
	return d;
}

void Check_not_trapR1(int i[13][13], int x, int y, int a, int value, int* flag){
    if (a == 0){
        if (i[x][y-1] == 0 && (i[x][y-2] == 0 || i[x][y-2] == 2)  && !(*flag)){
            if (y-2 == a) *flag = 1;
            else i[x][y-2] += value, Check_not_trapR1(i, x, y-2, a, value, flag);
        }
        if (x != 12 && i[x+1][y] == 0 && (i[x+2][y] == 0 || i[x+2][y] == 2) && !(*flag)) i[x+2][y] += value, Check_not_trapR1(i, x+2, y, a, value, flag);
        if (x != 0 && i[x-1][y] == 0 && (i[x-2][y] == 0 || i[x-2][y] == 2) && !(*flag)) i[x-2][y] += value, Check_not_trapR1(i, x-2, y, a, value, flag);
        if (y != 12 && i[x][y+1] == 0 && (i[x][y+2] == 0 || i[x][y+2] == 2) && !(*flag)) i[x][y+2] += value, Check_not_trapR1(i, x, y+2, a, value, flag);
    }
    else {
        if (i[x][y + 1] == 0 && (i[x][y+2] == 0 || i[x][y+2] == 2) && !(*flag)) {
            if (y + 2 == a) *flag = 1;
            else i[x][y + 2] += value, Check_not_trapR1(i, x, y + 2, a, value, flag);
        }
        if (x != 12 && i[x + 1][y] == 0 && (i[x+2][y] == 0 || i[x+2][y] == 2) && !(*flag)) i[x + 2][y] += value, Check_not_trapR1(i, x + 2, y, a, value, flag);
        if (x != 0 && i[x - 1][y] == 0 && (i[x-2][y] == 0 || i[x-2][y] == 2) && !(*flag)) i[x - 2][y] += value, Check_not_trapR1(i, x - 2, y, a, value, flag);
        if (y != 0 && i[x][y - 1] == 0 && (i[x][y-2] == 0 || i[x][y-2] == 2) && !(*flag)) i[x][y - 2] += value, Check_not_trapR1(i, x, y - 2, a, value, flag);
    }
}

int anyMoves(G* gioco){        //controlla se da una posizione data un muro possa fare almeno una mossa ---> serve per non rimanere incastrati
	int flag = 0, t;
	
	for (t = 2; t < 12; t += 2){
						
		if ((gioco->muro_sospeso.x - t < 1) && ((gioco->i[gioco->muro_sospeso.x - t][gioco->muro_sospeso.y] == 0 &&
			gioco->i[gioco->muro_sospeso.x - t][gioco->muro_sospeso.y + 1] == 0 &&
		gioco->i[gioco->muro_sospeso.x - t][gioco->muro_sospeso.y - 1] == 0)
		|| (gioco->i[gioco->muro_sospeso.x - t][gioco->muro_sospeso.y] == 0 &&
			gioco->i[gioco->muro_sospeso.x - t - 1 ][gioco->muro_sospeso.y] == 0 &&
		gioco->i[gioco->muro_sospeso.x - t + 1 ][gioco->muro_sospeso.y] == 0))){
			return 1;
		}
		if ((gioco->muro_sospeso.x + t < 11) && ((gioco->i[gioco->muro_sospeso.x + t][gioco->muro_sospeso.y] == 0 &&
			gioco->i[gioco->muro_sospeso.x + t][gioco->muro_sospeso.y + 1] == 0 &&
		gioco->i[gioco->muro_sospeso.x + t][gioco->muro_sospeso.y - 1] == 0)
		|| (gioco->i[gioco->muro_sospeso.x + t][gioco->muro_sospeso.y] == 0 &&
			gioco->i[gioco->muro_sospeso.x + t - 1 ][gioco->muro_sospeso.y] == 0 &&
		gioco->i[gioco->muro_sospeso.x + t + 1 ][gioco->muro_sospeso.y] == 0))){
			return 1;
		}
		if ((gioco->muro_sospeso.y + t < 11) && ((gioco->i[gioco->muro_sospeso.x][gioco->muro_sospeso.y + t] == 0 &&
			gioco->i[gioco->muro_sospeso.x + 1][gioco->muro_sospeso.y + t] == 0 &&
		gioco->i[gioco->muro_sospeso.x - 1][gioco->muro_sospeso.y + t] == 0)
		|| (gioco->i[gioco->muro_sospeso.x][gioco->muro_sospeso.y + t] == 0 &&
			gioco->i[gioco->muro_sospeso.x][gioco->muro_sospeso.y + t - 1 ] == 0 &&
		gioco->i[gioco->muro_sospeso.x][gioco->muro_sospeso.y + t + 1 ] == 0))){
			return 1;
		}
		if ((gioco->muro_sospeso.y - t > 1) && ((gioco->i[gioco->muro_sospeso.x][gioco->muro_sospeso.y - t] == 0 &&
			gioco->i[gioco->muro_sospeso.x + 1][gioco->muro_sospeso.y - t] == 0 &&
		gioco->i[gioco->muro_sospeso.x - 1][gioco->muro_sospeso.y - t] == 0)
		|| (gioco->i[gioco->muro_sospeso.x][gioco->muro_sospeso.y - t] == 0 &&
			gioco->i[gioco->muro_sospeso.x][gioco->muro_sospeso.y - t - 1 ] == 0 &&
		gioco->i[gioco->muro_sospeso.x][gioco->muro_sospeso.y - t + 1 ] == 0))){
			return 1;
		}
	}
	return 0;
}




