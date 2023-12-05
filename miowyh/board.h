#ifndef BOARDY_H
#define BOARDY_H
#include "move.h"
#include <cstdio>

static const int N = 4;
static const int INFNTY =     999999999;
static const int MAXSCORE =   99999999;
static inline int negScr(int s) {return MAXSCORE-(s);}  

//  Y-board       N cells per side    N(N+1)/2  total cells
//  fat board, aka. guarded board with GUARDS extra rows/cols,
//  allows fixed offsets for nbrs @ dist 1 or 2, 
//  e.g. below with GUARDS = 2
//          . . g g g g g g g 
//           . g g g g g g g g 
//            g g * * * * * g g 
//             g g * * * * g g . 
//              g g * * * g g . . 
//               g g * * g g . . . 
//                g g * g g . . . . 
//                 g g g g . . . . . 
//                  g g g . . . . . . 

static const int GUARDS = 2; // must be at least one
static const int Np2G = N+2*GUARDS; //for one whole side
//changed
static const int TotalCells = N*N; //total cell ffor play 
static const int TotalGBCells = Np2G*Np2G; //total cell

static const int BRDR_NIL   = 0; // 000   border values, used bitwise
static const int BRDR_TOP   = 1; // 0001
static const int BRDR_L     = 2; // 0010
static const int BRDR_R     = 4; // 0100
static const int BRDR_BOT   = 8; // 1000

//not use
static const int BRDR_W   = 10; // 110
static const int BRDR_B   = -2; // 001
//static const int BRDR_ALL   = 7; // 111

static const int EMP = 0; // empty cell
static const int BLK = 1; // black "
static const int WHT = 2; // white "
static const int GRD = 3; // guard "
static const int TMP = 4; // unitialized: none of the above

static const char EMP_CH = '.';
static const char BLK_CH = 'b';
static const char WHT_CH = 'w';

static inline int nx(int x) {return x-1;}  // index of x:    black,white are 1,2
static inline int opt(int x) {return 3^x;} // opponent of x

char emit(int value) ;       // EMP, BLK, WHT
void emitString(int value) ; // empty, black, white
void prtLcn(int psn);        // a5, b13, ...
static inline int fatten(int r,int c) {return Np2G*(r+GUARDS)+(c+GUARDS);}
static inline int board_row(int lcn) {return (lcn/Np2G)-GUARDS;}
static inline int board_col(int lcn) {return (lcn%Np2G)-GUARDS;}
static inline int  numerRow(int psn) {return psn/Np2G - (GUARDS-1);}
static inline char alphaCol(int psn) {return 'a' + psn%Np2G - GUARDS;}
static inline bool near_edge(int lcn) { 

// changed


//static inline bool near_edge(int lcn,int d) { 
// near_edge(lcn,0) true if lcn on edge
// near_edge(lcn,1) true if lcn 1-away from edge
  int r = board_row(lcn);
  int c = board_col(lcn);
  //return ((d==r)||(d==c)||(N==r+c+d+1));
  return ((0==r)||(0==c)||(N==r+1)||(N==c+1));
}

static inline bool has_win(int bd_set, int s) {
    if (s == BLK){
      printf("tell black wins?\n");
      bool win = (bd_set==11)||(bd_set==9)||(bd_set==13)||(bd_set==15);
      if (win){
        printf("black wins\n");
      }else{
        //printf("%d \n",bd_set);
        printf("black not \n");
      }
        return win;
    }
    if (s == WHT){
      printf("tell white wins\n?");
      bool win = (bd_set==6)||(bd_set==7)||(bd_set==14)||(bd_set==15);
      if (win){
        printf("wht wins\n");
      }else{
        //printf("%d \n",bd_set);
        printf("wht not \n");
      }
        return win;
    }
    return false;
}

struct Board {                 // for each cell ...
  int board[TotalGBCells];     //   b, w, empty ?
  int p    [TotalGBCells];     //   parents, for union-find cnnctvty
  int brdr [TotalGBCells];     //   brdrs cnnctd to ?
  int reply[2][TotalGBCells];  //   miai reply ? (if any)
  //int hist[TotalCells];        //   lcns   of all moves (permutation of lcns) 
  //int clr[TotalCells];         //   colors of all moves 
  //int numMvs;                  //   number of moves made so far.. with Moves, gives history

  void init();
  void zero_connectivity(int s, bool removeStone = true);
  void set_miai    (int s, int m1, int m2);
  void release_miai(Move mv);
  bool not_in_miai (Move mv);
  void put_stone   (Move mv);
  void remove_stone(int lcn);
  int  move(Move mv, bool useMiai, int& brdset); // ret: miaiMove
  int  moveMiaiPart(Move mv, bool useMiai, int& brdset, int cpt); // miai part of move
  void YborderRealign(Move mv, int& cpt, int c1, int c2, int c3);
  int num(int cellKind);
  void show();
  void showMi(int s);
  void showBothMi();
  void showMiaiPar();
  void showP();
  void showBr(); //showBorder connectivity values
  void showAll();
  Board(); // constructor
} ;

struct Playout { // aggregate data, over all playouts for a fixed position...
  int Avail[TotalCells];  // locations of available cells
  int numAvail;           // number of available cells
  int MP[TotalCells];     // locations of mustplay cells
  int mpsz;               // mustplay size
  int wins[2];            // black, white
  int cellWins[2][TotalGBCells];
  int AMAF[2][TotalGBCells];
  int minwinlen[2];
  int win_length[2]; // sum (over wins) of stones in win
  Playout(Board& B); // constructor

  void single_playout(int& turn, int& moves_to_winner, bool useMiai); //
  void listLive(int num);  // cell is live if winning move in some simulation
  int eval(int st, int maxSims);    // use data, return score for player st
  Board& B;
} ;

//extern const int Nbr_offsets[NumNbrs+1] ;   // now in move.h
//extern const int Bridge_offsets[NumNbrs] ;  // now in move.h

static const int RHOMBUS = 0;
static const int TRI   = 1;
void shapeAs(int shape, int X[]);
void showYcore(int X[]) ;  // simple version of Board.show
void showBothYcore(int X[], int Y[]) ; // two at a time
void display_nearedges();
#endif
