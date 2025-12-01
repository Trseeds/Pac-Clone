#include "Init.h"
#include "Engine.h"

/*
TODO:
    give pacman cornering abilities
    complete title screen
    add cutscenes
    create reset function for level completions and gamestate changes
    allow ghosts to be eaten, enter the pen, and exit the pen
    kill pacman
    level changes
    maze tunnels
    use a reversed directions array instead of switch statements
    config.ini implementation
BUGS:
    scared ghosts can get stuck
    pacman turns like a maniac (finish TurnPacMan)
    pacman can eat maze tiles when offscreen which somehow also affects ghost movement (not important in real gameplay, but interesting nonetheless)
*/

int main(int argc, char *argv[])
{
    //init
    SDLInit();
    MazeInit();
    SpriteInit(0);
    ActorInit();
    while (Running)
    {
        while (SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                Running = 0;
            }
        }
        Input();
        if(GameState == Maze_Chase)
        {
            if(!DotsLeft)
            {
                Running = 0;
            }
            if(CanMoveInDirection(&ACT_Pac))
            {
                MoveInDirection(&ACT_Pac);
                EatDot(&ACT_Pac);
            }
            DoGhostAi(&ACT_Blinky);
            DoGhostAi(&ACT_Inky);
            DoGhostAi(&ACT_Pinky);
            DoGhostAi(&ACT_Clyde);
        }
        Render();
        SDL_Delay(Framerate);
        //getchar();
    }
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    SDL_Quit();
    return(0);
}