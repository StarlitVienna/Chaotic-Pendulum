#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>
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
    trail = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height );
    SDL_SetTextureBlendMode( trail, SDL_BLENDMODE_BLEND );

    //SDL_SetRenderDrawBlendMode( renderer, SDL_BLENDMODE_ADD );

    if ( trail == NULL ) {
        std::cerr << "Could not create trail texture --> " << SDL_GetError() << std::endl;
    }

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

void Pendulum::set_length1( float increase ) {
    length1 += increase;
    //length2 += increase;
}
void Pendulum::set_length2( float increase ) {
    length2 += increase;
}

void Pendulum::draw_ellipse( int center_x, int center_y, int radius_x, int radius_y ) {
    //...
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

float Pendulum::color_map( float value, float max_value ) {
    return (value * 255 / max_value);
}

void Pendulum::draw_pendulum() {
    px2 = x2;
    py2 = y2;




 
    set_cords();


    /*
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
    SDL_RenderDrawLine( renderer, px2, py2, x2, y2 );
    */

    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderDrawLine( renderer, (start_x), (start_y), (x1+start_x), (y1) );
    SDL_RenderDrawLine( renderer, (x1+start_x), (y1), (x2+start_x), (y2) );

    float color = color_map(angle2, 500);
    SDL_SetRenderTarget( renderer, trail );
    SDL_SetRenderDrawColor( renderer, std::cos(angle2)*256, std::cos(angle2+12)*256, std::cos(angle2-12)*256, 255 );
    SDL_RenderDrawLine( renderer, px2+window_width/2, py2, x2+window_width/2, y2 );
    SDL_SetRenderTarget( renderer, NULL );

    circle_rect1.x = x1 + window_width/2 - (float)circle_rect1.w/2;
    circle_rect1.y = y1 - (float)circle_rect1.h/2;
    circle_rect2.x = x2 + window_width/2 - (float)circle_rect2.w/2;
    circle_rect2.y = y2 - (float)circle_rect2.h/2;

    SDL_RenderCopy( renderer, trail, NULL, NULL );

    SDL_RenderCopy( renderer, circle1, NULL, &circle_rect1 );
    SDL_RenderCopy( renderer, circle2, NULL, &circle_rect2 );
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
// be able to unzoom
//
// be able to pause the pendulum
//
// for the aftermath, save it in a list of points ( size decided in the constructor ) ( use drawline )
//
// enable air atrition
