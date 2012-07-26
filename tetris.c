#include <SDL.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "defines.h"

/*********************Define structs and enums***************************/

typedef enum TetrominoType { I = 1, J, L, O, S, T, Z } TetrominoType;

typedef enum Direction { LEFT = 1, RIGHT, DOWN } Direction;

typedef struct
{
    int rowOffset;
    int colOffset;
} SquareOffset;

typedef struct
{
    int row;
    int col;
} TetrominoPosition;

typedef struct
{
    TetrominoType type;
    SquareOffset offsets[4];
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

SquareOffset I_Offsets[4] = { {0, 0}, {1, 0}, {2, 0}, {3, 0} };
SquareOffset J_Offsets[4] = { {0, 1}, {1, 1}, {2, 1}, {2, 0} };
SquareOffset L_Offsets[4] = { {0, 0}, {1, 0}, {2, 0}, {2, 1} };
SquareOffset O_Offsets[4] = { {0, 0}, {0, 1}, {1, 0}, {1, 1} };
SquareOffset S_Offsets[4] = { {0, 0}, {1, 0}, {1, 1}, {2, 1} };
SquareOffset T_Offsets[4] = { {0, 1}, {1, 0}, {1, 1}, {1, 2} };
SquareOffset Z_Offsets[4] = { {0, 1}, {1, 0}, {1, 1}, {2, 0} };

Gamedata gamedata;
Graphicsdata gfxdata;

/***********************Define functions*************************/

TetrominoPosition getInitialPostion( )
{
    TetrominoPosition tp;
    tp.row = 0;
    tp.col = 3;
    return tp;
}

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
        }  break;
        case( J ):
        {
            memcpy( tm.offsets, J_Offsets, sizeof(SquareOffset) * 4 );
            tm.square.x = PURPLE_SQUARE_X;
            tm.square.y = PURPLE_SQUARE_Y;
        }  break;
        case( L ):
        {
            memcpy( tm.offsets, L_Offsets, sizeof(SquareOffset) * 4 );
            tm.square.x = GREY_SQUARE_X;
            tm.square.y = GREY_SQUARE_Y;
        }  break;
        case( O ):
        {
            memcpy( tm.offsets, O_Offsets, sizeof(SquareOffset) * 4 );
            tm.square.x = BLUE_SQUARE_X;
            tm.square.y = BLUE_SQUARE_Y;
        }  break;
        case( S ):
        {
            memcpy( tm.offsets, S_Offsets, sizeof(SquareOffset) * 4 );
            tm.square.x = GREEN_SQUARE_X;
            tm.square.y = GREEN_SQUARE_Y;
        }  break;
        case( T ):
        {
            memcpy( tm.offsets, T_Offsets, sizeof(SquareOffset) * 4 );
            tm.square.x = BLACK_SQUARE_X;
            tm.square.y = BLACK_SQUARE_Y;
        }  break;
        case( Z ):
        {
            memcpy( tm.offsets, Z_Offsets, sizeof(SquareOffset) * 4 );
            tm.square.x = YELLOW_SQUARE_X;
            tm.square.y = YELLOW_SQUARE_Y;
        }  break;
    }
    tm.square.w = SQUARE_SIDE;
    tm.square.h = SQUARE_SIDE;
    tm.pos = getInitialPostion( );
    return tm;
}

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

int getMaxOffset( Tetromino *tt, Direction dir )
{
    int maxOffset = 0;
    switch( dir )
    {
        case( LEFT ):
        {
            maxOffset = 0;
        }
        case( RIGHT ):
        {
            for( int i = 0; i < 4; i++ )
            {
                if( tt->offsets[i].colOffset > maxOffset )
                {
                    maxOffset = tt->offsets[i].colOffset;
                }
            }
        }
        case( DOWN ):
        {
            for( int i = 0; i < 4; i++ )
            {
                if( tt->offsets[i].rowOffset > maxOffset )
                {
                    maxOffset = tt->offsets[i].rowOffset;
                }
            }
        }
    }
    return maxOffset;
}

int canMove( Tetromino *tt, Direction moveDir )
{
    int retval = 0;
    switch( moveDir )
    {
        case( LEFT ):
        if( tt->pos.col > 0 )
        {
            retval = 1;
        } break;
        case( RIGHT ):
        if( tt->pos.col < 10 - getMaxOffset( tt, RIGHT ) )
        {
            retval = 1;
        } break;
        case( DOWN ):
        if( tt->pos.row < 15 - getMaxOffset( tt, DOWN ) )
        {
            retval = 1;
        } break;
    }
    return retval;
}

void updatePosition( Tetromino *tt, Direction dir )
{
    switch( dir )
    {
        case LEFT:
        if( canMove( tt, LEFT ) )
        {
            tt->pos.row = tt->pos.row + 1;
            tt->pos.col = tt->pos.col - 1;
        } break;
        case RIGHT:
        if( canMove( tt, RIGHT ) )
        {
            tt->pos.row = tt->pos.row + 1;
            tt->pos.col = tt->pos.col + 1;
        } break;
        case DOWN:
        if( canMove( tt, DOWN ) )
        {
            tt->pos.row = tt->pos.row + 1;
            tt->pos.col = tt->pos.col;
        } break;
    }
}

void updateBoard( Tetromino tm )
{
    for( int i = 0; i < 4; i++ )
    {
        gamedata.board[tm.pos.row + tm.offsets[i].rowOffset][tm.pos.col + tm.offsets[i].colOffset] = 1;
    }
}

// These two functions get called each time the game progresses
void progressGame( Direction dir )
{
    updatePosition( &(gamedata.curTm), dir );
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
    for( int i = 0; i < ROWS; i++ )
    {
        for( int j = 0; j < COLUMNS; j++ )
        {
            gamedata.board[i][j] = 0;
        }
    }
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
    SDL_UpdateRect( gfxdata.screen, 0, 0, 0, 0 );
}

int main( int argc, char **argv )
{
    initializeGame( );
    initializeDisplay( );
    SDL_Event event;
    int startTime = 0;
    int over = 0;
    Direction dir;
    startTime = SDL_GetTicks( );
    printf( "%d\n", startTime );
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
                        dir = RIGHT; break;
                    case( SDLK_LEFT ):
                        dir = LEFT; break;
                    default:
                        dir = DOWN; break;
                }
            }
        }
        if( SDL_GetTicks( ) - startTime > TIMESTEP )
        {
            progressGame( dir );
            dir = DOWN;
            updateDisplay( );
            startTime = SDL_GetTicks( );
            printf( "%d\n", startTime );
        }
    }
    SDL_Quit( );
    return 0;
}