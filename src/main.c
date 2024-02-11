#include <raylib.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
  // initialise the window
  InitWindow(640, 480, "OSPS");

  // for debugging
  printf("Hello, World!\n");

  // create camera
  Camera3D cam = {0};
  cam.position = (Vector3){0.0f, 0.0f, 0.0f};
  cam.target = (Vector3){0.0f, 2.0f, 0.0f};
  cam.up = (Vector3){0.0f, 1.0f, 0.0f};
  cam.fovy = 60.0f;
  cam.projection = CAMERA_PERSPECTIVE;

  int cameraMode = CAMERA_FREE;

  // misc
  DisableCursor();
  SetTargetFPS(60);

  // main loop
  while (!WindowShouldClose()) {

    // update camera
    UpdateCamera(&cam, cameraMode);

    BeginDrawing();
      ClearBackground(RAYWHITE);
      BeginMode3D(cam);
      DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 32.0f, 32.0f }, LIGHTGRAY);
      DrawCube((Vector3){ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);
      DrawCube((Vector3){ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME);
      DrawCube((Vector3){ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD);
      EndMode3D();
      DrawRectangle(5, 5, 330, 100, Fade(SKYBLUE, 0.5f));
      DrawRectangleLines(5, 5, 330, 100, BLUE);
      DrawText("Camera controls:", 15, 15, 10, BLACK);
      DrawText("- Move keys: W, A, S, D, Space, Left-Ctrl", 15, 30, 10, BLACK);
      DrawText("- Look around: arrow keys or mouse", 15, 45, 10, BLACK);
      DrawText("- Camera mode keys: 1, 2, 3, 4", 15, 60, 10, BLACK);
      DrawText("- Zoom keys: num-plus, num-minus or mouse scroll", 15, 75, 10, BLACK);
      DrawText("- Camera projection key: P", 15, 90, 10, BLACK);

    EndDrawing();
  }

  // clean up
  CloseWindow();

  return 0;
}
