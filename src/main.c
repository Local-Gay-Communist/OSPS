#include <stdio.h>
#define  SDL_MAIN_HANDLED
#include "SDL2/include/SDL2/SDL.h"
#include "SDL2/include/SDL2/SDL_timer.h"
#include "SDL2/include/SDL2/SDL_image.h"

// window dimensions
#define WINDOW_WIDTH (1280)
#define WINDOW_HEIGHT (720)

// speed in pixels/second
#define SCROLL_SPEED (300)

int main(int argc, char const *argv[]) {
  // initialisation
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("SDL ERROR: %s\n", SDL_GetError());
    return 1;
  }
  printf("SDL initialised successfully\n");

  // create window
  SDL_Window* win = SDL_CreateWindow("Hello, World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);

  // error handling on the window creation
  if (!win) {
    printf("error creating window: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  // create renderer
  Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
  SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

  // renderer error handeling
  if (!rend) {
    printf("error creating renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 1;
  }

  // load image into memory
  SDL_Surface* surface = IMG_Load("resources/pride.png");

  // load image error handling
  if (!surface) {
    printf("error loading image: %s\n", SDL_GetError());
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 1;
  }

  // create texture
  SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
  SDL_FreeSurface(surface);

  // create texture error handling
  if (!tex) {
    printf("error creating texture: %s\n", SDL_GetError());
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 1;
  }

  // struct for size and position of sprite
  SDL_Rect dest;

  // get dimensions of texture
  SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);

  /* position the sprite at the bottom center of the window
     origin is the top left corner, positive y is down*/
  dest.x = (WINDOW_WIDTH - dest.w) / 2;

  // require float resolution for y position
  float y_pos = WINDOW_HEIGHT;

  // animation loop
  while (dest.y >= dest.h) {
    // clear the window
    SDL_RenderClear(rend);

    // set the y position in the struct
    dest.y = (int) y_pos;

    // draw the image to the window
    SDL_RenderCopy(rend, tex, NULL, &dest);
    SDL_RenderPresent(rend);

    // update sprite position
    y_pos -= (float) SCROLL_SPEED / 60;

    // wait 1/60th of a second
    SDL_Delay(1000/60);
  }

  // clear the window
  SDL_RenderClear(rend);

  // draw the image to the window
  SDL_RenderCopy(rend, tex, NULL, NULL);
  SDL_RenderPresent(rend);

  // TODO: make loop
  SDL_Delay(5000);

  // clean-up
  SDL_DestroyTexture(tex);
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
