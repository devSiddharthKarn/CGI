// =============================================================
//  CGI - C++ Graphics Ingine
//  Simple. Effective. Elegant.
//  Copyright (c) 2025 Siddharth Karn
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file in the project root for full license information.
// =============================================================


#include <vector>
#include <cstdlib>  // for rand()
#include <ctime>    // for srand()
#include "cgi_system_utils.hpp"
#include "cgi_window.hpp"

// Assume cgi::window is your class from the provided library
using namespace cgi;

struct Rect {
    int x, y, w, h;
};

struct Bird {
    Rect rect;
    double velocity = 0;
};

struct Pipe {
    int x;
    int gap_y;
    int gap_size;
    int width;
};

window win("Flappy Rectangle", 100, 100, 400, 600, cgi::color::rgb(0, 0, 0));

Bird bird;
std::vector<Pipe> pipes;

const double gravity = 0.5;
const double jump_strength = -10;
const int pipe_speed = 4;
const int gap_size = 150;
const int pipe_width = 70;
bool game_over = false;

void draw_rect(int x, int y, int w, int h, type::color_t color) {
    for (int iy = y; iy < y + h; ++iy) {
        for (int ix = x; ix < x + w; ++ix) {
            win.set_pixel(ix, iy, color);
        }
    }
}

void reset_game() {
    bird.rect = {100, win.get_buffer_height() / 2, 40, 30};
    bird.velocity = 0;
    pipes.clear();
    game_over = false;
}

void update() {
    if (game_over) {
        // Flash screen red when game over
        win.clear(cgi::color::rgb(255, 0, 0));
        return;
    }

    // Update bird physics
    bird.velocity += gravity;
    bird.rect.y += (int)bird.velocity;

    // Add pipes every 90 frames (~1.5 sec at 60 FPS)
    static int frame_count = 0;
    if (frame_count % 90 == 0) {
        Pipe p;
        p.x = win.get_buffer_width();
        p.gap_y = 100 + (std::rand() % (win.get_buffer_height() - 200 - gap_size));
        p.gap_size = gap_size;
        p.width = pipe_width;
        pipes.push_back(p);
    }
    ++frame_count;

    // Update pipes
    for (auto &pipe : pipes) {
        pipe.x -= pipe_speed;
    }

    // Remove off-screen pipes
    while (!pipes.empty() && pipes.front().x + pipe_width < 0) {
        pipes.erase(pipes.begin());
    }

    // Collision detection
    // if (bird.rect.y < 0 || bird.rect.y + bird.rect.h > win.get_buffer_height()) {
    //     game_over = true;
    // }

    for (const auto& pipe : pipes) {
        // Bird rectangle
        Rect b = bird.rect;

        // Pipe top rect
        Rect top = {pipe.x, 0, pipe.width, pipe.gap_y};
        // Pipe bottom rect
        Rect bottom = {pipe.x, pipe.gap_y + pipe.gap_size, pipe.width, win.get_buffer_height() - (pipe.gap_y + pipe.gap_size)};

        bool collide_top = !(b.x + b.w < top.x || b.x > top.x + top.w || b.y + b.h < top.y || b.y > top.y + top.h);
        bool collide_bottom = !(b.x + b.w < bottom.x || b.x > bottom.x + bottom.w || b.y + b.h < bottom.y || b.y > bottom.y + bottom.h);

        // if (collide_top || collide_bottom) {
        //     game_over = true;
        // }
    }

    // Clear screen
    win.clear(cgi::color::rgb(0, 0, 0));

    // Draw bird (blue)
    draw_rect(bird.rect.x, bird.rect.y, bird.rect.w, bird.rect.h, cgi::color::rgb(50, 100, 255));

    // Draw pipes (green)
    for (const auto& pipe : pipes) {
        draw_rect(pipe.x, 0, pipe.width, pipe.gap_y, cgi::color::rgb(50, 255, 50));
        draw_rect(pipe.x, pipe.gap_y + pipe.gap_size, pipe.width, win.get_buffer_height() - (pipe.gap_y + pipe.gap_size), cgi::color::rgb(50, 255, 50));
    }

    std::cout<<win.fps()<<'\n';
}

void handle_input() {
    // Check for space key press to jump
    if (cgi::system::keyboard::is_ASCII_key_pressed(' ') && !game_over) {
        bird.velocity = jump_strength;
    }

    // Restart game on 'R' if game over
    if (game_over && cgi::system::keyboard::is_ASCII_key_pressed('R')) {
        reset_game();
    }
}

int main() {
    std::srand(std::time(nullptr));

    win.create(true);
    win.show();
    reset_game();

    win.run_as([]() {
        handle_input();
        update();
    },60);

    return 0;
}
