#include "game.h"

direction_t opposite(direction_t direction);

Game::Game()
{
    this->initialize();
}

Game::Game(end_t* addr)
{
    endingAddress = addr;
    this->initialize();
}

void Game::initialize()
{
    // Initialize
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);

    windowSize = std::min(dm.w, dm.h) * PORTION_OF_THE_SCREEN;
    windowSize -= windowSize % NUMBER_OF_BLOCKS;
    blockSize = windowSize / NUMBER_OF_BLOCKS;

    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSize, windowSize, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);

    // Initialize snake and apple
    SDL_Point initialPosition = {NUMBER_OF_BLOCKS / 2, NUMBER_OF_BLOCKS / 2};
    SDL_Rect playground = {1, 1, NUMBER_OF_BLOCKS - 2, NUMBER_OF_BLOCKS - 2};
    snake = new Snake(initialPosition, playground);

    srand(time(0));

    apple.x = rand() % playground.w + playground.x;
    apple.y = rand() % playground.h + playground.y;

    return;
}


Game::~Game()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    SDL_Quit();
}

void Game::handleEvents()
{
    SDL_Event event;
    int maximumQueueSize = 3;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) 
        {
            running = false;
            if (endingAddress) *endingAddress = quit;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if (paused) paused = false;
            direction_t direction;
            switch (event.key.keysym.sym)
            {
                case SDLK_UP:
                case SDLK_w:
                case SDLK_k:
                    direction = up;
                    break;

                case SDLK_LEFT:
                case SDLK_a:
                case SDLK_h:
                    direction = left;
                    break;

                case SDLK_DOWN:
                case SDLK_s:
                case SDLK_j:
                    direction = down;
                    break;

                case SDLK_RIGHT:
                case SDLK_d:
                case SDLK_l:
                    direction = right;
                    break;

                default: return;
            } // End switch
            
            if ( (directionQueue.size() < maximumQueueSize) && (direction != none) ) directionQueue.push(direction);

        } // End else-if keyDown
    } // End while(PollEvent())

    return;
}

direction_t opposite(direction_t direction)
{
    switch  (direction)
    {
        case up: return down; break;
        case left: return right; break;
        case down: return up; break;
        case right: return left; break;
        case none: return none; break;
    }

    return none;
}


void Game::update()
{
    direction_t direction = none;
    if (!directionQueue.empty())
    {
        direction = directionQueue.front();
        directionQueue.pop();
    }

    if ( (direction != none) && (direction != opposite(lastDirectionTaken)) ) lastDirectionTaken = direction;

    event_t event = snake.move(lastDirectionTaken, apple);

    switch (event)
    {
        case died:
            running = false;
            if (endingAddress) *endingAddress = loss;
            break;

        case ate:
            SDL_Rect playground = {1, 1, NUMBER_OF_BLOCKS - 2 , NUMBER_OF_BLOCKS - 2};
            bool appleColiding = true;
            while (appleColiding)
            {
                appleColiding = false;
                apple.x = rand() % playground.w + playground.x;
                apple.y = rand() % playground.h + playground.y;

                for (SDL_Point snakeBody : snake.get())
                {
                    if ( (apple.x == snakeBody.x) && (apple.y == snakeBody.y) ) appleColiding = true;
                }
            }
            break;
    }

    return;
}

void Game::draw()
{
    // Border and background
    {
        int borderSize = blockSize / 2;

        setRendererColor(renderer, BG_COLOR);
        SDL_RenderClear(renderer);
        setRendererColor(renderer, BORDER_COLOR);
        SDL_Rect rect = {0, 0, windowSize, borderSize};
        SDL_RenderFillRect(renderer, &rect);
        rect.w = borderSize;
        rect.h = windowSize;
        SDL_RenderFillRect(renderer, &rect);
        rect.y = windowSize - borderSize;
        rect.w = windowSize;
        rect.h = borderSize;
        SDL_RenderFillRect(renderer, &rect);
        rect.x = windowSize - borderSize;
        rect.y = 0;
        rect.w = borderSize;
        rect.h = windowSize;
        SDL_RenderFillRect(renderer, &rect);
    }
    
    // Apple and snake
    {
        SDL_Rect rect = {apple.x * blockSize, apple.y * blockSize, blockSize, blockSize};
        setRendererColor(renderer, APPLE_COLOR);
        SDL_RenderFillRect(renderer, &rect);

        setRendererColor(renderer, HEAD_COLOR);

        std::vector<SDL_Point> points = snake.get();

        rect.x = points[0].x * blockSize;
        rect.y = points[0].y * blockSize;
        SDL_RenderFillRect(renderer, &rect);

        setRendererColor(renderer, BODY_COLOR);
        // rect.w *= 3.0f/4;
        // rect.h *= 3.0f/4;
        for (int i = 1; i < points.size(); i++)
        {
            rect.x = points[i].x * blockSize; //+ rect.w / 4;
            rect.y = points[i].y * blockSize; //+ rect.h / 4;
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    SDL_RenderPresent(renderer);

    return;
}

void Game::setRendererColor(SDL_Renderer* ren, SDL_Color color)
{
    SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);
    return;
}

