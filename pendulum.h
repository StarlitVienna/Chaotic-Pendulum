#include <SDL2/SDL.h>
class Pendulum {
    public:
        Pendulum() = default;
        Pendulum( float base_length1, float base_length2, float base_mass1, float base_mass2, float base_angle1, float base_angle2, float base_vel1, float base_vel2, float base_acceleration1, float base_acceleration2, SDL_Renderer*& render, float width, float height );
        void draw_ellipse( int center_x, int center_y, int radius_x, int radius_y );
        void set_starting_cords( float x, float y );
        void draw_pendulum();
        void set_gravity( float g );
        void set_length1( float increase );
        void set_length2( float increase );
        float color_map( float value, float max_value );
        void liner( float initial_x, float initial_y, float end_x, float end_y, bool trail );
        void set_equivalent( bool equivalent );
        void set_renderer( SDL_Renderer*& render );
        void gen_balls();
        void gen_trail( SDL_Texture*& trail_texture );
        void ball_switch( bool ball_state );
        void set_color( int r, int g, int b );
        void trail_switch();
        bool initialized = false;

        float angle1 = 180;
        float angle2 = 170;

        int arm_r = 255;
        int arm_g = 255;
        int arm_b = 255;

        int trail_r = 103;
        int trail_g = 25;
        int trail_b = 230;
        int num = 0;
        bool trail_shown = false;

        float get_x1();
        float get_x2();
        
        float get_y1();
        float get_y2();

        void clear_trail();

    private:
        void set_cords();
        void plot( float x, float y, float c, int r, int g, int b );

        
        float length1 = 90;
        float length2 = 160;

        float mass1 = 40;
        float mass2 = 10;

        float start_x = 250;
        float start_y = 250;

        float x1 = 0;
        float x2 = 0;
        float px2 = 0;
        
        float y1 = 0;
        float y2 = 0;
        float py2 = 0;

        float vel1 = 0.015;
        float vel2 = 0.015;

        float acceleration1 = 0;
        float acceleration2 = 0;

        float gravity = 2/9.8;
        int steps = 100;

        bool ball_shown = true;

        float window_width = 500;
        float window_height = 500;

        SDL_Texture* trail = NULL;
        SDL_Texture* circle1 = NULL;
        SDL_Texture* circle2 = NULL;
        SDL_Rect circle_rect1;
        SDL_Rect circle_rect2;
        SDL_Renderer* renderer = NULL;
};
