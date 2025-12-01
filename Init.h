#ifndef INIT_H
#define INIT_H
#include <SDL.h>
#include <stdio.h>

typedef unsigned char byte; //0-255 unsigned
typedef char Byte; //-127-127 signed

extern SDL_Event Event;
extern SDL_Window* Window;
extern SDL_Renderer* Renderer;
extern SDL_Rect Display;
extern Uint8* Key;

extern byte Running;
extern byte Frame;
extern byte Scale;
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
    byte Type;
    SDL_Texture* Sprite;
    int Width;
    int Height;
    Byte HorizontalOffset;
    Byte VerticalOffset;
    byte NumberOfFrames;
    byte Frame; //current frame
    byte Frames[16];
    byte FramesPerFrame; //animation speed
};
struct Actor {
    byte Type;
    byte GhostType;
    int X;
    int Y;
    int TargetX;
    int TargetY;
    byte Direction;
    byte FramesPerPixel; //move speed
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
extern byte GhostMode;

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
extern byte Reversed[4];
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
