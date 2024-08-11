#include <SDL2/SDL.h>
#include "game.h"

int main()
{
    end_t ending;
    std::string endMessage;
    Game game(&ending);

    Uint64 nextTick = 0;
    while (game.running)
    {
        game.handleEvents();

        Uint64 currentTick = SDL_GetTicks64();
        if (currentTick >= nextTick)
        {
            nextTick = currentTick + game.deltaTime;

            game.update();
            game.draw();

        }

        if (game.paused) continue;
    }

    switch (ending)
    {
        case win: endMessage = "You won!"; break;
        case loss: endMessage = "You loose :("; break;
        case error: endMessage = "An error ocurred :/"; break;
        case quit: endMessage = "Good bye!"; break;
    }

    std::cout << endMessage << "\n";

    return 0;
}

