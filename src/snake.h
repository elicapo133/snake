#include <SDL2/SDL.h>
#include <vector>
#include <assert.h>
#include <iostream>

enum direction_t {up = 0, left = 1, down = 2, right = 3, none = 4};
enum event_t {nothing = 0, moved = 1, ate = 2, died = 3, event_error = 4};

class Snake
{
public:
    // Methods
    Snake(SDL_Point initialPoint, SDL_Rect playground);
    Snake();
    event_t move(direction_t direction, SDL_Point apple);
    std::vector<SDL_Point> get() { return snake; }

    Snake operator=(const Snake* src);

private:
    // State
    std::vector<SDL_Point> snake;
    int minX = 0, minY = 0, maxX = 0, maxY = 0;
};






