#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>
#include <type_traits>
#include "pendulum.h"

// length --> meters
// mass --> kilograms
//

Pendulum::Pendulum( float base_length1, float base_length2, float base_mass1, float base_mass2, float base_angle1, float base_angle2, float base_vel1, float base_vel2, float base_acceleration1, float base_acceleration2, SDL_Renderer*& render, float width, float height ) {

    length1 = base_length1;
    length2 = base_length2;

    mass1 = base_mass1;
    mass2 = base_mass2;

    angle1 = base_angle1;
    angle2 = base_angle2;

    vel1 = base_vel1;
    vel2 = base_vel2;

    acceleration1 = base_acceleration1;
    acceleration2 = base_acceleration2;

    window_width = width;
    window_height = height;

    renderer = render;
    //SDL_Surface* trail_surface = SDL_LoadBMP("./src/blank.bmp");
    //SDL_SetColorKey( trail_surface, SDL_TRUE, SDL_MapRGB( trail_surface->format, 255, 0xFF, 0xFF ) );
    //trail = SDL_CreateTextureFromSurface( renderer, trail_surface );

    gen_balls();

}

float Pendulum::get_x1() {
    return x1;
}
float Pendulum::get_x2() {
    return x2;
}

float Pendulum::get_y1() {
    return y1;
}
float Pendulum::get_y2() {
    return y2;
}

void Pendulum::ball_switch( bool ball_state ) {
    if ( !ball_state ) {
        ball_shown = false;
    } else {
        ball_shown = true;
    }
}

void Pendulum::gen_trail(SDL_Texture*& trail_texture) {
    //trail = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, window_width, window_height );
    trail = trail_texture;
    SDL_SetTextureBlendMode( trail, SDL_BLENDMODE_BLEND );

    if ( trail == NULL ) {
        std::cerr << "Could not create trail texture --> " << SDL_GetError() << std::endl;
    }
}

void Pendulum::gen_balls() {
    SDL_Surface* circle1_surface = NULL;
    SDL_Surface* circle2_surface = NULL;

    circle1_surface = SDL_LoadBMP("./src/wcircle.bmp");
    circle2_surface = SDL_LoadBMP("./src/wcircle.bmp");

    float size_ball1 = mass1;
    float size_ball2 = mass2;

    if ( circle1_surface == NULL || circle2_surface == NULL ) {
        std::cerr << "Surface was not created --> " << SDL_GetError() << std::endl;
    } else {

        SDL_SetColorKey( circle1_surface, SDL_TRUE, SDL_MapRGB( circle1_surface->format, 255, 0xFF, 0xFF ) );
        SDL_SetColorKey( circle2_surface, SDL_TRUE, SDL_MapRGB( circle2_surface->format, 0, 0xFF, 0xFF ) );

        circle1 = SDL_CreateTextureFromSurface( renderer, circle1_surface );
        circle2 = SDL_CreateTextureFromSurface( renderer, circle2_surface );

        SDL_FreeSurface( circle1_surface );
        SDL_FreeSurface( circle2_surface );
    }


    if ( circle1 == NULL ) {
        std::cerr << "Circle texture1 was not created --> " << SDL_GetError() << std::endl;
    } else {
        circle_rect1.x = 0;
        circle_rect1.y = 0;
        circle_rect1.w = size_ball1;
        circle_rect1.h = size_ball1;

        SDL_RenderCopy( renderer, circle1, NULL, &circle_rect1 );
    }

    if ( circle2 == NULL ) {
        std::cerr << "Circle texture2 was not created --> " << SDL_GetError() << std::endl;
    } else {
        circle_rect2.x = 0;
        circle_rect2.y = 0;
        circle_rect2.w = size_ball2;
        circle_rect2.h = size_ball2;

        SDL_RenderCopy( renderer, circle2, NULL, &circle_rect2 );
    }
    SDL_RenderSetScale( renderer, 1, 1 );
}
void Pendulum::set_renderer( SDL_Renderer*& render ) {
    renderer = render;
}

void Pendulum::set_length1( float increase ) {
    length1 += increase;
    //length2 += increase;
}
void Pendulum::set_length2( float increase ) {
    length2 += increase;
}

void Pendulum::set_equivalent( bool equivalent ) {
    if ( equivalent ) {
        circle_rect1.w = mass1;
        circle_rect1.h = mass1;
        circle_rect2.w = mass2;
        circle_rect2.h = mass2;
    } else {
        circle_rect1.w = mass1/4;
        circle_rect1.h = mass1/4;
        circle_rect2.w = mass1/4;
        circle_rect2.h = mass1/4;
    }
}

void Pendulum::draw_ellipse( int center_x, int center_y, int radius_x, int radius_y ) {
    //...
}

void Pendulum::clear_trail() {
    SDL_SetRenderTarget( renderer, trail );
    SDL_SetRenderDrawColor( renderer, 15, 15, 15, 255 );
    SDL_RenderClear( renderer );
    SDL_SetRenderTarget( renderer, NULL );
}

void Pendulum::trail_switch() {
    if ( trail_shown ) {
        trail_shown = false;
    } else {
        trail_shown = true;
    }
}

void Pendulum::set_gravity( float g ) {
    gravity = g;
}

void Pendulum::set_cords() {
    x1 = length1*std::sin(angle1);
    y1 = length1*std::cos(angle1)+start_y;

    x2 = x1 + length2*std::sin(angle2);
    y2 = y1 + length2*std::cos(angle2);
}

void Pendulum::set_starting_cords( float x, float y ) {
    start_x = x;
    start_y = y;
}

int ipart(float x) {
    return (int)x;
}
  
int rounder(float x) {
    return ipart(x + 0.5) ;
}
  
float fpart(float x) {
    if (x>0) {
        return x - ipart(x);
    } else {
        return x - (ipart(x)+1);
    }
}
  
float rfpart(float x)
{
    return 1 - fpart(x);
}
  
void Pendulum::plot( float x, float y, float c, int r, int g, int b ) {
    if ( r == 255 && b == 255 && g == 255 ) {
        c = 255+c;
        SDL_SetRenderDrawColor( renderer, c, c, c, 255 );
    } else {
        // gltich
        //SDL_SetRenderDrawColor( renderer, r/y*y*x, g/x*y*y, b/x*y*y, 255 );
        //SDL_SetRenderDrawColor( renderer, (float)arm_r*r*r-91, (float)arm_g-100, (float)arm_b, 255 );
        SDL_SetRenderDrawColor( renderer, 91+150, 206-(num/3), 250-(num/2)*3, 255 );
    } 
    SDL_RenderDrawPoint( renderer, x, y );
}

void Pendulum::liner( float x0, float y0, float x1, float y1, bool trail ) {
    bool steep = abs(y1 - y0) > abs(x1 - x0);
  
    if ( steep ) {
        std::swap(x0 , y0);
        std::swap(x1 , y1);
    }
    if ( x0 > x1 ) {
        std::swap(x0 ,x1);
        std::swap(y0 ,y1);
    }
  
    float dx = x1-x0;
    float dy = y1-y0;
    float gradient = dy/dx;

    if ( dx == 0.0 ) {
        gradient = 1;
    }
  
    int xpxl1 = x0;
    int xpxl2 = x1;
    float intersect_y = y0;
  
    if ( !trail ) {
        if ( steep ) {
            for (int x = xpxl1 ; x <=xpxl2 ; x++) {
                plot(ipart(intersect_y), x, rfpart(intersect_y), arm_r, arm_g, arm_b);
                plot(ipart(intersect_y)-1, x, fpart(intersect_y), arm_r, arm_g, arm_b);
                intersect_y += gradient;
            }
        } else {
            for ( int x = xpxl1 ; x <=xpxl2 ; x++ ) {
                plot(x, ipart(intersect_y), rfpart(intersect_y), arm_r, arm_g, arm_b);
                plot(x, ipart(intersect_y)-1, fpart(intersect_y), arm_r, arm_g, arm_b);
                intersect_y += gradient;
            }
        }
    } else {
        if ( steep ) {
            for (int x = xpxl1 ; x <=xpxl2 ; x++) {
                plot(ipart(intersect_y), x, rfpart(intersect_y), trail_r, trail_g, trail_b);
                plot(ipart(intersect_y)-1, x, fpart(intersect_y), trail_r, trail_g, trail_b);
                intersect_y += gradient;
            }
        } else {
            for ( int x = xpxl1 ; x <=xpxl2 ; x++ ) {
                plot(x, ipart(intersect_y), rfpart(intersect_y),trail_r, trail_g, trail_b);
                plot(x, ipart(intersect_y)-1, fpart(intersect_y), trail_r, trail_g, trail_b);
                intersect_y += gradient;
            }
        }
    }
}

float Pendulum::color_map( float value, float max_value ) {
    return (value * 255 / max_value);
}

void Pendulum::draw_pendulum() {
    if ( trail_shown ) {
        SDL_RenderCopy( renderer, trail, NULL, NULL );
    }
    px2 = x2;
    py2 = y2;

    set_cords();

    /*
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
    SDL_RenderDrawLine( renderer, px2, py2, x2, y2 );
    */

    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
    float dx = x1-start_x;
    float dy = y1-start_y;
    float d = 2*dy-dx;
    float y = start_y;

    /*
    SDL_RenderDrawLine( renderer, (start_x), (start_y), (x1+start_x), (y1) );
    SDL_RenderDrawLine( renderer, (x1+start_x), (y1), (x2+start_x), (y2) );
    */

    liner( start_x+x1, y1, start_x, start_y, false );
    liner( start_x+x1-1, y1, start_x-1, start_y, false );
    liner( start_x+x1+1, y1, start_x+1, start_y, false );
    
    liner( start_x+x1, y1, start_x+x2, y2, false );
    liner( start_x+x1-1, y1, start_x+x2-1, y2, false );
    liner( start_x+x1+1, y1, start_x+x2+1, y2, false );
   
    float color = color_map(angle2, 500);
    float shade = y2;
    SDL_SetRenderTarget( renderer, trail );
    //SDL_SetRenderDrawColor( renderer, std::cos(angle2)*256, std::cos(angle2+12)*256, std::cos(angle2-12)*256, 200);
    liner( px2+window_width/2, py2, x2+window_width/2, y2, true );
    liner( px2+window_width/2-1, py2+1, x2+window_width/2-1, y2+1, true );
    liner( px2+window_width/2+1, py2-1, x2+window_width/2+1, y2-1, true );
    //SDL_RenderDrawLine( renderer, px2+window_width/2, py2, x2+window_width/2, y2 );
    SDL_SetRenderTarget( renderer, NULL );

    if ( ball_shown ) {
        circle_rect1.x = x1 + window_width/2 - (float)circle_rect1.w/2;
        circle_rect1.y = y1 - (float)circle_rect1.h/2;
        circle_rect2.x = x2 + window_width/2 - (float)circle_rect2.w/2;
        circle_rect2.y = y2 - (float)circle_rect2.h/2;


        SDL_RenderCopy( renderer, circle1, NULL, &circle_rect1 );
        SDL_RenderCopy( renderer, circle2, NULL, &circle_rect2 );
    }

    float part1 = -gravity*(2*mass1+mass2)*std::sin(angle1);
    float part2 = -mass2*gravity*std::sin(angle1-angle2);
    float part3 = -2*std::sin(angle1-angle2)*mass2;
    float part4 = ((vel2*vel2)*length2+(vel1*vel1)*length1*std::cos(angle1-angle2));
    float part5 = length1*(2*mass1+mass2-mass2*std::cos(2*angle1-2*angle2));
    vel1 += (part1+part2+part3*part4)/part5;

    part1 = 2*std::sin(angle1-angle2);
    part2 = (vel1*vel1)*length1*(mass1+mass2);
    part3 = gravity*(mass1+mass2)*std::cos(angle1);
    part4 = (vel2*vel2)*length2*mass2*std::cos(angle1-angle2);
    part5 = length2*(2*mass1+mass2-mass2*std::cos(2*angle1-2*angle2));
    vel2 += part1*(part2+part3+part4)/part5;

    angle1 += vel1;
    angle2 += vel2;

}
