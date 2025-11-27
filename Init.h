#ifndef INIT_H
#define INIT_H
#include <SDL.h>
#include <stdio.h>

typedef unsigned char byte;

extern SDL_Event Event;
extern SDL_Window* Window;
extern SDL_Renderer* Renderer;
extern SDL_Rect Display;
extern Uint8* Key;

extern int Running;
extern int Frame;
extern int Scale;
extern int ScreenWidth;
extern int ScreenHeight;
extern const long double Framerate;

extern byte GameState;
extern byte Region;
extern byte TitleType;
extern byte CoinsPerCredit;
extern byte Coins;
extern byte Credits;
extern byte BonusLife;
extern byte Cheating;
extern byte Debug;
extern char Names[8][8];
extern unsigned int HiScores[4];
extern byte MazeTiles[36][28];
extern byte Dots[36][28];
extern byte DotsLeft;

struct Sprite {
    int X;
    int Y;
    int Type;
    SDL_Texture* Sprite;
    int Width;
    int Height;
    int HorizontalOffset;
    int VerticalOffset;
    int NumberOfFrames;
    int Frame; //current frame
    int Frames[16];
    int FramespPerFrame; //animation speed
};
struct Actor {
    int Type;
    int GhostType;
    int X;
    int Y;
    int TargetX;
    int TargetY;
    int Direction;
    int AtIntersection;
    int FramesPerPixel; //move speed
    int Lives;
};
typedef struct Sprite Sprite;
typedef struct Actor Actor;

extern Sprite Black_Tile;
extern Sprite SPR_Text;
extern Sprite BG_Title_Names;
extern Sprite SPR_BlinkyTarget;
extern Sprite SPR_InkyTarget;
extern Sprite SPR_PinkyTarget;
extern Sprite SPR_ClydeTarget;
extern Sprite BG_Maze;
extern Sprite SPR_Pac;
extern Sprite SPR_Blinky;
extern Sprite SPR_Inky;
extern Sprite SPR_Pinky;
extern Sprite SPR_Clyde;
extern Sprite SPR_Scared_Ghost;

extern Actor ACT_Generic;
extern Actor ACT_Pac;
extern Actor ACT_Blinky;
extern Actor ACT_Inky;
extern Actor ACT_Pinky;
extern Actor ACT_Clyde;

extern int RandomTable[512];
extern int RandomIndex;
extern int GhostMode;

enum Regions {
    USA,
    JAP
};
enum FrameTypes {
    Default,
    Alternate,
    Custom,
    Scores
};
enum GameStates {
    Title_Names,
    Title_Play,
    Cutscene,
    Maze_Chase
};
enum Color { //order in which each color appears on static spritesheets vertically
    White,
    Red,
    Pink,
    Blue,
    Orange,
    Peach,
    Yellow
};
enum Types {
    Maze,
    Pac,
    Debug_Target,
    Ghost
};
enum Direction { //order in which each direction appears on spritesheets vertically
    Right,
    Left,
    Up,
    Down
};
enum Ghosts {
    Blinky,
    Inky,
    Pinky, //stinky :-)
    Clyde,
    Slimy
};
enum GhostModes {
    Chase,
    Scatter,
    Scared
};

int SDLInit();
void MazeInit(); 
int SDLRestart();
void SpriteInit(int Restart);
void ActorInit();

#endif
