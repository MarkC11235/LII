#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include "../../helpers/Value.hpp"
#include "embedded_font.hpp"

#ifdef __has_include
#if __has_include(<SDL2/SDL.h>)
    #include <SDL2/SDL.h>

    // External declarations for global variables
    extern SDL_Window* win;
    extern SDL_Renderer* ren;
    extern std::vector<SDL_Event> eventQueue;
    extern std::mutex eventMutex;
    extern std::atomic<bool> running;
    extern std::thread eventThread;

    // Function declarations
    int event_thread();
    int init_graphics(std::string title, int width, int height);
    int close_graphics();
    std::vector<Value> get_events();
    int clear_screen();
    int update_screen();
    int change_color(int r, int g, int b);
    int draw_rect(int x, int y, int w, int h);
    int draw_line(int x1, int y1, int x2, int y2);
    int draw_circle(int x, int y, int r);
    int draw_lines(std::vector<Value> points);
    int draw_text(std::string text, int x, int y, int size);
    int graph(std::vector<Value> lines, double min_x, double max_x, double min_y, double max_y,
        std::string title, std::string x_label, std::string y_label);
#else
    // SDL2 is not available, provide declarations for stub functions
    int init_graphics(std::string title, int width, int height);
    int close_graphics();
    int clear_screen();
    int update_screen();
    int draw_rect(int x, int y, int w, int h);
    int draw_line(int x1, int y1, int x2, int y2);
    int draw_circle(int x, int y, int r);
    int draw_lines(std::vector<Value> points);
    int draw_text(std::string text, int x, int y);
    int graph(std::vector<Value> lines, double min_x, double max_x, double min_y, double max_y, std::string title, std::string x_label, std::string y_label);

#endif
#else
    #error "__has_include not supported by your compiler. Please install SDL2 to use graphics features."
#endif

#endif // GRAPHICS_HPP