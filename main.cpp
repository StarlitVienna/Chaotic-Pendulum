#include <cstdint>
#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>
#include "pendulum.h"
#include "timer.hpp"

bool init();
float calc_gravity(float g);
void close();

const int WINDOW_HEIGHT = 500;
const int WINDOW_WIDTH = 500;
const int FPS = 60;
const int TICK_PER_FRAME = 1000/FPS;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int main( int argc, char* args[] ) {
    if ( !init() ) {
        std::cerr << "Failed to initialize" << std::endl;
    } else {
        bool quit = false;
        SDL_Event event;
        SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
        SDL_RenderClear( renderer );
        Pendulum chaotic( 100, 100, 30, 20, 180, 160, 0.001, -0.001, 0.00000, -0.00000,  renderer, WINDOW_WIDTH, WINDOW_HEIGHT );
        chaotic.set_starting_cords(500.0/2, 500.0/2);
        chaotic.set_gravity(2/9.8);
        Timer fps_handler;

        float gravity = 1;
        float length_increase1 = 2;
        float length_increase2 = 2;
        while ( !quit ) {
            fps_handler.start_timer();
            uint32_t current_tick = fps_handler.ticks();
            if ( current_tick < TICK_PER_FRAME ) {
                SDL_Delay( TICK_PER_FRAME - current_tick );
            }
            while ( SDL_PollEvent( &event ) != 0 ) {
                if ( event.type == SDL_QUIT ) {
                    quit = true;
                } else if ( event.type == SDL_KEYDOWN ) {
                    switch ( event.key.keysym.sym ) {

                        case SDLK_g:
                            ++gravity;
                            chaotic.set_gravity( calc_gravity( gravity ) );
                            std::cout << gravity << std::endl;
                            break;

                        case SDLK_v:
                            --gravity;
                            chaotic.set_gravity( calc_gravity( gravity ) );
                            break;

                        case SDLK_a:
                            chaotic.set_length1( -length_increase1 );
                            break;

                        case SDLK_d:
                            chaotic.set_length1( length_increase1 );
                            break;

                        case SDLK_q:
                            chaotic.set_length2( -length_increase2 );
                            break;

                        case SDLK_e:
                            chaotic.set_length2( length_increase2 );
                            break;

                    }
                }
            }
            SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
            SDL_RenderClear( renderer );
            chaotic.draw_pendulum();
            SDL_RenderPresent( renderer );
        }
    }
    return 0;
}

bool init() {
    bool success = true;
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        std::cerr << "SDL was not initialized --> " << SDL_GetError() << std::endl;
        success = false;
    } else {
        window = SDL_CreateWindow( "Starlust", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN );
        if ( window == NULL ) {
            std::cerr << "Window was not created --> " << SDL_GetError() << std::endl;
            success = false;
        } else {
            renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
            if ( renderer == NULL ) {
                std::cerr << "Could not create renderer --> " << SDL_GetError() << std::endl;
                success = false;
            }
        }

    }
    return success;
}

float calc_gravity(float g) {
    return g/9.8;
}

void close() {
    SDL_DestroyWindow( window );
    SDL_DestroyRenderer( renderer );
    window = NULL;
    renderer = NULL;
    SDL_Quit();
}

