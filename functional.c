#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "blackjack.h"
#include "functional.h"


no_cartas *carregar_baralhos(int num, no_cartas *head){

  // carrega um mega baralho ordenada por naipes e codigos

  no_cartas *aux; 
  no_cartas *new_node;

  if(num<1){  // minimo de baralhos
    printf("Erro!\n");
    exit(0);
  }

  for(int x=0; x<num; ++x){          // para cada baralho
    for(int y=0; y<=NAIPE; ++y){     // para cada naipe
      for(int z=0; z<=CODIGO; ++z){  // para cada codigo

        new_node=(no_cartas*)malloc(sizeof(no_cartas));  // criaçao da nova carta

        if(new_node==NULL){
          printf("Falhou a criaçao de memoria\n");
          exit(0);
        }

        new_node->payload.naipe=y;  // atribuiçao do naipe
        new_node->payload.codigo=z; // atribuiçao do codigo

        if(head==NULL){  // primeira carta
          head=new_node;
          head->next=NULL;
          head->prev=NULL;
          continue;
        }

        aux=head;

        // primeira posiçao livre
        while(aux->next!=NULL){
          aux=aux->next;
        }

        // inserir a carta no fim da baralho
        aux->next=new_node;
        new_node->next=NULL;
        new_node->prev=NULL;

      }
    }
  }
  
  return head;
}

void print_cartas(no_cartas *head){

  // funçao auxiliar que imprime a lista de cartas no ecra, util para a correçao de erros

  no_cartas *aux;
  aux=head;

  while(aux!=NULL){

    // escolha do codigo da carta

    if((*aux).payload.codigo<9){
      printf("%i", (*aux).payload.codigo+2);
    }
    else if((*aux).payload.codigo==9){
      printf("J");
    }
    else if((*aux).payload.codigo==10){
      printf("Q");
    }
    else if((*aux).payload.codigo==11){
      printf("K");
    }
    else if((*aux).payload.codigo==12){
      printf("A");
    }else{
      printf("Erro na carta!\n");
      exit(0);
    }
    
    // escolha do naipe

    if((*aux).payload.naipe==0){
      printf("  Paus\n");
    }
    else if((*aux).payload.naipe==1){
      printf("  Ouros\n");
    }
    else if((*aux).payload.naipe==2){
      printf("  Copas\n");
    }
    else if((*aux).payload.naipe==3){
      printf("  Espadas\n");
    }else{
      printf("Erro na naipe!\n");
      exit(0);
    }
    aux=aux->next;
  }

}

no_cartas *retirar_carta(no_cartas **head, int *num_bara, int *div){

  // retira uma carta ao baralho e retorna essa carta

  no_cartas *aux1;
  no_cartas *aux2;
  int num_random=0;


  // caso o baralho tenha chegado ao fim, carrega-se um novo
  if(*head==NULL){
    (*head)=carregar_baralhos(*num_bara, *head);
    *div=(*num_bara)*52;
  }

  aux1=(*head);
  aux2=aux1;

  num_random=rand()%(*div); // gerar um numero aleatorio dentro do intervalo desejado

  // primeira carta
  if(num_random==0){
    (*head)=(*head)->next;
    aux1->next=NULL;
  }
  else{  // resto das cartas

    while(num_random!=0){
      aux1=aux1->next;
      --num_random;
      if(num_random==1){
        aux2=aux1;
      }
    }

    aux2->next=aux1->next;

    aux1->next=NULL;
  }

  --*div; // decremento do divisor

  return aux1;
}

void distribuir_cartas(no_cartas *carta_retirada, no_jogador *aux){

  // atribuiçao da carta retirada a um jogador

  no_cartas *transporte=NULL;
  no_cartas *aux1=NULL;

  if(aux->payload.topo!=NULL){  // se for a primeira carta
    transporte=aux->payload.topo;
    aux1=carta_retirada;
  }
  else{                         // quando o jogador ja possui pelo menos uma carta
    aux->payload.fim=carta_retirada;
  }

  ++aux->payload.num_cartas;     // incremento do numero de cartas do jogadro
  aux->payload.topo=carta_retirada;  // carta retirada para o top da pilha
  if(transporte!=NULL){
    transporte->prev=aux1;   // ligaçao das outras cartas à do topo 
  }
  aux->payload.topo->next=transporte;  // ligaçao da carta do topo às outras
}

void cartas_para_casa(no_cartas *carta_retirada, est_casa *pont_casa){

  // atribuiçao de cartas para a casa

  no_cartas *transporte=NULL;
  no_cartas *aux1=NULL;

  if(pont_casa->topo!=NULL){    // se for a primeira
    transporte=pont_casa->topo;
    aux1=carta_retirada;
  }
  else{                         // quando a casa ja possui pelo menos uma carta
    pont_casa->fim=carta_retirada;
  }

  pont_casa->topo=carta_retirada;  // carta retirada para o topo da pilha
  if(transporte!=NULL){
    transporte->prev=aux1;  // ligaçao das outras cartas à do topo
  }
  pont_casa->topo->next=transporte;  // ligaçao da carta do topo às outras

}

void atualizar_dinheiro(no_jogador *aux, int *num_jogadores, est_casa *pont_casa){

  //atualizaçao do valor do dinheiro segundo as regras

  while(aux!=NULL){
    if(aux->payload.active==true){
      if( ((((aux->payload.pontos>pont_casa->pontos)||(pont_casa->state==2)) && (aux->payload.state!=2)) || ((aux->payload.state==1) && (pont_casa->state!=1)))&&(aux->payload.state!=4) ){

        aux->payload.dinheiro+=aux->payload.aposta;  //Jogo ganho
        ++aux->payload.jganhos;

        if((aux->payload.state==1) && (pont_casa->state!=1)) {
          aux->payload.dinheiro+=aux->payload.aposta/2;  //BlackJack
        }
      }

      else if( ((((aux->payload.pontos<pont_casa->pontos)||(aux->payload.state==2)) && (pont_casa->state!=2)) || ((pont_casa->state==1)&&(aux->payload.state!=1)))||(aux->payload.state==4) ){
        aux->payload.dinheiro-=aux->payload.aposta;  //Jogo perdido
        ++aux->payload.jperdidos;
      }

      else{
        ++aux->payload.jempatados;
      }
    }
    // proximo jogador
    aux=aux->next;
  }
}

void pontos(no_jogador *aux){

  //calculo dos pontos dos jogadores
  no_cartas *check=aux->payload.topo;

  aux->payload.pontos=0;
  aux->payload.soft=false;

  while(check!=NULL){
    if((check->payload.codigo<9)&&(check->payload.codigo>=0)){
      aux->payload.pontos+=check->payload.codigo+2;  //carta 2 a 10
    }

    else if((check->payload.codigo>8)&&(check->payload.codigo<12)){
      aux->payload.pontos+=10;  //carta J, Q e K
    }

    else if(check->payload.codigo==12){
      aux->payload.pontos+=1;  // carta A com valor 1
    }
    else{
      printf("Erro nos pontos!\n");
      exit(0);
    }
    // proxima carta
    check=check->next;
  }

  // voltar ao inicio das cartas
  check=aux->payload.topo;

  while(check!=NULL){
    if((check->payload.codigo==12)&&(aux->payload.pontos<=11)){
      aux->payload.pontos+=10;  //escolha para o valor do A, se 1 ou 11
      aux->payload.soft=true;
      break;
    }
    // proxima carta
    check=check->next;
  }

  if(aux->payload.pontos==21&&aux->payload.num_cartas==2){
    aux->payload.state=1;  //quando o jogador tem BlackJack é impedido de jogar mais
  }

  else if(aux->payload.pontos==21&&aux->payload.num_cartas!=2){
    aux->payload.state=3;  //quando o jogador tem 21 pontos é impedido de jogar mais
  }

  else if(aux->payload.pontos>21){
    aux->payload.state=2;  //quando ocorre bust o jogador perde
  }
  else{
    aux->payload.state=0;
  }
}

void pontos_casa(est_casa *pont_casa){

  //calculo dos pontos da casa
  no_cartas *check=pont_casa->fim;
  int x=0;   // controle devido a carta virada para baixo
  int y=0;

  pont_casa->pontos=0;

  // sempre que se calculam os pontos da mesa é adicionada uma carta à mesa
  ++pont_casa->num_cartas;

  while(check!=NULL&&x<pont_casa->num_cartas){
    if((check->payload.codigo<9)&&(check->payload.codigo>=0)){
      pont_casa->pontos+=check->payload.codigo+2;  //carta 2 a 10
    }

    else if((check->payload.codigo>8)&&(check->payload.codigo<12)){
      pont_casa->pontos+=10;  //carta J, Q e K
    }

    else if(check->payload.codigo==12){
      pont_casa->pontos+=1;  // carta A com valor 1
    }
    else{
      printf("Erro nos pontos da casa!\n");
      exit(0);
    }
    // proxima carta
    check=check->prev;
    ++x;
  }

  check=pont_casa->fim;

  while(check!=NULL&&y<pont_casa->num_cartas){
    if((check->payload.codigo==12)&&(pont_casa->pontos<=11)){
      pont_casa->pontos+=10;  //escolha para o valor do A, se 1 ou 11
      break;
    }
    // proxima carta
    check=check->prev;
    ++y;
  }

  if(pont_casa->pontos==21&&pont_casa->num_cartas==2){
    pont_casa->state=1;  //quando a casa tem BlackJack é impedido de jogar mais
  }

  else if(pont_casa->pontos==21&&pont_casa->num_cartas!=2){
    pont_casa->state=3;  //quando a casa tem 21 pontos é impedido de jogar mais
  }

  else if(pont_casa->pontos>21){
    pont_casa->state=2;  //quando ocorre bust a casa perde
  }
  else{
    pont_casa->state=0;
  }

}

void registo(no_jogador *head_jogador){

  //regista as estatisticas do jogo num ficheito .txt que podera ser consultado posteriormente ao jogo
  no_jogador *aux=head_jogador;
  FILE*writef;

  writef=fopen("Stats.txt", "w");

  while(aux!=NULL){  
    if(aux->payload.aposta!=0){ // caso exista alguma estrutura vazia
      fprintf(writef, "%s (%s): %iW  %iD  %iL     Ficou com: %i euros     Ganhou/Perdeu: %i euros\n", aux->payload.nome, aux->payload.tipo, aux->payload.jganhos, aux->payload.jempatados, aux->payload.jperdidos, aux->payload.dinheiro, aux->payload.dinheiro-aux->payload.dinheiro_inicial);
    }
    // proximo jogador
    aux=aux->next;
  }

  fclose(writef);
}


void atualizar_apostas(no_jogador *head_jogador, int *div){

  // atualiza o valor das apostas que foram alteras pelo double e pelo surrender
  // desativa os jogadores que ja nao tem dinheiro sufeciente para apostar
  // atualizar o valor das apostas segunda a estrategia HI-LOW

  no_jogador *aux=head_jogador;
  float auxiliar;

  while(aux!=NULL){

    aux->payload.aposta=aux->payload.aposta_inicial;

    if(aux->payload.dinheiro<aux->payload.aposta_inicial){
      aux->payload.active=false;
    }

    if(*aux->payload.tipo=='E'&&*(aux->payload.tipo+1)=='A'&&aux->payload.contagem>0){
      auxiliar=aux->payload.contagem*52/(float)(*div);
      if(auxiliar-(int)auxiliar>0.5){
        ++auxiliar;
      }
      while(aux->payload.dinheiro<aux->payload.aposta*(int)auxiliar*2){
        --auxiliar;
      }
      if(auxiliar>=1){
        aux->payload.aposta*=(int)auxiliar*2;
      }
    }

    aux=aux->next;
  }
}


int decision(int **estrategia, est_casa *pont_casa, no_jogador *aux){

  // escolha de qual a opçao que o jogador EA vai tomar

  int jogada=0;
  int lim=0;
  int y=pont_casa->pontos-2;

  if(aux->payload.soft==true){  // soft hand
    lim=aux->payload.pontos;
    if(lim>19){
      lim=19;                   // se tiver 19 ou + pontos o comportamento é igual
    }else if(lim<13){
      lim=13;                   // se tiver 12 ou 13 pontos o comportamento é igual
    }
    jogada=estrategia[lim-3][y];  // lim-3 -> começa na linha 10 e nao na 13 
  }
  else if(aux->payload.soft==false){  // hard hand
    if(aux->payload.pontos<9){
      jogada=estrategia[0][y];     // o comportamento é igual entre 4 e 8 pontos
    }
    else if(aux->payload.pontos>16){
      jogada=estrategia[9][y];     // o comportamento é igual a partir dos 17 ou mais pontos
    }
    else{
      jogada=estrategia[aux->payload.pontos-8][y];   // aux->(...)-8 -> começa na linha 1 e nao na 9
    }
  }

  // casos em que nao se pode fazer nem double nem surrender
  if(jogada==2&&(aux->payload.num_cartas!=2||aux->payload.dinheiro<aux->payload.aposta*2)){
    if(aux->payload.pontos==18){
      jogada=0;
    }      
    else{
      jogada=1;
    }
  }

  else if(jogada==3&&aux->payload.num_cartas!=2){
    jogada=1;
  }

  return jogada;
}

void contegem_cartas(no_jogador *head_jogador, no_cartas *carta_retirada, int *div){

  no_jogador *aux=head_jogador;

  while(aux!=NULL){
    if(*div==0){
      aux->payload.contagem=0;
    }
    else if(aux->payload.active==true){
      if(carta_retirada->payload.codigo>7){
        --aux->payload.contagem;
      }
      else if(carta_retirada->payload.codigo<5){
        ++aux->payload.contagem;
      }
    }
    aux=aux->next;
  }
}

