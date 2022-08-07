#include <SDL2/SDL.h>
class Pendulum {
    public:
        Pendulum( float base_length1, float base_length2, float base_mass1, float base_mass2, float base_angle1, float base_angle2, float base_vel1, float base_vel2, float base_acceleration1, float base_acceleration2, SDL_Renderer*& render, float width, float height );
        void draw_ellipse( int center_x, int center_y, int radius_x, int radius_y );
        void set_starting_cords( float x, float y );
        void draw_pendulum();
        void set_gravity( float g );
        void set_length1( float increase );
        void set_length2( float increase );
        float color_map( float value, float max_value );

    private:
        void set_cords();

        float length1 = 0;
        float length2 = 0;

        float mass1 = 0;
        float mass2 = 0;

        float angle1 = 0;
        float angle2 = 0;

        float start_x = 0;
        float start_y = 0;

        float x1 = 0;
        float x2 = 0;
        float px2 = 0;
        
        float y1 = 0;
        float y2 = 0;
        float py2 = 0;

        float vel1 = 0;
        float vel2 = 0;

        float acceleration1 = 0;
        float acceleration2 = 0;

        float gravity = 1;
        int steps = 100;

        float window_width = 0;
        float window_height = 0;

        SDL_Texture* trail = NULL;
        SDL_Texture* circle1 = NULL;
        SDL_Texture* circle2 = NULL;
        SDL_Rect circle_rect1;
        SDL_Rect circle_rect2;
        SDL_Renderer* renderer = NULL;
};
