#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "blackjack.h"
#include "clean.h"
#include "recptions.h"


void clean_estrategia(int **estrategia){

  for(int x=0; x<STRATEGY_LINE; ++x){
      free(estrategia[x]);
  }
  free(estrategia);
}

void clean_jogadores(no_jogador *head_jogador){

  no_jogador *aux=head_jogador;
  no_jogador *tmp;

  while(aux!=NULL){
    tmp=aux;
    aux=aux->next;
    free(tmp);
  }
}

void clean_pilha_casa(est_casa *pont_casa){

  no_cartas *aux=pont_casa->topo;
  no_cartas *tmp;

  while(aux!=NULL){
    tmp=aux;
    aux=aux->next;
    free(tmp);
  }

  pont_casa->num_cartas=0;
  pont_casa->topo=NULL;
  pont_casa->fim=NULL;

}

void clean_baralho(no_cartas *head){

  no_cartas *aux=head;
  no_cartas *tmp;

  while(aux!=NULL){
    tmp=aux;
    aux=aux->next;
    free(tmp);
  }

}

void clean_pilha_jogador(no_jogador *head_jogador){

  no_jogador *aux_jogador=head_jogador;
  no_cartas *tmp;
  no_cartas *aux_cartas;


  while(aux_jogador!=NULL){

    aux_cartas=aux_jogador->payload.topo;

    while(aux_cartas!=NULL){
      tmp=aux_cartas;
      aux_cartas=aux_cartas->next;
      free(tmp);
    }
    aux_jogador->payload.num_cartas=0;
    aux_jogador->payload.topo=NULL;
    aux_jogador->payload.fim=NULL;
    aux_jogador=aux_jogador->next;
  }
}

