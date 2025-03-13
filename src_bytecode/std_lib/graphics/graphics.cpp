#include "graphics.hpp"

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
            std::map<std::string, Value> eventMap;
            switch (event.type) {  
            case SDL_QUIT:
                eventMap["type"] = Value(Value_Type::STRING, "quit");
                events.push_back(Value(Value_Type::MAP, eventMap));
                break;
            case SDL_KEYDOWN:
                eventMap["type"] = Value(Value_Type::STRING, "keydown");
                eventMap["key"] = Value(Value_Type::STRING, SDL_GetKeyName(event.key.keysym.sym));
                events.push_back(Value(Value_Type::MAP, eventMap));
                break;
            case SDL_KEYUP:
                eventMap["type"] = Value(Value_Type::STRING, "keyup");
                eventMap["key"] = Value(Value_Type::STRING, SDL_GetKeyName(event.key.keysym.sym));
                events.push_back(Value(Value_Type::MAP, eventMap));
                break;
            case SDL_MOUSEBUTTONDOWN:
                eventMap["type"] = Value(Value_Type::STRING, "mousedown");
                eventMap["button"] = Value(Value_Type::NUMBER, static_cast<double>(event.button.button));
                eventMap["x"] = Value(Value_Type::NUMBER, static_cast<double>(event.button.x));
                eventMap["y"] = Value(Value_Type::NUMBER, static_cast<double>(event.button.y));
                events.push_back(Value(Value_Type::MAP, eventMap));
                break;
            case SDL_MOUSEBUTTONUP:
                eventMap["type"] = Value(Value_Type::STRING, "mouseup");
                eventMap["button"] = Value(Value_Type::NUMBER, static_cast<double>(event.button.button));
                eventMap["x"] = Value(Value_Type::NUMBER, static_cast<double>(event.button.x));
                eventMap["y"] = Value(Value_Type::NUMBER, static_cast<double>(event.button.y));
                events.push_back(Value(Value_Type::MAP, eventMap));
                break;
            case SDL_MOUSEMOTION:
                eventMap["type"] = Value(Value_Type::STRING, "mousemove");
                eventMap["x"] = Value(Value_Type::NUMBER, static_cast<double>(event.motion.x));
                eventMap["y"] = Value(Value_Type::NUMBER, static_cast<double>(event.motion.y));
                events.push_back(Value(Value_Type::MAP, eventMap));
                break;
            default:
                eventMap["type"] = Value(Value_Type::STRING, "unknown");
                events.push_back(Value(Value_Type::MAP, eventMap));
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

    int change_color(int r, int g, int b) {
        SDL_SetRenderDrawColor(ren, r, g, b, 255);

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

        SDL_RenderFillRect(ren, &rect);

        return 0;
    }

    int draw_line(int x1, int y1, int x2, int y2) {
        SDL_RenderDrawLine(ren, x1, y1, x2, y2);

        return 0;
    }

    int draw_circle(int x, int y, int r) {
        for (int w = 0; w < r * 2; w++) {
            for (int h = 0; h < r * 2; h++) {
                int dx = r - w; // horizontal offset
                int dy = r - h; // vertical offset
                if ((dx*dx + dy*dy) <= (r * r)) {
                    SDL_RenderDrawPoint(ren, x + dx, y + dy);
                }
            }
        }

        return 0;
    }

    int draw_lines(std::vector<Value> points) {
        for (int i = 0; i < (int)points.size() - 1; i++) {
            draw_line(VALUE_AS_NUMBER(VALUE_AS_VECTOR(points[i])[0]), 
                      VALUE_AS_NUMBER(VALUE_AS_VECTOR(points[i])[1]), 
                      VALUE_AS_NUMBER(VALUE_AS_VECTOR(points[i+1])[0]), 
                      VALUE_AS_NUMBER(VALUE_AS_VECTOR(points[i+1])[1]));
        }
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
