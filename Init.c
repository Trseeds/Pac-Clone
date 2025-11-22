#include "Init.h"
#include "Engine.h"

SDL_Event Event;
SDL_Window* Window;
SDL_Renderer* Renderer;
SDL_Rect Display;
Uint8* Key;

int Running = 1;
int Frame = 1;
int Scale = 3;
int ScreenWidth = 224;
int ScreenHeight = 288;
const long double Framerate = 100.0/6.0;

byte GameState = Title_Names;
byte Region = USA;
byte TitleType = Default;
byte CoinsPerCredit = -1; //free
byte Coins = 0;
byte BonusLife = 10; //* 1000
byte NameTypes = 0; //0 us, 1 alt us, 2 jap, 3 alt jap, 4 hi scores, 5 custom
byte Cheating = 0;
byte Debug = 0;
char Names[8][8] = {"CHASER  ","BLINKY\" ",
                    "HUNTER  ","PINKY\"  ",
                    "FLANKER ","INKY\"   ",
                    "MORON   ","CLYDE\"  "}; //custom (usa,programmers choice)
// char Names[8][8] = {"CHASER__","\"BLINKY\"",
//                     "HUNTER__","_\"PINKY\"",
//                     "FLANKER_","__\"INKY\"",
//                     "MORON___","_\"CLYDE\""}; //custom (jap,programmers choice)
unsigned int HiScores[4] = {0,0,0,0};

typedef struct Sprite Sprite;
Sprite Black_Tile;
Sprite SPR_Text;
Sprite BG_Title_Names;
Sprite SPR_BlinkyTarget;
Sprite SPR_InkyTarget;
Sprite SPR_PinkyTarget;
Sprite SPR_ClydeTarget;
Sprite BG_Maze;
Sprite SPR_Pac;
Sprite SPR_Blinky;
Sprite SPR_Inky;
Sprite SPR_Pinky;
Sprite SPR_Clyde;
Sprite SPR_Scared_Ghost;
typedef struct Actor Actor;
Actor ACT_Generic;
Actor ACT_Pac;
Actor ACT_Blinky;
Actor ACT_Inky;
Actor ACT_Pinky;
Actor ACT_Clyde;

int RandomTable[512] = {419,199,377,34,38,7,109,122,18,487,340,198,305,379,372,46,342,281,406,459,233,290,164,61,143,227,303,361,301,325,489,129,285,40,246,413,410,244,264,409,336,37,152,469,502,327,196,389,259,436,499,172,432,183,41,144,224,151,480,453,87,124,48,341,339,412,161,298,338,296,108,446,279,203,189,157,276,24,420,470,384,442,119,84,137,467,438,99,146,182,404,77,311,49,270,481,483,488,58,14,292,316,248,441,131,104,322,439,335,193,332,10,5,510,421,458,21,422,184,360,170,445,33,158,243,116,312,331,117,352,494,426,401,346,22,433,498,171,478,390,323,25,254,110,364,266,284,213,293,50,407,280,252,492,491,57,66,148,225,493,185,269,508,19,283,387,288,68,215,287,496,394,398,431,416,393,477,291,42,147,102,423,82,268,231,396,234,465,302,272,355,455,128,328,263,256,417,114,103,505,437,509,357,457,36,65,350,4,349,238,32,399,187,156,400,220,240,304,191,415,430,362,307,20,1,367,395,169,315,71,277,162,121,319,43,166,262,363,353,67,6,112,127,211,253,206,376,130,177,247,142,245,74,294,100,0,28,497,228,194,485,217,29,26,375,501,97,52,75,202,324,70,219,260,381,237,330,216,64,56,374,479,447,320,452,167,257,78,425,23,27,13,195,258,462,136,351,90,163,388,310,286,385,204,251,383,81,278,461,212,89,226,444,2,391,486,140,165,73,113,261,139,506,500,207,197,449,386,405,51,451,200,179,369,232,454,218,365,95,300,429,176,354,435,326,190,295,249,106,126,472,297,271,134,222,105,62,411,471,149,475,223,343,115,490,80,378,141,241,178,267,94,188,3,309,83,205,54,173,334,155,201,138,250,133,359,443,17,402,265,397,511,230,428,463,47,424,39,503,460,368,456,9,382,107,306,507,482,31,85,235,8,476,450,466,392,345,153,69,11,333,414,91,30,63,53,72,174,79,484,59,313,495,329,440,275,86,45,96,274,145,358,160,15,44,210,101,408,373,120,464,181,380,347,154,348,434,60,168,273,221,282,427,370,12,192,118,448,366,314,35,88,255,132,337,318,208,229,308,186,135,92,55,123,214,209,321,159,76,150,317,98,180,299,289,242,125,356,473,403,236,474,111,371,344,239,93,418,175,16,504,468};
int RandomIndex = -1;
int GhostMode = Scatter;

/*0 free space
  1 wall
  2 ghost no turn
  3 pen gate
  4 intersection/ghost turn node
*/
//if you squint really hard, you can vaguely make out the shape of the maze!
byte MazeTiles[36][28] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,4,0,0,0,0,4,0,0,0,0,0,4,1},
    {1,0,1,1,1,1,0,1,1,1,1,1,0,1},
    {1,0,1,0,0,1,0,1,0,0,0,1,0,1},
    {1,0,1,1,1,1,0,1,1,1,1,1,0,1},
    {1,4,0,0,0,0,4,0,0,4,0,0,4,0},
    {1,0,1,1,1,1,0,1,1,0,1,1,1,1},
    {1,0,1,1,1,1,0,1,1,0,1,1,1,1},
    {1,4,0,0,0,0,4,1,1,4,0,0,4,1},
    {1,1,1,1,1,1,0,1,1,1,1,1,0,1},
    {0,0,0,0,0,1,0,1,1,1,1,1,0,1},
    {0,0,0,0,0,1,0,1,1,4,0,2,2,2},
    {0,0,0,0,0,1,0,1,1,0,1,1,1,3},
    {1,1,1,1,1,1,0,1,1,0,1,4,4,4},
    {0,0,0,0,0,0,4,0,0,4,1,4,4,4},
    {1,1,1,1,1,1,0,1,1,0,1,4,4,4},
    {0,0,0,0,0,1,0,1,1,0,1,1,1,1},
    {0,0,0,0,0,1,0,1,1,4,0,0,0,0},
    {0,0,0,0,0,1,0,1,1,0,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,0,1,1,1,1},
    {1,4,0,0,0,0,4,0,0,4,0,0,4,1},
    {1,0,1,1,1,1,0,1,1,1,1,1,0,1},
    {1,0,1,1,1,1,0,1,1,1,1,1,0,1},
    {1,4,0,4,1,1,4,0,0,4,0,2,2,2},
    {1,1,1,0,1,1,0,1,1,0,1,1,1,1},
    {1,1,1,0,1,1,0,1,1,0,1,1,1,1},
    {1,4,0,4,0,0,4,1,1,4,0,0,4,1},
    {1,0,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,4,0,0,0,0,0,0,0,0,0,0,4,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
byte Dots[36][28] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,0,0,0,0,1,0,0,0,0,0,1,0},
    {0,2,0,0,0,0,1,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0,0,0,0,0,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {0,1,0,0,0,0,1,0,0,1,0,0,0,0},
    {0,1,0,0,0,0,1,0,0,1,0,0,0,0},
    {0,1,1,1,1,1,1,0,0,1,1,1,1,0},
    {0,0,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,0,0,0,0,1,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0,0,0,0,0,1,0},
    {0,2,1,1,0,0,1,1,1,1,1,1,1,0},
    {0,0,0,1,0,0,1,0,0,1,0,0,0,0},
    {0,0,0,1,0,0,1,0,0,1,0,0,0,0},
    {0,1,1,1,1,1,1,0,0,1,1,1,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,1,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
byte DotsLeft = 1;

void MazeInit()
{
    // Mirror the maze
    for(int i = 0; i < 36; i++)
    {
        for(int j = 0; j < 14; j++)
        {
            MazeTiles[i][27-j] = MazeTiles[i][j];
            Dots[i][27-j] = Dots[i][j];
        }
    }
}

int SDLInit()
{
    //sdl init
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        return(-1);
    }
    //get displaysize
    if (SDL_GetDisplayBounds(0, &Display) != 0)
    {
        SDL_Quit();
        return(-1);
    }
    else //decide window size
    {
        Scale = ((Display.w/Display.h) >= 1) ? Display.h/ScreenHeight : Display.w/ScreenWidth;
    }
    //window init
    Window = SDL_CreateWindow("Pac-Man",100,100,ScreenWidth*Scale,ScreenHeight*Scale,0);
    SDL_Surface* WindowIcon = SDL_LoadBMP("Assets/Icon.bmp");
    SDL_SetWindowIcon(Window,WindowIcon);
    SDL_FreeSurface(WindowIcon);
    if(!Window)
    {
        SDL_Quit();
        return(-1);
    }
    //render init
    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_PRESENTVSYNC); //choose best renderer, non resizeable
    if(!Renderer)
    {
        SDL_DestroyWindow(Window);
        SDL_Quit();
        return(-1);
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    SDL_RenderSetLogicalSize(Renderer,ScreenWidth,ScreenHeight);
    return(0);
}

int SDLRestart()
{
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    //window init
    Window = SDL_CreateWindow("Pac-Man",100,100,ScreenWidth*Scale,ScreenHeight*Scale,0);
    SDL_Surface* WindowIcon = SDL_LoadBMP("Assets/Icon.bmp");
    SDL_SetWindowIcon(Window,WindowIcon);
    SDL_FreeSurface(WindowIcon);
    if(!Window)
    {
        //printf("Window creation failed with error: %s\n", SDL_GetError());
        SDL_Quit();
        return(-1);
    }
    //render init
    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_PRESENTVSYNC); //choose best renderer, non resizeable
    if(!Renderer)
    {
        //printf("Renderer creation failed with error: %s\n", SDL_GetError());
        SDL_DestroyWindow(Window);
        SDL_Quit();
        return(-1);
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    SDL_RenderSetLogicalSize(Renderer,ScreenWidth,ScreenHeight);
    return(0);
}

SDL_Texture* MakeTexture(char* Image)
{
    SDL_Surface* Surface = SDL_LoadBMP(Image);
    if(!Surface)
    {
        //printf("Unable to load image %s with error: %s\n", Image, SDL_GetError());
        return(NULL);
    }
    SDL_Texture* Texture = SDL_CreateTextureFromSurface(Renderer, Surface);
    SDL_FreeSurface(Surface);
    if(!Texture)
    {
        //printf("Unable to create texture %s with error: %s\n", Image, SDL_GetError());
        return(NULL);
    }
    return(Texture);
}

void SpriteInit(int Restart) //defines sprite x y width height direction *frames
{
    Black_Tile.Sprite = MakeTexture("Assets/Blackout_Tile.bmp");
    SPR_Text.Sprite = MakeTexture("Assets/Text.bmp");
    BG_Maze.Sprite = MakeTexture("Assets/Maze.bmp");
    BG_Title_Names.Sprite = MakeTexture("Assets/TitleNames.bmp");
    SPR_Pac.Sprite = MakeTexture("Assets/Pac_WakaWaka.bmp");
    SPR_BlinkyTarget.Sprite = MakeTexture("Assets/Debug_Target.bmp");
    SPR_InkyTarget.Sprite = MakeTexture("Assets/Debug_Target.bmp");
    SPR_PinkyTarget.Sprite = MakeTexture("Assets/Debug_Target.bmp");
    SPR_ClydeTarget.Sprite = MakeTexture("Assets/Debug_Target.bmp");
    SPR_Blinky.Sprite = MakeTexture("Assets/Blinky.bmp");
    SPR_Inky.Sprite = MakeTexture("Assets/Inky.bmp");
    SPR_Pinky.Sprite = MakeTexture("Assets/Pinky.bmp");
    SPR_Clyde.Sprite = MakeTexture("Assets/Clyde.bmp");
    SPR_Scared_Ghost.Sprite = MakeTexture("Assets/Scared.bmp");
    if(!Restart)
    {
        Black_Tile.Width = 8;
        Black_Tile.Height = 8;
        //text
        SPR_Text.Width = 8;
        SPR_Text.Height = 8;
        //title
        BG_Title_Names.Width = 224;
        BG_Title_Names.Height = 288;
        BG_Title_Names.X = 0;
        BG_Title_Names.Y = 0;
        BG_Title_Names.HorizontalOffset = 0;
        BG_Title_Names.VerticalOffset = 0;
        BG_Title_Names.Frames[0] = 0;//us
        BG_Title_Names.Frames[1] = 1;//jap
        //maze
        BG_Maze.Type = Maze;
        BG_Maze.Width = 224;
        BG_Maze.Height = 288;
        BG_Maze.X = 0;
        BG_Maze.Y = 0;
        //pac
        SPR_Pac.Type = Pac;
        SPR_Pac.Height = 13;
        SPR_Pac.Width = 13;
        SPR_Pac.HorizontalOffset = -2;
        SPR_Pac.VerticalOffset = -2;
        SPR_Pac.NumberOfFrames = 4;
        SPR_Pac.FramespPerFrame = 3;
        SPR_Pac.Frames[0] = 0;
        SPR_Pac.Frames[1] = 1;
        SPR_Pac.Frames[2] = 2;
        SPR_Pac.Frames[3] = 1;
        //debug blinky
        SPR_BlinkyTarget.Type = Debug_Target;
        SPR_BlinkyTarget.Width = 8;
        SPR_BlinkyTarget.Height = 8;
        SPR_BlinkyTarget.HorizontalOffset = 0;
        SPR_BlinkyTarget.VerticalOffset = 0;
        SPR_BlinkyTarget.Frame = 0;
        SPR_BlinkyTarget.Frames[0] = 0;
        SPR_BlinkyTarget.Frames[1] = 1;
        SPR_BlinkyTarget.Frames[2] = 2;
        SPR_BlinkyTarget.Frames[3] = 3;
        SPR_BlinkyTarget.Frames[4] = 4;
        //debug inky
        SPR_InkyTarget.Type = Debug_Target;
        SPR_InkyTarget.Width = 8;
        SPR_InkyTarget.Height = 8;
        SPR_InkyTarget.HorizontalOffset = 0;
        SPR_InkyTarget.VerticalOffset = 0;
        SPR_InkyTarget.Frame = 1;
        SPR_InkyTarget.Frames[0] = 0;
        SPR_InkyTarget.Frames[1] = 1;
        SPR_InkyTarget.Frames[2] = 2;
        SPR_InkyTarget.Frames[3] = 3;
        SPR_InkyTarget.Frames[4] = 4;
        //debug pinky
        SPR_PinkyTarget.Type = Debug_Target;
        SPR_PinkyTarget.Width = 8;
        SPR_PinkyTarget.Height = 8;
        SPR_PinkyTarget.HorizontalOffset = 0;
        SPR_PinkyTarget.VerticalOffset = 0;
        SPR_PinkyTarget.Frame = 2;
        SPR_PinkyTarget.Frames[0] = 0;
        SPR_PinkyTarget.Frames[1] = 1;
        SPR_PinkyTarget.Frames[2] = 2;
        SPR_PinkyTarget.Frames[3] = 3;
        SPR_PinkyTarget.Frames[4] = 4;
        //debug clyde
        SPR_ClydeTarget.Type = Debug_Target;
        SPR_ClydeTarget.Width = 8;
        SPR_ClydeTarget.Height = 8;
        SPR_ClydeTarget.HorizontalOffset = 0;
        SPR_ClydeTarget.VerticalOffset = 0;
        SPR_ClydeTarget.Frame = 3;
        SPR_ClydeTarget.Frames[0] = 0;
        SPR_ClydeTarget.Frames[1] = 1;
        SPR_ClydeTarget.Frames[2] = 2;
        SPR_ClydeTarget.Frames[3] = 3;
        SPR_ClydeTarget.Frames[4] = 4;
        //blinky
        SPR_Blinky.Type = Ghost;
        SPR_Blinky.Height = 14;
        SPR_Blinky.Width = 14;
        SPR_Blinky.HorizontalOffset = -3;
        SPR_Blinky.VerticalOffset = -3;
        SPR_Blinky.FramespPerFrame = 8;
        SPR_Blinky.NumberOfFrames = 2;
        SPR_Blinky.Frames[0] = 0;
        SPR_Blinky.Frames[1] = 1;
        //inky
        SPR_Inky.Type = Ghost;
        SPR_Inky.Height = 14;
        SPR_Inky.Width = 14;
        SPR_Inky.HorizontalOffset = -3;
        SPR_Inky.VerticalOffset = -3;
        SPR_Inky.FramespPerFrame = 8;
        SPR_Inky.NumberOfFrames = 2;
        SPR_Inky.Frames[0] = 0;
        SPR_Inky.Frames[1] = 1;
        //Pinky
        SPR_Pinky.Type = Ghost;
        SPR_Pinky.Height = 14;
        SPR_Pinky.Width = 14;
        SPR_Pinky.HorizontalOffset = -3;
        SPR_Pinky.VerticalOffset = -3;
        SPR_Pinky.FramespPerFrame = 8;
        SPR_Pinky.NumberOfFrames = 2;
        SPR_Pinky.Frames[0] = 0;
        SPR_Pinky.Frames[1] = 1;
        //clyde
        SPR_Clyde.Type = Ghost;
        SPR_Clyde.Height = 14;
        SPR_Clyde.Width = 14;
        SPR_Clyde.HorizontalOffset = -3;
        SPR_Clyde.VerticalOffset = -3;
        SPR_Clyde.FramespPerFrame = 8;
        SPR_Clyde.NumberOfFrames = 2;
        SPR_Clyde.Frames[0] = 0;
        SPR_Clyde.Frames[1] = 1;
        //scaredy cat
        SPR_Scared_Ghost.Type = Ghost;
        SPR_Scared_Ghost.Height = 14;
        SPR_Scared_Ghost.Width = 14;
        SPR_Scared_Ghost.HorizontalOffset = -3;
        SPR_Scared_Ghost.VerticalOffset = -3;
        SPR_Scared_Ghost.FramespPerFrame = 8;
        SPR_Scared_Ghost.NumberOfFrames = 2;
        SPR_Scared_Ghost.Frames[0] = 0;
        SPR_Scared_Ghost.Frames[1] = 1;
    }
}

void ActorInit() //defines x y direction type(*s) movespeed target
{
    //pac
    ACT_Pac.X = 104;
    ACT_Pac.Y = 208;
    ACT_Pac.Direction = Left;
    ACT_Pac.Type = Pac;
    ACT_Pac.FramesPerPixel = 1;
    //blinky
    ACT_Blinky.X = 104;
    ACT_Blinky.Y = 112;
    ACT_Blinky.Direction = Left;
    ACT_Blinky.Type = Ghost;
    ACT_Blinky.GhostType = Blinky;
    ACT_Blinky.FramesPerPixel = 1;
    //inky
    ACT_Inky.X = 88;
    ACT_Inky.Y = 112;
    ACT_Inky.Direction = Left;
    ACT_Inky.Type = Ghost;
    ACT_Inky.GhostType = Inky;
    ACT_Inky.FramesPerPixel = 1;
    //pinky
    ACT_Pinky.X = 136;
    ACT_Pinky.Y = 112;
    ACT_Pinky.Direction = Left;
    ACT_Pinky.Type = Ghost;
    ACT_Pinky.GhostType = Pinky;
    ACT_Pinky.FramesPerPixel = 1;
    //clyde
    ACT_Clyde.X = 120;
    ACT_Clyde.Y = 112;
    ACT_Clyde.Direction = Left;
    ACT_Clyde.Type = Ghost;
    ACT_Clyde.GhostType = Clyde;
    ACT_Clyde.FramesPerPixel = 1;
}