#include <algorithm>
#include <cstdint>
#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>
#include "pendulum.h"
#include "timer.hpp"

bool init();
float calc_gravity(float g);
void create_pendulum_balls();
void close();

const int WINDOW_HEIGHT = 500;
const int WINDOW_WIDTH = 500;
const int FPS = 60;
const int TICK_PER_FRAME = 1000/FPS;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* pendulum_trail = NULL;
int main( int argc, char* args[] ) {
    if ( !init() ) {
        std::cerr << "Failed to initialize" << std::endl;
    } else {
        bool quit = false;
        SDL_Event event;
        SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
        SDL_RenderClear( renderer );
        Pendulum chaotic( 120, 130, 40, 10, 180, 160, 0.01, -0.02, 0.2000, -0.05500,  renderer, WINDOW_WIDTH, WINDOW_HEIGHT );
        chaotic.set_starting_cords(500.0/2, 500.0/2);
        chaotic.set_gravity(2/9.8);
        chaotic.gen_trail( pendulum_trail );
        Timer fps_handler;

        const int max_pendulums = 100;
        Pendulum pendulums[max_pendulums];
        float gravity = 1;
        float length_increase1 = 2;
        float length_increase2 = 2;

        bool equivalent_sizes = false;
        bool ball_shown = true;

        chaotic.set_equivalent( equivalent_sizes );
        int current_pendulum = 0;
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
                            for ( int i = 0; i <= max_pendulums; ++i ) {
                                if ( pendulums[i].initialized ) {
                                    pendulums[i].set_gravity( calc_gravity( gravity ) );
                                }
                            }
                            std::cout << gravity << std::endl;
                            break;

                        case SDLK_v:
                            --gravity;
                            chaotic.set_gravity( calc_gravity( gravity ) );
                            for ( int i = 0; i <= max_pendulums; ++i ) {
                                if ( pendulums[i].initialized ) {
                                    pendulums[i].set_gravity( calc_gravity( gravity ) );
                                }
                            }
                            break;

                        case SDLK_a:
                            chaotic.set_length1( -length_increase1 );
                            break;

                        case SDLK_d:
                            chaotic.set_length1( length_increase1 );
                            break;

                        case SDLK_q:
                            chaotic.set_length2( -length_increase2 );
                            for ( int i = 0; i <= max_pendulums; ++i ) {
                                if ( pendulums[i].initialized ) {
                                    pendulums[i].set_length2( -length_increase2 );
                                }
                            }
                            break;

                        case SDLK_e:
                            chaotic.set_length2( length_increase2 );
                            for ( int i = 0; i <= max_pendulums; ++i ) {
                                if ( pendulums[i].initialized ) {
                                    pendulums[i].set_length2( length_increase2 );
                                }
                            }
                            break;

                        case SDLK_w:
                            if (equivalent_sizes) {
                                equivalent_sizes = false;
                            } else {
                                equivalent_sizes = true;
                            }
                            chaotic.set_equivalent( equivalent_sizes );
                            for ( int i = 0; i <= max_pendulums; ++i ) {
                                pendulums[i].set_equivalent( equivalent_sizes );
                            }
                            break;

                        case SDLK_b:
                            if ( ball_shown ) {
                                ball_shown = false;
                            } else {
                                ball_shown = true;
                            }
                            chaotic.ball_switch( ball_shown );
                            for ( int i = 0; i <= max_pendulums; ++i ) {
                                if ( pendulums[i].initialized ) {
                                    pendulums[i].ball_switch( ball_shown );
                                }
                            }
                            break;

                        case SDLK_p:
                            if ( current_pendulum <= max_pendulums ) {
                                ++current_pendulum;
                                pendulums[current_pendulum].initialized = true;
                                pendulums[current_pendulum].num = current_pendulum;
                                pendulums[current_pendulum].set_renderer( renderer );
                                pendulums[current_pendulum].angle1+=((float)current_pendulum/100*3.141+5*current_pendulum);
                                pendulums[current_pendulum].angle2-=((float)current_pendulum/100*3.141+5-10*current_pendulum);


                                pendulums[current_pendulum].arm_r = current_pendulum+106;
                                pendulums[current_pendulum].arm_g = current_pendulum/pendulums[current_pendulum].arm_r+13;
                                pendulums[current_pendulum].arm_b = current_pendulum/pendulums[current_pendulum].arm_g+173;

                                pendulums[current_pendulum].gen_balls();
                                pendulums[current_pendulum].gen_trail(pendulum_trail);
                                create_pendulum_balls();
                                pendulums[current_pendulum].set_equivalent( equivalent_sizes );
                                std::cout << current_pendulum << std::endl;
                            }
                            break;

                        case SDLK_t:
                            chaotic.trail_switch();
                            break;

                        case SDLK_c:
                            chaotic.clear_trail();
                            break;
                        }
                    }
                }

            SDL_SetRenderDrawColor( renderer, 15, 15, 15, 255 );
            SDL_RenderClear( renderer );
            chaotic.trail_r += (chaotic.get_y1()*chaotic.get_y2()*chaotic.get_x2()/chaotic.get_x1());
            chaotic.trail_g += (chaotic.get_y1()*chaotic.get_y2()/chaotic.get_x2()/chaotic.get_x1());
            chaotic.trail_b += (chaotic.get_y1()/chaotic.get_y2()*chaotic.get_x2()*chaotic.get_x1());
            chaotic.trail_r += 103;
            chaotic.trail_g += 25;
            chaotic.trail_b += 240;
            chaotic.draw_pendulum();
            for ( int i = 0; i < max_pendulums; ++i ) {
                if ( !pendulums[i].initialized ) {
                } else {
                    pendulums[i].draw_pendulum();
                    pendulums[i].set_starting_cords((float)WINDOW_WIDTH/2, (float)WINDOW_HEIGHT/2);
                }
            }
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
            } else {
                pendulum_trail = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_HEIGHT, WINDOW_HEIGHT );
                if ( pendulum_trail == NULL ) {
                    std::cerr << "Could not create pendulum trail texture --> " << SDL_GetError() << std::endl;
                }
            }
        }

    }
    return success;
}

void create_pendulum_balls() {

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

