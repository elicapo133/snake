#include <SDL2/SDL.h>
#include <iostream>
#include <algorithm>
#include <random>
#include <time.h>
#include <queue>
#include "snake.h"

#define NUMBER_OF_BLOCKS 15

enum end_t {win = 0, loss = 1, error = 2, quit = 4};

class Game
{
    public:
        // Methods
        Game();
        Game(end_t* addr);
        void handleEvents();
        void update();
        void draw();
        ~Game();

        // State
        bool running = true;
        bool paused = true;
        end_t* endingAddress = NULL;
        Uint64 deltaTime = 170;

    private:
        // Methods
        void initialize();
        void setRendererColor(SDL_Renderer* ren, SDL_Color color);

        // State
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        Snake snake;
        SDL_Point apple;

        const float PORTION_OF_THE_SCREEN = 0.7;

        direction_t lastDirectionTaken = none;
        std::queue<direction_t> directionQueue;

        int windowSize = 0;
        int blockSize = 0;

        const SDL_Color BG_COLOR = {0, 0, 0, 255};
        const SDL_Color BORDER_COLOR = {0, 0, 200, 255};
        const SDL_Color SNAKE_COLOR = {0, 170, 0, 255};
        const SDL_Color APPLE_COLOR = {255, 0, 0, 255};

};


