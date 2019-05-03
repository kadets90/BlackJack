/*******************************************************
**Autores: David Ribeiro e Rafael Forte
**Data: 20/05/2016
**Projeto: Jogo BlackJack
********************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "blackjack.h"
#include "functional.h"
#include "clean.h"
#include "recptions.h"
#include "graphic.h"

int main(int argc, char *argv[]){

  int x, y;
  int *num_bara=&x;                                   // numero de baralhos
  int *num_players=&y;                                // numero de jogadores
  int quit=0;                                         // sair?
  no_cartas *head=NULL;                               // inicio do baralho
  no_cartas *carta_retirada;                          // carta retirada aliatoriamente do baralho
  srand(time(NULL));                                  // numero aleatorio
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  TTF_Font *serif = NULL;
  SDL_Surface *cards[MAX_DECK_SIZE+1], *imgs[2];
  int delay = 300;
  SDL_Event event;
  est_casa casa;                                      // estrutura com os dados da casa
  est_casa *pont_casa=&casa;                          // ponteiro da estrutura casa
  pont_casa->topo=NULL;                               // ponteiro para o topo das cartas da casa
  pont_casa->num_cartas=0;                            // numero de cartas da casa
  int divisor;                                        // numero pelo qual o numero aleatorio e dividido
  int *div=&divisor;                                  // ponteiro para passar o divisor por referencia
  int **estrategia=NULL;                              // matriz com os dados da estrategia
  int option=0;                                       // decisao tomada pela EA
  int delay_EA =1000;                                 // delay das jogadas da EA
  bool sinal;

  estrategia=(int**)malloc(STRATEGY_LINE*sizeof(int*)); //dimensao de cada coluna - numero de linhas

  for(int z=0; z<STRATEGY_LINE; ++z){
    estrategia[z]=(int*)malloc(STRATEGY_COLUMN*sizeof(int)); //ponteiros para o inicio das colunas
  }

  no_jogador *head_jogador=receber_parametros(num_bara, num_players, argv);  // inicializar jogadores e numero de baralhos
  leitura_estrategia(estrategia); // leitura da estrategia do ficheiro para a matriz

  // initialize graphics
  InitEverything(WIDTH_WINDOW, HEIGHT_WINDOW, &serif, imgs, &window, &renderer);
  // loads the cards images
  LoadCards(cards);

  no_jogador *aux_player;  // ponteiro auxiliar para os jogadores

  head=carregar_baralhos(*num_bara, head);  // criaçao do baralho

  *div=(*num_bara)*52;  // inicializar o divisor

  do{

    // add a delay
    SDL_Delay(delay);

    aux_player=head_jogador;  // primeiro jogador

    for(int x=0; aux_player!=NULL; ++x){

      // distribuiçao das cartas pelos jogadores ATIVOS

      if(((x%2==0)&&(x!=0))||(aux_player->payload.active==false)){
        pontos(aux_player);
        aux_player=aux_player->next;
        if(aux_player==NULL){
          break;
        }
        if(aux_player->payload.active==false){
          ++x;
          continue;
        }
      }
      // retirar carta
      carta_retirada=retirar_carta(&head, num_bara, div);
      // contagem das cartas por parrte da EA
      contegem_cartas(head_jogador, carta_retirada, div);
      // atribuila ao jogador
      distribuir_cartas(carta_retirada, aux_player);
    }

    aux_player=head_jogador;

    for(int s=0; s<2; ++s){
      // dar cartas para a casa
      carta_retirada=retirar_carta(&head, num_bara, div);
      // contagem das cartas por parrte da EA
      contegem_cartas(head_jogador, carta_retirada, div);
      cartas_para_casa(carta_retirada, pont_casa);
    }

    pontos_casa(pont_casa);

    // ciclo de jogo
    for(int player=0; player<*num_players&&quit!=1; ++player){

      // ciclo de jogo para a EA
      while(quit==0&&*(aux_player->payload.tipo)=='E'&&*(aux_player->payload.tipo+1)=='A'&&aux_player->payload.active==1){

        option=decision(estrategia, pont_casa, aux_player); // escolha da opçao

        // render game table
        RenderTable(serif, imgs, renderer, head_jogador, player, pont_casa, num_players, div);
        // render house cards
        RenderHouseCards(cards, renderer, pont_casa);
        // render player cards
        RenderPlayerCards(cards, renderer, head_jogador, num_players);
        // render in the screen all changes above
        SDL_RenderPresent(renderer);
        // add a delay
        SDL_Delay(delay_EA);

        switch (option){
          case STAND_CODE:              // stand
            quit=2;
            break;

          case DOUBLE_CODE:              // double
            carta_retirada=retirar_carta(&head, num_bara, div);
            // contagem das cartas por parrte da EA
            contegem_cartas(head_jogador, carta_retirada, div);
            distribuir_cartas(carta_retirada, aux_player);
            pontos(aux_player);
            if(aux_player->payload.state!=2){
              aux_player->payload.state=5;
            }
            quit=2;
            aux_player->payload.aposta*=2;
            break;

          case SURRENDER_CODE:              // surrender
            quit=2;
            aux_player->payload.aposta/=2;
            aux_player->payload.state=4;
            break;

          case HIT_CODE:              // hit
            carta_retirada=retirar_carta(&head, num_bara, div);
            // contagem das cartas por parrte da EA
            contegem_cartas(head_jogador, carta_retirada, div);
            distribuir_cartas(carta_retirada, aux_player);
            pontos(aux_player);
            break;

          default:
            break;
        }

        if(aux_player->payload.state!=0){
          quit=2;             // Bust?? BlackJack?? 21?? surrender?? ok....
        }

        while(SDL_PollEvent(&event)){
          if(event.type==SDL_QUIT){
            quit=1;        // quit the program
            break;
          }
          else if(event.type==SDL_KEYDOWN){
	    switch (event.key.keysym.sym){
	      case SDLK_UP:              // aumentar o tempo
                sinal=1;
                delay_EA=alterar_delay(delay_EA, sinal);
                break;

	      case SDLK_DOWN:              // diminuir o tempo
                sinal=0;
                delay_EA=alterar_delay(delay_EA, sinal);
                break;

	      default:
	        break;
	    }
          }
        }
      }

      // ciclo de jogo para os HU
      while(quit==0){
          // while there's events to handle
        while(SDL_PollEvent(&event)&&aux_player->payload.state==0&&aux_player->payload.active==1){
          if(event.type==SDL_QUIT){
            quit=1;        // quit the program
          }
          else if(event.type==SDL_KEYDOWN){
	    switch (event.key.keysym.sym){
	      case SDLK_s:              // stand
                quit=2;
                break;

	      case SDLK_d:              // double
                if(aux_player->payload.num_cartas!=2||aux_player->payload.dinheiro<aux_player->payload.aposta*2){
                  break;
                }
                carta_retirada=retirar_carta(&head, num_bara, div);
                // contagem das cartas por parrte da EA
                contegem_cartas(head_jogador, carta_retirada, div);
                distribuir_cartas(carta_retirada, aux_player);
                pontos(aux_player);
                if(aux_player->payload.state!=2){
                  aux_player->payload.state=5;
                }
                quit=2;
                aux_player->payload.aposta*=2;
                break;

	      case SDLK_r:              // surrender
                if(aux_player->payload.num_cartas!=2){
                  break;
                }
                quit=2;
                aux_player->payload.aposta/=2;
                aux_player->payload.state=4;
                break;

	      case SDLK_h:              // hit
                carta_retirada=retirar_carta(&head, num_bara, div);
                // contagem das cartas por parrte da EA
                contegem_cartas(head_jogador, carta_retirada, div);
                distribuir_cartas(carta_retirada, aux_player);
                pontos(aux_player);
                break;

	      default:
	        break;
	    }
          }
        }

        if(aux_player->payload.state!=0){
          quit=2;             // Bust?? BlackJack?? 21?? surrender?? ok....
        }

        if(aux_player->payload.active==false){
          break;              // nao esta ativo, passa ao proximo
        }

        // render game table
        RenderTable(serif, imgs, renderer, head_jogador, player, pont_casa, num_players, div);
        // render house cards
        RenderHouseCards(cards, renderer, pont_casa);
        // render player cards
        RenderPlayerCards(cards, renderer, head_jogador, num_players);
        // render in the screen all changes above
        SDL_RenderPresent(renderer);
      }

      // proximo jogador
      aux_player=aux_player->next;

      // confirma se é para sair ou nao
      quit%=2;

    }

    // jogada da casa
    while(pont_casa->pontos<16||pont_casa->num_cartas==1){
      pontos_casa(pont_casa);
      if(pont_casa->pontos>15){
        break;
      }
      // retirar uma carta
      carta_retirada=retirar_carta(&head, num_bara, div);
      // contagem das cartas por parrte da EA
      contegem_cartas(head_jogador, carta_retirada, div);
      // atribuir a carta à casa
      cartas_para_casa(carta_retirada, pont_casa);
    }

    if(quit==0){
      atualizar_dinheiro(head_jogador, num_players, pont_casa);
    }

    // ciclo entre jogos
    while(quit==0){
          // while there's events to handle
      while(SDL_PollEvent(&event)){
        if(event.type==SDL_QUIT){
          quit=1;        // quit the program
        }
        else if(event.type==SDL_KEYDOWN){
          switch (event.key.keysym.sym){

            case SDLK_a:            // adicionar jogador
              printf("1\n");
              head_jogador=adicionar_jogador(head_jogador, num_players);
              break;

            case SDLK_b:            // alterar o valor da aposta
              mudar_aposta(head_jogador, num_players);
              break;

	    case SDLK_q:            // sair do jogo
              clean_pilha_jogador(head_jogador);
              clean_pilha_casa(pont_casa);
              registo(head_jogador);
              quit=1;
              break;

	    case SDLK_n:            // novo jogo
              clean_pilha_jogador(head_jogador);
              clean_pilha_casa(pont_casa);
              quit=2;
              break;

	    case SDLK_UP:              // aumentar o tempo
              sinal=1;
              delay_EA=alterar_delay(delay_EA, sinal);
              break;

	    case SDLK_DOWN:              // diminuir o tempo
              sinal=0;
              delay_EA=alterar_delay(delay_EA, sinal);
              break;

	    default:
	      break;
	  }
        }
      }
      // render game table
      RenderTable(serif, imgs, renderer, head_jogador, 4, pont_casa, num_players, div);
      // render house cards
      RenderHouseCards(cards, renderer, pont_casa);
      // render player cards
      RenderPlayerCards(cards, renderer, head_jogador, num_players);
      // render in the screen all changes above
      SDL_RenderPresent(renderer);
    }

    // confirmar se é para sair
    quit%=2;

    atualizar_apostas(head_jogador, div);

  }while(quit!=1);

  // free memory allocated for players and deck's cards
  clean_baralho(head);
  clean_jogadores(head_jogador);
  clean_estrategia(estrategia);

  // free memory allocated for images and textures and close everything including fonts
  UnLoadCards(cards);
  TTF_CloseFont(serif);
  SDL_FreeSurface(imgs[0]);
  SDL_FreeSurface(imgs[1]);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

 return 0;
}

