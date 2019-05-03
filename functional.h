#ifndef functional

no_cartas *carregar_baralhos(int num, no_cartas *head);
void print_cartas(no_cartas *head);
void distribuir_cartas(no_cartas *carta_retirada, no_jogador *aux);
void atualizar_dinheiro(no_jogador *head_jogador, int *num_jogadores, est_casa *pont_casa);
void pontos(no_jogador *aux);
void cartas_para_casa(no_cartas *carta_retirada, est_casa *pont_casa);
void pontos_casa(est_casa *pont_casa);
void registo(no_jogador *head_jogador);
void atualizar_apostas(no_jogador *head_jogador, int *div);
int decision(int **estrategia, est_casa *pont_casa, no_jogador *aux);
void contegem_cartas(no_jogador *head_jogador, no_cartas *carta_retirada, int *div);

#endif
