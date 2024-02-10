#include <stdio.h>
#include <math.h>
#define  SDL_MAIN_HANDLED
#include "SDL2/include/SDL2/SDL.h"
#include "SDL2/include/SDL2/SDL_timer.h"
#include "SDL2/include/SDL2/SDL_image.h"

// window dimensions
#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

// speed in pixels/second
#define SPEED (300)

int main(int argc, char const *argv[]) {
  // initialisation
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("SDL ERROR: %s\n", SDL_GetError());
    return 1;
  }
  printf("SDL initialised successfully\n");

  // create window
  SDL_Window* win = SDL_CreateWindow("Hello, World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

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
  float x_pos = (WINDOW_WIDTH - dest.w) / 2;
  float y_pos = (WINDOW_HEIGHT - dest.h) / 2;

  // give sprite velocity
  float x_vel = 0;
  float y_vel = 0;

  // keep track of keyboard inputs
  int up = 0;
  int down = 0;
  int left = 0;
  int right = 0;

  // set to 1 when (X) button is pressed
  int close_requested = 0;

  // animation loop
  while (!close_requested) {
    // debugging
    printf("while loop is happening\n");

    // process events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        close_requested = 1;
      }
    }

    // get cursor position relative to window
    int mouse_x, mouse_y;
    int buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

    // determine velocity toward mouse
    int target_x = mouse_x - dest.w / 2;
    int target_y = mouse_y - dest.h / 2;
    float delta_x = target_x - x_pos;
    float delta_y = target_y - y_pos;
    float distance = sqrt(delta_x * delta_x + delta_y * delta_y);

    // prevent jitter
    if (distance < 5) {
      x_vel = y_vel = 0;
    }
    else {
      x_vel = delta_x * SPEED / distance;
      y_vel = delta_y * SPEED / distance;
    }

    // reverse velocity
    if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) {
      x_vel = -x_vel;
      y_vel = -y_vel;
    }

    // update positions
    x_pos += x_vel / 60;
    y_pos += y_vel / 60;

    // collision detection with bounds
    if (x_pos <= 0) {
      x_pos = 0;
    }
    if (y_pos <= 0) {
      y_pos = 0;
    }
    if (x_pos >= WINDOW_WIDTH - dest.w) {
      x_pos = WINDOW_WIDTH - dest.w;
    }
    if (y_pos >= WINDOW_HEIGHT - dest.h) {
      y_pos = WINDOW_HEIGHT - dest.h;
    }

    // set the positions in the struct
    dest.y = (int) y_pos;
    dest.x = (int) x_pos;

    // clear the window
    SDL_RenderClear(rend);

    // draw the image to the window
    SDL_RenderCopy(rend, tex, NULL, &dest);
    SDL_RenderPresent(rend);

    // wait 1/60th of a second
    SDL_Delay(1000/60);
  }

  // clean-up
  SDL_DestroyTexture(tex);
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
