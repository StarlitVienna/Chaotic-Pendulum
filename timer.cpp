#include <SDL2/SDL.h>
#include <cstdint>
#include "timer.hpp"

void Timer::start_timer() {
    running = true;
    start_tick = SDL_GetTicks();
    paused_tick = 0;
}

void Timer::stop_timer() {
    running = false;
    start_tick = 0;
    paused_tick = 0;
}

void Timer::pause_timer() {
    if ( running && !paused ) {
        paused_tick = SDL_GetTicks() - start_tick;
        start_tick = 0;
    }
}

void Timer::unpause_timer() {
    if ( running && paused ) {
        paused = false;
        start_tick = SDL_GetTicks() - paused_tick;
        paused_tick = 0;
    }
}

uint32_t Timer::ticks() {
    if ( paused ) {
        return static_cast<uint32_t>(paused_tick);
    } else {
        return static_cast<uint32_t>(SDL_GetTicks() - start_tick);
    }
}
