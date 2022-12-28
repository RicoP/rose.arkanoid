#include <raylib.h>
#include <rlgl.h>
#include <rose.h>
#include <rose/event.h>
#include <rose/hash.h>
#include <roseio.h>
#include <imgui.h>
#include <components/padevents.h>
#include <components/components.h>
#include <components/components_ser.h>
#include <serializer/imguiserializer.h>
#include <algorithm>

#include "world.h"
#define IMPL_SERIALIZER
#include "world.serializer.h"

World world;
Camera3D camera = { 0 };
float ballSpeed = .3f;

static Vector3 operator*(Vector3 lhs, float rhs) {
    lhs.x *= rhs;
    lhs.y *= rhs;
    lhs.z *= rhs;
    return lhs;
}

static Vector3 operator+(Vector3 lhs, Vector3  rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

static Vector3 operator-(Vector3 lhs, Vector3  rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
}

static Vector3 & operator+=(Vector3 & lhs, Vector3 rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

bool equalish(Vector3 lhs, Vector3 rhs) {
    float dx = lhs.x - rhs.x;
    float dy = lhs.y - rhs.y;
    float dz = lhs.z - rhs.z;
    if(dx < 0) dx = -dx;
    if(dy < 0) dy = -dy;
    if(dz < 0) dz = -dz;

    return dx < 0.001f && dy < 0.001f && dz < 0.001f;
}

ROSE_EXPORT void postload() {
    // Define the camera to look into our 3d world
    camera.position = { 0.0f, 10.0f, 30.0f }; // Camera position
    camera.target = { 0.0f, 10.0f, 0.0f };      // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                   // Camera mode type
    SetCameraMode(camera, CAMERA_FREE); // Set a free camera mode

    rose::io::json::read<World>(world, rose::io::Folder::Working, "game_state.json");
}

ROSE_EXPORT void predestroy() {
    rose::io::json::write(world, rose::io::Folder::Working, "game_state.json");
}

void update() {
    world.cubePosition.x += rose::input::stick().x;
    world.ballPosition += world.ballVelocity * ballSpeed;

    if(world.cubePosition.x < -11.5) world.cubePosition.x = -11.5;
    if(world.cubePosition.x > 11.5) world.cubePosition.x = 11.5;

    if(world.ballPosition.x < -13) world.ballVelocity.x *= -1;
    if(world.ballPosition.x > 13) world.ballVelocity.x *= -1;
    if(world.ballPosition.y > 20) world.ballVelocity.y *= -1;
    
    if(world.ballPosition.y < 0) {
        world.points -= 1;

        world.ballPosition = world.cubePosition + Vector3 {0.0f, 1.0f, 0.0f};
        auto rand = rose::hash_from_clock();
        float vx = rose::nextf(rand) * 2 - 1;
        world.ballVelocity = Vector3 {vx, 1.0, 0};
    }
    else if(world.ballPosition.y < 1 && world.ballVelocity.y < 0) {
        if(world.ballPosition.x > world.cubePosition.x - 2 && world.ballPosition.x < world.cubePosition.x + 2) {
            world.ballVelocity.y *= -1;
            Vector3 vec_dif = world.ballPosition - world.cubePosition;
            world.ballVelocity.x += vec_dif.x * .25f;
        }
    }

    if(ImGui::Button("New Stone")) {
        float x = -10;
        float y = 17;
        float xyi = 2;
        int i = 0;

        int rows_max = 5;
        int row = 0;

        Color colors[] = {RED,BLUE,YELLOW, ORANGE,GREEN};

        Stone stone { {x, y, 0} };
        for(;;) {
            if(i < world.stones.size()) {
                if(equalish(world.stones[i].position, stone.position)) {
                    i++;
                    stone.position.x += xyi;
                    if(stone.position.x > 10) {
                        stone.position.x = -10;
                        stone.position.y -= xyi;
                        row++;
                        if(row == rows_max) break;
                        stone.color = colors[row];
                    }
                    continue;
                }
            }
            break;
        }

        if(row != rows_max) {
            world.stones.push_back(stone);
        }
        std::sort(world.stones.begin(), world.stones.end(), [](auto & lhs, auto & rhs) {
            if(lhs.position.y == rhs.position.y) {
                return lhs.position.x < rhs.position.x;
            }
            return lhs.position.y > rhs.position.y;
        });
        
    }

    if(ImGui::Button("Clear Stone")) {
        world.stones.clear();
    }
    if(ImGui::Button("Safe Game")) {
        rose::io::json::write(world, rose::io::Folder::Working, "game_state.json");
    }
    /*
    int i = 0;
    for(auto & stone : world.stones) {
        ImGui::PushID(i++);
        ImGui::LabelText("Stone", "ID: %d, Pos: (%g, %g)", i, stone.position.x, stone.position.y);
        ImGui::PopID();
    }
    */

    ImGui::Separator();

    ImguiSerializer serializer;
    rose::ecs::serialize(world, serializer);
}

void DrawCubeWiresOutline(Vector3 position, float width, float height, float length, Color colorA, Color colorB)
{
    DrawCube(position, width,height, length, colorA);
    DrawCubeWires(position, width+.01f,height+.01f, length+.01f, colorB);
}

ROSE_EXPORT void draw() {
    update();

    BeginDrawing();
    {
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        {
            //Paddle
            DrawCubeWiresOutline(world.cubePosition, 3.0f, 1.0f, 1.0f, YELLOW, MAROON);

            //Ball
            DrawSphere(world.ballPosition, .5f, RED);

            //Stones
            for(auto & stone : world.stones) {
                DrawCubeWiresOutline(stone.position, 1.0f, 1.0f, 1.0f, stone.color, BLACK);
            }
            
            //Borders
            DrawCubeWiresOutline({-13.5f, .0f, .0f,}, 1.0f, 100.0f, 1.0f, GRAY, BLACK);
            DrawCubeWiresOutline({13.5f, .0f, .0f,}, 1.0f, 100.0f, 1.0f, GRAY, BLACK);
            DrawCubeWiresOutline({.0f, 20.0f, .0f,}, 100.0f, 1.0f, 1.0f, GRAY, BLACK);

            DrawGrid(10, 1.0f);
        }
        EndMode3D();

        DrawRectangle( 10, 10, 320, 133, Fade(SKYBLUE, 0.5f));
        DrawRectangleLines( 10, 10, 320, 133, BLUE);

        DrawText("Free camera default controls:", 20, 20, 10, BLACK);
        DrawText("- Mouse Wheel to Zoom in-out", 40, 40, 10, DARKGRAY);
        DrawText("- Mouse Wheel Pressed to Pan", 40, 60, 10, DARKGRAY);
        DrawText("- Alt + Mouse Wheel Pressed to Rotate", 40, 80, 10, DARKGRAY);
        DrawText("- Alt + Ctrl + Mouse Wheel Pressed for Smooth Zoom", 40, 100, 10, DARKGRAY);
        DrawText("- Z to zoom to (0, 0, 0)", 40, 120, 10, DARKGRAY);
    }
    EndDrawing();

}

ROSE_EXPORT void event(const rose::Event & ev) {
}

ROSE_EXPORT void ui() {
}

#include <rose/engine.hpp>