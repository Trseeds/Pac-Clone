#include "Init.h"

/*blinky 25 -1
pinky 2 -1
inky 27 34
clyde 0 34*/

int Distance(int Pos, int Target)
{
    return((Pos - Target)*(Pos - Target));
}

int RandomNumber(int Max)
{
    if(RandomIndex == 512)
    {
        RandomIndex = -1;
    }
    RandomIndex++;
    return(RandomTable[RandomIndex]%Max);
}

int ShortestTurn(Actor* Ghost, int* ViableTurns)
{
    unsigned int BestDistance = -1; //underflows to highest value
    int BestDirection = -1;
    unsigned int Distances[4] = {-1,-1,-1,-1};
    if(ViableTurns[Right])
    {
        Distances[Right] = Distance(Ghost->X+8,Ghost->TargetX) + Distance(Ghost->Y,Ghost->TargetY);
    }
    if(ViableTurns[Down])
    {
        Distances[Down] = Distance(Ghost->X,Ghost->TargetX) + Distance(Ghost->Y+8,Ghost->TargetY);
    }
    if(ViableTurns[Left])
    {
        Distances[Left] = Distance(Ghost->X-8,Ghost->TargetX) + Distance(Ghost->Y,Ghost->TargetY);
    }
    if(ViableTurns[Up])
    {
        Distances[Up] = Distance(Ghost->X,Ghost->TargetX) + Distance(Ghost->Y-8,Ghost->TargetY);
    }
    for(int i = 0; i < 4; i++)
    {
        if(Distances[i] < BestDistance)
        {
            BestDistance = Distances[i];
            BestDirection = i;
        }
    }
    for(int i = 0; i < 4; i++)
    {
        if(Distances[Up] == BestDistance && i != ViableTurns[Up])
        {
            return(Up);
        }
        if(Distances[Left] == BestDistance && i != ViableTurns[Left])
        {
            return(Left);
        }
        if(Distances[Down] == BestDistance && i != ViableTurns[Down])
        {
            return(Down);
        }
        if(Distances[Right] == BestDistance && i != ViableTurns[Right])
        {
            return(Right);
        }    
    }
    return(BestDirection);
}

int Turn(Actor* Ghost)
{
    int ViableTurns[4] = {1,1,1,1};
    int TileX = Ghost->X/8;
    int TileY = Ghost->Y/8;
    switch (Ghost->Direction)
    {
        case Right:
            ViableTurns[Left] = 0;
            break;
        case Left:
            ViableTurns[Right] = 0;
            break;
        case Down:
            ViableTurns[Up] = 0;
            break;
        case Up:
            ViableTurns[Down] = 0;
            break;
    }
    if(MazeTiles[TileY][TileX+1] == 1)
    {
        ViableTurns[Right] = 0;
    }
    if(MazeTiles[TileY][TileX-1] == 1)
    {
        ViableTurns[Left] = 0;
    }
    if(MazeTiles[TileY-1][TileX] == 1)
    {
        ViableTurns[Up] = 0;
    }
    if(MazeTiles[TileY+1][TileX] == 1)
    {
        ViableTurns[Down] = 0;
    }
    if(MazeTiles[TileY][TileX] == 2 && Ghost->Direction == Down)
    {
        ViableTurns[Up] = 0;
        ViableTurns[Down] = 0;
        ViableTurns[Right] = 1;
        ViableTurns[Left] = 1;
    }
    if(MazeTiles[TileY][TileX] == 2 && (Ghost->Direction == Left || Ghost->Direction == Right))
    {
        ViableTurns[Up] = 0;
        ViableTurns[Down] = 0;
        ViableTurns[Right] = 0;
        ViableTurns[Left] = 0;
        ViableTurns[Ghost->Direction] = 1;
    }
    return(ShortestTurn(Ghost,ViableTurns));
}

void BlinkySetTarget(Actor* Ghost, Actor* PacMan)
{
    if(GhostMode == Scatter)
    {
        Ghost->TargetX = 200;
        Ghost->TargetY = -8;
    }
    if(GhostMode == Chase)
    {
        Ghost->TargetX = PacMan->X;
        Ghost->TargetY = PacMan->Y;
    }
}

void InkySetTarget(Actor* Ghost, Actor* PacMan, Actor* Blinky)
{
    if(GhostMode == Scatter)
    {
        Ghost->TargetX = 216;
        Ghost->TargetY = 272;
    }
    else
    {
        int DistanceX;
        int DistanceY;
        int PacTargetX;
        int PacTargetY;
        switch (PacMan->Direction)
        {
            case Right:
                PacTargetX = PacMan->X+16;
                PacTargetY = PacMan->Y;
                break;
            case Left:
                PacTargetX = PacMan->X-16;
                PacTargetY = PacMan->Y;
                break;
            case Up:
                PacTargetX = PacMan->X-16;
                PacTargetY = PacMan->Y-16;
                break;
            case Down:
                PacTargetX = PacMan->X;
                PacTargetY = PacMan->Y+16;
                break;
            default:
                break;
        }
        DistanceX = PacTargetX - Blinky->X;
        DistanceY = PacTargetY - Blinky->Y;
        Ghost->TargetX = (DistanceX*2) + Blinky->X;
        Ghost->TargetY = (DistanceY*2) + Blinky->Y;
    }
}

void PinkySetTarget(Actor* Ghost, Actor* PacMan)
{
    if(GhostMode == Scatter)
    {
        Ghost->TargetX = 16;
        Ghost->TargetY = -8;
    }
    if(GhostMode == Chase)
    {
        switch (PacMan->Direction)
        {
            case Right:
                Ghost->TargetX = PacMan->X+32;
                Ghost->TargetY = PacMan->Y;
                break;
            case Left:
                Ghost->TargetX = PacMan->X-32;
                Ghost->TargetY = PacMan->Y;
                break;
            case Up:
                Ghost->TargetX = PacMan->X-32;
                Ghost->TargetY = PacMan->Y-32;
                break;
            case Down:
                Ghost->TargetX = PacMan->X;
                Ghost->TargetY = PacMan->Y+32;
                break;
            default:
                break;
        }
    }
}

void ClydeSetTarget(Actor* Ghost, Actor* PacMan)
{
    if(GhostMode == Scatter)
    {
        Ghost->TargetX = 0;
        Ghost->TargetY = 272;
    }
    if(GhostMode == Chase)
    {
        int Dis = Distance(Ghost->X, PacMan->X) + Distance(Ghost->Y, PacMan->Y);
        if(Dis <= 4096) //8 tiles
        {
            Ghost->TargetX = 0;
            Ghost->TargetY = 272;
        }
        else
        {
            Ghost->TargetX = PacMan->X;
            Ghost->TargetY = PacMan->Y;
        }
    }
}

void GhostSetTarget(Actor* Ghost, Actor* PacMan, Actor* ACT_Blinky)
{
    if(GhostMode == Scared)
    {
        Ghost->TargetX = RandomNumber(224);
        Ghost->TargetY = RandomNumber(288);
        Ghost->FramesPerPixel = 2;
    }
    else
    {
        switch (Ghost->GhostType)
        {
            case Blinky:
                BlinkySetTarget(Ghost,PacMan);
                break;
            case Inky:
                InkySetTarget(Ghost,PacMan,ACT_Blinky);
                break;
            case Pinky:
                PinkySetTarget(Ghost,PacMan);
                break;
            case Clyde:
                ClydeSetTarget(Ghost,PacMan);
                break;
            default:
                break;
        }
        Ghost->FramesPerPixel = 1;
    }
}

void GhostMove(Actor* Ghost)
{
    Ghost->Direction = Turn(Ghost);
}