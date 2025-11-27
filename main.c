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
BUGS:
    scared blinky always seems to get stuck to the right of his spawn
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