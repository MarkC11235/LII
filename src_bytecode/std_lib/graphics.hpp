#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>

#ifdef __has_include
#if __has_include(<SDL2/SDL.h>)
    #include <SDL2/SDL.h>

    SDL_Window *win = nullptr;
    SDL_Renderer *ren = nullptr;
    std::vector<SDL_Event> eventQueue;
    std::mutex eventMutex;
    std::atomic<bool> running(true);
    std::thread eventThread;

    /*
    Event thread to handle SDL events.
    Pushes events to the event queue that can be processed by the main thread.
    */
    int event_thread() {
        SDL_Event event;
        while (running) {
            while (SDL_PollEvent(&event)) {
                std::lock_guard<std::mutex> lock(eventMutex);
                eventQueue.push_back(event);
                if (event.type == SDL_QUIT) {
                    running = false;
                }
            }
            SDL_Delay(10); // Small delay to prevent high CPU usage
        }

        return 0;
    }

    /*
    Create a window and renderer for graphics output.
    Needs to be called before any other graphics functions.
    Sets up the event thread to handle events.
    */
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

        eventThread = std::thread(event_thread);

        return 0;
    }

    /*
    Close the window and renderer.
    Ends the event thread.
    To be called when done with graphics output.
    */
    int close_graphics() {
        running = false;
        eventThread.join();

        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();

        return 0;
    }

    /*
    Get a list of events that have occurred since the last call.
    Returns a list of strings representing the events.
    Clears the event queue after getting the events.
    */
    std::vector<Value> get_events() {
        std::vector<Value> events;
        std::lock_guard<std::mutex> lock(eventMutex);
        for (auto &event : eventQueue) {
            switch (event.type) {
                case SDL_QUIT:
                    events.push_back({Value_Type::STRING, "quit"});
                    break;
                case SDL_KEYDOWN:
                    events.push_back({Value_Type::STRING, "keydown"});
                    break;
                default:
                    events.push_back({Value_Type::STRING, "unknown"});
                    break;
            }
        }
        eventQueue.clear();
        return events;
    }

    /*
    Clear the screen to black.
    */
    int clear_screen() {
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        return 0;
    }

    /*
    Update the screen with any changes made since the last update.
    TODO: maybe make this happen automatically after every draw call? 
            -Would be inefficient, but more user-friendly becasue less functions the user has to call
    */
    int update_screen() {
        SDL_RenderPresent(ren);

        return 0;
    }

    /*
    Draw a rectangle on the screen.
    Top-left corner is at (x, y), with width w and height h.
    */
    int draw_rect(int x, int y, int w, int h) {
        SDL_Rect rect;
        rect.x = x;
        rect.y = y;
        rect.w = w;
        rect.h = h;

        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderFillRect(ren, &rect);

        return 0;
    }

#else
    // SDL2 is not available, provide alternative implementations or error messages

    int init_graphics(std::string title, int width, int height) {
        std::cerr << "Graphics not supported (SDL2 not available)" << std::endl;
        return 1;
    }

    int close_graphics() {
        std::cerr << "Graphics not supported (SDL2 not available)" << std::endl;
        return 1;
    }

    int clear_screen() {
        std::cerr << "Graphics not supported (SDL2 not available)" << std::endl;
        return 1;
    }

    int update_screen() {
        std::cerr << "Graphics not supported (SDL2 not available)" << std::endl;
        return 1;
    }

    int draw_rect(int x, int y, int w, int h) {
        std::cerr << "Graphics not supported (SDL2 not available)" << std::endl;
        return 1;
    }

#endif
#else
    #error "__has_include not supported by your compiler. Please install SDL2 to use graphics features."
#endif

#endif // GRAPHICS_HPP