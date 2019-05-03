#ifndef blackjack

#define NAME_SIZE 9           // caracteres maximos para o nome dos jogadores
#define NAIPE 3               // numero que representa o ultimo naipe
#define CODIGO 12             // numero que representa a ultima carta de cada naipe
#define HIT_CODE 1            // codigo da matriz correspondete ao hit das EA
#define STAND_CODE 0          // codigo da matriz correspondete ao stand das EA
#define DOUBLE_CODE 2         // codigo da matriz correspondete ao double das EA
#define SURRENDER_CODE 3      // codigo da matriz correspondete ao surrender das EA


typedef struct{

  int codigo;
  int naipe;

}carta;

typedef struct no_cartas{

  carta payload;
  struct no_cartas *next;
  struct no_cartas *prev;

}no_cartas;

typedef struct{

  char nome[NAME_SIZE];
  int dinheiro;
  int dinheiro_inicial;
  int aposta;
  int aposta_inicial;
  int pontos;
  int jganhos;
  int jperdidos;
  int jempatados;
  char tipo[NAME_SIZE]; //EA ou HU
  bool active;
  bool soft;  // Indica se o As vale 1 ou 11
  int num_cartas;
  int contagem;  // contagem para a aplicar a estrategia Hi-Low
  int state; //Bust(2), BlackJack(1), 21 pontos(3), surrender(4), etc
  no_cartas *topo;
  no_cartas *fim;

}jogador;

typedef struct no_jogador{

  jogador payload;
  struct no_jogador *next;

}no_jogador;

typedef struct{

  int pontos;
  int jganhos;
  int jperdidos;
  int jempatados;
  int num_cartas;
  int state; //Bust, BlackJack, etc
  no_cartas *topo;
  no_cartas *fim;

}est_casa;

#endif
