#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <string>
#include <iostream>

#ifdef __has_include
#if __has_include(<SDL2/SDL.h>)
    #include <SDL2/SDL.h>

    SDL_Window *win = nullptr;
    SDL_Renderer *ren = nullptr;

    int init_graphics(std::string title, int width, int height) {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return 1;
        }
        
        win = SDL_CreateWindow(title.c_str(), 100, 100, width, height, SDL_WINDOW_SHOWN);
        if (win == nullptr) {
            std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return 1;
        }

        ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (ren == nullptr) {
            SDL_DestroyWindow(win);
            std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return 1;
        }

        return 0;
    }

    void close_graphics() {
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
    }

    void clear_screen() {
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
    }

    void update_screen() {
        SDL_RenderPresent(ren);
    }

    void draw_rect(int x, int y, int w, int h) {
        SDL_Rect rect;
        rect.x = x;
        rect.y = y;
        rect.w = w;
        rect.h = h;

        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderFillRect(ren, &rect);
    }

#else
    // SDL2 is not available, provide alternative implementations or stubs

    int init_graphics(std::string title, int width, int height) {
        std::cerr << "Graphics not supported (SDL2 not available)" << std::endl;
        return 1;
    }

    void close_graphics() {
        std::cerr << "Graphics not supported (SDL2 not available)" << std::endl;
    }

    void clear_screen() {
        std::cerr << "Graphics not supported (SDL2 not available)" << std::endl;
    }

    void update_screen() {
        std::cerr << "Graphics not supported (SDL2 not available)" << std::endl;
    }

    void draw_rect(int x, int y, int w, int h) {
        std::cerr << "Graphics not supported (SDL2 not available)" << std::endl;
    }

#endif
#else
    #error "__has_include not supported by your compiler. Please install SDL2 to use graphics features."
#endif

#endif // GRAPHICS_HPP