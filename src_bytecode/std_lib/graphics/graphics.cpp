#include "graphics.hpp"

#ifdef __has_include
#if __has_include(<SDL2/SDL.h>)
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_ttf.h>

    SDL_Window *win = nullptr;
    SDL_Renderer *ren = nullptr;
    std::vector<SDL_Event> eventQueue;
    std::mutex eventMutex;
    std::atomic<bool> running(true);
    std::thread eventThread;

    TTF_Font* font = nullptr;

    int width = 800;
    int height = 600;

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


    TTF_Font* load_embedded_font(int size) {
        SDL_RWops* rw = SDL_RWFromMem((void*)embedded_font, embedded_font_size);
        if (!rw) {
            std::cerr << "SDL_RWFromMem Error: " << SDL_GetError() << std::endl;
            return nullptr;
        }
        
        TTF_Font* font = TTF_OpenFontRW(rw, 1, size); // 1 means SDL_RWops will be auto-freed
        if (!font) {
            std::cerr << "TTF_OpenFontRW Error: " << TTF_GetError() << std::endl;
            return nullptr;
        }
        
        return font;
    }

    /*
    Create a window and renderer for graphics output.
    Needs to be called before any other graphics functions.
    Sets up the event thread to handle events.
    */
    int init_graphics(std::string title, int width, int height) {
        // Set the width and height for the global variables
        ::width = width;
        ::height = height;

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

        if (TTF_Init() == -1) {
            std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
            SDL_DestroyRenderer(ren);
            SDL_DestroyWindow(win);
            SDL_Quit();
            return 1;
        }
        font = load_embedded_font(16);
        if (!font) {
            SDL_DestroyRenderer(ren);
            SDL_DestroyWindow(win);
            TTF_Quit();
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

        
    int draw_text(std::string text, int x, int y, int size) {
        // Set font size
        TTF_SetFontSize(font, size);
        
        // Create surface from text
        SDL_Color color = {255, 255, 255, 255}; // Use current draw color
        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
        if (!surface) {
            return 1;
        }
    
        // Create texture from surface
        SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surface);
        SDL_FreeSurface(surface);
        if (!texture) {
            return 1;
        }
    
        // Render texture
        SDL_Rect dest = {x, y, 0, 0};
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
        SDL_RenderCopy(ren, texture, NULL, &dest);
        SDL_DestroyTexture(texture);
    
        return 0;
    }

    int draw_rotated_text(std::string text, int x, int y, int size, double angle) {
        TTF_SetFontSize(font, size);
        
        SDL_Color color = {255, 255, 255, 255};
        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
        if (!surface) return 1;
        
        SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surface);
        SDL_FreeSurface(surface);
        if (!texture) return 1;
        
        SDL_Rect dest = {x, y, 0, 0};
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
        
        // Set rotation center to middle of text
        SDL_Point center = {dest.w / 2, dest.h / 2};
        dest.x -= center.x;
        dest.y -= center.y;
        
        SDL_RenderCopyEx(ren, texture, NULL, &dest, angle, &center, SDL_FLIP_NONE);
        SDL_DestroyTexture(texture);
        
        return 0;
    }

    /*
    Creates grid lines and labels for a graph.
    Then loops through the lines and draws them on the graph.
    a line is two vectors of doubles where one is the x values and the other is the y values
    */
   int graph(std::vector<Value> x_vals, std::vector<Value> y_vals, double min_x, double max_x, double min_y, double max_y,
    std::string title, std::string x_label, std::string y_label) {

        // Calculate layout based on window dimensions
        const float MARGIN_RATIO = 0.15f;
        const int MARGIN = std::min(width, height) * MARGIN_RATIO;
        const int GRAPH_WIDTH = width - 2 * MARGIN;
        const int GRAPH_HEIGHT = height - 2 * MARGIN;
        const int GRID_SPACING = std::min(GRAPH_WIDTH, GRAPH_HEIGHT) / 10;
        const int LABEL_FONT_SIZE = std::min(width, height) * 0.02;
        const int TITLE_FONT_SIZE = LABEL_FONT_SIZE * 1.5;
        const int POINT_RADIUS = std::min(width, height) * 0.005;

        // Clear and set up background
        clear_screen();

        // Draw border and background
        change_color(64, 64, 64);
        draw_rect(MARGIN-2, MARGIN-2, GRAPH_WIDTH+4, GRAPH_HEIGHT+4);
        change_color(0, 0, 0);
        draw_rect(MARGIN, MARGIN, GRAPH_WIDTH, GRAPH_HEIGHT);

        // Draw grid lines and labels
        const int num_x_lines = GRAPH_WIDTH / GRID_SPACING;
        const int num_y_lines = GRAPH_HEIGHT / GRID_SPACING;

        // X grid lines and labels
        for (int i = 0; i <= num_x_lines; i++) {
            int x = MARGIN + i * GRID_SPACING;
            change_color(32, 32, 32);
            draw_line(x, MARGIN, x, height - MARGIN);
            
            double value = min_x + (i * (max_x - min_x) / num_x_lines);
            std::string label = std::to_string(int(value));
            change_color(200, 200, 200);
            draw_text(label, x - LABEL_FONT_SIZE/2, height - MARGIN + LABEL_FONT_SIZE/2, LABEL_FONT_SIZE);
        }

        // Y grid lines and labels
        for (int i = 0; i <= num_y_lines; i++) {
            int y = MARGIN + i * GRID_SPACING;
            change_color(32, 32, 32);
            draw_line(MARGIN, y, width - MARGIN, y);
            
            double value = max_y - (i * (max_y - min_y) / num_y_lines);
            std::string label = std::to_string(int(value));
            change_color(200, 200, 200);
            draw_text(label, MARGIN - LABEL_FONT_SIZE*3, y - LABEL_FONT_SIZE/2, LABEL_FONT_SIZE);
        }

        // Draw title and labels
        change_color(255, 255, 255);
        int title_x = width/2 - title.length()*TITLE_FONT_SIZE/4;
        int title_y = MARGIN/3;
        draw_text(title, title_x, title_y, TITLE_FONT_SIZE);

        int x_label_x = width/2 - x_label.length()*LABEL_FONT_SIZE/4;
        int x_label_y = height - MARGIN/3;
        draw_text(x_label, x_label_x, x_label_y, LABEL_FONT_SIZE);

        // Draw rotated y label
        int y_label_x = MARGIN/3;
        int y_label_y = height/2 + y_label.length()*LABEL_FONT_SIZE/4;
        draw_rotated_text(y_label, y_label_x, y_label_y, LABEL_FONT_SIZE, 270);

        // Handle multiple data series
        std::vector<std::vector<Value>> x_series;
        std::vector<std::vector<Value>> y_series;

        // Check if we have multiple series or single series
        if (x_vals.empty() || y_vals.empty()) return 1;

        if (x_vals[0].type == Value_Type::VECTOR) {
            // Multiple series
            for (const auto& x : x_vals) {
                x_series.push_back(VALUE_AS_VECTOR(x));
            }
            for (const auto& y : y_vals) {
                y_series.push_back(VALUE_AS_VECTOR(y));
            }
        } else {
            // Single series
            x_series.push_back(x_vals);
            y_series.push_back(y_vals);
        }

        // Draw data series
        const std::vector<SDL_Color> colors = {
            {255, 255, 255, 255},  // White
            {255, 100, 100, 255},  // Red
            {100, 255, 100, 255},  // Green
            {100, 100, 255, 255},  // Blue
            {255, 255, 100, 255}   // Yellow
        };

        for (size_t series = 0; series < x_series.size(); series++) {
            const auto& x_values = x_series[series];
            const auto& y_values = y_series[series];
            
            // Set color for this series
            SDL_Color color = colors[series % colors.size()];
            change_color(color.r, color.g, color.b);

            // Draw lines and points
            for (size_t i = 0; i < x_values.size() - 1; i++) {
                int x1 = MARGIN + (VALUE_AS_NUMBER(x_values[i]) - min_x) * GRAPH_WIDTH / (max_x - min_x);
                int y1 = height - MARGIN - (VALUE_AS_NUMBER(y_values[i]) - min_y) * GRAPH_HEIGHT / (max_y - min_y);
                int x2 = MARGIN + (VALUE_AS_NUMBER(x_values[i + 1]) - min_x) * GRAPH_WIDTH / (max_x - min_x);
                int y2 = height - MARGIN - (VALUE_AS_NUMBER(y_values[i + 1]) - min_y) * GRAPH_HEIGHT / (max_y - min_y);
                
                draw_line(x1, y1, x2, y2);
                draw_circle(x1, y1, POINT_RADIUS);
            }

            // Draw final point
            if (!x_values.empty()) {
                int last_x = MARGIN + (VALUE_AS_NUMBER(x_values.back()) - min_x) * GRAPH_WIDTH / (max_x - min_x);
                int last_y = height - MARGIN - (VALUE_AS_NUMBER(y_values.back()) - min_y) * GRAPH_HEIGHT / (max_y - min_y);
                draw_circle(last_x, last_y, POINT_RADIUS);
            }
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
