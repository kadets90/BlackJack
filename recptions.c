#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "blackjack.h"
#include "recptions.h"


no_jogador *receber_parametros(int *num_bara, int *num_players, char *argv[]){

  int bara_num;
  int play_num;
  int count=0;
  char str[MINI_STRING];

  no_jogador *head_jogador=NULL;  // cabeça da lista de jogadores
  no_jogador *new_node;
  no_jogador *aux;

  FILE *dados=fopen(argv[1], "r");  // ficheiro de configuraçao do jogo

  if(dados==NULL){   // Se nao for lido nenhum ficheiro
    printf("Erro ao carregar parametros!\n");
    exit(0);
  }

  if(fscanf(dados, "%i-%i\n", &bara_num, &play_num)!=2){
    printf("Erro nos numeros!->(Tipo1)\n");  // leitura do numero de baralhos e de jogadores
    exit(0);                                 // e verificaçao se foram recebidos os dados certos
  }

  if(bara_num<MIN_BARALHOS||bara_num>MAX_BARALHOS||play_num<MIN_PLAYERS||play_num>MAX_PLAYERS){
    printf("Erro nos numeros!->(Tipo2)\n");  // verificaçao se os numeros estao dentro do intervalo esperado
    exit(0);
  }

  *num_bara=bara_num;     // ponteiro para passar o numero de baralhos por referencia
  *num_players=play_num;  // ponteiro para passar o numero de jogadores por referencia
  

  while((count<play_num)&&fgets(str, MINI_STRING, dados)!=NULL){  // obtençao dos dados de cada jogador

    ++count;

    new_node=nova_estrutura_vazia();  // criaçao da estrutura para o jogador

    // passagem dos parametros da string auxiliar para as devidas entidades da estrutura jogador
    sscanf(str, "%[^-]-%[^-]-%i-%i", new_node->payload.tipo, new_node->payload.nome, &new_node->payload.dinheiro, &new_node->payload.aposta);

    if(strlen(new_node->payload.nome)>=NAME_SIZE){
      printf("Erro, excesso de caracters no nome!!\n");
      exit(0);
    }

    char a, b;

    a=*new_node->payload.tipo;
    b=*(new_node->payload.tipo+1);

    if((a!='H'||b!='U')&&(a!='E'||b!='A')){  // verificar se o tipo esta correto
      printf("Erro no tipo!!\n");
      exit(0);
    }

    new_node->payload.dinheiro_inicial=new_node->payload.dinheiro;
    new_node->payload.aposta_inicial=new_node->payload.aposta;

    if(new_node->payload.dinheiro>MAX_MONEY||new_node->payload.dinheiro<MIN_MONEY||new_node->payload.aposta>new_node->payload.dinheiro/4||new_node->payload.aposta<MIN_APOSTA){

     // verificar se os valores do dinheiro inicila e das apostas estao corretos
     // e indicaçao de qual o jogador que tem os valores invalidos

      printf("Erro, valor de dinheiro ou aposta inválidos!\nJogaodor: %s\n", new_node->payload.nome);
      exit(0);
    }

    // ativaçao do jogador
    new_node->payload.active=true;

    // primeiro jogador da lista
    if(head_jogador==NULL){
      head_jogador=new_node;
      head_jogador->next=NULL;
      continue;
    }

    aux=head_jogador;

    // procura a primeira posiçao disponivel
    while(aux->next!=NULL){
      aux=aux->next;
    }

    // adicionar o jogador na devida posiçao
    aux->next=new_node;
    new_node->next=NULL;
  }


  return head_jogador;
}

void leitura_estrategia(int **estrategia){

  // receçao da matriz com a estrategia que as EA deverao seguir

  int count=0;
  FILE *dados=fopen("estrategia", "r");  // leitura de o ficheiro "estrategia"

  for(int x=0; x<STRATEGY_LINE; ++x){
    for(int y=0; y<STRATEGY_COLUMN; ++y){
      count=fscanf(dados, "%d", &estrategia[x][y]);  // passagem dos numeros do ficheiro para a matriz
      if(count!=1){
        printf("Erro na leitura da estrategia!!\n");  // caso o ficheiro nao esteja correto
      }
    }
  }
}

no_jogador *adicionar_jogador(no_jogador *head_jogador, int *num_player){

  // funçao que trata de todo o procedimento de adicionar um novo jogador
  int pos=0;
  int quit=0;
  no_jogador *aux=head_jogador;
  int npa=0;  // numero de players ativos
  no_jogador *tmp=NULL;
  no_jogador *prev=NULL;
  no_jogador *new_player=NULL;
  int count=0;

  while(aux!=NULL){
    if(aux->payload.active==true){   // conta quantos jogadores estao ativos
      ++npa;
    }
    aux=aux->next;
  }
  if(npa==4){                        // se o numero de jogadores ativos for 4 ele sai da funçao
    printf("Erro, impossivel adicionar jogador, numero limite: %i\n", npa);
    return head_jogador;
  }

  while(quit!=1){

    pos=posicao_jogador(quit);

    aux=head_jogador;

    for(int v=0; v<pos; ++v){

      // avançar o ponteiro auxiliar ate a posiçao em que se pretende colocar o novo jogador

      prev=aux;
      aux=aux->next;

      if(aux==NULL){        
        new_player=nova_estrutura_vazia(); // estrutura "vazia" para permitir colocar   
        aux=new_player;                    // jogadores mais à direita, independentemente 
        prev->next=aux;                    // de quantos jogadores iniciaram o jogo
      }
    }

    if(aux->payload.active==true){
       // caso ja esteja um jogador na posiçao é apresentada uma mensagem de erro
       printf("Erro, jogador ja existente nessa posicao\n");
    }
    else if(aux->payload.active==false&&aux->payload.aposta!=0&&pos==0){

      // caso a posiçao seja a primeira

      new_player=nova_estrutura_vazia();
      new_player=inicializar_jogador(new_player);
      tmp=head_jogador->next;
      head_jogador->next=NULL;
      if(tmp!=NULL){
        new_player->next=tmp;
        while(tmp->next!=NULL){
          tmp=tmp->next;
        }
        tmp->next=head_jogador;
      }
      else{
        new_player->next=head_jogador;
      }
      head_jogador->next=NULL;
      head_jogador=new_player;
      quit=1;

    }
    else if(aux->payload.active==false&&aux->payload.aposta!=0&&pos!=0){

      // caso a posiçao seja onde ja esteve um jogador que agora esta desativo

      new_player=nova_estrutura_vazia();
      new_player=inicializar_jogador(new_player);
      tmp=aux->next;
      aux->next=NULL;
      prev->next=new_player;
      if(tmp!=NULL){
        new_player->next=tmp;
        while(tmp->next!=NULL){
          tmp=tmp->next;
        }
        tmp->next=aux;
      }
      else{
        new_player->next=aux;
      }
      aux=NULL;
      quit=1;

    }
    else if(aux->payload.active==false&&aux->payload.aposta==0){

      // posiçao onde nunca esteve um jogador, e existe uma estrutura vazia

      new_player=aux;
      new_player=inicializar_jogador(new_player);
      quit=1;

    }
  }

  aux=head_jogador;

  count=0;

  while(aux!=NULL){
    ++count;
    if(aux->payload.active==true){
      *num_player=count;           // contagem para saber qual a ultima posiçao com um jogador ativo
    }
    aux=aux->next;
  }

  return head_jogador;
}

no_jogador *inicializar_jogador(no_jogador *new_player){

  // define o nome, tipo, dinheiro incial e aposta do jogador adicionado e "ativa" a estrutura

  char auxstr[NAME_SIZE];
  int count=0, result=0;

  printf("Insira o nome do jogador:");
  fgets(auxstr, NAME_SIZE, stdin);  // nome do novo jogador
  sscanf(auxstr, "%s", new_player->payload.nome);

  do{
    printf("Insira o tipo do jogador:"); 
    fgets(auxstr, NAME_SIZE, stdin);  // tipo do novo jogador
    sscanf(auxstr, "%s", new_player->payload.tipo);

    if( (*(new_player->payload.tipo)!='E'||*(new_player->payload.tipo+1)!='A')&&(*(new_player->payload.tipo)!='H'||*(new_player->payload.tipo+1)!='U') ){
      printf("Erro, tipo de jogador invalido!!\n");  //aviso caso esteja errado
    }
  }while((*(new_player->payload.tipo)!='E'||*(new_player->payload.tipo+1)!='A')&&(*(new_player->payload.tipo)!='H'||*(new_player->payload.tipo+1)!='U')); //verificar se é valido


  do{
    printf("Inserir valor do dinheiro:");  
    fgets(auxstr, NAME_SIZE, stdin);  //receber o valor do dinheiro
    count=sscanf(auxstr, "%i", &result);

    if(result<MIN_MONEY||result>MAX_MONEY||count==0){
      printf("Valor invalido, valor pretendido é entre %d e %d\n", MIN_MONEY, MAX_MONEY);  //aviso caso esteja errado
    }
  }while(result<MIN_MONEY||result>MAX_MONEY); // verificar se é valido


  new_player->payload.dinheiro=result;
  new_player->payload.dinheiro_inicial=new_player->payload.dinheiro;  // aplica o valor do dinheiro selecionado


  do{
    printf("Inserir valor da aposta:");  
    fgets(auxstr, NAME_SIZE, stdin);  // receber o valor da aposta
    count=sscanf(auxstr, "%i", &result);

    if(result<MIN_APOSTA||result>new_player->payload.dinheiro/4||count==0){
      printf("Valor invalido, valor pretendido é entre %d e %d\n", MIN_APOSTA, new_player->payload.dinheiro/4);  // aviso caso esteja errado
    }
  }while(result<MIN_APOSTA||result>new_player->payload.dinheiro/4); // verificar se é valido

  new_player->payload.aposta=result;
  new_player->payload.aposta_inicial=new_player->payload.aposta;  // aplica o valor da aposta selecionado

  new_player->payload.active=true;  // ativar a estrutura jogador

  printf("SUCESSO!!\n");

  return new_player;
}

no_jogador *nova_estrutura_vazia(){

  // criaçao de um no "vazio" 

  no_jogador *new_player=(no_jogador*)malloc(sizeof(no_jogador));
        
  if(new_player==NULL){  // verificar se a memoria foi bem alocada
    printf("Falhou a criaçao de memoria\n");
    exit(0);
  }

  //inicializaçao dos dados da estrutura jogador a 0 ou NULL
  new_player->payload.active=false;
  new_player->payload.jganhos=0;
  new_player->payload.jperdidos=0;
  new_player->payload.jempatados=0;
  new_player->payload.num_cartas=0;
  new_player->payload.state=0;
  new_player->payload.pontos=0;
  new_player->payload.topo=NULL;
  new_player->payload.fim=NULL;
  new_player->payload.dinheiro=0;
  new_player->payload.aposta=0;
  new_player->next=NULL;

  return new_player;
}


void mudar_aposta(no_jogador *head_jogador, int *num_player){
  
  int result=0;
  char num[MINI_STRING];
  int count=0;                   // verifica se recebeu algum inteiro
  no_jogador *aux=head_jogador;

  do{
    aux=head_jogador;
    printf("Nome do jogador:"); 
    fgets(num, MINI_STRING, stdin);  //receber o valor da posiçao

    while(aux!=NULL&&result==0){

      result=1;

      for(int x=0; x<strlen(aux->payload.nome); ++x){
        if(aux->payload.nome[x]!=num[x]){
          result=0;
          break;
        }
      }

      if(result==0){
        aux=aux->next;
      }
    }

    if(result==0){
      printf("Nome invalido!!\n");  //aviso caso esteja errado
    }

  }while(result==0); //verificar se é valido

  if(aux->payload.active==false){
    printf("Erro, impossivel alterar valor da aposta!!\n");
  }
  else{

    do{
      printf("Novo valor da aposta:");  
      fgets(num, MINI_STRING, stdin);  //receber o valor da nova aposta
      count=sscanf(num, "%i", &result);

      if(result<MIN_APOSTA||result>aux->payload.dinheiro/4||count==0){
        printf("Valor invalido, valor pretendido é entre %d e %d\n", MIN_APOSTA, aux->payload.dinheiro/4);  //aviso caso esteja errado
      }
    }while(result<MIN_APOSTA||result>aux->payload.dinheiro/4); //verificar se é valido

    aux->payload.aposta_inicial=result;  // aplica o novo valor como o da aposta

    printf("SUCESSO!!\n");
  }
}

int posicao_jogador(int quit){

  SDL_Event event;
  int x, y;
  int pos=0;

  while(quit==0){
    while(SDL_PollEvent(&event)){
      if(event.type==SDL_MOUSEBUTTONDOWN){
        SDL_GetMouseState(&x, &y);
        if(y>290&&y<508){
          for(int n=0; n<MAX_PLAYERS; ++n){
            if(x>12+206*n&&x<12+206*(n+1)){   // leitura da posiçao em que devera ficar o 
              pos=n;                          // novo jogador com recurso ao rato
              quit=1;
            }
          }
        }
      }
    }
  }

  return pos;
}

int alterar_delay(int delay_EA, bool sinal){

  if(sinal==true){             // +0.5 segundos de espera
    if(delay_EA<5000){         // maximo de 5 segundos
      delay_EA+=500;
    }
    printf("%f segundos\n", (float)delay_EA/1000);
  }else{                       // -0.5 segundos de espera
    if(delay_EA>500){          // minimo de 0.5 segundos
      delay_EA-=500;
    }
    printf("%f segundos\n", (float)delay_EA/1000);
  }

  return delay_EA;
}

