#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <string>
#include <iostream>


#ifdef __has_include
#if __has_include(<SDL.h>) // check if SDL.h is available, 
#include <SDL.h> 
#endif
#endif

int init_graphics();
int close_graphics();

void draw_pixel(int x, int y, int color);


int init_graphics(std::string title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    return 0;
}

int close_graphics() {
    return 0;
}

void draw_pixel(int x, int y, int color) {
    
}


#endif // GRAPHICS_HPP