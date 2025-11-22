#include "Init.h"
#include "GhostAI.h"

byte CharToFrame(byte Char);
void IntToString(char* Output, int Number, byte Offset);
byte IntToChar(byte Number);

void DrawSprite(Sprite* Sprite, Actor* Actor)
{
    SDL_Rect Source;
    SDL_Rect Destination;
    Source.w = Sprite->Width;
    Source.h = Sprite->Height;
    Destination.w = Sprite->Width;
    Destination.h = Sprite->Height;
    if(Actor == NULL || Actor == 1)
    {
        Source.x = Sprite->Width*Sprite->Frame;
        Source.y = 0;
        Destination.x = Sprite->X;
        Destination.y = Sprite->Y;
    }
    else
    {
        Source.x = Sprite->Width*Sprite->Frames[Sprite->Frame];
        Source.y = Actor->Direction*Sprite->Height;
        Destination.x = Actor->X+Sprite->HorizontalOffset;
        Destination.y = Actor->Y+Sprite->VerticalOffset;
    }
    if(Actor == 1)
    {
        Source.x = Sprite->Width*Sprite->Frame;
        Source.y = ACT_Generic.Direction*8;
    }
    SDL_RenderCopy(Renderer,Sprite->Sprite,&Source,&Destination);
}

void DrawText(int X, int Y, int Color, char* String)
{
    for(int i = 0; i < 8; i++)
    {
        SPR_Text.Frame = CharToFrame(String[i]);
        SPR_Text.X = X+i*8;
        SPR_Text.Y = Y;
        ACT_Generic.Direction = Color;
        DrawSprite(&SPR_Text,1);
    }
}

void CoverDots()
{
    DotsLeft = 0;
    for(int i = 0; i < 36; i++)
    {
        for(int j = 0; j < 28; j++)
        {
            if(!Dots[i][j] && MazeTiles[i][j] != 1)
            {
                Black_Tile.X = j*8;
                Black_Tile.Y = i*8;
                DrawSprite(&Black_Tile,NULL);
            }
            if(Dots[i][j])
            {
                DotsLeft++;
            }
        }
    }
    DotsLeft -= 3; //im not sure why, but there are always three ernonious dots left when they all appear to have been eaten
}

void AnimateSprite(Sprite* Sprite)
{
    Sprite->Frame = Sprite->Frames[(Frame / Sprite->FramespPerFrame) % Sprite->NumberOfFrames];
}

int AlignedToGrid(Actor* Actor)
{
    return ((Actor->X % 8 == 0) && (Actor->Y % 8 == 0));
}

int CanMoveInDirection(Actor* Actor)
{
    int TargetX = Actor->X/8;
    int TargetY = Actor->Y/8;
    switch (Actor->Direction)
    {
        case Right:
            TargetX = (Actor->X + 8) / 8; 
            break;
        case Left:
            TargetX = (Actor->X-1) / 8;
            break;
        case Up:
            TargetY = (Actor->Y-1) / 8;
            break;
        case Down:
            TargetY = (Actor->Y + 8) / 8;
            break;
    }
    if(Actor->Type == Pac)
    {
        if(MazeTiles[TargetY][TargetX] != 1 && MazeTiles[TargetY][TargetX] != 3)
        {
            return(1);
        }
        return(0);
    }
    else
    {
        if(MazeTiles[TargetY][TargetX] != 1)
        {
            return(1);
        }
        return(0);
    }
}

void MoveInDirection(Actor* Actor)
{
    if(Frame%Actor->FramesPerPixel || Actor->FramesPerPixel == 1)
    {
        switch (Actor->Direction)
        {
        case Right:
            Actor->X++;
            break;
        case Left:
            Actor->X--;
            break;
        case Up:
            Actor->Y--;
            break;
        case Down:
            Actor->Y++;
            break;
        default:
            break;
        }
    }
}

void DoGhostAi(Actor* Ghost)
{
    if(Frame%Ghost->FramesPerPixel || Ghost->FramesPerPixel == 1)
    {
        GhostSetTarget(Ghost,&ACT_Pac,&ACT_Blinky);
        if(AlignedToGrid(Ghost) && (MazeTiles[Ghost->Y/8][Ghost->X/8] == 4 || MazeTiles[Ghost->Y/8][Ghost->X/8] == 2))
        {
            GhostMove(Ghost);
        }
        if(CanMoveInDirection(Ghost))
        {
            MoveInDirection(Ghost);
        }
    }
}

void EatDot(Actor* Pac)
{
    byte TargetX = Pac->X;
    int TargetY = Pac->Y;
    switch (Pac->Direction)
    {
        case Up:
            TargetY += 4;
            break;
        case Left:
            TargetX += 4;
            break;
        default:
            break;
    }
    if(Dots[TargetY/8][TargetX/8])
    {
        Dots[TargetY/8][TargetX/8] = 0;
    }
}

void Render()
{
    byte String[8] = {41,41,41,41,41,41,41,41};
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
    SDL_RenderClear(Renderer);
    if(GameState == Title_Names)
    {
        BG_Title_Names.Frame = Region;
        ACT_Generic.Direction = TitleType;
        ACT_Generic.X = 0;
        ACT_Generic.Y = 0;
        DrawSprite(&BG_Title_Names,&ACT_Generic);
        IntToString(String,HiScores[0],4);
        if(TitleType > 1)
        {
            if(TitleType == Scores)
            {
                for(int i = 1; i < 8; i += 2)
                {
                    for(int j = 0; j < 8; j++)
                    {
                        Names[i][j] = ' ';
                    }
                }
                IntToString(Names[1],HiScores[0],7);
                IntToString(Names[3],HiScores[1],7);
                IntToString(Names[5],HiScores[2],7);
                IntToString(Names[7],HiScores[3],7);
            }
            DrawText(65,56,Red,Names[0]);
            DrawText(153,56,Red,Names[1]);
            DrawText(65,80,Pink,Names[2]);
            DrawText(153,80,Pink,Names[3]);
            DrawText(65,104,Blue,Names[4]);
            DrawText(153,104,Blue,Names[5]);
            DrawText(65,128,Orange,Names[6]);
            DrawText(153,128,Orange,Names[7]);
        }
        DrawText(88,8,White,String);
    }
    if(GameState == Maze_Chase)
    {
        BG_Maze.Frame = 0;
        DrawSprite(&BG_Maze,NULL); //blue
        BG_Maze.Frame = 2;
        DrawSprite(&BG_Maze,NULL); //pellets
        BG_Maze.Frame = 3;
        if(Debug)
        {
            DrawSprite(&BG_Maze,NULL); //tiles
        }
        CoverDots();
        DrawSprite(&SPR_Pac,&ACT_Pac);
        if(CanMoveInDirection(&ACT_Pac))
        {
            AnimateSprite(&SPR_Pac);
        }
        else
        {
            SPR_Pac.Frame = 1;
        }
        if(GhostMode != Scared)
        {
            DrawSprite(&SPR_Blinky,&ACT_Blinky);
            AnimateSprite(&SPR_Blinky);
            DrawSprite(&SPR_Inky,&ACT_Inky);
            AnimateSprite(&SPR_Inky);
            DrawSprite(&SPR_Pinky,&ACT_Pinky);
            AnimateSprite(&SPR_Pinky);
            DrawSprite(&SPR_Clyde,&ACT_Clyde);
            AnimateSprite(&SPR_Clyde);
        }
        else
        {
            ACT_Blinky.Direction = 0;
            DrawSprite(&SPR_Scared_Ghost,&ACT_Blinky);
            DrawSprite(&SPR_Scared_Ghost,&ACT_Inky);
            DrawSprite(&SPR_Scared_Ghost,&ACT_Pinky);
            DrawSprite(&SPR_Scared_Ghost,&ACT_Clyde);
            AnimateSprite(&SPR_Scared_Ghost);
        }
        if(Debug)
        {
            SPR_BlinkyTarget.X = ACT_Blinky.TargetX;
            SPR_BlinkyTarget.Y = ACT_Blinky.TargetY;
            SPR_InkyTarget.X = ACT_Inky.TargetX;
            SPR_InkyTarget.Y = ACT_Inky.TargetY;
            SPR_PinkyTarget.X = ACT_Pinky.TargetX;
            SPR_PinkyTarget.Y = ACT_Pinky.TargetY;
            SPR_ClydeTarget.X = ACT_Clyde.TargetX;
            SPR_ClydeTarget.Y = ACT_Clyde.TargetY;
            DrawSprite(&SPR_BlinkyTarget,NULL);
            DrawSprite(&SPR_InkyTarget,NULL);
            DrawSprite(&SPR_PinkyTarget,NULL);
            DrawSprite(&SPR_ClydeTarget,NULL);
        }
    }
    SDL_RenderPresent(Renderer);
    if(Frame == 60)
    {
        Frame = 1;
    }
    else
    {
        Frame++;
    }
}

void Input()
{
    Key = SDL_GetKeyboardState(NULL);
    if (Key[SDL_SCANCODE_UP])
    {
        int OldDirection = ACT_Pac.Direction;
        ACT_Pac.Direction = Up;
        if(!CanMoveInDirection(&ACT_Pac) || !AlignedToGrid(&ACT_Pac))
        {
            ACT_Pac.Direction = OldDirection;
        }
    }
    else if (Key[SDL_SCANCODE_DOWN])
    {
        int OldDirection = ACT_Pac.Direction;
        ACT_Pac.Direction = Down;
        if(!CanMoveInDirection(&ACT_Pac) || !AlignedToGrid(&ACT_Pac))
        {
            ACT_Pac.Direction = OldDirection;
        }
    }
    else if (Key[SDL_SCANCODE_RIGHT])
    {
        int OldDirection = ACT_Pac.Direction;
        ACT_Pac.Direction = Right;
        if(!CanMoveInDirection(&ACT_Pac) || !AlignedToGrid(&ACT_Pac))
        {
            ACT_Pac.Direction = OldDirection;
        }
    }
    else if (Key[SDL_SCANCODE_LEFT])
    {
        int OldDirection = ACT_Pac.Direction;
        ACT_Pac.Direction = Left;
        if(!CanMoveInDirection(&ACT_Pac) || !AlignedToGrid(&ACT_Pac))
        {
            ACT_Pac.Direction = OldDirection;
        }
    }
    if (Key[SDL_SCANCODE_ESCAPE])
    {
        Running = 0;
    }
    if (Key[SDL_SCANCODE_EQUALS])
    {
        Scale++;
        SDLRestart();
        SpriteInit(1);
    }
    if (Key[SDL_SCANCODE_MINUS] && Scale > 1)
    {
        Scale--;
        SDLRestart();
        SpriteInit(1);
    }
    if (Key[SDL_SCANCODE_R])
    {
        SDLRestart();
        MazeInit();
        SpriteInit(0);
        ActorInit();
    }
    if (Key[SDL_SCANCODE_D])
    {
        Debug = !Debug;
    }
    if (Key[SDL_SCANCODE_1])
    {
        GhostMode = Chase;
    }
    if (Key[SDL_SCANCODE_2])
    {
        GhostMode = Scatter;
    }
    if (Key[SDL_SCANCODE_3])
    {
        GhostMode = Scared;
    }
    if (Key[SDL_SCANCODE_9])
    {
        GameState = Maze_Chase;
    }
    if (Key[SDL_SCANCODE_0])
    {
        GameState = Title_Names;
    }
}

byte CharToFrame(byte Char)
{
    switch (Char)
    {
        case 'A':
            return(0);
            break;
        case 'B':
            return(1);
            break;
        case 'C':
            return(2);
            break;
        case 'D':
            return(3);
            break;
        case 'E':
            return(4);
            break;
        case 'F':
            return(5);
            break;
        case 'G':
            return(6);
            break;
        case 'H':
            return(7);
            break;
        case 'I':
            return(8);
            break;
        case 'J':
            return(9);
            break;
        case 'K':
            return(10);
            break;
        case 'L':
            return(11);
            break;
        case 'M':
            return(12);
            break;
        case 'N':
            return(13);
            break;
        case 'O':
            return(14);
            break;
        case 'P':
            return(15);
            break;
        case 'Q':
            return(16);
            break;
        case 'R':
            return(17);
            break;
        case 'S':
            return(18);
            break;
        case 'T':
            return(19);
            break;
        case 'U':
            return(20);
            break;
        case 'V':
            return(21);
            break;
        case 'W':
            return(22);
            break;
        case 'X':
            return(23);
            break;
        case 'Y':
            return(24);
            break;
        case 'Z':
            return(25);
            break;
        case '0':
            return(26);
            break;
        case '1':
            return(27);
            break;
        case '2':
            return(28);
            break;
        case '3':
            return(29);
            break;
        case '4':
            return(30);
            break;
        case '5':
            return(31);
            break;
        case '6':
            return(32);
            break;
        case '7':
            return(33);
            break;
        case '8':
            return(34);
            break;
        case '9':
            return(35);
            break;
        case '!':
            return(36);
            break;
        case '/':
            return(37);
            break;
        case '\"':
            return(38);
            break;
        case '-':
            return(39);
            break;
        case '+':
            return(40);
            break;
        case ' ':
            return(41);
            break;
        case '_':
            return(42);
            break;
        default:
            return(42);
            break;
    }
}
void IntToString(char* Output, int Number, byte Offset)
{
    byte i = Offset;
    while(Number > 0)
    {
        Output[i] = IntToChar(Number%10);
        Number = Number / 10;
        i--;
    }
}
byte IntToChar(byte Number)
{
    return(Number+48);
}