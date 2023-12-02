#include "snake.h"

Snake::Snake()
{
}

Snake::Snake(SDL_Point initialPoint, SDL_Rect playground)
{
    snake.push_back(initialPoint);

    minX = playground.x;
    minY = playground.y;
    maxX = playground.x + playground.w - 1;
    maxY = playground.y + playground.h - 1;
}

Snake Snake::operator=(const Snake* src)
{
    this->snake = src->snake;

    this->minX = src->minX;
    this->minY = src->minY;
    this->maxX = src->maxX;
    this->maxY = src->maxY;

    delete(src);

    return *this;
}

event_t Snake::move(direction_t direction, SDL_Point apple)
{
    SDL_Point snakeHead = snake.front();
    SDL_Point newHead = snakeHead;

    // Check bounds
    switch (direction)
    {
        case up:
            if (snakeHead.y - 1 < minY) return died;
            else newHead.y--;
            break;
        
        case left:
            if (snakeHead.x - 1 < minX) return died;
            else newHead.x--;
            break;

        case down:
            if (snakeHead.y + 1 > maxX) return died;
            else newHead.y++;
            break;

        case right:
            if (snakeHead.x + 1 > maxX) return died;
            else newHead.x++;
            break;

        case none:
            return nothing;
            break;
    }

    // Check other parts of snake
    for (SDL_Point bodyPart : snake)
        if ( (bodyPart.x == newHead.x) && (bodyPart.y == newHead.y) ) return died;

    if ( (newHead.x == apple.x) && (newHead.y == apple.y) )
    {
        snake.insert(snake.begin(), newHead);
        return ate;
    }
    else
    {
        std::vector<SDL_Point> original = snake;
        for (int i = 1; i < snake.size(); i++)
        {
            snake[i] = original[i - 1];
        }

        snake[0] = newHead;

        return moved;
    }

    return event_error;

}


