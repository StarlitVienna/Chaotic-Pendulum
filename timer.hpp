#include <cstdint>
#include <iostream>
class Timer {
    public:
        Timer() = default;
        void start_timer();
        void stop_timer();
        void pause_timer();
        void unpause_timer();
        uint32_t ticks();
        bool state();

    private:
        uint32_t time = 0;
        uint32_t start_tick = 0;
        uint32_t paused_tick = 0;
        int trail_counter = 0;
        int max_trail = 100;
        bool running = false;
        bool paused = false;
};
