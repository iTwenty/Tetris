#include <SDL.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "defines.h"

/*********************Define structs and enums***************************/

typedef enum TetrominoType { I = 1, J, L, O, S, T, Z } TetrominoType;

typedef enum Direction { LEFT = 1, RIGHT, DOWN } Direction;

typedef enum Boolean { FALSE, TRUE } Boolean;

typedef struct
{
    int rowOffset;
    int colOffset;
} SquareOffset;

typedef struct
{
    int maxHeight; 
} BottomPile;

typedef struct
{
    int row;
    int col;
} TetrominoPosition;

typedef struct
{
    Direction moveDir;
    Boolean shouldRotate;
} InputData;

typedef struct
{
    TetrominoType type;
    SquareOffset offsets[4];
    SquareOffset maxOffset;
    SquareOffset rotnPt;
    TetrominoPosition pos;
    SDL_Rect square;
} Tetromino;

typedef struct
{
    int level;
    int score;
    int speed;
    int board[ROWS][COLUMNS];
    Tetromino curTm;
    Tetromino nxtTm;
} Gamedata;

typedef struct
{
    SDL_Surface *screen;
    SDL_Surface *bitmap;
    SDL_Surface *board;
} Graphicsdata;

SquareOffset I_Offsets[4] = {
                              {0,0},
                              {1,0},
                              {2,0},
                              {3,0}
                            };
                              
SquareOffset J_Offsets[4] = {
                                     {0,1},
                                     {1,1},
                              {2,0}, {2,1}
                            };
                          
SquareOffset L_Offsets[4] = {
                              {0,0},
                              {1,0},
                              {2,0}, {2,1}
                            }; 
SquareOffset O_Offsets[4] = {
                              {0,0}, {0,1},
                              {1,0}, {1,1}
                            };
SquareOffset S_Offsets[4] = {
                              {0,0},
                              {1,0}, {1,1},
                                     {2,1}
                            };
SquareOffset T_Offsets[4] = {
                                     {0,1},
                              {1,0}, {1,1}, {1,2}
                            };
SquareOffset Z_Offsets[4] = {
                                     {0,1},
                              {1,0}, {1,1},
                              {2,0}
                            };

Gamedata gamedata;
Graphicsdata gfxdata;

/***********************Define functions*************************/


// Returns the initial starting postion of Tetromino's top left square
TetrominoPosition getInitialPostion( )
{
    TetrominoPosition tp;
    tp.row = 0;
    tp.col = 3;
    return tp;
}

// Returns a random Tetromino each time it is called
Tetromino createTetromino( )
{
    Tetromino tm;
    srand( time( NULL ) );
    tm.type = 1 + rand( ) % 7;
    switch( tm.type )
    {
        case( I ):
        {
            memcpy( tm.offsets, I_Offsets, sizeof(SquareOffset) * 4 );
            tm.square.x = RED_SQUARE_X;
            tm.square.y = RED_SQUARE_Y;
            tm.maxOffset.rowOffset = 3;
            tm.maxOffset.colOffset = 0;
            tm.rotnPt = I_Offsets[0];
        }  break;
        case( J ):
        {
            memcpy( tm.offsets, J_Offsets, sizeof(SquareOffset) * 4 );
            tm.square.x = PURPLE_SQUARE_X;
            tm.square.y = PURPLE_SQUARE_Y;
            tm.maxOffset.rowOffset = 2;
            tm.maxOffset.colOffset = 1;
            tm.rotnPt = J_Offsets[0];
        }  break;
        case( L ):
        {
            memcpy( tm.offsets, L_Offsets, sizeof(SquareOffset) * 4 );
            tm.square.x = GREY_SQUARE_X;
            tm.square.y = GREY_SQUARE_Y;
            tm.maxOffset.rowOffset = 2;
            tm.maxOffset.colOffset = 1;
            tm.rotnPt = L_Offsets[0];
        }  break;
        case( O ):
        {
            memcpy( tm.offsets, O_Offsets, sizeof(SquareOffset) * 4 );
            tm.square.x = BLUE_SQUARE_X;
            tm.square.y = BLUE_SQUARE_Y;
            tm.maxOffset.rowOffset = 1;
            tm.maxOffset.colOffset = 1;
            tm.rotnPt = O_Offsets[0];
        }  break;
        case( S ):
        {
            memcpy( tm.offsets, S_Offsets, sizeof(SquareOffset) * 4 );
            tm.square.x = GREEN_SQUARE_X;
            tm.square.y = GREEN_SQUARE_Y;
            tm.maxOffset.rowOffset = 2;
            tm.maxOffset.colOffset = 1;
            tm.rotnPt = S_Offsets[0];
        }  break;
        case( T ):
        {
            memcpy( tm.offsets, T_Offsets, sizeof(SquareOffset) * 4 );
            tm.square.x = BLACK_SQUARE_X;
            tm.square.y = BLACK_SQUARE_Y;
            tm.maxOffset.rowOffset = 1;
            tm.maxOffset.colOffset = 2;
            tm.rotnPt = T_Offsets[0];
        }  break;
        case( Z ):
        {
            memcpy( tm.offsets, Z_Offsets, sizeof(SquareOffset) * 4 );
            tm.square.x = YELLOW_SQUARE_X;
            tm.square.y = YELLOW_SQUARE_Y;
            tm.maxOffset.rowOffset = 2;
            tm.maxOffset.colOffset = 1;
            tm.rotnPt = Z_Offsets[0];
        }  break;
    }
    tm.square.w = SQUARE_SIDE;
    tm.square.h = SQUARE_SIDE;
    tm.pos = getInitialPostion( );
    return tm;
}

// Resets the input data to prepare for next progression
void resetInputData( InputData *in )
{
    in->moveDir = DOWN;
    in->shouldRotate = FALSE;
}

// Draws the tetromino squares based on its position and square offsets
void drawTetromino( Tetromino tm )
{
    SDL_Rect drawer;;
    for( int i = 0; i < 4; ++i )
    {    
        drawer.x = ( tm.pos.col + tm.offsets[i].colOffset ) * SQUARE_SIDE;
        drawer.y = ( tm.pos.row + tm.offsets[i].rowOffset ) * SQUARE_SIDE;
        drawer.w = SQUARE_SIDE;
        drawer.h = SQUARE_SIDE;
        SDL_BlitSurface( gfxdata.bitmap, &( gamedata.curTm.square ), gfxdata.screen, &drawer );
        SDL_UpdateRect( gfxdata.screen, 0, 0, 0, 0 );
    }
}

// void rotateTetromino( Tetromino *tt, Boolean shouldRotate )
// {
//     if( !shouldRotate )
//         return;
//     
// }

// Returns true if tetromino can move in given direction 
Boolean canMove( Tetromino *tt, Direction moveDir )
{
    Boolean retval = FALSE;
    switch( moveDir )
    {
        case( LEFT ):
        if( tt->pos.col > 0 )
        {
            retval = TRUE;
        } break;
        case( RIGHT ):
        if( tt->pos.col < (COLUMNS - 1) - tt->maxOffset.colOffset )
        {
            retval = TRUE;
        } break;
        case( DOWN ):
        if( tt->pos.row < (ROWS -1) - tt->maxOffset.rowOffset )
        {
            retval = TRUE;
        } break;
    }
    return retval;
}

// Actually moves the tetromino in given direction
void move( Tetromino *tt, Direction dir )
{
    switch( dir )
    {
        case LEFT:
            tt->pos.col = tt->pos.col - 1; break;
        case RIGHT:
            tt->pos.col = tt->pos.col + 1; break;
        case DOWN:
            tt->pos.row = tt->pos.row + 1; break;
    }
}


// Wrapper to previous two moving functions
void updatePosition( Tetromino *tt, Direction dir )
{
    if( canMove( tt, dir ) )
        move( tt, dir );
}

// Updates the game-board to reflect current position of tetromino
void updateBoard( Tetromino tm )
{
    for( int i = 0; i < 4; i++ )
    {
        gamedata.board[tm.pos.row + tm.offsets[i].rowOffset][tm.pos.col + tm.offsets[i].colOffset] = 1;
    }
}

// These two functions get called each time the game progresses
void progressGame( InputData inputdata )
{
    updatePosition( &(gamedata.curTm), inputdata.moveDir );
    updatePosition( &(gamedata.curTm), DOWN );
    updateBoard( gamedata.curTm );
}

void updateDisplay( )
{
    SDL_BlitSurface( gfxdata.board, NULL, gfxdata.screen, NULL );
    drawTetromino( gamedata.curTm );
}


// These two functions get called only once to initialise game state and graphics
void initializeGame( )
{
    gamedata.level = 1;
    gamedata.score = 0;
    gamedata.speed = INITIAL_SPEED;
    gamedata.curTm = createTetromino( );
    gamedata.nxtTm = createTetromino( );
    updateBoard( gamedata.curTm );
}

void initializeDisplay( )
{
    SDL_Init( SDL_INIT_VIDEO );
    SDL_WM_SetCaption( WINDOW_CAPTION, WINDOW_CAPTION );
    gfxdata.screen = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0 );
    SDL_Surface* tmp = SDL_LoadBMP( "data/tetris.bmp" );
    gfxdata.bitmap = SDL_DisplayFormat( tmp );
    tmp = SDL_LoadBMP( "data/bg.bmp" );
    gfxdata.board = SDL_DisplayFormat( tmp );
    SDL_FreeSurface( tmp );
    SDL_BlitSurface( gfxdata.board, NULL, gfxdata.screen, NULL );
    drawTetromino( gamedata.curTm );
}


// Main enters the scene...
int main( int argc, char **argv )
{
    initializeGame( );
    initializeDisplay( );
    SDL_Event event;
    int startTime = 0;
    int over = 0;
    InputData inputdata = { DOWN, FALSE };
    startTime = SDL_GetTicks( );
    while( !over )
    {
        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT )
            {
                over = 1;
            }
            if( event.type == SDL_KEYDOWN )
            {
                switch( event.key.keysym.sym )
                {
                    case( SDLK_RIGHT ):
                        inputdata.moveDir = RIGHT; break;
                    case( SDLK_LEFT ):
                        inputdata.moveDir = LEFT; break;
                    default:
                        inputdata.moveDir = 0; break;
                }
            }
        }
        if( SDL_GetTicks( ) - startTime > TIMESTEP )
        {
            progressGame( inputdata );
            updateDisplay( );
            resetInputData( &inputdata );
            startTime = SDL_GetTicks( );
            printf( "%d\n", startTime );
        }
    }
    SDL_Quit( );
    return 0;
}