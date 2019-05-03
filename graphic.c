#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "blackjack.h"
#include "graphic.h"

// definition of some strings: they cannot be changed when the program is executed !
const char myName[] = "David Ribeiro e Rafael Forte";
const char myNumber[] = "84027 e 84174";


/**
 * RenderTable: Draws the table where the game will be played, namely:
 * -  some texture for the background
 * -  the right part with the IST logo and the student name and number
 * -  squares to define the playing positions of each player
 * -  names and the available money for each player
 * \param _money amount of money of each player
 * \param _img surfaces where the table background and IST logo were loaded
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderTable(TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer, no_jogador *head_jogador, int player, est_casa *pont_casa, int *num_players, int *div)
{
    SDL_Color black = { 0, 0, 0 }; // black
    SDL_Color white = { 255, 255, 255 }; // white
    SDL_Color red = { 255, 0, 0 };  // red
    SDL_Color yellow = {255, 255, 0}; //yellow
    SDL_Color orange= {255, 128, 0}; //orange
    SDL_Color purple= {255, 0, 255}; //purple
    
    int place;
    int x_info=(int) (0.955f*WIDTH_WINDOW);
    int y_info=(int) (0.75f*HEIGHT_WINDOW);
    char name_money_str[STRING_SIZE];
    char name_pts_str[STRING_SIZE];
    char house_pts_str[STRING_SIZE];
    char state_str[STRING_SIZE];
    char aux_string[STRING_SIZE];;
    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest, playerRect;
    int separatorPos = (int)(0.95f*WIDTH_WINDOW); // seperates the left from the right part of the window
    int height;
    no_jogador *aux=head_jogador;
   
    // set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );
    
    // clear the window
    SDL_RenderClear( _renderer );

    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;

    tableDest.w = separatorPos;
    tableDest.h = HEIGHT_WINDOW;

    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);
   
    // render the IST Logo
    height = RenderLogo(separatorPos, 0, _img[1], _renderer);
    
    // render the student name
    height += RenderText(separatorPos+3*MARGIN-10, height, myName, _font, &black, _renderer);
    
    // this renders the student number
    RenderText(separatorPos+3*MARGIN, height, myNumber, _font, &black, _renderer);

    // quando o deck acaba
    if(*div<10){
      RenderText(x_info+20, y_info-120, "Baralho a acabar!!", _font, &red, _renderer);
    }
    // quando o deck acaba
    else if(*div==0){
      RenderText(x_info+20, y_info-120, "Novo baralho!!", _font, &red, _renderer);
    }

    // renders the areas for each player: names and money too !
    for ( int i = 0; i < *num_players; i++){

      if(aux->payload.active==true){
        RenderText(x_info, y_info+i*20, aux->payload.nome, _font, &black, _renderer);
        RenderText(x_info+90, y_info+i*20, aux->payload.tipo, _font, &black, _renderer);

        sprintf(aux_string, "%i euros", aux->payload.dinheiro);

        RenderText(x_info+120, y_info+i*20, aux_string, _font, &black, _renderer);
      }

      // renders the squares + name for each player
      SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255 );
   
      //mover a label dos pontos para nao ficar de baixo das cartas
      if(pont_casa->num_cartas<=4){
        place=1;
      }else{
        place=0;
      }
   
      if(pont_casa->state==2){

        playerRect.x = 3*(separatorPos/4-5)+10;  //render house points
        sprintf(house_pts_str, "%i pontos - HOUSE BUST!", pont_casa->pontos);
        RenderText(playerRect.x-60*place, playerRect.y-110, house_pts_str, _font, &yellow, _renderer);

      }else{
        playerRect.x = 3*(separatorPos/4-5)+10;  //render house points
        sprintf(house_pts_str, "%i pontos", pont_casa->pontos);
        RenderText(playerRect.x-60*place, playerRect.y-110, house_pts_str, _font, &white, _renderer);
      }
    

      if(i==player){ 
        // renders the squares + name for active player
        SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 0 );
      }

      playerRect.x = i*(separatorPos/4-5)+10;
      playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
      playerRect.w = separatorPos/4-5;
      playerRect.h = (int) (0.42f*HEIGHT_WINDOW);
      SDL_RenderDrawRect(_renderer, &playerRect);

      if(aux->payload.active==1||aux->payload.pontos!=0){ 
        sprintf(name_money_str,"%s -- %d euros", aux->payload.nome, aux->payload.aposta);
        RenderText(playerRect.x+45, playerRect.y-45, name_money_str, _font, &white, _renderer);
        sprintf(name_pts_str, "%i pontos", aux->payload.pontos);
        RenderText(playerRect.x+95, playerRect.y-28, name_pts_str, _font, &white, _renderer);  //render player's points

        if(aux->payload.state==1){
          //BlackJack
          playerRect.x = i*(separatorPos/4-5)+10;
          sprintf(state_str, "BlackJack");
          RenderText(playerRect.x+20, playerRect.y-25, state_str, _font, &yellow, _renderer);  //render player's points
          //SDL_RenderDrawRect(_renderer, &playerRect);
        }
        else if(aux->payload.state==2){
          //Bust
          playerRect.x = i*(separatorPos/4-5)+10;
          sprintf(state_str, "Bust");
          RenderText(playerRect.x+20, playerRect.y-25, state_str, _font, &red, _renderer);  //render player's points
          //SDL_RenderDrawRect(_renderer, &playerRect);
        }
        else if(aux->payload.state==4){
          //Surrend
          playerRect.x = i*(separatorPos/4-5)+10;
          sprintf(state_str, "Surrender");
          RenderText(playerRect.x+20, playerRect.y-25, state_str, _font, &orange, _renderer);  //render player's points
          //SDL_RenderDrawRect(_renderer, &playerRect);
        }
        else if(aux->payload.state==5){
          //Double
          playerRect.x = i*(separatorPos/4-5)+10;
          sprintf(state_str, "Double");
          RenderText(playerRect.x+20, playerRect.y-25, state_str, _font, &purple, _renderer);  //render player's points
          //SDL_RenderDrawRect(_renderer, &playerRect);
        }
      }
      aux=aux->next;
    }
    
    // destroy everything
    SDL_DestroyTexture(table_texture);
}

/**
 * RenderHouseCards: Renders cards of the house
 * \param _house vector with the house cards
 * \param _pos_house_hand position of the vector _house with valid card IDs
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderHouseCards(SDL_Surface **_cards, SDL_Renderer* _renderer, est_casa *pont_casa)
{
    int card, x, y;
    int div = WIDTH_WINDOW/CARD_WIDTH;
    no_cartas *aux;
    aux=pont_casa->fim;

    // drawing all house cards
    for ( card = 0; card < pont_casa->num_cartas; card++)
    {
        // calculate its position
        x = (div/2-pont_casa->num_cartas/2+card)*CARD_WIDTH + 15;
        y = (int) (0.26f*HEIGHT_WINDOW);
        // render it !
        RenderCard(x, y, aux->payload.codigo+13*aux->payload.naipe, _cards, _renderer);
        aux=aux->prev;
    }
    // just one card ?: draw a card face down
    if (pont_casa->num_cartas == 1)
    {
        x = (div/2-pont_casa->num_cartas/2+1)*CARD_WIDTH + 15;
        y = (int) (0.26f*HEIGHT_WINDOW);
        RenderCard(x, y, MAX_DECK_SIZE, _cards, _renderer);
    }
}

/**
 * RenderPlayerCards: Renders the hand, i.e. the cards, for each player
 * \param _player_cards 2D array with the player cards, 1st dimension is the player ID
 * \param _pos_player_hand array with the positions of the valid card IDs for each player
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderPlayerCards(SDL_Surface **_cards, SDL_Renderer* _renderer, no_jogador *head_jogador, int *num_player)
{
  int pos, x, y;
  no_jogador *aux=head_jogador;
  no_cartas *check;
 
  for(int v=0; v<*num_player; ++v){

    check=aux->payload.fim;

    for(int w=0; w!=aux->payload.num_cartas; ++w){
    // draw all cards of the player: calculate its position: only 4 positions are available !
      pos = w % 4;
      x = (int) v*((0.95f*WIDTH_WINDOW)/4-5)+(w/4)*12+15;
      y = (int) (0.55f*HEIGHT_WINDOW)+10;
      if ( pos == 1 || pos == 3) x += CARD_WIDTH + 30;
      if ( pos == 2 || pos == 3) y += CARD_HEIGHT+ 10;
      // render it !
      RenderCard(x, y, check->payload.codigo+check->payload.naipe*13, _cards, _renderer);
      check=check->prev;
    }
    aux=aux->next;
  }
}

/**
 * RenderCard: Draws one card at a certain position of the window, based on the card code
 * \param _x X coordinate of the card position in the window
 * \param _y Y coordinate of the card position in the window
 * \param _num_card card code that identifies each card
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderCard(int _x, int _y, int _num_card, SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    SDL_Texture *card_text;
    SDL_Rect boardPos;

    // area that will be occupied by each card
    boardPos.x = _x;
    boardPos.y = _y;
    boardPos.w = CARD_WIDTH;
    boardPos.h = CARD_HEIGHT;

    // render it !
    card_text = SDL_CreateTextureFromSurface(_renderer, _cards[_num_card]);
    SDL_RenderCopy(_renderer, card_text, NULL, &boardPos);
    
    // destroy everything
    SDL_DestroyTexture(card_text);
}

/**
 * LoadCards: Loads all images of the cards
 * \param _cards vector with all loaded card images
 */
void LoadCards(SDL_Surface **_cards)
{
    int i;
    char filename[STRING_SIZE];

     // loads all cards to an array
    for (i = 0 ; i < MAX_DECK_SIZE; i++ )
    {
        // create the filename !
        sprintf(filename, ".//cartas//carta_%02d.png", i+1);
        // loads the image !
        _cards[i] = IMG_Load(filename);
        // check for errors: deleted files ?
        if (_cards[i] == NULL)
        {
            printf("Unable to load image: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }
    // loads the card back
    _cards[i] = IMG_Load(".//cartas//carta_back.jpg");
    if (_cards[i] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}


/**
 * UnLoadCards: unloads all card images of the memory
 * \param _cards vector with all loaded card images
 */
void UnLoadCards(SDL_Surface **_array_of_cards)
{
    // unload all cards of the memory: +1 for the card back
    for (int i = 0 ; i < MAX_DECK_SIZE + 1; i++ )
    {
        SDL_FreeSurface(_array_of_cards[i]);
    }
}

/**
 * RenderLogo function: Renders the IST Logo on the window screen
 * \param x X coordinate of the Logo
 * \param y Y coordinate of the Logo
 * \param _logoIST surface with the IST logo image to render
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderLogo(int x, int y, SDL_Surface *_logoIST, SDL_Renderer* _renderer)
{
	SDL_Texture *text_IST;
	SDL_Rect boardPos;
    
    // space occupied by the logo
	boardPos.x = x;
	boardPos.y = y;
	boardPos.w = _logoIST->w;
	boardPos.h = _logoIST->h;

    // render it 
	text_IST = SDL_CreateTextureFromSurface(_renderer, _logoIST);
	SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

    // destroy associated texture !
	SDL_DestroyTexture(text_IST);
	return _logoIST->h;
}

/**
 * RenderText function: Renders the IST Logo on the window screen
 * \param x X coordinate of the text
 * \param y Y coordinate of the text
 * \param text string where the text is written
 * \param font TTF font used to render the text
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer)
{
	SDL_Surface *text_surface;
	SDL_Texture *text_texture;
	SDL_Rect solidRect;

	solidRect.x = x;
	solidRect.y = y;
    // create a surface from the string text with a predefined font
	text_surface = TTF_RenderText_Blended(_font,text,*_color);
	if(!text_surface)
	{
	    printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
	    exit(EXIT_FAILURE);
	}
    // create texture
	text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
    // obtain size
	SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    // render it !
	SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);

	SDL_DestroyTexture(text_texture);
	SDL_FreeSurface(text_surface);
	return solidRect.h;
}

/**
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _img surface to be created with the table background and IST logo
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 */
void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer)
{
    InitSDL();
    InitFont();
    *_window = CreateWindow(width, height);
    *_renderer = CreateRenderer(width, height, *_window);
    
    // load the table texture
    _img[0] = IMG_Load("table_texture.png");
    if (_img[0] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    // load IST logo
    _img[1] = SDL_LoadBMP("ist_logo.bmp");
    if (_img[1] == NULL)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    // this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont("FreeSerif.ttf", 16);
    if(!*_font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitSDL: Initializes the SDL2 graphic library
 */
void InitSDL()
{
    // init SDL library
	if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
	}
}

/**
 * InitFont: Initializes the SDL2_ttf font library
 */
void InitFont()
{
	// Init font library
	if(TTF_Init()==-1)
	{
	    printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
	}
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
 */
SDL_Window* CreateWindow(int width, int height)
{
    SDL_Window *window;
    // init window
	window = SDL_CreateWindow( "BlackJack", WINDOW_POSX, WINDOW_POSY, width+EXTRASPACE, height, 0 );
    // check for error !
	if ( window == NULL )
	{
		printf("Failed to create window : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	return window;
}

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
 */
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window)
{
    SDL_Renderer *renderer;
    // init renderer
	renderer = SDL_CreateRenderer( _window, -1, 0 );

	if ( renderer == NULL )
	{
		printf("Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
	}

	// set size of renderer to the same as window
	SDL_RenderSetLogicalSize( renderer, width+EXTRASPACE, height );

	return renderer;
}


