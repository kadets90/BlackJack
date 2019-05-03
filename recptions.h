#ifndef recptions

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#define MAX_PLAYERS 4         // number of maximum players
#define MIN_BARALHOS 4        // numero minimo de baralhos
#define MAX_BARALHOS 8        // numero maximo de baralhos
#define MIN_PLAYERS 1         // numero minimo de jogadores
#define MIN_APOSTA 2          // aposta minima
#define MAX_MONEY 500         // dinheiro maximo
#define MIN_MONEY 10          // dinheiro minimo
#define MAX_POINTS 21         // limite de pontos
#define STRATEGY_LINE 17      // linhas de parametros da estrategia da EA
#define STRATEGY_COLUMN 10    // linhas de parametros da estrategia da EA
#define MINI_STRING 20        // for small strings

no_jogador *receber_parametros();
void mudar_aposta(no_jogador *head_jogador, int *num_player);
void leitura_estrategia(int **estrategia);
no_cartas *retirar_carta(no_cartas **head, int *num_bara, int *div);
no_jogador *adicionar_jogador(no_jogador *head_jogador, int *num_player);
no_jogador *inicializar_jogador(no_jogador *new_player);
no_jogador *nova_estrutura_vazia();
int posicao_jogador(int quit);
int alterar_delay(int delay_EA, bool sinal);

#endif
